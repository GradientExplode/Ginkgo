#ifndef _gkg_dartel_volume_DartelVolumeTransformation_h_
#define _gkg_dartel_volume_DartelVolumeTransformation_h_


#include <gkg-dartel-core/DartelTransformation.h>


namespace gkg
{


template < class S, class T >
class DartelVolumeTransformation : public DartelTransformation< S, T >
{

  public:

    DartelVolumeTransformation( int32_t k, int32_t degree = 1 );
    virtual ~DartelVolumeTransformation();

    void wrap( const Volume< S >& inVolume,
               const Volume< T >& flowField, 
               Volume< S >& outVolume );
    void unwrap( const Volume< S >& inVolume,
                 const Volume< T >& flowField, 
                 const Volume< S >& templateProxy,
                 Volume< S >& outVolume );

  protected:

    void weights( float x,  int32_t* i, float w[] );

    S sample3( const S* c, int32_t m0, int32_t m1, int32_t m2,
               float x0, float x1, float x2 );

    void setHeader( const Volume< T >& flowfield,
                    Volume< S >& outVolume,
                    const Volume< S >& templateProxy,
                    bool inverse );

};


}


#endif
