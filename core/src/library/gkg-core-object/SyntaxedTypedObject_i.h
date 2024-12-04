#ifndef _gkg_core_object_SyntaxedTypedObject_i_h_
#define _gkg_core_object_SyntaxedTypedObject_i_h_


#include <gkg-core-object/SyntaxedTypedObject.h>
#include <gkg-core-object/TypedObject_i.h>


//
// SyntaxedTypedObject methods
//

template < class T >
inline
gkg::SyntaxedTypedObject< T >::SyntaxedTypedObject(
                                                  const std::string& syntactic )
                         : gkg::TypedObject< T >(),
                           _syntactic( syntactic )
{
}


template < class T >
inline
gkg::SyntaxedTypedObject< T >::SyntaxedTypedObject(
                                    const gkg::SyntaxedTypedObject< T >& other )
                         : gkg::TypedObject< T >( other ),
                           _syntactic( other._syntactic )
{
}


template < class T >
inline
gkg::SyntaxedTypedObject< T >::~SyntaxedTypedObject()
{
}


template < class T >
inline
bool gkg::SyntaxedTypedObject< T >::hasSyntax() const
{

  return true;

}


template < class T >
inline
std::string gkg::SyntaxedTypedObject< T >::getSyntax() const
{

  return _syntactic;

}


template < class T >
inline
void gkg::SyntaxedTypedObject< T >::setSyntax( const std::string& syntactic )
{

  _syntactic = syntactic;

}


template < class T >
inline
bool gkg::SyntaxedTypedObject< T >::check(
                                        const gkg::SyntaxSet& syntax,
                                        std::set< std::string >& missing ) const
{

  return true;

}


//
// specializations
//

namespace gkg
{


template <>
inline
bool SyntaxedTypedObject< Dictionary >::check(
                                        const SyntaxSet& syntax, 
                                        std::set< std::string >& missing ) const
{

  // get the syntactic attribute
  SyntaxSet::const_iterator ss = syntax.find( getSyntax() );
  if ( ss == syntax.end() )
  {

    missing.insert( "<syntax not found>" );
    return false;

  }

  bool result = true;

  // make sure all mandatory semantic attributes are here
  std::set< std::string > attributes = getAttributes();

  Syntax::const_iterator s = ss->second.begin(),
                         se = ss->second.end();
  while ( s != se )
  {

    if ( ( s->second.needed == true ) &&
	 ( attributes.find( s->first ) == attributes.end() ) )
    {

      missing.insert( s->first );
      result = false;

    }
    ++ s;

  }
  return result;

}


}


#endif
