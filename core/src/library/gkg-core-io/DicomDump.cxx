#include <gkg-core-io/DicomDump.h>
#include <gkg-core-io/DicomDumpImplementation.h>
#include <gkg-core-io/DicomSelector.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomDump::DicomDump( const std::string& fileName )
{

  try
  {

    _dicomDumpImplementation =
      gkg::DicomSelector::getInstance().getImplementationFactory()->
                                createDicomDumpImplementation( this, fileName );

  }
  GKG_CATCH( "gkg::DicomDump::DicomDump( const std::string& fileName )" );

}


gkg::DicomDump::~DicomDump()
{

  delete _dicomDumpImplementation;

}


gkg::DicomTagInfo& gkg::DicomDump::getDicomTagInfo() const
{

  try
  {

    return _dicomDumpImplementation->getDicomTagInfo();

  }
  GKG_CATCH( "gkg::DicomTagInfo& gkg::DicomDump::getDicomTagInfo() const" );

}


gkg::DicomDumpImplementation* gkg::DicomDump::getImplementation() const
{

  return _dicomDumpImplementation;

}
