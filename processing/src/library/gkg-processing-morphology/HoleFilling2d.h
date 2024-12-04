#ifndef _gkg_processing_morphology_HoleFilling2d_h_
#define _gkg_processing_morphology_HoleFilling2d_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T > class Volume;


template < class T >
class HoleFilling2d
{

  public:

    HoleFilling2d( const T& foreground,
                   const T& background );
    virtual ~HoleFilling2d();

    void fill( const Volume< T >& volume, Volume< T >& filled ) const;

  protected:

    void reallocate( const Volume< T >& in, Volume< T >& out ) const;
    void propagate( const Volume< T >& in, Volume< T >& out,
                    int32_t x, int32_t y, int32_t z, int32_t t ) const;

    T _foreground;
    T _background;

};


}


#endif
