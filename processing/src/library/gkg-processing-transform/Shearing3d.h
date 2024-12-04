#ifndef _gkg_processing_transform_Shearing3d_h_
#define _gkg_processing_transform_Shearing3d_h_


#include <gkg-processing-transform/CompositeTransform3d.h>


namespace gkg
{


template < class T > class ShearingXY3d;
template < class T > class ShearingXZ3d;
template < class T > class ShearingYZ3d;
class Matrix;


//
// Sh = Shyz.Shxz.Shxy
//
template < class T >
class Shearing3d : public CompositeTransform3d< T >
{

  public:

    Shearing3d( const T& sxy = 0.0,
                const T& sxz = 0.0,
                const T& syz = 0.0,
                bool direct = true );
    Shearing3d( const Vector3d< T >& shearings,
                bool direct = true );
    Shearing3d( const Shearing3d< T >& other );
    virtual ~Shearing3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    Shearing3d< T >& operator=( const Shearing3d< T >& other );

    void setDirectShearings( const T& sxy,
                             const T& sxz,
                             const T& syz,
                             bool update = true,
                             bool notify = true );
    void setDirectShearings( const Vector3d< T >& angles,
                             bool update = true,
                             bool notify = true );
    void getDirectShearings( T& sxy,
                             T& sxz,
                             T& syz ) const;
    void getDirectShearings( Vector3d< T >& shearings ) const;

    void setInverseShearings( const T& sxy,
                              const T& sxz,
                              const T& syz,
                              bool update = true,
                              bool notify = true );
    void setInverseShearings( const Vector3d< T >& shearings,
                              bool update = true,
                              bool notify = true );
    void getInverseShearings( T& sxy,
                              T& sxz,
                              T& syz ) const;
    void getInverseShearings( Vector3d< T >& shearings ) const;

    const ShearingXY3d< T >& getShearingXY3d() const;
    const ShearingXZ3d< T >& getShearingXZ3d() const;
    const ShearingYZ3d< T >& getShearingYZ3d() const;

  protected:

    RCPointer< ShearingXY3d< T > > _shearingXY3d;
    RCPointer< ShearingXZ3d< T > > _shearingXZ3d;
    RCPointer< ShearingYZ3d< T > > _shearingYZ3d;

};


}


#endif
