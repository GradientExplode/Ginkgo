#ifndef _gkg_processing_transform_RigidTransform3d_h_
#define _gkg_processing_transform_RigidTransform3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>
#include <vector>


namespace gkg
{


template < class T > class Rotation3d;
template < class T > class Translation3d;


template < class T >
class RigidTransform3d : public CompositeTransform3d< T >
{

  public:

    RigidTransform3d( const T& angleX = 0.0,
                      const T& angleY = 0.0,
                      const T& angleZ = 0.0,
                      const T& translationX = 0.0,
                      const T& translationY = 0.0,
                      const T& translationZ = 0.0,
                      bool degree = false,
                      bool direct = true );
    RigidTransform3d( const Vector3d< T >& angles,
                      const Vector3d< T >& translation,
                      bool degree = false,
                      bool direct = true );
    RigidTransform3d( const std::vector< T >& parameters,
                      bool degree = false,
                      bool direct = true );
    RigidTransform3d( const RigidTransform3d< T >& other );
    virtual ~RigidTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    RigidTransform3d< T >& operator=( const RigidTransform3d< T >& other );

    void setDirectParameters( const T& angleX,
                              const T& angleY,
                              const T& angleZ,
                              const T& translationX,
                              const T& translationY,
                              const T& translationZ,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );
    void setDirectParameters( const Vector3d< T >& angles,
                              const Vector3d< T >& translation,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );
    void setDirectParameters( const std::vector< T >& parameters,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );

    void getDirectParameters( T& angleX,
                              T& angleY,
                              T& angleZ,
                              T& translationX,
                              T& translationY,
                              T& translationZ,
                              bool degree = false ) const;
    void getDirectParameters( Vector3d< T >& angles,
                              Vector3d< T >& translation,
                              bool degree = false ) const;
    void getDirectParameters( std::vector< T >& parameters,
                              bool degree = false ) const;

    void setInverseParameters( const T& angleX,
                               const T& angleY,
                               const T& angleZ,
                               const T& translationX,
                               const T& translationY,
                               const T& translationZ,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );
    void setInverseParameters( const Vector3d< T >& angles,
                               const Vector3d< T >& translation,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );
    void setInverseParameters( const std::vector< T >& parameters,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );

    void getInverseParameters( T& angleX,
                               T& angleY,
                               T& angleZ,
                               T& translationX,
                               T& translationY,
                               T& translationZ,
                               bool degree = false ) const;
    void getInverseParameters( Vector3d< T >& angles,
                               Vector3d< T >& translation,
                               bool degree = false ) const;
    void getInverseParameters( std::vector< T >& parameters,
                               bool degree = false ) const;

    const Translation3d< T >& getFromFovCenterTranslation3d() const;
    const Rotation3d< T >& getRotation3d() const;
    const Translation3d< T >& getTranslation3d() const;
    const Translation3d< T >& getToFovCenterInverseTranslation3d() const;

    T getNorm2() const;

    void setFromFieldOfViewCenter( const Vector3d< T >& fromFovCenter,
                                   bool updating = true,
                                   bool notify = true );
                                      
    void setToFieldOfViewCenter( const Vector3d< T >& toFovCenter,
                                 bool updating = true,
                                 bool notify = true );

    void read( std::istream& is );
    void readTrm( std::istream& is );
    void write( std::ostream& os ) const;
    void writeTrm( std::ostream& os ) const;

  protected:

    //
    // ( to + toFovCenter ) = R ( from + fromFovCenter ) + T
    // that gives using matrices:
    //
    // to = [toFovCenter]^-1 x [T] x [R] x [fromFovCenter] x from
    //

    RCPointer< Translation3d< T > > _fromFovCenterTranslation3d;

    RCPointer< Rotation3d< T > > _rotation3d;
    RCPointer< Translation3d< T > > _translation3d;

    RCPointer< Translation3d< T > > _toFovCenterInverseTranslation3d;

};


}


#endif
