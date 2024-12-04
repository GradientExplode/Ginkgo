#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Rotation3d< T >::Rotation3d( const T& angleX,
                                  const T& angleY,
                                  const T& angleZ,
                                  bool degree,
                                  bool direct )
                     : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _rotationX3d.reset( new gkg::RotationX3d< T >( angleX, degree, direct ) );
  _rotationY3d.reset( new gkg::RotationY3d< T >( angleY, degree, direct ) );
  _rotationZ3d.reset( new gkg::RotationZ3d< T >( angleZ, degree, direct ) );

  if ( direct )
  {
  
    transforms.push_back( _rotationX3d );
    transforms.push_back( _rotationY3d );
    transforms.push_back( _rotationZ3d );
    
  }
  else
  {
  
    transforms.push_back( _rotationZ3d );
    transforms.push_back( _rotationY3d );
    transforms.push_back( _rotationX3d );
    
  }

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::Rotation3d< T >::Rotation3d( const gkg::Vector3d< T >& angles,
                                  bool degree,
                                  bool direct )
                     : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _rotationX3d.reset( new gkg::RotationX3d< T >( angles.x, degree, direct ) );
  _rotationY3d.reset( new gkg::RotationY3d< T >( angles.y, degree, direct ) );
  _rotationZ3d.reset( new gkg::RotationZ3d< T >( angles.z, degree, direct ) );

  if ( direct )
  {
  
    transforms.push_back( _rotationX3d );
    transforms.push_back( _rotationY3d );
    transforms.push_back( _rotationZ3d );
    
  }
  else
  {
  
    transforms.push_back( _rotationZ3d );
    transforms.push_back( _rotationY3d );
    transforms.push_back( _rotationX3d );
    
  }

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::Rotation3d< T >::Rotation3d( const gkg::Rotation3d< T >& other )
                     : gkg::CompositeTransform3d< T >( other )
{

  _rotationX3d = other._transforms[ 0 ];
  _rotationY3d = other._transforms[ 1 ];
  _rotationZ3d = other._transforms[ 2 ];

}


template < class T >
gkg::Rotation3d< T >::~Rotation3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::Rotation3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::Rotation3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::Rotation3d< T >& 
gkg::Rotation3d< T >::operator=( const gkg::Rotation3d< T >& other )
{

  this->gkg::CompositeTransform3d< T >::operator=( other );

  _rotationX3d = other._transforms[ 0 ];
  _rotationY3d = other._transforms[ 1 ];
  _rotationZ3d = other._transforms[ 2 ];

  return *this;

}


template < class T >
void gkg::Rotation3d< T >::setDirectAngles( const T& angleX,
                                            const T& angleY,
                                            const T& angleZ,
                                            bool degree,
                                            bool updating,
                                            bool notify )
{

  _rotationX3d->setDirectAngle( angleX, degree, updating, false );
  _rotationY3d->setDirectAngle( angleY, degree, updating, false );
  _rotationZ3d->setDirectAngle( angleZ, degree, updating, false );
  if ( updating )
  {

    this->update( 0, &notify );

  }
  if ( !updating && notify )
  {

    this->notifyObservers( this );

  }

}


template < class T >
void gkg::Rotation3d< T >::setDirectAngles( const gkg::Vector3d< T >& angles,
                                            bool degree,
                                            bool updating,
                                            bool notify )
{

  _rotationX3d->setDirectAngle( angles.x, degree, updating, false );
  _rotationY3d->setDirectAngle( angles.y, degree, updating, false );
  _rotationZ3d->setDirectAngle( angles.z, degree, updating, false );
  if ( updating )
  {

    this->update( 0, &notify );

  }
  if ( !updating && notify )
  {

    this->notifyObservers( this );

  }

}


template < class T >
void gkg::Rotation3d< T >::getDirectAngles( T& angleX,
                                            T& angleY,
                                            T& angleZ,
                                            bool degree ) const
{

  _rotationX3d->getDirectAngle( angleX, degree );
  _rotationY3d->getDirectAngle( angleY, degree );
  _rotationZ3d->getDirectAngle( angleZ, degree );

}


template < class T >
void gkg::Rotation3d< T >::getDirectAngles( gkg::Vector3d< T >& angles,
                                            bool degree ) const
{

  _rotationX3d->getDirectAngle( angles.x, degree );
  _rotationY3d->getDirectAngle( angles.y, degree );
  _rotationZ3d->getDirectAngle( angles.z, degree );

}


template < class T >
void gkg::Rotation3d< T >::setInverseAngles( const T& angleX,
                                             const T& angleY,
                                             const T& angleZ,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  _rotationX3d->setInverseAngle( angleX, degree, updating, false );
  _rotationY3d->setInverseAngle( angleY, degree, updating, false );
  _rotationZ3d->setInverseAngle( angleZ, degree, updating, false );
  if ( updating )
  {

    this->update( 0, &notify );

  }
  if ( !updating && notify )
  {

    this->notifyObservers( this );

  }

}


template < class T >
void gkg::Rotation3d< T >::setInverseAngles( const gkg::Vector3d< T >& angles,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  _rotationX3d->setInverseAngle( angles.x, degree, updating, false );
  _rotationY3d->setInverseAngle( angles.y, degree, updating, false );
  _rotationZ3d->setInverseAngle( angles.z, degree, updating, false );
  if ( updating )
  {

    this->update( 0, &notify );

  }
  if ( !updating && notify )
  {

    this->notifyObservers( this );

  }

}


template < class T >
void gkg::Rotation3d< T >::getInverseAngles( T& angleX,
                                             T& angleY,
                                             T& angleZ,
                                             bool degree ) const
{

  _rotationX3d->getInverseAngle( angleX, degree );
  _rotationY3d->getInverseAngle( angleY, degree );
  _rotationZ3d->getInverseAngle( angleZ, degree );

}


template < class T >
void gkg::Rotation3d< T >::getInverseAngles( gkg::Vector3d< T >& angles,
                                             bool degree ) const
{

  _rotationX3d->getInverseAngle( angles.x, degree );
  _rotationY3d->getInverseAngle( angles.y, degree );
  _rotationZ3d->getInverseAngle( angles.z, degree );

}


template < class T >
const gkg::RotationX3d< T >& gkg::Rotation3d< T >::getRotationX3d() const
{

  return *_rotationX3d;

}


template < class T >
const gkg::RotationY3d< T >& gkg::Rotation3d< T >::getRotationY3d() const
{

  return *_rotationY3d;

}


template < class T >
const gkg::RotationZ3d< T >& gkg::Rotation3d< T >::getRotationZ3d() const
{

  return *_rotationZ3d;

}


template < class T >
void gkg::Rotation3d< T >::getRodriguesParameters( T& theta,
                                                   gkg::Matrix& Omega,
                                                   bool degree ) const
{

  try
  {

    // copying rotation into a matrix
    gkg::Matrix R( 3, 3 );
    R( 0, 0 ) = this->_composition.getDirectCoefficient( 0, 0 );
    R( 0, 1 ) = this->_composition.getDirectCoefficient( 0, 1 );
    R( 0, 2 ) = this->_composition.getDirectCoefficient( 0, 2 );

    R( 1, 0 ) = this->_composition.getDirectCoefficient( 1, 0 );
    R( 1, 1 ) = this->_composition.getDirectCoefficient( 1, 1 );
    R( 1, 2 ) = this->_composition.getDirectCoefficient( 1, 2 );

    R( 2, 0 ) = this->_composition.getDirectCoefficient( 2, 0 );
    R( 2, 1 ) = this->_composition.getDirectCoefficient( 2, 1 );
    R( 2, 2 ) = this->_composition.getDirectCoefficient( 2, 2 );

    // formula of Rodrigues:
    // theta = arccos( ( Tr(R) - 1 ) / 2 )
    // Omega = ( R - R^T ) / ( 2 * sin( theta ) )


    theta = gkg::safeAcos( ( T )( ( R.getTrace() - 1.0 ) / 2.0 ) );
    Omega = ( R - R.getTransposition() ) / ( 2.0 * std::sin( theta ) );

    if ( degree )
    {

      theta *= ( T )( 180.0 / M_PI );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Rotation3d< T >::getRodriguesParameters( "
             "T& theta, "
             "gkg::Matrix& R, "
             "bool degree ) const" );

}


template class gkg::Rotation3d< float >;
template class gkg::Rotation3d< double >;

