#include <gkg-core-io/DicomDumpImplementation.h>


gkg::DicomDumpImplementation::DicomDumpImplementation(
                                             gkg::DicomDump* dicomDump,
                                             const std::string& /* fileName */ )
                            : _target( dicomDump )
{
}


gkg::DicomDumpImplementation::~DicomDumpImplementation()
{
}


gkg::DicomDump* gkg::DicomDumpImplementation::getTarget() const
{

  return _target;

}
