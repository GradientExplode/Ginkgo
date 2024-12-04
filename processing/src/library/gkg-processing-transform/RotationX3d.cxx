#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class T >
gkg::RotationX3d< T >::RotationX3d( const T& angle, bool degree, bool direct )
                      : gkg::HomogeneousTransform3d< T >()
{

  try
  {

    if ( direct )
    {

      setDirectAngle( angle, degree, true, false );

    }
    else
    {

      setInverseAngle( angle, degree, true, false );

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::RotationX3d< T >::RotationX3d( "
             "const T& angle, bool degree, bool direct ) " );

}


template < class T >
gkg::RotationX3d< T >::RotationX3d( const gkg::RotationX3d< T >& other )
                      : gkg::HomogeneousTransform3d< T >( other ),
                        _directAngle( other._directAngle ),
                        _inverseAngle( other._inverseAngle )
{
}


template < class T >
gkg::RotationX3d< T >::~RotationX3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::RotationX3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::RotationX3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::RotationX3d< T >& 
gkg::RotationX3d< T >::operator=( const gkg::RotationX3d< T >& other )
{

  this->gkg::HomogeneousTransform3d< T >::operator=( other );
  _directAngle = other._directAngle;
  _inverseAngle = other._inverseAngle;
  return *this;

}


template < class T >
void gkg::RotationX3d< T >::setDirectAngle( const T& angle,
                                            bool degree,
                                            bool update,
                                            bool notify )
{

  try
  {

    _directAngle = angle;
    if ( degree )
    {

      _directAngle *= M_PI / 180.0;

    }
    _inverseAngle = -_directAngle;
    this->setDirectItems(
                  1.0, 0.0, 0.0, 0.0,
                  0.0, std::cos( _directAngle ), -std::sin( _directAngle ), 0.0,
                  0.0, std::sin( _directAngle ), std::cos( _directAngle ), 0.0,
                  0.0, 0.0, 0.0, 1.0,
                  update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RotationX3d< T >::setDirectAngle( const T& angle, "
             "bool degree, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::RotationX3d< T >::getDirectAngle( T& angle, bool degree ) const
{

  angle = _directAngle;
  if ( degree )
  {

    angle *= 180.0 / M_PI;

  }
                            
}


template < class T >
void gkg::RotationX3d< T >::setInverseAngle( const T& angle,
                                             bool degree,
                                             bool update,
                                             bool notify )
{

  try
  {

    _inverseAngle = angle;
    if ( degree )
    {

      _inverseAngle *= M_PI / 180.0;

    }
    _directAngle = -_inverseAngle;
    this->setInverseItems(
                1.0, 0.0, 0.0, 0.0,
                0.0, std::cos( _inverseAngle ), -std::sin( _inverseAngle ), 0.0,
                0.0, std::sin( _inverseAngle ), std::cos( _inverseAngle ), 0.0,
                0.0, 0.0, 0.0, 1.0,
                update, notify );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RotationX3d< T >::setInverseAngle( const T& angle, "
             "bool degree, "
             "bool update, "
             "bool notify )" );

}


template < class T >
void gkg::RotationX3d< T >::getInverseAngle( T& angle, bool degree ) const
{

  angle = _inverseAngle;
  if ( degree )
  {

    angle *= 180.0 / M_PI;

  }
                            
}


template class gkg::RotationX3d< float >;
template class gkg::RotationX3d< double >;
