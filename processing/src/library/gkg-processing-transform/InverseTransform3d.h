#ifndef _gkg_processing_transform_InverseTransform3d_h_
#define _gkg_processing_transform_InverseTransform3d_h_


#include <gkg-processing-transform/Transform3d.h>
#include <gkg-core-pattern/Observer.h>


namespace gkg
{


template < class T >
class InverseTransform3d : public Transform3d< T >,
                           public Observer
{

  public:

    InverseTransform3d( RCPointer< Transform3d< T > > transform3d );
    InverseTransform3d( const InverseTransform3d< T >& other );
    virtual ~InverseTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;


    void getDirect( const Vector3d< T >& from, Vector3d< T >& to ) const;
    void getInverse( const Vector3d< T >& to, Vector3d< T >& from ) const;

    bool isComposable() const;

    const Transform3d< T >& getDirectTransform3d() const;

  protected:

    // forbidden
    InverseTransform3d< T >& operator=( const InverseTransform3d< T >& );

    void update( const gkg::Observable* observable = 0, void* arg = 0 );
    void unregisterObservable( Observable* observable );

    RCPointer< Transform3d< T > > _directTransform3d;
    //bool _autoDelete;

};


}


#endif
