#include <gkg-processing-plugin-functors/EnlargeVolume/EnlargeVolumeCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/EnlargeVolumeProcess.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::EnlargeVolumeCommand::EnlargeVolumeCommand( int32_t argc,
                                                 char* argv[],
                                                 bool loadPlugin,
                                                 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::EnlargeVolumeCommand::EnlargeVolumeCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::EnlargeVolumeCommand::EnlargeVolumeCommand(
                                 const std::string& fileNameIn,
                                 const std::string& fileNameOut, 
                                 int32_t sx, int32_t sy, int32_t sz, int32_t st,
                                 int32_t ex, int32_t ey, int32_t ez, int32_t et,
                                 bool ascii,
                                 const std::string& format,
                                 bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, sx, sy, sz, st,
             ex, ey, ez, et, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::EnlargeVolumeCommand::EnlargeVolumeCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, int32_t ez, int32_t et, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::EnlargeVolumeCommand::EnlargeVolumeCommand(
                                             const gkg::Dictionary& parameters )
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
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, sx, sy, sz, st,
             ex, ey, ez, et, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::EnlargeVolumeCommand::EnlargeVolumeCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::EnlargeVolumeCommand::~EnlargeVolumeCommand()
{
}


std::string gkg::EnlargeVolumeCommand::getStaticName()
{

  try
  {

    return "EnlargeVolume";

  }
  GKG_CATCH( "std::string gkg::EnlargeVolumeCommand::getStaticName()" );

}


void gkg::EnlargeVolumeCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t sx = 0, sy = 0, sz = 0, st = 0;
    int32_t ex = 0, ey = 0, ez = 0, et = 0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv, 
                                  "Add slices to a volume",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-x", "Number of sagittal slices to add at "
                                 "the beginning (default 0)",
                                 sx, true );
    application.addSingleOption( "-X", "Number of sagittal slices to add at "
                                 "the end (default 0)",
                                 ex, true );
    application.addSingleOption( "-y", "Number of coronal slices to add at "
                                 "the beginning (default 0)",
                                 sy, true );
    application.addSingleOption( "-Y", "Number of coronal slices to add at "
                                 "the end (default 0)",
                                 ey, true );
    application.addSingleOption( "-z", "Number of axial slices to add at "
                                 "the beginning (default 0)",
                                 sz, true );
    application.addSingleOption( "-Z", "Number of axial slices to add at "
                                 "the end (default 0)",
                                 ez, true );
    application.addSingleOption( "-t", "Number of frames to add at the "
                                 "beginning (default 0)",
                                 st, true );
    application.addSingleOption( "-T", "Number of frames to add at the end "
                                 "(default 0)",
                                 et, true );
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
             ex, ey, ez, et, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::EnlargeVolumeCommand::parse()" );

}


void gkg::EnlargeVolumeCommand::execute( 
                                 const std::string& fileNameIn,
                                 const std::string& fileNameOut, 
                                 int32_t sx, int32_t sy, int32_t sz, int32_t st,
                                 int32_t ex, int32_t ey, int32_t ez, int32_t et,
                                 bool ascii,
                                 const std::string& format,
                                 bool verbose )
{

  try
  {

    //
    // launching enlarge-volume process
    //
    gkg::EnlargeVolumeProcess enlargeVolumeProcess( fileNameOut,
                                                    sx, sy, sz, st,
                                                    ex, ey, ez, et,
                                                    ascii,
                                                    format,
                                                    verbose );

    enlargeVolumeProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::EnlargeVolumeCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t sx, int32_t sy, int32_t sz, int32_t st, "
             "int32_t ex, int32_t ey, int32_t ez, int32_t et, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( EnlargeVolumeCommand,
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
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
