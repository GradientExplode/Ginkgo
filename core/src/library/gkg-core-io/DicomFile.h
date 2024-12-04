#ifndef _gkg_core_io_DicomFile_h_
#define _gkg_core_io_DicomFile_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class DicomFileImplementation;


class DicomFile
{

  public:

    DicomFile( const std::string& fileName );
    virtual ~DicomFile();

    std::string getInstitutionName() const;
    std::string getManufacturersModelName() const;
    std::string getDate() const;
    std::string getPatientID() const;
    int32_t getStudyID() const;
    int32_t getSeriesNumber() const;
    int32_t getInstanceNumber() const;
    std::string getSeriesDescription() const;

    DicomFileImplementation* getImplementation() const;

  protected:

    DicomFileImplementation* _dicomFileImplementation;

};


}


#endif
