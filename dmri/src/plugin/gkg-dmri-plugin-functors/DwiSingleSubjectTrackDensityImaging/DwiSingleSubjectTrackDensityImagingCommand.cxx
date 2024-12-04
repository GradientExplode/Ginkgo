#include <gkg-dmri-plugin-functors/DwiSingleSubjectTrackDensityImaging/DwiSingleSubjectTrackDensityImagingCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>


//
// class TrackDensityImagingTraits< T >
//

template < class T >
struct TrackDensityImagingTraits
{

  typedef float accumulator_type;

  static void accumulatorInitialize(
                           gkg::Volume< accumulator_type >& accumulatorVolume );
  static void accumulate( const T& value,
                          accumulator_type& destination );
  static void average( const accumulator_type& accumulatedValue,
                       float density,
                       T& destination );

};


template <>
struct TrackDensityImagingTraits< gkg::RGBComponent >
{

  typedef gkg::Vector3d< float > accumulator_type;

  static void accumulatorInitialize(
                     gkg::Volume< gkg::Vector3d< float > >& accumulatorVolume );
  static void accumulate( const gkg::RGBComponent& value,
                          accumulator_type& destination );
  static void average( const accumulator_type& accumulatedValue,
                       float density,
                       gkg::RGBComponent& destination );

};


template < class T >
void TrackDensityImagingTraits< T >::accumulatorInitialize(
       gkg::Volume< typename TrackDensityImagingTraits< T >::accumulator_type >&
                                                             accumulatorVolume )
{

  accumulatorVolume.fill( 0.0 );

}


template < class T >
void TrackDensityImagingTraits< T >::accumulate(
        const T& value,
        typename TrackDensityImagingTraits< T >::accumulator_type& destination )
{

  destination += ( float )value;

}


template < class T >
void TrackDensityImagingTraits< T >::average(
  const typename TrackDensityImagingTraits< T >::accumulator_type& 
                                                               accumulatedValue,
  float density,
  T& destination )
{

  destination = ( T )( accumulatedValue / density );

}


void TrackDensityImagingTraits< gkg::RGBComponent >::accumulatorInitialize(
                      gkg::Volume< gkg::Vector3d< float > >& accumulatorVolume )
{

  accumulatorVolume.fill( gkg::Vector3d< float >( 0.0, 0.0, 0.0 ) );

}


void TrackDensityImagingTraits< gkg::RGBComponent >::accumulate(
               const gkg::RGBComponent& value,
               TrackDensityImagingTraits< gkg::RGBComponent
                                              >::accumulator_type& destination )
{

  destination.x += ( float )value.r;
  destination.y += ( float )value.g;
  destination.z += ( float )value.b;

}


void TrackDensityImagingTraits< gkg::RGBComponent >::average(
  const TrackDensityImagingTraits< gkg::RGBComponent
                                          >::accumulator_type& accumulatedValue,
  float density,
  gkg::RGBComponent& destination )
{

  destination.r = ( uint8_t )( accumulatedValue.x / density );
  destination.g = ( uint8_t )( accumulatedValue.y / density );
  destination.b = ( uint8_t )( accumulatedValue.z / density );

}


//
// class TrackDensityImagingProcess
//


class TrackDensityImagingProcess : public gkg::Process
{

  public:

    TrackDensityImagingProcess(
                const std::vector< std::string >& theFileNameBundleMaps,
                const gkg::Vector3d< double >& theOutputResolution,
                const std::string& theFileNameInputVolume,
                const std::string& theFileNameTransform3d,
                const std::string& theFileNameOutputVolume,
                const double& theStep,
                const std::string& theOutputFormat,
                const bool& theAscii,
                const bool& theVerbose );

    const std::vector< std::string >& fileNameBundleMaps;
    const gkg::Vector3d< double >& outputResolution;
    const std::string& fileNameInputVolume;
    const std::string& fileNameTransform3d;
    const std::string& fileNameOutputVolume;
    const double& step;
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
// class SubTrackDensityImagingProcess
//

template < class L >
class SubTrackDensityImagingProcess : public gkg::Process
{

  public:

    SubTrackDensityImagingProcess(
              const TrackDensityImagingProcess& theTrackDensityImagingProcess );

    const TrackDensityImagingProcess& trackDensityImagingProcess;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameInputVolume,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


//
// members of class SubTrackDensityImagingProcess
//

template < class L >
SubTrackDensityImagingProcess< L >::SubTrackDensityImagingProcess(
               const TrackDensityImagingProcess& theTrackDensityImagingProcess )
                                   : gkg::Process( "Volume" ),
                                     trackDensityImagingProcess(
                                                 theTrackDensityImagingProcess )
{

  registerProcess(
                 "Volume", gkg::TypeOf< int8_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< int8_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< uint8_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< uint8_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< int16_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< int16_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< uint16_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< uint16_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< int32_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< int32_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< uint32_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< uint32_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< int64_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< int64_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< uint64_t >::getName(),
                 &SubTrackDensityImagingProcess::template operate< uint64_t > );
  registerProcess(
                 "Volume", gkg::TypeOf< float >::getName(),
                 &SubTrackDensityImagingProcess::template operate< float > );
  registerProcess(
                  "Volume", gkg::TypeOf< double >::getName(),
                 &SubTrackDensityImagingProcess::template operate< double > );
  registerProcess(
                 "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                 &SubTrackDensityImagingProcess::template operate< 
                                                          gkg::RGBComponent > );

}


template < class L > template < class T >
void 
SubTrackDensityImagingProcess< L >::operate(
                                   gkg::Process& process,
 				   const std::string& /* fileNameInputVolume */,
 				   const gkg::AnalyzedObject&,
                                   const std::string& )
{

  try
  {

    SubTrackDensityImagingProcess&
      subTrackDensityImagingProcess =
        static_cast< SubTrackDensityImagingProcess& >( process );

    const TrackDensityImagingProcess& trackDensityImagingProcess =
      subTrackDensityImagingProcess.trackDensityImagingProcess;

    //////////////////////////////////////////////////////////////////////////
    // reading input volume
    //////////////////////////////////////////////////////////////////////////

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "reading '"
                << trackDensityImagingProcess.fileNameInputVolume
                << "' : " << std::flush;

    }

    
    gkg::Volume< T > inputVolume;
    std::string format = gkg::Reader::getInstance().read(
                                 trackDensityImagingProcess.fileNameInputVolume, 
                                 inputVolume );

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    gkg::Vector3d< int32_t > inputSize;
    int32_t inputSizeT = 1;
    inputVolume.getSize( inputSize, inputSizeT );

    gkg::Vector3d< double > inputResolution;
    double inputResolutionT = 1.0;
    inputVolume.getResolution( inputResolution, inputResolutionT );


    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "input volume size : "
                << inputSize.x << " x "
                << inputSize.y << " x "
                << inputSize.z << " x "
                << inputSizeT << std::endl;
      std::cout << "input volume resolution : "
                << inputResolution.x << "mm x "
                << inputResolution.y << "mm x "
                << inputResolution.z << "mm x "
                << inputResolutionT << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // creating ouput volume
    ////////////////////////////////////////////////////////////////////////////

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "creating output volume : " << std::flush;

    }

    gkg::Vector3d< int32_t >
      outputSize( ( int32_t )( ( inputSize.x * inputResolution.x ) /
                          trackDensityImagingProcess.outputResolution.x + 0.5 ),
                  ( int32_t )( ( inputSize.y * inputResolution.y ) /
                          trackDensityImagingProcess.outputResolution.y + 0.5 ),
                  ( int32_t )( ( inputSize.z * inputResolution.z ) /
                          trackDensityImagingProcess.outputResolution.z + 0.5 )
                 );
    int32_t outputSizeT = inputSizeT;
    double outputResolutionT = inputResolutionT;


    gkg::Volume< T > outputVolume( outputSize, outputSizeT );
    outputVolume.setResolution( trackDensityImagingProcess.outputResolution,
                                outputResolutionT );
    outputVolume.fill( T( 0 ) );

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "output volume size : "
                << outputSize.x << " x "
                << outputSize.y << " x "
                << outputSize.z << " x "
                << outputSizeT << std::endl;
      std::cout << "output volume resolution : "
                << trackDensityImagingProcess.outputResolution.x << "mm x "
                << trackDensityImagingProcess.outputResolution.y << "mm x "
                << trackDensityImagingProcess.outputResolution.z << "mm x "
                << outputResolutionT << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating volume accumulator and density mask
    ////////////////////////////////////////////////////////////////////////////

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "creating volume accumulator and density mask : "
                << std::flush;

    }

    gkg::Volume< typename TrackDensityImagingTraits< T >::accumulator_type >
      accumulatorVolume( outputSize, outputSizeT );
    accumulatorVolume.setResolution(
                                    trackDensityImagingProcess.outputResolution,
                                    outputResolutionT );
    TrackDensityImagingTraits< T >::accumulatorInitialize( accumulatorVolume );

    gkg::Volume< float > densityVolume( outputSize );
    densityVolume.setResolution( trackDensityImagingProcess.outputResolution );
    densityVolume.fill( 0.0 );

    gkg::BoundingBox< int32_t > boundingBox( densityVolume );

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading or creating 3D transform (inputVolume -> DWI)
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Transform3d< float > > transform3d;

    if ( !trackDensityImagingProcess.fileNameTransform3d.empty() )
    {

      if ( trackDensityImagingProcess.verbose )
      {

       std::cout << "reading '"
                 << trackDensityImagingProcess.fileNameTransform3d
                 << "' : " << std::flush;

      }
      try
      {

        gkg::RigidTransform3d< float >*
          rigidTransform3d = new gkg::RigidTransform3d< float >;
        std::ifstream 
          is( trackDensityImagingProcess.fileNameTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d.reset( rigidTransform3d );

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineWoShearingTransform3d< float >*
            affineWoShearingTransform3d =
              new gkg::AffineWoShearingTransform3d< float >;
          std::ifstream 
            is( trackDensityImagingProcess.fileNameTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineWoShearingTransform3d );

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream 
            is( trackDensityImagingProcess.fileNameTransform3d.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3d.reset( affineTransform3d );

        }

      }

      if ( trackDensityImagingProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( trackDensityImagingProcess.verbose )
      {

       std::cout << "creating identity 3D transform  : " << std::flush;

      }

      transform3d.reset( new gkg::IdentityTransform3d< float > );

      if ( trackDensityImagingProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating resampler
    ////////////////////////////////////////////////////////////////////////////

    
    gkg::QuickResampler< T >* resampler =
      gkg::QuickResamplerFactory< T >::getInstance().getQuickResampler(
                                            "Partial Voluming QuickResampler" );

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "switching to resampler : " << resampler->getName()
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over bundle map(s)
    ////////////////////////////////////////////////////////////////////////////

   
    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "looping over bundle map(s) : " << std::flush;

    }

    int32_t t = 0;
    T value;
    std::vector< std::string >::const_iterator
      fn = trackDensityImagingProcess.fileNameBundleMaps.begin(),
      fne = trackDensityImagingProcess.fileNameBundleMaps.end();

    int32_t bundleMapIndex = 0;
    int32_t bundleMapCount =
                ( int32_t )trackDensityImagingProcess.fileNameBundleMaps.size();

    while ( fn != fne )
    {

      if ( trackDensityImagingProcess.verbose )
      {

        if ( bundleMapIndex )
        {

          std::cout << gkg::Eraser( 26 );

        }
        std::cout << "bundle map [ " << std::setw( 3 ) << bundleMapIndex + 1 
                  << " / " << std::setw( 3 ) << bundleMapCount
                  << " ] " << std::flush;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading bundle map
      //////////////////////////////////////////////////////////////////////////

      gkg::RCPointer< gkg::BundleMap< L, std::less< L > > > 
        bundleMap( new gkg::BundleMap< L, std::less< L > > );
      gkg::Reader::getInstance().read( *fn, *bundleMap );
      int32_t bundleCount = bundleMap->getBundleCount();


      //////////////////////////////////////////////////////////////////////////
      // looping over bundle(s) and filling accumulator and density volume(s)
      //////////////////////////////////////////////////////////////////////////

      typename gkg::BundleMap< L >::const_iterator
        b = bundleMap->begin(),
        be = bundleMap->end();
      int32_t bundleIndex = 0;
      gkg::Vector3d< float > siteInInputVolumeFrame;
      gkg::Vector3d< int32_t > voxelInOutputVolumeFrame;

      while ( b != be )
      {

        const typename gkg::BundleMap< L >::Bundle& bundle = b->second;


        if ( trackDensityImagingProcess.verbose )
        {

          if ( bundleIndex )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << " bundle [ " << std::setw( 3 ) << bundleIndex + 1 
                    << " / " << std::setw( 3 ) << bundleCount
                    << " ] " << std::flush;

        }

        gkg::Curve3dMap< float >::const_iterator f = bundle.begin(),
                                                 fe = bundle.end();
        int32_t fiberIndex = 0;
        int32_t fiberCount = bundle.getCurve3dCount();
        while ( f != fe )
        {

          if ( trackDensityImagingProcess.verbose )
          {

            if ( ( fiberIndex == 0 ) ||
                 ( ( fiberIndex + 1 ) % 100 == 0 ) || 
                 ( fiberIndex == fiberCount - 1 ) )
            {

              if ( fiberIndex )
              {

                std::cout << gkg::Eraser( 28 );

              }
              std::cout << " fiber [ " << std::setw( 7 ) << fiberIndex + 1
                        << " / " << std::setw( 7 ) << fiberCount
                        << " ]" << std::flush;

            }

          }

          const gkg::LightCurve3d< float >& fiber = *f;

          float length = fiber.getLength();

          int32_t newPointCount =
           ( int32_t )( length / trackDensityImagingProcess.step + 0.5 );

          gkg::LightCurve3d< float >
            resampledFiber = fiber.getEquidistantCurve( newPointCount );

          gkg::LightCurve3d< float >::const_iterator
            s = resampledFiber.begin(),
            se = resampledFiber.end();
          while ( s != se )
          {

            transform3d->getInverse( *s, siteInInputVolumeFrame );
            
            voxelInOutputVolumeFrame.x =
              ( int32_t )( siteInInputVolumeFrame.x /
                          trackDensityImagingProcess.outputResolution.x + 0.5 );
            voxelInOutputVolumeFrame.y =
              ( int32_t )( siteInInputVolumeFrame.y /
                          trackDensityImagingProcess.outputResolution.y + 0.5 );
            voxelInOutputVolumeFrame.z =
              ( int32_t )( siteInInputVolumeFrame.z /
                          trackDensityImagingProcess.outputResolution.z + 0.5 );

            if ( boundingBox.contains( voxelInOutputVolumeFrame ) )
            {

              for ( t = 0; t < inputSizeT; t++ )
              {

                resampler->resample( inputVolume,
                                     T( 0 ),
                                     siteInInputVolumeFrame,
                                     value,
                                     &inputResolution,
                                     t );

                TrackDensityImagingTraits< T >::accumulate(
                     value,
                     accumulatorVolume( voxelInOutputVolumeFrame, t ) );

              }

              ++ densityVolume( voxelInOutputVolumeFrame );

            }

            ++ s;

          }

          ++ f;
          ++ fiberIndex;

        }

        ++ b;
        ++ bundleIndex;

      }

      ++ fn;
      ++ bundleMapIndex;

    }


    //////////////////////////////////////////////////////////////////////////
    // filling ouput volume
    //////////////////////////////////////////////////////////////////////////
   
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;


    for ( z = 0; z < outputSize.z; z++ )
    {

      for ( y = 0; y < outputSize.y; y++ )
      {

        for ( x = 0; x < outputSize.x; x++ )
        {

          if ( densityVolume( x, y, z ) > 0.0 )
          {

            for ( t = 0; t < outputSizeT; t++ )
            {

              TrackDensityImagingTraits< T >::average(
                                                accumulatorVolume( x, y, z, t ),
                                                densityVolume( x, y, z ),
                                                outputVolume( x, y, z, t ) );

            }

          }

        }

      }

    }

    //////////////////////////////////////////////////////////////////////////
    // writing output volume
    //////////////////////////////////////////////////////////////////////////

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "writing '"
                << trackDensityImagingProcess.fileNameOutputVolume
                << "' : " << std::flush;

    }

    if ( !trackDensityImagingProcess.outputFormat.empty() )
    {

      gkg::Writer::getInstance().write(
                                trackDensityImagingProcess.fileNameOutputVolume,
                                outputVolume,
                                trackDensityImagingProcess.ascii,
                                trackDensityImagingProcess.outputFormat );

    }
    else
    {

      gkg::Writer::getInstance().write(
                                trackDensityImagingProcess.fileNameOutputVolume,
                                outputVolume,
                                trackDensityImagingProcess.ascii,
                                format );

    }

    if ( trackDensityImagingProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }



  }
  GKG_CATCH( "template < class L > template < class T > "
             "void "
             "SubTrackDensityImagingProcess< L >::operate( "
             "gkg::Process& process, "
 	     "const std::string& fileNameInputVolume, "
 	     "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// members of class TrackDensityImagingProcess
//

TrackDensityImagingProcess::TrackDensityImagingProcess(
                const std::vector< std::string >& theFileNameBundleMaps,
                const gkg::Vector3d< double >& theOutputResolution,
                const std::string& theFileNameInputVolume,
                const std::string& theFileNameTransform3d,
                const std::string& theFileNameOutputVolume,
                const double& theStep,
                const std::string& theOutputFormat,
                const bool& theAscii,
                const bool& theVerbose )
                           : gkg::Process( "BundleMap" ),
                             fileNameBundleMaps( theFileNameBundleMaps ),
                             outputResolution( theOutputResolution ),
                             fileNameInputVolume( theFileNameInputVolume ),
                             fileNameTransform3d( theFileNameTransform3d ),
                             fileNameOutputVolume( theFileNameOutputVolume ),
                             step( theStep ),
                             outputFormat( theOutputFormat ),
                             ascii( theAscii ),
                             verbose( theVerbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &TrackDensityImagingProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &TrackDensityImagingProcess::operate< int16_t > );

}


template < class L >
void TrackDensityImagingProcess::operate( gkg::Process& process,
                                          const std::string& /* fileNameIn */,
                                          const gkg::AnalyzedObject&,
                                          const std::string& )
{

  try
  {

    TrackDensityImagingProcess&
      trackDensityImagingProcess =
        static_cast< TrackDensityImagingProcess& >( process );


    SubTrackDensityImagingProcess< L >
      subTrackDensityImagingProcess( trackDensityImagingProcess );
    subTrackDensityImagingProcess.execute(
                               trackDensityImagingProcess.fileNameInputVolume );



  }
  GKG_CATCH( "template < class L > "
             "void TrackDensityImagingProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiSingleSubjectTrackDensityImagingCommand
//

gkg::DwiSingleSubjectTrackDensityImagingCommand::
                                     DwiSingleSubjectTrackDensityImagingCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::DwiSingleSubjectTrackDensityImagingCommand::"
             "DwiSingleSubjectTrackDensityImagingCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiSingleSubjectTrackDensityImagingCommand::
                                     DwiSingleSubjectTrackDensityImagingCommand(
                           const std::vector< std::string >& fileNameBundleMaps,
                           const std::vector< double >& outputResolutionVector,
                           const std::string& fileNameInputVolume,
                           const std::string& fileNameTransform3d,
                           const std::string& fileNameOutputVolume,
                           float step,
                           const std::string& outputFormat,
                           bool ascii,
                           bool verbose )
                                               : gkg::Command()
{

  try
  {

    execute( fileNameBundleMaps,
             outputResolutionVector,
             fileNameInputVolume,
             fileNameTransform3d,
             fileNameOutputVolume,
             step,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSingleSubjectTrackDensityImagingCommand::"
             "DwiSingleSubjectTrackDensityImagingCommand( "
             "const std::vector< std::string >& fileNameBundleMaps, "
             "const std::vector< double >& outputResolutionVector, "
             "const std::string& fileNameInputVolume, "
             "const std::string& fileNameTransform3d, "
             "const std::string& fileNameOutputVolume, "
             "float step, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiSingleSubjectTrackDensityImagingCommand::
                                     DwiSingleSubjectTrackDensityImagingCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameBundleMaps );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           outputResolutionVector );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputVolume );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameBundleMaps,
             outputResolutionVector,
             fileNameInputVolume,
             fileNameTransform3d,
             fileNameOutputVolume,
             step,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiSingleSubjectTrackDensityImagingCommand::"
             "DwiSingleSubjectTrackDensityImagingCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiSingleSubjectTrackDensityImagingCommand::
                                   ~DwiSingleSubjectTrackDensityImagingCommand()
{
}


std::string gkg::DwiSingleSubjectTrackDensityImagingCommand::getStaticName()
{

  try
  {

    return "DwiSingleSubjectTrackDensityImaging";

  }
  GKG_CATCH( "std::string gkg::DwiSingleSubjectTrackDensityImagingCommand::"
             "getStaticName()" );

}


void gkg::DwiSingleSubjectTrackDensityImagingCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameBundleMaps;
    std::vector< double > outputResolutionVector;
    std::string fileNameInputVolume;
    std::string fileNameTransform3d;
    std::string fileNameOutputVolume;
    float step = 0.1;
    std::string outputFormat;
    bool ascii = false;
    bool verbose = false;

 
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "DWI Track Density Imaging tool",
                                  _loadPlugin );
    application.addSeriesOption( "-b",
                                 "Input bundle map file name list",
                                 fileNameBundleMaps,
                                 1 );
    application.addSeriesOption( "-resolution",
                                 "Output resolution",
                                 outputResolutionVector,
                                 3,
                                 3 );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameInputVolume );
    application.addSingleOption( "-t",
                                 "Input volume to bundle map "
                                 "3D transform file name",
                                 fileNameTransform3d,
                                 true );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOutputVolume );
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

    execute( fileNameBundleMaps,
             outputResolutionVector,
             fileNameInputVolume,
             fileNameTransform3d,
             fileNameOutputVolume,
             step,
             outputFormat,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::DwiSingleSubjectTrackDensityImagingCommand::"
                     "parse()" );

}


void gkg::DwiSingleSubjectTrackDensityImagingCommand::execute(
                           const std::vector< std::string >& fileNameBundleMaps,
                           const std::vector< double >& outputResolutionVector,
                           const std::string& fileNameInputVolume,
                           const std::string& fileNameTransform3d,
                           const std::string& fileNameOutputVolume,
                           float step,
                           const std::string& outputFormat,
                           bool ascii,
                           bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

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


    TrackDensityImagingProcess 
      trackDensityImagingProcess( fileNameBundleMaps,
                                  outputResolution,
                                  fileNameInputVolume,
                                  fileNameTransform3d,
                                  fileNameOutputVolume,
                                  step,
                                  outputFormat,
                                  ascii,
                                  verbose );

    trackDensityImagingProcess.execute( fileNameBundleMaps.front() );

  }
  GKG_CATCH( "void gkg::DwiSingleSubjectTrackDensityImagingCommand::execute( "
             "const std::vector< std::string >& fileNameBundleMaps, "
             "const std::vector< double >& outputResolutionVector, "
             "const std::string& fileNameInputVolume, "
             "const std::string& fileNameTransform3d, "
             "const std::string& fileNameOutputVolume, "
             "float step, "
             "const std::string& outputFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiSingleSubjectTrackDensityImagingCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( =fileNameBundleMaps ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outputResolutionVector ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameInputVolume ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTransform3d ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutputVolume ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
