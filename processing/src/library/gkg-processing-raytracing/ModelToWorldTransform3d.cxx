#include <gkg-processing-raytracing/ModelToWorldTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/InverseTransform3d.h>


template < class T >
gkg::ModelToWorldTransform3d< T >::ModelToWorldTransform3d(
                                                       const T& rotationCenterX,
                                                       const T& rotationCenterY,
                                                       const T& rotationCenterZ,
                                                       const T& angleX,
                                                       const T& angleY,
                                                       const T& angleZ,
                                                       const T& translationX,
                                                       const T& translationY,
                                                       const T& translationZ,
                                                       bool degree )
                                  : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _translationCenter3d.reset( new gkg::Translation3d< T >( rotationCenterX,
                                                           rotationCenterY,
                                                           rotationCenterZ,
                                                           true ) );
  _rotation3d.reset( new gkg::Rotation3d< T >( angleX,
                                               angleY,
                                               angleZ,
                                               degree, true ) );
  _translation3d.reset( new gkg::Translation3d< T >( translationX,
                                                     translationY,
                                                     translationZ,
                                                     true ) );

  gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
  inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                        _translationCenter3d ) );
  transforms.push_back( inverseTransform3d );
  transforms.push_back( _rotation3d );
  transforms.push_back( _translationCenter3d );
  transforms.push_back( _translation3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::ModelToWorldTransform3d< T >::ModelToWorldTransform3d(
                                       const gkg::Vector3d< T >& rotationCenter,
                                       const gkg::Vector3d< T >& angles,
                                       const gkg::Vector3d< T >& translation,
                                       bool degree )
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _translationCenter3d.reset( new gkg::Translation3d< T >( rotationCenter,
                                                           true ) );
  _rotation3d.reset( new gkg::Rotation3d< T >( angles, degree, true ) );
  _translation3d.reset( new gkg::Translation3d< T >( translation, true ) );

  gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
  inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                        _translationCenter3d ) );
  transforms.push_back( inverseTransform3d );

  transforms.push_back( _rotation3d );
  transforms.push_back( _translationCenter3d );
  transforms.push_back( _translation3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::ModelToWorldTransform3d< T >::ModelToWorldTransform3d(
                                const gkg::ModelToWorldTransform3d< T >& other )
                                  : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _translationCenter3d.reset( new gkg::Translation3d< T >(
                                            other.getTranslationCenter3d() ) ) ;
  _rotation3d.reset( new gkg::Rotation3d< T >( other.getRotation3d() ) );
  _translation3d.reset(
                      new gkg::Translation3d< T >( other.getTranslation3d() ) );

  gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
  inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                        _translationCenter3d ) );
  transforms.push_back( inverseTransform3d );
  transforms.push_back( _rotation3d );
  transforms.push_back( _translationCenter3d );
  transforms.push_back( _translation3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}




template < class T >
gkg::ModelToWorldTransform3d< T >::~ModelToWorldTransform3d()
{


}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::ModelToWorldTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::ModelToWorldTransform3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::ModelToWorldTransform3d< T >&
gkg::ModelToWorldTransform3d< T >::operator=(
                                const gkg::ModelToWorldTransform3d< T >& other )
{

  this->removeTransforms();

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _translationCenter3d.reset( new gkg::Translation3d< T >(
                                             other.getTranslationCenter3d() ) );
  _rotation3d.reset( new gkg::Rotation3d< T >( other.getRotation3d() ) );
  _translation3d.reset( new gkg::Translation3d< T >(
                                                   other.getTranslation3d() ) );

  gkg::RCPointer< gkg::InverseTransform3d< T > > inverseTransform3d;
    inverseTransform3d.reset( new gkg::InverseTransform3d< T >(
                                                        _translationCenter3d ) );
  transforms.push_back( inverseTransform3d );
  transforms.push_back( _rotation3d );
  transforms.push_back( _translationCenter3d );
  transforms.push_back( _translation3d );

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, true );

  return *this;

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::setParameters( const T& rotationCenterX,
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

  _translationCenter3d->setDirectTranslation( rotationCenterX,
                                              rotationCenterY,
                                              rotationCenterZ,
                                              updating, false );
  _rotation3d->setDirectAngles( angleX,
                                angleY,
                                angleZ,
                                degree, updating, false );
  _translation3d->setDirectTranslation( translationX,
                                        translationY,
                                        translationZ,
                                        updating, false );
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
void gkg::ModelToWorldTransform3d< T >::setParameters(
                                       const gkg::Vector3d< T >& rotationCenter,
                                       const gkg::Vector3d< T >& angles,
                                       const gkg::Vector3d< T >& translation,
                                       bool degree,
                                       bool updating,
                                       bool notify )
{

  _translationCenter3d->setDirectTranslation( rotationCenter, updating, false );
  _rotation3d->setDirectAngles( angles, degree, updating, false );
  _translation3d->setDirectTranslation( translation, updating, false );
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
void gkg::ModelToWorldTransform3d< T >::getParameters( T& rotationCenterX,
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
  _rotation3d->getDirectAngles( angleX, angleY, angleZ, degree );
  _translation3d->getDirectTranslation( translationX,
                                        translationY,
                                        translationZ );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::getParameters(
                                             gkg::Vector3d< T >& rotationCenter,
                                             gkg::Vector3d< T >& angles,
                                             gkg::Vector3d< T >& translation,
                                             bool degree ) const
{

  _translationCenter3d->getDirectTranslation( rotationCenter );
  _rotation3d->getDirectAngles( angles, degree );
  _translation3d->getDirectTranslation( translation );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::setRotationCenter(
                                                       const T& rotationCenterX,
                                                       const T& rotationCenterY,
                                                       const T& rotationCenterZ,
                                                       bool updating,
                                                       bool notify )
{

  _translationCenter3d->setDirectTranslation( rotationCenterX,
                                              rotationCenterY,
                                              rotationCenterZ,
                                              updating, false );
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
void gkg::ModelToWorldTransform3d< T >::setRotationCenter(
                                       const gkg::Vector3d< T >& rotationCenter,
                                       bool updating,
                                       bool notify )
{

  _translationCenter3d->setDirectTranslation( rotationCenter,
                                              updating, false );
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
void gkg::ModelToWorldTransform3d< T >::getRotationCenter(
                                                      T& rotationCenterX,
                                                      T& rotationCenterY,
                                                      T& rotationCenterZ ) const
{

  _translationCenter3d->getDirectTranslation( rotationCenterX,
                                              rotationCenterY,
                                              rotationCenterZ );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::getRotationCenter(
                                      gkg::Vector3d< T >& rotationCenter ) const
{

  _translationCenter3d->getDirectTranslation( rotationCenter );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::setAngles( const T& angleX,
                                                   const T& angleY,
                                                   const T& angleZ,
                                                   bool degree,
                                                   bool updating,
                                                   bool notify )
{

  _rotation3d->setDirectAngles( angleX,
                                angleY,
                                angleZ,
                                degree,
                                updating, false );
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
void gkg::ModelToWorldTransform3d< T >::setAngles(
                                               const gkg::Vector3d< T >& angles,
                                               bool degree,
                                               bool updating,
                                               bool notify )
{

  _rotation3d->setDirectAngles( angles, degree, updating, false );
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
void gkg::ModelToWorldTransform3d< T >::getAngles( T& angleX,
                                                   T& angleY,
                                                   T& angleZ,
                                                   bool degree ) const
{

  _rotation3d->getDirectAngles( angleX, angleY, angleZ, degree );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::getAngles( gkg::Vector3d< T >& angles,
                                                   bool degree ) const
{

  _rotation3d->getDirectAngles( angles, degree );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::setTranslation( const T& translationX,
                                                        const T& translationY,
                                                        const T& translationZ,
                                                        bool updating,
                                                        bool notify )
{

  _translation3d->setDirectTranslation( translationX,
                                        translationY,
                                        translationZ,
                                        updating, false );
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
void gkg::ModelToWorldTransform3d< T >::setTranslation(
                                          const gkg::Vector3d< T >& translation,
                                          bool updating,
                                          bool notify )
{

  _translation3d->setDirectTranslation( translation, updating, false );
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
void gkg::ModelToWorldTransform3d< T >::getTranslation( T& translationX,
                                                        T& translationY,
                                                        T& translationZ ) const
{

  _translation3d->getDirectTranslation( translationX,
                                        translationY,
                                        translationZ );

}


template < class T >
void gkg::ModelToWorldTransform3d< T >::getTranslation(
                                         gkg::Vector3d< T >& translation ) const
{

  _translation3d->getDirectTranslation( translation );

}


template < class T >
const gkg::Translation3d< T >& 
gkg::ModelToWorldTransform3d< T >::getTranslationCenter3d() const
{

  return *_translationCenter3d;

}


template < class T >
const gkg::Rotation3d< T >&
gkg::ModelToWorldTransform3d< T >::getRotation3d() const
{

  return *_rotation3d;

}


template < class T >
const gkg::Translation3d< T >&
gkg::ModelToWorldTransform3d< T >::getTranslation3d() const
{

  return *_translation3d;

}


template class gkg::ModelToWorldTransform3d< float >;
template class gkg::ModelToWorldTransform3d< double >;
