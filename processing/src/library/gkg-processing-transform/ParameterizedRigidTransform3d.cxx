#include <gkg-processing-transform/ParameterizedRigidTransform3d.h>


template < class T >
gkg::ParameterizedRigidTransform3d< T >::ParameterizedRigidTransform3d()
                                      : gkg::ParameterizedTransform3d< T >(
                                   gkg::RCPointer< gkg::RigidTransform3d< T > >(
                                              new gkg::RigidTransform3d< T > ) )
{

  _rigidTransform3d = dynamic_cast< gkg::RigidTransform3d< T >* >(
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedRigidTransform3d< T >::ParameterizedRigidTransform3d(
                            const gkg::RigidTransform3d< T >& rigidTransform3d )
                                      : gkg::ParameterizedTransform3d< T >(
                                                          rigidTransform3d )
{

  _rigidTransform3d = dynamic_cast< gkg::RigidTransform3d< T >* >( 
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedRigidTransform3d< T >::ParameterizedRigidTransform3d(
                          const gkg::ParameterizedRigidTransform3d< T >& other )
                                      : gkg::ParameterizedTransform3d< T >(
                                               other.getRigidTransform3d() )
{

  _rigidTransform3d = dynamic_cast< gkg::RigidTransform3d< T >* >( 
                                                     this->_transform3d.get() );

}


template < class T >
gkg::ParameterizedRigidTransform3d< T >::~ParameterizedRigidTransform3d()
{
}


template < class T >
int32_t gkg::ParameterizedRigidTransform3d< T >::getParameterCount() const
{

  return 6;

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::ParameterizedRigidTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ParameterizedRigidTransform3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::ParameterizedRigidTransform3d< T >& 
gkg::ParameterizedRigidTransform3d< T >::operator=(
                          const gkg::ParameterizedRigidTransform3d< T >& other )
{

  //delete this->_transform3d;
  this->_transform3d = other._transform3d->clone();
  _rigidTransform3d = dynamic_cast< gkg::RigidTransform3d< T >* >( 
                                                     this->_transform3d.get() );
  return *this;

}


template < class T >
const gkg::RigidTransform3d< T >&
gkg::ParameterizedRigidTransform3d< T >::getRigidTransform3d() const
{

  return *_rigidTransform3d;

}


template < class T >
gkg::RigidTransform3d< T >& 
gkg::ParameterizedRigidTransform3d< T >::getRigidTransform3d()
{

  return *_rigidTransform3d;

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::getDirectParameters(
                                            std::vector< T >& parameters ) const
{

  _rigidTransform3d->getDirectParameters( parameters );

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::getInverseParameters(
                                            std::vector< T >& parameters ) const
{

  _rigidTransform3d->getInverseParameters( parameters );

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::readTrm( std::istream& is )
{

  _rigidTransform3d->readTrm( is );

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::writeTrm( std::ostream& os ) const
{

  _rigidTransform3d->writeTrm( os );

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::setDirectParametersImpl(
                                            const std::vector< T >& parameters )
{

  _rigidTransform3d->setDirectParameters( parameters );

}


template < class T >
void gkg::ParameterizedRigidTransform3d< T >::setInverseParametersImpl(
                                            const std::vector< T >& parameters )
{

  _rigidTransform3d->setInverseParameters( parameters );

}


template class gkg::ParameterizedRigidTransform3d< float >;
template class gkg::ParameterizedRigidTransform3d< double >;
