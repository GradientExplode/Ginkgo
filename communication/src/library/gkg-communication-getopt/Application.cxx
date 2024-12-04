#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-log/LogWriter.h>
#include <gkg-communication-log/HistoryWriter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cstdlib>


static void drawLine( size_t columns );
static std::string drawFormattedText( const std::string& text,
                                      size_t columns,
                                      size_t indent );


gkg::Application::Application( int32_t argc, char* argv[],
                               const std::string& documentation,
                               bool loadPlugin,
                               bool commandLauncher )
                : gkg::OptionParser( argc, argv, commandLauncher ),
                  _name( argv[ 0 ] ),
                  _documentation( documentation ),
                  _loadPlugin( loadPlugin ),
                  _commandLauncher( commandLauncher )
{

  try
  {

    gkg::LogWriter::getInstance().initialize( _name );
    gkg::HistoryWriter::getInstance().initialize( _name );

  }
  GKG_CATCH( "gkg::Application::Application( int32_t argc, char* argv[], "
             "const std::string& documentation )" );

}


gkg::Application::~Application()
{
}


void gkg::Application::initialize()
{

  // managing arguments
  bool verbosePluginLoading = false;
  addSingleOption( "-verbosePluginLoading",
                   "Verbose plugin loading (default=false)",
                   verbosePluginLoading, true );

  // managing arguments
  bool anonymous = false;
  bool help = false;
  addSingleOption( "-anonymous", "anonymize patient name", anonymous, true );

  if ( !_commandLauncher )
  {

    addSingleOption( "-h", "show help", help, true );

  }
  parse();

  // loading plugins
  if ( _loadPlugin )
  {

    gkg::PluginLoader::getInstance().load( verbosePluginLoading,
                                           _commandLauncher );

  }

  // setting patient name(s) anonymous or not
  gkg::ConfigurationInfo::getInstance().setAnonymousName( anonymous );

  if ( help ||
       ( _commandLauncher && _arguments.empty() ) )
  {

    std::cout << std::endl;
    drawLine( 80 );
    std::cout << _name << std::endl;
    drawLine( 80 );
    std::cout << drawFormattedText( _documentation, 80, 0 ) << std::endl;
    drawLine( 80 );
    std::cout << std::endl;
    std::deque< gkg::Option* >::const_iterator o = _options.begin(),
                                               oe = _options.end();
    while ( o != oe )
    {

      std::cout << drawFormattedText( ( *o )->getInformation(), 80, 4 )
                << std::endl
                << std::endl;
      ++o;

    }
    drawLine( 80 );
    throw gkg::HelpOptionException();

  }
  else
  {

    check();

  }

}


void drawLine( size_t columns )
{

  try
  {

    for ( size_t i = 0; i < columns; i++ )
    {

      std::cout << "-";

    }
    std::cout << std::endl;

  }
  GKG_CATCH( "void drawLine( size_t columns )" );

}


std::string
drawFormattedText( const std::string& text, size_t columns, size_t indent )
{

  try
  {

    size_t position = 0;
    size_t size = text.size();
    size_t p2;
    size_t ccols = columns;
    size_t i;
    size_t skip;
    std::string formatted;
    std::string prefix;
    char c;
    bool first = true;

    while ( position < size )
    {

      skip = 1;
      p2 = text.find( '\n', position );
      if ( p2 == std::string::npos )
      {

        p2 = size;

      }
      if ( p2 - position >= ccols )
      {

        p2 = position + ccols;
        // try to find a cesure
        for ( i = p2 - 1; i > position; i-- )
        {

          c = text[ i ];
          if ( c == ' ' )
          {

            p2 = i;
            break;

          }
          if ( c == ',' )
          {

            p2 = i + 1;
            skip = 0;
            break;

          }

        }
        while ( ( p2 > position ) && ( text[ p2 - 1 ] == ' ' ) )
        {

          p2 --;
          skip ++;

        }

      }
      formatted += prefix + text.substr( position, p2 - position );
      if ( first )
      {

        first = false;
        for ( i = 0; i < indent; i++ )
        {

          prefix += ' ';

        }
        ccols = columns - indent;

      }
      while ( ( p2 + skip < size ) && ( text[ p2 + skip ] == ' ' ) )
      {

        skip ++;

      }
      position = p2 + skip;
      if ( position < size )
      {

        formatted += '\n';

      }

    }
    return formatted;

  }
  GKG_CATCH( "std::string "
             "drawFormattedText( const std::string& text, "
             "size_t columns, size_t indent )" );

}


const std::string& gkg::Application::getName() const
{

  try
  {

    return _name;

  }
  GKG_CATCH( "const std::string& gkg::Application::getName() const" );

}


