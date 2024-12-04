#ifndef _gkg_processing_transform_Scaling3d_h_
#define _gkg_processing_transform_Scaling3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class Scaling3d : public HomogeneousTransform3d< T >
{

  public:

    Scaling3d( const T& sx = 1.0, const T& sy = 1.0, const T& sz = 1.0,
               bool direct = true );
    Scaling3d( const Vector3d< T >& scaling, bool direct = true );
    Scaling3d( const Scaling3d< T >& other );
    virtual ~Scaling3d();

    Scaling3d< T >& operator = ( const Scaling3d< T >& other );

    virtual RCPointer< Transform3d< T > > clone() const;

    void setDirectScaling( const T& sx, const T& sy, const T& sz,
                           bool update = true,
                           bool notify = true );
    void setDirectScaling( const Vector3d< T >& scaling,
                           bool update = true,
                           bool notify = true );
    void getDirectScaling( Vector3d< T >& scaling ) const;
    void getDirectScaling( T& sx, T& sy, T& sz ) const;

    void setInverseScaling( const T& sx, const T& sy, const T& sz,
                            bool update = true,
                            bool notify = true );
    void setInverseScaling( const Vector3d< T >& scaling,
                            bool update = true,
                            bool notify = true );
    void getInverseScaling( T& sx, T& sy, T& sz ) const;
    void getInverseScaling( Vector3d< T >& scaling ) const;

};


}


#endif
