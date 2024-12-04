#include <gkg-processing-transform/Shearing3d.h>
#include <gkg-processing-transform/ShearingXY3d.h>
#include <gkg-processing-transform/ShearingXZ3d.h>
#include <gkg-processing-transform/ShearingYZ3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Shearing3d< T >::Shearing3d( const T& sxy,
                                  const T& sxz,
                                  const T& syz,
                                  bool direct )
                     : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _shearingXY3d.reset( new gkg::ShearingXY3d< T >( sxy, direct ) );
  _shearingXZ3d.reset( new gkg::ShearingXZ3d< T >( sxz, direct ) );
  _shearingYZ3d.reset( new gkg::ShearingYZ3d< T >( syz, direct ) );

  if ( direct )
  {
  
    transforms.push_back( _shearingXY3d );
    transforms.push_back( _shearingXZ3d );
    transforms.push_back( _shearingYZ3d );
    
  }
  else
  {
  
    transforms.push_back( _shearingYZ3d );
    transforms.push_back( _shearingXZ3d );
    transforms.push_back( _shearingXY3d );
    
  }

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::Shearing3d< T >::Shearing3d( const gkg::Vector3d< T >& shearings,
                                  bool direct )
                     : gkg::CompositeTransform3d< T >()
{

  std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
  std::deque< bool > clonings;

  _shearingXY3d.reset( new gkg::ShearingXY3d< T >( shearings.x, direct ) );
  _shearingXZ3d.reset( new gkg::ShearingXZ3d< T >( shearings.y, direct ) );
  _shearingYZ3d.reset( new gkg::ShearingYZ3d< T >( shearings.z, direct ) );

  if ( direct )
  {
  
    transforms.push_back( _shearingXY3d );
    transforms.push_back( _shearingXZ3d );
    transforms.push_back( _shearingYZ3d );
    
  }
  else
  {
  
    transforms.push_back( _shearingYZ3d );
    transforms.push_back( _shearingXZ3d );
    transforms.push_back( _shearingXY3d );
    
  }

  clonings.push_back( false );
  clonings.push_back( false );
  clonings.push_back( false );

  this->compose( transforms, clonings, true, false );

}


template < class T >
gkg::Shearing3d< T >::Shearing3d( const gkg::Shearing3d< T >& other )
                     : gkg::CompositeTransform3d< T >( other )
{

  _shearingXY3d = other._transforms[ 0 ];
  _shearingXZ3d = other._transforms[ 1 ];
  _shearingYZ3d = other._transforms[ 2 ];

}


template < class T >
gkg::Shearing3d< T >::~Shearing3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > > gkg::Shearing3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::Shearing3d< T >( *this ) );
  return transform;

}


template < class T >
gkg::Shearing3d< T >& 
gkg::Shearing3d< T >::operator=( const gkg::Shearing3d< T >& other )
{

  this->gkg::CompositeTransform3d< T >::operator=( other );

  _shearingXY3d = other._transforms[ 0 ];
  _shearingXZ3d = other._transforms[ 1 ];
  _shearingYZ3d = other._transforms[ 2 ];

  return *this;

}


template < class T >
void gkg::Shearing3d< T >::setDirectShearings( const T& sxy,
                                               const T& sxz,
                                               const T& syz,
                                               bool updating,
                                               bool notify )
{

  _shearingXY3d->setDirectShearingXY( sxy, updating, false );
  _shearingXZ3d->setDirectShearingXZ( sxz, updating, false );
  _shearingYZ3d->setDirectShearingYZ( syz, updating, false );
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
void gkg::Shearing3d< T >::setDirectShearings(
                                            const gkg::Vector3d< T >& shearings,
                                            bool updating,
                                            bool notify )
{

  _shearingXY3d->setDirectShearingXY( shearings.x, updating, false );
  _shearingXZ3d->setDirectShearingXZ( shearings.y, updating, false );
  _shearingYZ3d->setDirectShearingYZ( shearings.z, updating, false );
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
void gkg::Shearing3d< T >::getDirectShearings( T& sxy,
                                               T& sxz,
                                               T& syz ) const
{

  _shearingXY3d->getDirectShearingXY( sxy );
  _shearingXZ3d->getDirectShearingXZ( sxz );
  _shearingYZ3d->getDirectShearingYZ( syz );

}


template < class T >
void 
gkg::Shearing3d< T >::getDirectShearings( gkg::Vector3d< T >& shearings ) const
{

  _shearingXY3d->getDirectShearingXY( shearings.x );
  _shearingXZ3d->getDirectShearingXZ( shearings.y );
  _shearingYZ3d->getDirectShearingYZ( shearings.z );

}


template < class T >
void gkg::Shearing3d< T >::setInverseShearings( const T& sxy,
                                                const T& sxz,
                                                const T& syz,
                                                bool updating,
                                                bool notify )
{

  _shearingXY3d->setInverseShearingXY( sxy, updating, false );
  _shearingXZ3d->setInverseShearingXZ( sxz, updating, false );
  _shearingYZ3d->setInverseShearingYZ( syz, updating, false );
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
void gkg::Shearing3d< T >::setInverseShearings(
                                            const gkg::Vector3d< T >& shearings,
                                            bool updating,
                                            bool notify )
{

  _shearingXY3d->setInverseShearingXY( shearings.x, updating, false );
  _shearingXZ3d->setInverseShearingXZ( shearings.y, updating, false );
  _shearingYZ3d->setInverseShearingYZ( shearings.z, updating, false );
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
void gkg::Shearing3d< T >::getInverseShearings( T& sxy,
                                                T& sxz,
                                                T& syz ) const
{

  _shearingXY3d->getInverseShearingXY( sxy );
  _shearingXZ3d->getInverseShearingXZ( sxz );
  _shearingYZ3d->getInverseShearingYZ( syz );

}


template < class T >
void 
gkg::Shearing3d< T >::getInverseShearings( gkg::Vector3d< T >& shearings ) const
{

  _shearingXY3d->getInverseShearingXY( shearings.x );
  _shearingXZ3d->getInverseShearingXZ( shearings.y );
  _shearingYZ3d->getInverseShearingYZ( shearings.z );

}


template < class T >
const gkg::ShearingXY3d< T >& gkg::Shearing3d< T >::getShearingXY3d() const
{

  return *_shearingXY3d;

}


template < class T >
const gkg::ShearingXZ3d< T >& gkg::Shearing3d< T >::getShearingXZ3d() const
{

  return *_shearingXZ3d;

}


template < class T >
const gkg::ShearingYZ3d< T >& gkg::Shearing3d< T >::getShearingYZ3d() const
{

  return *_shearingYZ3d;

}


template class gkg::Shearing3d< float >;
template class gkg::Shearing3d< double >;

