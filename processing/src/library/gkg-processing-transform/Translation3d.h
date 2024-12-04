#ifndef _gkg_processing_transform_Translation3d_h_
#define _gkg_processing_transform_Translation3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>


namespace gkg
{


template < class T >
class Translation3d : public HomogeneousTransform3d< T >
{

  public:

    Translation3d( const T& tx = 0.0, const T& ty = 0.0, const T& tz = 0.0,
                   bool direct = true );
    Translation3d( const Vector3d< T >& translation, bool direct = true );
    Translation3d( const Translation3d< T >& other );
    virtual ~Translation3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    void setDirectTranslation( const T& tx, const T& ty, const T& tz,
                               bool update = true,
                               bool notify = true );
    void setDirectTranslation( const Vector3d< T >& translation,
                               bool update = true,
                               bool notify = true );
    void getDirectTranslation( Vector3d< T >& translation ) const;
    void getDirectTranslation( T& tx, T& ty, T& tz ) const;

    void setInverseTranslation( const T& tx, const T& ty, const T& tz,
                                bool update = true,
                                bool notify = true );
    void setInverseTranslation( const Vector3d< T >& translation,
                                bool update = true,
                                bool notify = true );
    void getInverseTranslation( T& tx, T& ty, T& tz ) const;
    void getInverseTranslation( Vector3d< T >& translation ) const;

};


}


#endif
