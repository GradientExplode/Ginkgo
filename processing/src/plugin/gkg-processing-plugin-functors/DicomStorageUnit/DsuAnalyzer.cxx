#include <gkg-processing-plugin-functors/DicomStorageUnit/DsuAnalyzer.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/DicomFile.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <cstdio>


//
// class DsuSeries
//

gkg::DsuSeries::DsuSeries( const std::string& institutionName,
                     	   const std::string& manufacturersModelName,
                     	   const std::string& date,
                     	   const std::string& patientId,
                     	   int32_t studyId,
                     	   int32_t seriesId,
                     	   gkg::DsuSeries::Type type )
              : _institutionName( institutionName ),
              	_manufacturersModelName( manufacturersModelName ),
              	_date( date ),
              	_patientId( patientId ),
              	_studyId( studyId ),
              	_seriesId( seriesId ),
              	_type( type )
{
}


gkg::DsuSeries::DsuSeries( const gkg::DsuSeries& other )
              : _institutionName( other._institutionName ),
                _manufacturersModelName( other._manufacturersModelName ),
                _date( other._date ),
                _patientId( other._patientId ),
                _studyId( other._studyId ),
                _seriesId( other._seriesId ),
                _type( other._type ),
                _lutImageNumber( other._lutImageNumber ),
                _lutOriginFileName( other._lutOriginFileName ),
                _originFileNames( other._originFileNames )
{
}


gkg::DsuSeries::~DsuSeries()
{
}


const std::string& gkg::DsuSeries::getInstitutionName() const
{

  return _institutionName;

}


const std::string& gkg::DsuSeries::getManufacturersModelName() const
{

  return _manufacturersModelName;

}


const std::string& gkg::DsuSeries::getDate() const
{

  return _date;

}


const std::string& gkg::DsuSeries::getPatientId() const
{

  return _patientId;

}


int32_t gkg::DsuSeries::getStudyId() const
{

  return _studyId;

}


int32_t gkg::DsuSeries::getDsuSeriesId() const
{

  return _seriesId;

}


std::string gkg::DsuSeries::getExamString() const
{

  std::string
    studyIdString = gkg::StringConverter::toString( getStudyId() );
  studyIdString = std::string( "000" ).substr( 0,
                                               3U - studyIdString.length() ) +
                  studyIdString;

  return _patientId + '_' + studyIdString;

}


gkg::DsuSeries::Type gkg::DsuSeries::getType() const
{

  return _type;

}


std::string gkg::DsuSeries::getOriginName( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }
    return _lutOriginFileName[ index ];

  }
  GKG_CATCH( "std::string gkg::DsuSeries::getOriginName( int index ) const" );

}


std::string gkg::DsuSeries::getDestinationName( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }
    char buffer[ 256 ];
    switch ( _type )
    {

      case gkg::DsuSeries::DICOM:

        sprintf( buffer, "patient%s"
                         "_study%06" PRId32
                         "_series%06" PRId32
                         "_image%06" PRId32
                         ".MRDC",
                 _patientId.c_str(),
                 _studyId,
                 _seriesId,
                 _lutImageNumber[ index ] );
        break;

      case gkg::DsuSeries::UNKNOWN:

        throw std::runtime_error( "not a good series type" );

    }
    return std::string( buffer );

  }
  GKG_CATCH( "std::string gkg::DsuSeries::getDestinationName( "
             "int index ) const" );

}


int32_t gkg::DsuSeries::getCount() const
{

  try
  {

    return ( int32_t )_originFileNames.size();

  }
  GKG_CATCH( "int gkg::DsuSeries::getCount() const" );

}


void gkg::DsuSeries::addImage( int32_t imageNumber,
                               const std::string& originFileName )
{

  try
  {

    _originFileNames[ imageNumber ] = originFileName;

  }
  GKG_CATCH( "void gkg::DsuSeries::addImage( int imageNumber, "
             "const std::string& originFileName )" );

}


bool gkg::DsuSeries::check( const std::string& institutionName,
                            const std::string& manufacturersModelName,
                            const std::string& date,
                            const std::string& patientId,
                            int32_t studyId,
                            int32_t seriesId ) const
{

  return ( _institutionName == institutionName ) &&
         ( _manufacturersModelName == manufacturersModelName ) &&
         ( _date == date ) &&
         ( _patientId == patientId ) &&
         ( _studyId == studyId ) &&
         ( _seriesId == seriesId );

}


void gkg::DsuSeries::updateLut()
{

  try
  {

    std::map< int32_t, std::string >::const_iterator 
      o = _originFileNames.begin(),
      oe = _originFileNames.end();
    while ( o != oe )
    {

      _lutImageNumber.push_back( o->first );
      _lutOriginFileName.push_back( o->second );
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::DsuSeries::updateLut()" );

}


//
// class DsuAnalyzer
//

gkg::DsuAnalyzer::DsuAnalyzer( const std::string& directory,
                               const std::string& defaultInstitutionName )
                : _directory( directory ),
	          _defaultInstitutionName( defaultInstitutionName )
{

  analyzeSelection();

}


gkg::DsuAnalyzer::~DsuAnalyzer()
{
}


const std::string& gkg::DsuAnalyzer::getDirectory() const
{

  return _directory;

}


const std::list< gkg::DsuSeries >& gkg::DsuAnalyzer::getSeries() const
{

  return _series;

}


void gkg::DsuAnalyzer::analyzeSelection()
{

  try
  {

    std::list< gkg::DsuSeries >::iterator s, se;

    // collecting image file name(s)
    std::list< std::string > fileNames = getFileNames();

    // building and filling selection serie(s) with previous file name(s)
    std::list< std::string >::const_iterator f = fileNames.begin(),
                                             fe = fileNames.end();
    while ( f != fe )
    {

      std::string institutionName;
      std::string manufacturersModelName;
      std::string date;
      std::string patientId;
      int32_t studyId;
      int32_t seriesId;
      int32_t imageNumber;
      gkg::DsuSeries::Type
        type = analyzeFile( *f,
                            institutionName,
                            manufacturersModelName,
                            date,
                            patientId,
                            studyId,
                            seriesId,
                            imageNumber );

      bool found = false;
      s = _series.begin(),
      se = _series.end();
      while ( s != se )
      {

        if ( s->check( institutionName,
                       manufacturersModelName,
                       date,
                       patientId,
                       studyId,
                       seriesId ) )
        {

          found = true;
          break;

        }
        ++ s;

      }
      if ( found )
      {

        s->addImage( imageNumber, *f );

      }
      else
      {

        gkg::DsuSeries series( institutionName,
                               manufacturersModelName,
                               date,
                               patientId,
                               studyId,
                               seriesId,
                               type );
        series.addImage( imageNumber, *f );
        _series.push_back( series );

      }
      ++ f;

    }

    // updating series look-up table(s)
    s = _series.begin(),
    se = _series.end();
    while ( s != se )
    {

      s->updateLut();
      ++ s;

    }

  }
  GKG_CATCH( "std::list< gkg::DsuSeries > "
             "gkg::DsuAnalyzer::analyzeSelection() const" );

}


std::list< std::string > gkg::DsuAnalyzer::getFileNames() const
{

  try
  {

    // assuming we are in the concerned directory
    gkg::Directory directory( _directory );
    if ( !directory.isValid() )
    {

      throw std::runtime_error( _directory + " directory not found" );

    }

    int32_t index;
    std::string name;
    std::list< std::string > names;
    for ( index = 0; index < directory.getCount(); index++ )
    {

      if ( !directory.isDirectory( index ) )
      {

        name = directory.getPath() +
               gkg::getDirectorySeparator() +
               directory.getName( index );

        resolveLinkOrDirectory( name, names );

      }

    }

    return names;

  }
  GKG_CATCH( "std::list< std::string > "
             "gkg::DsuAnalyzer::getFileNames() const" );

}


void gkg::DsuAnalyzer::resolveLinkOrDirectory(
                                         const std::string& name,
                                         std::list< std::string >& names ) const
{

  try
  {

    struct stat statistics;
    std::string realName;

    // getting file information(s)
    lstat( name.c_str(), &statistics );

    if ( S_ISLNK( statistics.st_mode ) )
    {

      char buffer[ 1024 ];
      int32_t count = readlink( name.c_str(), buffer, 1024U );
      buffer[ count ] = '\0';
      resolveLinkOrDirectory( std::string( buffer ), names );

    }
    else if ( S_ISDIR( statistics.st_mode ) )
    {

      DIR* directory = ::opendir( name.c_str() );
      if ( !directory )
      {

        throw std::runtime_error( std::string( "wrong directory '" ) +
                                  name + "'" );

      }
      struct dirent* item;
      while ( ( item = ::readdir( directory ) ) )
      {

        std::string childName = std::string( item->d_name );
        if ( ( childName != "." ) && ( childName != ".." ) )
        {

          resolveLinkOrDirectory( name + '/' + childName, names );

        }

      }

    }
    else
    {

      names.push_back( name );

    }

  }
  GKG_CATCH( "std::string gkg::DsuAnalyzer::resolveLinkOrDirectory( "
             "const std::string& name ) const" );

}


gkg::DsuSeries::Type 
gkg::DsuAnalyzer::analyzeFile( const std::string& fileName,
                               std::string& institutionName,
                               std::string& manufacturersModelName,
                               std::string& date,
                               std::string& patientId,
                               int32_t& studyId,
                               int32_t& seriesId,
                               int32_t& imageNumber ) const
{

  try
  {

    std::ifstream is( fileName.c_str() );
    if ( !is )
    {

      throw std::runtime_error( std::string( "unable to open file '" ) +
                                fileName + "'" );

    }
    is.close();

    gkg::DsuSeries::Type type = gkg::DsuSeries::UNKNOWN;

    analyzeDicomFile( fileName,
                      institutionName,
                      manufacturersModelName,
                      date,
                      patientId,
                      studyId,
                      seriesId,
                      imageNumber );
    type = gkg::DsuSeries::DICOM;

    return type;

  }
  GKG_CATCH( "gkg::DsuSeries::Type gkg::DsuAnalyzer::analyzeFile( "
             "const std::string& fileName, "
             "std::string& institutionName, "
             "std::string& manufacturersModelName, "
             "std::string& date, "
             "std::string& patientId, "
             "int32_t& studyId, "
             "int32_t& seriesId, "
             "int32_t& imageNumber ) const" );

}


void gkg::DsuAnalyzer::analyzeDicomFile( const std::string& fileName,
                                      	 std::string& institutionName,
                                      	 std::string& manufacturersModelName,
                                      	 std::string& date,
                                      	 std::string& patientId,
                                      	 int32_t& studyId,
                                      	 int32_t& seriesId,
                                      	 int32_t& imageNumber ) const
{

  try
  {

    gkg::DicomFile dicomFile( fileName );

    try
    {
    
      institutionName = dicomFile.getInstitutionName();
      
    }
    catch ( std::exception& )
    {
    
      institutionName = _defaultInstitutionName;

    }
    manufacturersModelName = dicomFile.getManufacturersModelName();
    date = dicomFile.getDate();
    patientId = dicomFile.getPatientID();
    studyId = dicomFile.getStudyID();
    seriesId = dicomFile.getSeriesNumber();
    imageNumber = dicomFile.getInstanceNumber();

  }
  GKG_CATCH( "void gkg::DsuAnalyzer::analyzeGenesisFile( "
             "const std::string& fileName, "
             "std::string& institutionName, "
             "std::string& manufacturersModelName, "
             "std::string& date, "
             "std::string& patientId, "
             "int32_t& studyId, "
             "int32_t& seriesId, "
             "int32_t& imageNumber ) const" );

}


//
// stream function
//


std::ostream& std::operator << ( std::ostream& os,
                                 const gkg::DsuSeries& thing )
{

  os << "institutionName: " << thing.getInstitutionName() << " "
     << "manufacturersModelName: " << thing.getManufacturersModelName() << " "
     << "date: " << thing.getDate() << " "
     << "patient ID: " << thing.getPatientId() << " "
     << "study ID: " << thing.getStudyId() << " "
     << "series ID: " << thing.getDsuSeriesId() << std::endl;

  int32_t index;
  for ( index = 0; index < thing.getCount(); index++ )
  {

    os << thing.getOriginName( index )
       << " -> "
       << thing.getDestinationName( index )
       << std::endl;
       
  }
  return os;

}
