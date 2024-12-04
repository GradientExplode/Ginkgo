#include <gkg-processing-plugin-functors/DicomAnalyzer/DicomAnalyzer.h>
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


static std::string replaceSpaceByUnderscore( const std::string& text )
{

  try
  {

    std::string newText = text;
    size_t i;
    for ( i = 0; i < newText.length(); i++ )
    {

      if ( newText[ i ] == ' ' )
      {

        newText[ i ] = '_';

      }

    }
    return newText;

  }
  GKG_CATCH( "static void replaceSpaceByUnderscore( std::string& text )" );

}

//
// class DicomSeries
//

gkg::DicomSeries::DicomSeries( const std::string& institutionName,
                     	       const std::string& manufacturersModelName,
                     	       const std::string& date,
                     	       const std::string& patientId,
                     	       int32_t studyId,
                     	       int32_t seriesId,
                     	       const std::string& seriesDescription,
                     	       gkg::DicomSeries::Type type )
            	: _institutionName( institutionName ),
            	  _manufacturersModelName( manufacturersModelName ),
            	  _date( date ),
            	  _patientId( patientId ),
            	  _studyId( studyId ),
            	  _seriesId( seriesId ),
            	  _seriesDescription( seriesDescription ),
            	  _type( type )
{
}


gkg::DicomSeries::DicomSeries( const gkg::DicomSeries& other )
                : _institutionName( other._institutionName ),
                  _manufacturersModelName( other._manufacturersModelName ),
                  _date( other._date ),
                  _patientId( other._patientId ),
                  _studyId( other._studyId ),
                  _seriesId( other._seriesId ),
                  _seriesDescription( other._seriesDescription ),
                  _type( other._type ),
                  _lutImageNumber( other._lutImageNumber ),
                  _lutOriginFileName( other._lutOriginFileName ),
                  _originFileNames( other._originFileNames )

{
}


gkg::DicomSeries::~DicomSeries()
{
}


const std::string& gkg::DicomSeries::getInstitutionName() const
{

  return _institutionName;

}


const std::string& gkg::DicomSeries::getManufacturersModelName() const
{

  return _manufacturersModelName;

}


const std::string& gkg::DicomSeries::getDate() const
{

  return _date;

}


const std::string& gkg::DicomSeries::getPatientId() const
{

  return _patientId;

}


int32_t gkg::DicomSeries::getStudyId() const
{

  return _studyId;

}


int32_t gkg::DicomSeries::getSeriesId() const
{

  return _seriesId;

}


const std::string& gkg::DicomSeries::getSeriesDescription() const
{

  return _seriesDescription;

}


std::string gkg::DicomSeries::getExamString() const
{

  std::string studyIdString = gkg::StringConverter::toString( getStudyId() );
  studyIdString = std::string( "000" ).substr( 0,
                                               3U - studyIdString.length() ) +
                  studyIdString;

  return _patientId + '_' + studyIdString;

}


gkg::DicomSeries::Type gkg::DicomSeries::getType() const
{

  return _type;

}


std::string gkg::DicomSeries::getOriginName( int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "invalid index" );

    }
    return _lutOriginFileName[ index ];

  }
  GKG_CATCH( "std::string gkg::DicomSeries::getOriginName( int index ) const" );

}


std::string gkg::DicomSeries::getDestinationName( int32_t index ) const
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

      case gkg::DicomSeries::DICOM:

        sprintf( buffer, "patient%s"
                         "_study%06d"
                         "_series%06d"
                         "_image%06d"
                         ".MRDC",
                 _patientId.c_str(),
                 _studyId,
                 _seriesId,
                 _lutImageNumber[ index ] );
        break;

      case gkg::DicomSeries::UNKNOWN:

        throw std::runtime_error( "not a good series type" );

    }
    return std::string( buffer );

  }
  GKG_CATCH( "std::string gkg::DicomSeries::getDestinationName( "
             "int index ) const" );

}


int32_t gkg::DicomSeries::getCount() const
{

  try
  {

    return ( int32_t )_lutImageNumber.size();

  }
  GKG_CATCH( "int gkg::DicomSeries::getCount() const" );

}


void gkg::DicomSeries::addImage( int32_t imageNumber,
                                 const std::string& originFileName )
{

  try
  {

    _originFileNames[ imageNumber ].push_back( originFileName );

  }
  GKG_CATCH( "void gkg::DicomSeries::addImage( int imageNumber, "
             "const std::string& originFileName )" );

}


bool gkg::DicomSeries::check( const std::string& institutionName,
                              const std::string& manufacturersModelName,
                              const std::string& date,
                              const std::string& patientId,
                              int32_t studyId,
                              int32_t seriesId,
			      const std::string& seriesDescription ) const
{

  return ( _institutionName == institutionName ) &&
         ( _manufacturersModelName == manufacturersModelName ) &&
         ( _date == date ) &&
         ( _patientId == patientId ) &&
         ( _studyId == studyId ) &&
         ( _seriesId == seriesId ) &&
         ( _seriesDescription == seriesDescription );

}


void gkg::DicomSeries::updateLut()
{

  try
  {

    std::map< int32_t, std::list< std::string > >::const_iterator 
      o = _originFileNames.begin(),
      oe = _originFileNames.end();
    int32_t count = 0;
    while ( o != oe )
    {

      std::list< std::string >::const_iterator
        n = o->second.begin(),
        ne = o->second.end();
      while ( n != ne )
      {

        _lutImageNumber.push_back( count );
        _lutOriginFileName.push_back( *n );
        ++ n;
        ++ count;

      }
      ++ o;

    }

  }
  GKG_CATCH( "void gkg::DicomSeries::updateLut()" );

}


//
// class DicomAnalyzer
//

gkg::DicomAnalyzer::DicomAnalyzer( const std::string& directory,
                                   const std::string& defaultInstitutionName )
                  : _directory( directory ),
	            _defaultInstitutionName( defaultInstitutionName )
{

  analyzeSelection();

}


gkg::DicomAnalyzer::~DicomAnalyzer()
{
}


const std::string& gkg::DicomAnalyzer::getDirectory() const
{

  return _directory;

}


const std::list< gkg::DicomSeries >& gkg::DicomAnalyzer::getSeries() const
{

  return _series;

}


void gkg::DicomAnalyzer::analyzeSelection()
{

  try
  {

    std::list< gkg::DicomSeries >::iterator s, se;

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
      std::string seriesDescription;
      int32_t imageNumber;
      gkg::DicomSeries::Type
        type = analyzeFile( *f,
                            institutionName,
                            manufacturersModelName,
                            date,
                            patientId,
                            studyId,
                            seriesId,
			    seriesDescription,
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
                       seriesId,
                       seriesDescription ) )
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

        gkg::DicomSeries series( institutionName,
                            	 manufacturersModelName,
                            	 date,
                            	 patientId,
                            	 studyId,
                            	 seriesId,
                            	 seriesDescription,
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
  GKG_CATCH( "std::list< gkg::DicomSeries > "
             "gkg::DicomAnalyzer::analyzeSelection() const" );

}


std::list< std::string > gkg::DicomAnalyzer::getFileNames() const
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
             "gkg::DicomAnalyzer::getFileNames() const" );

}


void gkg::DicomAnalyzer::resolveLinkOrDirectory(
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
  GKG_CATCH( "std::string gkg::DicomAnalyzer::resolveLinkOrDirectory( "
             "const std::string& name ) const" );

}


gkg::DicomSeries::Type 
gkg::DicomAnalyzer::analyzeFile( const std::string& fileName,
                            	 std::string& institutionName,
                            	 std::string& manufacturersModelName,
                            	 std::string& date,
                            	 std::string& patientId,
                            	 int32_t& studyId,
                            	 int32_t& seriesId,
			    	 std::string& seriesDescription,
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

    gkg::DicomSeries::Type type = gkg::DicomSeries::UNKNOWN;

    analyzeDicomFile( fileName,
                      institutionName,
                      manufacturersModelName,
                      date,
                      patientId,
                      studyId,
                      seriesId,
                      seriesDescription,
                      imageNumber );
    type = gkg::DicomSeries::DICOM;

    return type;

  }
  GKG_CATCH( "gkg::DicomSeries::Type gkg::DicomAnalyzer::analyzeFile( "
             "const std::string& fileName, "
             "std::string& institutionName, "
             "std::string& manufacturersModelName, "
             "std::string& date, "
             "std::string& patientId, "
             "int32_t& studyId, "
             "int32_t& seriesId, "
             "std::string& seriesDescription, "
             "int32_t& imageNumber ) const" );

}


void gkg::DicomAnalyzer::analyzeDicomFile( const std::string& fileName,
                                      	   std::string& institutionName,
                                      	   std::string& manufacturersModelName,
                                      	   std::string& date,
                                      	   std::string& patientId,
                                      	   int32_t& studyId,
                                      	   int32_t& seriesId,
                                      	   std::string& seriesDescription,
                                      	   int32_t& imageNumber ) const
{

  try
  {

    gkg::DicomFile dicomFile( fileName );

    try
    {
    
      institutionName = 
                     replaceSpaceByUnderscore( dicomFile.getInstitutionName() );
      
    }
    catch ( std::exception& )
    {
    
      institutionName = replaceSpaceByUnderscore( _defaultInstitutionName );

    }
    manufacturersModelName = 
              replaceSpaceByUnderscore( dicomFile.getManufacturersModelName() );
    date = dicomFile.getDate();
    patientId = replaceSpaceByUnderscore( dicomFile.getPatientID() );
    studyId = dicomFile.getStudyID();
    seriesId = dicomFile.getSeriesNumber();
    seriesDescription = filterCharacters( dicomFile.getSeriesDescription() );
    imageNumber = dicomFile.getInstanceNumber();

  }
  GKG_CATCH( "void gkg::DicomAnalyzer::analyzeGenesisFile( "
             "const std::string& fileName, "
             "std::string& institutionName, "
             "std::string& manufacturersModelName, "
             "std::string& date, "
             "std::string& patientId, "
             "int32_t& studyId, "
             "int32_t& seriesId, "
             "std::string& seriesDescription, "
             "int32_t& imageNumber ) const" );

}


std::string gkg::DicomAnalyzer::filterCharacters( const std::string& name )
{

  try
  {

    std::string nameCopy = name;
    std::string::size_type i;
    for ( i = 0; i < nameCopy.length(); i++ )
    {
    
      if ( ( nameCopy[ i ] == ' ' ) ||
           ( nameCopy[ i ] == '\t' ) ||
           ( nameCopy[ i ] == '&' ) ||
           ( nameCopy[ i ] == '~' ) ||
           ( nameCopy[ i ] == '\"' ) ||
           ( nameCopy[ i ] == '#' ) ||
           ( nameCopy[ i ] == '\'' ) ||
           ( nameCopy[ i ] == '{' ) ||
           ( nameCopy[ i ] == '(' ) ||
           ( nameCopy[ i ] == '[' ) ||
           ( nameCopy[ i ] == '|' ) ||
           ( nameCopy[ i ] == '`' ) ||
           ( nameCopy[ i ] == '\\' ) ||
           ( nameCopy[ i ] == '^' ) ||
           ( nameCopy[ i ] == '@' ) ||
           ( nameCopy[ i ] == ')' ) ||
           ( nameCopy[ i ] == ']' ) ||
           ( nameCopy[ i ] == '=' ) ||
           ( nameCopy[ i ] == '}' ) ||
           ( nameCopy[ i ] == '$' ) ||
           ( nameCopy[ i ] == '%' ) ||
           ( nameCopy[ i ] == '*' ) ||
           ( nameCopy[ i ] == '<' ) ||
           ( nameCopy[ i ] == '>' ) ||
           ( nameCopy[ i ] == '?' ) ||
           ( nameCopy[ i ] == ',' ) ||
           ( nameCopy[ i ] == '.' ) ||
           ( nameCopy[ i ] == '_' ) ||
           ( nameCopy[ i ] == ';' ) ||
           ( nameCopy[ i ] == '/' ) ||
           ( nameCopy[ i ] == ':' ) ||
           ( nameCopy[ i ] == '!' )/* ||
           ( nameCopy[ i ] == '§' ) ||
           ( nameCopy[ i ] == '£' )*/ )
      {

        nameCopy[ i ] = '-';

      }
      // e accent aigu
      if ( ( int32_t )nameCopy[ i ] == -23 )
      {

        nameCopy[ i ] = 'e';

      }
      // e accent grave
      else if ( ( int32_t )nameCopy[ i ] == -24 )
      {

        nameCopy[ i ] = 'e';

      }
      // c cedille
      else if ( ( int32_t )nameCopy[ i ] == -25 )
      {

        nameCopy[ i ] = 'c';

      }
      // a accent grave
      else if ( ( int32_t )nameCopy[ i ] == -32 )
      {

        nameCopy[ i ] = 'a';

      }
      // u accent aigu
      else if ( ( int32_t )nameCopy[ i ] == -7 )
      {

        nameCopy[ i ] = 'u';

      }
      // a accent circonflexe
      else if ( ( int32_t )nameCopy[ i ] == -30 )
      {

        nameCopy[ i ] = 'a';

      }
      // e accent circonflexe
      else if ( ( int32_t )nameCopy[ i ] == -22 )
      {

        nameCopy[ i ] = 'e';

      }
      // i accent circonflexe
      else if ( ( int32_t )nameCopy[ i ] == -18 )
      {

        nameCopy[ i ] = 'i';

      }
      // o accent circonflexe
      else if ( ( int32_t )nameCopy[ i ] == -12 )
      {

        nameCopy[ i ] = 'o';

      }
      // u accent circonflexe
      else if ( ( int32_t )nameCopy[ i ] == -5 )
      {

        nameCopy[ i ] = 'u';

      }
      // a trema
      else if ( ( int32_t )nameCopy[ i ] == -28 )
      {

        nameCopy[ i ] = 'a';

      }
      // e trema
      else if ( ( int32_t )nameCopy[ i ] == -21 )
      {

        nameCopy[ i ] = 'e';

      }
      // i trema
      else if ( ( int32_t )nameCopy[ i ] == -17 )
      {

        nameCopy[ i ] = 'i';

      }
      // o trema
      else if ( ( int32_t )nameCopy[ i ] == -10 )
      {

        nameCopy[ i ] = 'o';

      }
      // u trema
      else if ( ( int32_t )nameCopy[ i ] == -36 )
      {

        nameCopy[ i ] = 'u';

      }
      // y trema
      else if ( ( int32_t )nameCopy[ i ] == -1 )
      {

        nameCopy[ i ] = 'y';

      }

    } 
  
    return nameCopy;  
  
  }
  GKG_CATCH( "std::string gkg::DicomAnalyzer::filterCharacters( "
             "const std::string& name )" );


}


//
// stream function
//

std::ostream& std::operator << ( std::ostream& os,
                                 const gkg::DicomSeries& thing )
{

  os << "institutionName: " << thing.getInstitutionName() << " "
     << "manufacturersModelName: " << thing.getManufacturersModelName() << " "
     << "date: " << thing.getDate() << " "
     << "patient ID: " << thing.getPatientId() << " "
     << "study ID: " << thing.getStudyId() << " "
     << "series ID: " << thing.getSeriesId() << " "
     << "series description: " << thing.getSeriesDescription()
     << std::endl;

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
