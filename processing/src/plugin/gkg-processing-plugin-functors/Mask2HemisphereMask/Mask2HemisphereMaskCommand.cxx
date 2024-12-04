#include <gkg-processing-plugin-functors/Mask2HemisphereMask/Mask2HemisphereMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>



//
//   Mask2HemisphereMaskCommand
//


gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand( int32_t argc,
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
  GKG_CATCH( "gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand(
                                             const std::string& fileNameIn,
                        		     const std::string& fileNameOut,
                                             int32_t middle,
                        		     double background,
                        		     bool ascii,
                        		     const std::string& format,
                        		     bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             middle,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t middle, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, middle );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             middle,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::Mask2HemisphereMaskCommand::Mask2HemisphereMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Mask2HemisphereMaskCommand::~Mask2HemisphereMaskCommand()
{
}


std::string gkg::Mask2HemisphereMaskCommand::getStaticName()
{

  try
  {

    return "Mask2HemisphereMask";

  }
  GKG_CATCH( "std::string gkg::Mask2HemisphereMaskCommand::getStaticName()" );

}


void gkg::Mask2HemisphereMaskCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t middle = -1;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Int16_t mask to int16_t hemisphere mask",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-m",
                                 "Middle coordinate (default=sizeX / 2)",
                                 middle,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
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

    execute( fileNameIn,
             fileNameOut,
             middle,
             background,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Mask2HemisphereMaskCommand::parse()" );

}


void gkg::Mask2HemisphereMaskCommand::execute( const std::string& fileNameIn,
                                               const std::string& fileNameOut,
                                               int32_t middle,
                                               double background,
                                               bool ascii,
                                               const std::string& format,
                                               bool verbose )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // reading data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }

    gkg::Volume< int16_t > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    gkg::Vector3d< int32_t > size;
    volume.getSize( size );

    int32_t middleCoordinate = middle;
    if ( middleCoordinate < 0 )
    {
      middleCoordinate = ( size.x / 2 );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // dividing mask in 2 hemispheres
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "dividing mask in 2 hemispheres : " << std::flush;

    }



    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = 0; z < size.z; z++ )
    {

      for ( y = 0; y < size.y; y++ )
      {

        for ( x = 0; x < size.x; x++ )
        {

          int16_t& value = volume( x, y, z );
          if ( value )
          {

            if ( x < middle )
            {

              value = 1;

            }
            else
            {

              value = 2;

            }

          }
          else
          {

            value = background;

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOut,
                                      volume,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


  }
  GKG_CATCH( "void gkg::Mask2HemisphereMaskCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t middle, "
             "double background, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( Mask2HemisphereMaskCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( middle ) +
    			DECLARE_FLOATING_PARAMETER_HELP( background ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
