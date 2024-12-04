#include <gkg-dmri-plugin-functors/DwiMultipleSubjectTrackDensityImaging/DwiMultipleSubjectTrackDensityImagingCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>


//
// class SuperResolutionTraits< T >
//

template < class T >
struct SuperResolutionTraits
{

  typedef float accumulator_type;

  static void initialize( accumulator_type& value );

  static void accumulate( const T& value,
                          accumulator_type& destination );
  static void accumulateAccumulatedMaps( const accumulator_type& value,
                          accumulator_type& destination );
  static void average( const accumulator_type& accumulatedValue,
                       float density,
                       T& destination );

};


template <>
struct SuperResolutionTraits< gkg::RGBComponent >
{

  typedef gkg::Vector3d< float > accumulator_type;

  static void initialize( accumulator_type& value );

  static void accumulate( const gkg::RGBComponent& value,
                          accumulator_type& destination );
  static void accumulateAccumulatedMaps( const accumulator_type& value,
                          accumulator_type& destination );
  static void average( const accumulator_type& accumulatedValue,
                       float density,
                       gkg::RGBComponent& destination );
};

template < class T >
void SuperResolutionTraits< T >::initialize(
            typename SuperResolutionTraits< T >::accumulator_type& value )
{

  value = 0.0;

}

template < class T >
void SuperResolutionTraits< T >::accumulate(
            const T& value,
            typename SuperResolutionTraits< T >::accumulator_type& destination )
{

  destination += ( float )value;

}

template < class T >
void SuperResolutionTraits< T >::accumulateAccumulatedMaps(
            const typename SuperResolutionTraits< T >::accumulator_type& value,
            typename SuperResolutionTraits< T >::accumulator_type& destination )
{

  destination += value;

}


template < class T >
void SuperResolutionTraits< T >::average(
  const typename SuperResolutionTraits< T >::accumulator_type& accumulatedValue,
  float density,
  T& destination )
{

  destination = ( T )( accumulatedValue / density );

}


void SuperResolutionTraits< gkg::RGBComponent >::initialize(
           SuperResolutionTraits< gkg::RGBComponent >::accumulator_type& value )
{

  value.x = 0.0;
  value.y = 0.0;
  value.z = 0.0;

}

void SuperResolutionTraits< gkg::RGBComponent >::accumulate(
               const gkg::RGBComponent& value,
               SuperResolutionTraits< gkg::RGBComponent
                                              >::accumulator_type& destination )
{


  destination.x += ( float )value.r;
  destination.y += ( float )value.g;
  destination.z += ( float )value.b;


}

void SuperResolutionTraits< gkg::RGBComponent >::accumulateAccumulatedMaps(
               const SuperResolutionTraits< gkg::RGBComponent
                                              >::accumulator_type& value,
               SuperResolutionTraits< gkg::RGBComponent
                                              >::accumulator_type& destination )
{

  destination.x += ( float )value.x;
  destination.y += ( float )value.y;
  destination.z += ( float )value.z;

}


void SuperResolutionTraits< gkg::RGBComponent >::average(
  const SuperResolutionTraits< gkg::RGBComponent
                                          >::accumulator_type& accumulatedValue,
  float density,
  gkg::RGBComponent& destination )
{


  destination.r = ( int32_t )( accumulatedValue.x / density );
  destination.g = ( int32_t )( accumulatedValue.y / density );
  destination.b = ( int32_t )( accumulatedValue.z / density );


}


//
// class SuperResolutionContext< T >
//

namespace gkg
{

  template < class L,  class T >
  class SuperResolutionContext : public LoopContext< int32_t >
  {

    public:

      SuperResolutionContext( 
           const gkg::Volume< T >& inputVolume,
           const gkg::RCPointer< gkg::BundleMap< L,
                                                  std::less< L > > >& bundleMap,
           std::vector< std::map< gkg::Vector3d< int32_t >,
           std::pair< typename SuperResolutionTraits< T >::accumulator_type,
                                                            int32_t >,
                      Vector3dCompare< int32_t > > >& maps,
           const gkg::Vector3d< double >& inputResolution,
           const gkg::Vector3d< double >& outputResolution,
//           const gkg::DtkTransform3d< float >& transform3dForBundleMap,
           const gkg::QuickResampler< T >* quickResampler,
           const gkg::BoundingBox< int32_t >& boundingBox,
           const float& step,
           const int32_t& threadCount,
           const bool& verbose );
			   
      void doIt( int32_t startIndex, int32_t count );

    private:

      const gkg::Volume< T >& _inputVolume;
      const gkg::RCPointer< gkg::BundleMap< L, std::less< L > > >& _bundleMap;
      std::vector< std::map< gkg::Vector3d< int32_t >,
               std::pair< typename SuperResolutionTraits< T >::accumulator_type,
                                                           int32_t >,
               Vector3dCompare< int32_t > > >& _maps;
      const gkg::Vector3d< double >& _inputResolution;
      const gkg::Vector3d< double >& _outputResolution;
//      const gkg::DtkTransform3d< float >& _transform3dForBundleMap;
      const gkg::QuickResampler< T >* _quickResampler;
      const gkg::BoundingBox< int32_t >& _boundingBox;
      const float& _step;
      const int32_t& _threadCount;
      const bool& _verbose;

      std::vector< L > _bundleIterator;
      std::vector< int32_t > _fiberIterator;

  };

}

template < class L, class T >
gkg::SuperResolutionContext< L, T >::SuperResolutionContext(
           const gkg::Volume< T >& inputVolume,
           const gkg::RCPointer< gkg::BundleMap< L,
                                                  std::less< L > > >& bundleMap,
           std::vector< std::map< gkg::Vector3d< int32_t >,
           std::pair< typename SuperResolutionTraits< T >::accumulator_type,
                                                            int32_t >,
                   Vector3dCompare< int32_t > > >& maps,
           const gkg::Vector3d< double >& inputResolution,
           const gkg::Vector3d< double >& outputResolution,
//           const gkg::DtkTransform3d< float >& transform3dForBundleMap,
           const gkg::QuickResampler< T >* quickResampler,
           const gkg::BoundingBox< int32_t >& boundingBox,
           const float& step,
           const int32_t& threadCount,
           const bool& verbose ): gkg::LoopContext< int32_t >(),
                            _inputVolume( inputVolume ),
                            _bundleMap( bundleMap ),
                            _maps( maps ),
                            _inputResolution( inputResolution ),
                            _outputResolution( outputResolution ),
//                            _transform3dForBundleMap( transform3dForBundleMap ),
                            _quickResampler( quickResampler ),
                            _boundingBox( boundingBox ),
                            _step( step ),
                            _threadCount( threadCount ),
                            _verbose( verbose )
{

  double curveCounts;
  _bundleMap->getHeader().getAttribute( "curves_count", curveCounts );
  int32_t fiberCount = ( int32_t ) curveCounts / threadCount;
  int32_t i = 0;
  int32_t bundleIndex = 0;
  int32_t fiberIndex = 0;
  int32_t fiberAccumulator = 0;

  typename gkg::BundleMap< L >::const_iterator b = bundleMap->begin(),
                                               be = bundleMap->end();
   _bundleIterator.push_back( b->first );
   _fiberIterator.push_back( fiberIndex );

  while ( b != be )
  {

    if( ( ( b->second ).getCurve3dCount() - fiberIndex ) < 
                                             ( fiberCount - fiberAccumulator ) )
    {

      ++bundleIndex;
      ++b; 
      fiberAccumulator += ( ( b->second ).getCurve3dCount() - fiberIndex );
      fiberIndex = 0;

    }
    else
    {

      fiberIndex =  fiberIndex + fiberCount - fiberAccumulator;
      ++i;
      fiberAccumulator = 0;
      _bundleIterator.push_back( b->first );
      _fiberIterator.push_back( fiberIndex );

    }

  }

}


template < class L , class T >
void gkg::SuperResolutionContext< L, T >::doIt( int32_t startIndex, 
                                                int32_t /* count */ )
{

  try
  {

    T value;
    //SuperResolutionTraits< T >::initialize( value );
    int32_t bundleIndex = 0;
    bool lastBundle = false;

    typename gkg::BundleMap< L, std::less< L > >::iterator
                     b = _bundleMap->find( _bundleIterator[ startIndex ] ),
                     be = _bundleMap->find( _bundleIterator[ startIndex + 1 ] );

    while( lastBundle == false )
    {

      if( b == be )
      {

        lastBundle = true;

      }
      //////////////////////////////////////////////////////////////////////////
      //looping over bundle(s) and filling accumulator and density volume(s)
      //////////////////////////////////////////////////////////////////////////

      gkg::Vector3d< float > siteResampledVolumeFrame;
      gkg::Vector3d< int32_t > voxelInOutputVolumeFrame;

      typename gkg::BundleMap< L >::Bundle& bundle = b->second;

       int32_t f = 0;
       int32_t fe = bundle.getCurve3dCount();       

       if( bundleIndex == 0 )
       {

         f = _fiberIterator[ startIndex ];

       }
       if( lastBundle && startIndex != ( _threadCount - 1 ) )
       {

         fe = _fiberIterator[ startIndex + 1 ];

       }
      int32_t fiberIndex = 0;
      for ( fiberIndex = f; fiberIndex < fe; ++fiberIndex )
      {

        gkg::LightCurve3d< float > resampledFiber = bundle[ fiberIndex ];
        float length = resampledFiber.getLength();
        if ( length > 0 )
        {

          int32_t oldPointCount = resampledFiber.getPointCount();
          int32_t newPointCount = ( int32_t )( length / _step + 0.5 );

          if( newPointCount > oldPointCount )
          {

            resampledFiber = 
                            resampledFiber.getEquidistantCurve( newPointCount );

          }

        }
        gkg::LightCurve3d< float >::const_iterator s = resampledFiber.begin(),
                                                   se = resampledFiber.end();
        while ( s != se )
        {

          gkg::Vector3d< float > siteResampledVolumeFrame;

          // _transform3dForBundleMap.getDirect( *s, siteResampledVolumeFrame );
          siteResampledVolumeFrame = *s;
          voxelInOutputVolumeFrame.x = ( int32_t )( siteResampledVolumeFrame.x /
                                                    _outputResolution.x + 0.5 );
          voxelInOutputVolumeFrame.y = ( int32_t )( siteResampledVolumeFrame.y /
                                                    _outputResolution.y + 0.5 );
          voxelInOutputVolumeFrame.z = ( int32_t )( siteResampledVolumeFrame.z /
                                                    _outputResolution.z + 0.5 );

          if ( _boundingBox.contains( voxelInOutputVolumeFrame ) )
          {

            _quickResampler->resample( _inputVolume,
                                       T( 0 ),
                                       siteResampledVolumeFrame,
                                       value,
                                       &_inputResolution,
                                       0 );
            if( _maps[ startIndex ].count( voxelInOutputVolumeFrame ) == 0 )
            {

              SuperResolutionTraits< T >::initialize( 
                        _maps[ startIndex ][ voxelInOutputVolumeFrame ].first );
               SuperResolutionTraits< T >::accumulate( value,
                        _maps[ startIndex ][ voxelInOutputVolumeFrame ].first );
              _maps[ startIndex ][ voxelInOutputVolumeFrame ].second = 1;


            }
            else
            {
 

               SuperResolutionTraits< T >::accumulate( value,
                        _maps[ startIndex ][ voxelInOutputVolumeFrame ].first );
               ++_maps[ startIndex ][ voxelInOutputVolumeFrame ].second;

            }

          }
          ++ s;

        }

      } 
      ++ b;
      ++ bundleIndex;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::VolumeThresholderContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}
//
// class SuperResolutionProcess
//

class SuperResolutionProcess : public gkg::Process
{

  public:

    SuperResolutionProcess(
          const std::vector< std::string >& theFileNameBundleMapDirectories,
          const gkg::Vector3d< double >& theOutputResolution,
          const std::vector< std::string >& theFileNameInputVolumes,
          const std::string& fileNameTarget,
          const std::vector< std::string >& fileNameReferences,
          const std::vector< std::string >& fileNameDeformations,
          const std::vector< std::string >& theFileNameTransform3dForBundleMaps,
          const std::vector< std::string >& theFileNameTransform3dForVolume,
          const std::string& theFileNameOutputVolume,
          const float& theStep,
          const int32_t& theThreadCount,
          const std::string& theOutputFormat,
          const bool& theAscii,
          const bool& theVerbose );

    const std::vector< std::string >& fileNameBundleMapDirectories;
    const gkg::Vector3d< double >& outputResolution;
    const std::vector< std::string >& fileNameInputVolumes;
    const std::string& fileNameTarget;
    const std::vector< std::string >& fileNameReferences;
    const std::vector< std::string >& fileNameDeformations;
    const std::vector< std::string >& fileNameTransform3dForBundleMaps;
    const std::vector< std::string >& fileNameTransform3dForVolume;
    const std::string& fileNameOutputVolume;
    const float& step;
    const int32_t& threadCount;
    const std::string& outputFormat;
    const bool& ascii;
    const bool& verbose;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


//
// class SubSuperResolutionProcess
//

template < class L >
class SubSuperResolutionProcess : public gkg::Process
{

  public:

    SubSuperResolutionProcess(
                      const SuperResolutionProcess& theSuperResolutionProcess );

    const SuperResolutionProcess& superResolutionProcess;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameInputVolume,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


//
// members of class SubSuperResolutionProcess
//

template < class L >
SubSuperResolutionProcess< L >::SubSuperResolutionProcess(
                       const SuperResolutionProcess& theSuperResolutionProcess )
                       : gkg::Process( "Volume" ),
                         superResolutionProcess( theSuperResolutionProcess )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &SubSuperResolutionProcess::operate< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &SubSuperResolutionProcess::operate< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &SubSuperResolutionProcess::operate< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &SubSuperResolutionProcess::operate< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &SubSuperResolutionProcess::operate< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &SubSuperResolutionProcess::operate< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &SubSuperResolutionProcess::operate< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &SubSuperResolutionProcess::operate< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &SubSuperResolutionProcess::operate< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &SubSuperResolutionProcess::operate< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &SubSuperResolutionProcess::operate< gkg::RGBComponent > );

}


template < class L > template < class T >
void 
SubSuperResolutionProcess< L >::operate(
                                   gkg::Process& process,
                                   const std::string& /* fileNameInputVolume */,
                                   const gkg::AnalyzedObject&,
                                   const std::string& )
{

  try
  {

    SubSuperResolutionProcess&
      subSuperResolutionProcess =
        static_cast< SubSuperResolutionProcess& >( process );

    const SuperResolutionProcess&
      superResolutionProcess = subSuperResolutionProcess.superResolutionProcess;

    //////////////////////////////////////////////////////////////////////////
    // reading input volume for sanity check
    //////////////////////////////////////////////////////////////////////////

    if ( superResolutionProcess.verbose )
    {

      std::cout << "reading '"
                << superResolutionProcess.fileNameInputVolumes[ 0 ]
                << "' : " << std::flush;

    }

    gkg::Volume< T > sanityCheckInputVolume;
    std::string format = gkg::Reader::getInstance().read(
                               superResolutionProcess.fileNameInputVolumes[ 0 ],
                               sanityCheckInputVolume );
    gkg::Vector3d< int32_t > sanityCheckInputSize;
    int32_t sanityCheckInputSizeT = 1;
    sanityCheckInputVolume.getSize( sanityCheckInputSize,
                                    sanityCheckInputSizeT );

    gkg::Vector3d< double > sanityCheckInputResolution;
    double sanityCheckInputResolutionT = 1.0;
    sanityCheckInputVolume.getResolution( sanityCheckInputResolution,
                                          sanityCheckInputResolutionT );
    std::cout << "input volume size : "
              << sanityCheckInputSize.x << " x "
              << sanityCheckInputSize.y << " x "
              << sanityCheckInputSize.z << " x "
              << sanityCheckInputSizeT << std::endl;
    std::cout << "input volume resolution : "
              << sanityCheckInputResolution.x << "mm x "
              << sanityCheckInputResolution.y << "mm x "
              << sanityCheckInputResolution.z << "mm x "
              << sanityCheckInputResolutionT << std::endl;

    if ( superResolutionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating ouput volume
    ////////////////////////////////////////////////////////////////////////////

    if ( superResolutionProcess.verbose )
    {

      std::cout << "creating output volume : " << std::flush;

    }

    gkg::Vector3d< int32_t >
      outputSize( ( int32_t )( ( sanityCheckInputSize.x * 
                              sanityCheckInputResolution.x ) /
                              superResolutionProcess.outputResolution.x + 0.5 ),
                  ( int32_t )( ( sanityCheckInputSize.y * 
                              sanityCheckInputResolution.y ) /
                              superResolutionProcess.outputResolution.y + 0.5 ),
                  ( int32_t )( ( sanityCheckInputSize.z * 
                              sanityCheckInputResolution.z ) /
                              superResolutionProcess.outputResolution.z + 0.5 )
                 );
    int32_t outputSizeT = sanityCheckInputSizeT;
    double outputResolutionT = sanityCheckInputResolutionT;
    gkg::BoundingBox< int32_t > boundingBox( 0, outputSize.x - 1,
                                             0, outputSize.y - 1,
                                             0, outputSize.z - 1,
                                             0, 1 );

    if ( superResolutionProcess.verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "output volume size : "
                << outputSize.x << " x "
                << outputSize.y << " x "
                << outputSize.z << " x "
                << outputSizeT << std::endl;
      std::cout << "output volume resolution : "
                << superResolutionProcess.outputResolution.x << "mm x "
                << superResolutionProcess.outputResolution.y << "mm x "
                << superResolutionProcess.outputResolution.z << "mm x "
                << outputResolutionT << std::endl;

    }

    gkg::QuickResampler< T >* quickResampler = 
              gkg::QuickResamplerFactory< T >::getInstance().getQuickResampler(
                                            "Partial Voluming QuickResampler" );

    ////////////////////////////////////////////////////////////////////////////
    // creating accumulator maps
    ////////////////////////////////////////////////////////////////////////////

    if ( superResolutionProcess.verbose )
    {

      std::cout << "creating volume accumulator and density mask : "
                << std::flush;

    }

  
    std::vector< std::map< gkg::Vector3d< int32_t >,
           std::pair< typename SuperResolutionTraits< T >::accumulator_type,
                       int32_t >,
            gkg::Vector3dCompare< int32_t > > > maps( superResolutionProcess.threadCount );
    if ( superResolutionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating output volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< T > outputVolume( outputSize, outputSizeT );

    outputVolume.setResolution( superResolutionProcess.outputResolution,
                                outputResolutionT );
    outputVolume.fill( T( 0 ) );

/*
    ////////////////////////////////////////////////////////////////////////////
    // initializing bundle resampling data
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > deformationSize, referenceSize, targetSize;
    gkg::Vector3d< double > referenceResolution, targetResolution;
    int32_t deformationSizeT = 1;

    gkg::Vector3d< double > deformationResolution;

    gkg::Volume< T > volumeReference;
    gkg::Volume< float >volumeDeformation;

    gkg::Vector3d< float > targetOffset( 0.0, 0.0, 0.0 );
    gkg::Vector3d< float > targetFullSize;

    if( superResolutionProcess.fileNameTarget != "" )
    {

      gkg::Volume< float > volumeTarget;

      format = gkg::Reader::getInstance().read( 
                          superResolutionProcess.fileNameTarget, volumeTarget );
      volumeTarget.getResolution( targetResolution );

      volumeTarget.getSize( targetSize );
      targetFullSize.x = targetResolution.x * targetSize.x;
      targetFullSize.y = targetResolution.y * targetSize.y;
      targetFullSize.z = targetResolution.z * targetSize.z;
      if ( volumeTarget.getHeader().hasAttribute( "qoffset_x" ) )
      {

        volumeTarget.getHeader().getAttribute( "qoffset_x", targetOffset.x );

      }
      if ( volumeTarget.getHeader().hasAttribute( "qoffset_y" ) )
      {

        volumeTarget.getHeader().getAttribute( "qoffset_y", targetOffset.y );

      }
      if ( volumeTarget.getHeader().hasAttribute( "qoffset_z" ) )
      {

        volumeTarget.getHeader().getAttribute( "qoffset_z", targetOffset.z );

      }

      if ( superResolutionProcess.verbose )
      {

        std::cout << "Target resolution: "
                  << targetResolution.x << " "
                  << targetResolution.y << " "
                  << targetResolution.z << " " << std::endl << std::flush;
        std::cout << "Target size: "
                  << targetSize.x << " "
                  << targetSize.y << " "
                  << targetSize.z << " " << std::endl << std::flush;
        std::cout << "Target offset: "
                  << targetOffset.x << " "
                  << targetOffset.y << " "
                  << targetOffset.z << " " << std::endl << std::flush;

      }

    }
*/
    std::vector< std::string >::const_iterator
      in = superResolutionProcess.fileNameInputVolumes.begin(),
      ine = superResolutionProcess.fileNameInputVolumes.end();
    int32_t subjectIndex = 0;
    int32_t subjectCount = superResolutionProcess.fileNameInputVolumes.size();
    gkg::Volume< T > inputVolume;
    // gkg::RCPointer< gkg::Transform3d< float > > transform3dForVolume;
    ////////////////////////////////////////////////////////////////////////////
    // looping over subjects
    ////////////////////////////////////////////////////////////////////////////
    while ( in != ine )
    {

      if ( superResolutionProcess.verbose )
      {

        if ( subjectIndex )
        {

          std::cout << gkg::Eraser( 22 );

        }
        std::cout << " Subject [ " << std::setw( 3 ) << subjectIndex + 1 
                  << " / " << std::setw( 3 ) << subjectCount
                  << " ] " << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading or creating 3D transform (inputVolume -> common space)
      //////////////////////////////////////////////////////////////////////////
/*

      if ( !superResolutionProcess.fileNameTransform3dForVolume.empty() )
      {

        if ( superResolutionProcess.verbose )
        {

         std::cout << "reading '"
                   << superResolutionProcess.fileNameTransform3dForVolume[ 
                                                                  subjectIndex ]
                   << "' : " << std::flush;

        }
        try
        {

          gkg::RigidTransform3d< float >*
            rigidTransform3d = new gkg::RigidTransform3d< float >;
          std::ifstream 
            is( superResolutionProcess.fileNameTransform3dForVolume[ 
                                                       subjectIndex ].c_str() );
          rigidTransform3d->readTrm( is );
          is.close();
          transform3dForVolume.reset( rigidTransform3d );

        }
        catch ( std::exception& )
        {

          try
          {

            gkg::AffineWoShearingTransform3d< float >*
              affineWoShearingTransform3d =
                new gkg::AffineWoShearingTransform3d< float >;
            std::ifstream 
              is( superResolutionProcess.fileNameTransform3dForVolume[ 
                                                       subjectIndex ].c_str() );
            affineWoShearingTransform3d->readTrm( is );
            is.close();
            transform3dForVolume.reset( affineWoShearingTransform3d );

          }
          catch ( std::exception& )
          {

            gkg::AffineTransform3d< float >*
              affineTransform3d =
                new gkg::AffineTransform3d< float >;
            std::ifstream 
              is( superResolutionProcess.fileNameTransform3dForVolume[ 
                                                       subjectIndex ].c_str() );
            affineTransform3d->readTrm( is );
            is.close();
            transform3dForVolume.reset( affineTransform3d );

          }

        }

        if ( superResolutionProcess.verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        if ( superResolutionProcess.verbose )
        {

         std::cout << "creating identity 3D transform  : " << std::flush;

        }

        transform3dForVolume.reset( new gkg::IdentityTransform3d< float > );

        if ( superResolutionProcess.verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
*/
      //////////////////////////////////////////////////////////////////////////
      // creating resampler
      //////////////////////////////////////////////////////////////////////////
      gkg::Resampler< T >* resampler =
                    gkg::ResamplerFactory< T >::getInstance().getResampler( 3 );

      if ( superResolutionProcess.verbose )
      {

        std::cout << "switching to resampler : " << resampler->getName()
                  << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading input volume
      //////////////////////////////////////////////////////////////////////////
      format = gkg::Reader::getInstance().read( *in, inputVolume );

      gkg::Vector3d< double > inputResolution;
      double inputResolutionT = 1.0;
      inputVolume.getResolution( inputResolution, inputResolutionT );

/*
      resampler->resample( inputVolume,
                           *transform3dForVolume,
                           T( 0 ),
                           inputVolume,
                           superResolutionProcess.verbose );
*/


      //////////////////////////////////////////////////////////////////////////
      // creating bundle resampler
      //////////////////////////////////////////////////////////////////////////
/*
      gkg::DtkTransform3d< float > dtkTransform3d;

      if( superResolutionProcess.fileNameReferences.size() != 0 )
      {

        format = gkg::Reader::getInstance().read( 
                      superResolutionProcess.fileNameReferences[ subjectIndex ],
                                                              volumeReference );

        if( superResolutionProcess.fileNameDeformations.size() != 0 )
        {
          format = gkg::Reader::getInstance().read( 
                    superResolutionProcess.fileNameDeformations[ subjectIndex ],
                                                            volumeDeformation );
          volumeDeformation.getSize( deformationSize, deformationSizeT );

          if ( superResolutionProcess.verbose )
          {

            std::cout << "volumeDeformation resolution: "
                      << deformationSize.x << " "
                      << deformationSize.y << " "
                      << deformationSize.z << " " 
                      << deformationSizeT<< std::endl << std::flush;

         }

       }

       /////////////////////////////////////////////////////////////////////////
       //get ref and target sizes
       /////////////////////////////////////////////////////////////////////////
        volumeReference.getResolution( referenceResolution );

        volumeReference.getSize( referenceSize );
        gkg::Vector3d< float > referenceFullSize( 
                                      referenceResolution.x * referenceSize.x,
                                      referenceResolution.y * referenceSize.y,
                                      referenceResolution.z * referenceSize.z );
        gkg::Vector3d< float > referenceOffset( 0.0, 0.0, 0.0 );
        if ( volumeReference.getHeader().hasAttribute( "qoffset_x" ) )
        {

          volumeReference.getHeader().getAttribute( "qoffset_x",
                                                    referenceOffset.x );

        }
        if ( volumeReference.getHeader().hasAttribute( "qoffset_y" ) )
        {

          volumeReference.getHeader().getAttribute( "qoffset_y",
                                                    referenceOffset.y );

        }
        if ( volumeReference.getHeader().hasAttribute( "qoffset_z" ) )
        {

          volumeReference.getHeader().getAttribute( "qoffset_z",
                                                    referenceOffset.z );

        }

        if ( superResolutionProcess.verbose )
        {

          std::cout << "Reference resolution: "
                    << referenceResolution.x << " "
                    << referenceResolution.y << " "
                    << referenceResolution.z << " " << std::endl << std::flush;
          std::cout << "Reference size: "
                    << referenceSize.x << " "
                    << referenceSize.y << " "
                    << referenceSize.z << " " << std::endl << std::flush;
          std::cout << "Reference offset: "
                    << referenceOffset.x << " "
                    << referenceOffset.y << " "
                    << referenceOffset.z << " " << std::endl << std::flush;

        }

        if( superResolutionProcess.fileNameDeformations.size() != 0 )
        {

          volumeDeformation.getResolution( deformationResolution );
          if( superResolutionProcess.verbose )
          {

            std::cout << "Deformation field resolution: "
                      << deformationResolution.x << " "
                      << deformationResolution.y << " "
                      << deformationResolution.z << " " 
                      << std::endl << std::flush;

          }

        }

        gkg::Vector3d< double > targetReferenceResolutionDelta(
                           ( targetResolution.x - referenceResolution.x ) / 2,
                           ( targetResolution.y - referenceResolution.y ) / 2,
                           ( targetResolution.z - referenceResolution.z ) / 2 );

        gkg::Vector3d< float > offsetDelta( 0.0, 0.0, 0.0 );
        offsetDelta.x = -referenceOffset.x - ( -targetOffset.x ) + 
                                               targetReferenceResolutionDelta.x;
        offsetDelta.y = -referenceOffset.y - ( -targetOffset.y ) - 
                                               targetReferenceResolutionDelta.y;
        offsetDelta.z = -referenceOffset.z - ( -targetOffset.z ) - 
                                               targetReferenceResolutionDelta.z;


        float step = std::min( std::min( deformationResolution.x,
                                         deformationResolution.y ),
                                         deformationResolution.z );
       if( superResolutionProcess.verbose )
        {

          std::cout << "Minimum step between fiber points: " << step
                  << std::endl << std::flush;

        }

        ////////////////////////////////////////////////////////////////////////
        //read transformation matrix
        ////////////////////////////////////////////////////////////////////////
        std::ifstream is( 
                        superResolutionProcess.fileNameTransform3dForBundleMaps[
                                                       subjectIndex ].c_str() );
        if( is.fail() )
        {

          throw std::runtime_error( "Error while opening " + 
                        superResolutionProcess.fileNameTransform3dForBundleMaps[ 
                                                               subjectIndex ] );

        }

        dtkTransform3d.setOffsetParameters( volumeDeformation,
                                            referenceFullSize,
                                            targetFullSize,
                                            offsetDelta );
        dtkTransform3d.readTrm( is );

        if( superResolutionProcess.verbose )
        {

          std::cout << "Direct Translation vector: " << std::endl << std::flush;
          std::cout << dtkTransform3d.getDirectCoefficient( 0, 3 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 1, 3 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 2, 3 ) << 
                       std::endl << std::flush;

          std::cout << "Direct Rotation matrix: " << std::endl << std::flush;
          std::cout << dtkTransform3d.getDirectCoefficient( 0, 0 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 0, 1 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 0, 2 ) << 
                       std::endl << std::flush;
          std::cout << dtkTransform3d.getDirectCoefficient( 1, 0 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 1, 1 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 1, 2 ) <<
                       std::endl << std::flush;
          std::cout << dtkTransform3d.getDirectCoefficient( 2, 0 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 2, 1 ) << " " <<
                       dtkTransform3d.getDirectCoefficient( 2, 2 ) <<
                       std::endl << std::flush;

          std::cout << "Applied Rotation matrix: " << std::endl << std::flush;
          std::cout << dtkTransform3d.getInverseCoefficient( 0, 0 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 0, 1 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 0, 2 ) << 
                       std::endl << std::flush;
          std::cout << dtkTransform3d.getInverseCoefficient( 1, 0 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 1, 1 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 1, 2 ) <<
                       std::endl << std::flush;
          std::cout << dtkTransform3d.getInverseCoefficient( 2, 0 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 2, 1 ) << " " <<
                       dtkTransform3d.getInverseCoefficient( 2, 2 ) <<
                       std::endl << std::flush;

        }

      }
*/
      //////////////////////////////////////////////////////////////////////////
      // looping over bundle map(s)
      //////////////////////////////////////////////////////////////////////////
   
      if ( superResolutionProcess.verbose )
      {

        std::cout << "looping over bundle map(s) : " << std::flush;

      }
      gkg::Directory 
        directory( superResolutionProcess.fileNameBundleMapDirectories[ 
                                                               subjectIndex ] );

      if ( !directory.isValid() )
      {

        throw std::runtime_error( std::string( "invalid directory '" ) +
               superResolutionProcess.fileNameBundleMapDirectories[ 
                                                         subjectIndex ] + "'" );

      }

      std::vector< std::string > fileNameBundleMaps;
      std::set< std::string > fileNames = directory.getFiles();
      std::set< std::string >::const_iterator
        f = fileNames.begin(),
        fe = fileNames.end();
      while ( f != fe )
      {

        std::string fileName = *f;
        std::string::size_type found = fileName.find( "." );
        if ( found <= fileName.size() )
        {

          std::string extension = fileName.substr( found );
          if ( extension == ".bundles" || extension == ".bundlemap" )
          {

            std::string fullPath = 
              superResolutionProcess.fileNameBundleMapDirectories[ 
                       subjectIndex ] + gkg::getDirectorySeparator() + fileName;
            fileNameBundleMaps.push_back( fullPath );

          }

        }

      }

      int32_t i = 0;
      for( i = 0; i < ( int32_t )fileNameBundleMaps.size(); ++i )
      {

        ////////////////////////////////////////////////////////////////////////
        // reading bundle map
        ////////////////////////////////////////////////////////////////////////
        gkg::RCPointer< gkg::BundleMap< L, std::less< L > > > 
                           bundleMap( new gkg::BundleMap< L, std::less< L > > );
        gkg::Reader::getInstance().read( fileNameBundleMaps[ i ], *bundleMap );

        if ( superResolutionProcess.verbose )
        {

          std::cout << "Bundle map: " << fileNameBundleMaps[ i ]
                                      << std::endl << std::flush;

        }
        double curveCounts;
        bundleMap->getHeader().getAttribute( "curves_count", curveCounts );
        int32_t threadCount = superResolutionProcess.threadCount;
        if( curveCounts < threadCount )
        {

          threadCount = curveCounts;

        }

        if( curveCounts != 0.0 )
        {


          gkg::SuperResolutionContext< L, T > context( 
                                        inputVolume,
                                        bundleMap,
                                        maps,
                                        inputResolution,
                                        superResolutionProcess.outputResolution,
//                                        dtkTransform3d,
                                        quickResampler,
                                        boundingBox,
                                        superResolutionProcess.step,
                                        threadCount,
                                        superResolutionProcess.verbose );

          gkg::ThreadedLoop< int32_t >
            threadedLoop( &context,
                          0,
                          superResolutionProcess.threadCount );

          threadedLoop.launch();

        }
        else
        {

          if ( superResolutionProcess.verbose )
          {

            std::cout << "Bundle map: " << fileNameBundleMaps[ i ]
                      << " is empty, will be ignored."
                      << std::endl << std::flush;

          }

        }

        
      }
      ++ in;
      ++ subjectIndex;

    }

    //////////////////////////////////////////////////////////////////////////
    // filling ouput volume
    //////////////////////////////////////////////////////////////////////////

    typename SuperResolutionTraits< T >::accumulator_type value;

    SuperResolutionTraits< T >::initialize( value );

    int32_t density = 0;
    gkg::Vector3d< int32_t > voxel( 0, 0, 0 );
    std::pair< typename SuperResolutionTraits< T >::accumulator_type,
               int32_t > pair;
    uint32_t mapIndex = 0;
    int fiberCount = 0;

    for( mapIndex = 0; mapIndex < maps.size(); ++mapIndex )
    {

      typename std::map< gkg::Vector3d< int32_t >,
               std::pair< typename SuperResolutionTraits< T >::accumulator_type,
                                                   int32_t > >::iterator 
                                                   m = maps[ mapIndex ].begin(),
                                                   me = maps[ mapIndex ].end(),
                                                   it;

      while( m != me )
      {

        voxel = m->first;

        SuperResolutionTraits< T >::initialize( value );
        SuperResolutionTraits< T >::accumulateAccumulatedMaps( 
                                                   ( m->second ).first, value );
        density = ( m->second ).second;
        fiberCount += density;


        maps[ mapIndex ].erase( m );
        uint32_t i = 0;
        for( i = mapIndex + 1; i < maps.size(); ++i )
        {

          it = maps[ i ].find( voxel );
          if( it != maps[ i ].end() )
          {

            pair = it->second;
            SuperResolutionTraits< T >::accumulateAccumulatedMaps( pair.first,
                                                                   value );
            density += pair.second;
            maps[ i ].erase( it );
            fiberCount += density;

          }      

        }

        SuperResolutionTraits< T >::average(
                                    value,
                                    density,
                                    outputVolume( voxel.x, voxel.y, voxel.z ) );
        ++m;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing output volume
    ////////////////////////////////////////////////////////////////////////////
    if ( superResolutionProcess.verbose )
    {

      std::cout << "writing '"
                << superResolutionProcess.fileNameOutputVolume
                << "' : " << std::flush;

    }

    if ( !superResolutionProcess.outputFormat.empty() )
    {

      gkg::Writer::getInstance().write(
                                    superResolutionProcess.fileNameOutputVolume,
                                    outputVolume,
                                    superResolutionProcess.ascii,
                                    superResolutionProcess.outputFormat );

    }
    else
    {

      gkg::Writer::getInstance().write(
                                    superResolutionProcess.fileNameOutputVolume,
                                    outputVolume,
                                    superResolutionProcess.ascii,
                                    format );

    }

    if ( superResolutionProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }



  }
  GKG_CATCH( "template < class L > template < class T > "
             "void "
             "SubSuperResolutionProcess< L >::operate( "
             "gkg::Process& process, "
 	     "const std::string& fileNameInputVolume, "
 	     "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// members of class SuperResolutionProcess
//

SuperResolutionProcess::SuperResolutionProcess(
          const std::vector< std::string >& theFileNameBundleMapDirectories,
          const gkg::Vector3d< double >& theOutputResolution,
          const std::vector< std::string >& theFileNameInputVolumes,
          const std::string& theFileNameTarget,
          const std::vector< std::string >& theFileNameReferences,
          const std::vector< std::string >& theFileNameDeformations,
          const std::vector< std::string >& theFileNameTransform3dForBundleMaps,
          const std::vector< std::string >& theFileNameTransform3dForVolume,
          const std::string& theFileNameOutputVolume,
          const float& theStep,
          const int32_t& theThreadCount,
          const std::string& theOutputFormat,
          const bool& theAscii,
          const bool& theVerbose )
        : gkg::Process( "BundleMap" ),
        fileNameBundleMapDirectories( theFileNameBundleMapDirectories ),
        outputResolution( theOutputResolution ),
        fileNameInputVolumes( theFileNameInputVolumes ),
        fileNameTarget( theFileNameTarget ),
        fileNameReferences( theFileNameReferences ),
        fileNameDeformations( theFileNameDeformations ),
        fileNameTransform3dForBundleMaps( theFileNameTransform3dForBundleMaps ),
        fileNameTransform3dForVolume( theFileNameTransform3dForVolume ),
        fileNameOutputVolume( theFileNameOutputVolume ),
        step( theStep ),
        threadCount( theThreadCount ),
        outputFormat( theOutputFormat ),
        ascii( theAscii ),
        verbose( theVerbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &SuperResolutionProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &SuperResolutionProcess::operate< int16_t > );

}


template < class L >
void SuperResolutionProcess::operate( gkg::Process& process,
 				      const std::string& /* fileNameIn */,
 				      const gkg::AnalyzedObject&,
 				      const std::string& )
{

  try
  {

    SuperResolutionProcess&
      superResolutionProcess =
        static_cast< SuperResolutionProcess& >( process );


    SubSuperResolutionProcess< L >
      subSuperResolutionProcess( superResolutionProcess );

    subSuperResolutionProcess.execute( 
                          superResolutionProcess.fileNameInputVolumes.front() );



  }
  GKG_CATCH( "template < class L > "
             "void SuperResolutionProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiMultipleSubjectTrackDensityImagingCommand
//

gkg::DwiMultipleSubjectTrackDensityImagingCommand::
                DwiMultipleSubjectTrackDensityImagingCommand( int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                                  : gkg::Command( argc, argv,
                                                                  loadPlugin,
                                                                  removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiMultipleSubjectTrackDensityImagingCommand::"
             "DwiMultipleSubjectTrackDensityImagingCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiMultipleSubjectTrackDensityImagingCommand::
                                   DwiMultipleSubjectTrackDensityImagingCommand(
             const std::vector< std::string >& fileNameBundleMapDirectories,
             const std::vector< double >& outputResolutionVector,
             const std::vector< std::string >& fileNameInputVolumes,
             const std::string& fileNameTarget,
             const std::vector< std::string >& fileNameReferences,
             const std::vector< std::string >& fileNameDeformations,
             const std::vector< std::string >& fileNameTransform3dForBundleMaps,
             const std::vector< std::string >& fileNameTransform3dForVolume,
             const std::string& fileNameOutputVolume,
             float step,
             int32_t threadCount,
             const std::string& outputFormat,
             bool ascii,
             bool verbose )
                                                  : gkg::Command()
{

  try
  {

    execute( fileNameBundleMapDirectories,
             outputResolutionVector,
             fileNameInputVolumes,
             fileNameTarget,
             fileNameReferences,
             fileNameDeformations,
             fileNameTransform3dForBundleMaps,
             fileNameTransform3dForVolume,
             fileNameOutputVolume,
             step,
             threadCount,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMultipleSubjectTrackDensityImagingCommand::"
             "DwiMultipleSubjectTrackDensityImagingCommand( "
             "const std::vector< std::string >& fileNameBundleMapDirectories, "
             "const std::vector< double >& outputResolutionVector, "
             "const std::vector< std::string >& fileNameInputVolumes, "
             "const std::string& fileNameTarget, "
             "const std::vector< std::string >& fileNameReferences, "
             "const std::vector< std::string >& fileNameDeformations, "
             "const std::vector< std::string >& "
             "fileNameTransform3dForBundleMaps, "
             "const std::vector< std::string >& fileNameTransform3dForVolume, "
             "const std::string& fileNameOutputVolume, "
             "float step, "
             "int32_t threadCount, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiMultipleSubjectTrackDensityImagingCommand::
                                   DwiMultipleSubjectTrackDensityImagingCommand(
                                             const gkg::Dictionary& parameters )
                                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameBundleMapDirectories );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           outputResolutionVector );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameInputVolumes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTarget );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameReferences );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameDeformations );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameTransform3dForBundleMaps );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameTransform3dForVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputVolume );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, threadCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMapDirectories,
             outputResolutionVector,
             fileNameInputVolumes,
             fileNameTarget,
             fileNameReferences,
             fileNameDeformations,
             fileNameTransform3dForBundleMaps,
             fileNameTransform3dForVolume,
             fileNameOutputVolume,
             step,
             threadCount,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMultipleSubjectTrackDensityImagingCommand::"
             "DwiMultipleSubjectTrackDensityImagingCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiMultipleSubjectTrackDensityImagingCommand::
                                 ~DwiMultipleSubjectTrackDensityImagingCommand()
{
}


std::string gkg::DwiMultipleSubjectTrackDensityImagingCommand::getStaticName()
{

  try
  {

    return "DwiMultipleSubjectTrackDensityImaging";

  }
  GKG_CATCH(
        "std::string "
        " gkg::DwiMultipleSubjectTrackDensityImagingCommand::getStaticName()" );

}


void gkg::DwiMultipleSubjectTrackDensityImagingCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameBundleMapDirectories;
    std::vector< double > outputResolutionVector;
    std::vector< std::string > fileNameInputVolumes;
    std::string fileNameTarget = "";
    std::vector< std::string > fileNameReferences;
    std::vector< std::string > fileNameDeformations;
    std::vector< std::string > fileNameTransform3dForBundleMaps;
    std::vector< std::string > fileNameTransform3dForVolume;
    std::string fileNameOutputVolume;
    float step = 0.1;
    int32_t threadCount = 1;
    std::string outputFormat;
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "DWI super-resolution mapper",
                                  _loadPlugin );
    application.addSeriesOption( "-b",
                                 "Input bundle map directory list",
                                 fileNameBundleMapDirectories );
    application.addSeriesOption( "-resolution",
                                 "Output resolution",
                                 outputResolutionVector,
                                 3,
                                 3 );
    application.addSeriesOption( "-i",
                                 "Input volume file name",
                                 fileNameInputVolumes,
                                 false );
    application.addSingleOption( "-target",
                                 "target",
                                 fileNameTarget, true );
    application.addSeriesOption( "-ref",
                                 "ref",
                                 fileNameReferences );
    application.addSeriesOption( "-trans",
                                 "transformation file",
                                 fileNameTransform3dForBundleMaps );
    application.addSeriesOption( "-def",
                                 "deformation file",
                                 fileNameDeformations );
    application.addSeriesOption( "-ti",
                                 "Input volume to common space  "
                                 "3D transform file name",
                                 fileNameTransform3dForVolume );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOutputVolume );
    application.addSingleOption( "-thread",
                                 "number of thread launched",
                                 threadCount,
                                 true );
    application.addSingleOption( "-s",
                                 "step ",
                                 step,
				                         true );
    application.addSingleOption( "-format",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 

    execute( fileNameBundleMapDirectories,
             outputResolutionVector,
             fileNameInputVolumes,
             fileNameTarget,
             fileNameReferences,
             fileNameDeformations,
             fileNameTransform3dForBundleMaps,
             fileNameTransform3dForVolume,
             fileNameOutputVolume,
             step,
             threadCount,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiMultipleSubjectTrackDensityImagingCommand::"
                     "parse()" );

}


void gkg::DwiMultipleSubjectTrackDensityImagingCommand::execute(
             const std::vector< std::string >& fileNameBundleMapDirectories,
             const std::vector< double >& outputResolutionVector,
             const std::vector< std::string >& fileNameInputVolumes,
             const std::string& fileNameTarget,
             const std::vector< std::string >& fileNameReferences,
             const std::vector< std::string >& fileNameDeformations,
             const std::vector< std::string >& fileNameTransform3dForBundleMaps,
             const std::vector< std::string >& fileNameTransform3dForVolume,
             const std::string& fileNameOutputVolume,
             float step,
             int32_t threadCount,
             const std::string& outputFormat,
             bool ascii,
             bool verbose )
 {

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////
    if( fileNameBundleMapDirectories.size() != fileNameInputVolumes.size() )
    {

       throw std::runtime_error( "bundle map directories vector "
                             "must be of same length than scalar volume maps" );

    }

    if( ( fileNameBundleMapDirectories.size() != 
                                        fileNameTransform3dForVolume.size() ) &&
                                  ( fileNameTransform3dForVolume.size() != 0 ) )
    {

       throw std::runtime_error( "bundle map directories vector "
              "must be of same length than scalar volume maps transformation" );

    }

    if( ( fileNameBundleMapDirectories.size() != fileNameReferences.size() ) &&
                                            ( fileNameReferences.size() != 0 ) )
    {

       throw std::runtime_error( "bundle map directories vector "
                          "must be of same length than bundle map references" );

    }

    if( ( fileNameReferences.size() != 
         fileNameDeformations.size() ) && ( fileNameDeformations.size() != 0 ) )
    {

       throw std::runtime_error( "bundle map transformations vector "
                        "must be of same length than bundle map deformations" );

    }

    if( fileNameTransform3dForBundleMaps.size() != fileNameReferences.size() )
    {

       throw std::runtime_error( "bundle map transformations vector "
                          "must be of same length than bundle map references" );

    }

    if( ( fileNameTransform3dForBundleMaps.size() != 0 ) && 
                                                      ( fileNameTarget == "" ) )
    {

       throw std::runtime_error( 
                         "need a target file name for bundle maps resampling" );

    }

    if ( step <= 0 )
    {

       throw std::runtime_error( "step must be strictly positive" );

    }

    if ( outputResolutionVector[ 0 ] <= 0 )
    {

       throw std::runtime_error( 
                              "output X resolution must be strictly positive" );

    }
    if ( outputResolutionVector[ 1 ] <= 0 )
    {

       throw std::runtime_error( 
                              "output Y resolution must be strictly positive" );

    }
    if ( outputResolutionVector[ 2 ] <= 0 )
    {

       throw std::runtime_error( 
                              "output Z resolution must be strictly positive" );

    }

    gkg::Vector3d< double > outputResolution( outputResolutionVector[ 0 ],
                                              outputResolutionVector[ 1 ],
                                              outputResolutionVector[ 2 ] );


    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    SuperResolutionProcess 
      superResolutionProcess( fileNameBundleMapDirectories,
                              outputResolution,
                              fileNameInputVolumes,
                              fileNameTarget,
                              fileNameReferences,
                              fileNameDeformations,
                              fileNameTransform3dForBundleMaps,
                              fileNameTransform3dForVolume,
                              fileNameOutputVolume,
                              step,
                              threadCount,
                              outputFormat,
                              ascii,
                              verbose );

      gkg::Directory directory( fileNameBundleMapDirectories.front() );

      if ( !directory.isValid() )
      {

        throw std::runtime_error( std::string( "invalid directory '" ) +
                           fileNameBundleMapDirectories.front() + "'" );

      }

      std::set< std::string > fileNames = directory.getFiles();
      std::string firstBundleMapPath = "";
      std::set< std::string >::const_iterator
        f = fileNames.begin(),
        fe = fileNames.end();
      while ( ( f != fe ) && firstBundleMapPath.empty() )
      {

        std::string fileName = *f;
        std::string::size_type found = fileName.find( "." );
        if ( found <= fileName.size() )
        {

          std::string extension = fileName.substr( found );
          if ( extension == ".bundles" || extension == ".bundlemap" )
          {

            firstBundleMapPath = fileNameBundleMapDirectories.front() +
                                   gkg::getDirectorySeparator() + fileName;

          }

        }
        ++ f;

      }

      superResolutionProcess.execute( firstBundleMapPath );

  }
  GKG_CATCH( "void gkg::DwiMultipleSubjectTrackDensityImagingCommand::execute( "
            "DwiMultipleSubjectTrackDensityImagingCommand( "
             "const std::vector< std::string >& fileNameBundleMapDirectories, "
             "const std::vector< double >& outputResolutionVector, "
             "const std::vector< std::string >& fileNameInputVolumes, "
             "const std::string& fileNameTarget, "
             "const std::vector< std::string >& fileNameReferences, "
             "const std::vector< std::string >& fileNameDeformations, "
             "const std::vector< std::string >& "
             "fileNameTransform3dForBundleMaps, "
             "const std::vector< std::string >& fileNameTransform3dForVolume, "
             "const std::string& fileNameOutputVolume, "
             "float step, "
             "int32_t threadCount, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiMultipleSubjectTrackDensityImagingCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameBundleMapDirectories ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outputResolutionVector ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameInputVolumes ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTarget ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameReferences ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameDeformations ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP(
                                            fileNameTransform3dForBundleMaps ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameTransform3dForVolume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputVolume ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_INTEGER_PARAMETER_HELP( threadCount ) +
    DECLARE_STRING_PARAMETER_HELP(  outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
