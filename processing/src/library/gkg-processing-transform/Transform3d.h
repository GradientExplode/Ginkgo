#ifndef _gkg_processing_transform_Transform3d_h_
#define _gkg_processing_transform_Transform3d_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-pattern/Observable.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class T >
class Transform3d : public Observable
{

  public:

    virtual ~Transform3d();

    virtual RCPointer< Transform3d< T > > clone() const = 0;

    virtual void getDirect( const Vector3d< T >& from, 
                            Vector3d< T >& to ) const = 0;
    virtual void getInverse( const Vector3d< T >& to, 
                             Vector3d< T >& from ) const = 0;

    virtual void getIntegerDirect( const Vector3d< T >& from, 
                                   Vector3d< int32_t >& to ) const;
    virtual void getIntegerInverse( const Vector3d< T >& to, 
                                    Vector3d< int32_t >& from ) const;
    virtual void getFullIntegerDirect( const Vector3d< int32_t >& from, 
                                       Vector3d< int32_t >& to ) const;
    virtual void getFullIntegerInverse( const Vector3d< int32_t >& to, 
                                        Vector3d< int32_t >& from ) const;

    virtual T getDerivativeAlongX( const Vector3d< T >& from ) const;
    virtual T getDerivativeAlongY( const Vector3d< T >& from ) const;
    virtual T getDerivativeAlongZ( const Vector3d< T >& from ) const;
    virtual void getDerivativeAlongXYZ(
                                      const Vector3d< T >& from,
                                      Vector3d< T >& derivativeAlongXYZ ) const;

    virtual bool isComposable() const;
    virtual bool hasDerivativeAlongX() const;
    virtual bool hasDerivativeAlongY() const;
    virtual bool hasDerivativeAlongZ() const;
    virtual bool hasDerivativeAlongXYZ() const;

    virtual bool isIndirect() const;

  protected:

    Transform3d();

};


}


#endif
