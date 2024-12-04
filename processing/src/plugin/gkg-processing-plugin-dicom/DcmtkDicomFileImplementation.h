#ifndef _gkg_processing_plugin_dicom_DcmtkDicomFileImplementation_h_
#define _gkg_processing_plugin_dicom_DcmtkDicomFileImplementation_h_


#include <gkg-core-io/DicomFileImplementation.h>

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcistrmf.h>
//#define INCLUDE_CSTDLIB
//#define INCLUDE_CSTRING
#include <dcmtk/ofstd/ofstdinc.h>


namespace gkg
{


class DcmtkDicomFileImplementation : public DicomFileImplementation
{

  public:


    DcmtkDicomFileImplementation( DicomFile* dicomFile,
                                  const std::string& fileName );
    ~DcmtkDicomFileImplementation();

    std::string getInstitutionName();
    std::string getManufacturersModelName();
    std::string getDate();
    std::string getPatientID();
    int32_t getStudyID();
    int32_t getSeriesNumber();
    int32_t getInstanceNumber();
    std::string getSeriesDescription();

    static DcmtkDicomFileImplementation* 
             getImplementation( DicomFile* dicomFile );

  private:

    ::DcmFileFormat _dicomFile;
    ::DcmStack _stack;

};


}


#endif
