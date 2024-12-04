#include <gkg-deep-nuclei-plugin-functors/ProbabilisticAtlas/ProbabilisticAtlasCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>


gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand( int32_t argc,
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
  GKG_CATCH( "gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand( 
                                              const std::string& atlasFileName,
                                              const std::string& fileNameOut,
                                              bool dilate,
                                              float dilationRadius,
                                              int16_t occurencyPercentThreshold,
                                              bool ascii,
                                              const std::string& format,
                                              bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( atlasFileName, fileNameOut, dilate, dilationRadius,
             occurencyPercentThreshold, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand( "
             "const std::string& atlasFileName, "
             "const std::string& fileNameOut, "
             "bool dilate, float dilationRadius, "
             "int16_t occurencyPercentThreshold, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, atlasFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, dilate );
    DECLARE_FLOATING_PARAMETER( parameters, float, dilationRadius );
    DECLARE_INTEGER_PARAMETER( parameters, int16_t, occurencyPercentThreshold );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( atlasFileName, fileNameOut, dilate, dilationRadius,
             occurencyPercentThreshold, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ProbabilisticAtlasCommand::ProbabilisticAtlasCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ProbabilisticAtlasCommand::~ProbabilisticAtlasCommand()
{
}


std::string gkg::ProbabilisticAtlasCommand::getStaticName()
{

  try
  {

    return "ProbabilisticAtlas";

  }
  GKG_CATCH( "std::string gkg::ProbabilisticAtlasCommand::getStaticName()" );

}


void gkg::ProbabilisticAtlasCommand::parse()
{

  try
  {

    std::string atlasFileName;
    std::string fileNameOut;
    bool dilate = false;
    float dilationRadius = 2.0;
    int16_t occurencyPercentThreshold = 75;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Probabilistic atlas builder",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input occurrence atlas file name",
                                 atlasFileName );
    application.addSingleOption( "-o",
                                 "Output probabilistic atlas volume file name",
                                 fileNameOut );
    application.addSingleOption( "-dilate",
                                 "Dilate the atlas (default=false)",
                                 dilate,
                                 true );
    application.addSingleOption( "-radius",
                                 "Dilatation radius in mm (default=5)",
                                 dilationRadius,
                                 true );
    application.addSingleOption( "-occurencyPercentThreshold",
                                 "Percent thresold on occurency (default=75%)",
                                 occurencyPercentThreshold,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode "
                                 "(default=false)",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( atlasFileName, fileNameOut, dilate, dilationRadius,
             occurencyPercentThreshold, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::ProbabilisticAtlasCommand::parse()" );

}


void gkg::ProbabilisticAtlasCommand::execute( const std::string& atlasFileName,
                             		      const std::string& fileNameOut,
                             		      bool dilate,
                             		      float dilationRadius,
                             		      int16_t occurencyPercentThreshold,
                             		      bool ascii,
                             		      const std::string& format,
                             		      bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading accumulation atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '"
                << atlasFileName
                << "' : " << std::flush;

    }
    gkg::Volume< int16_t > occurrenceAtlas;
    gkg::Reader::getInstance().read( atlasFileName, occurrenceAtlas );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // get look-up table(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "getting label<->rank LUT(s) : " << std::flush;

    }
    gkg::Dictionary labelToRankLut;
    std::map< int32_t, int16_t > rankToLabelLut;

    if ( occurrenceAtlas.getHeader().hasAttribute( "label_to_rank_lut" ) )
    {

      occurrenceAtlas.getHeader().getAttribute( "label_to_rank_lut",
                                                labelToRankLut );

      int16_t label;
      gkg::Dictionary::const_iterator
        l = labelToRankLut.begin(),
        le = labelToRankLut.end();

      while ( l != le )
      {

        int32_t rank = int32_t( l->second->getScalar() );
        gkg::StringConverter::toScalar( l->first, label );
        rankToLabelLut.insert( std::make_pair( rank, label ) );
        ++l;

      }

    }
    else
    {

      throw std::runtime_error( "no label to rank look up table in header" );

    }

    int32_t count = 0;
    if ( occurrenceAtlas.getHeader().hasAttribute( "data_base_count" ) )
    {

      double tmpValue = 0.0;
      occurrenceAtlas.getHeader().getAttribute( "data_base_count", tmpValue );
      count = int32_t( tmpValue );

    }
    else
    {

      throw std::runtime_error( "no data_base_count attribute in header" );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating a 4D Volume : atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "allocating atlas : " << std::flush;

    }
    int32_t sizeX = occurrenceAtlas.getSizeX();
    int32_t sizeY = occurrenceAtlas.getSizeY();
    int32_t sizeZ = occurrenceAtlas.getSizeZ();
    int32_t sizeT = occurrenceAtlas.getSizeT();
    gkg::Vector3d< double > resolution;
    gkg::Volume< int16_t > atlas( sizeX, sizeY, sizeZ, sizeT + 1 );

    occurrenceAtlas.getResolution( resolution );

    atlas.getHeader() = occurrenceAtlas.getHeader();
    atlas.setResolution( resolution );
    atlas.getHeader().addAttribute( "sizeT", sizeT + 1 );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // initializing volume of rank 0 which contains an initialized label 
    // volume computed from the atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "initializing atlas for rank : " << sizeT << " " 
                << std::flush;

    }
    gkg::GreaterThanFunction< int16_t >
      greaterThanFunction( ( int16_t )( ( ( float )occurencyPercentThreshold /
                                          100.0 ) *
                                          ( float )count ) );
    gkg::ConnectedComponents< int16_t, int16_t > 
      connectedComponents( greaterThanFunction, 
                           0,
                           1,
                           1,
                           true,
                           gkg::Neighborhood3d::Neighborhood3d_26 );
    gkg::Volume< int16_t > tmpVolume( sizeX, sizeY, sizeZ );
    int32_t i, j, k, rank;

    tmpVolume.setResolution( resolution );

    for ( rank = 0; rank < sizeT; rank++ )
    {

      connectedComponents.getComponents( occurrenceAtlas, tmpVolume, rank );

      int16_t label = rankToLabelLut[ rank ];

      for ( k = 0; k < sizeZ; k++ )
      {

        for ( j = 0; j < sizeY; j++ )
        {

          for ( i = 0; i < sizeX; i++ )
          {

            if ( tmpVolume( i, j, k ) != 0 )
            {

              atlas( i, j, k, sizeT ) = label;

            }

            if ( occurrenceAtlas( i, j, k, rank ) )
            {

              atlas( i, j, k, rank ) = sizeT + 1;

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // dilating atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( dilate )
    {

      if ( verbose )
      {

        std::cout << "dilating atlas : " << std::flush;

      }

      int16_t foreground = ( int16_t )( sizeT + 1 );
      int16_t background = 0;

      gkg::GreaterThanFunction< int16_t > greaterThanZero( 0 );
      gkg::Dilation< int16_t, int16_t > dilation( dilationRadius,
                                                  greaterThanZero,
                                                  foreground,
                                                  background );

      for ( rank = 0; rank < sizeT; rank++ )
      {

        for ( k = 0; k < sizeZ; k++ )
        {

          for ( j = 0; j < sizeY; j++ )
          {

            for ( i = 0; i < sizeX; i++ )
            {

              tmpVolume( i, j, k ) = atlas( i, j, k, rank );

            }

          }

        }

        dilation.dilate( tmpVolume, tmpVolume, verbose );

        for ( k = 0; k < sizeZ; k++ )
        {

          for ( j = 0; j < sizeY; j++ )
          {

            for ( i = 0; i < sizeX; i++ )
            {

              if ( !atlas( i, j, k, rank ) )
              {

                atlas( i, j, k, rank ) = tmpVolume( i, j, k );

              }

            }

          }

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, atlas, ascii, format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::ProbabilisticAtlasCommand::execute( "
             "const std::string& atlasFileName, "
             "const std::string& fileNameOut, "
             "bool dilate, float dilationRadius, "
             "int16_t occurencyPercentThreshold, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                   ProbabilisticAtlasCommand,
    		   DECLARE_STRING_PARAMETER_HELP( atlasFileName ) +
    		   DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		   DECLARE_BOOLEAN_PARAMETER_HELP( dilate ) +
    		   DECLARE_FLOATING_PARAMETER_HELP( dilationRadius ) +
    		   DECLARE_INTEGER_PARAMETER_HELP( occurencyPercentThreshold ) +
    		   DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		   DECLARE_STRING_PARAMETER_HELP( format ) +
    		   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
