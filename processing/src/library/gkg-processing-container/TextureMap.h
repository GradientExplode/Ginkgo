#ifndef _gkg_processing_container_TextureMap_h_
#define _gkg_processing_container_TextureMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <list>
#include <vector>
#include <map>
#include <iostream>


namespace gkg
{


//
// class TextureMap
//

template < class T >
class TextureMap : public HeaderedObject
{

  public:

    typedef typename std::map< int32_t, T >::iterator iterator;
    typedef typename std::map< int32_t, T >::const_iterator const_iterator;
    typedef typename std::map< int32_t, T >::reverse_iterator reverse_iterator;
    typedef typename std::map< int32_t, T >::const_reverse_iterator 
                                                         const_reverse_iterator;

    TextureMap();
    TextureMap( const TextureMap< T >& other );
    virtual ~TextureMap();

    TextureMap< T >& operator=( const TextureMap< T >& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator lower_bound( const int32_t& index );
    iterator upper_bound( const int32_t& index );

    const_iterator lower_bound( const int32_t& index ) const;
    const_iterator upper_bound( const int32_t& index ) const;

    std::pair< iterator, iterator > equal_range( const int32_t& index );
    std::pair< const_iterator,
               const_iterator > equal_range( const int32_t& index ) const;

    void clear();

    T& addTexture( const int32_t& index, const T& texture );
    void addTextures( const std::list< T >& textures );
    void addTextures( const std::vector< T >& textures );
    bool hasTexture( const int32_t& index ) const;

    void setTexture( const int32_t& index, const T& texture );
    void setTextures( const std::map< int32_t, T >& textures );
    void setTextures( const std::vector< T >& textures );

    void removeTexture( const int32_t& index );
    void removeTextures( const std::set< int32_t >& indices );

    int32_t getCount() const;
    T& getTexture( const int32_t& index );
    const T& getTexture( const int32_t& index ) const;

  protected:

    void updateHeader();

    std::map< int32_t, T > _textures;

};


}


namespace std
{


template < class T >
ostream& 
operator<<( ostream& os, const gkg::TextureMap< T >& thing );


}


#endif
