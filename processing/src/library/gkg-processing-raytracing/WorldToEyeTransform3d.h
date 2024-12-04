#ifndef _gkg_processing_raytracing_WorldToEyeTransform3d_h_
#define _gkg_processing_raytracing_WorldToEyeTransform3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>


namespace gkg
{


template < class T > class Translation3d;
template < class T > class Rotation3d;
template < class T > class InverseTransform3d;


//
// Viewing transform is given by the following equation:
//
//  P{eye} = R{eye->world}^(-1).T(-E{world})P{world}
//
// with:
//
//       R{eye->world} = Rz{eye->world}.Ry{eye->world}.Rx{eye->world}
//
// and:
//
//    V = normalized upward vector
//    N = N / | N |
//    U = V * N
//                         [ Ux Uy Uz 0 ]
//    R{eye->world}^(-1) = [ Vx Vy Vz 0 ]
//                         [ Nx Ny Nz 0 ]
//                         [ 0  0  0  1 ]
//
template < class T >
class WorldToEyeTransform3d : public CompositeTransform3d< T >
{

  public:

    WorldToEyeTransform3d( const T& eyePositionX = 0.0,
                           const T& eyePositionY = 0.0,
                           const T& eyePositionZ = 0.0,
                           const T& referencePositionX = 0.0,
                           const T& referencePositionY = 0.0,
                           const T& referencePositionZ = 0.0,
                           const T& upwardDirectionX = 0.0,
                           const T& upwardDirectionY = 0.0,
                           const T& upwardDirectionZ = 0.0 );
    WorldToEyeTransform3d( const Vector3d< T >& eyePosition,
                           const Vector3d< T >& referencePosition,
                           const Vector3d< T >& upwardDirection );
    WorldToEyeTransform3d( const WorldToEyeTransform3d< T >& other );
    virtual ~WorldToEyeTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    WorldToEyeTransform3d< T >&
      operator=( const WorldToEyeTransform3d< T >& other );

    void setParameters( const T& eyePositionX,
                        const T& eyePositionY,
                        const T& eyePositionZ,
                        const T& referencePositionX,
                        const T& referencePositionY,
                        const T& referencePositionZ,
                        const T& upwardDirectionX,
                        const T& upwardDirectionY,
                        const T& upwardDirectionZ,
                        bool updating = true,
                        bool notify = true );
    void setParameters( const Vector3d< T >& eyePosition,
                        const Vector3d< T >& referencePosition,
                        const Vector3d< T >& upwardDirection,
                        bool updating = true,
                        bool notify = true );

    void getParameters( T& eyePositionX,
                        T& eyePositionY,
                        T& eyePositionZ,
                        T& referencePositionX,
                        T& referencePositionY,
                        T& referencePositionZ,
                        T& upwardDirectionX,
                        T& upwardDirectionY,
                        T& upwardDirectionZ ) const;
    void getParameters( Vector3d< T >& eyePosition,
                        Vector3d< T >& referencePosition,
                        Vector3d< T >& upwardDirection ) const;

    void setEyePosition( const T& eyePositionX,
                         const T& eyePositionY,
                         const T& eyePositionZ,
                         bool updating = true,
                         bool notify = true );
    void setEyePosition( const Vector3d< T >& eyePosition,
                         bool updating = true,
                         bool notify = true );

    void getEyePosition( T& eyePositionX,
                         T& eyePositionY,
                         T& eyePositionZ ) const;
    void getEyePosition( Vector3d< T >& eyePosition ) const;

    void setReferencePosition( const T& referencePositionX,
                               const T& referencePositionY,
                               const T& referencePositionZ,
                               bool updating = true,
                               bool notify = true );
    void setReferencePosition( const Vector3d< T >& referencePosition,
                               bool updating = true,
                               bool notify = true );

    void getReferencePosition( T& referencePositionX,
                               T& referencePositionY,
                               T& referencePositionZ ) const;
    void getReferencePosition( Vector3d< T >& referencePosition ) const;

    void setUpwardDirection( const T& upwardDirectionX,
                             const T& upwardDirectionY,
                             const T& upwardDirectionZ,
                             bool updating = true,
                             bool notify = true );
    void setUpwardDirection( const Vector3d< T >& upwardDirection,
                             bool updating = true,
                             bool notify = true );

    void getUpwardDirection( T& upwardDirectionX,
                             T& upwardDirectionY,
                             T& upwardDirectionZ ) const;
    void getUpwardDirection( Vector3d< T >& upwardDirection ) const;

    const Translation3d< T >& getTranslation3d() const;
    const Rotation3d< T >& getRotation3d() const;

  private:

    Vector3d< T > _eyePosition;           // in world coordinate system
    Vector3d< T > _referencePosition;     // in world coordinate system
    Vector3d< T > _upwardDirection;       // in world coordinate system

    RCPointer< Translation3d< T > > _translation3d;
    RCPointer< RotationX3d< T > > _rotationX3d;
    RCPointer< RotationY3d< T > > _rotationY3d;
    RCPointer< RotationZ3d< T > > _rotationZ3d;

};


}


#endif
