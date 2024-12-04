#include <gkg-core-io/DicomFileImplementation.h>


gkg::DicomFileImplementation::DicomFileImplementation( 
                                               gkg::DicomFile* dicomFile,
                                               const std::string& /*fileName*/ )
                             : _target( dicomFile )
{
}


gkg::DicomFileImplementation::~DicomFileImplementation()
{
}


gkg::DicomFile* gkg::DicomFileImplementation::getTarget() const
{

  return _target;

}
