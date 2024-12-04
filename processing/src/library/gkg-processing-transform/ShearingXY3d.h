#ifndef _gkg_processing_transform_ShearingXY3d_h_
#define _gkg_processing_transform_ShearingXY3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class ShearingXY3d : public HomogeneousTransform3d< T >
{

  public:

    ShearingXY3d( const T& sxy = 0.0, bool direct = true );
    ShearingXY3d( const ShearingXY3d< T >& other );
    virtual ~ShearingXY3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    void setDirectShearingXY( const T& sxy,
                              bool update = true,
                              bool notify = true );
    void getDirectShearingXY( T& sxy ) const;

    void setInverseShearingXY( const T& sxy,
                              bool update = true,
                              bool notify = true );
    void getInverseShearingXY( T& sxy ) const;

};


}


#endif
