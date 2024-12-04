#include <gkg-processing-plugin-functors/ConvexHull/ConvexHullCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>


gkg::ConvexHullCommand::ConvexHullCommand( int32_t argc,
                                 	   char* argv[],
                                 	   bool loadPlugin,
                                 	   bool removeFirst )
                      : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::ConvexHullCommand::ConvexHullCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::ConvexHullCommand::ConvexHullCommand( const std::string& fileNameIn,
                                           const std::string& fileNameOut,
                                           bool addSymmetrical,
                                           bool ascii,
                                           const std::string& format,
                                           bool verbose )
{

  try
  {

    execute( fileNameIn, fileNameOut, addSymmetrical, 
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ConvexHullCommand::ConvexHullCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool addSymmetrical, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::ConvexHullCommand::ConvexHullCommand( const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, addSymmetrical );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, addSymmetrical, 
             ascii, format, verbose );

  }
  GKG_CATCH( "gkg::ConvexHullCommand::ConvexHullCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::ConvexHullCommand::~ConvexHullCommand()
{
}


std::string gkg::ConvexHullCommand::getStaticName()
{

  try
  {

    return "ConvexHull";

  }
  GKG_CATCH( "std::string gkg::ConvexHullCommand::getStaticName()" );

}


void gkg::ConvexHullCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string format = "aimsmesh";
    bool ascii = false;
    bool verbose = false;
    bool addSymmetrical = false;

    gkg::Application application( _argc, _argv,
                                  "Convex hull mesher from point set",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Ouput volume format :\n"
                                 " - meshmap,\n"
                                 " - aimsmesh\n"
                                 "(default=aimsmesh)",
                                 format,
                                 true );
    application.addSingleOption( "-addSymmetrical",
                                 "Added symmetrical points",
                                 addSymmetrical,
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

    execute( fileNameIn, fileNameOut, addSymmetrical, 
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::ConvexHullCommand::parse()" );

}


void gkg::ConvexHullCommand::execute( const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      bool addSymmetrical,
                                      bool ascii,
                                      const std::string& format,
                                      bool verbose )
{

  try
  {

    // reading points
    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    std::vector< gkg::Vector3d< float > > points;

    std::ifstream in( fileNameIn.c_str() );
    if ( ! in )
    {

      throw std::runtime_error( "cannot read input file" );

    }
    int32_t count;
    in >> count;

    if ( addSymmetrical )
    {

      points = std::vector< gkg::Vector3d< float > >( 2 * count );

    }
    else
    {

      points = std::vector< gkg::Vector3d< float > >( count );

    }

    int i;
    for ( i = 0; i < count; i++ )
    {

      in >> points[ i ].x >> points[ i ].y >> points[ i ].z;
      if ( ! in )
      {

        throw std::runtime_error( "problem while reading input file" );

      }

    }
    in.close();

    if ( addSymmetrical )
    {

      for ( i = 0; i < count; i++ )
      {

        points[ i + count ] = -points[ i ];

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // processing convex hull
    if ( verbose )
    {

      std::cout << "processing convex hull : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3 > meshMap;
    gkg::ConvexHull::getInstance().addConvexHull( points, 0, meshMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // wrting result
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, meshMap, ascii, format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::ConvexHullCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "bool addSymmetrical, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( ConvexHullCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( addSymmetrical ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
