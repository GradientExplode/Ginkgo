#ifndef _gkg_core_object_GenericObject_h_
#define _gkg_core_object_GenericObject_h_


#include <gkg-core-object/BaseObject.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class GenericObject : public RCPointer< BaseObject >
{

  public:

    GenericObject();
    GenericObject( const GenericObject& other );
    GenericObject( BaseObject* thing );
    template < class T > GenericObject( const T& t );
    virtual ~GenericObject();

    GenericObject& operator=( const GenericObject& other );
    GenericObject& operator=( BaseObject* thing );
    template < class T > GenericObject& operator=( const T& t );

};


}


#endif
