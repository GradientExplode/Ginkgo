#include <gkg-core-io/DicomFile.h>
#include <gkg-core-io/DicomFileImplementation.h>
#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomFile::DicomFile( const std::string& fileName )
{

  try
  {

    _dicomFileImplementation =
    gkg::DicomSelector::getInstance().getImplementationFactory()->
                                createDicomFileImplementation( this, fileName );

  }
  GKG_CATCH( "gkg::DicomFile::DicomFile( const std::string& fileName )" );

}


gkg::DicomFile::~DicomFile()
{

  delete _dicomFileImplementation;

}


std::string gkg::DicomFile::getInstitutionName() const
{

  try
  {

    return _dicomFileImplementation->getInstitutionName();

  }
  GKG_CATCH( "std::string gkg::DicomFile::getInstitutionName() const" );

}


std::string gkg::DicomFile::getManufacturersModelName() const
{

  try
  {

    return _dicomFileImplementation->getManufacturersModelName();

  }
  GKG_CATCH( "std::string gkg::DicomFile::getManufacturersModelName() const" );

}


std::string gkg::DicomFile::getDate() const
{

  try
  {

    return _dicomFileImplementation->getDate();

  }
  GKG_CATCH( "std::string gkg::DicomFile::getDate() const" );

}


std::string gkg::DicomFile::getPatientID() const
{

  try
  {

    return _dicomFileImplementation->getPatientID();

  }
  GKG_CATCH( "std::string gkg::DicomFile::getPatientID() const" );

}


int32_t gkg::DicomFile::getStudyID() const
{

  try
  {

    return _dicomFileImplementation->getStudyID();

  }
  GKG_CATCH( "int32_t gkg::DicomFile::getStudyID() const" );

}


int32_t gkg::DicomFile::getSeriesNumber() const
{

  try
  {

    return _dicomFileImplementation->getSeriesNumber();

  }
  GKG_CATCH( "int32_t gkg::DicomFile::getSeriesNumber() const" );

}


int32_t gkg::DicomFile::getInstanceNumber() const
{

  try
  {

    return _dicomFileImplementation->getInstanceNumber();

  }
  GKG_CATCH( "int32_t gkg::DicomFile::getInstanceNumber() const" );

}


std::string gkg::DicomFile::getSeriesDescription() const
{

  try
  {

    return _dicomFileImplementation->getSeriesDescription();

  }
  GKG_CATCH( "std::string gkg::DicomFile::getSeriesDescription() const" );

}


gkg::DicomFileImplementation* gkg::DicomFile::getImplementation() const
{

  return _dicomFileImplementation;

}


