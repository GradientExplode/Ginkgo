#include <gkg-processing-plugin-functors/PostMortemBlockToWholeImageComposer/PostMortemBlockToWholeImageComposerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-communication-thread/ThreadGauge.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>


//
// class ComposingGauge
//

class ComposingGauge : public gkg::ThreadGauge< int32_t >
{

  public: 
  
    ComposingGauge( int32_t maxCount ); 
    
    void add( int32_t value ); 
    void reset(); 
    
  private: 
  
    void display(); 
    
    int32_t _count; 
    int32_t _maxCount; 
    bool _alreadyDisplayed; 

};


ComposingGauge::ComposingGauge( int32_t maxCount )
              : _count( 0 ),
                _maxCount( maxCount ),
                _alreadyDisplayed( false )
{
}


void ComposingGauge::add( int32_t value )
{

  try
  {
   
    _count += value; 
    
    display();
   
  }
  GKG_CATCH( "ComposingGauge::add( int32_t value )" );
  
}


void ComposingGauge::reset()
{

  try
  {
   
    _count = 0; 
   
  }
  GKG_CATCH( "ComposingGauge::reset()" );
  
}


void ComposingGauge::display()
{

  try
  {
    
    if ( _alreadyDisplayed )
    {
    
      std::cout << gkg::Eraser( 20 );
    
    }
    
    std::cout << std::setw( 5 ) << _count
              << " / "
              << std::setw( 5 ) << _maxCount
              << " slices" 
              << std::flush;

    _alreadyDisplayed = true; 
  
  }
  GKG_CATCH( "void ComposingGauge::display()" ); 

}


//
// class ComposingLoopContext
//

template < class T >
class ComposingLoopContext : public gkg::LoopContext< int32_t >
{

  public:

    ComposingLoopContext( 
              ComposingGauge& gauge,
              const gkg::Vector3d< int32_t >& theReferenceSizes,
              const int32_t& theReferenceSizeT,
              const gkg::Vector3d< double >& theReferenceResolutions,
              const double& theReferenceResolutionT,
              const double& currentFovScaling,
              gkg::Resampler< T >* resampler,
              const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                                   transform3ds,
              const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                                    rotation3ds,
              gkg::RCPointer< gkg::Volume< T > > fovVolume,
              double background,
              gkg::PartialVolumingQuickResampler< T >& quickResampler,
              gkg::Volume< T >& outputVolume,
              bool verbose );

    void doIt( int32_t startIndex, int32_t count );

  protected:

    void composeItem(
                T& newOutputValue,
                const T& currentOutputValue,
                const T& outputValue,
                double currentFovScaling,
                const std::list< gkg::RCPointer< gkg::Transform3d< float > > >& 
                                                                  rotation3ds );

    const gkg::Vector3d< int32_t >& _referenceSizes;
    const int32_t& _referenceSizeT;
    const gkg::Vector3d< double >& _referenceResolutions;
    const double& _referenceResolutionT;
    const double& _currentFovScaling;
    gkg::Resampler< T >* _resampler;
    std::list< gkg::RCPointer< gkg::Transform3d< float > > > _transform3ds;
    std::list< gkg::RCPointer< gkg::Transform3d< float > > > _rotation3ds;
    gkg::RCPointer< gkg::Volume< T > > _fovVolume;
    double _background;
    gkg::PartialVolumingQuickResampler< T >& _quickResampler;
    gkg::Volume< T >& _outputVolume;
    bool _verbose;

};


template < class T >
inline
ComposingLoopContext< T >::ComposingLoopContext( 
   ComposingGauge& gauge,
   const gkg::Vector3d< int32_t >& theReferenceSizes,
   const int32_t& theReferenceSizeT,
   const gkg::Vector3d< double >& theReferenceResolutions,
   const double& theReferenceResolutionT,
   const double& currentFovScaling,
   gkg::Resampler< T >* resampler,
   const std::list< gkg::RCPointer< gkg::Transform3d< float > > >& transform3ds,
   const std::list< gkg::RCPointer< gkg::Transform3d< float > > >& rotation3ds,
   gkg::RCPointer< gkg::Volume< T > > fovVolume,
   double background,
   gkg::PartialVolumingQuickResampler< T >& quickResampler,
   gkg::Volume< T >& outputVolume,
   bool verbose )
                         : gkg::LoopContext< int32_t >( &gauge ),
                           _referenceSizes( theReferenceSizes ),
                           _referenceSizeT( theReferenceSizeT ),
                           _referenceResolutions( theReferenceResolutions ),
                           _referenceResolutionT( theReferenceResolutionT ),
                           _currentFovScaling( currentFovScaling ),
                           _resampler( resampler ),
                           _transform3ds( transform3ds ),
                           _rotation3ds( rotation3ds ),
                           _fovVolume( fovVolume ),
                           _background( background ),
                           _quickResampler( quickResampler ),
                           _outputVolume( outputVolume ),
                           _verbose( verbose )
{
}


template < class T >
inline
void ComposingLoopContext< T >::doIt( int32_t startIndex, int32_t count )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // collecting the FOV resolution
    ////////////////////////////////////////////////////////////////////////////


    gkg::Vector3d< int32_t > fovSize;
    _fovVolume->getSize( fovSize );

    gkg::Vector3d< double > fovResolution;
    _fovVolume->getResolution( fovResolution );

    gkg::BoundingBox< float >
      fovBoundingBox( ( float )( -fovResolution.x / 2.0f ), 
                      ( float )( ( ( double )fovSize.x + 0.5 ) *
                                 fovResolution.x ),
                      ( float )( -fovResolution.y / 2.0f ), 
                      ( float )( ( ( double )fovSize.y + 0.5 ) *
                                 fovResolution.y ),
                      ( float )( -fovResolution.z / 2.0f ), 
                      ( float )( ( ( double )fovSize.z + 0.5 ) *
                                 fovResolution.z ) );



    ////////////////////////////////////////////////////////////////////////////
    // allocating memory buffers for output T2 and DW items
    ////////////////////////////////////////////////////////////////////////////

    std::vector< bool > mask( _referenceSizes.x * _referenceSizes.y, false );
    std::vector< std::vector< T > > outputs( _referenceSizes.x *
                                             _referenceSizes.y );
    typename std::vector< std::vector< T > >::iterator
      o = outputs.begin(),
      oe = outputs.end();
    while ( o != oe )
    {

      o->resize( _referenceSizeT );
      ++ o;

    }

    ////////////////////////////////////////////////////////////////////////////
    // looping over output volume voxel(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > outputVoxel;
    gkg::Vector3d< float > outputSite;
    gkg::Vector3d< float > fovSite;
    gkg::Vector3d< float > tmpSite;
    T outputValue = T( 0 );
    int32_t n;
    int32_t index = 0;
    int32_t rank = 0;
    for ( n = 0, outputVoxel.z = startIndex; n < count; n++, outputVoxel.z++ )
    {

      if ( _verbose )
      {

        lock();
        gaugeAdd( 1 );
        unlock();

      }

      outputSite.z = ( outputVoxel.z + 0.5 ) * _referenceResolutions.z;
      index = 0;
      for ( outputVoxel.y = 0; outputVoxel.y < _referenceSizes.y;
            outputVoxel.y++ )
      {

        outputSite.y = ( outputVoxel.y + 0.5 ) * _referenceResolutions.y;
        for ( outputVoxel.x = 0; outputVoxel.x < _referenceSizes.x;
              outputVoxel.x++ )
        {

          outputSite.x = ( outputVoxel.x + 0.5 ) * _referenceResolutions.x;
          outputValue = ( T )0;
/*
          _resampler->resample( *_fovVolume,
                                *_transform3d,
                                _background,
                                outputSite,
                                outputValue,
                                &_fovResolution );
*/

          tmpSite = outputSite;
          std::list< gkg::RCPointer< gkg::Transform3d< float > > >::
            const_reverse_iterator t = _transform3ds.rbegin(),
                                   te = _transform3ds.rend();
          while ( t != te )
          {

            ( *t )->getInverse( tmpSite, fovSite );
            tmpSite = fovSite;
            ++ t;

          }

          if ( fovBoundingBox.contains( fovSite ) )
          {

            mask[ index ] = true;

            for ( rank = 0; rank < _referenceSizeT; rank++ )
            {

              _quickResampler.resample( *_fovVolume,
                                        _background,
                                        fovSite,
                                        outputValue,
                                        &fovResolution,
                                        rank );

              this->composeItem( outputs[ index ][ rank ],
                                 _outputVolume( outputVoxel, rank ),
                                 outputValue,
                                 _currentFovScaling,
                                 _rotation3ds );

            }

          }
          ++ index;

        }

      }

      lock();
      index = 0;
      for ( outputVoxel.y = 0; outputVoxel.y < _referenceSizes.y;
            outputVoxel.y++ )
      {

        for ( outputVoxel.x = 0; outputVoxel.x < _referenceSizes.x;
              outputVoxel.x++ )
        {

          if ( mask[ index ] )
          {

            for ( rank = 0; rank < _referenceSizeT; rank++ )
            {

              _outputVolume( outputVoxel, rank ) = outputs[ index ][ rank ];
              outputs[ index ][ rank ] = T( 0 );

            }
            mask[ index ] = false;

          }
          ++ index;

        }

      }
      unlock();

    }

  }
  GKG_CATCH( "template < class T > inline "
             "void ComposingLoopContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );
  

}


template < class T >
inline
void ComposingLoopContext< T >::composeItem( 
                 T& newOutputValue,
                 const T& currentOutputValue,
                 const T& outputValue,
                 double currentFovScaling,
                 const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                             /* rotation3ds */ )
{

  try
  {

    if ( ( double )outputValue * currentFovScaling >
         ( double )currentOutputValue )
    {

      newOutputValue = ( T )( ( double )outputValue * currentFovScaling );

    }
    else
    {

      newOutputValue = currentOutputValue;

    }

  }
  GKG_CATCH( "template < class T > inline "
             "void ComposingLoopContext< T >::composeItem( "
             "T& newOutputValue, "
             "const T& currentOutputValue, "
             "const T& outputValue, "
             "double currentFovScaling, "
             "const std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "/* rotation3ds */ )" );

}


template <>
inline
void ComposingLoopContext< gkg::RGBComponent >::composeItem(
                 gkg::RGBComponent& newOutputValue,
                 const gkg::RGBComponent& currentOutputValue,
                 const gkg::RGBComponent& outputValue,
                 double currentFovScaling,
                 const std::list< gkg::RCPointer< gkg::Transform3d< float > > >&
                                                                   rotation3ds )
{

  try
  {

    gkg::Vector3d< float > outputValueAsVector( ( float )outputValue.r,
                                                ( float )outputValue.g,
                                                ( float )outputValue.b );
    gkg::Vector3d< float > rotatedOutputValueAsVector;
    gkg::Vector3d< float > tmpVector = outputValueAsVector;
    std::list< gkg::RCPointer< gkg::Transform3d< float > > >::const_iterator
      r = rotation3ds.begin(),
      re = rotation3ds.end();
    while ( r != re )
    {

     ( *r )->getDirect( tmpVector, rotatedOutputValueAsVector );
     tmpVector = rotatedOutputValueAsVector;
     ++ r;

    }

    gkg::RGBComponent
      rotatedOutputValue( ( uint8_t )std::abs( rotatedOutputValueAsVector.x ),
                          ( uint8_t )std::abs( rotatedOutputValueAsVector.y ),
                          ( uint8_t )std::abs( rotatedOutputValueAsVector.z ) );
          


    if ( ( ( ( double )rotatedOutputValue.r * ( double )rotatedOutputValue.r +
             ( double )rotatedOutputValue.g * ( double )rotatedOutputValue.g +
             ( double )rotatedOutputValue.b * ( double )rotatedOutputValue.b ) *
           currentFovScaling * currentFovScaling ) >
         ( ( double )currentOutputValue.r *
           ( double )currentOutputValue.r +
           ( double )currentOutputValue.g *
           ( double )currentOutputValue.g +
           ( double )currentOutputValue.b *
           ( double )currentOutputValue.b ) )
    {

/*
      // we need here to flip R,G,B coordinates according to the following
      // transformation:
      //   r' = g
      //   g' = b
      //   b' = r

      newOutputValue.r =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.g *
                               currentFovScaling ) );
      newOutputValue.g =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.b *
                               currentFovScaling ) );
      newOutputValue.b =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.r *
                               currentFovScaling ) );
*/

      newOutputValue.r =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.r *
                               currentFovScaling ) );
      newOutputValue.g =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.g *
                               currentFovScaling ) );
      newOutputValue.b =
        ( uint8_t )( std::min( 255.0,
                               ( double )rotatedOutputValue.b *
                               currentFovScaling ) );

    }
    else
    {

      newOutputValue = currentOutputValue;

    }

  }
  GKG_CATCH( "template <> inline "
             "void ComposingLoopContext::composeItem( "
             "gkg::RGBComponent& newOutputValue, "
             "const gkg::RGBComponent& currentOutputValue, "
             "const gkg::RGBComponent& outputValue, "
             "double currentFovScaling, "
             "const std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "rotation3ds )" );

}


//
// class PostMortemBlockToWholeImageComposerProcess
//

class PostMortemBlockToWholeImageComposerProcess : public gkg::Process
{

  public:

    PostMortemBlockToWholeImageComposerProcess(
                   const std::vector< std::string >& theFileNameFovs,
                   const std::vector< std::string >&
                                          theFileNameFovToReferenceTransform3ds,
                   const std::vector< std::string >&
                                              theFovToReferenceTransform3dTypes,
                   const std::string& theFileNameOutput,
                   const std::vector< double >& theFovScalings,
                   const gkg::Vector3d< int32_t >& theReferenceSizes,
                   const gkg::Vector3d< double >& theReferenceResolutions,
                   int32_t theResamplingOrder,
                   double theBackground,
                   bool theAscii,
                   const std::string& theFormat,
                   bool theVerbose );

    const std::vector< std::string >& fileNameFovs;
    const std::vector< std::string >& fileNameFovToReferenceTransform3ds;
    const std::vector< std::string >& fovToReferenceTransform3dTypes;
    const std::string& fileNameOutput;
    const std::vector< double >& fovScalings;
    const gkg::Vector3d< int32_t >& referenceSizes;
    const gkg::Vector3d< double >& referenceResolutions;
    int32_t resamplingOrder;
    double background;
    bool ascii;
    const std::string& format;
    bool verbose;

  private:

    template < class T >
    static void compose( gkg::Process& process,
                         const std::string& fileNameFov,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    static void readTransform3ds(
        const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
        int32_t offsetTransform3d,
        const std::vector< int32_t >& transform3dFileNameCounts,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& transform3ds,
        const std::list< bool >& applyLinearTransformFirstList,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& rotation3ds,
        bool verbose );

};


PostMortemBlockToWholeImageComposerProcess::
                                     PostMortemBlockToWholeImageComposerProcess(
                   const std::vector< std::string >& theFileNameFovs,
                   const std::vector< std::string >&
                                          theFileNameFovToReferenceTransform3ds,
                   const std::vector< std::string >&
                                              theFovToReferenceTransform3dTypes,
                   const std::string& theFileNameOutput,
                   const std::vector< double >& theFovScalings,
                   const gkg::Vector3d< int32_t >& theReferenceSizes,
                   const gkg::Vector3d< double >& theReferenceResolutions,
                   int32_t theResamplingOrder,
                   double theBackground,
                   bool theAscii,
                   const std::string& theFormat,
                   bool theVerbose )
                         : gkg::Process( "Volume" ),
                           fileNameFovs( theFileNameFovs ),
                           fileNameFovToReferenceTransform3ds(
                                        theFileNameFovToReferenceTransform3ds ),
                           fovToReferenceTransform3dTypes(
                                            theFovToReferenceTransform3dTypes ),
                           fileNameOutput(
                             theFileNameOutput ),
                           fovScalings(
                             theFovScalings ),
                           referenceSizes(
                             theReferenceSizes ),
                           referenceResolutions(
                             theReferenceResolutions ),
                           resamplingOrder(
                             theResamplingOrder ),
                           background(
                             theBackground ),
                           ascii(
                             theAscii ),
                           format(
                             theFormat ),
                           verbose(
                             theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &compose< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &compose< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &compose< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &compose< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &compose< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &compose< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &compose< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &compose< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &compose< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &compose< double > );
    registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                     &compose< gkg::RGBComponent > );

  }
  GKG_CATCH( "PostMortemBlockToWholeImageComposerProcess:: "
             "PostMortemBlockToWholeImageComposerProcess( "
             "const std::vector< std::string >& theFileNameFovs, "
             "const std::vector< std::string >& "
             "theFileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "theFovToReferenceTransform3dTypes, "
             "const std::string& theFileNameOutput, "
             "const std::vector< float >& theFovScalings, "
             "const gkg::Vector3d< int32_t >& theReferenceSizes, "
             "const gkg::Vector3d< double >& theReferenceResolutions, "
             "int32_t theResamplingOrder, "
             "double theBackground, "
             "bool theAscii, "
             "const std::string& theFormat, "
             "bool theVerbose )" );

}


template < class T >
void 
PostMortemBlockToWholeImageComposerProcess::compose(
                                                gkg::Process& process,
                                                const std::string&,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    PostMortemBlockToWholeImageComposerProcess&
      postMortemBlockToWholeImageComposerProcess = static_cast<
                       PostMortemBlockToWholeImageComposerProcess& >( process );

    const std::vector< std::string >& fileNameFovs = 
      postMortemBlockToWholeImageComposerProcess.fileNameFovs;
    const std::vector< std::string >& 
      fileNameFovToReferenceTransform3ds =
                                     postMortemBlockToWholeImageComposerProcess.
                                       fileNameFovToReferenceTransform3ds;
    const std::vector< std::string >& 
      fovToReferenceTransform3dTypes =
                                     postMortemBlockToWholeImageComposerProcess.
                                                 fovToReferenceTransform3dTypes;
    const std::string& fileNameOutput = 
      postMortemBlockToWholeImageComposerProcess.fileNameOutput;
    const std::vector< double >& fovScalings = 
      postMortemBlockToWholeImageComposerProcess.fovScalings;
    const gkg::Vector3d< int32_t >& referenceSizes = 
      postMortemBlockToWholeImageComposerProcess.referenceSizes;
    const gkg::Vector3d< double >& referenceResolutions = 
      postMortemBlockToWholeImageComposerProcess.referenceResolutions;
    int32_t resamplingOrder = 
      postMortemBlockToWholeImageComposerProcess.resamplingOrder;
    double background = 
      postMortemBlockToWholeImageComposerProcess.background;
    bool ascii = 
      postMortemBlockToWholeImageComposerProcess.ascii;
    const std::string& format = 
      postMortemBlockToWholeImageComposerProcess.format;
    bool verbose = 
      postMortemBlockToWholeImageComposerProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    int32_t fovCount = ( int32_t )fileNameFovs.size();

    if ( fileNameFovToReferenceTransform3ds.empty() !=
         fovToReferenceTransform3dTypes.empty() )
    {

      throw std::runtime_error(
                "inconsistent FOV to reference 3D transform filename count and "
                "3D transform types" );

    }

    int32_t transform3dCount = ( int32_t )fovToReferenceTransform3dTypes.size();
    std::vector< int32_t > transform3dFileNameCounts;
    int32_t totalFileNameTransform3dCount = 0;
    std::list< bool > applyLinearTransformFirstList;
    if ( !fovToReferenceTransform3dTypes.empty() )
    {

      transform3dFileNameCounts.resize( transform3dCount );
      int32_t t = 0;
      for ( t = 0; t < transform3dCount; t++ )
      {

        if ( fovToReferenceTransform3dTypes[ t ] == "linear" )
        {

          transform3dFileNameCounts[ t ] = 1;
          totalFileNameTransform3dCount += 1;

        }
        else if ( fovToReferenceTransform3dTypes[ t ] ==
                  "linear_and_diffeomorphic" )
        {

          transform3dFileNameCounts[ t ] = 3;
          totalFileNameTransform3dCount += 3;
          applyLinearTransformFirstList.push_back( false );

        }
        else if ( fovToReferenceTransform3dTypes[ t ] ==
                  "diffeomorphic_and_linear" )
        {

          transform3dFileNameCounts[ t ] = 3;
          totalFileNameTransform3dCount += 3;
          applyLinearTransformFirstList.push_back( true );

        }
        else
        {

          throw std::runtime_error(
                                  std::string( "unknown 3d transform type '" ) +
                                  fovToReferenceTransform3dTypes[ t ] + "'" );

        }

      }

    }

    if ( ( int32_t )fileNameFovToReferenceTransform3ds.size() !=
         fovCount * totalFileNameTransform3dCount )
    {

      throw std::runtime_error(
                              "inconsistent number of 3D transforms and FOVs" );

    }

    if ( ( int32_t )fovScalings.size() != fovCount )
    {

      throw std::runtime_error( "inconsistent number of scalings and FOVs" );

    }

    if ( ( referenceSizes.x <= 0 ) ||
         ( referenceSizes.y <= 0 ) ||
         ( referenceSizes.z <= 0 ) )
    {

      throw std::runtime_error( "reference sizes must be strictly positive" );

    }
         
    if ( ( referenceResolutions.x <= 0 ) ||
         ( referenceResolutions.y <= 0 ) ||
         ( referenceResolutions.z <= 0 ) )
    {

      throw std::runtime_error(
                            "reference resolutions must be strictly positive" );

    }



    ////////////////////////////////////////////////////////////////////////////
    // collecting  output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "collecting t-dimension and t resolution of FOV volume(s) : "
                << std::flush;

    }
        
    int32_t referenceSizeT = 1;
    double referenceResolutionT = 1.0;
    {

      gkg::RCPointer< gkg::Volume< T > > fovVolume( new gkg::Volume< T > );
        gkg::Reader::getInstance().read( fileNameFovs[ 0 ],
                                         *fovVolume );
      referenceSizeT = fovVolume->getSizeT();
      gkg::Vector3d< double > fovResolution;
      fovVolume->getResolution( fovResolution, referenceResolutionT );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating output volume : "
                << std::flush;

    }


    gkg::Volume< T > outputVolume( referenceSizes, referenceSizeT );
    outputVolume.setResolution( referenceResolutions, referenceResolutionT );
    outputVolume.fill( T( 0 ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating a resampler
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating " << resamplingOrder << "-order resampler : "
                << std::flush;

    }

    gkg::Resampler< T >* 
      resampler = gkg::ResamplerFactory< T >::getInstance().getResampler(
                                                              resamplingOrder );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over FOVs
    ////////////////////////////////////////////////////////////////////////////

    int32_t fovIndex = 0;
    int32_t offsetTransform3d = 0;
    for ( fovIndex = 0; fovIndex < fovCount; fovIndex++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading current FOV
      //////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading current FOV '" << fileNameFovs[ fovIndex ]
                  << "' : "
                  << std::flush;

      }


      int32_t currentReferenceSizeT = 1;
      double currentReferenceResolutionT = 1.0;

      gkg::RCPointer< gkg::Volume< T > > fovVolume( new gkg::Volume< T > );
      gkg::Reader::getInstance().read( fileNameFovs[ fovIndex ],
                                       *fovVolume );
      currentReferenceSizeT = fovVolume->getSizeT();
      gkg::Vector3d< double > fovResolution;
      fovVolume->getResolution( fovResolution, currentReferenceResolutionT );

      if ( fovIndex )
      {

        if ( currentReferenceSizeT != referenceSizeT )
        {

          throw std::runtime_error( "inconsistent FOVs T size" );

        }
        if ( currentReferenceResolutionT != referenceResolutionT )
        {

          throw std::runtime_error( "inconsistent FOVs T resolution" );

        }

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // reading 3D transform
      //////////////////////////////////////////////////////////////////////////

      std::list< gkg::RCPointer< gkg::Transform3d< float > > > transform3ds;
      std::list< gkg::RCPointer< gkg::Transform3d< float > > > rotation3ds;
      if ( !fovToReferenceTransform3dTypes.empty() )
      {

        if ( verbose )
        {

          std::cout << "reading 3D transform(s) : "
                    << std::flush;

        }


        PostMortemBlockToWholeImageComposerProcess::readTransform3ds(
                                             fileNameFovToReferenceTransform3ds,
                                             offsetTransform3d,
                                             transform3dFileNameCounts,
                                             transform3ds,
                                             applyLinearTransformFirstList,
                                             rotation3ds,
                                             verbose );

      }
      else
      {

        if ( verbose )
        {

          std::cout << "setting 3D transform(s) to identity : "
                    << std::flush;

        }

        gkg::RCPointer< gkg::Transform3d< float > >
          identityTransform3d( new gkg::IdentityTransform3d< float > );

        transform3ds.push_back( identityTransform3d );
        rotation3ds.push_back( identityTransform3d );

        if ( verbose )
        {

          std::cout << "done"
                    << std::endl;

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // pointing to the current scaling factor
      //////////////////////////////////////////////////////////////////////////

      const double& currentFovScaling = fovScalings[ fovIndex ];
      if ( verbose )
      {

        std::cout << "applied FOV scaling : " << currentFovScaling
                  << std::endl;

      }


      //////////////////////////////////////////////////////////////////////////
      // resetting resampling if it is a spline resampler
      //////////////////////////////////////////////////////////////////////////

      gkg::SplineResampler< T >* splineResampler =
        dynamic_cast< gkg::SplineResampler< T >* >( resampler );
      if ( splineResampler )
      {

        splineResampler->reset();

      }


      //////////////////////////////////////////////////////////////////////////
      // looping over voxel of the output volume
      //////////////////////////////////////////////////////////////////////////


      if ( verbose )
      {

        std::cout << "composing : " << std::flush;

      }

      ComposingGauge gauge( referenceSizes.z );
      ComposingLoopContext< T > context(
                         gauge,
                         referenceSizes,
                         referenceSizeT,
                         referenceResolutions,
                         referenceResolutionT,
                         currentFovScaling,
                         resampler,
                         transform3ds,
                         rotation3ds,
                         fovVolume,
                         background,
                         gkg::PartialVolumingQuickResampler< T >::getInstance(),
                         outputVolume,
                         verbose );
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &context, 0, referenceSizes.z );

      threadedLoop.launch();

      if ( verbose )
      {

        std::cout << std::endl;

      }

      offsetTransform3d += totalFileNameTransform3dCount;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing output volume '"
                << fileNameOutput
                << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutput,
                                      outputVolume,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void "
             "PostMortemBlockToWholeImageComposerProcess::compose( "
             "gkg::Process& process, "
             "const std::string& floatingName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


void PostMortemBlockToWholeImageComposerProcess::readTransform3ds(
        const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
        int32_t offsetTransform3d,
        const std::vector< int32_t >& transform3dFileNameCounts,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& transform3ds,
        const std::list< bool >& applyLinearTransformFirstList,
        std::list< gkg::RCPointer< gkg::Transform3d< float > > >& rotation3ds,
        bool verbose )
{

  try
  {

    int32_t transform3dCount = ( int32_t )transform3dFileNameCounts.size();

    int32_t t = 0;
    int32_t localOffsetTransform3d = offsetTransform3d;
    std::list< bool >::const_iterator
      applyLinear = applyLinearTransformFirstList.begin();
    for ( t = 0; t < transform3dCount; t++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // in all cases, the first transformation is linear transformation
      //////////////////////////////////////////////////////////////////////////


      gkg::RCPointer< gkg::Rotation3d< float > > rotation3d(
                                                 new gkg::Rotation3d< float > );
      gkg::RCPointer< gkg::Transform3d< float > > transform3d;

      if ( transform3dFileNameCounts[ t ] == 1 )
      {

        std::string fileName = fileNameFovToReferenceTransform3ds[
                                                      localOffsetTransform3d ];
        ++ localOffsetTransform3d;
        try
        {

          gkg::RigidTransform3d< float >*
            rigidTransform3d = new gkg::RigidTransform3d< float >;
          std::ifstream is( fileName.c_str() );
          rigidTransform3d->readTrm( is );
          is.close();
          *rotation3d = rigidTransform3d->getRotation3d();
          transform3d.reset( rigidTransform3d );

          transform3ds.push_back( transform3d );
          rotation3ds.push_back( rotation3d );

          if ( verbose )
          {

            std::cout << "- " << fileName 
                      << " -> rigid transformation detected" << std::endl;

          }

        }
        catch ( std::exception& )
        {

          gkg::AffineTransform3d< float >*
            affineTransform3d =
              new gkg::AffineTransform3d< float >;
          std::ifstream is( fileName.c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          *rotation3d = affineTransform3d->getRotation3d();
          transform3d.reset( affineTransform3d );

          transform3ds.push_back( transform3d );
          rotation3ds.push_back( rotation3d );

          if ( verbose )
          {

            std::cout << "- " << fileName 
                      << " -> affine transformation detected" << std::endl;

          }

        }
      
      }


      //////////////////////////////////////////////////////////////////////////
      // then if it is a diffeomorphic transformation, reading the vector field
      //////////////////////////////////////////////////////////////////////////

      if ( transform3dFileNameCounts[ t ] == 3 )
      {

        gkg::RCPointer< gkg::NonLinearTransform3d< float > > 
          nonLinearTransform3d;

        nonLinearTransform3d.reset(
                      new gkg::NonLinearTransform3d< float >( *applyLinear ) );
        
        nonLinearTransform3d->readTrm(
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d ],
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d + 1 ],
                                  fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d + 2 ]  );
        ++ localOffsetTransform3d;
        ++ localOffsetTransform3d;
        ++ localOffsetTransform3d;
        ++ applyLinear;

        transform3ds.push_back( nonLinearTransform3d );


        if ( verbose )
        {

          std::cout << "- " << fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d - 2 ]
                    << " / " << fileNameFovToReferenceTransform3ds[
                                                localOffsetTransform3d - 1 ]
                    << " -> diffeomorphic transformation detected" << std::endl;

        }


      }

    }

  }
  GKG_CATCH( "void "
             "PostMortemBlockToWholeImageComposerProcess::readTransform3ds( "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "int32_t offsetTransform3d, "
             "const std::vector< int32_t >& transform3dFileNameCounts, "
             "std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "transform3ds, "
             "const std::list< bool >& applyLinearTransformFirstList, "
             "std::list< gkg::RCPointer< gkg::Transform3d< float > > >& "
             "rotation3ds, "
             "bool verbose )" );


}


//
//   PostMortemBlockToWholeImageComposerCommand
//

gkg::PostMortemBlockToWholeImageComposerCommand::
                                     PostMortemBlockToWholeImageComposerCommand(
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
  GKG_CATCH( "gkg::PostMortemBlockToWholeImageComposerCommand::"
             "PostMortemBlockToWholeImageComposerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::PostMortemBlockToWholeImageComposerCommand::
                                     PostMortemBlockToWholeImageComposerCommand(
                      const std::vector< std::string >& fileNameFovs,
                      const std::vector< std::string >&
                                             fileNameFovToReferenceTransform3ds,
                      const std::vector< std::string >&
                                                 fovToReferenceTransform3dTypes,
                      const std::string& fileNameOutput,
                      const std::vector< double >& fovScalings,
                      const gkg::Vector3d< int32_t >& referenceSizes,
                      const gkg::Vector3d< double >& referenceResolutions,
                      int32_t resamplingOrder,
                      double background,
                      bool ascii,
                      const std::string& format,
                      bool verbose )
                                                : gkg::Command()
{

  try
  {

    execute( fileNameFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutput,
             fovScalings,
             referenceSizes,
             referenceResolutions,
             resamplingOrder,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::PostMortemBlockToWholeImageComposerCommand::"
             "PostMortemBlockToWholeImageComposerCommand( "
             "const std::vector< std::string >& fileNameFovs, "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "fovToReferenceTransform3dTypes, "
             "const std::string& fileNameOutput, "
             "const std::vector< double >& fovScalings, "
             "const gkg::Vector3d< int32_t >& referenceSizes, "
             "const gkg::Vector3d< double >& referenceResolutions, "
             "int32_t resamplingOrder, "
             "double background, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::PostMortemBlockToWholeImageComposerCommand::
                                    PostMortemBlockToWholeImageComposerCommand(
                                             const gkg::Dictionary& parameters )
                                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameFovs );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fileNameFovToReferenceTransform3ds );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >, 
                                         fovToReferenceTransform3dTypes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           fovScalings );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          referenceSizes );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           referenceResolutions );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    gkg::Vector3d< int32_t > vector3dOfReferenceSizes( referenceSizes[ 0 ],
                                                       referenceSizes[ 1 ],
                                                       referenceSizes[ 2 ] );
    gkg::Vector3d< double >
      vector3dOfReferenceResolutions( referenceResolutions[ 0 ],
                                      referenceResolutions[ 1 ],
                                      referenceResolutions[ 2 ] );

    execute( fileNameFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutput,
             fovScalings,
             vector3dOfReferenceSizes,
             vector3dOfReferenceResolutions,
             resamplingOrder,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::PostMortemBlockToWholeImageComposerCommand::"
             "PostMortemBlockToWholeImageComposerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::PostMortemBlockToWholeImageComposerCommand::
                                   ~PostMortemBlockToWholeImageComposerCommand()
{
}


std::string gkg::PostMortemBlockToWholeImageComposerCommand::getStaticName()
{

  try
  {

    return "PostMortemBlockToWholeImageComposer";

  }
  GKG_CATCH( "std::string gkg::PostMortemBlockToWholeImageComposerCommand::"
             "getStaticName()" );

}


void gkg::PostMortemBlockToWholeImageComposerCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameFovs;
    std::vector< std::string > fileNameFovToReferenceTransform3ds;
    std::vector< std::string > fovToReferenceTransform3dTypes;
    std::string fileNameOutput;
    std::vector< double > fovScalings;
    std::vector< int32_t > vectorOfReferenceSizes;
    std::vector< double > vectorOfReferenceResolutions;
    int32_t resamplingOrder = 3;
    double background = 0.0;
    bool ascii = false;
    std::string format;
    bool verbose = false;


    gkg::Application application( _argc, _argv,
                                  "Post-mortem block to whole brain composer",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Block FOV filename list",
                                 fileNameFovs,
                                 1 );
    application.addSeriesOption( "-t",
                                 "Block to whole brain 3D transform(s) "
                                 "filename list",
                                 fileNameFovToReferenceTransform3ds,
                                 0 );
    application.addSeriesOption( "-types",
                                 "Block to whole brain 3D transform(s) "
                                 "type(s) among :\n"
                                 "- linear (<direct>.trm)\n"
                                 "- linear_and_diffeomorphic "
                                 "(<direct>.trm <direct>.ima <inverse>.ima)\n"
                                 "- diffeomorphic_and_linear "
                                 "(<direct>.trm <direct>.ima <inverse>.ima)"
                                 "(apply linear transform first)\n"
                                 "(default=none)",
                                 fovToReferenceTransform3dTypes,
                                 0 );
    application.addSingleOption( "-o",
                                 "Output volume filename",
                                 fileNameOutput );
    application.addSeriesOption( "-s",
                                 "FOV scaling list",
                                 fovScalings,
                                 1 );
    application.addSeriesOption( "-referenceSizes",
                                 "Reference sizes along X, Y and Z axis",
                                 vectorOfReferenceSizes,
                                 3, 3 );
    application.addSeriesOption( "-referenceResolutions",
                                 "Reference resolutions along X, Y and Z axis",
                                 vectorOfReferenceResolutions,
                                 3, 3 );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (0 to 7) (default=1)",
                                 resamplingOrder,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0.0)",
                                 background,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();


    gkg::Vector3d< int32_t > referenceSizes( vectorOfReferenceSizes[ 0 ],
                                             vectorOfReferenceSizes[ 1 ],
                                             vectorOfReferenceSizes[ 2 ] );
    gkg::Vector3d< double >
      referenceResolutions( vectorOfReferenceResolutions[ 0 ],
                            vectorOfReferenceResolutions[ 1 ],
                            vectorOfReferenceResolutions[ 2 ] );

    execute( fileNameFovs,
             fileNameFovToReferenceTransform3ds,
             fovToReferenceTransform3dTypes,
             fileNameOutput,
             fovScalings,
             referenceSizes,
             referenceResolutions,
             resamplingOrder,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::PostMortemBlockToWholeImageComposerCommand::"
                     "parse()" );

}


void gkg::PostMortemBlockToWholeImageComposerCommand::execute(
     const std::vector< std::string >& fileNameFovs,
     const std::vector< std::string >& fileNameFovToReferenceTransform3ds,
     const std::vector< std::string >& fovToReferenceTransform3dTypes,
     const std::string& fileNameOutput,
     const std::vector< double >& fovScalings,
     const Vector3d< int32_t >& referenceSizes,
     const Vector3d< double >& referenceResolutions,
     int32_t resamplingOrder,
     double background,
     bool ascii,
     const std::string& format,
     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // launching registration process
    ////////////////////////////////////////////////////////////////////////////

    PostMortemBlockToWholeImageComposerProcess
      postMortemBlockToWholeImageComposerProcess(
                                             fileNameFovs,
                                             fileNameFovToReferenceTransform3ds,
                                             fovToReferenceTransform3dTypes,
                                             fileNameOutput,
                                             fovScalings,
                                             referenceSizes,
                                             referenceResolutions,
                                             resamplingOrder,
                                             background,
                                             ascii,
                                             format,
                                             verbose );
    postMortemBlockToWholeImageComposerProcess.execute( fileNameFovs[ 0 ] );

  }
  GKG_CATCH( "void gkg::PostMortemBlockToWholeImageComposerCommand::execute( "
             "const std::vector< std::string >& fileNameFovs, "
             "const std::vector< std::string >& "
             "fileNameFovToReferenceTransform3ds, "
             "const std::vector< std::string >& "
             "fovToReferenceTransform3dTypes, "
             "const std::string& fileNameOutput, "
             "const std::vector< double >& fovScalings, "
             "const Vector3d< int32_t >& referenceSizes, "
             "const Vector3d< double >& referenceResolutions, "
             "int32_t resamplingOrder, "
             "double background, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
    PostMortemBlockToWholeImageComposerCommand,
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameFovs ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( 
                                          fileNameFovToReferenceTransform3ds ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fovToReferenceTransform3dTypes ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fovScalings ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( referenceSizes ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( rReferenceResolutions ) +
    DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder ) +
    DECLARE_FLOATING_PARAMETER_HELP( background ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
