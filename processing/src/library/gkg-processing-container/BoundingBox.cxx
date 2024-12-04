#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::BoundingBox< T >::BoundingBox( const T& lowerX, const T& upperX,
                                    const T& lowerY, const T& upperY,
                                    const T& lowerZ, const T& upperZ,
                                    const T& lowerT, const T& upperT )
                      : gkg::RegionInformation(),
                        _lowerX( lowerX ),
                        _upperX( upperX ),
                        _lowerY( lowerY ),
                        _upperY( upperY ),
                        _lowerZ( lowerZ ),
                        _upperZ( upperZ ),
                        _lowerT( lowerT ),
                        _upperT( upperT )
{
}


template < class T >
gkg::BoundingBox< T >::BoundingBox( const gkg::BoundingBox< T >& other )
                      : gkg::RegionInformation(),
                        _lowerX( other._lowerX ),
                        _upperX( other._upperX ),
                        _lowerY( other._lowerY ),
                        _upperY( other._upperY ),
                        _lowerZ( other._lowerZ ),
                        _upperZ( other._upperZ ),
                        _lowerT( other._lowerT ),
                        _upperT( other._upperT )
{
}


template < class T >
gkg::BoundingBox< T >::BoundingBox( const gkg::HeaderedObject& object )
                      : gkg::RegionInformation(),
                        _lowerX( 0 ),
                        _lowerY( 0 ),
                        _lowerZ( 0 ),
                        _lowerT( 0 )
{

  try
  {

    int32_t size;

    _upperX = _upperY = _upperZ = _upperT = 0;

    if ( object.getHeader().hasAttribute( "sizeX" ) )
    {

      object.getHeader().getAttribute( "sizeX", size );
      _upperX = size - 1;

    }

    if ( object.getHeader().hasAttribute( "sizeY" ) )
    {

      object.getHeader().getAttribute( "sizeY", size );
      _upperY = size - 1;

    }

    if ( object.getHeader().hasAttribute( "sizeZ" ) )
    {

      object.getHeader().getAttribute( "sizeZ", size );
      _upperZ = size - 1;

    }

    if ( object.getHeader().hasAttribute( "sizeT" ) )
    {

      object.getHeader().getAttribute( "sizeT", size );
      _upperT = size - 1;

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::BoundingBox< T >::BoundingBox( "
             "gkg::HeaderedObject& object )" );

}


template < class T >
gkg::BoundingBox< T >::~BoundingBox()
{
}


template < class T >
gkg::BoundingBox< T >& 
gkg::BoundingBox< T >::operator=( const gkg::BoundingBox< T >& other )
{

  _lowerX = other._lowerX;
  _upperX = other._upperX;
  _lowerY = other._lowerY;
  _upperY = other._upperY;
  _lowerZ = other._lowerZ;
  _upperZ = other._upperZ;
  _lowerT = other._lowerT;
  _upperT = other._upperT;

  return *this;

}

template < class T >
void gkg::BoundingBox< T >::setLowerX( const T& lowerX )
{

  _lowerX = lowerX;

}


template < class T >
const T& gkg::BoundingBox< T >::getLowerX() const
{

  return _lowerX;

}


template < class T >
void gkg::BoundingBox< T >::setUpperX( const T& upperX )
{

  _upperX = upperX;

}


template < class T >
const T& gkg::BoundingBox< T >::getUpperX() const
{

  return _upperX;

}


template < class T >
void gkg::BoundingBox< T >::setLowerY( const T& lowerY )
{

  _lowerY = lowerY;

}


template < class T >
const T& gkg::BoundingBox< T >::getLowerY() const
{

  return _lowerY;

}


template < class T >
void gkg::BoundingBox< T >::setUpperY( const T& upperY )
{

  _upperY = upperY;

}


template < class T >
const T& gkg::BoundingBox< T >::getUpperY() const
{

  return _upperY;

}


template < class T >
void gkg::BoundingBox< T >::setLowerZ( const T& lowerZ )
{

  _lowerZ = lowerZ;

}


template < class T >
const T& gkg::BoundingBox< T >::getLowerZ() const
{

  return _lowerZ;

}


template < class T >
void gkg::BoundingBox< T >::setUpperZ( const T& upperZ )
{

  _upperZ = upperZ;

}


template < class T >
const T& gkg::BoundingBox< T >::getUpperZ() const
{

  return _upperZ;

}


template < class T >
void gkg::BoundingBox< T >::setLowerT( const T& lowerT )
{

  _lowerT = lowerT;

}


template < class T >
const T& gkg::BoundingBox< T >::getLowerT() const
{

  return _lowerT;

}


template < class T >
void gkg::BoundingBox< T >::setUpperT( const T& upperT )
{

  _upperT = upperT;

}


template < class T >
const T& gkg::BoundingBox< T >::getUpperT() const
{

  return _upperT;

}


template < class T >
T gkg::BoundingBox< T >::getSpanX() const
{

  return _upperX - _lowerX;

}


template < class T >
T gkg::BoundingBox< T >::getSpanY() const
{

  return _upperY - _lowerY;

}


template < class T >
T gkg::BoundingBox< T >::getSpanZ() const
{

  return _upperZ - _lowerZ;

}


template < class T >
T gkg::BoundingBox< T >::getSpanT() const
{

  return _upperT - _lowerT;

}


template < class T >
gkg::Vector3d< double > gkg::BoundingBox< T >::getSpan() const
{

  return gkg::Vector3d< double >( ( double )( _upperX - _lowerX ),
                                  ( double )( _upperY - _lowerY ),
                                  ( double )( _upperZ - _lowerZ ) );

}


template < class T >
bool gkg::BoundingBox< T >::contains( const T& x,
                                      const T& y,
                                      const T& z,
                                      const T& t ) const
{

  return ( x >= _lowerX ) && ( x <= _upperX ) &&
         ( y >= _lowerY ) && ( y <= _upperY ) &&
         ( z >= _lowerZ ) && ( z <= _upperZ ) &&
         ( t >= _lowerT ) && ( t <= _upperT );

}


template < class T >
bool gkg::BoundingBox< T >::contains( const gkg::Vector2d< T >& site,
                                      const T& z,
                                      const T& t ) const
{

  return ( site.x >= _lowerX ) && ( site.x <= _upperX ) &&
         ( site.y >= _lowerY ) && ( site.y <= _upperY ) &&
         ( z >= _lowerZ ) && ( z <= _upperZ ) &&
         ( t >= _lowerT ) && ( t <= _upperT );

}


template < class T >
bool gkg::BoundingBox< T >::contains( const gkg::Vector3d< T >& site,
                                      const T& t ) const
{

  return ( site.x >= _lowerX ) && ( site.x <= _upperX ) &&
         ( site.y >= _lowerY ) && ( site.y <= _upperY ) &&
         ( site.z >= _lowerZ ) && ( site.z <= _upperZ ) &&
         ( t >= _lowerT ) && ( t <= _upperT );

}


template < class T >
bool gkg::BoundingBox< T >::contains( const gkg::BoundingBox< T >& other ) const
{

  return ( other.getLowerX() >= _lowerX ) && ( other.getUpperX() <= _upperX ) &&
         ( other.getLowerY() >= _lowerY ) && ( other.getUpperY() <= _upperY ) &&
         ( other.getLowerZ() >= _lowerZ ) && ( other.getUpperZ() <= _upperZ ) &&
         ( other.getLowerT() >= _lowerT ) && ( other.getUpperT() <= _upperT );

}


template < class T >
bool gkg::BoundingBox< T >::isOnBoundary( const T& x,
                                          const T& y,
                                          const T& z,
                                          const T& t ) const
{

  bool result = false;

  // the bounding box is 3D
  if ( _upperT == 0 )
  {

    // the bounding box is 2D
    if ( _upperZ == 0 )
    {

      result = ( ( x == _lowerX ) || ( x == _upperX ) ||
                 ( y == _lowerY ) || ( y == _upperY ) ) &&
               contains( x, y, z, t );

    }
    // the bounding box is really 3D
    else
    {

      result = ( ( x == _lowerX ) || ( x == _upperX ) ||
                 ( y == _lowerY ) || ( y == _upperY ) ||
                 ( z == _lowerZ ) || ( z == _upperZ ) ) &&
               contains( x, y, z, t );

    }

  }
  // else it is a 4D container
  else
  {

    result = ( ( x == _lowerX ) || ( x == _upperX ) ||
               ( y == _lowerY ) || ( y == _upperY ) ||
               ( z == _lowerZ ) || ( z == _upperZ ) ||
               ( t == _lowerT ) || ( t == _upperT ) ) &&
             contains( x, y, z, t );

  }

  return result;

}


template < class T >
bool gkg::BoundingBox< T >::isOnBoundary( const gkg::Vector2d< T >& site,
                                          const T& z,
                                          const T& t ) const
{

  return isOnBoundary( site.x, site.y, z, t );

}


template < class T >
bool gkg::BoundingBox< T >::isOnBoundary( const gkg::Vector3d< T >& site,
                                          const T& t ) const
{

  return isOnBoundary( site.x, site.y, site.z, t );

}


template class gkg::BoundingBox< int8_t >;
template class gkg::BoundingBox< int16_t >;
template class gkg::BoundingBox< int32_t >;
template class gkg::BoundingBox< int64_t >;
template class gkg::BoundingBox< float >;
template class gkg::BoundingBox< double >;
