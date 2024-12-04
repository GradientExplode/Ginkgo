#ifndef _gkg_processing_transform_IdentityTransform3d_h_
#define _gkg_processing_transform_IdentityTransform3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T >
class IdentityTransform3d : public HomogeneousTransform3d< T >
{

  public:

    IdentityTransform3d();
    ~IdentityTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

  protected:

    void setIdentity( bool update = true, bool notify = true );

  private:

    // forbidden
    IdentityTransform3d( const IdentityTransform3d< T >& );
    IdentityTransform3d< T >& operator=( const IdentityTransform3d< T >& );

};


}


#endif
