#include <gkg-communication-log/LogWriter.h>
#include <gkg-core-exception/Exception.h>

#include <ctime>


gkg::LogWriter::LogWriter()
              : gkg::Singleton< LogWriter >(),
                gkg::InfoWriter( "log" )
{
}


void gkg::LogWriter::add( const std::string& text )
{

  try
  {

    if ( !_fileName.empty() )
    {

      checkAndCreateDirectory();

      std::ofstream fs( _fileName.c_str(), std::ios_base::app );

      if ( fs.good() )
      {

        time_t t = time( 0 );
        std::string timeStr( ctime( &t ) );
        timeStr = timeStr.substr( 0, timeStr.length() - 1 ) + "\0";
        fs << timeStr << " : " << text << std::endl;
        fs.close();

      }

    }

  }
  GKG_CATCH( "void gkg::LogWriter::add( const std::string& text )" );

}
