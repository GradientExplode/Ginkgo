#include <gkg-processing-transform/IdentityTransform3d.h>


template < class T >
gkg::IdentityTransform3d< T >::IdentityTransform3d()
                              : gkg::HomogeneousTransform3d< T >()
{
}


template < class T >
gkg::IdentityTransform3d< T >::~IdentityTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::IdentityTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::IdentityTransform3d< T > );
  return transform;

}


template class gkg::IdentityTransform3d< float >;
template class gkg::IdentityTransform3d< double >;

