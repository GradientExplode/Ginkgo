#ifndef _gkg_core_io_DicomDumpImplementation_h_
#define _gkg_core_io_DicomDumpImplementation_h_


#include <gkg-core-io/DicomTagInfo.h>


namespace gkg
{


class DicomDump;


class DicomDumpImplementation
{

  public:

    virtual ~DicomDumpImplementation();

    virtual DicomDump* getTarget() const;

    virtual DicomTagInfo& getDicomTagInfo() = 0;

  protected:

    DicomDumpImplementation( DicomDump* dicomDump,
                             const std::string& fileName );

    DicomDump* _target;

};


}


#endif
