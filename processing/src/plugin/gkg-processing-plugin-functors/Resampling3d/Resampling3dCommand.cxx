#include <gkg-processing-plugin-functors/Resampling3d/Resampling3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>


//
//
//


class TemplateInformationProcess : public gkg::Process
{

  public:

    TemplateInformationProcess( std::vector< int32_t >& theOutSize,
                                std::vector< double >& theOutResolution );

    std::vector< int32_t >& outSize;
    std::vector< double >& outResolution;

  private:

    template < class T >
    static void getInformation( gkg::Process& process,
                                const std::string& fileNameTemplate,
                                const gkg::AnalyzedObject&,
                                const std::string& );

};


TemplateInformationProcess::TemplateInformationProcess(
                                       std::vector< int32_t >& theOutSize,
                                       std::vector< double >& theOutResolution )
                           : gkg::Process( "Volume" ),
                             outSize( theOutSize ),
                             outResolution( theOutResolution )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &TemplateInformationProcess::getInformation< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &TemplateInformationProcess::getInformation< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &TemplateInformationProcess::getInformation< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &TemplateInformationProcess::getInformation< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &TemplateInformationProcess::getInformation< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &TemplateInformationProcess::getInformation< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &TemplateInformationProcess::getInformation< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &TemplateInformationProcess::getInformation< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &TemplateInformationProcess::getInformation< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &TemplateInformationProcess::getInformation< double > );
  registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                   &TemplateInformationProcess::getInformation< 
                                                          gkg::RGBComponent > );

}


template < class T >
void 
TemplateInformationProcess::getInformation( gkg::Process& process,
                                            const std::string& fileNameTemplate,
                                            const gkg::AnalyzedObject&,
                                            const std::string& )
{

  try
  {

    TemplateInformationProcess&
      templateInformationProcess = static_cast< TemplateInformationProcess& >(
                                                                      process );


    gkg::Volume< T > templateVolume;
    gkg::Reader::getInstance().readHeader( fileNameTemplate,
                                           templateVolume );

    templateVolume.getHeader().getAttribute(
                                      "sizeX", 
                                      templateInformationProcess.outSize[ 0 ] );
    templateVolume.getHeader().getAttribute(
                                      "sizeY",
                                      templateInformationProcess.outSize[ 1 ] );
    templateVolume.getHeader().getAttribute( 
                                      "sizeZ",
                                      templateInformationProcess.outSize[ 2 ] );
    templateVolume.getHeader().getAttribute( 
                                "resolutionX",             
                                templateInformationProcess.outResolution[ 0 ] );
    templateVolume.getHeader().getAttribute( 
                                "resolutionY",              
                                templateInformationProcess.outResolution[ 1 ] );
    templateVolume.getHeader().getAttribute(
                                "resolutionZ",               
                                templateInformationProcess.outResolution[ 2 ] );


  }
  GKG_CATCH( "template < class T > "
             "void "
             "TemplateInformationProcess::getInformation( "
             "gkg::Process& process, "
             "const std::string& fileNameTemplate, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// rotation of items by 3D rotation included in the provided trasnformation
// in case of RGB volume resampling
//


template < class T >
void rotate( const gkg::Rotation3d< double >& /*rotation3d*/, T& /*item*/ )
{
}


void rotate( const gkg::Rotation3d< double >& rotation3d,
             gkg::RGBComponent& item )
{

  try
  {

    gkg::Vector3d< double > itemAsVector( ( double )item.r,
                                          ( double )item.g,
                                          ( double )item.b );
    gkg::Vector3d< double > rotatedItemAsVector;

    rotation3d.getDirect( itemAsVector, rotatedItemAsVector );

    item.r = ( uint8_t )std::abs( rotatedItemAsVector.x );
    item.g = ( uint8_t )std::abs( rotatedItemAsVector.y );
    item.b = ( uint8_t )std::abs( rotatedItemAsVector.z );

  }
  GKG_CATCH( "void rotate( "
             "const gkg::Rotation3d< double >& rotation3d, "
             "gkg::RGBComponent& tmpOutVolume )" );

}


//
// class ResamplingProcess
//


class ResamplingProcess : public gkg::Process
{

  public:

    ResamplingProcess( const std::string& fileNameTemplate,
                       const std::vector< int32_t >& outSize,
                       const std::vector< double >& outResolution,
                       const std::vector< std::string >& fileNameTransforms,
                       const std::string& fileNameOut,
                       int32_t order,
                       double outBackground,
                       bool ascii,
                       const std::string& format,
                       bool applyInverseLinearTransformFirst,
                       bool verbose );

    const std::string& getFileNameTemplate() const;
    const std::vector< int32_t >& getOutSize() const;
    const std::vector< double >& getOutResolution() const;
    const std::vector< std::string >& getFileNameTransforms() const;
    const std::string& getFileNameOut() const;
    int32_t getOrder() const;
    double getOutBackground() const;
    bool getAscii() const;
    const std::string& getFormat() const;
    bool getApplyInverseLinearTransformFirst() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void resample( gkg::Process& process,
                          const std::string& fileNameReference,
                          const gkg::AnalyzedObject&,
                          const std::string& );

    const std::string& _fileNameTemplate;
    const std::vector< int32_t >& _outSize;
    const std::vector< double >& _outResolution;
    const std::vector< std::string >& _fileNameTransforms;
    const std::string& _fileNameOut;
    int32_t _order;
    double _outBackground;
    bool _ascii;
    const std::string& _format;
    bool _applyInverseLinearTransformFirst;
    bool _verbose;

};


ResamplingProcess::ResamplingProcess( 
                           const std::string& fileNameTemplate,
                           const std::vector< int32_t >& outSize,
                           const std::vector< double >& outResolution,
                           const std::vector< std::string >& fileNameTransforms,
                           const std::string& fileNameOut,
                           int32_t order,
                           double outBackground,
                           bool ascii,
                           const std::string& format,
                           bool applyInverseLinearTransformFirst,
                           bool verbose )
                  : gkg::Process( "Volume" ),
                    _fileNameTemplate( fileNameTemplate ),
                    _outSize( outSize ),
                    _outResolution( outResolution ),
                    _fileNameTransforms( fileNameTransforms ),
                    _fileNameOut( fileNameOut ),
                    _order( order ),
                    _outBackground( outBackground ),
                    _ascii( ascii ),
                    _format( format ),
                    _applyInverseLinearTransformFirst(
                                             applyInverseLinearTransformFirst ),
                    _verbose( verbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &ResamplingProcess::resample< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &ResamplingProcess::resample< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &ResamplingProcess::resample< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &ResamplingProcess::resample< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &ResamplingProcess::resample< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &ResamplingProcess::resample< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &ResamplingProcess::resample< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &ResamplingProcess::resample< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &ResamplingProcess::resample< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &ResamplingProcess::resample< double > );
    registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
                     &ResamplingProcess::resample< gkg::RGBComponent > );

  }
  GKG_CATCH( "ResamplingProcess::ResamplingProcess( "
             "const std::string& fileNameTemplate, "
             "const std::vector< int32_t >& outSize, "
             "const std::vector< double >& outResolution, "
             "const std::vector< std::string >& fileNameTransforms, "
             "const std::string& fileNameOut, "
             "int32_t order, "
             "double outBackground, "
             "bool ascii, "
             "const std::string& format, "
             "bool applyInverseLinearTransformFirst, "
             "bool verbose )" );

}


const std::string& ResamplingProcess::getFileNameTemplate() const
{

  return _fileNameTemplate;

}


const std::vector< int32_t >& ResamplingProcess::getOutSize() const
{

  return _outSize;

}


const std::vector< double >& ResamplingProcess::getOutResolution() const
{

  return _outResolution;

}


const std::vector< std::string >& 
ResamplingProcess::getFileNameTransforms() const
{

  return _fileNameTransforms;

}


const std::string& ResamplingProcess::getFileNameOut() const
{

  return _fileNameOut;

}


int32_t ResamplingProcess::getOrder() const
{

  return _order;

}


double ResamplingProcess::getOutBackground() const
{

  return _outBackground;

}


bool ResamplingProcess::getAscii() const
{

  return _ascii;

}


const std::string& ResamplingProcess::getFormat() const
{

  return _format;

}


bool ResamplingProcess::getApplyInverseLinearTransformFirst() const
{

  return _applyInverseLinearTransformFirst;

}


bool ResamplingProcess::getVerbose() const
{

  return _verbose;

}



template < class T >
void ResamplingProcess::resample( gkg::Process& process,
                                  const std::string& fileNameReference,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    ResamplingProcess&
      resamplingProcess = static_cast< ResamplingProcess& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // swicthing to appropriate resampler
    ////////////////////////////////////////////////////////////////////////////

    gkg::Resampler< T >* resampler =
      gkg::ResamplerFactory< T >::getInstance().getResampler(
                                                 resamplingProcess.getOrder() );
    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "switching to resampler : " << resampler->getName()
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading reference volume
    ////////////////////////////////////////////////////////////////////////////

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "reading reference volume '" << fileNameReference
                << "' : " << std::flush;

    }
    gkg::Volume< T > referenceVolume;
    gkg::Reader::getInstance().read( fileNameReference,
                                     referenceVolume );

    gkg::Vector3d< int32_t > referenceSize;
    int32_t referenceSizeT = 1;
    referenceVolume.getSize( referenceSize, referenceSizeT );

    gkg::Vector3d< double > referenceResolution;
    double referenceResolutionT = 1.0;
    referenceVolume.getResolution( referenceResolution, referenceResolutionT );

    // because some Nifti TR values are set to 0 
    if ( referenceResolutionT <= 0.0 )
    {

      referenceResolutionT = 1.0;

    }

    gkg::Volume< T > tmpReferenceVolume( referenceSize );
    tmpReferenceVolume.setResolution( referenceResolution,
                                      referenceResolutionT );

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting template information
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int32_t > outSize( 3 );
    std::vector< double > outResolution( 3 );

    if ( resamplingProcess.getFileNameTemplate().empty() )
    {

      outSize = resamplingProcess.getOutSize();
      outResolution = resamplingProcess.getOutResolution();

    }
    else
    {

      TemplateInformationProcess templateInformationProcess( outSize,
                                                             outResolution );
      templateInformationProcess.execute( 
                                      resamplingProcess.getFileNameTemplate() );

    }
    gkg::Vector3d< double > outResolutionVector3d;
    outResolutionVector3d.x = outResolution[ 0 ];
    outResolutionVector3d.y = outResolution[ 1 ];
    outResolutionVector3d.z = outResolution[ 2 ];

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "output size : "
                << outSize[ 0 ] << ", "
                << outSize[ 1 ] << ", "
                << outSize[ 2 ] << ", "
                << referenceSizeT << std::endl;
      std::cout << "output resolution : "
                << outResolution[ 0 ] << ", "
                << outResolution[ 1 ] << ", "
                << outResolution[ 2 ] << ", "
                << referenceResolutionT << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating output volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< T > outVolume( outSize[ 0 ], outSize[ 1 ], outSize[ 2 ],
                                referenceSizeT );
    outVolume.getHeader() = referenceVolume.getHeader();
    outVolume.getHeader()[ "sizeX" ] = outSize[ 0 ];
    outVolume.getHeader()[ "sizeY" ] = outSize[ 1 ];
    outVolume.getHeader()[ "sizeZ" ] = outSize[ 2 ];
    outVolume.getHeader()[ "sizeT" ] = referenceSizeT;
    outVolume.getHeader()[ "resolutionX" ] = outResolution[ 0 ];
    outVolume.getHeader()[ "resolutionY" ] = outResolution[ 1 ];
    outVolume.getHeader()[ "resolutionZ" ] = outResolution[ 2 ];
    outVolume.getHeader()[ "resolutionT" ] = referenceResolutionT;


    ////////////////////////////////////////////////////////////////////////////
    // creating temporary output volume
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< T > tmpOutVolume( outSize[ 0 ], outSize[ 1 ], outSize[ 2 ] );
    tmpOutVolume.getHeader()[ "resolutionX" ] = outResolution[ 0 ];
    tmpOutVolume.getHeader()[ "resolutionY" ] = outResolution[ 1 ];
    tmpOutVolume.getHeader()[ "resolutionZ" ] = outResolution[ 2 ];


    ////////////////////////////////////////////////////////////////////////////
    // reading transform
    ////////////////////////////////////////////////////////////////////////////

    if ( resamplingProcess.getVerbose() )
    {

      if ( resamplingProcess.getFileNameTransforms().size() == 1U )
      {

        std::cout << "reading transform '"
                  << resamplingProcess.getFileNameTransforms()[ 0 ]
                  << "' : " << std::flush;

      }
      else
      {

        std::cout << "reading transform '"
                  << resamplingProcess.getFileNameTransforms()[ 0 ]
                  << "' / '"
                  << resamplingProcess.getFileNameTransforms()[ 1 ]
                  << "' / '"
                  << resamplingProcess.getFileNameTransforms()[ 2 ]
                  << "' : " << std::flush;


      }

    }
    gkg::Transform3d< double >* transform3d = 0;
    gkg::RCPointer< gkg::Rotation3d< double > > rotation3d(
                                       new gkg::Rotation3d< double > );
    std::string foundTransformText = "undefined transform type";
    if ( resamplingProcess.getFileNameTransforms().size() == 1U )
    {

      try
      {

        gkg::RigidTransform3d< double >*
          rigidTransform3d = new gkg::RigidTransform3d< double >;
        std::ifstream
          is( resamplingProcess.getFileNameTransforms()[ 0 ].c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3d = rigidTransform3d;
        *rotation3d = rigidTransform3d->getRotation3d();

        foundTransformText = "rigid transformation detected";

      }
      catch ( std::exception& )
      {

        try
        {

          gkg::AffineTransform3d< double >*
            affineTransform3d =
              new gkg::AffineTransform3d< double >;
          std::ifstream
            is( resamplingProcess.getFileNameTransforms()[ 0 ].c_str() );
          affineTransform3d->readTrm( is );
          is.close();
          transform3d = affineTransform3d;
          *rotation3d = affineTransform3d->getRotation3d();
          foundTransformText = "affine transformation detected";

        }
        catch ( std::exception& )
        {

          throw std::runtime_error(
                                "unable to find an adequate linear transform" );

        }

      }

    }
    else
    {

      try
      {

        gkg::NonLinearTransform3d< double >*
          nonLinearTransform3d = new gkg::NonLinearTransform3d< double >(
                      resamplingProcess.getApplyInverseLinearTransformFirst() );
        nonLinearTransform3d->readTrm(
                               resamplingProcess.getFileNameTransforms()[ 0 ],
                               resamplingProcess.getFileNameTransforms()[ 1 ],
                               resamplingProcess.getFileNameTransforms()[ 2 ] );
        transform3d = nonLinearTransform3d;


        if ( !nonLinearTransform3d->
                                   getParameterizedRigidTransform3d().isNull() )
        {

          const gkg::RigidTransform3d< double >& 
            rigidTransform3d = 
              nonLinearTransform3d->getParameterizedRigidTransform3d()->
                                                          getRigidTransform3d();
          *rotation3d = rigidTransform3d.getRotation3d();

        }
        else if ( !nonLinearTransform3d->
                        getParameterizedAffineWoShearingTransform3d().isNull() )
        {

          const gkg::AffineWoShearingTransform3d< double >& 
            affineWoShearingTransform3d = 
              nonLinearTransform3d->
                                 getParameterizedAffineWoShearingTransform3d()->
                                               getAffineWoShearingTransform3d();
          *rotation3d = affineWoShearingTransform3d.getRotation3d();

        }
        else if ( !nonLinearTransform3d->
                                  getParameterizedAffineTransform3d().isNull() )
        {

          const gkg::AffineTransform3d< double >& 
            affineTransform3d = 
              nonLinearTransform3d-> getParameterizedAffineTransform3d()->
                                                         getAffineTransform3d();
          *rotation3d = affineTransform3d.getRotation3d();

        }


        foundTransformText = "non-linear transformation detected";

      }
      catch ( std::exception& )
      {

        throw std::runtime_error(
                            "unable to find an adequate non-linear transform" );

      }


    }
    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;
      std::cout << foundTransformText << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // resampling
    ////////////////////////////////////////////////////////////////////////////

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "resampling : " << std::flush;

    }

    
    gkg::Vector3d< int32_t > site;
    int32_t t = 0;
    for ( t = 0; t < referenceSizeT; t++ )
    {


      if ( resamplingProcess.getVerbose() )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << t + 1 
        	  << " / " << std::setw( 3 ) << referenceSizeT
        	  << " ]" << std::flush;


      }

      // copy temporary reference volume
      for ( site.z = 0; site.z < referenceSize.z; site.z++ )
      {

        for ( site.y = 0; site.y < referenceSize.y; site.y++ )
        {

          for ( site.x = 0; site.x < referenceSize.x; site.x++ )
          {

            tmpReferenceVolume( site ) = referenceVolume( site, t );

          }

        }

      }

      // resampling temporary output volume
      gkg::SplineResampler< T >* splineResampler =
        dynamic_cast< gkg::SplineResampler< T >* >( resampler );
      if ( splineResampler )
      {

        splineResampler->reset();

      }
      resampler->resample( tmpReferenceVolume,
                           *transform3d,
                           ( T )resamplingProcess.getOutBackground(),
                           tmpOutVolume,
                           resamplingProcess.getVerbose() );


      // copy resampled volume to output volume
      for ( site.z = 0; site.z < outSize[ 2 ]; site.z++ )
      {

        for ( site.y = 0; site.y < outSize[ 1 ]; site.y++ )
        {

          for ( site.x = 0; site.x < outSize[ 0 ]; site.x++ )
          {

            T& item = tmpOutVolume( site );
            rotate( *rotation3d, item );
            outVolume( site, t ) = item;

          }

        }

      }

    }

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "writing resampled volume '"
                << resamplingProcess.getFileNameOut()
                << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( resamplingProcess.getFileNameOut(),
                                      outVolume,
                                      resamplingProcess.getAscii(),
                                      resamplingProcess.getFormat() );
    if ( resamplingProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( transform3d )
    {

      delete transform3d;

    }

  }
  GKG_CATCH( "template < class T > "
             "void ResamplingProcess::resample( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//
//


gkg::Resampling3dCommand::Resampling3dCommand( int32_t argc,
                                 	       char* argv[],
                                 	       bool loadPlugin,
                                 	       bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Resampling3dCommand::Resampling3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Resampling3dCommand::Resampling3dCommand(
                           const std::string& fileNameReference,
                           const std::string& fileNameTemplate,
                           const std::vector< int32_t >& outSize,
                           const std::vector< double >& outResolution,
                           const std::vector< std::string >& fileNameTransforms,
                           const std::string& fileNameOut,
                           int32_t order,
                           double outBackground,
                           bool ascii,
                           const std::string& format,
                           bool applyInverseLinearTransformFirst,
                           bool verbose )
                        : gkg::Command()
{

  try
  {

    execute( fileNameReference,
             fileNameTemplate,
             outSize,
             outResolution,
             fileNameTransforms,
             fileNameOut,
             order,
             outBackground,
             ascii,
             format,
             applyInverseLinearTransformFirst,
             verbose );

  }
  GKG_CATCH( "gkg::Resampling3dCommand::Resampling3dCommand( "
             "const std::string& fileNameReference, "
             "const std::string& fileNameTemplate, "
             "const std::vector< int32_t >& outSize, "
             "const std::vector< double >& outResolution, "
             "const std::vector< std::string >& fileNameTransforms, "
             "const std::string& fileNameOut, int32_t order, "
             "double outBackground, bool ascii, const std::string& format, "
             "bool applyInverseLinearTransformFirst, "
             "bool verbose )" );

}


gkg::Resampling3dCommand::Resampling3dCommand(
                                             const gkg::Dictionary& parameters )
                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameReference );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTemplate );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          outSize );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           outResolution );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameTransforms );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, order );
    DECLARE_FLOATING_PARAMETER( parameters, double, outBackground );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool,
                               applyInverseLinearTransformFirst );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameReference,
             fileNameTemplate,
             outSize,
             outResolution,
             fileNameTransforms,
             fileNameOut,
             order,
             outBackground,
             ascii,
             format,
             applyInverseLinearTransformFirst,
             verbose );

  }
  GKG_CATCH( "gkg::Resampling3dCommand::Resampling3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Resampling3dCommand::~Resampling3dCommand()
{
}


std::string gkg::Resampling3dCommand::getStaticName()
{

  try
  {

    return "Resampling3d";

  }
  GKG_CATCH( "std::string gkg::Resampling3dCommand::getStaticName()" );

}


void gkg::Resampling3dCommand::parse()
{

  try
  {

    std::string fileNameReference;
    std::string fileNameTemplate;
    std::vector< int32_t > outSize;
    std::vector< double > outResolution;
    std::vector< std::string > fileNameTransforms;
    std::string fileNameOut;
    int32_t order = 3;
    double outBackground = 0.0;
    bool ascii = false;
    std::string format = "gis";
    bool applyInverseLinearTransformFirst = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D volume resampling tool",
                                  _loadPlugin );
    application.addSingleOption( "-reference",
                                 "Reference volume name",
                                 fileNameReference );
    application.addSingleOption( "-template",
                                 "Template volume name",
                                 fileNameTemplate,
                                 true );
    application.addSeriesOption( "-size",
                                 "Output size",
                                 outSize,
                                 0, 3 );
    application.addSeriesOption( "-resolution",
                                 "Output resolution in millimeter",
                                 outResolution,
                                 0, 3 );
    application.addSeriesOption( "-transforms",
                                 "Linear or non-linear transform file name(s)",
                                 fileNameTransforms,
                                 1, 3 );
    application.addSingleOption( "-output",
                                 "Output resampled volume name",
                                 fileNameOut );
    application.addSingleOption( "-order",
                                 "Resampling order (default=3):\n"
                                 "  - 0 -> nearest neighbor\n"
                                 "  - 1 -> linear\n"
                                 "  - 2 -> quadratic\n"
                                 "  - 3 -> cubic\n"
                                 "  - 4 -> quartic\n"
                                 "  - 5 -> quitinc\n"
                                 "  - 6 -> sixth order\n"
                                 "  - 7 -> seventh order\n"
                                 "(order should be kept to 0 for RGB volumes)",
                                 order,
                                 true );
    application.addSingleOption( "-background",
                                 "Output background (default=0.0)",
                                 outBackground,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Output volume format (default=gis)",
                                 format,
                                 true );
    application.addSingleOption( "-applyInverseLinearTransformFirst",
                                 "When set to true, apply the transformations "
                                 "in this order:\n"
                                 "- direct transformation:\n"
                                 "     p' = Diffeo * Linear * p \n"
                                 "- inverse transformation:\n"
                                 "     p' = Linear * Diffeo * p \n"
                                 "When set to false, apply the transformations "
                                 "in this order:\n"
                                 "- direct transformation:\n"
                                 "     p' = Linear * Diffeo * p \n"
                                 "- inverse transformation:\n"
                                 "     p' = Diffeo * Linear * p\n"
                                 "(default=false)",
                                 applyInverseLinearTransformFirst,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameReference,
             fileNameTemplate,
             outSize,
             outResolution,
             fileNameTransforms,
             fileNameOut,
             order,
             outBackground,
             ascii,
             format,
             applyInverseLinearTransformFirst,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Resampling3dCommand::parse()" );

}


void gkg::Resampling3dCommand::execute(
                  	   const std::string& fileNameReference,
                           const std::string& fileNameTemplate,
                           const std::vector< int32_t >& outSize,
                           const std::vector< double >& outResolution,
                           const std::vector< std::string >& fileNameTransforms,
                           const std::string& fileNameOut,
                           int32_t order,
                           double outBackground,
                           bool ascii,
                           const std::string& format,
                           bool applyInverseLinearTransformFirst,
                           bool verbose )
{

  try
  {

    //
    // sanity checks
    //
    if ( order > 7 )
    {

      throw std::runtime_error( "resampling order must be between 0 and 7" );

    }

    if ( fileNameTemplate.empty() &&
         ( ( outSize.size() != 3U ) || ( outResolution.size() != 3U ) ) )
    {

      throw std::runtime_error( "either give a template name or"
                                " give both size and resolution" );

    }
           
    if ( !fileNameTemplate.empty() &&
         ( !outSize.empty() || !outResolution.empty() ) )
    {

      throw std::runtime_error( "cannot provide size or resolution"
                                " when template name is given" );

    }

    if ( !( ( fileNameTransforms.size() == 1U ) ||
            ( fileNameTransforms.size() == 3U ) ) )
    {

      throw std::runtime_error(
                      "number of 3D transformation filenames must be 1 or 3" );

    }

    //
    // launching resampling process
    //
    ResamplingProcess resamplingProcess( fileNameTemplate,
                                         outSize,
                                         outResolution,
                                         fileNameTransforms,
                                         fileNameOut,
                                         order,
                                         outBackground,
                                         ascii,
                                         format,
                                         applyInverseLinearTransformFirst,
                                         verbose );
    resamplingProcess.execute( fileNameReference );

  }
  GKG_CATCH( "void gkg::Resampling3dCommand::execute( "
             "const std::string& fileNameReference, "
             "const std::string& fileNameTemplate, "
             "const std::vector< int32_t >& outSize, "
             "const std::vector< double >& outResolution, "
             "const std::vector< std::string >& fileNameTransforms, "
             "const std::string& fileNameOut, "
             "int32_t order, "
             "double outBackground, "
             "bool ascii, "
             "const std::string& format, "
             "bool applyInverseLinearTransformFirst, "
             "bool verbose )" );

}


RegisterCommandCreator(
                Resampling3dCommand,
                DECLARE_STRING_PARAMETER_HELP( fileNameReference ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameTemplate ) +
                DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( outSize ) +
                DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( outResolution ) +
                DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameTransforms ) +
                DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                DECLARE_INTEGER_PARAMETER_HELP( order ) +
                DECLARE_FLOATING_PARAMETER_HELP( outBackground ) +
                DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                DECLARE_STRING_PARAMETER_HELP( format ) +
                DECLARE_BOOLEAN_PARAMETER_HELP( 
                                            applyInverseLinearTransformFirst ) +
                DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
