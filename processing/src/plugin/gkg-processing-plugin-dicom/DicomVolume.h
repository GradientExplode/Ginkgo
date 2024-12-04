#ifndef _gkg_processing_plugin_dicom_DicomVolume_h_
#define _gkg_processing_plugin_dicom_DicomVolume_h_


#include <pdicom/container/DicomProxy.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class DicomVolume : public pdcm::DicomProxy
{

  public:

    DicomVolume( Volume< T >& volume, pdcm::DataInfo* info = 0 );

    bool allocate( pdcm::DataInfo* info = 0 );
    void destroy();
    bool isNull() const;
    bool isMemoryMapped() const;

    pdcm::BinaryHeader& getBinaryHeader();

    uint8_t* operator() ( int32_t x,
                          int32_t y = 0, 
                          int32_t z = 0, 
                          int32_t t = 0 );

  private:

    Volume< T >& _volume;

};


}


#endif
