#ifndef _gkg_core_object_SyntaxedTypedObject_h_
#define _gkg_core_object_SyntaxedTypedObject_h_


#include <gkg-core-object/TypedObject.h>


namespace gkg
{


template < class T >
class SyntaxedTypedObject : public TypedObject< T >
{

  public:

    SyntaxedTypedObject( const std::string& syntactic );
    SyntaxedTypedObject( const SyntaxedTypedObject< T >& other );
    virtual ~SyntaxedTypedObject();

    virtual bool hasSyntax() const;
    virtual std::string getSyntax() const;
    virtual void setSyntax( const std::string& syntactic );

    virtual bool check( const SyntaxSet& syntax,
                        std::set< std::string >& missing ) const;

  private:

    std::string _syntactic;

};


}


#endif
