#ifndef _gkg_processing_transform_Quaternion_h_
#define _gkg_processing_transform_Quaternion_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>

#include <stdint.h>


namespace gkg
{


template < class T >
class Quaternion : public HomogeneousTransform3d< T >
{

  public:

    typedef T Rotation[ 16 ];

    Quaternion();
    Quaternion( const Quaternion< T >& q );
    Quaternion( T x, T y, T z, T w );
    virtual ~Quaternion();
    
    Quaternion< T >& operator = ( const Quaternion< T >& other );

    T operator [] ( int32_t i );
    Quaternion& operator *= ( Quaternion& q );

    void set( T x, T y, T z, T w );
    void normalize();
    const Rotation& getRotation() const;
    const Rotation& getInverseRotation() const;

  private:

    void getMatrix( Rotation& r, T s );
    void computeTransformation();

    T _q[ 4 ];
    Rotation _r;
    Rotation _ri;

};


}


template < class T >
gkg::Quaternion< T > operator * ( gkg::Quaternion< T >& q1,
                                  gkg::Quaternion< T >& q2 );


#endif
