#include <gkg-communication-log/HistoryWriter.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>

#include <ctime>
#include <sstream>
#include <fstream>


gkg::HistoryWriter::HistoryWriter()
                  : gkg::Singleton< HistoryWriter >(),
                    gkg::InfoWriter( "history" ),
                    _first( true ),
                    _active( true )
{
}


void gkg::HistoryWriter::initialize( const std::string& applicationName )
{

  try
  {

    if ( !applicationName.empty() )
    {

      gkg::InfoWriter::initialize( applicationName );

      clearOldDataDirectories();

      _dataDirectoryName = _directoryName + "data" + _timeString +
                           gkg::getDirectorySeparator();

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::initialize( "
             "const std::string& applicationName )" );

}


void gkg::HistoryWriter::clearOldDataDirectories()
{

  try
  {

    gkg::Directory directory( _directoryName );

    if ( directory.isValid() )
    {

      std::set< std::string > subDirectories = directory.getSubDirectories();

      if ( !subDirectories.empty() )
      {

        time_t localTime = time( 0 );
        std::set< std::string >::const_iterator
          d = subDirectories.begin(),
          de = subDirectories.end();

        // parse sub-directories
        while ( d != de )
        {

          size_t pos = d->find( "data" );

          if ( pos != std::string::npos )
          {

            time_t directoryTime;
            std::stringstream timeSStream( d->substr( pos + 4 ) );
            timeSStream >> directoryTime;

            // remove directory if older than one month
            if ( ( localTime - directoryTime ) > 2600000.0 )
            {

              gkg::Directory dataDirectory( _directoryName + *d );
              dataDirectory.remove();

            }

          }

          ++d;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::clearOldDataDirectories( "
             "const std::string& directoryName )" );

}


void gkg::HistoryWriter::activate( bool active )
{

  _active = active;

}


bool gkg::HistoryWriter::isActive()
{

  return _active;

}


void gkg::HistoryWriter::add( const std::string& text  )
{

  try
  {

    if ( _active && !_fileName.empty() )
    {

      checkAndCreateDirectory();

      std::ofstream fs( _fileName.c_str(), std::ios_base::app );

      if ( fs.good() )
      {

        addDate( fs );
        fs << text << std::endl;
        fs.close();

      }

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::add( const std::string& text, "
             "T value, bool newLine )" );

}


void gkg::HistoryWriter::add( const std::string& text,
                              const std::string& fileName, 
                              char* data, 
                              size_t size )
{

  try
  {
    
    if ( _active && !_fileName.empty() && 
         !_dataDirectoryName.empty() && !fileName.empty() )
    {

      checkAndCreateDirectory();

      gkg::Directory dataDirectory( _dataDirectoryName );
      if ( !dataDirectory.isValid() )
      {

        dataDirectory.mkdir();

      }

      std::string dataName = _dataDirectoryName + fileName;
      std::ofstream ofs( dataName.c_str(), std::ofstream::binary );

      if ( ofs.good() )
      {

        ofs.write( data, size );
        ofs.close();
      
        std::ofstream fs( _fileName.c_str(), std::ios_base::app );

        if ( fs.good() )
        {

          addDate( fs );

          if ( !text.empty() )
          {

            fs << text << " ";

          }

          fs << dataName << std::endl;
          fs.close();

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::add( "
             "const std::string& fileName, char* data, size_t size )" );

}


void gkg::HistoryWriter::addDate( std::ofstream& stream  )
{

  try
  {

    if ( _first && stream.good() )
    {

      time_t t = time( 0 );
      std::string timeStr( ctime( &t ) );
      timeStr = timeStr.substr( 0, timeStr.length() - 1 ) + "\0";
      stream << "##### " << timeStr << " #####" << std::endl << std::endl;
      _first = false;

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::addDate( std::ofstream& stream )" );

}
