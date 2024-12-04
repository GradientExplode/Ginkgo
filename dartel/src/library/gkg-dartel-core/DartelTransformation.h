#ifndef _gkg_dartel_core_DartelTransformation_h_
#define _gkg_dartel_core_DartelTransformation_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class S, class T >
class DartelTransformation
{

  public:

    DartelTransformation( int32_t k, int32_t degree = 1 );
    virtual ~DartelTransformation();

  protected:

    void affineTransformation( const Volume< S >& inVolume,
                               const Volume< T >& U,
                               const Volume< S >& templateProxy,
                               bool inverse );

    virtual void weights( float x,  int32_t* i, float w[] );

    int32_t mirror( int32_t i, int32_t m );

    T sample1( T* c, int32_t m0, int32_t m1, int32_t m2,
               float x0, float x1, float x2 );

    HomogeneousTransform3d< T > _m;
    int32_t _k;
    int32_t _degree;

};


}


#endif
