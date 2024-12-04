#ifndef _gkg_processing_plugin_dicom_GkgHeader_h_
#define _gkg_processing_plugin_dicom_GkgHeader_h_


#include <gkg-core-object/HeaderedObject.h>

#include <pdicom/object/HeaderProxy.h>


namespace gkg
{


class GkgHeader : public pdcm::HeaderProxy
{

  public:

    GkgHeader( HeaderedObject& header );

    void addAttribute( const std::string& name, int32_t value );
    void addAttribute( const std::string& name, double value );
    void addAttribute( const std::string& name, const std::string& value );

    void addAttribute( const std::string& name, 
                       const std::vector< int32_t >& value );
    void addAttribute( const std::string& name, 
                       const std::vector< double >& value );
    void addAttribute( const std::string& name, 
                       const std::vector< std::string >& value );

    void addAttribute( const std::string& name, 
                       const std::vector< std::vector< double > >& value );

    bool hasAttribute( const std::string& name );

  private:

    HeaderedObject::Header& _header;

};


}


#endif
