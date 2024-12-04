#include <gkg-processing-raytracing/WorldToEyeTransform3d.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/InverseTransform3d.h>
#include <gkg-core-exception/BadParameterCountException.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::WorldToEyeTransform3d< T >::WorldToEyeTransform3d(
                                                    const T& eyePositionX,
                                                    const T& eyePositionY,
                                                    const T& eyePositionZ,
                                                    const T& referencePositionX,
                                                    const T& referencePositionY,
                                                    const T& referencePositionZ,
                                                    const T& upwardDirectionX,
                                                    const T& upwardDirectionY,
                                                    const T& upwardDirectionZ,
                                                    bool direct )
                                  : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  gkg::Vector< T > n = gkg::Vector< T >( eyePositionX,
                                         eyePositionY,
                                         eyePositionZ ) -
                       gkg::Vector< T >( referencePositionX,
                                         referencePositionY,
                                         referencePositionZ );
  n.normalize();
  gkg::Vector< T > u = gkg::Vector< T >( upwardDirectionX,
                                         upwardDirectionY,
                                         upwardDirectionZ ).cross( n );
  u.normalize();
  gkg::Vector< T > v = n.cross( u );   
                                         

  gkg::Vector< T > angles = gkg::getXYZAnglesFromTrieder( u, v, n, false );

  _translation3d.reset( new gkg::Translation3d< T >( -eyePositionX,
                                                     -eyePositionY,
                                                     -eyePositionZ,
                                                     direct ) );
  _rotationZ3d.reset( new gkg::RotationZ3d< T >( angles.z, false, !direct ) );
  _rotationY3d.reset( new gkg::RotationY3d< T >( angles.y, false, !direct ) );
  _rotationX3d.reset( new gkg::RotationX3d< T >( angles.x, false, !direct ) );

  transforms.push_back( _translation3d );
  // careful, we put inverse of RzRyRx, i.e. (Rx^-1)(Ry^-1)(Rz^-1)
  transforms.push_back( _rotationZ3d );
  transforms.push_back( _rotationY3d );
  transforms.push_back( _rotationX3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  compose( transforms, clonings, true, false );

}


template < class T >
gkg::WorldToEyeTransform3d< T >::WorldToEyeTransform3d(
                                    const gkg::Vector3d< T >& eyePosition,
                                    const gkg::Vector3d< T >& referencePosition,
                                    const gkg::Vector3d< T >& upwardDirection,
                                    bool direct )
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  gkg::Vector< T > n = eyePosition - referencePosition;
  n.normalize();
  gkg::Vector< T > u = upwardDirection.cross( n );
  u.normalize();
  gkg::Vector< T > v = n.cross( u );   
                                         

  gkg::Vector< T > angles = gkg::getXYZAnglesFromTrieder( u, v, n, false );

  _translation3d.reset( new gkg::Translation3d< T >( -eyePosition.x,
                                                     -eyePosition.y,
                                                     -eyePosition.z,
                                                     direct ) ) ;
  _rotationZ3d.reset( new gkg::RotationZ3d< T >( angles.z, false, !direct ) );
  _rotationY3d.reset( new gkg::RotationY3d< T >( angles.y, false, !direct ) );
  _rotationX3d.reset( new gkg::RotationX3d< T >( angles.x, false, !direct ) );

  transforms.push_back( _translation3d );
  // careful, we put inverse of RzRyRx, i.e. (Rx^-1)(Ry^-1)(Rz^-1)
  transforms.push_back( _rotationZ3d );
  transforms.push_back( _rotationY3d );
  transforms.push_back( _rotationX3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  compose( transforms, clonings, true, false );

}


template < class T >
gkg::WorldToEyeTransform3d< T >::WorldToEyeTransform3d(
                                             const std::vector< T >& parameters,
                                             bool direct )
{

  if ( parameters.size()!= 9U )
  {

    throw gkg::BadParameterCountException( 9,
               "gkg::WorldToEyeTransform3d< T >::WorldToEyeTransform3d: " );

  }

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  gkg::Vector< T > n = gkg::Vector< T >( parameters[ 0 ],
                                         parameters[ 1 ],
                                         parameters[ 2 ] ) -
                       gkg::Vector< T >( parameters[ 3 ],
                                         parameters[ 4 ],
                                         parameters[ 5 ] );
  n.normalize();
  gkg::Vector< T > u = gkg::Vector< T >( parameters[ 6 ],
                                         parameters[ 7 ],
                                         parameters[ 8 ] ).cross( n );
  u.normalize();
  gkg::Vector< T > v = n.cross( u );   
                                         

  gkg::Vector< T > angles = gkg::getXYZAnglesFromTrieder( u, v, n, false );

  _translation3d.reset( new gkg::Translation3d< T >( -parameters[ 0 ],
                                                     -parameters[ 1 ],
                                                     -parameters[ 2 ],
                                                     direct ) );
  _rotationZ3d.reset( gkg::RotationZ3d< T >( angles.z, false, !direct ) );
  _rotationY3d.reset( gkg::RotationY3d< T >( angles.y, false, !direct ) );
  _rotationX3d.reset( gkg::RotationX3d< T >( angles.x, false, !direct ) );

  transforms.push_back( _translation3d );
  // careful, we put inverse of RzRyRx, i.e. (Rx^-1)(Ry^-1)(Rz^-1)
  transforms.push_back( _rotationZ3d );
  transforms.push_back( _rotationY3d );
  transforms.push_back( _rotationX3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  compose( transforms, clonings, true, false );

}


template < class T >
gkg::WorldToEyeTransform3d< T >::WorldToEyeTransform3d(
                                  const gkg::WorldToEyeTransform3d< T >& other )
                                : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _translation3d.reset(gkg::Translation3d< T >( other.getTranslation3d() ) );
  _rotationZ3d.reset( new gkg::RotationZ3d< T >( other.getRotationZ3d() ) );
  _rotationY3d.reset( new gkg::RotationY3d< T >( other.getRotationY3d() ) );
  _rotationX3d.reset( new gkg::RotationX3d< T >( other.getRotationX3d() ) );

  transforms.push_back( _translation3d );
  transforms.push_back( _rotationZ3d );
  transforms.push_back( _rotationY3d );
  transforms.push_back( _rotationX3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  compose( transforms, clonings, true, false );

}




template < class T >
gkg::WorldToEyeTransform3d< T >::~WorldToEyeTransform3d()
{


}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::WorldToEyeTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::WorldToEyeTransform3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::WorldToEyeTransform3d< T >&
gkg::WorldToEyeTransform3d< T >::operator=(
                                  const gkg::WorldToEyeTransform3d< T >& other )
{

  this->gkg::CompositeTransform3d< T >::operator=( other );
  _translation3d = dynamic_cast< gkg::RCPointer< gkg::Translation3d< T > > >(
                                                       other._transforms[ O ] );
  _rotationZ3d = dynamic_cast< gkg::RCPointer< gkg::RotationZ3d< T > > >(
                                                       other._transforms[ 1 ] );
  _rotationY3d = dynamic_cast< gkg::RCPointer< gkg::RotationY3d< T > > >(
                                                       other._transforms[ 2 ] );
  _rotationX3d = dynamic_cast< gkg::RCPointer< gkg::RotationX3d< T > > >(
                                                       other._transforms[ 3 ] );

  return *this;

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setDirectParameters(
                                                    const T& eyePositionX,
                                                    const T& eyePositionY,
                                                    const T& eyePositionZ,
                                                    const T& referencePositionX,
                                                    const T& referencePositionY,
                                                    const T& referencePositionZ,
                                                    const T& upwardDirectionX,
                                                    const T& upwardDirectionY,
                                                    const T& upwardDirectionZ,
                                                    bool updating,
                                                    bool notify )
{

  gkg::Vector< T > n = gkg::Vector< T >( eyePositionX,
                                         eyePositionY,
                                         eyePositionZ ) -
                       gkg::Vector< T >( referencePositionX,
                                         referencePositionY,
                                         referencePositionZ );
  n.normalize();
  gkg::Vector< T > u = gkg::Vector< T >( upwardDirectionX,
                                         upwardDirectionY,
                                         upwardDirectionZ ).cross( n );
  u.normalize();
  gkg::Vector< T > v = n.cross( u );   
                                         

  gkg::Vector< T > angles = gkg::getXYZAnglesFromTrieder( u, v, n, false );

  _translation3d->setDirectTranslation( -eyePositionX,
                                        -eyePositionY,
                                        -eyePositionZ,
                                        updating, false );
  _rotationX3d->setInverseAngle( angles.z, false, updating, false );
  _rotationY3d->setInverseAngle( angles.y, false, updating, false );
  _rotationZ3d->setInverseAngle( angles.x, false, updating, notify );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setDirectParameters(
                                       const gkg::Vector3d< T >& rotationCenter,
                                       const T& angleX,
                                       const T& angleY,
                                       const T& angleZ,
                                       const gkg::Vector3d< T >& translation,
                                       bool degree,
                                       bool updating,
                                       bool notify )
{

  _translationCenter3d->setDirectTranslation( rotationCenter,
                                              updating, false );
  _rotationX3d->setDirectAngle( angleX, degree, updating, false );
  _rotationY3d->setDirectAngle( angleY, degree, updating, false );
  _rotationZ3d->setDirectAngle( angleZ, degree, updating, false );
  _translation3d->setDirectTranslation( translation,
                                        updating, notify );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setDirectParameters(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  if ( parameters.size()!= 9U )
  {

    throw gkg::BadParameterCountException( 9,
                   "gkg::WorldToEyeTransform3d< T >::setDirectParameters: " );

  }
  _translationCenter3d->setDirectTranslation( parameters[ 0 ],
                                              parameters[ 1 ],
                                              parameters[ 2 ],
                                              updating, false );
  _rotationX3d->setDirectAngle( parameters[ 3 ], degree, updating, false );
  _rotationY3d->setDirectAngle( parameters[ 4 ], degree, updating, false );
  _rotationZ3d->setDirectAngle( parameters[ 5 ], degree, updating, false );
  _translation3d->setDirectTranslation( parameters[ 6 ],
                                        parameters[ 7 ],
                                        parameters[ 8 ],
                                        updating, notify );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getDirectParameters( T& rotationCenterX,
                                                             T& rotationCenterY,
                                                             T& rotationCenterZ,
                                                             T& angleX,
                                                             T& angleY,
                                                             T& angleZ,
                                                             T& translationX,
                                                             T& translationY,
                                                             T& translationZ,
                                                             bool degree ) const
{

  _translationCenter3d->getDirectTranslation( rotationCenterX,
                                              rotationCenterY,
                                              rotationCenterZ );
  _rotationX3d->getDirectAngle( angleX, degree );
  _rotationY3d->getDirectAngle( angleY, degree );
  _rotationZ3d->getDirectAngle( angleZ, degree );
  _translation3d->getDirectTranslation( translationX,
                                        translationY,
                                        translationZ );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getDirectParameters(
                                             gkg::Vector3d< T >& rotationCenter,
                                             T& angleX,
                                             T& angleY,
                                             T& angleZ,
                                             gkg::Vector3d< T >& translation,
                                             bool degree ) const
{

  _translationCenter3d->getDirectTranslation( rotationCenter );
  _rotationX3d->getDirectAngle( angleX, degree );
  _rotationY3d->getDirectAngle( angleY, degree );
  _rotationZ3d->getDirectAngle( angleZ, degree );
  _translation3d->getDirectTranslation( translation );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getDirectParameters(
                                                   std::vector< T >& parameters,
                                                   bool degree ) const
{

  if ( parameters.size()!= 9U )
  {

    parameters = std::vector< T >( 9 );

  }
  _translationCenter3d->getDirectTranslation( parameters[ 0 ],
                                              parameters[ 1 ],
                                              parameters[ 2 ] );
  _rotationX3d->getDirectAngle( parameters[ 3 ], degree );
  _rotationY3d->getDirectAngle( parameters[ 4 ], degree );
  _rotationZ3d->getDirectAngle( parameters[ 5 ], degree );
  _translation3d->getDirectTranslation( parameters[ 6 ],
                                        parameters[ 7 ],
                                        parameters[ 8 ] );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setInverseParameters(
                                                       const T& rotationCenterX,
                                                       const T& rotationCenterY,
                                                       const T& rotationCenterZ,
                                                       const T& angleX,
                                                       const T& angleY,
                                                       const T& angleZ,
                                                       const T& translationX,
                                                       const T& translationY,
                                                       const T& translationZ,
                                                       bool degree,
                                                       bool updating,
                                                       bool notify )
{

  try
  {

    _translationCenter3d->setInverseTranslation( rotationCenterX,
                                                 rotationCenterY,
                                                 rotationCenterZ,
                                                 updating, false );
    _rotationX3d->setInverseAngle( angleX, degree, updating, false );
    _rotationY3d->setInverseAngle( angleY, degree, updating, false );
    _rotationZ3d->setInverseAngle( angleZ, degree, updating, false );
    _translation3d->setInverseTranslation( translationX,
                                           translationY,
                                           translationZ,
                                           updating, notify );

  }
  GKG_CATCH( "template < class T > void gkg::WorldToEyeTransform3d< T >::"
             "setInverseParameters( const T& rotationCenterX, "
             "const T& rotationCenterY, const T& rotationCenterZ, "
             "const T& angleX, const T& angleY, const T& angleZ, " 
             "const T& translationX, const T& translationY, "
             "const T& translationZ, bool degree, bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setInverseParameters(
                                       const gkg::Vector3d< T >& rotationCenter,
                                       const T& angleX,
                                       const T& angleY,
                                       const T& angleZ,
                                       const gkg::Vector3d< T >& translation,
                                       bool degree,
                                       bool updating,
                                       bool notify )
{

  _translationCenter3d->setInverseTranslation( rotationCenter,
                                               updating, false );
  _rotationX3d->setInverseAngle( angleX, degree, updating, false );
  _rotationY3d->setInverseAngle( angleY, degree, updating, false );
  _rotationZ3d->setInverseAngle( angleZ, degree, updating, false );
  _translation3d->setInverseTranslation( translation, updating, notify );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::setInverseParameters(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  if ( parameters.size()!= 9U )
  {

    throw gkg::BadParameterCountException( 9,
                  "gkg::WorldToEyeTransform3d< T >::setInverseParameters: " );

  }
  _translationCenter3d->setInverseTranslation( parameters[ 0 ],
                                              parameters[ 1 ],
                                              parameters[ 2 ],
                                              updating, false );
  _rotationX3d->setInverseAngle( parameters[ 3 ], degree, updating, false );
  _rotationY3d->setInverseAngle( parameters[ 4 ], degree, updating, false );
  _rotationZ3d->setInverseAngle( parameters[ 5 ], degree, updating, false );
  _translation3d->setInverseTranslation( parameters[ 6 ],
                                         parameters[ 7 ],
                                         parameters[ 8 ],
                                         updating, notify );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getInverseParameters(
                                                             T& rotationCenterX,
                                                             T& rotationCenterY,
                                                             T& rotationCenterZ,
                                                             T& angleX,
                                                             T& angleY,
                                                             T& angleZ,
                                                             T& translationX,
                                                             T& translationY,
                                                             T& translationZ,
                                                             bool degree ) const
{

  _translationCenter3d->getInverseTranslation( rotationCenterX,
                                               rotationCenterY,
                                               rotationCenterZ );
  _rotationX3d->getInverseAngle( angleX, degree );
  _rotationY3d->getInverseAngle( angleY, degree );
  _rotationZ3d->getInverseAngle( angleZ, degree );
  _translation3d->getInverseTranslation( translationX,
                                         translationY,
                                         translationZ );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getInverseParameters(
                                             gkg::Vector3d< T >& rotationCenter,
                                             T& angleX,
                                             T& angleY,
                                             T& angleZ,
                                             gkg::Vector3d< T >& translation,
                                             bool degree ) const
{

  _translationCenter3d->getInverseTranslation( rotationCenter );
  _rotationX3d->getInverseAngle( angleX, degree );
  _rotationY3d->getInverseAngle( angleY, degree );
  _rotationZ3d->getInverseAngle( angleZ, degree );
  _translation3d->getInverseTranslation( translation );

}


template < class T >
void gkg::WorldToEyeTransform3d< T >::getInverseParameters(
                                                   std::vector< T >& parameters,
                                                   bool degree ) const
{

  if ( parameters.size()!= 9U )
  {

    parameters = std::vector< T >( 9 );

  }
  _translationCenter3d->getInverseTranslation( parameters[ 0 ],
                                               parameters[ 1 ],
                                               parameters[ 2 ] );
  _rotationX3d->getInverseAngle( parameters[ 3 ], degree );
  _rotationY3d->getInverseAngle( parameters[ 4 ], degree );
  _rotationZ3d->getInverseAngle( parameters[ 5 ], degree );
  _translation3d->getInverseTranslation( parameters[ 6 ],
                                         parameters[ 7 ],
                                         parameters[ 8 ] );

}


template < class T >
const gkg::Translation3d< T >& 
gkg::WorldToEyeTransform3d< T >::getTranslationCenter3d() const
{

  return *_translationCenter3d;

}


template < class T >
const gkg::RotationX3d< T >&
gkg::WorldToEyeTransform3d< T >::getRotationX3d() const
{

  return *_rotationX3d;

}


template < class T >
const gkg::RotationY3d< T >&
gkg::WorldToEyeTransform3d< T >::getRotationY3d() const
{

  return *_rotationY3d;

}


template < class T >
const gkg::RotationZ3d< T >&
gkg::WorldToEyeTransform3d< T >::getRotationZ3d() const
{

  return *_rotationZ3d;

}


template < class T >
const gkg::Translation3d< T >&
gkg::WorldToEyeTransform3d< T >::getTranslation3d() const
{

  return *_translation3d;

}


template class gkg::WorldToEyeTransform3d< float >;
template class gkg::WorldToEyeTransform3d< double >;
