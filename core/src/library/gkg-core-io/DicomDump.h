#ifndef _gkg_core_io_DicomDump_h_
#define _gkg_core_io_DicomDump_h_


#include <gkg-core-io/DicomTagInfo.h>
#include <string>


namespace gkg
{


class DicomDumpImplementation;


class DicomDump
{

  public:

    DicomDump( const std::string& fileName );
    virtual ~DicomDump();

    DicomTagInfo& getDicomTagInfo() const;

    DicomDumpImplementation* getImplementation() const;

  protected:

    DicomDumpImplementation* _dicomDumpImplementation;

};


}


#endif
