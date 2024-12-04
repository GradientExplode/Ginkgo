#include <gkg-communication-log/InfoWriter.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>

#include <ctime>
#include <sstream>


gkg::InfoWriter::InfoWriter( const std::string& name )
               : _name( name )
{
}


gkg::InfoWriter::~InfoWriter()
{
}


void gkg::InfoWriter::initialize( const std::string& applicationName )
{

  try
  {

    if ( !applicationName.empty() && !_name.empty() )
    {

      _applicationName = applicationName;
      _directoryName = 
                      gkg::ConfigurationInfo::getInstance().getHiddenGkgPath() +
                      _name + gkg::getDirectorySeparator() +
                      _applicationName + gkg::getDirectorySeparator();

      clearOldFiles();

      time_t t = time( 0 );
      std::stringstream sstime;
      sstime << t;
      _timeString = sstime.str();
      _fileName = _directoryName + _name + _timeString;


    }

  }
  GKG_CATCH( "void gkg::InfoWriter::initialize( "
             "const std::string& applicationName )" );

}


void gkg::InfoWriter::checkAndCreateDirectory()
{

  try
  {

    gkg::Directory directory( _directoryName );

    if ( !directory.isValid() )
    {

      std::string directoryName( 
                     gkg::ConfigurationInfo::getInstance().getHiddenGkgPath() );
      gkg::Directory hiddenDirectory( directoryName );
      if ( !hiddenDirectory.isValid() )
      {

        hiddenDirectory.mkdir();

      }
      
      directoryName += _name + gkg::getDirectorySeparator();
      gkg::Directory infoDirectory( directoryName );
      if ( !infoDirectory.isValid() )
      {

        infoDirectory.mkdir();

      }

      directoryName += _applicationName + gkg::getDirectorySeparator();
      gkg::Directory applicationDirectory( directoryName );
      if ( !applicationDirectory.isValid() )
      {

        applicationDirectory.mkdir();

      }

    }

  }
  GKG_CATCH( "bool gkg::InfoWriter::checkDirectory()" );

}


void gkg::InfoWriter::clearOldFiles()
{

  try
  {

    gkg::Directory directory( _directoryName );

    if ( directory.isValid() )
    {

      std::set< std::string > files = directory.getFiles();

      if ( !files.empty() )
      {

        time_t localTime = time( 0 );
        std::set< std::string >::const_iterator
          f = files.begin(),
          fe = files.end();

        while ( f != fe )
        {

          gkg::File file( _directoryName + *f );
          time_t fileTime = file.getLastModificationTime();

          // remove file if older than one month
          if ( ( localTime - fileTime ) > 2600000.0 )
          {

            file.remove();

          }

          ++f;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::InfoWriter::clearOldFiles()" );

}
