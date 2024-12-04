#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-sysinfo/SystemInfo.h>
#include <gkg-core-pattern/RCPointer.h>

#include <string>
#include <list>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    bool listCmd = false;

    gkg::Application application( argc, argv, "Execute Gkg commands",
                                  true, true );
    application.addSingleOption( "-commandList",
                                 "List available commands (default=false)",
                                 listCmd,
                                 true );
    application.initialize();

    if ( listCmd )
    {

      std::vector< std::string > commands = 
                                  gkg::CommandFactory::getInstance().getNames();
      std::vector< std::string >::const_iterator
        c = commands.begin(),
        ce = commands.end();
      int32_t width = gkg::SystemInfo::getInstance().getTerminalColumnCount();
      int32_t length = c->length();
      int32_t maxLength = length;

      ++c;

      while ( c != ce )
      {

        length = c->length();

        if ( length > maxLength )
        {

          maxLength = length;

        }

        ++ c;

      }

      maxLength += 2; // to get at least 2 white spaces

      int32_t cmdCount = (int32_t)commands.size();
      int32_t nPerLine = width / maxLength;

      if ( nPerLine < 1 )
      {

        nPerLine = 1;

      }
 
      int32_t nPerCol = (int32_t)::ceil( float( cmdCount ) / 
                                         float( nPerLine ) );
      int32_t i, j;

      std::cout << std::endl;
      std::cout << "Available commands are : " << std::endl;
      std::cout << std::endl;

      for ( j = 0; j < nPerCol; j++ )
      {

        for ( i = j; i < cmdCount; i += nPerCol )
        {

          std::cout << std::setw( maxLength ) << std::left << commands[ i ];

        }

        std::cout << std::endl;

      }

      std::cout << std::endl;

    }
    else
    {

      gkg::RCPointer< gkg::Command > command( 
                      gkg::CommandFactory::getInstance().create( argc, argv ) );

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
