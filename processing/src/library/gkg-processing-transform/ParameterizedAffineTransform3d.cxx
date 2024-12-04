#include <gkg-processing-transform/ParameterizedAffineTransform3d.h>


template < class T >
gkg::ParameterizedAffineTransform3d< T >::ParameterizedAffineTransform3d()
                                         : gkg::ParameterizedTransform3d< T >(
                                  gkg::RCPointer< gkg::AffineTransform3d< T > >(
                                             new gkg::AffineTransform3d< T > ) )
{

  _affineTransform3d =
    dynamic_cast< gkg::AffineTransform3d< T >* >( this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineTransform3d< T >::ParameterizedAffineTransform3d(
                                            const gkg::AffineTransform3d< T >&
                                                            affineTransform3d )
                                          : gkg::ParameterizedTransform3d< T >(
                                                             affineTransform3d )
{

  _affineTransform3d =
      dynamic_cast< gkg::AffineTransform3d< T >* >( this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineTransform3d< T >::ParameterizedAffineTransform3d(
                         const gkg::ParameterizedAffineTransform3d< T >& other )
                                          : gkg::ParameterizedTransform3d< T >(
                                                  other.getAffineTransform3d() )
{

  _affineTransform3d =
      dynamic_cast< gkg::AffineTransform3d< T >* >( this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineTransform3d< T >::~ParameterizedAffineTransform3d()
{
}


template < class T >
int32_t 
gkg::ParameterizedAffineTransform3d< T >::getParameterCount() const
{

  return 12;

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::ParameterizedAffineTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ParameterizedAffineTransform3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::ParameterizedAffineTransform3d< T >& 
gkg::ParameterizedAffineTransform3d< T >::operator=(
                         const gkg::ParameterizedAffineTransform3d< T >& other )
{

  //delete this->_transform3d;
  this->_transform3d = other._transform3d->clone();
  _affineTransform3d =
      dynamic_cast< gkg::AffineTransform3d< T >* >( this->_transform3d.get() );
  return *this;

}


template < class T >
const gkg::AffineTransform3d< T >&
gkg::ParameterizedAffineTransform3d< T >::getAffineTransform3d() const
{

  return *_affineTransform3d;

}


template < class T >
gkg::AffineTransform3d< T >& 
gkg::ParameterizedAffineTransform3d< T >::getAffineTransform3d()
{

  return *_affineTransform3d;

}


template < class T >
void gkg::ParameterizedAffineTransform3d< T >::getDirectParameters(
                                           std::vector< T >& parameters ) const
{

  _affineTransform3d->getDirectParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineTransform3d< T >::getInverseParameters(
                                           std::vector< T >& parameters ) const
{

  _affineTransform3d->getInverseParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineTransform3d< T >::readTrm( std::istream& is )
{

  _affineTransform3d->readTrm( is );

}


template < class T >
void gkg::ParameterizedAffineTransform3d< T >::writeTrm(
                                                       std::ostream& os ) const
{

  _affineTransform3d->writeTrm( os );

}


template < class T >
void 
gkg::ParameterizedAffineTransform3d< T >::setDirectParametersImpl(
                                           const std::vector< T >& parameters )
{

  _affineTransform3d->setDirectParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineTransform3d< T >::setInverseParametersImpl(
                                           const std::vector< T >& parameters )
{

  _affineTransform3d->setInverseParameters( parameters );

}


template class gkg::ParameterizedAffineTransform3d< float >;
template class gkg::ParameterizedAffineTransform3d< double >;
