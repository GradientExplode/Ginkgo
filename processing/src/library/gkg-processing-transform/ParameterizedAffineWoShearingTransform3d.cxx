#include <gkg-processing-transform/ParameterizedAffineWoShearingTransform3d.h>


template < class T >
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                     ParameterizedAffineWoShearingTransform3d()
                                  : gkg::ParameterizedTransform3d< T >(
                             gkg::RCPointer< AffineWoShearingTransform3d< T > >(
                                   new gkg::AffineWoShearingTransform3d< T > ) )
{

  _affineWoShearingTransform3d =
    dynamic_cast< gkg::AffineWoShearingTransform3d< T >* >(
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                      ParameterizedAffineWoShearingTransform3d(
                                   const gkg::AffineWoShearingTransform3d< T >&
                                                  affineWoShearingTransform3d )
                                          : gkg::ParameterizedTransform3d< T >(
                                                  affineWoShearingTransform3d )
{

  _affineWoShearingTransform3d =
    dynamic_cast< gkg::AffineWoShearingTransform3d< T >* >(
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                      ParameterizedAffineWoShearingTransform3d(
                      const gkg::ParameterizedAffineWoShearingTransform3d< T >&
                                                                        other )
                                          : gkg::ParameterizedTransform3d< T >(
                                       other.getAffineWoShearingTransform3d() )
{

  _affineWoShearingTransform3d =
    dynamic_cast< gkg::AffineWoShearingTransform3d< T >* >(
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                    ~ParameterizedAffineWoShearingTransform3d()
{
}


template < class T >
int32_t 
gkg::ParameterizedAffineWoShearingTransform3d< T >::getParameterCount() const
{

  return 9;

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::ParameterizedAffineWoShearingTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset(
              new gkg::ParameterizedAffineWoShearingTransform3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::ParameterizedAffineWoShearingTransform3d< T >& 
gkg::ParameterizedAffineWoShearingTransform3d< T >::operator=(
              const gkg::ParameterizedAffineWoShearingTransform3d< T >& other )
{

  this->_transform3d = other._transform3d->clone();
  _affineWoShearingTransform3d =
    dynamic_cast< gkg::AffineWoShearingTransform3d< T >* >(
                                                     this->_transform3d.get() );
  return *this;

}


template < class T >
const gkg::AffineWoShearingTransform3d< T >&
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                         getAffineWoShearingTransform3d() const
{

  return *_affineWoShearingTransform3d;

}


template < class T >
gkg::AffineWoShearingTransform3d< T >& 
gkg::ParameterizedAffineWoShearingTransform3d< T >::
                                               getAffineWoShearingTransform3d()
{

  return *_affineWoShearingTransform3d;

}


template < class T >
void gkg::ParameterizedAffineWoShearingTransform3d< T >::getDirectParameters(
                                           std::vector< T >& parameters ) const
{

  _affineWoShearingTransform3d->getDirectParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineWoShearingTransform3d< T >::getInverseParameters(
                                           std::vector< T >& parameters ) const
{

  _affineWoShearingTransform3d->getInverseParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineWoShearingTransform3d< T >::readTrm( std::istream& is )
{

  _affineWoShearingTransform3d->readTrm( is );

}


template < class T >
void gkg::ParameterizedAffineWoShearingTransform3d< T >::writeTrm(
                                                       std::ostream& os ) const
{

  _affineWoShearingTransform3d->writeTrm( os );

}


template < class T >
void 
gkg::ParameterizedAffineWoShearingTransform3d< T >::setDirectParametersImpl(
                                           const std::vector< T >& parameters )
{

  _affineWoShearingTransform3d->setDirectParameters( parameters );

}


template < class T >
void 
gkg::ParameterizedAffineWoShearingTransform3d< T >::setInverseParametersImpl(
                                           const std::vector< T >& parameters )
{

  _affineWoShearingTransform3d->setInverseParameters( parameters );

}


template class gkg::ParameterizedAffineWoShearingTransform3d< float >;
template class gkg::ParameterizedAffineWoShearingTransform3d< double >;
