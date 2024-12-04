#include <gkg-processing-plugin-dicom/DcmtkDicomFileImplementation.h>
#include <gkg-core-io/DicomFile.h>
#include <gkg-core-exception/Exception.h>
#include <sstream>

gkg::DcmtkDicomFileImplementation::DcmtkDicomFileImplementation(
                                                   gkg::DicomFile* dicomFile,
                                                   const std::string& fileName )
                                  : gkg::DicomFileImplementation( dicomFile,
                                                                  fileName )
{

  try
  {

#ifdef UID_RawDataStorage

    ::DcmInputFileStream dicomStream( fileName.c_str(), 0U );

#else

    ::DcmFileStream dicomStream( fileName.c_str(), DCM_ReadMode );

#endif

    _dicomFile.transferInit();
    _dicomFile.read( dicomStream, EXS_Unknown, EGL_noChange );
    _dicomFile.transferEnd();

  }
  GKG_CATCH( "gkg::DcmtkDicomFileImplementation::DcmtkDicomFileImplementation( "
             "gkg::DicomFile* dicomFile, const std::string& fileName )" );

}


gkg::DcmtkDicomFileImplementation::~DcmtkDicomFileImplementation()
{
}


std::string gkg::DcmtkDicomFileImplementation::getInstitutionName()
{

  try
  {

    std::string institutionName = "";
    // reading exam number
    if ( _dicomFile.search( DCM_InstitutionName, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0008,0080) InstitutionName tag not found" );

    }
    ::DcmLongString* is = static_cast< ::DcmLongString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad long string" );

    }
    char* longStringChPointer = 0;
    is->getString( longStringChPointer );
    if ( longStringChPointer )
    {

      institutionName = std::string( longStringChPointer );

    }

    return institutionName;

  }
  GKG_CATCH( "std::string "
             "gkg::DcmtkDicomFileImplementation::getInstitutionName()" );

}

std::string gkg::DcmtkDicomFileImplementation::getManufacturersModelName()
{

  try
  {

    std::string manufacturersModelName = "";
    // reading exam number
#if OFFIS_DCMTK_VERSION_NUMBER >= 360
    if ( _dicomFile.search( DCM_ManufacturerModelName, _stack ) != EC_Normal )
#else
    if ( _dicomFile.search( DCM_ManufacturersModelName, _stack ) != EC_Normal )
#endif
    {

      throw std::runtime_error(
                           "(0008,1090) ManufacturersModelName tag not found" );

    }
    ::DcmLongString* is = static_cast< ::DcmLongString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad long string" );

    }
    char* longStringChPointer = 0;
    is->getString( longStringChPointer );
    if ( longStringChPointer )
    {

      manufacturersModelName = std::string( longStringChPointer );

    }

    return manufacturersModelName;

  }
  GKG_CATCH( "std::string "
             "gkg::DcmtkDicomFileImplementation::getInstitutionName()" );

}


std::string gkg::DcmtkDicomFileImplementation::getDate()
{

  try
  {

    std::string dateString = "";
    // reading image number
    if ( _dicomFile.search( DCM_SeriesDate, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0008,0021)SeriesDate tag not found" );

    }
    ::DcmDate* d = static_cast< ::DcmDate* >( _stack.top() );
    if ( !d )
    {

      throw std::runtime_error( "bad date" );

    }
    char* dateChPointer = 0;
    d->getString( dateChPointer );
    if ( dateChPointer )
    {

      dateString = std::string( dateChPointer );

    }

    return dateString;

  }
  GKG_CATCH( "std::string gkg::DcmtkDicomFileImplementation::getDate()" );

}


std::string gkg::DcmtkDicomFileImplementation::getPatientID()
{

  try
  {

    std::string patientId = "";
    // reading exam number
    if ( _dicomFile.search( DCM_PatientID, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0010,0020) PatientID tag not found" );

    }
    ::DcmLongString* is = static_cast< ::DcmLongString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad long string" );

    }
    char* longStringChPointer = 0;
    is->getString( longStringChPointer );
    if ( longStringChPointer )
    {

      patientId = std::string( longStringChPointer );

    }

    return patientId;

  }
  GKG_CATCH( "std::string gkg::DcmtkDicomFileImplementation::getPatientID()" );

}


int32_t gkg::DcmtkDicomFileImplementation::getStudyID()
{

  try
  {

    int32_t examNumber = 0;
    // reading exam number
    if ( _dicomFile.search( DCM_StudyID, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0020,0010) StudyID tag not found" );

    }
    ::DcmShortString* sh = static_cast< ::DcmShortString* >( _stack.top() );
    if ( !sh )
    {

      throw std::runtime_error( "bad short string" );

    }
    char* shortStringChPointer = 0;
    sh->getString( shortStringChPointer );
    if ( shortStringChPointer )
    {

      std::string shortString = std::string( shortStringChPointer );
      std::istringstream iss( shortString );
      iss >> examNumber;

    }

    return examNumber;

  }
  GKG_CATCH( "int32_t gkg::DcmtkDicomFileImplementation::getStudyID()" );

}


int32_t gkg::DcmtkDicomFileImplementation::getSeriesNumber()
{

  try
  {

    int32_t seriesNumber = 0;
    // reading series number
    if ( _dicomFile.search( DCM_SeriesNumber, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0020,0011) SeriesNumber tag not found" );

    }
    ::DcmIntegerString* is = static_cast< ::DcmIntegerString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad integer string" );

    }
    char* integerStringChPointer = 0;
    is->getString( integerStringChPointer );
    if ( integerStringChPointer )
    {

      std::string integerString = std::string( integerStringChPointer );
      std::istringstream iss( integerString );
      iss >> seriesNumber;

    }

    return seriesNumber;

  }
  GKG_CATCH( "int32_t gkg::DcmtkDicomFileImplementation::getSeriesNumber()" );

}


int32_t gkg::DcmtkDicomFileImplementation::getInstanceNumber()
{

  try
  {

    int32_t instanceNumber = 0;
    // reading image number
    if ( _dicomFile.search( DCM_InstanceNumber, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0020,0013)InstanceNumber tag not found" );

    }
    ::DcmIntegerString* is = static_cast< ::DcmIntegerString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad integer string" );

    }
    char* integerStringChPointer = 0;
    is->getString( integerStringChPointer );
    if ( integerStringChPointer )
    {

      std::string integerString = std::string( integerStringChPointer );
      std::istringstream iss( integerString );
      iss >> instanceNumber;

    }

    return instanceNumber;

  }
  GKG_CATCH( "int32_t gkg::DcmtkDicomFileImplementation::getInstanceNumber()" );

}


std::string gkg::DcmtkDicomFileImplementation::getSeriesDescription()
{

  try
  {

    std::string seriesDescriptionString = "";
    // reading series description
    if ( _dicomFile.search( DCM_SeriesDescription, _stack ) != EC_Normal )
    {

      throw std::runtime_error( "(0008,103e)SeriesDate tag not found" );

    }
    ::DcmLongString* is = static_cast< ::DcmLongString* >( _stack.top() );
    if ( !is )
    {

      throw std::runtime_error( "bad long string" );

    }
    char* longStringChPointer = 0;
    is->getString( longStringChPointer );
    if ( longStringChPointer )
    {

      seriesDescriptionString = std::string( longStringChPointer );

    }

    return seriesDescriptionString;

  }
  GKG_CATCH( "std::string "
             "gkg::DcmtkDicomFileImplementation::getSeriesDescription()" );

}


gkg::DcmtkDicomFileImplementation* 
gkg::DcmtkDicomFileImplementation::getImplementation(
                                                     gkg::DicomFile* dicomFile )
{

  return static_cast< gkg::DcmtkDicomFileImplementation* >(
                                               dicomFile->getImplementation() );

}
