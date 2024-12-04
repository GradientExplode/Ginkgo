#include <gkg-dmri-plugin-functors/DwiOdfLMSDifference/DwiOdfLMSDifferenceCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-io/DefaultAsciiItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemWriterOfOdf_i.h>
#include <gkg-dmri-io/DefaultAsciiItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBinaryItemReaderOfOdf_i.h>
#include <gkg-dmri-io/DefaultBSwapItemReaderOfOdf_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TextureMapAnalyzer_i.h>
#include <gkg-processing-io/TextureMapDiskFormat_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <algorithm>
#include <iomanip>


////////////////////////////////////////////////////////////////////////////////
// creating disk format for TextureMap< OrientationDistributionFunction >
////////////////////////////////////////////////////////////////////////////////

template class gkg::TextureMapAnalyzer< gkg::OrientationDistributionFunction >;
template class gkg::DiskFormatFactory<
                     gkg::TextureMap< gkg::OrientationDistributionFunction > >;
template class gkg::TextureMapDiskFormat<
                                        gkg::OrientationDistributionFunction >;

RegisterTextureAnalyzer( gkg::OrientationDistributionFunction, gkg_odf );
RegisterTextureDiskFormat( gkg::OrientationDistributionFunction, gkg_odf );


//
// class DwiOdfLMSDifferenceCommand
//

gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand( int32_t argc,
                                                             char* argv[],
                                                             bool loadPlugin,
                                                             bool removeFirst )
                                : gkg::Command( argc, argv, loadPlugin,
                                                removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand(
                                      const std::string& fileNameOdfTextureMap1,
                                      const std::string& fileNameOdfTextureMap2,
                                      const std::string& fileNameSiteMap1,
                                      const std::string& fileNameSiteMap2,
                                      const std::string& fileNameLMSDifference,
                                      const std::vector< int32_t >& sizes,
                                      bool ascii,
                                      const std::string& volumeFormat,
                                      bool displaySitesAndValues,
                                      bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameOdfTextureMap1,
             fileNameOdfTextureMap2,
             fileNameSiteMap1,
             fileNameSiteMap2,
             fileNameLMSDifference,
             sizes,
             ascii,
             volumeFormat,
             displaySitesAndValues,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand( "
             "const std::string& fileNameOdfTextureMap1, "
             "const std::string& fileNameOdfTextureMap2, "
             "const std::string& fileNameSiteMap1, "
             "const std::string& fileNameSiteMap2, "
             "const std::string& fileNameLMSDifference, "
             "const std::vector< int32_t >& sizes, "
             "bool ascii, "
             "const std::string& volumeFormat, "
             "bool displaySitesAndValues, "
             "bool verbose )" );

}


gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfTextureMap1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfTextureMap2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSiteMap1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSiteMap2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameLMSDifference );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          sizes );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, displaySitesAndValues );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameOdfTextureMap1,
             fileNameOdfTextureMap2,
             fileNameSiteMap1,
             fileNameSiteMap2,
             fileNameLMSDifference,
             sizes,
             ascii,
             volumeFormat,
             displaySitesAndValues,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiOdfLMSDifferenceCommand::DwiOdfLMSDifferenceCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiOdfLMSDifferenceCommand::~DwiOdfLMSDifferenceCommand()
{
}


std::string gkg::DwiOdfLMSDifferenceCommand::getStaticName()
{

  try
  {

    return "DwiOdfLMSDifference";

  }
  GKG_CATCH( "std::string gkg::DwiOdfLMSDifferenceCommand::getStaticName()" );

}


void gkg::DwiOdfLMSDifferenceCommand::parse()
{

  try
  {

    std::string fileNameOdfTextureMap1;
    std::string fileNameOdfTextureMap2;
    std::string fileNameSiteMap1;
    std::string fileNameSiteMap2;
    std::string fileNameLMSDifference;
    std::vector< int32_t > sizes( 3 );
    bool ascii = false;
    std::string volumeFormat = "gis";
    bool displaySitesAndValues = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI ODF LMS difference",
                                  _loadPlugin );
    application.addSingleOption( "-odf1",
                                 "Input first ODF texture map file name",
                                 fileNameOdfTextureMap1 );
    application.addSingleOption( "-odf2",
                                 "Input second ODF texture map file name",
                                 fileNameOdfTextureMap2 );
    application.addSingleOption( "-site1",
                                 "Input first site map file name",
                                 fileNameSiteMap1 );
    application.addSingleOption( "-site2",
                                 "Input first site map file name",
                                 fileNameSiteMap2 );
    application.addSingleOption( "-o",
                                 "Output square difference volume",
                                 fileNameLMSDifference );
    application.addSeriesOption( "-s",
                                 "Output volume sizes",
                                 sizes,
                                 3, 3 );
    application.addSingleOption( "-displaySitesAndValues",
                                 "Display site:lms_value (default=false)",
                                 displaySitesAndValues, 
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameOdfTextureMap1,
             fileNameOdfTextureMap2,
             fileNameSiteMap1,
             fileNameSiteMap2,
             fileNameLMSDifference,
             sizes,
             ascii,
             volumeFormat,
             displaySitesAndValues,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiOdfLMSDifferenceCommand::parse()" );

}


void gkg::DwiOdfLMSDifferenceCommand::execute(
                                     const std::string& fileNameOdfTextureMap1,
                                      const std::string& fileNameOdfTextureMap2,
                                      const std::string& fileNameSiteMap1,
                                      const std::string& fileNameSiteMap2,
                                      const std::string& fileNameLMSDifference,
                                      const std::vector< int32_t >& sizes,
                                      bool ascii,
                                      const std::string& /* volumeFormat */,
                                      bool displaySitesAndValues,
                                      bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading site map(s), ODF texture(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading ODF '" << fileNameOdfTextureMap1
                << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfTextureMap1;
    gkg::Reader::getInstance().read( fileNameOdfTextureMap1, odfTextureMap1 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading ODF '" << fileNameOdfTextureMap2
                << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfTextureMap2;
    gkg::Reader::getInstance().read( fileNameOdfTextureMap2, odfTextureMap2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading site map '" << fileNameSiteMap1 << "' : "
                << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap1;
    gkg::Reader::getInstance().read( fileNameSiteMap1, siteMap1 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    if ( verbose )
    {

      std::cout << "reading site map '" << fileNameSiteMap2 << "' : "
                << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap2;
    gkg::Reader::getInstance().read( fileNameSiteMap2, siteMap2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing LMS difference(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building rank/site LUT(s) :" << std::flush;

    }
    gkg::RankSiteLut< int32_t, int32_t > rankSiteLut1( siteMap1 );
    gkg::RankSiteLut< int32_t, int32_t > rankSiteLut2( siteMap2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // processing LMS difference(s)
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing LMS difference :";
      if ( displaySitesAndValues )
      {

        std::cout << std::endl;

      }
      else
      {

        std::cout << std::flush;

      }

    }
    gkg::Volume< float > lms( sizes[ 0 ], sizes[ 1 ], sizes[ 2 ] );    
    lms.fill( 0.0 );

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = siteMap1.getSites( 0 ).begin(),
      se = siteMap1.getSites( 0 ).end();
    while ( s != se )
    {

      if ( siteMap2.hasSite( 0, *s ) && 
           ( s->x < sizes[ 0 ] ) &&
           ( s->y < sizes[ 1 ] ) &&
           ( s->z < sizes[ 2 ] ) )
      {

        int32_t index1 = rankSiteLut1.getIndex( 0, *s );
        int32_t index2 = rankSiteLut2.getIndex( 0, *s );
        const gkg::OrientationDistributionFunction&
          odf1 = odfTextureMap1.getTexture( index1 );
        const gkg::OrientationDistributionFunction&
          odf2 = odfTextureMap2.getTexture( index2 );

        std::vector< float > probabilities1 = odf1.getOrientationProbabilities();
        std::vector< float > probabilities2 = odf2.getOrientationProbabilities();

        int32_t orientationCount = ( int32_t )probabilities1.size();
        if ( ( int32_t )probabilities2.size() != orientationCount )
        {

          throw std::runtime_error( "probability vector(s) have "
                                    "different size(s)" );

        }
        float value = 0.0;
        float difference = 0.0;
        int32_t o;
        for ( o = 0; o < orientationCount; o++ )
        {

          difference = probabilities1[ o ] - probabilities2[ o ];
          value += difference * difference;

        }
        lms( *s ) = std::sqrt( value / ( float )orientationCount );
        if ( displaySitesAndValues )
        {

          std::cout << *s << " : " << lms( *s ) << std::endl;

        }

      }
      ++ s;

    }
    if ( verbose && !displaySitesAndValues )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // saving LMS difference volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "saving LMS difference '" << fileNameLMSDifference << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameLMSDifference,
                                      lms,
                                      ascii );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiOdfLMSDifferenceCommand::execute( "
             "const std::string& fileNameOdfTextureMap1, "
             "const std::string& fileNameOdfTextureMap2, "
             "const std::string& fileNameSiteMap1, "
             "const std::string& fileNameSiteMap2, "
             "const std::string& fileNameLMSDifference, "
             "const std::vector< int32_t >& sizes, "
             "bool ascii, "
             "const std::string& volumeFormat, "
             "bool displaySitesAndValues, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiOdfLMSDifferenceCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfTextureMap1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfTextureMap2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSiteMap1 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSiteMap2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameLMSDifference ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( sizes ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( displaySitesAndValues ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
