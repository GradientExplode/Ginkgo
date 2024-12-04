#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Scaling3d< T >::Scaling3d( const T& sx, const T& sy, const T& sz,
                                bool direct )
                    : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectScaling( sx, sy, sz, true, false );

    }
    else
    {

      setInverseScaling( sx, sy, sz, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Scaling3d< T >::Scaling3d( const T& sx, const T& sy, "
             "const T& sz, bool direct )" );

}


template < class T >
gkg::Scaling3d< T >::Scaling3d( const gkg::Vector3d< T >& scaling, bool direct )
                    : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectScaling( scaling, true, false );

    }
    else
    {

      setInverseScaling( scaling, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Scaling3d< T >::Scaling3d( const "
             "gkg::Vector3d< T >& scaling, bool direct )" );

}


template < class T >
gkg::Scaling3d< T >::Scaling3d( const gkg::Scaling3d< T >& other )
                    : gkg::HomogeneousTransform3d< T >( other )
{
}


template < class T >
gkg::Scaling3d< T >::~Scaling3d()
{
}


template < class T >
gkg::Scaling3d< T >& gkg::Scaling3d< T >::operator = (
                                              const gkg::Scaling3d< T >& other )
{

  this->copy( other );
  return *this;

}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::Scaling3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::Scaling3d< T >( *this ) );
  return transform;

}


template < class T >
void gkg::Scaling3d< T >::setDirectScaling( const T& sx,
                                            const T& sy,
                                            const T& sz,
                                            bool update,
                                            bool notify )
{

  try
  {

    this->setDirectItems( sx, 0.0, 0.0, 0.0,
                          0.0, sy, 0.0, 0.0,
                          0.0, 0.0, sz, 0.0,
                          0.0, 0.0, 0.0, 1.0,
                          update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Scaling3d< T >::setDirectScaling( const T& sx, "
             "const T& sy, "
             "const T& sz, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Scaling3d< T >::setDirectScaling( const gkg::Vector3d< T >& scaling,
                                            bool update,
                                            bool notify )
{

  try
  {

    this->setDirectItems( scaling.x, 0.0, 0.0, 0.0,
                          0.0, scaling.y, 0.0, 0.0,
                          0.0, 0.0, scaling.z, 0.0,
                          0.0, 0.0, 0.0, 1.0,
                          update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Scaling3d< T >::setDirectScaling( "
             "const gkg::Vector3d< T >& scaling, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Scaling3d< T >::getDirectScaling( T& sx, T& sy, T& sz ) const
{

  sx = this->_directItems[ 0 ][ 0 ];
  sy = this->_directItems[ 1 ][ 1 ];
  sz = this->_directItems[ 2 ][ 2 ];
                            
}


template < class T >
void gkg::Scaling3d< T >::getDirectScaling( gkg::Vector3d< T >& scaling ) const
{

  getDirectScaling( scaling.x, scaling.y, scaling.z );
                            
}


template < class T >
void gkg::Scaling3d< T >::setInverseScaling( const T& sx,
                                             const T& sy,
                                             const T& sz,
                                             bool update,
                                             bool notify )
{

  try
  {

    this->setInverseItems( sx, 0.0, 0.0, 0.0,
                           0.0, sy, 0.0, 0.0,
                           0.0, 0.0, sz, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Scaling3d< T >::setInverseScaling( const T& sx, "
             "const T& sy, "
             "const T& sz, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Scaling3d< T >::setInverseScaling( const gkg::Vector3d< T >& scaling,
                                             bool update,
                                             bool notify )
{

  try
  {

    this->setInverseItems( scaling.x, 0.0, 0.0, 0.0,
                           0.0, scaling.y, 0.0, 0.0,
                           0.0, 0.0, scaling.z, 0.0,
                           0.0, 0.0, 0.0, 1.0,
                           update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Scaling3d< T >::setInverseScaling( "
             "const gkg::Vector3d< T >& scaling, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::Scaling3d< T >::getInverseScaling( T& sx, T& sy, T& sz ) const
{

  sx = this->_inverseItems[ 0 ][ 0 ];
  sy = this->_inverseItems[ 1 ][ 1 ];
  sz = this->_inverseItems[ 2 ][ 2 ];
                            
}


template < class T >
void gkg::Scaling3d< T >::getInverseScaling( gkg::Vector3d< T >& scaling ) const
{

  getInverseScaling( scaling.x, scaling.y, scaling.z );
                            
}


template class gkg::Scaling3d< float >;
template class gkg::Scaling3d< double >;
