#ifndef _gkg_processing_algobase_Binarizer_h_
#define _gkg_processing_algobase_Binarizer_h_


#include <vector>


namespace gkg
{


template < class T > class TestFunction;
template < class T > class Volume;
template < class T > class TextureMap;


//
// class Binarizer< IN, OUT >
//

template < class IN, class OUT >
class Binarizer
{

  public:

    Binarizer( const TestFunction< IN >& testFunction,
               const OUT& foreground,
               const OUT& background );

    void binarize( const IN& in, OUT& out ) const;

  private:

    const TestFunction< IN >& _testFunction;
    OUT _foreground;
    OUT _background;

};


//
// class Binarizer< std::vector< IN >, std::vector< OUT > >
//

template < class IN, class OUT >
class Binarizer< std::vector< IN >, std::vector< OUT > >
{

  public:

    Binarizer( const TestFunction< IN >& testFunction,
               const OUT& foreground,
               const OUT& background );

    void binarize( const std::vector< IN >& in, std::vector< OUT >& out ) const;

  private:

    void reallocate( const std::vector< IN >& in,
                     std::vector< OUT >& out ) const;

    Binarizer< IN, OUT > _itemBinarizer;

};


//
// class Binarizer< Volume< IN >, Volume< OUT > >
//

template < class IN, class OUT >
class Binarizer< Volume< IN >, Volume< OUT > >
{

  public:

    Binarizer( const TestFunction< IN >& testFunction,
               const OUT& foreground,
               const OUT& background );

    void binarize( const Volume< IN >& in, Volume< OUT >& out ) const;

  private:

    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;

    Binarizer< IN, OUT > _itemBinarizer;

};


//
// class Binarizer< TextureMap< IN >, TextureMap< OUT > >
//

template < class IN, class OUT >
class Binarizer< TextureMap< IN >, TextureMap< OUT > >
{

  public:

    Binarizer( const TestFunction< IN >& testFunction,
               const OUT& foreground,
               const OUT& background );

    void binarize( const TextureMap< IN >& in, TextureMap< OUT >& out ) const;

  private:

    void reallocate( const TextureMap< IN >& in, TextureMap< OUT >& out ) const;

    Binarizer< IN, OUT > _itemBinarizer;

};


}


#endif
