#ifndef _gkg_processing_transform_ShearingXZ3d_h_
#define _gkg_processing_transform_ShearingXZ3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class ShearingXZ3d : public HomogeneousTransform3d< T >
{

  public:

    ShearingXZ3d( const T& sxz = 0.0, bool direct = true );
    ShearingXZ3d( const ShearingXZ3d< T >& other );
    virtual ~ShearingXZ3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    void setDirectShearingXZ( const T& sxz,
                              bool update = true,
                              bool notify = true );
    void getDirectShearingXZ( T& sxz ) const;

    void setInverseShearingXZ( const T& sxz,
                               bool update = true,
                               bool notify = true );
    void getInverseShearingXZ( T& sxz ) const;

};


}


#endif
