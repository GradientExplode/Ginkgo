#include <gkg-processing-transform/ParameterizedTransform3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ParameterizedTransform3d< T >::ParameterizedTransform3d(
                           gkg::RCPointer< gkg::Transform3d< T > > transform3d )
                                   : gkg::Transform3d< T >(),
                                     _transform3d( transform3d )
{
}


template < class T >
gkg::ParameterizedTransform3d< T >::ParameterizedTransform3d(
                                      const gkg::Transform3d< T >& transform3d )
                                   : gkg::Transform3d< T >(),
                                     _transform3d( transform3d.clone() )
{
}


template < class T >
gkg::ParameterizedTransform3d< T >::~ParameterizedTransform3d()
{

  //delete _transform3d;

}


template < class T >
void gkg::ParameterizedTransform3d< T >::getDirect(
                                                 const gkg::Vector3d< T >& from, 
                                                 gkg::Vector3d< T >& to ) const
{

  _transform3d->getDirect( from, to );

}


template < class T >
void gkg::ParameterizedTransform3d< T >::getInverse(
                                                 const gkg::Vector3d< T >& from, 
                                                 gkg::Vector3d< T >& to ) const
{

  _transform3d->getInverse( from, to );

}


template < class T >
bool gkg::ParameterizedTransform3d< T >::isComposable() const
{

  return _transform3d->isComposable();

}


template < class T >
void gkg::ParameterizedTransform3d< T >::setDirectParameters(
                                            const std::vector< T >& parameters,
                                            bool update )
{

  setDirectParametersImpl( parameters );
  if ( update )
  {

    this->setChanged();

  }
  this->notifyObservers( this );

}


template < class T >
void gkg::ParameterizedTransform3d< T >::setDirectParameters(
                                            const gkg::Vector& parameters,
                                            bool update )
{

  int32_t parameterCount = parameters.getSize();
  std::vector< T > vectorOfParameters( parameterCount );
  int32_t p;
  for ( p = 0; p < parameterCount; p++ )
  {

    vectorOfParameters[ p ] = ( T )parameters( p );

  }

  setDirectParameters( vectorOfParameters, update );

}


template < class T >
void gkg::ParameterizedTransform3d< T >::getDirectParameters(
                                                 gkg::Vector& parameters ) const
{

  std::vector< T > tmpParameters;
  this->getDirectParameters( tmpParameters );
  int32_t parameterCount = ( int32_t )tmpParameters.size();

  parameters.reallocate( parameterCount );
  int32_t p;
  for ( p = 0; p < parameterCount; p++ )
  {

    parameters( p ) = ( double )tmpParameters[ p ];

  }

}


template < class T >
void gkg::ParameterizedTransform3d< T >::setInverseParameters(
                                            const std::vector< T >& parameters,
                                            bool update )
{

  setInverseParametersImpl( parameters );
  if ( update )
  {

    this->setChanged();

  }
  this->notifyObservers( this );

}


template < class T >
void gkg::ParameterizedTransform3d< T >::setInverseParameters(
                                            const gkg::Vector& parameters,
                                            bool update )
{

  int32_t parameterCount = parameters.getSize();
  std::vector< T > vectorOfParameters( parameterCount );
  int32_t p;
  for ( p = 0; p < parameterCount; p++ )
  {

    vectorOfParameters[ p ] = ( T )parameters( p );

  }

  setInverseParameters( vectorOfParameters, update );

}


template < class T >
void gkg::ParameterizedTransform3d< T >::getInverseParameters(
                                                 gkg::Vector& parameters ) const
{

  std::vector< T > tmpParameters;
  this->getInverseParameters( tmpParameters );
  int32_t parameterCount = ( int32_t )tmpParameters.size();

  parameters.reallocate( parameterCount );
  int32_t p;
  for ( p = 0; p < parameterCount; p++ )
  {

    parameters( p ) = ( double )tmpParameters[ p ];

  }

}


template < class T >
void gkg::ParameterizedTransform3d< T >::getDerivativeAlongParameters(
                                       const gkg::Vector3d< T >& /*from*/,
                                       std::vector< T >& /*derivatives*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ParameterizedTransform3d< T >::"
             "getDerivativeAlongParameters( "
             "const gkg::Vector3d< T >& /*from*/, "
             "std::vector< T >& /*derivatives*/ ) cons" );

}


template < class T >
 void gkg::ParameterizedTransform3d< T >::getDerivativeAlongParameters(
                                             const gkg::Vector3d< T >& /*from*/,
                                             gkg::Vector& /*derivatives*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ParameterizedTransform3d< T >::"
             "getDerivativeAlongParameters( "
             "const gkg::Vector3d< T >& /*from*/, "
             "gkg::Vector& /*derivatives*/ ) const" );

}


template < class T >
T gkg::ParameterizedTransform3d< T >::getDerivativeAlongParameter(
                                             const gkg::Vector3d< T >& /*from*/,
                                             int32_t /*parameterIndex*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

    return ( T )0.0;

  }
  GKG_CATCH( "template < class T > "
             "T gkg::ParameterizedTransform3d< T >::"
             "getDerivativeAlongParameter( "
             "const gkg::Vector3d< T >& /*from*/, "
             "int32_t /*parameterIndex*/ ) const" );

}


template < class T >
bool gkg::ParameterizedTransform3d< T >::hasDerivativeAlongParameters() const
{

  try
  {

    return false;

  }
  GKG_CATCH( "template < class T > "
             "bool gkg::ParameterizedTransform3d< T >::"
             "hasDerivativeAlongParameters() const" );

}


template class gkg::ParameterizedTransform3d< float >;
template class gkg::ParameterizedTransform3d< double >;
