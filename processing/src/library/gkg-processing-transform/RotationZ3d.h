#ifndef _gkg_processing_transform_RotationZ3d_h_
#define _gkg_processing_transform_RotationZ3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class RotationZ3d : public HomogeneousTransform3d< T >
{

  public:

    RotationZ3d( const T& angle = 0.0,
                 bool degree = false,
                 bool direct = true );
    RotationZ3d( const RotationZ3d< T >& other );
    virtual ~RotationZ3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    RotationZ3d< T >& operator=( const RotationZ3d< T >& other );

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
