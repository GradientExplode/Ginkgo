#ifndef _gkg_graphics_QtGL_slicing_CubeZBuffer_h_
#define _gkg_graphics_QtGL_slicing_CubeZBuffer_h_


#include <gkg-core-cppext/StdInt.h>


namespace gkg
{


template < class T >
class CubeZBuffer
{

  public:

    CubeZBuffer();
    virtual ~CubeZBuffer();

    void setTransform( const T* m );

    const T& getMinZ() const;
    const T& getMaxZ() const;
    const T& getVertexZ( int32_t i ) const;
    int32_t getKey( T z ) const;
    T getStep( int32_t nbSlices ) const;
    T getZ( T x, T y, T z ) const;

  private:

    T _vz[ 4 ];
    T _zi[ 8 ];
    T _zMin;
    T _zMax;

};


}


#endif
