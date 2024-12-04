#include <gkg-processing-transform/FrameTransform3d.h>
#include <gkg-processing-transform/HomogeneousTransform3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-exception/BadParameterCountException.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <string>

template < class T >
gkg::FrameTransform3d< T >::FrameTransform3d( const Vector3d< T >& vectorU,
                                              const Vector3d< T >& vectorV,
                                              const Vector3d< T >& vectorW,
                                              const Vector3d< T >& origin,
                                              bool direct )
                           : gkg::CompositeTransform3d< T >()
{

  try
  {

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _frame3d.reset( new gkg::HomogeneousTransform3d< T >(
                                      vectorU.x, vectorU.y, vectorU.z,( T )0.0,
                                      vectorV.x, vectorV.y, vectorV.z,( T )0.0,
                                      vectorW.x, vectorW.y, vectorW.z,( T )0.0,
                                      ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,
                                      direct ) );

    _translation3d.reset( new gkg::Translation3d< T >( origin, direct ) );

    if ( direct )
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _frame3d );
      
    }
    else
    {
    
      transforms.push_back( _frame3d );
      transforms.push_back( _translation3d );
      
    }    

    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T >"
             "gkg::FrameTransform3d< T >::FrameTransform3d( "
             "const Vector3d< T >& vectorU,"
             "const Vector3d< T >& vectorV,"
             "const Vector3d< T >& vectorW,"
             "const Vector3d< T >& origin,"
             "bool direct = true )" );
    
}


template < class T >
gkg::FrameTransform3d< T >::FrameTransform3d( 
                                         const T& uX, const T& uY, const T& uZ,
                                         const T& vX, const T& vY, const T& vZ,
                                         const T& wX, const T& wY, const T& wZ,
                                         const T& originX,
                                         const T& originY,
                                         const T& originZ,
                                         bool direct )
                           : gkg::CompositeTransform3d< T >() 
{

  try
  {

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _frame3d.reset(new gkg::HomogeneousTransform3d< T >(
                                       uX, uY, uZ, ( T )0.0,
                                       vX, vY, vZ, ( T )0.0,
                                       wX, wY, wZ, ( T )0.0,
                                       ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,
                                       direct ) );

    _translation3d.reset(new gkg::Translation3d< T >( originX, originY, originZ,
                                                  direct ) );

    if ( direct )
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _frame3d );
      
    }
    else
    {
    
      transforms.push_back( _frame3d );
      transforms.push_back( _translation3d );
      
    }    

    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T >"
             "gkg::FrameTransform3d< T >::FrameTransform3d(" 
             "const T& uX, const T& uY, const T& uZ,"
             "const T& vX, const T& vY, const T& vZ,"
             "const T& wX, const T& wY, const T& wZ,"
             "const T& originX, const T& originY, const T& originZ,"
             "bool direct )" );
  
}

template < class T >
gkg::FrameTransform3d< T >::FrameTransform3d( 
                               const std::vector< T >& parameters, bool direct )
                           : gkg::CompositeTransform3d< T >()
{

  try
  {

    if ( parameters.size()!= 12U )
    {

      throw gkg::BadParameterCountException( 12 );

    }

    std::deque< gkg::RCPointer< gkg::Transform3d< T > > > transforms;
    std::deque< bool > clonings;

    _frame3d.reset( new gkg::HomogeneousTransform3d< T >(
                    parameters[ 0 ], parameters[ 1 ], parameters[ 2 ], ( T )0.0,
                    parameters[ 3 ], parameters[ 4 ], parameters[ 5 ], ( T )0.0,
                    parameters[ 6 ], parameters[ 7 ], parameters[ 8 ], ( T )0.0,
                    ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,
                    direct ) );

    _translation3d.reset( new gkg::Translation3d< T >( parameters[ 9 ],
                                                  parameters[ 10 ],
                                                  parameters[ 11 ],
                                                  direct ) );

    if ( direct )
    {
    
      transforms.push_back( _translation3d );
      transforms.push_back( _frame3d );
      
    }
    else
    {
    
      transforms.push_back( _frame3d );
      transforms.push_back( _translation3d );
      
    }    

    clonings.push_back( false );
    clonings.push_back( false );

    this->compose( transforms, clonings, true, false );

  }
  GKG_CATCH( "template < class T >"
             "gkg::FrameTransform3d< T >::FrameTransform3d( "
             "const std::vector< T >& parameters, bool direct = true )" );
  
}

template < class T >
gkg::FrameTransform3d< T >::FrameTransform3d( 
                                            const FrameTransform3d< T >& other )
                           : gkg::CompositeTransform3d< T >( other )
{

  _translation3d = this->_transforms[ 0 ];
  _frame3d = this->_transforms[ 1 ];

}

template < class T >
gkg::FrameTransform3d< T >::~FrameTransform3d()
{
}

template < class T >
gkg::RCPointer< gkg::Transform3d< T > >
gkg::FrameTransform3d< T >::clone() const
{

  gkg::RCPointer< gkg::Transform3d< T > > transform;
  transform.reset( new gkg::FrameTransform3d< T >( *this ) );
  return transform;

}

template < class T >
gkg::FrameTransform3d< T >& 
gkg::FrameTransform3d< T >::operator=( const gkg::FrameTransform3d< T >& other )
{

  this->gkg::CompositeTransform3d< T >::operator=( other );

  _translation3d = other._transforms[ 0 ];
  _frame3d = other._transforms[ 1 ];

  return *this;

}


template < class T >
void gkg::FrameTransform3d< T >::setDirectParameters(
                                                  const Vector3d< T >& vectorU,
                                                  const Vector3d< T >& vectorV,
                                                  const Vector3d< T >& vectorW,
                                                  const Vector3d< T >& origin,
                                                  bool updating,
                                                  bool notify )
{

  _frame3d->setDirectItems( vectorU.x, vectorU.y, vectorU.z, ( T )0.0,
                            vectorV.x, vectorV.y, vectorV.z, ( T )0.0,
                            vectorW.x, vectorW.y, vectorW.z, ( T )0.0,
                            ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                            updating, false );
  _translation3d->setDirectTranslation( origin,
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
void gkg::FrameTransform3d< T >::setDirectParameters(
                                         const T& uX, const T& uY, const T& uZ,
                                         const T& vX, const T& vY, const T& vZ,
                                         const T& wX, const T& wY, const T& wZ,
                                         const T& originX,
                                         const T& originY,
                                         const T& originZ,
                                         bool  updating,
                                         bool notify  )
{
    
  _frame3d->setDirectItems( uX, uY, uZ, ( T )0.0,
                            vX, vY, vZ, ( T )0.0,
                            wX, wY, wZ, ( T )0.0,
                            ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                            updating, false );
  _translation3d->setDirectTranslation( originX, originY, originZ,
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
void gkg::FrameTransform3d< T >::setDirectParameters(
                                             const std::vector< T >& parameters,
                                             bool  updating,
                                             bool notify )
{

  try
  {
  
    if ( parameters.size()!= 12U )
    {

      throw gkg::BadParameterCountException( 12 );
      
    }
 
    _frame3d->setDirectItems(
                    parameters[ 0 ], parameters[ 1 ], parameters[ 2 ], ( T )0.0,
                    parameters[ 3 ], parameters[ 4 ], parameters[ 5 ], ( T )0.0,
                    parameters[ 6 ], parameters[ 7 ], parameters[ 8 ], ( T )0.0,
                    ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                        updating, false );
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
  GKG_CATCH( "template < class T >"
             "void gkg::FrameTransform3d< T >::setDirectParameters("
             "const std::vector< T >& parameters ) const" );

}

template < class T >
void gkg::FrameTransform3d< T >::getDirectParameters( 
                                                   Vector3d< T >& vectorU,
                                                   Vector3d< T >& vectorV,
                                                   Vector3d< T >& vectorW,
                                                   Vector3d< T >& origin ) const
{

  _translation3d->getDirectTranslation( origin );
  vectorU.x = _frame3d->getDirectCoefficient( 0, 0 );
  vectorU.y = _frame3d->getDirectCoefficient( 0, 1 );
  vectorU.z = _frame3d->getDirectCoefficient( 0, 2 );
  vectorV.x = _frame3d->getDirectCoefficient( 1, 0 );
  vectorV.y = _frame3d->getDirectCoefficient( 1, 1 );
  vectorV.z = _frame3d->getDirectCoefficient( 1, 2 );  
  vectorW.x = _frame3d->getDirectCoefficient( 2, 0 );
  vectorW.y = _frame3d->getDirectCoefficient( 2, 1 );
  vectorW.z = _frame3d->getDirectCoefficient( 2, 2 );
  
}

template < class T >
void gkg::FrameTransform3d< T >::getDirectParameters( T& uX, T& uY, T& uZ,
                                                      T& vX, T& vY, T& vZ,
                                                      T& wX, T& wY, T& wZ,
                                                      T& originX,
                                                      T& originY,
                                                      T& originZ ) const
{

  _translation3d->getDirectTranslation( originX, originY, originZ );
  uX = _frame3d->getDirectCoefficient( 0, 0 );
  uY = _frame3d->getDirectCoefficient( 0, 1 );
  uZ = _frame3d->getDirectCoefficient( 0, 2 );
  vX = _frame3d->getDirectCoefficient( 1, 0 );
  vY = _frame3d->getDirectCoefficient( 1, 1 );
  vZ = _frame3d->getDirectCoefficient( 1, 2 );  
  wX = _frame3d->getDirectCoefficient( 2, 0 );
  wY = _frame3d->getDirectCoefficient( 2, 1 );
  wZ = _frame3d->getDirectCoefficient( 2, 2 );
  
}

template < class T >
void gkg::FrameTransform3d< T >::getDirectParameters(
                                            std::vector< T >& parameters ) const
{

  _translation3d->getDirectTranslation( parameters[ 9 ],
                                        parameters[ 10 ],
                                        parameters[ 11 ] );
  parameters[ 0 ] = _frame3d->getDirectCoefficient( 0, 0 );
  parameters[ 1 ] = _frame3d->getDirectCoefficient( 0, 1 );
  parameters[ 2 ] = _frame3d->getDirectCoefficient( 0, 2 );
  parameters[ 3 ] = _frame3d->getDirectCoefficient( 1, 0 );
  parameters[ 4 ] = _frame3d->getDirectCoefficient( 1, 1 );
  parameters[ 5 ] = _frame3d->getDirectCoefficient( 1, 2 );  
  parameters[ 6 ] = _frame3d->getDirectCoefficient( 2, 0 );
  parameters[ 7 ] = _frame3d->getDirectCoefficient( 2, 1 );
  parameters[ 8 ] = _frame3d->getDirectCoefficient( 2, 2 );
  
}


template < class T >
void gkg::FrameTransform3d< T >::setInverseParameters(
                                                  const Vector3d< T >& vectorU,
                                                  const Vector3d< T >& vectorV,
                                                  const Vector3d< T >& vectorW,
                                                  const Vector3d< T >& origin,
                                                  bool updating,
                                                  bool notify )
{

  _frame3d->setInverseItems( vectorU.x, vectorU.y, vectorU.z, ( T )0.0,
                             vectorV.x, vectorV.y, vectorV.z, ( T )0.0,
                             vectorW.x, vectorW.y, vectorW.z, ( T )0.0,
                             ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                             updating, false );
  _translation3d->setInverseTranslation( origin,
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
void gkg::FrameTransform3d< T >::setInverseParameters(
                                         const T& uX, const T& uY, const T& uZ,
                                         const T& vX, const T& vY, const T& vZ,
                                         const T& wX, const T& wY, const T& wZ,
                                         const T& originX,
                                         const T& originY,
                                         const T& originZ,
                                         bool  updating,
                                         bool notify )
{

  _frame3d->setInverseItems( uX, uY, uZ, ( T )0.0,
                             vX, vY, vZ, ( T )0.0,
                             wX, wY, wZ, ( T )0.0,
                             ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                             updating, false );
  _translation3d->setInverseTranslation( originX, originY, originZ,
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
void gkg::FrameTransform3d< T >::setInverseParameters(
                                     const std::vector< T >& parameters,
                                     bool updating,
                                     bool notify  )
{

  try
  {
  
    if ( parameters.size()!= 12U )
    {

      throw gkg::BadParameterCountException( 12 );
      
    }
 
    _frame3d->setInverseItems(
                    parameters[ 0 ], parameters[ 1 ], parameters[ 2 ], ( T )0.0,
                    parameters[ 3 ], parameters[ 4 ], parameters[ 5 ], ( T )0.0,
                    parameters[ 6 ], parameters[ 7 ], parameters[ 8 ], ( T )0.0,
                    ( T )0.0, ( T )0.0, ( T )0.0, ( T )1.0,  
                    updating, false );
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
  GKG_CATCH( "template < class T >"
             "void gkg::FrameTransform3d< T >::setInverseParameters("
             "const std::vector< T >& parameters ) const" );

}


template < class T >
void gkg::FrameTransform3d< T >::getInverseParameters( 
                                                   Vector3d< T >& vectorU,
                                                   Vector3d< T >& vectorV,
                                                   Vector3d< T >& vectorW,
                                                   Vector3d< T >& origin ) const
{

  _translation3d->getInverseTranslation( origin );
  vectorU.x = _frame3d->getInverseCoefficient( 0, 0 );
  vectorU.y = _frame3d->getInverseCoefficient( 0, 1 );
  vectorU.z = _frame3d->getInverseCoefficient( 0, 2 );
  vectorV.x = _frame3d->getInverseCoefficient( 1, 0 );
  vectorV.y = _frame3d->getInverseCoefficient( 1, 1 );
  vectorV.z = _frame3d->getInverseCoefficient( 1, 2 );  
  vectorW.x = _frame3d->getInverseCoefficient( 2, 0 );
  vectorW.y = _frame3d->getInverseCoefficient( 2, 1 );
  vectorW.z = _frame3d->getInverseCoefficient( 2, 2 );
  
}

template < class T >
void gkg::FrameTransform3d< T >::getInverseParameters( T& uX, T& uY, T& uZ,
                                                       T& vX, T& vY, T& vZ,
                                                       T& wX, T& wY, T& wZ,
                                                       T& originX,
                                                       T& originY,
                                                       T& originZ ) const
{

  _translation3d->getInverseTranslation( originX, originY, originZ );
  uX = _frame3d->getInverseCoefficient( 0, 0 );
  uY = _frame3d->getInverseCoefficient( 0, 1 );
  uZ = _frame3d->getInverseCoefficient( 0, 2 );
  vX = _frame3d->getInverseCoefficient( 1, 0 );
  vY = _frame3d->getInverseCoefficient( 1, 1 );
  vZ = _frame3d->getInverseCoefficient( 1, 2 );  
  wX = _frame3d->getInverseCoefficient( 2, 0 );
  wY = _frame3d->getInverseCoefficient( 2, 1 );
  wZ = _frame3d->getInverseCoefficient( 2, 2 );
  
}

template < class T >
void gkg::FrameTransform3d< T >::getInverseParameters(
                                            std::vector< T >& parameters ) const
{

  _translation3d->getInverseTranslation( parameters[ 9 ],
                                         parameters[ 10 ],
                                         parameters[ 11 ] );
  parameters[ 0 ] = _frame3d->getInverseCoefficient( 0, 0 );
  parameters[ 1 ] = _frame3d->getInverseCoefficient( 0, 1 );
  parameters[ 2 ] = _frame3d->getInverseCoefficient( 0, 2 );
  parameters[ 3 ] = _frame3d->getInverseCoefficient( 1, 0 );
  parameters[ 4 ] = _frame3d->getInverseCoefficient( 1, 1 );
  parameters[ 5 ] = _frame3d->getInverseCoefficient( 1, 2 );  
  parameters[ 6 ] = _frame3d->getInverseCoefficient( 2, 0 );
  parameters[ 7 ] = _frame3d->getInverseCoefficient( 2, 1 );
  parameters[ 8 ] = _frame3d->getInverseCoefficient( 2, 2 );
  
}

template < class T >
const gkg::HomogeneousTransform3d< T >& 
gkg::FrameTransform3d< T >::getFrame3d() const
{

  return *_frame3d;

}


template < class T >
const gkg::Translation3d< T >&
gkg::FrameTransform3d< T >::getTranslation3d() const
{

  return *_translation3d;

}


template class gkg::FrameTransform3d< float >;
template class gkg::FrameTransform3d< double >;



