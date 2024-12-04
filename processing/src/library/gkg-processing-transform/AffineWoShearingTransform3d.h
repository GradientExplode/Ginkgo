#ifndef _gkg_processing_transform_AffineWoShearingTransform3d_h_
#define _gkg_processing_transform_AffineWoShearingTransform3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>
#include <vector>


namespace gkg
{


template < class T > class Scaling3d;
template < class T > class Rotation3d;
template < class T > class Translation3d;

//
// to = T . R . S . from
//

template < class T >
class AffineWoShearingTransform3d : public CompositeTransform3d< T >
{

  public:

    AffineWoShearingTransform3d(
                      const T& scalingX = 1.0,
                      const T& scalingY = 1.0,
                      const T& scalingZ = 1.0,
                      const T& angleX = 0.0,
                      const T& angleY = 0.0,
                      const T& angleZ = 0.0,
                      const T& translationX = 0.0,
                      const T& translationY = 0.0,
                      const T& translationZ = 0.0,
                      bool degree = false,
                      bool direct = true );
    AffineWoShearingTransform3d(
                      const Vector3d< T >& scalings,
                      const Vector3d< T >& angles,
                      const Vector3d< T >& translation,
                      bool degree = false,
                      bool direct = true );
    AffineWoShearingTransform3d(
                      const std::vector< T >& parameters,
                      bool degree = false,
                      bool direct = true );
    AffineWoShearingTransform3d(
                      const AffineWoShearingTransform3d< T >& other );
    virtual ~AffineWoShearingTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    AffineWoShearingTransform3d< T >& operator=(
                      const AffineWoShearingTransform3d< T >& other );

    void setDirectParameters( const T& scalingX,
                              const T& scalingY,
                              const T& scalingZ,
                              const T& angleX,
                              const T& angleY,
                              const T& angleZ,
                              const T& translationX,
                              const T& translationY,
                              const T& translationZ,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );
    void setDirectParameters( const Vector3d< T >& scalings,
                              const Vector3d< T >& angles,
                              const Vector3d< T >& translation,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );
    void setDirectParameters( const std::vector< T >& parameters,
                              bool degree = false,
                              bool updating = true,
                              bool notify = true );

    void getDirectParameters( T& scalingX,
                              T& scalingY,
                              T& scalingZ,
                              T& angleX,
                              T& angleY,
                              T& angleZ,
                              T& translationX,
                              T& translationY,
                              T& translationZ,
                              bool degree = false ) const;
    void getDirectParameters( Vector3d< T >& scalings,
                              Vector3d< T >& angles,
                              Vector3d< T >& translation,
                              bool degree = false ) const;
    void getDirectParameters( std::vector< T >& parameters,
                              bool degree = false ) const;

    void setInverseParameters( const T& scalingX,
                               const T& scalingY,
                               const T& scalingZ,
                               const T& angleX,
                               const T& angleY,
                               const T& angleZ,
                               const T& translationX,
                               const T& translationY,
                               const T& translationZ,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );
    void setInverseParameters( const Vector3d< T >& scalings,
                               const Vector3d< T >& angles,
                               const Vector3d< T >& translation,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );
    void setInverseParameters( const std::vector< T >& parameters,
                               bool degree = false,
                               bool updating = true,
                               bool notify = true );

    void getInverseParameters( T& scalingX,
                               T& scalingY,
                               T& scalingZ,
                               T& angleX,
                               T& angleY,
                               T& angleZ,
                               T& translationX,
                               T& translationY,
                               T& translationZ,
                               bool degree = false ) const;
    void getInverseParameters( Vector3d< T >& scalings,
                               Vector3d< T >& angles,
                               Vector3d< T >& translation,
                               bool degree = false ) const;
    void getInverseParameters( std::vector< T >& parameters,
                               bool degree = false ) const;

    const Scaling3d< T >& getScaling3d() const;
    const Rotation3d< T >& getRotation3d() const;
    const Translation3d< T >& getTranslation3d() const;

    T getNorm2() const;

    void read( std::istream& is );
    void readTrm( std::istream& is );
    void write( std::ostream& os ) const;
    void writeTrm( std::ostream& os ) const;

  protected:

    RCPointer< Scaling3d< T > > _scaling3d;
    RCPointer< Rotation3d< T > > _rotation3d;
    RCPointer< Translation3d< T > > _translation3d;

};


}


#endif
