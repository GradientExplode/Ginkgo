#include <gkg-graphics-X11-plugin-functors/BitmapEditor/BitmapEditorCommand.h>
#include <gkg-graphics-X11-core/Session.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <map>
#include <cassert>
#include <cstring>
#include <cstdlib>


gkg::BitmapEditorCommand::BitmapEditorCommand( int32_t argc,
                                	       char* argv[],
                                	       bool loadPlugin,
                                	       bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::BitmapEditorCommand::BitmapEditorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BitmapEditorCommand::BitmapEditorCommand()
                        : gkg::Command()
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::BitmapEditorCommand::BitmapEditorCommand()" );

}


gkg::BitmapEditorCommand::BitmapEditorCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::BitmapEditorCommand::BitmapEditorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BitmapEditorCommand::~BitmapEditorCommand()
{
}


std::string gkg::BitmapEditorCommand::getStaticName()
{

  try
  {

    return "BitmapEditor";

  }
  GKG_CATCH( "std::string gkg::BitmapEditorCommand::getStaticName()" );

}


void gkg::BitmapEditorCommand::parse()
{

  try
  {

    execute();

  }
  GKG_CATCH_FUNCTOR( "void gkg::BitmapEditorCommand::parse()" );

}


void gkg::BitmapEditorCommand::execute()
{

  try
  {

    gkg::Session& session = gkg::Session::getInstance();
    session.initialize( "Demo39", _argc, _argv );

    if ( _argc < 3 )
    {

      std::cerr << "Usage: GkgBitmapEditor <input_txt_file>  <output_bm_file>"
    		<< std::endl;
      std::cerr << " width a text file of the form :" << std::endl << std::endl;
      std::cerr << "16" << std::endl;
      std::cerr << "16" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 1 1 1 1 1 1 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 1 1 1 1 1 1 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 1 1 1 1 1 1 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 1 1 1 1 1 1 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      std::cerr << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0" << std::endl;
      ::exit( 1 );

    }

    assert( _argc == 3 );

    std::ifstream is( _argv[ 1 ] );
    std::ofstream os( ( std::string( _argv[ 2 ] ) + ".bm" ).c_str() );

    assert( is );
    assert( os );

    int32_t width, height;

    is >> width;
    is >> height;

    os << "#define " << _argv[ 2 ] << "_width " << width << std::endl;
    os << "#define " << _argv[ 2 ] << "_height " << height << std::endl;
    os << "#define " << _argv[ 2 ] << "_x_hot " << width / 2 << std::endl;
    os << "#define " << _argv[ 2 ] << "_y_hot " << height / 2 << std::endl;
    os << "static char " << _argv[ 2 ] << "_bits[] = {" << std::endl;

    int32_t v1, v2, v3, v4, v5, v6, v7, v8;

    std::map< int32_t, std::string > table;

    table[ 0 ] = "0";
    table[ 1 ] = "1";
    table[ 10 ] = "2";
    table[ 11 ] = "3";
    table[ 100 ] = "4";
    table[ 101 ] = "5";
    table[ 110 ] = "6";
    table[ 111 ] = "7";
    table[ 1000 ] = "8";
    table[ 1001 ] = "9";
    table[ 1010 ] = "A";
    table[ 1011 ] = "B";
    table[ 1100 ] = "C";
    table[ 1101 ] = "D";
    table[ 1110 ] = "E";
    table[ 1111 ] = "F";

    int32_t low, high;

    for ( int32_t y = 0; y < height; y++ )
    {

      for ( int32_t x = 0; x < width / 8; x++ )
      {

    	is >> v1 >> v2 >> v3 >> v4 >> v5 >> v6 >> v7 >> v8;
    	low = v1 + v2 * 10 + v3 * 100 + v4 * 1000;
    	high = v5 + v6 * 10 + v7 * 100 + v8 * 1000;
    	std::cout << "low = " << low << "   high = " << high << std::endl;
    	os << "0x" << table[ high ] << table[ low];
    	if ( !( ( y == height - 1 ) && ( x == width / 8 - 1 ) ) )
    	{

    	  os << ", ";

    	}

      }
      os << std::endl;

    }
    os << "};" << std::endl;

    os.close();
    is.close();

  }
  GKG_CATCH( "void gkg::BitmapEditorCommand::execute()" );

}


RegisterCommandCreator( BitmapEditorCommand,
                        DECLARE_NO_PARAMETER_HELP );
