#ifndef _gkg_processing_transform_Rotation3d_h_
#define _gkg_processing_transform_Rotation3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>


namespace gkg
{


template < class T > class RotationX3d;
template < class T > class RotationY3d;
template < class T > class RotationZ3d;
class Matrix;


//
// R = Rz.Ry.Rx
//
template < class T >
class Rotation3d : public CompositeTransform3d< T >
{

  public:

    Rotation3d( const T& angleX = 0.0,
                const T& angleY = 0.0,
                const T& angleZ = 0.0,
                bool degree = false,
                bool direct = true );
    Rotation3d( const Vector3d< T >& angles,
                bool degree = false,
                bool direct = true );
    Rotation3d( const Rotation3d< T >& other );
    virtual ~Rotation3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    Rotation3d< T >& operator=( const Rotation3d< T >& other );

    void setDirectAngles( const T& angleX,
                          const T& angleY,
                          const T& angleZ,
                          bool degree = false,
                          bool update = true,
                          bool notify = true );
    void setDirectAngles( const Vector3d< T >& angles,
                          bool degree = false,
                          bool update = true,
                          bool notify = true );
    void getDirectAngles( T& angleX,
                          T& angleY,
                          T& angleZ,
                          bool degree = false ) const;
    void getDirectAngles( Vector3d< T >& angles,
                          bool degree = false ) const;

    void setInverseAngles( const T& angleX,
                           const T& angleY,
                           const T& angleZ,
                           bool degree = false,
                           bool update = true,
                           bool notify = true );
    void setInverseAngles( const Vector3d< T >& angles,
                           bool degree = false,
                           bool update = true,
                           bool notify = true );
    void getInverseAngles( T& angleX,
                           T& angleY,
                           T& angleZ,
                           bool degree = false ) const;
    void getInverseAngles( Vector3d< T >& angles,
                           bool degree = false ) const;

    const RotationX3d< T >& getRotationX3d() const;
    const RotationY3d< T >& getRotationY3d() const;
    const RotationZ3d< T >& getRotationZ3d() const;

    void getRodriguesParameters( T& theta,
                                 Matrix& Omega,
                                 bool degree = false ) const;

  protected:

    RCPointer< RotationX3d< T > > _rotationX3d;
    RCPointer< RotationY3d< T > > _rotationY3d;
    RCPointer< RotationZ3d< T > > _rotationZ3d;

};


}


#endif
