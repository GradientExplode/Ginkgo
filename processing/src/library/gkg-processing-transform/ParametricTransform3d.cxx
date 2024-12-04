#include <gkg-processing-transform/ParametricTransform3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::ParametricTransform3d< T >::ParametricTransform3d( int32_t parameterCount )
                               : gkg::Transform3d< T >()
{

  try
  {

    if ( parameterCount > 0 )
    {

      _parameters.resize( parameterCount );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::ParametricTransform3d< T >::ParametricTransform3d( "
             "int32_t parameterCount )" );

}


template < class T > 
gkg::ParametricTransform3d<T>::ParametricTransform3d(
                               const gkg::ParametricTransform3d<T>& other )
                              : gkg::Transform3d<T>(),
                                _parameters( other._parameters )
{
}


template < class T >
gkg::ParametricTransform3d< T >::~ParametricTransform3d()
{
}


template < class T >
gkg::ParametricTransform3d< T >& 
gkg::ParametricTransform3d< T >::operator=(
                                  const gkg::ParametricTransform3d< T >& other )
{

  try
  {

    _parameters = other._parameters;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::ParametricTransform3d< T >& "
             "gkg::ParametricTransform3d< T >::operator=( "
             "const gkg::ParametricTransform3d< T >& other )" );

}



template < class T >
void gkg::ParametricTransform3d< T >::getParameters(
                                            std::vector< T >& parameters ) const
{

  try
  {

    parameters = _parameters;

  }
  GKG_CATCH( "template < class T > "
             "void "
             "gkg::ParametricTransform3d< T >::getParameters( "
             "std::vector< T >& parameters ) const" );

}


template class gkg::ParametricTransform3d< float >;
template class gkg::ParametricTransform3d< double >;
