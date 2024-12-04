#ifndef _gkg_core_io_DicomFileImplementation_h_
#define _gkg_core_io_DicomFileImplementation_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class DicomFile;


class DicomFileImplementation
{

  public:

    virtual ~DicomFileImplementation();

    virtual DicomFile* getTarget() const;

    virtual std::string getInstitutionName() = 0;
    virtual std::string getManufacturersModelName() = 0;
    virtual std::string getDate() = 0;
    virtual std::string getPatientID() = 0;
    virtual int32_t getStudyID() = 0;
    virtual int32_t getSeriesNumber() = 0;
    virtual int32_t getInstanceNumber() = 0;
    virtual std::string getSeriesDescription() = 0;

  protected:

    DicomFileImplementation( DicomFile* dicomFile,
                             const std::string& fileName );

    DicomFile* _target;

};


}


#endif
