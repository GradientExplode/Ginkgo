#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Shearing3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/BadParameterCountException.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <string>


template < class T >
gkg::AffineTransform3d< T >::AffineTransform3d( const T& scalingX,
                                                const T& scalingY,
                                                const T& scalingZ,
                                                const T& shearingXY,
                                                const T& shearingXZ,
                                                const T& shearingYZ,
                                                const T& angleX,
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

    _scaling3d.reset( new gkg::Scaling3d< T >( scalingX, scalingY, scalingZ,
                                               direct ) );
    _shearing3d.reset( new gkg::Shearing3d< T >( shearingXY, shearingXZ,
                                                 shearingYZ, direct ) );
    _rotation3d.reset( new gkg::Rotation3d< T >( angleX, angleY, angleZ,
                                             degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( translationX,
                                                   translationY,
                                                   translationZ,
                                                   direct ) );

    if ( direct )
    {
    
      transforms.push_back( _scaling3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );

    }
    else
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _scaling3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::AffineTransform3d< T >::"
             "AffineTransform3d( "
             "const T& scalingX, "
             "const T& scalingY, "
             "const T& scalingZ, "
             "const T& shearingX, "
             "const T& shearingY, "
             "const T& shearingZ, "
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
gkg::AffineTransform3d< T >::AffineTransform3d(
                                         const gkg::Vector3d< T >& scalings,
                                         const gkg::Vector3d< T >& shearings,
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

    _scaling3d.reset( new gkg::Scaling3d< T >( scalings, direct ) );
    _shearing3d.reset( new gkg::Shearing3d< T >( shearings, direct ) );
    _rotation3d.reset( new gkg::Rotation3d< T >( angles, degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( translation, direct ) );

    if ( direct )
    {
    
      transforms.push_back( _scaling3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );

    }
    else
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _scaling3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::AffineTransform3d< T >::"
             "AffineTransform3d( "
             "const gkg::Vector3d< T >& scalings, "
             "const gkg::Vector3d< T >& shearings, "
             "const gkg::Vector3d< T >& angles, "
             "const gkg::Vector3d< T >& translation, "
             "bool degree, "
             "bool direct )" );

}


template < class T >
gkg::AffineTransform3d< T >::AffineTransform3d(
                                            const std::vector< T >& parameters,
                                            bool degree,
                                            bool direct )
                                      : gkg::CompositeTransform3d< T >()
{

  try
  {

    if ( parameters.size() != 12U )
    {

      throw gkg::BadParameterCountException( 12 );

    }

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _scaling3d.reset( new gkg::Scaling3d< T >( parameters[ 0 ],
                                               parameters[ 1 ],
                                               parameters[ 2 ],
                                               direct ) );
    _shearing3d.reset( new gkg::Shearing3d< T >( parameters[ 3 ],
                                                 parameters[ 4 ],
                                                 parameters[ 5 ],
                                                 direct ) );
    _rotation3d.reset( new gkg::Rotation3d< T >( parameters[ 6 ],
                                                 parameters[ 7 ],
                                                 parameters[ 8 ],
                                                 degree, direct ) );
    _translation3d.reset( new gkg::Translation3d< T >( parameters[ 9 ],
                                                       parameters[ 10 ],
                                                       parameters[ 11 ],
                                                       direct ) );

    if ( direct )
    {
    
      transforms.push_back( _scaling3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _translation3d );

    }
    else
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _rotation3d );
      transforms.push_back( _shearing3d );
      transforms.push_back( _scaling3d );

    }

    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T > "
             "gkg::AffineTransform3d< T >::"
             "AffineTransform3d( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool direct )" );

}


template < class T >
gkg::AffineTransform3d< T >::AffineTransform3d(
                           const gkg::AffineTransform3d< T >& other )
                                      : gkg::CompositeTransform3d< T >( other )
{

  try
  {

    _scaling3d = this->_transforms[ 0 ];
    _shearing3d = this->_transforms[ 1 ];
    _rotation3d = this->_transforms[ 2 ];
    _translation3d = this->_transforms[ 3 ];

  }
  GKG_CATCH( "template < class T > "
             "gkg::AffineTransform3d< T >::"
             "AffineTransform3d( "
             "const gkg::AffineTransform3d< T >& other )" );

}


template < class T >
gkg::AffineTransform3d< T >::~AffineTransform3d()
{
}


template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::AffineTransform3d< T >::clone() const
{

  try
  {

    gkg::RCPointer< gkg::Transform3d< T > > transform;
    transform.reset( new gkg::AffineTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::AffineTransform3d< T >::clone() const" );

}


template < class T >
gkg::AffineTransform3d< T >& 
gkg::AffineTransform3d< T >::operator=(
                           const gkg::AffineTransform3d< T >& other )
{

  try
  {

    this->gkg::CompositeTransform3d< T >::operator=( other );

    _scaling3d =  other._transforms[ 0 ];
    _shearing3d = other._transforms[ 1 ];
    _rotation3d = other._transforms[ 2 ];
    _translation3d = other._transforms[ 3 ];

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::AffineTransform3d< T >& "
             "gkg::AffineTransform3d< T >::operator=( "
             "const gkg::AffineTransform3d< T >& other )" );

}


template < class T >
void gkg::AffineTransform3d< T >::setDirectParameters(
                                                      const T& scalingX,
                                                      const T& scalingY,
                                                      const T& scalingZ,
                                                      const T& shearingXY,
                                                      const T& shearingXZ,
                                                      const T& shearingYZ,
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

    _scaling3d->setDirectScaling( scalingX,
                                  scalingY,
                                  scalingZ,
                                  updating, false );
    _shearing3d->setDirectShearings( shearingXY,
                                     shearingXZ,
                                     shearingYZ,
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
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "setDirectParameters( "
             "const T& scalingX, "
             "const T& scalingY, "
             "const T& scalingZ, "
             "const T& shearingXY, "
             "const T& shearingXZ, "
             "const T& shearingYZ, "
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
void gkg::AffineTransform3d< T >::setDirectParameters(
                                         const gkg::Vector3d< T >& scalings,
                                         const gkg::Vector3d< T >& shearings,
                                         const gkg::Vector3d< T >& angles,
                                         const gkg::Vector3d< T >& translation,
                                         bool degree,
                                         bool updating,
                                         bool notify )
{

  try
  {

    _scaling3d->setDirectScaling( scalings, updating, false );
    _shearing3d->setDirectShearings( shearings, updating, false );
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
             "void gkg::AffineTransform3d< T >::"
             "setDirectParameters( "
             "const gkg::Vector3d< T >& scalings, "
             "const gkg::Vector3d< T >& shearings, "
             "const gkg::Vector3d< T >& angles, "
             "const gkg::Vector3d< T >& translation, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::AffineTransform3d< T >::setDirectParameters(
                                            const std::vector< T >& parameters,
                                            bool degree,
                                            bool updating,
                                            bool notify )
{

  try
  {

    if ( parameters.size() != 12U )
    {

      throw gkg::BadParameterCountException( 12 );

    }
    _scaling3d->setDirectScaling( parameters[ 0 ],
                                  parameters[ 1 ],
                                  parameters[ 2 ],
                                  updating, false );
    _shearing3d->setDirectShearings( parameters[ 3 ],
                                     parameters[ 4 ],
                                     parameters[ 5 ],
                                     updating, false );
    _rotation3d->setDirectAngles( parameters[ 6 ],
                                  parameters[ 7 ],
                                  parameters[ 8 ],
                                  degree, updating, false );
    _translation3d->setDirectTranslation( parameters[ 9 ],
                                          parameters[ 10 ],
                                          parameters[ 11 ],
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
             "void gkg::AffineTransform3d< T >::"
             "setDirectParameters( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::AffineTransform3d< T >::getDirectParameters(
                                                      T& scalingX,
                                                      T& scalingY,
                                                      T& scalingZ,
                                                      T& shearingXY,
                                                      T& shearingXZ,
                                                      T& shearingYZ,
                                                      T& angleX,
                                                      T& angleY,
                                                      T& angleZ,
                                                      T& translationX,
                                                      T& translationY,
                                                      T& translationZ,
                                                      bool degree ) const
{

  try
  {

    _scaling3d->getDirectScaling( scalingX,
                                  scalingY,
                                  scalingZ );
    _shearing3d->getDirectShearings( shearingXY,
                                     shearingXZ,
                                     shearingYZ );
    _rotation3d->getDirectAngles( angleX,
                                  angleY,
                                  angleZ,
                                  degree );
    _translation3d->getDirectTranslation( translationX,
                                          translationY,
                                          translationZ );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getDirectParameters( "
             "T& scalingX, "
             "T& scalingY, "
             "T& scalingZ, "
             "T& shearingXY, "
             "T& shearingXZ, "
             "T& shearingYZ, "
             "T& angleX, "
             "T& angleY, "
             "T& angleZ, "
             "T& translationX, "
             "T& translationY, "
             "T& translationZ, "
             "bool degree ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::getDirectParameters(
                                               gkg::Vector3d< T >& scalings,
                                               gkg::Vector3d< T >& shearings,
                                               gkg::Vector3d< T >& angles,
                                               gkg::Vector3d< T >& translation,
                                               bool degree ) const
{

  try
  {

    _scaling3d->getDirectScaling( scalings );
    _shearing3d->getDirectShearings( shearings );
    _rotation3d->getDirectAngles( angles, degree );
    _translation3d->getDirectTranslation( translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getDirectParameters( "
             "gkg::Vector3d< T >& scalings, "
             "gkg::Vector3d< T >& shearings, "
             "gkg::Vector3d< T >& angles, "
             "gkg::Vector3d< T >& translation, "
             "bool degree ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::getDirectParameters(
                                                  std::vector< T >& parameters,
                                                  bool degree ) const
{

  try
  {

    if ( parameters.size() != 12U )
    {

      parameters = std::vector< T >( 12 );

    }
    _scaling3d->getDirectScaling( parameters[ 0 ],
                                  parameters[ 1 ],
                                  parameters[ 2 ] );
    _shearing3d->getDirectShearings( parameters[ 3 ],
                                     parameters[ 4 ],
                                     parameters[ 5 ] );
    _rotation3d->getDirectAngles( parameters[ 6 ],
                                  parameters[ 7 ],
                                  parameters[ 8 ],
                                  degree );
    _translation3d->getDirectTranslation( parameters[ 9 ],
                                          parameters[ 10 ],
                                          parameters[ 11 ] );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getDirectParameters( "
             "std::vector< T >& parameters, "
             "bool degree ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::setInverseParameters(
                                                       const T& scalingX,
                                                       const T& scalingY,
                                                       const T& scalingZ,
                                                       const T& shearingXY,
                                                       const T& shearingXZ,
                                                       const T& shearingYZ,
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

    _scaling3d->setInverseScaling( scalingX,
                                   scalingY,
                                   scalingZ,
                                   updating, false );
    _shearing3d->setInverseShearings( shearingXY,
                                      shearingXZ,
                                      shearingYZ,
                                      updating, false );
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
             "void gkg::AffineTransform3d< T >::"
             "setInverseParameters( "
             "const T& scalingX, "
             "const T& scalingY, "
             "const T& scalingZ, "
             "const T& shearingXY, "
             "const T& shearingXZ, "
             "const T& shearingYZ, "
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
void gkg::AffineTransform3d< T >::setInverseParameters(
                                          const gkg::Vector3d< T >& scalings,
                                          const gkg::Vector3d< T >& shearings,
                                          const gkg::Vector3d< T >& angles,
                                          const gkg::Vector3d< T >& translation,
                                          bool degree,
                                          bool updating,
                                          bool notify )
{

  try
  {

    _scaling3d->setInverseScaling( scalings, updating, false );
    _shearing3d->setInverseShearings( shearings, updating, false );
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
             "void gkg::AffineTransform3d< T >::"
             "setInverseParameters( "
             "const gkg::Vector3d< T >& scalings, "
             "const gkg::Vector3d< T >& shearings, "
             "const gkg::Vector3d< T >& angles, "
             "const gkg::Vector3d< T >& translation, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::AffineTransform3d< T >::setInverseParameters(
                                             const std::vector< T >& parameters,
                                             bool degree,
                                             bool updating,
                                             bool notify )
{

  try
  {

    if ( parameters.size() != 12U )
    {

      throw gkg::BadParameterCountException( 12 );

    }
    _scaling3d->setInverseScaling( parameters[ 0 ],
                                   parameters[ 1 ],
                                   parameters[ 2 ],
                                   updating, false );
    _shearing3d->setInverseShearings( parameters[ 3 ],
                                      parameters[ 4 ],
                                      parameters[ 5 ],
                                      updating, false );
    _rotation3d->setInverseAngles( parameters[ 6 ],
                                   parameters[ 7 ],
                                   parameters[ 8 ],
                                   degree, updating, false );
    _translation3d->setInverseTranslation( parameters[ 9 ],
                                           parameters[ 10 ],
                                           parameters[ 11 ],
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
             "void gkg::AffineTransform3d< T >::setInverseParameters( "
             "const std::vector< T >& parameters, "
             "bool degree, "
             "bool updating, "
             "bool notify )" );

}


template < class T >
void gkg::AffineTransform3d< T >::getInverseParameters(
                                                       T& scalingX,
                                                       T& scalingY,
                                                       T& scalingZ,
                                                       T& shearingXY,
                                                       T& shearingXZ,
                                                       T& shearingYZ,
                                                       T& angleX,
                                                       T& angleY,
                                                       T& angleZ,
                                                       T& translationX,
                                                       T& translationY,
                                                       T& translationZ,
                                                       bool degree ) const
{

  try
  {

    _scaling3d->getInverseScaling( scalingX,
                                   scalingY,
                                   scalingZ );
    _shearing3d->getInverseShearings( shearingXY,
                                      shearingXZ,
                                      shearingYZ );
    _rotation3d->getInverseAngles( angleX,
                                   angleY,
                                   angleZ,
                                   degree );
    _translation3d->getInverseTranslation( translationX,
                                           translationY,
                                           translationZ );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getInverseParameters( "
             "T& scalingX, "
             "T& scalingY, "
             "T& scalingZ, "
             "T& shearingXY, "
             "T& shearingXZ, "
             "T& shearingYZ, "
             "T& angleX, "
             "T& angleY, "
             "T& angleZ, "
             "T& translationX, "
             "T& translationY, "
             "T& translationZ, "
             "bool degree ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::getInverseParameters(
                                                gkg::Vector3d< T >& scalings,
                                                gkg::Vector3d< T >& shearings,
                                                gkg::Vector3d< T >& angles,
                                                gkg::Vector3d< T >& translation,
                                                bool degree ) const
{

  try
  {

    _scaling3d->getInverseScaling( scalings );
    _shearing3d->getInverseShearings( shearings );
    _rotation3d->getInverseAngles( angles, degree );
    _translation3d->getInverseTranslation( translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getInverseParameters( "
             "gkg::Vector3d< T >& scalings, "
             "gkg::Vector3d< T >& shearings, "
             "gkg::Vector3d< T >& angles, "
             "gkg::Vector3d< T >& translation, "
             "bool degree ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::getInverseParameters(
                                                   std::vector< T >& parameters,
                                                   bool degree ) const
{

  try
  {

    if ( parameters.size()!= 12U )
    {

      parameters = std::vector< T >( 12 );

    }
    _scaling3d->getInverseScaling( parameters[ 0 ],
                                   parameters[ 1 ],
                                   parameters[ 2 ] );
    _shearing3d->getInverseShearings( parameters[ 3 ],
                                      parameters[ 4 ],
                                      parameters[ 5 ] );
    _rotation3d->getInverseAngles( parameters[ 6 ],
                                   parameters[ 7 ],
                                   parameters[ 8 ],
                                   degree );
    _translation3d->getInverseTranslation( parameters[ 9 ],
                                           parameters[ 10 ],
                                           parameters[ 11 ] );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::"
             "getInverseParameters( "
             "std::vector< T >& parameters, "
             "bool degree ) const" );

}


template < class T >
const gkg::Scaling3d< T >& 
gkg::AffineTransform3d< T >::getScaling3d() const
{

  try
  {

    return *_scaling3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Scaling3d< T >& "
             "gkg::AffineTransform3d< T >::getScaling3d() const" );

}


template < class T >
const gkg::Shearing3d< T >& 
gkg::AffineTransform3d< T >::getShearing3d() const
{

  try
  {

    return *_shearing3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Shearing3d< T >& "
             "gkg::AffineTransform3d< T >::getShearing3d() const" );

}


template < class T >
const gkg::Rotation3d< T >& 
gkg::AffineTransform3d< T >::getRotation3d() const
{

  try
  {

    return *_rotation3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Rotation3d< T >& "
             "gkg::AffineTransform3d< T >::getRotation3d() const" );

}


template < class T >
const gkg::Translation3d< T >&
gkg::AffineTransform3d< T >::getTranslation3d() const
{

  try
  {

    return *_translation3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::Translation3d< T >& "
             "gkg::AffineTransform3d< T >::getTranslation3d() "
             "const" );

}


template < class T >
T gkg::AffineTransform3d< T >::getNorm2() const
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

    // getting scalings
    gkg::Vector3d< T > scaling;
    _scaling3d->getDirectScaling( scaling );

    // getting shearings
    gkg::Vector3d< T > shearings;
    _shearing3d->getDirectShearings( shearings );

    return theta * theta +
           translation.getNorm2() +
           ( scaling - gkg::Vector3d< T >( 1.0, 1.0, 1.0 ) ).getNorm2() +
           shearings.getNorm2();

  }
  GKG_CATCH( "template < class T > "
             "T gkg::AffineShearingTransform3d< T >::getNorm2() const" );

}


template < class T >
void gkg::AffineTransform3d< T >::read( std::istream& is )
{

  try
  {

    std::vector< T > parameters( 12 );
    is >> parameters[ 0 ]
       >> parameters[ 1 ]
       >> parameters[ 2 ]
       >> parameters[ 3 ]
       >> parameters[ 4 ]
       >> parameters[ 5 ]
       >> parameters[ 6 ]
       >> parameters[ 7 ]
       >> parameters[ 8 ]
       >> parameters[ 9 ]
       >> parameters[ 10 ]
       >> parameters[ 11 ];

    setDirectParameters( parameters, true );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::read( "
             "std::istream& is )" );

}


template < class T >
void gkg::AffineTransform3d< T >::readTrm( std::istream& is )
{

  try
  {

    gkg::Vector3d< T > translation;
    gkg::Matrix rotationAndScalingAndShearings( 3, 3 );
    is >> translation.x
       >> translation.y
       >> translation.z
       >> rotationAndScalingAndShearings( 0, 0 )
       >> rotationAndScalingAndShearings( 0, 1 )
       >> rotationAndScalingAndShearings( 0, 2 )
       >> rotationAndScalingAndShearings( 1, 0 )
       >> rotationAndScalingAndShearings( 1, 1 )
       >> rotationAndScalingAndShearings( 1, 2 )
       >> rotationAndScalingAndShearings( 2, 0 )
       >> rotationAndScalingAndShearings( 2, 1 )
       >> rotationAndScalingAndShearings( 2, 2 );
    gkg::Vector3d< T > scalings, shearings, u, v, w;
    gkg::getXYZScalingsAndShearingsAndTrieder( rotationAndScalingAndShearings,
                                               scalings,
                                               shearings,
                                               u, v, w );
    gkg::Vector3d< T > angles = gkg::getXYZAnglesFromTrieder( u, v, w );
    setDirectParameters( scalings, shearings, angles, translation );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::read( "
             "std::istream& is )" );

}


template < class T >
void gkg::AffineTransform3d< T >::write( std::ostream& os ) const
{

  try
  {

    std::vector< T > parameters( 12 );
    getDirectParameters( parameters, true );
    os << parameters[ 0 ] << "\t"
       << parameters[ 1 ] << "\t"
       << parameters[ 2 ] << "\t"
       << parameters[ 3 ] << "\t"
       << parameters[ 4 ] << "\t"
       << parameters[ 5 ] << "\t"
       << parameters[ 6 ] << "\t"
       << parameters[ 7 ] << "\t"
       << parameters[ 8 ] << "\t"
       << parameters[ 9 ] << "\t"
       << parameters[ 10 ] << "\t"
       << parameters[ 11 ] << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::write( "
             "std::ostream& os ) const" );

}


template < class T >
void gkg::AffineTransform3d< T >::writeTrm( std::ostream& os ) const
{

  try
  {

    this->getComposition().writeTrm( os );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AffineTransform3d< T >::writeTrm( "
             "std::ostream& os ) const" );

}


template class gkg::AffineTransform3d< float >;
template class gkg::AffineTransform3d< double >;

