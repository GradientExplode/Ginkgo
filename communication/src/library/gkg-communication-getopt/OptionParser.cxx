#include <gkg-communication-getopt/OptionParser_i.h>
#include <gkg-core-exception/BadOptionException.h>
#include <gkg-core-exception/UnexpectedOptionException.h>


gkg::OptionParser::OptionParser( int32_t argc, char* argv[],
                                 bool commandLauncher )
                 : _commandLauncher( commandLauncher )
{

  for ( int32_t i = 1; i < argc; i++ )
  {

    _arguments.push_back( std::string( argv[ i ] ) );

  }

}


gkg::OptionParser::~OptionParser()
{

  std::deque< gkg::Option* >::iterator o = _options.begin(),
                                       oe = _options.end();
  while ( o != oe )
  {

    delete *o;
    o ++;

  }
  _options.clear();

}


void gkg::OptionParser::alias( const std::string& from, const std::string& to )
{

  try
  {

    std::deque< gkg::Option* >::iterator o = _options.begin(),
                                         oe = _options.end();
    while ( o != oe )
    {

      if ( ( *o )->belongToNames( from ) )
      {

        break;

      }
      o ++;

    }
    if ( o == _options.end() )
    {

      throw gkg::BadOptionException( from );

    }
    else
    {

      ( *o )->addName( to );

    }

  }
  GKG_CATCH( "void gkg::OptionParser::alias( const std::string& from, "
             "const std::string& to" );

}


void gkg::OptionParser::parse()
{

  try
  {

    gkg::Option* currentOption = 0;

    std::deque< std::string >::const_iterator a = _arguments.begin(),
                                              ae = _arguments.end();
    while ( a != ae )
    {

      // trying to match an option name
      std::deque< gkg::Option* >::const_iterator o = _options.begin(),
                                                 oe = _options.end();
      while ( o != oe )
      {

        if ( ( *o )->recognize( *a ) )
        {

          break;

        }
        o ++;

      }

      // if not an option name
      if ( o == _options.end() )
      {

        if ( !_commandLauncher )
        {

          if ( currentOption )
          {

            if ( currentOption->feed( *a ) )
            {

              a ++;
              continue;

            }
            else
            {

              currentOption = 0;

            }

          }
          o = _options.begin();
          oe = _options.end();
          while ( o != oe )
          {

            if ( ( *o )->feed( *a ) )
            {

              break;

            }
            o ++;

          }
          if ( o == _options.end() )
          {

            throw gkg::UnexpectedOptionException( *a );

          }
          else
          {

            a ++;

          }

        }
        else
        {

          a ++;

        }

      }
      else   // if this an option name
      {

        currentOption = *o;
        a ++;
        if ( a != ae )
        {

          if ( currentOption->feed( *a ) )
          {

            a ++;

          }
          else
          {

            currentOption = 0;

          }

        }

      }

    }

  }
  GKG_CATCH( "void gkg::OptionParser::parse()" );

}


void gkg::OptionParser::check()
{

  std::deque< gkg::Option* >::const_iterator o = _options.begin(),
                                             oe = _options.end();
  while ( o != oe )
  {

    ( *o )->check();
    o ++;

  }

}
