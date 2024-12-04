#ifndef _gkg_core_object_TypedObject_h_
#define _gkg_core_object_TypedObject_h_


#include <gkg-core-object/BaseObject.h>


namespace gkg
{


template < class T >
class TypedObject : public BaseObject
{

  public:

    TypedObject();
    TypedObject( const T& t );
    TypedObject( const TypedObject< T >& other );
    virtual ~TypedObject();

    TypedObject< T >& operator=( const TypedObject< T >& other );

    virtual std::string getTypeName() const;
    virtual std::string getTypeOf() const;

    virtual BaseObject* clone() const;
    virtual BaseObject* mute( const ObjectFactory& objectFactory );

    T& getTypedValue();
    const T& getTypedValue() const;

  private:

    T _value;

};



}


#endif
