#ifndef _gkg_core_object_ObjectFactory_h_
#define _gkg_core_object_ObjectFactory_h_


#include <gkg-core-object/BaseObject.h>
#include <map>


namespace gkg
{


template < class T > class TypedObject;


class ObjectFactory
{

  public:

    typedef BaseObject* ( *Creator )( BaseObject& );

    ObjectFactory();
    template < class T > ObjectFactory( const T& t );

    template < class T > BaseObject* create( T& t ) const;
    template < class T > BaseObject* create( TypedObject< T >& thing ) const;

    void setId( const std::string& name );

    template < class T, class U >
    static void registerCreator( const T& outType,
                                 const U& inType,
                                 Creator creator );
    template < class T >
    static void registerCreatorForId( const std::string& outType,
                                      const T& inType,
                                      Creator creator );
    static void registerCreatorIds( const std::string& outType,
                                    const std::string& inType,
                                    Creator creator );
    static std::map< std::string, std::map< std::string, Creator > >&
      getCreators();

  private:

    std::string _id;

};


}


#endif
