#include <gkg-dmri-plugin-functors/DwiDiffusoidField/DwiDiffusoidFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-dmri-container/DiffusoidCartesianFieldFactory.h>
#include <gkg-dmri-io/DiffusoidFunctorWriter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/GeodesicSO3Interpolator.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand( int32_t argc,
                                                         char* argv[],
                                                         bool loadPlugin,
                                                         bool removeFirst )
                              : gkg::Command( argc, argv,
                                              loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& diffusoidFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     int32_t maximumNeighborCount,
                     float sigma,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             diffusoidFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             maximumNeighborCount,
             sigma,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& diffusoidFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "int32_t maximumNeighborCount, "
             "float sigma, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, modelType );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       diffusoidFunctorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          iBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           fBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           superResolution );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumNeighborCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, sigma );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, float, rgbScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, meshScale );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           specificScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         specificStringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             diffusoidFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             maximumNeighborCount,
             sigma,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiDiffusoidFieldCommand::DwiDiffusoidFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiDiffusoidFieldCommand::~DwiDiffusoidFieldCommand()
{
}


std::string gkg::DwiDiffusoidFieldCommand::getStaticName()
{

  try
  {

    return "DwiDiffusoidField";

  }
  GKG_CATCH( "std::string gkg::DwiDiffusoidFieldCommand::getStaticName()" );

}


void gkg::DwiDiffusoidFieldCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string modelType;
    std::list< std::string > diffusoidFunctorNames;
    std::list< std::string > outputFileNames;
    std::vector< int32_t > iBox;
    std::vector< float > fBox;
    std::vector< float > superResolution;
    int32_t outputOrientationCount = 0;
    int32_t maximumNeighborCount = 10;
    float sigma = 1.0;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    std::vector< double > specificScalarParameters;
    std::vector< std::string > specificStringParameters;
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // building Diffusoid model list
    ////////////////////////////////////////////////////////////////////////////

    std::string modelText = "Type of the Diffusoid model :\n";
    std::list< std::string >
      allModelNames = 
        gkg::DiffusoidCartesianFieldFactory::getInstance().getNames();
    std::list< std::string >::const_iterator
      m = allModelNames.begin(),
      me = allModelNames.end();
    while ( m != me )
    {

      modelText += "- " + *m + "\n";
      ++ m;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building Diffusoid functor list
    ////////////////////////////////////////////////////////////////////////////

    std::string diffusoidFunctorText = "Diffusoid functor list among :\n";
    std::list< std::string >
      allDiffusoidFunctorNames = gkg::DiffusoidFunctorWriter::getNames();
    std::list< std::string >::const_iterator
      f = allDiffusoidFunctorNames.begin(),
      fe = allDiffusoidFunctorNames.end();
    while ( f != fe )
    {

      diffusoidFunctorText += "- " + *f + "\n";
      ++ f;

    }


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI Diffusoid field model",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2 file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameDW );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-type",
                                 modelText,
                                 modelType );
    application.addSeriesOption( "-f",
                                 diffusoidFunctorText,
                                 diffusoidFunctorNames,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSeriesOption( "-ibox",
                                 "Integer bounding box on which to compute"
                                 " the ODF field",
                                 iBox );
    application.addSeriesOption( "-fbox",
                                 "Float bounding box on which to compute"
                                 " the ODF field",
                                 fBox );
    application.addSeriesOption( "-superResolution",
                                 "Super resolution of the ODF field"
                                 " (default=input resolution)",
                                 superResolution );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count "
                                 "(default=symmetrical input orientation set)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-maximumNeighborCount",
                                 "Geodesic SO3 maximum orientation neighbor "
                                 "count (default=10)",
                                 maximumNeighborCount,
                                 true );
    application.addSingleOption( "-sigma",
                                 "Geodesic SO3 interpolator std deviation "
                                 "(default=1.0)",
                                 sigma,
                                 true );

    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );

    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-meshMapFormat",
                                 "Ouput MeshMap format (default=aimsmesh)",
                                 meshMapFormat,
                                 true );
    application.addSingleOption( "-textureMapFormat",
                                 "Ouput MeshMap format (default=aimstexture)",
                                 textureMapFormat,
                                 true );

    application.addSingleOption( "-rgbScale",
                                 "Scale factor for RGB map(s) (default=1.0)",
                                 rgbScale,
                                 true );
    application.addSingleOption( "-meshScale",
                                 "Scale factor for mesh (default=1.0)",
                                 meshScale,
                                 true );

    application.addSeriesOption( "-scalarParameters",
                                 "Diffusoid cartesian field specific parameters "
                                 "as a vector of double <P1> <P2> ... <Pn>:"
                                 "\n\n"
                                 "- in case of "
                                 "acquisition_diffusoid_cartesian_field,\n"
                                 ".  N/A"
                                 "\n",
                                 specificScalarParameters );
    application.addSeriesOption( "-stringParameters",
                                 "Diffusoid cartesian field specific parameters "
                                 "as a vector of string <P1> <P2> ... <Pn>:"
                                 "\n\n"
                                 "- in case of "
                                 "acquisition_diffusoid_cartesian_field,\n"
                                 ".  N/A"
                                 "\n",
                                 specificStringParameters );

    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             diffusoidFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             maximumNeighborCount,
             sigma,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             specificScalarParameters,
             specificStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiDiffusoidFieldCommand::parse()" );

}


void gkg::DwiDiffusoidFieldCommand::execute( 
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& diffusoidFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     int32_t maximumNeighborCount,
                     float sigma,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( diffusoidFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error(
                             "different Diffusoid functor and file name count" );

    }
    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }
    if ( maximumNeighborCount < 0 )
    {

      throw std::runtime_error( "maximum neighbor couunt must be positive" );

    }
    if ( sigma <= 0.0 )
    {

      throw std::runtime_error( "Geodesic SO3 sigma order must be positive" );

    }

    std::vector< double >
      theSpecificScalarParameters = specificScalarParameters;
    std::vector< std::string > 
      theSpecificStringParameters = specificStringParameters;

    gkg::DiffusoidCartesianFieldFactory::getInstance().
      checkOrInitializeDefaultParameters( modelType,
                                          theSpecificScalarParameters,
                                          theSpecificStringParameters );


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > t2( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( *t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > dw( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( *dw );
    dwVolumeReaderProcess.execute( fileNameDW );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Volume< short > > mask( new gkg::Volume< int16_t > );
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask over T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        if ( t2->getHeader().hasAttribute( "resolutionX" ) &&
             t2->getHeader().hasAttribute( "resolutionY" ) &&
             t2->getHeader().hasAttribute( "resolutionZ" ) )
        {

          double resolutionX = 0.0;
          double resolutionY = 0.0;
          double resolutionZ = 0.0;
          t2->getHeader().getAttribute( "resolutionX", resolutionX );
          t2->getHeader().getAttribute( "resolutionY", resolutionY );
          t2->getHeader().getAttribute( "resolutionZ", resolutionZ );
          radius = 2.0 * std::min( resolutionX,
                                   std::min( resolutionY, resolutionZ ) );

        }
        else
        {

          radius = 1.0;

        }

      }
      gkg::ClosedMask< float, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( *t2, *mask, verbose );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( verbose )
      {
 
        std::cout << "writing 'mask' : " << std::flush;
 
      }

      gkg::File fileMask( outputFileNames.front() );
      std::string fileNameMask = fileMask.getDirectoryName() +
                                 gkg::getDirectorySeparator() +
                                 "mask";
      gkg::Writer::getInstance().write( fileNameMask,
                                        *mask,
                                        ascii,
                                        volumeFormat );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMask << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMask, *mask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output orientation set
    ////////////////////////////////////////////////////////////////////////////

    gkg::OrientationSet* outputOrientationSet = 0;
    if ( outputOrientationCount > 0 )
    {

      if ( verbose )
      {

        std::cout << "building output orientation count : " << std::flush;

      }
      outputOrientationSet = new gkg::OrientationSet(
                               gkg::ElectrostaticOrientationSet( 
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating Geodesic SO3 interpolator
    ////////////////////////////////////////////////////////////////////////////

    gkg::SO3Interpolator*
      so3Interpolator = new gkg::GeodesicSO3Interpolator( maximumNeighborCount,
                                                          sigma );


    ////////////////////////////////////////////////////////////////////////////
    // building Diffusoid cartesian field
    ////////////////////////////////////////////////////////////////////////////

    gkg::DiffusoidCartesianField* diffusoidCartesianField = 
      gkg::DiffusoidCartesianFieldFactory::getInstance().create(
                                                    modelType,
                                                    t2,
                                                    dw,
                                                    mask,
                                                    iBox,
                                                    fBox,
                                                    superResolution,
                                                    outputOrientationSet,
                                                    so3Interpolator,
                                                    theSpecificScalarParameters,
                                                    theSpecificStringParameters,
                                                    verbose );


    ////////////////////////////////////////////////////////////////////////////
    // building and writing Diffusoid functor map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::DiffusoidFunctorWriter diffusoidFunctorWriter( rgbScale,
                                                        meshScale,
                                                        volumeFormat,
                                                        meshMapFormat,
                                                        textureMapFormat,
                                                        ascii,
                                                        verbose );
    diffusoidFunctorWriter.write( diffusoidFunctorNames,
                                  outputFileNames,
                                  *diffusoidCartesianField );


    ////////////////////////////////////////////////////////////////////////////
    // deleting Geodesic SO3 interpolator, output orientation set, and
    // diffusoid field
    ////////////////////////////////////////////////////////////////////////////

    if ( so3Interpolator )
    {

      delete so3Interpolator;

    }
    if ( outputOrientationCount > 0 )
    {

      delete outputOrientationSet;

    }

    delete diffusoidCartesianField;

  }
  GKG_CATCH( "void gkg::DwiDiffusoidFieldCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& diffusoidFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "int32_t maximumNeighborCount, "
             "float sigma, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiDiffusoidFieldCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( modelType ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( diffusoidFunctorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( iBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( superResolution ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumNeighborCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( sigma ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( rgbScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( meshScale ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( specificScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( specificStringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
