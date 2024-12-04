#ifndef _gkg_processing_container_TextureMap_i_h_
#define _gkg_processing_container_TextureMap_i_h_


#include <gkg-processing-container/TextureMap.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>



//
// class TextureMap
//

template < class T >
inline
gkg::TextureMap< T >::TextureMap()
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "TextureMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< T >::getName() );
  _header.addAttribute( "item_count", 0 );

}


template < class T >
inline
gkg::TextureMap< T >::TextureMap( const gkg::TextureMap< T >& other )
                     : gkg::HeaderedObject( other ),
                       _textures( other._textures )
{
}


template < class T >
inline
gkg::TextureMap< T >::~TextureMap()
{
}


template < class T >
inline
gkg::TextureMap< T >&
gkg::TextureMap< T >::operator=( const gkg::TextureMap< T >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _textures = other._textures;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::TextureMap< T >& "
             "gkg::TextureMap< T >::operator=( "
             "const gkg::TextureMap< T >& other )" );

}


template < class T >
inline
typename gkg::TextureMap< T >::iterator
gkg::TextureMap< T >::begin()
{

  return _textures.begin();

}


template < class T >
inline
typename gkg::TextureMap< T >::iterator
gkg::TextureMap< T >::end()
{

  return _textures.end();

}


template < class T >
inline
typename gkg::TextureMap< T >::const_iterator
gkg::TextureMap< T >::begin() const
{

  return _textures.begin();

}


template < class T >
inline
typename gkg::TextureMap< T >::const_iterator
gkg::TextureMap< T >::end() const
{

  return _textures.end();

}


template < class T >
inline
typename gkg::TextureMap< T >::reverse_iterator
gkg::TextureMap< T >::rbegin()
{

  return _textures.rbegin();

}


template < class T >
inline
typename gkg::TextureMap< T >::reverse_iterator
gkg::TextureMap< T >::rend()
{

  return _textures.rend();

}


template < class T >
inline
typename gkg::TextureMap< T >::const_reverse_iterator
gkg::TextureMap< T >::rbegin() const
{

  return _textures.rbegin();

}


template < class T >
inline
typename gkg::TextureMap< T >::const_reverse_iterator 
gkg::TextureMap< T >::rend() const
{

  return _textures.rend();

}


template < class T >
inline
typename gkg::TextureMap< T >::iterator 
gkg::TextureMap< T >::lower_bound( const int32_t& index )
{

  return _textures.lower_bound( index );

}


template < class T >
inline
typename gkg::TextureMap< T >::iterator 
gkg::TextureMap< T >::upper_bound( const int32_t& index )
{

  return _textures.upper_bound( index );

}


template < class T >
inline
typename gkg::TextureMap< T >::const_iterator 
gkg::TextureMap< T >::lower_bound( const int32_t& index ) const
{

  return _textures.lower_bound( index );

}


template < class T >
inline
typename gkg::TextureMap< T >::const_iterator 
gkg::TextureMap< T >::upper_bound( const int32_t& index ) const
{

  return _textures.upper_bound( index );

}


template < class T >
inline
std::pair< typename gkg::TextureMap< T >::iterator,
           typename gkg::TextureMap< T >::iterator >
gkg::TextureMap< T >::equal_range( const int32_t& index )
{

  return _textures.equal_range( index );

}


template < class T >
inline
std::pair< typename gkg::TextureMap< T >::const_iterator,
           typename gkg::TextureMap< T >::const_iterator >
gkg::TextureMap< T >::equal_range( const int32_t& index ) const
{

  return _textures.equal_range( index );

}


template< class T >
inline
void gkg::TextureMap< T >::clear()
{

  try
  {

    _textures.clear();
    this->updateHeader();

  }
  GKG_CATCH( "template< class T > "
             "inline "
             "void gkg::TextureMap< T >::clear()" );

}


template < class T >
inline
T& gkg::TextureMap< T >::addTexture( const int32_t& index, const T& texture )
{

  try
  {

    if ( _textures.find( index ) != _textures.end() )
    {

      throw std::runtime_error( "texture already present in the texture map" );

    }
    T& t = _textures[ index ] = texture;

    updateHeader();

    return t;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T& gkg::TextureMap< T >::addTexture( "
             "const int32_t& index, "
             "const T& texture )" );

}


template < class T >
inline
void gkg::TextureMap< T >::addTextures( const std::list< T >& textures )
{

  try
  {

    int32_t count = getCount();

    typename std::list< T >::const_iterator
      t = textures.begin(),
      te = textures.end();
    while ( t != te )
    {

      _textures.insert( std::pair< int32_t, T >( count, *t ) );
      ++ t;
      ++ count;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::addTextures( "
             "const std::list< T >& textures )" );

}


template < class T >
inline
void gkg::TextureMap< T >::addTextures( const std::vector< T >& textures )
{

  try
  {

    int32_t count = getCount();

    typename std::vector< T >::const_iterator
      t = textures.begin(),
      te = textures.end();
    while ( t != te )
    {

      _textures.insert( std::pair< int32_t, T >( count, *t ) );
      ++ t;
      ++ count;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::addTextures( "
             "const std::vector< T >& textures )" );

}


template < class T >
inline
bool gkg::TextureMap< T >::hasTexture( const int32_t& index ) const
{

  try
  {

    if ( _textures.find( index ) != _textures.end() )
    {

      return true;

    }
    return false;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "bool gkg::TextureMap< T >::hasTexture( "
             "const int32_t& index ) const" );

}


template < class T >
inline
void gkg::TextureMap< T >::setTexture( const int32_t& index, const T& texture )
{

  try
  {

    _textures[ index ] = texture;
    updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::setTexture( "
             "const int32_t& index, "
             "const T& texture )" );

}


template < class T >
inline
void gkg::TextureMap< T >::setTextures( const std::map< int32_t, T >& textures )
{

  try
  {

    typename std::map< int32_t, T >::const_iterator
      t = textures.begin(),
      te = textures.end();
    while ( t != te )
    {

      _textures[ t->first ] = t->second;
      ++ t;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::setTextures( "
             "const std::map< int32_t, T >& textures )" );

}


template < class T >
inline
void gkg::TextureMap< T >::setTextures( const std::vector< T >& textures )
{

  try
  {

    int32_t index = 0;
    int32_t count = ( int32_t )textures.size();
    for ( index = 0; index < count; index++ )
    {

      _textures[ index ] = textures[ index ];

    }

    updateHeader();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::setTextures( "
             "const std::map< int32_t, T >& textures )" );

}


template < class T >
inline
void gkg::TextureMap< T >::removeTexture( const int32_t& index )
{

  try
  {

    typename std::map< int32_t, T >::iterator t = _textures.find( index );

    if ( t != _textures.end() )
    {

      _textures.erase( t );

    }
    else
    {

      throw std::runtime_error( "texture not present in the texture map" );

    }    

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::removeTexture( "
             "const int32_t& index )" );

}


template < class T >
inline
void gkg::TextureMap< T >::removeTextures( const std::set< int32_t >& indices )
{

  try
  {

    typename std::set< int32_t >::const_iterator 
      i = indices.begin(),
      ie = indices.end();

    while ( i != ie )
    {

      typename std::map< int32_t, T >::iterator t = _textures.find( *i );

      if ( t != _textures.end() )
      {

        _textures.erase( t );

      }
      else
      {

        throw std::runtime_error( "texture not present in the texture map" );

      }
      ++ i;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::removeTextures( "
             "const std::set< int32_t >& indices )" );

}


template < class T >
inline
int32_t gkg::TextureMap< T >::getCount() const
{

  return ( int32_t )_textures.size();

}


template < class T >
inline
T& gkg::TextureMap< T >::getTexture( const int32_t& index )
{

  try
  {

    typename std::map< int32_t, T >::iterator t = _textures.find( index );

    if ( t != _textures.end() )
    {

      return t->second;

    }
    else
    {

      throw std::runtime_error( "not a valid index key in the texture map" );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T& gkg::TextureMap< T >::getTexture( "
             "const int32_t& index )" );

}


template < class T >
inline
const T& gkg::TextureMap< T >::getTexture( const int32_t& index ) const
{

  try
  {

    typename std::map< int32_t, T >::const_iterator t = _textures.find( index );

    if ( t != _textures.end() )
    {

      return t->second;

    }
    else
    {

      throw std::runtime_error( "not a valid index key in the texture map" );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T& gkg::TextureMap< T >::getTexture( "
             "const int32_t& index ) const" );

}


template < class T >
inline
void gkg::TextureMap< T >::updateHeader()
{

  try
  {

    _header[ "item_count" ] = this->getCount();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::TextureMap< T >::updateHeader()" );

}


namespace std
{


template < class T >
ostream& operator<<( ostream& os,
                     const gkg::TextureMap< T >& thing )
{

  typename gkg::TextureMap< T >::const_iterator
    t = thing.begin(),
    te = thing.end();

  while ( t != te )
  {

    os << "( " << t->first << ", " << t->second << " )  ";
    ++ t;

  }
  return os;

}


}


#endif
