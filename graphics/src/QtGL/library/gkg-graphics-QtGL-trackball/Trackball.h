#ifndef _gkg_graphics_QtGL_trackball_Trackball_h_
#define _gkg_graphics_QtGL_trackball_Trackball_h_


#include <gkg-processing-transform/Quaternion.h>


namespace gkg
{


template < class T >
class Trackball
{

  public:

    Trackball();
    virtual ~Trackball();

    void setQuaternion( const Quaternion< T >& q );
    void setViewport( int32_t width, int32_t height, T radius = 0.8 );
    const typename Quaternion< T >::Rotation& getRotation() const;
    const typename Quaternion< T >::Rotation& getInverseRotation() const;

    Quaternion< T >& getQuaternion();

    void mousePress( int32_t x, int32_t y );
    void mouseMove( int32_t x, int32_t y );

  private:

    void mouseToSphere( T x, T y, T* p );

    Quaternion< T > _q;
    T _width;
    T _height;
    T _rho;
    T _v0[ 3 ];

};


}


#endif
