#ifndef _gkg_core_object_BaseObject_h_
#define _gkg_core_object_BaseObject_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-object/Syntax.h>
#include <gkg-core-pattern/Observable.h>
#include <string>
#include <list>
#include <vector>
#include <set>


namespace gkg
{


class GenericObject;
class ObjectFactory;


class BaseObject : public Observable
{

  public:

    virtual ~BaseObject();

    virtual std::string getTypeName() const = 0;
    virtual std::string getTypeOf() const = 0;

    virtual BaseObject* clone() const = 0;
    virtual BaseObject* mute( const ObjectFactory& objectFactory ) = 0;

    template < class T > const T& getValue() const;
    template < class T > T& getValue();

    template < class T > void setValue( const T& value );

    GenericObject& operator [] ( int32_t index );
    const GenericObject& operator [] ( int32_t index ) const;

    GenericObject& operator [] ( const std::string& name );
    const GenericObject& operator [] ( const std::string& name ) const;

    double getScalar() const;
    std::string getString() const;

    template < class T >
    void addAttribute( const std::string& name, const T& value );
    void copyAttributes( const BaseObject& other );
    void removeAttribute( const std::string& name );
    void removeAttributes();

    template < class T >
    void getAttribute( const std::string& name, T& value ) const;
    template < class T  >
    void  getAttribute( const std::string& name, std::list< T >& value ) const;
    template < class T  >
    void  getAttribute( const std::string& name, 
                        std::vector< T >& value ) const;
    template < class T  >
    void  getAttribute( const std::string& name, std::set< T >& value ) const;
    bool hasAttribute( const std::string& name ) const;
    std::set< std::string > getAttributes() const;

    virtual bool hasSyntax() const;
    virtual std::string getSyntax() const;
    virtual void setSyntax( const std::string& syntactic );

    virtual bool check( const SyntaxSet& syntax,
                        std::set< std::string >& missing ) const;

  protected:

    BaseObject();

};


}


#endif
