#ifndef _gkg_processing_transform_CompositeTransform3d_h_
#define _gkg_processing_transform_CompositeTransform3d_h_


#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-core-pattern/Observer.h>
#include <deque>


namespace gkg
{


template < class T >
class CompositeTransform3d : public Transform3d< T >,
                             public Observer
{

  public:

    CompositeTransform3d();
    CompositeTransform3d( const CompositeTransform3d< T >& other );
    virtual ~CompositeTransform3d();

    virtual RCPointer< Transform3d< T > > clone() const;

    CompositeTransform3d< T >& 
      operator=( const CompositeTransform3d< T >& other );

    // to = _composition[ n ] x ... x _composition[ 0 ] x from
    void getDirect( const Vector3d< T >& from, 
                    Vector3d< T >& to ) const;
    // to = _composition[ 0 ]^(-1) x ... x _composition[ n ]^(-1) x to
    void getInverse( const Vector3d< T >& to, 
                     Vector3d< T >& from ) const;

    bool isComposable() const;
    bool isIndirect() const;

    void compose( const Transform3d< T >& other );
    void compose( RCPointer< Transform3d< T > > other,
                  bool cloning = true,
                  bool updating = true,
                  bool notify = true );

    void compose( const std::deque< RCPointer< Transform3d< T > > >& others,
                  const std::deque< bool >& clonings,
                  bool updating = true,
                  bool notify = true );

    const HomogeneousTransform3d< T >& getComposition() const;

  protected:

    void removeTransforms();
    void update( const Observable* observable = 0, void* arg = 0 );
    void unregisterObservable( Observable* observable );

    bool _isComposable;
    HomogeneousTransform3d< T > _composition;
    std::deque< RCPointer< Transform3d< T > > > _transforms;

};


}


#endif
