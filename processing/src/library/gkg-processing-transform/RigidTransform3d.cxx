#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-exception/BadParameterCountException.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <string>


#define EPSILON 1e-4


template < class T >
gkg::RigidTransform3d< T >::RigidTransform3d( const T& angleX,
                                              const T& angleY,
                                              const T& angleZ,
                                              const T& translationX,
                                              const T& translationY,
                                              const T& translationZ,
                                              bool degree,
                                              bool direct )
                           : gkg::CompositeTransform3d< T >()
{

  try
  {

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _fromFovCenterTranslation3d.reset( new gkg::Translation3d< T > );
    _rotation3d.reset( new gkg::Rotation3d< T >( angleX, angleY, angleZ,
                                                 degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( translationX,
                                                       translationY,
                                                       translationZ,
                                                       direct ) );
    _toFovCenterInverseTranslation3d.reset( new gkg::Translation3d< T > );

    if ( direct )
    {

      transforms.push_back( _fromFovCenterTranslation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _toFovCenterInverseTranslation3d );

    }
    else
    {

      transforms.push_back( _toFovCenterInverseTranslation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _fromFovCenterTranslation3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RigidTransform3d< T >::RigidTransform3d( "
             "const T& angleX, "
             "const T& angleY, "
             "const T& angleZ, "
             "const T& translationX, "
             "const T& translationY, "
             "const T& translationZ, "
             "bool degree, "
             "bool direct )" );

}


template < class T >
gkg::RigidTransform3d< T >::RigidTransform3d(
                                          const gkg::Vector3d< T >& angles,
                                          const gkg::Vector3d< T >& translation,
                                          bool degree,
                                          bool direct )
                           : gkg::CompositeTransform3d< T >()
{

  try
  {

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _fromFovCenterTranslation3d.reset( new gkg::Translation3d< T > );
    _rotation3d.reset( new gkg::Rotation3d< T >( angles, degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( translation, direct ) );
    _toFovCenterInverseTranslation3d.reset( new gkg::Translation3d< T > );

    if ( direct )
    {

      transforms.push_back( _fromFovCenterTranslation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _toFovCenterInverseTranslation3d );

    }
    else
    {

      transforms.push_back( _toFovCenterInverseTranslation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _fromFovCenterTranslation3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RigidTransform3d< T >::RigidTransform3d( "
             "const gkg::Vector3d< T >& angles, "
             "const gkg::Vector3d< T >& translation, "
             "bool degree, "
             "bool direct )" );

}


template < class T >
gkg::RigidTransform3d< T >::RigidTransform3d(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool direct )
                           : gkg::CompositeTransform3d< T >()
{

  try
  {

    if ( parameters.size()!= 6U )
    {

      throw gkg::BadParameterCountException( 6 );

    }

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _fromFovCenterTranslation3d.reset( new gkg::Translation3d< T > );
    _rotation3d.reset( new gkg::Rotation3d< T >( parameters[ 0 ],
                                                 parameters[ 1 ],
                                                 parameters[ 2 ],
                                                 degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( parameters[ 3 ],
                                                       parameters[ 4 ],
                                                       parameters[ 5 ],
                                                       direct ) );
    _toFovCenterInverseTranslation3d.reset( new gkg::Translation3d< T > );

    if ( direct )
    {

      transforms.push_back( _fromFovCenterTranslation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _toFovCenterInverseTranslation3d );

    }
    else
    {

      transforms.push_back( _toFovCenterInverseTranslation3d );
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _fromFovCenterTranslation3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RigidTransform3d< T >::RigidTransform3d( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool direct )" );

}




template < class T >
gkg::RigidTransform3d< T >::RigidTransform3d(
                                       const gkg::RigidTransform3d< T >& other )
                           : gkg::CompositeTransform3d< T >( other )
{

  try
  {

    _fromFovCenterTranslation3d = this->_transforms[ 0 ];
    _rotation3d = this->_transforms[ 1 ];
    _translation3d = this->_transforms[ 2 ];
    _toFovCenterInverseTranslation3d = this->_transforms[ 3 ];

  }
  GKG_CATCH( "template < class T > "
             "gkg::RigidTransform3d< T >::RigidTransform3d( "
             "const gkg::RigidTransform3d< T >& other )" );

}


template < class T >
gkg::RigidTransform3d< T >::~RigidTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::RigidTransform3d< T >::clone() const
{

  try
  {

    gkg::RCPointer< gkg::Transform3d< T > > transform;
    transform.reset( new gkg::RigidTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::RigidTransform3d< T >::clone() const" );

}


template < class T >
gkg::RigidTransform3d< T >& 
gkg::RigidTransform3d< T >::operator=( const gkg::RigidTransform3d< T >& other )
{

  try
  {

    this->gkg::CompositeTransform3d< T >::operator=( other );

    _fromFovCenterTranslation3d = this->_transforms[ 0 ];
    _rotation3d = this->_transforms[ 1 ];
    _translation3d = this->_transforms[ 2 ];
    _toFovCenterInverseTranslation3d = this->_transforms[ 3 ];

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RigidTransform3d< T >& "
             "gkg::RigidTransform3d< T >::operator=( "
             "const gkg::RigidTransform3d< T >& other )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setDirectParameters( const T& angleX,
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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setDirectParameters( "
             "const T& angleX, "
             "const T& angleY, "
             "const T& angleZ, "
             "const T& translationX, "
             "const T& translationY, "
             "const T& translationZ, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setDirectParameters(
                                          const gkg::Vector3d< T >& angles,
                                          const gkg::Vector3d< T >& translation,
                                          bool degree,
                                          bool updating,
                                          bool notify )
{

  try
  {

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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setDirectParameters( "
              "const gkg::Vector3d< T >& angles, "
              "const gkg::Vector3d< T >& translation, "
              "bool degree, "
              "bool updating, "
              "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setDirectParameters(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  try
  {

    if ( parameters.size()!= 6U )
    {

      throw gkg::BadParameterCountException( 6 );

    }
    _rotation3d->setDirectAngles( parameters[ 0 ],
                                  parameters[ 1 ],
                                  parameters[ 2 ],
                                  degree, updating, false );
    _translation3d->setDirectTranslation( parameters[ 3 ],
                                          parameters[ 4 ],
                                          parameters[ 5 ],
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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setDirectParameters( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::getDirectParameters( T& angleX,
                                                      T& angleY,
                                                      T& angleZ,
                                                      T& translationX,
                                                      T& translationY,
                                                      T& translationZ,
                                                      bool degree ) const
{

  try
  {

    _rotation3d->getDirectAngles( angleX,
                                  angleY,
                                  angleZ,
                                  degree );
    _translation3d->getDirectTranslation( translationX,
                                          translationY,
                                          translationZ );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getDirectParameters( "
             "T& angleX, "
             "T& angleY, "
             "T& angleZ, "
             "T& translationX, "
             "T& translationY, "
             "T& translationZ, "
             "bool degree ) const" );

}


template < class T >
void gkg::RigidTransform3d< T >::getDirectParameters(
                                                gkg::Vector3d< T >& angles,
                                                gkg::Vector3d< T >& translation,
                                                bool degree ) const
{

  try
  {

    _rotation3d->getDirectAngles( angles, degree );
    _translation3d->getDirectTranslation( translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getDirectParameters( "
             "gkg::Vector3d< T >& angles, "
             "gkg::Vector3d< T >& translation, "
             "bool degree ) const" );

}


template < class T >
void gkg::RigidTransform3d< T >::getDirectParameters(
                                                   std::vector< T >& parameters,
                                                   bool degree ) const
{

  try
  {

    if ( parameters.size()!= 6U )
    {

      parameters = std::vector< T >( 6 );

    }
    _rotation3d->getDirectAngles( parameters[ 0 ],
                                  parameters[ 1 ],
                                  parameters[ 2 ],
                                  degree );
    _translation3d->getDirectTranslation( parameters[ 3 ],
                                          parameters[ 4 ],
                                          parameters[ 5 ] );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getDirectParameters( "
             "std::vector< T >& parameters, "
             "bool degree ) const" );

}


template < class T >
void gkg::RigidTransform3d< T >::setInverseParameters( const T& angleX,
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

    _rotation3d->setInverseAngles( angleX,
                                   angleY,
                                   angleZ,
                                   degree, updating, false );
    _translation3d->setInverseTranslation( translationX,
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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setInverseParameters( "
             "const T& angleX, "
             "const T& angleY, "
             "const T& angleZ, "
             "const T& translationX, "
             "const T& translationY, "
             "const T& translationZ, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setInverseParameters(
                                          const gkg::Vector3d< T >& angles,
                                          const gkg::Vector3d< T >& translation,
                                          bool degree,
                                          bool updating,
                                          bool notify )
{

  try
  {

    _rotation3d->setInverseAngles( angles, degree, updating, false );
    _translation3d->setInverseTranslation( translation,
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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setInverseParameters( "
             "const gkg::Vector3d< T >& angles, "
             "const gkg::Vector3d< T >& translation, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setInverseParameters(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  try
  {

    if ( parameters.size()!= 6U )
    {

      throw gkg::BadParameterCountException( 6 );

    }
    _rotation3d->setInverseAngles( parameters[ 0 ],
                                   parameters[ 1 ],
                                   parameters[ 2 ],
                                   degree, updating, false );
    _translation3d->setInverseTranslation( parameters[ 3 ],
                                           parameters[ 4 ],
                                           parameters[ 5 ],
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
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setInverseParameters( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::getInverseParameters( T& angleX,
                                                       T& angleY,
                                                       T& angleZ,
                                                       T& translationX,
                                                       T& translationY,
                                                       T& translationZ,
                                                       bool degree ) const
{

  try
  {

    _rotation3d->getInverseAngles( angleX,
                                   angleY,
                                   angleZ,
                                   degree );
    _translation3d->getInverseTranslation( translationX,
                                           translationY,
                                           translationZ );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getInverseParameters( "
             "T& angleX, "
             "T& angleY, "
             "T& angleZ, "
             "T& translationX, "
             "T& translationY, "
             "T& translationZ, "
             "bool degree ) const" );

}


template < class T >
void gkg::RigidTransform3d< T >::getInverseParameters(
                                                gkg::Vector3d< T >& angles,
                                                gkg::Vector3d< T >& translation,
                                                bool degree ) const
{

  try
  {

    _rotation3d->getInverseAngles( angles, degree );
    _translation3d->getInverseTranslation( translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getInverseParameters( "
             "gkg::Vector3d< T >& angles, "
             "gkg::Vector3d< T >& translation, "
             "bool degree ) const" );

}


template < class T >
void gkg::RigidTransform3d< T >::getInverseParameters(
                                                   std::vector< T >& parameters,
                                                   bool degree ) const
{

  try
  {

    if ( parameters.size()!= 6U )
    {

      parameters = std::vector< T >( 6 );

    }
    _rotation3d->getInverseAngles( parameters[ 0 ],
                                   parameters[ 1 ],
                                   parameters[ 2 ],
                                   degree );
    _translation3d->getInverseTranslation( parameters[ 3 ],
                                           parameters[ 4 ],
                                           parameters[ 5 ] );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::getInverseParameters( "
             "std::vector< T >& parameters, "
             "bool degree ) const" );

}


template < class T >
const gkg::Translation3d< T >& 
gkg::RigidTransform3d< T >::getFromFovCenterTranslation3d() const
{

  return *_fromFovCenterTranslation3d;

}


template < class T >
const gkg::Rotation3d< T >& 
gkg::RigidTransform3d< T >::getRotation3d() const
{

  return *_rotation3d;

}


template < class T >
const gkg::Translation3d< T >&
gkg::RigidTransform3d< T >::getTranslation3d() const
{

  return *_translation3d;

}


template < class T >
const gkg::Translation3d< T >& 
gkg::RigidTransform3d< T >::getToFovCenterInverseTranslation3d() const
{

  return *_toFovCenterInverseTranslation3d;

}


template < class T >
T gkg::RigidTransform3d< T >::getNorm2() const
{

  try
  {

    // getting rotation with Rodrigues parameters
    T theta = ( T )0;
    gkg::Matrix Omega;
    _rotation3d->getRodriguesParameters( theta, Omega, false );

    // getting translation
    gkg::Vector3d< T > translation;
    _translation3d->getDirectTranslation( translation );

    return theta * theta +
           translation.getNorm2();

  }
  GKG_CATCH( "template < class T > "
             "T gkg::RigidTransform3d< T >::getNorm2() const" );

}


template < class T >
void gkg::RigidTransform3d< T >::setFromFieldOfViewCenter(
                                        const gkg::Vector3d< T >& fromFovCenter,
                                        bool updating,
                                        bool notify )
{

  try
  {

    _fromFovCenterTranslation3d->setDirectTranslation( fromFovCenter,
                                                       updating,
                                                       false );

    if ( updating )
    {

      this->update( 0, &notify );

    }
    if ( !updating && notify )
    {

      this->notifyObservers( this );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setFromFieldOfViewCenter( "
             "const gkg::Vector3d< T >& fromFovCenter, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::setToFieldOfViewCenter(
                                          const gkg::Vector3d< T >& toFovCenter,
                                          bool updating,
                                          bool notify )
{

  try
  {

    _toFovCenterInverseTranslation3d->setInverseTranslation( toFovCenter,
                                                             updating,
                                                             false );

    if ( updating )
    {

      this->update( 0, &notify );

    }
    if ( !updating && notify )
    {

      this->notifyObservers( this );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::setToFieldOfViewCenter( "
             "const gkg::Vector3d< T >& toFovCenter, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::RigidTransform3d< T >::read( std::istream& is )
{

  try
  {

    std::vector< T > parameters( 6 );
    is >> parameters[ 0 ]
       >> parameters[ 1 ]
       >> parameters[ 2 ]
       >> parameters[ 3 ]
       >> parameters[ 4 ]
       >> parameters[ 5 ];

    setDirectParameters( parameters, true );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::read( std::istream& is )" );

}


template < class T >
void gkg::RigidTransform3d< T >::readTrm( std::istream& is )
{

  try
  {

    gkg::Vector3d< T > translation;
    gkg::Vector3d< T > u, v, w;
    is >> translation.x
       >> translation.y
       >> translation.z
       >> u.x
       >> v.x
       >> w.x
       >> u.y
       >> v.y
       >> w.y
       >> u.z
       >> v.z
       >> w.z;
    if ( ( std::abs( u.getNorm() - 1.0 ) > EPSILON ) ||
         ( std::abs( v.getNorm() - 1.0 ) > EPSILON ) ||
         ( std::abs( w.getNorm() - 1.0 ) > EPSILON ) )
    {

      throw std::runtime_error( "not a rigid transform" );

    }
         
    gkg::Vector3d< T > angles = gkg::getXYZAnglesFromTrieder( u, v, w );
    setDirectParameters( angles, translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::read( std::istream& is )" );

}


template < class T >
void gkg::RigidTransform3d< T >::write( std::ostream& os ) const
{

  try
  {

    std::vector< T > parameters( 6 );
    getDirectParameters( parameters, true );
    os << parameters[ 0 ] << "\t"
       << parameters[ 1 ] << "\t"
       << parameters[ 2 ] << "\t"
       << parameters[ 3 ] << "\t"
       << parameters[ 4 ] << "\t"
       << parameters[ 5 ] << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::write( std::ostream& os ) "
             "const" );

}


template < class T >
void gkg::RigidTransform3d< T >::writeTrm( std::ostream& os ) const
{

  try
  {

    this->getComposition().writeTrm( os );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RigidTransform3d< T >::writeTrm( std::ostream& os ) "
             "const" );

}


template class gkg::RigidTransform3d< float >;
template class gkg::RigidTransform3d< double >;


#undef EPSILON
