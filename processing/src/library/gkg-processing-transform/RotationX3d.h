#ifndef _gkg_processing_transform_RotationX3d_h_
#define _gkg_processing_transform_RotationX3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class RotationX3d : public HomogeneousTransform3d< T >
{

  public:

    RotationX3d( const T& angle = 0.0,
                 bool degree = false,
                 bool direct = true );
    RotationX3d( const RotationX3d< T >& other );
    virtual ~RotationX3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    RotationX3d< T >& operator=( const RotationX3d< T >& other );

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
