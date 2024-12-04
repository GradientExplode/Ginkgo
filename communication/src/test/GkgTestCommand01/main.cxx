#include <gkg-communication-command/Command.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class MyCommand : public gkg::Command
{

  public:

    MyCommand( int32_t argc, char* argv[] );

  protected:

    void parse();

};


MyCommand::MyCommand( int32_t argc, char* argv[] )
         : gkg::Command( argc, argv )
{

  parse();

}


void MyCommand::parse()
{

  int32_t i;

  std::cout << _argc << " filtered arguments :" << std::endl;

  for ( i = 0; i < _argc; i++ )
  {

    std::cout << "\t" << _argv[ i ] << std::endl;

  }

}


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    int32_t i;

    std::cout << argc << " input arguments :" << std::endl;

    for ( i = 0; i < argc; i++ )
    {

      std::cout << "\t" << argv[ i ] << std::endl;

    }

    std::cout << std::endl;

    MyCommand command( argc, argv );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
