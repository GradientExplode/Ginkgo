#include <gkg-processing-plugin-functors/RandomIndexGenerator/RandomIndexGeneratorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand(
                                                              int32_t argc,
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
  GKG_CATCH( "gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand( int32_t start,
                                                               int32_t size )
                                : gkg::Command()
{

  try
  {

    execute( start, size );

  }
  GKG_CATCH( "gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand( "
             "int32_t start, int32_t size )" );

}


gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_INTEGER_PARAMETER( parameters, int32_t, start );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, size );
    
    execute( start, size );

  }
  GKG_CATCH( "gkg::RandomIndexGeneratorCommand::RandomIndexGeneratorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RandomIndexGeneratorCommand::~RandomIndexGeneratorCommand()
{
}


std::string gkg::RandomIndexGeneratorCommand::getStaticName()
{

  try
  {

    return "RandomIndexGenerator";

  }
  GKG_CATCH( "std::string gkg::RandomIndexGeneratorCommand::getStaticName()" );

}


void gkg::RandomIndexGeneratorCommand::parse()
{

  try
  {

    int32_t start;
    int32_t size;

    gkg::Application application( _argc, _argv,
                                  "Generate a vector of random indices "
                                  "between 'start' and 'start+size'",
                                  _loadPlugin );
    application.addSingleOption( "-start",
                                 "Start integer value",
                                 start );
    application.addSingleOption( "-size",
                                 "Size of the vector",
                                 size );

    application.initialize();

    execute( start, size );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RandomIndexGeneratorCommand::parse()" );

}


void gkg::RandomIndexGeneratorCommand::execute( int32_t start, int32_t size )
{

  try
  {

    if ( size <= 0 )
    {

      throw std::runtime_error( "size must be strictly positive" );

    }

    std::vector< int32_t > indices( size );

    gkg::getRandomIndexVector( indices );

    int32_t i = 0;
    for ( i = 0; i < size; i++ )
    {

      std::cout << start + indices[ i ] << std::endl;

    }

  }
  GKG_CATCH( "void gkg::RandomIndexGeneratorCommand::execute( "
             "int32_t start, int32_t size )" );

}


RegisterCommandCreator( RandomIndexGeneratorCommand,
                        DECLARE_INTEGER_PARAMETER_HELP( start ) +
                        DECLARE_INTEGER_PARAMETER_HELP( size ) );
