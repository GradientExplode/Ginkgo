#ifndef _gkg_processing_transform_RotationY3d_h_
#define _gkg_processing_transform_RotationY3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class RotationY3d : public HomogeneousTransform3d< T >
{

  public:

    RotationY3d( const T& angle = 0.0,
                 bool degree = false,
                 bool direct = true );
    RotationY3d( const RotationY3d< T >& other );
    virtual ~RotationY3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    RotationY3d< T >& operator=( const RotationY3d< T >& other );

    void setDirectAngle( const T& angle,
                         bool degree = false,
                         bool update = true,
                         bool notify = true );
    void getDirectAngle( T& angle,
                         bool degree = false ) const;

    void setInverseAngle( const T& angle,
                          bool degree = false,
                          bool update = true,
                          bool notify = true );
    void getInverseAngle( T& angle,
                          bool degree = false ) const;

  protected:

    T _directAngle;
    T _inverseAngle;

};


}


#endif
