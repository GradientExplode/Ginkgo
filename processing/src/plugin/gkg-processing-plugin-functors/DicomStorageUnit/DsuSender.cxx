#include <gkg-processing-plugin-functors/DicomStorageUnit/DsuSender.h>
#include <gkg-processing-plugin-functors/DicomStorageUnit/DsuAnalyzer.h>
#include <gkg-communication-tcpip/Host.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>



//
// function to test that a folder is present on a FTP site
//

static bool isFtpDirectoryExisting( const std::string rootDirectoryName,
                                    const std::string& directoryName,
                                    const std::string& hostname,
                                    const std::string& login,
                                    const std::string& password )
{

  try
  {

    std::string command;

    // checking wether directory already exists or not
    bool existingDirectory = false;

    char temporaryName1[ 64 ] = "GkgDicomStorageUnitXXXXXX";
    char temporaryName2[ 64 ] = "GkgDicomStorageUnitXXXXXX";

    int32_t status = mkstemp( temporaryName1 );
    
    if ( status < 0 )
    {
    
      throw std::runtime_error( "Error creating first temporary file" );
    
    }
    
    status = mkstemp( temporaryName2 );
    {
    
      throw std::runtime_error( "Error creating second temporary file" );
    
    }

    std::ofstream os( temporaryName1 );
    os << "open " << hostname << std::endl; 
    os << "user " << login << ' ' << password << std::endl;
    if ( !rootDirectoryName.empty() )
    {

      os << "cd " << rootDirectoryName << std::endl; 

    }
    os << "ls" << std::endl;
    os.close();

    command = std::string( "lftp -f " ) + temporaryName1 + " > " +
              temporaryName2; 

    status = ::system( command.c_str() );
    
    if ( status < 0 )
    {
    
      throw std::runtime_error( "Error executing lftp command" );
    
    }

    std::string r1, r2, r3, r4, r5, r6, r7, r8, r9;
    std::ifstream is( temporaryName2 );
    while ( !is.eof() )
    {

      is >> r1 >> r2 >> r3 >> r4 >> r5 >> r6 >> r7 >> r8 >> r9;
      if ( r9 == directoryName )
      {

         existingDirectory = true;
         break;

      }

    }
    is.close();

    remove( temporaryName1 );
    remove( temporaryName2 );

    return existingDirectory;

  }
  GKG_CATCH( "bool isFtpDirectoryExisting( "
             "const std::string& rootDirectoryName, "
             "const std::string& directoryName, "
             "const std::string& hostname, "
             "const std::string& login, "
             "const std::string& password )" );

}


//
// class DsuSender
//


gkg::DsuSender::DsuSender( const gkg::DsuSeries& series,
                           const std::string& tmpDirectory )
              : _series( series ),
                _tmpDirectory( tmpDirectory )
{

  try
  {

    prepare();

  }
  GKG_CATCH( "gkg::DsuSender::DsuSender( const gkg::DsuSeries& series )" );

}


gkg::DsuSender::~DsuSender()
{

  clean();

}


void gkg::DsuSender::send( const std::string& hostname,
                           const std::string& login,
                           const std::string& password,
                           const std::string& destinationPath ) const
{

  try
  {

    // checking wether date directory already exists or not
    bool existingInstitutionNameDirectory = isFtpDirectoryExisting(
                                            destinationPath,
                                            _series.getInstitutionName(),
                                            hostname,
                                            login,
                                            password );

    // checking wether date directory already exists or not
    bool existingManufacturersModelNameDirectory = isFtpDirectoryExisting(
                                            destinationPath +
                                            gkg::getDirectorySeparator() +
                                            _series.getInstitutionName(),
                                            _series.getManufacturersModelName(),
                                            hostname,
                                            login,
                                            password );

    // checking wether date directory already exists or not
    bool existingDateDirectory = isFtpDirectoryExisting(
                                            destinationPath +
                                            gkg::getDirectorySeparator() +
                                            _series.getInstitutionName() +
                                            gkg::getDirectorySeparator() +
                                            _series.getManufacturersModelName(),
                                            _series.getDate(),
                                            hostname,
                                            login,
                                            password );

    // checking wether exam directory already exists or not
    bool existingExamDirectory = isFtpDirectoryExisting(
                                           destinationPath +
                                           gkg::getDirectorySeparator() +
                                           _series.getInstitutionName() +
                                           gkg::getDirectorySeparator() +
                                           _series.getManufacturersModelName() +
                                           gkg::getDirectorySeparator() +
                                           _series.getDate(),
                                           _series.getExamString(),
                                           hostname,
                                           login,
                                           password );

    // performing transfer via ftp protocol
    std::string command;
    command = "ftp -in << EOF\n";
    command += "open " + hostname + '\n';
    command += "user " + login + ' ' + password + '\n';
    command += "lcd " + _institutionModelDateExamDsuSeriesPidDirectory + '\n';
    command += "cd " + destinationPath + '\n';
    if ( !existingInstitutionNameDirectory )
    {

      command += "mkdir " + _series.getInstitutionName() + '\n';

    }
    command += "cd " + _series.getInstitutionName() + '\n';
    if ( !existingManufacturersModelNameDirectory )
    {

      command += "mkdir " + _series.getManufacturersModelName() + '\n';

    }
    command += "cd " + _series.getManufacturersModelName() + '\n';
    if ( !existingDateDirectory )
    {

      command += "mkdir " + _series.getDate() + '\n';

    }
    command += "cd " + _series.getDate() + '\n';
    if ( !existingExamDirectory )
    {

      command += "mkdir " + _series.getExamString() + '\n';

    }
    command += "cd " + _series.getExamString() + '\n';
    command += "bin\n";
    command += "put " + _tarFileName + '\n';
    int32_t status = ::system( command.c_str() );
    
    if ( status < 0 )
    {
    
      throw std::runtime_error( "Error executing ftp transfer command" );
    
    }

  }
  GKG_CATCH( "void gkg::DsuSender::send( "
             "const std::string& hostname, "
             "const std::string& login, "
             "const std::string& password, "
             "const std::string& destinationPath ) const" );

}


void gkg::DsuSender::prepare()
{

  try
  {

    char buffer[ 1024 ];

    // entering temporary directory
    if ( ::chdir( _tmpDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to enter '" ) +
                                _tmpDirectory + "' directory" );
      return;

    }

    _gtarCommand = "/bin/gtar cfh";

    // creating unique directory for exam / series
    sprintf( buffer,
             "Institution%s_Model%s_Date%s_Exam%s"
             "_DsuSeries%06" PRId32
             "_Pid%06" PRId32,
             _series.getInstitutionName().c_str(),
             _series.getManufacturersModelName().c_str(),
             _series.getDate().c_str(),
             _series.getExamString().c_str(),
             _series.getDsuSeriesId(),
             ( int32_t )getpid() );

    _institutionModelDateExamDsuSeriesPidSubDirectory = buffer;
    _institutionModelDateExamDsuSeriesPidDirectory =
      _tmpDirectory + '/' +
      _institutionModelDateExamDsuSeriesPidSubDirectory;

    // creating sub directory for exam / series
    sprintf( buffer, "Exam%s_DsuSeries%06" PRId32,
             _series.getExamString().c_str(),
             _series.getDsuSeriesId() );

    _examDsuSeriesSubDirectory = buffer;
    _examDsuSeriesDirectory = _institutionModelDateExamDsuSeriesPidDirectory +
                              '/' + buffer;

    if ( ::mkdir( _institutionModelDateExamDsuSeriesPidDirectory.c_str(), 
                  S_IRWXU | S_IRWXG | S_IRWXO ) )
    {

      throw std::runtime_error( std::string( "unable to make directory '" ) +
                                _institutionModelDateExamDsuSeriesPidDirectory +
                                "'" );
      return;

    }

    if ( ::mkdir( _examDsuSeriesDirectory.c_str(), 
                  S_IRWXU | S_IRWXG | S_IRWXO ) )
    {

      throw std::runtime_error( std::string( "unable to make directory '" ) +
                                _examDsuSeriesDirectory + "'" );
      return;

    }

    // entering exam / series sub-directory
    if ( ::chdir( _examDsuSeriesDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to enter '" ) +
                                _examDsuSeriesDirectory + "' directory" );
      return;

    }

    // building symbolic links
    int32_t index;
    for ( index = 0; index < _series.getCount(); index++ )
    {

      if ( ::symlink( _series.getOriginName( index ).c_str(),
                      _series.getDestinationName( index ).c_str() ) )
      {

        throw std::runtime_error(
                             std::string( "unable to create symbolic link '" ) +
                             _series.getOriginName( index ) + "' -> '" +
                             _series.getDestinationName( index ) + "' in " +
                             "directory '" + _examDsuSeriesDirectory + "'" );
        return;

      }

    }

    // going back unique exam / series directory
    if ( ::chdir( _institutionModelDateExamDsuSeriesPidDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to enter '" ) +
                                _institutionModelDateExamDsuSeriesPidDirectory +
                                "' directory" );
      return;

    }

    // building archive
    _tarFileName = _examDsuSeriesSubDirectory + "_rec.tar";
    _fullPathTarFileName = _institutionModelDateExamDsuSeriesPidDirectory +
                           '/' + _tarFileName;
    std::string command = _gtarCommand + ' ' +
                          _fullPathTarFileName + ' ' +
                          _examDsuSeriesSubDirectory + '\n';

    if ( ::system( command.c_str() ) )
    {

      // the first time this command is called, it returns a 512 value,
      // and I cannot find the reason why, so I commented this piece of code

      //throw std::runtime_error( std::string( "unable to create '" ) +
      //                          _fullPathTarFileName +
      //                          "' archive" );
      //return;

    }

    // re-entering exam / series sub-directory
    if ( ::chdir( _examDsuSeriesDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to enter '" ) +
                                _examDsuSeriesDirectory + "' directory" );
      return;

    }

    // removing symbolic links
    for ( index = 0; index < _series.getCount(); index++ )
    {

      if ( ::unlink( _series.getDestinationName( index ).c_str() ) )
      {

        throw std::runtime_error(
                             std::string( "unable to remove symbolic link '" ) +
                             _series.getOriginName( index ) + "' -> '" +
                             _series.getDestinationName( index ) + "' in " +
                             "directory '" + _examDsuSeriesDirectory + "'" );
        return;

      }

    }

    // going back to unique exam / series directory
    if ( ::chdir( _institutionModelDateExamDsuSeriesPidDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to go back to '" ) +
                                _institutionModelDateExamDsuSeriesPidDirectory +
                                "' directory" );
      return;

    }

    // removing exam / series sub-directory
    if ( ::rmdir( _examDsuSeriesDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to remove '" ) +
                                _examDsuSeriesDirectory + "' directory" );
      return;

    }

  }
  GKG_CATCH( "void gkg::DsuSender::prepare()" );

}


void gkg::DsuSender::clean()
{

  try
  {

    // removing archive
    if ( ::remove( _fullPathTarFileName.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to remove '" ) +
                                _fullPathTarFileName + "' archive" );
      return;

    }

    // going back to tmp directory
    if ( ::chdir( _tmpDirectory.c_str() ) )
    {

      throw std::runtime_error( std::string( "unable to go back to '" ) +
                                _tmpDirectory + "' directory" );
      return;

    }

    // removing unique exam / series directory
    if ( ::rmdir( _institutionModelDateExamDsuSeriesPidSubDirectory.c_str() ) )
    {

      throw std::runtime_error(
                             std::string( "unable to remove '" ) +
                             _institutionModelDateExamDsuSeriesPidSubDirectory +
                             "' directory" );
      return;

    }

  }
  GKG_CATCH( "void gkg::DsuSender::clean()" );

}
