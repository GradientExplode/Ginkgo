#include <gkg-dmri-plugin-functors/DwiOdfFieldFunctor/DwiOdfFieldFunctorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/SiteMapFactory_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-io/OdfFunctorWriter.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>


gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand( int32_t argc,
                                                           char* argv[],
                                                           bool loadPlugin,
                                                           bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand(
                     const std::string& fileNameSiteMap,
                     const std::string& fileNameOdfs,
                     const std::list< std::string >& odfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     int32_t outputOrientationCount,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameSiteMap,
             fileNameOdfs,
             odfFunctorNames,
             outputFileNames,
             outputOrientationCount,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "int32_t outputOrientationCount, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSiteMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfs );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       odfFunctorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, float, rgbScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, meshScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, lowerGFAThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, float, upperGFAThreshold );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           furtherScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         furtherStringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameSiteMap,
             fileNameOdfs,
             odfFunctorNames,
             outputFileNames,
             outputOrientationCount,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );


  }
  GKG_CATCH( "gkg::DwiOdfFieldFunctorCommand::DwiOdfFieldFunctorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiOdfFieldFunctorCommand::~DwiOdfFieldFunctorCommand()
{
}


std::string gkg::DwiOdfFieldFunctorCommand::getStaticName()
{

  try
  {

    return "DwiOdfFieldFunctor";

  }
  GKG_CATCH( "std::string gkg::DwiOdfFieldFunctorCommand::getStaticName()" );

}


void gkg::DwiOdfFieldFunctorCommand::parse()
{

  try
  {

    std::string fileNameSiteMap;
    std::string fileNameOdfs;
    std::list< std::string > odfFunctorNames;
    std::list< std::string > outputFileNames;
    int32_t outputOrientationCount = 0;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    float lowerGFAThreshold = 0.0;
    float upperGFAThreshold = 1.0;
    std::vector< double > furtherScalarParameters;
    std::vector< std::string > furtherStringParameters;
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // building ODF functor list
    ////////////////////////////////////////////////////////////////////////////

    std::string odfFunctorText = "ODF functor list among :\n";
    std::list< std::string >
      allOdfFunctorNames = gkg::OdfFunctorWriter
                                      ::getNamesForSiteTextureFunctorGeneration();
    std::list< std::string >::const_iterator
      f = allOdfFunctorNames.begin(),
      fe = allOdfFunctorNames.end();
    while ( f != fe )
    {

      odfFunctorText += "- " + *f + "\n";
      ++ f;

    }


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI ODF field functor(s)",
                                  _loadPlugin );
    application.addSingleOption( "-s",
                                 "Input site map",
                                 fileNameSiteMap );
    application.addSingleOption( "-o",
                                 "Input odf texture map file name",
                                 fileNameOdfs );
    application.addSeriesOption( "-f",
                                 odfFunctorText,
                                 odfFunctorNames,
                                 1 );
    application.addSeriesOption( "-O",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count "
                                 "(default=0)",
                                 outputOrientationCount,
                                 true );
    application.addSingleOption( "-rgbScale",
                                 "Scale factor for RGB map(s) (default=1.0)",
                                 rgbScale,
                                 true );
    application.addSingleOption( "-meshScale",
                                 "Scale factor for mesh (default=1.0)",
                                 meshScale,
                                 true );
    application.addSingleOption( "-lowerGFAThreshold",
                                 "Lower GFA threshold (default=0.0)",
                                 lowerGFAThreshold,
                                 true );
    application.addSingleOption( "-upperGFAThreshold",
                                 "Upper GFA threshold (default=1.0)",
                                 upperGFAThreshold,
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
    application.addSeriesOption( "-furtherScalarParameters",
                                 "Further scalar parameters to be used by "
                                 "functors",
                                 furtherScalarParameters );
    application.addSeriesOption( "-furtherStringParameters",
                                 "Further string parameters to be used by "
                                 "functors",
                                 furtherStringParameters );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameSiteMap,
             fileNameOdfs,
             odfFunctorNames,
             outputFileNames,
             outputOrientationCount,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiOdfFieldFunctorCommand::parse()" );

}


void gkg::DwiOdfFieldFunctorCommand::execute(
                     const std::string& fileNameSiteMap,
                     const std::string& fileNameOdfs,
                     const std::list< std::string >& odfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     int32_t outputOrientationCount,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( odfFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error( "different ODF functor and file name count" );

    }
    if ( outputOrientationCount <= 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading site(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameSiteMap << "' : " << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap;
    gkg::Reader::getInstance().read( fileNameSiteMap, siteMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading ODF texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameOdfs << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfs;
    gkg::Reader::getInstance().read( fileNameOdfs, odfs );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output orientation count
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building output orientation set : " << std::flush;

    }

    gkg::TextureMap< gkg::OrientationDistributionFunction >::const_iterator
      o = odfs.begin();
    if ( o == odfs.end() )
    {

      throw std::runtime_error( "ODF field is empty!" );

    }
    gkg::OrientationDistributionFunction::BasisType
      basisType = o->second.getBasisType();

    if ( basisType == gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      if ( outputOrientationCount <= 0 )
      {

        throw std::runtime_error( "missing or bad output orientation count" );

      }

    }
    else if ( basisType == gkg::OrientationDistributionFunction::Standard )
    {

      if ( outputOrientationCount > 0 )
      {

        std::cerr <<
         "'outputOrientationCount' parameter will be ignored as ODF "
         "texture map provided has got a standard basis, and therefore a "
         "fixed output orientation count" << std::endl;

      }

      outputOrientationCount = o->second.getValueCount();

    }

    gkg::OrientationSet outputOrientationSet( gkg::ElectrostaticOrientationSet(
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building cartesian ODF field from ODF texture map / site map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building ODF cartesian field : " << std::flush;

    }

    gkg::OdfCartesianField* 
      odfCartesianField = new gkg::OdfCartesianField( siteMap,
                                                      odfs,
                                                      outputOrientationSet );

    std::string name;
    odfs.getHeader().getAttribute( "odf_type", name );
    
    if ( verbose )
    {

      std::cout << "detected " << name << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building and writing ODF functor map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::OdfFunctorWriter odfFunctorWriter( rgbScale,
                                            meshScale,
                                            lowerGFAThreshold,
                                            upperGFAThreshold,
                                            volumeFormat,
                                            meshMapFormat,
                                            textureMapFormat,
                                            ascii,
                                            furtherScalarParameters,
                                            furtherStringParameters,
                                            verbose );

    odfFunctorWriter.write( odfFunctorNames,
                            outputFileNames,
                            *odfCartesianField );


    ////////////////////////////////////////////////////////////////////////////
    // ODF field
    ////////////////////////////////////////////////////////////////////////////

    delete odfCartesianField;

  }
  GKG_CATCH( "void gkg::DwiOdfFieldFunctorCommand::execute( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "int32_t outputOrientationCount, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiOdfFieldFunctorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameSiteMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfs ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( odfFunctorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( rgbScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( meshScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( lowerFAThreshold ) +
    DECLARE_FLOATING_PARAMETER_HELP( upperFAThreshold ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( furtherScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( furtherStringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
