#ifndef _gkg_processing_transform_Referential_h_
#define _gkg_processing_transform_Referential_h_


#include <gkg-core-object/HeaderedObject.h>
#include <string>


namespace gkg
{


class Referential : public HeaderedObject
{

  public:

    Referential( const std::string& name ="" );
    virtual ~Referential();

    void setName( const std::string& name );
    const std::string& getName() const;

  protected:

    // forbidden
    Referential( const Referential& );
    Referential& operator=( const Referential& );

    std::string _name;

};


}


#endif
