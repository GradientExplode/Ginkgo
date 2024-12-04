#include <gkg-processing-plugin-functors/SubVolume/SubVolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/SubVolumeProcess.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::SubVolumeCommand::SubVolumeCommand( int32_t argc,
                                         char* argv[],
                                         bool loadPlugin,
                                         bool removeFirst )
                     : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::SubVolumeCommand::SubVolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SubVolumeCommand::SubVolumeCommand(
                                 const std::string& fileNameIn,
                                 const std::string& fileNameOut, 
                                 int32_t sx, int32_t sy, int32_t sz, int32_t st,
                                 int32_t ex, int32_t ey, int32_t ez, int32_t et,
                                 const std::list< int32_t >& xIndices,
                                 const std::list< int32_t >& yIndices,
                                 const std::list< int32_t >& zIndices,
                                 const std::list< int32_t >& tIndices,
                                 bool ascii,
                                 const std::string& format,
                                 bool verbose )
                     : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, sx, sy, sz, st,
             ex, ey, ez, et, xIndices, yIndices, zIndices, tIndices,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::SubVolumeCommand::SubVolumeCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, int32_t ez, int32_t et, "
             "const std::list< int32_t >& xIndices, "
             "const std::list< int32_t >& yIndices, "
             "const std::list< int32_t >& zIndices, "
             "const std::list< int32_t >& tIndices, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::SubVolumeCommand::SubVolumeCommand( const gkg::Dictionary& parameters )
                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sx );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sy );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, sz );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, st );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, ex );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, ey );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, ez );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, et );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        xIndices );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        yIndices );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        zIndices );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        tIndices );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, sx, sy, sz, st,
             ex, ey, ez, et, xIndices, yIndices, zIndices, tIndices,
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::SubVolumeCommand::SubVolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SubVolumeCommand::~SubVolumeCommand()
{
}


std::string gkg::SubVolumeCommand::getStaticName()
{

  try
  {

    return "SubVolume";

  }
  GKG_CATCH( "std::string gkg::SubVolumeCommand::getStaticName()" );

}


void gkg::SubVolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t sx = -1, sy = -1, sz = -1, st = -1;
    int32_t ex = -1, ey = -1, ez = -1, et = -1;
    std::list< int32_t > xIndices;
    std::list< int32_t > yIndices;
    std::list< int32_t > zIndices;
    std::list< int32_t > tIndices;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Extraction of a sub-volume from a volume",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-x", "First x coordinate (default 0)",
                                 sx, true );
    application.addSingleOption( "-X", "Last x coordinate (default sizeX - 1)",
                                 ex, true );
    application.addSingleOption( "-y", "First y coordinate (default 0)",
                                 sy, true );
    application.addSingleOption( "-Y", "Last y coordinate (default sizeY - 1)",
                                 ey, true );
    application.addSingleOption( "-z", "First z coordinate (default 0)",
                                 sz, true );
    application.addSingleOption( "-Z", "Last z coordinate (default sizeZ - 1)",
                                 ez, true );
    application.addSingleOption( "-t", "First t coordinate (default 0)",
                                 st, true );
    application.addSingleOption( "-T", "Last t coordinate (default sizeT - 1)",
                                 et, true );
    application.addSeriesOption( "-xIndices", "X indices to be extracted "
                                 "(default=none)",
                                 xIndices );
    application.addSeriesOption( "-yIndices", "Y indices to be extracted "
                                 "(default=none)",
                                 yIndices );
    application.addSeriesOption( "-zIndices", "Z indices to be extracted "
                                 "(default=none)",
                                 zIndices );
    application.addSeriesOption( "-tIndices", "T indices to be extracted "
                                 "(default=none)",
                                 tIndices );
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

    execute( fileNameIn, fileNameOut, sx, sy, sz, st,
             ex, ey, ez, et, xIndices, yIndices, zIndices, tIndices,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SubVolumeCommand::parse()" );

}


void gkg::SubVolumeCommand::execute( 
                                 const std::string& fileNameIn,
                                 const std::string& fileNameOut, 
                                 int32_t sx, int32_t sy, int32_t sz, int32_t st,
                                 int32_t ex, int32_t ey, int32_t ez, int32_t et,
                                 const std::list< int32_t >& xIndices,
                                 const std::list< int32_t >& yIndices,
                                 const std::list< int32_t >& zIndices,
                                 const std::list< int32_t >& tIndices,
                                 bool ascii,
                                 const std::string& format,
                                 bool verbose )
{

  try
  {

    //
    // launching sub-volume process
    //
    gkg::SubVolumeProcess subVolumeProcess(
                                       fileNameOut,
                                       sx, sy, sz, st,
                                       ex, ey, ez, et,
                                       xIndices, yIndices, zIndices, tIndices,
                                       ascii,
                                       format,
                                       verbose );

    subVolumeProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::SubVolumeCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, int32_t ez, int32_t et, "
             "const std::list< int32_t >& xIndices, "
             "const std::list< int32_t >& yIndices, "
             "const std::list< int32_t >& zIndices, "
             "const std::list< int32_t >& tIndices, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( SubVolumeCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( sx ) +
    			DECLARE_INTEGER_PARAMETER_HELP( sy ) +
    			DECLARE_INTEGER_PARAMETER_HELP( sz ) +
    			DECLARE_INTEGER_PARAMETER_HELP( st ) +
    			DECLARE_INTEGER_PARAMETER_HELP( ex ) +
    			DECLARE_INTEGER_PARAMETER_HELP( ey ) +
    			DECLARE_INTEGER_PARAMETER_HELP( ez ) +
    			DECLARE_INTEGER_PARAMETER_HELP( et ) +
    			DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( xIndices ) +
    			DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( yIndices ) +
    			DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( zIndices ) +
    			DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( tIndices ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
