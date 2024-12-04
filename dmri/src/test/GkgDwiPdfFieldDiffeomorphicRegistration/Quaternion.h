#ifndef _gkg_processing_transform_Quaternion_h_
#define _gkg_processing_transform_Quaternion_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>

#include <stdint.h>


namespace gkg
{


template < class T >
class Quaternion : public HomogeneousTransform3d< T >
{

  public:

    typedef T Rotation[ 16 ];

    Quaternion();
    Quaternion( const Quaternion& q );
    Quaternion( T x, T y, T z, T w );
    Quaternion( const Matrix& rotation );
    virtual ~Quaternion();

    T operator [] ( int32_t i );
    Quaternion& operator *= ( Quaternion& q );
    Quaternion& operator += ( Quaternion& q );

    void set( T x, T y, T z, T w );
    void normalize();
    bool isClose( gkg::Quaternion< T >& other );
    void inverseSign();
    const Rotation& getRotation() const;
    void getRotationMatrix( gkg::Matrix& rotation ) const;
    const Rotation& getInverseRotation() const;
    gkg::Vector getComponents() const;

    template < class U >
    friend Quaternion< U > operator * ( Quaternion< U >& q1,
                                        Quaternion< U >& q2 );

  private:

    void getMatrix( Rotation& r, T s );
    void computeTransformation();

    T _q[ 4 ];
    Rotation _r;
    Rotation _ri;

};


}


#endif
