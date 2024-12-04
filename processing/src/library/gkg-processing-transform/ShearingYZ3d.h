#ifndef _gkg_processing_transform_ShearingYZ3d_h_
#define _gkg_processing_transform_ShearingYZ3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class ShearingYZ3d : public HomogeneousTransform3d< T >
{

  public:

    ShearingYZ3d( const T& syz = 0.0, bool direct = true );
    ShearingYZ3d( const ShearingYZ3d< T >& other );
    virtual ~ShearingYZ3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    void setDirectShearingYZ( const T& syz,
                              bool update = true,
                              bool notify = true );
    void getDirectShearingYZ( T& syz ) const;

    void setInverseShearingYZ( const T& syz,
                              bool update = true,
                              bool notify = true );
    void getInverseShearingYZ( T& syz ) const;

};


}


#endif
