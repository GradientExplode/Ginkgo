#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>


template < class T >
gkg::InverseTransform3d< T >::InverseTransform3d(
                           gkg::RCPointer< gkg::Transform3d< T > > transform3d )
                             : gkg::Transform3d< T >(),
                               gkg::Observer(),
                               _directTransform3d( transform3d )

{

  _directTransform3d->addObserver( this );

}


template < class T >
gkg::InverseTransform3d< T >::InverseTransform3d(
                                     const gkg::InverseTransform3d< T >& other )
                             : gkg::Transform3d< T >(),
                               gkg::Observer(),
                               _directTransform3d( other._directTransform3d )
{

  _directTransform3d->addObserver( this );


}


template < class T >
gkg::InverseTransform3d< T >::~InverseTransform3d()
{

  _directTransform3d->removeObserver( this );

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::InverseTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::InverseTransform3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::InverseTransform3d< T >::getDirect( const gkg::Vector3d< T >& from,
                                              gkg::Vector3d< T >& to ) const
{

  _directTransform3d->getInverse( from, to );

}


template < class T >
void gkg::InverseTransform3d< T >::getInverse( const gkg::Vector3d< T >& to,
                                               gkg::Vector3d< T >& from ) const
{

  _directTransform3d->getDirect( to, from );

}


template < class T >
bool gkg::InverseTransform3d< T >::isComposable() const
{

  return _directTransform3d->isComposable();

}


template < class T >
const gkg::Transform3d< T >& 
gkg::InverseTransform3d< T >::getDirectTransform3d() const
{

  return *_directTransform3d;

}


template < class T >
void gkg::InverseTransform3d< T >::update(
                                          const gkg::Observable* /*observable*/,
                                          void* arg )
{

  bool notify = true;
  if ( arg )
  {

    bool* notifyPointer = static_cast< bool* >( arg );
    if ( notifyPointer )
    {

      notify = *notifyPointer;

    }

  }
  this->setChanged();
  if ( notify )
  {

    this->notifyObservers( this );

  }

}


template < class T >
void 
gkg::InverseTransform3d< T >::unregisterObservable( Observable* /*observable*/ )
{

  _directTransform3d->removeObserver( this );
  _directTransform3d.reset( new gkg::IdentityTransform3d< T > );
  _directTransform3d->addObserver( this );
//_directTransform3d->clone();

}


template class gkg::InverseTransform3d< float >;
template class gkg::InverseTransform3d< double >;
