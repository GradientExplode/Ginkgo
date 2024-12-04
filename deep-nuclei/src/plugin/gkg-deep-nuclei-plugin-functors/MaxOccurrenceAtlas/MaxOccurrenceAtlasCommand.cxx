#include <gkg-deep-nuclei-plugin-functors/MaxOccurrenceAtlas/MaxOccurrenceAtlasCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>


gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand( 
                                               const std::string& atlasFileName,
                                               const std::string& fileNameOut,
                                               bool ascii,
                                               const std::string& format,
                                               bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( atlasFileName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand( "
             "const std::string& atlasFileName, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, atlasFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( atlasFileName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MaxOccurrenceAtlasCommand::MaxOccurrenceAtlasCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MaxOccurrenceAtlasCommand::~MaxOccurrenceAtlasCommand()
{
}


std::string gkg::MaxOccurrenceAtlasCommand::getStaticName()
{

  try
  {

    return "MaxOccurrenceAtlas";

  }
  GKG_CATCH( "std::string gkg::MaxOccurrenceAtlasCommand::getStaticName()" );

}


void gkg::MaxOccurrenceAtlasCommand::parse()
{

  try
  {

    std::string atlasFileName;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Maximum occurrence atlas builder",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input occurrence atlas file name",
                                 atlasFileName );
    application.addSingleOption( "-o",
                                 "Output max occurrence atlas volume file name",
                                 fileNameOut );
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

    execute( atlasFileName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MaxOccurrenceAtlasCommand::parse()" );

}


void gkg::MaxOccurrenceAtlasCommand::execute( const std::string& atlasFileName,
                             		      const std::string& fileNameOut,
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
    gkg::Volume< int16_t > atlas( sizeX, sizeY, sizeZ );

    occurrenceAtlas.getResolution( resolution );

    atlas.getHeader() = occurrenceAtlas.getHeader();
    atlas.setResolution( resolution );
    atlas.getHeader().addAttribute( "sizeT", 1 );

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

      std::cout << "initializing atlas : " << std::flush;

    }

    int32_t i, j, k, rank;

    for ( k = 0; k < sizeZ; k++ )
    {

      for ( j = 0; j < sizeY; j++ )
      {

        for ( i = 0; i < sizeX; i++ )
        {

          int16_t max = 0;
          int16_t labelMax = 0;

          for ( rank = 0; rank < sizeT; rank++ )
          {

            int16_t label = rankToLabelLut[ rank ];

            if ( label > 0 )
            {

              int16_t value = occurrenceAtlas( i, j, k, rank );

              if ( value > max )
              {

                max = value;
                labelMax = label;

              }

            }

          }

          atlas( i, j, k ) = labelMax;

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
  GKG_CATCH( "void gkg::MaxOccurrenceAtlasCommand::execute( "
             "const std::string& atlasFileName, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( MaxOccurrenceAtlasCommand,
    			DECLARE_STRING_PARAMETER_HELP( atlasFileName ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
