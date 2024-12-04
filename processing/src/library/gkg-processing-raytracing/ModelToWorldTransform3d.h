#ifndef _gkg_processing_raytracing_ModelToWorldTransform3d_h_
#define _gkg_processing_raytracing_ModelToWorldTransform3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>


namespace gkg
{


template < class T > class Translation3d;
template < class T > class Rotation3d;
template < class T > class InverseTransform3d;


//
// Model to world transform is given by the following equation:
//
//  P{world} = T(M{world}).T(+C{world}).R{world}.T(-C{world})P{model}
//
// with:
//       R{world} = Rz{world}.Ry{world}.Rx{world}
//
template < class T >
class ModelToWorldTransform3d : public CompositeTransform3d< T >
{

  public:

    ModelToWorldTransform3d( const T& rotationCenterX = 0.0,
                             const T& rotationCenterY = 0.0,
                             const T& rotationCenterZ = 0.0,
                             const T& angleX = 0.0,
                             const T& angleY = 0.0,
                             const T& angleZ = 0.0,
                             const T& translationX = 0.0,
                             const T& translationY = 0.0,
                             const T& translationZ = 0.0,
                             bool degree = false );
    ModelToWorldTransform3d( const Vector3d< T >& rotationCenter,
                             const Vector3d< T >& angles,
                             const Vector3d< T >& translation,
                             bool degree = false );
    ModelToWorldTransform3d( const ModelToWorldTransform3d< T >& other );
    virtual ~ModelToWorldTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    ModelToWorldTransform3d< T >&
      operator=( const ModelToWorldTransform3d< T >& other );

    void setParameters( const T& rotationCenterX,
                        const T& rotationCenterY,
                        const T& rotationCenterZ,
                        const T& angleX,
                        const T& angleY,
                        const T& angleZ,
                        const T& translationX,
                        const T& translationY,
                        const T& translationZ,
                        bool degree = false,
                        bool updating = true,
                        bool notify = true );
    void setParameters( const Vector3d< T >& rotationCenter,
                        const Vector3d< T >& angles,
                        const Vector3d< T >& translation,
                        bool degree = false,
                        bool updating = true,
                        bool notify = true );

    void getParameters( T& rotationCenterX,
                        T& rotationCenterY,
                        T& rotationCenterZ,
                        T& angleX,
                        T& angleY,
                        T& angleZ,
                        T& translationX,
                        T& translationY,
                        T& translationZ,
                        bool degree = false ) const;
    void getParameters( Vector3d< T >& rotationCenter,
                        Vector3d< T >& angles,
                        Vector3d< T >& translation,
                        bool degree = false ) const;

    void setRotationCenter( const T& rotationCenterX,
                            const T& rotationCenterY,
                            const T& rotationCenterZ,
                            bool updating = true,
                            bool notify = true );
    void setRotationCenter( const Vector3d< T >& rotationCenter,
                            bool updating = true,
                            bool notify = true );

    void getRotationCenter( T& rotationCenterX,
                            T& rotationCenterY,
                            T& rotationCenterZ ) const;
    void getRotationCenter( Vector3d< T >& rotationCenter ) const;


    void setAngles( const T& angleX,
                    const T& angleY,
                    const T& angleZ,
                    bool degree = false,
                    bool updating = true,
                    bool notify = true );
    void setAngles( const Vector3d< T >& angles,
                    bool degree = false,
                    bool updating = true,
                    bool notify = true );

    void getAngles( T& angleX,
                    T& angleY,
                    T& angleZ,
                    bool degree = false ) const;
    void getAngles( Vector3d< T >& angles,
                    bool degree = false ) const;

    void setTranslation( const T& translationX,
                         const T& translationY,
                         const T& translationZ,
                         bool updating = true,
                         bool notify = true );
    void setTranslation( const Vector3d< T >& translation,
                         bool updating = true,
                         bool notify = true );

    void getTranslation( T& translationX,
                         T& translationY,
                         T& translationZ ) const;
    void getTranslation( Vector3d< T >& translation ) const;

    const Translation3d< T >& getTranslationCenter3d() const;
    const Rotation3d< T >& getRotation3d() const;
    const Translation3d< T >& getTranslation3d() const;

  private:

    RCPointer< Translation3d< T > > _translationCenter3d;
    RCPointer< Rotation3d< T > > _rotation3d;
    RCPointer< Translation3d< T > > _translation3d;

};


}


#endif
