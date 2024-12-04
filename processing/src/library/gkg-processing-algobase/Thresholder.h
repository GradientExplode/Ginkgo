#ifndef _gkg_processing_algobase_Thresholder_h_
#define _gkg_processing_algobase_Thresholder_h_


#include <vector>


namespace gkg
{


template < class T > class TestFunction;
template < class T > class Volume;
template < class T > class TextureMap;


//
// class Thresholder< T >
//

template < class T >
class Thresholder
{

  public:

    Thresholder( const TestFunction< T >& testFunction,
                 const T& background );

    void threshold( const T& in, T& out ) const;

  private:

    const TestFunction< T >& _testFunction;
    T _background;

};


//
// class Thresholder< std::vector< T > >
//

template < class T >
class Thresholder< std::vector< T > >
{

  public:

    Thresholder( const TestFunction< T >& testFunction,
                 const T& background );

    void threshold( const std::vector< T >& in, std::vector< T >& out ) const;

  private:

    void reallocate( const std::vector< T >& in, std::vector< T >& out ) const;

    Thresholder< T > _itemThresholder;

};


//
// class Thresholder< Volume< T > >
//

template < class T >
class Thresholder< Volume< T > >
{

  public:

    Thresholder( const TestFunction< T >& testFunction,
                 const T& background );

    void threshold( const Volume< T >& in, Volume< T >& out ) const;

  private:

    void reallocate( const Volume< T >& in, Volume< T >& out ) const;

    Thresholder< T > _itemThresholder;

};


//
// class Thresholder< TextureMap< T > >
//

template < class T >
class Thresholder< TextureMap< T > >
{

  public:

    Thresholder( const TestFunction< T >& testFunction,
                 const T& background );

    void threshold( const TextureMap< T >& in, TextureMap< T >& out ) const;

  private:

    void reallocate( const TextureMap< T >& in, TextureMap< T >& out ) const;

    Thresholder< T > _itemThresholder;

};


}


#endif
