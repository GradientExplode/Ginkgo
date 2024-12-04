#ifndef _gkg_processing_plugin_dicom_DcmtkDicomDumpImplementation_h_
#define _gkg_processing_plugin_dicom_DcmtkDicomDumpImplementation_h_


#include <gkg-core-io/DicomDumpImplementation.h>
#include <gkg-core-io/DicomTagInfo.h>

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/dcistrmf.h>
//#define INCLUDE_CSTDLIB
//#define INCLUDE_CSTRING
#include <dcmtk/ofstd/ofstdinc.h>


namespace gkg
{


class DcmtkDicomDumpImplementation : public DicomDumpImplementation
{

  public:

    DcmtkDicomDumpImplementation( DicomDump* dicomDump,
                                  const std::string& fileName );

    DicomTagInfo& getDicomTagInfo();

    static DcmtkDicomDumpImplementation* getImplementation( 
                                                         DicomDump* dicomDump );

  private:

    void processGroup( DcmItem* item, std::list< DicomTagInfo >& tagList );
    void processElement( DcmElement* element, 
                         std::list< DicomTagInfo >& tagList );
    void processItem( DcmItem* item, std::list< DicomTagInfo >& tagList );
    void processPixelItem( DcmPixelItem* item, 
                           std::list< DicomTagInfo >& tagList );

    DicomTagInfo _dicomTagInfo;

};


}


#endif
