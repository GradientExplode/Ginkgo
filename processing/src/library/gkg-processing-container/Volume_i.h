#ifndef _gkg_processing_container_Volume_i_h_
#define _gkg_processing_container_Volume_i_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/VolumeProxy_i.h>
#include <gkg-processing-container/AutoMemoryMappedVector_i.h>
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <cstdio>


template < class T >
inline
gkg::Volume< T >::Volume( int32_t sizeX, int32_t sizeY, int32_t sizeZ, 
                          int32_t sizeT )
                 : gkg::VolumeProxy< T >( sizeX, sizeY, sizeZ, sizeT )
{

  try
  {

    allocate();

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< T >::Volume( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::Volume< T >::Volume( const gkg::Vector3d< int32_t >& size,
                          int32_t sizeT )
                 : gkg::VolumeProxy< T >( size, sizeT )
{

  try
  {

    allocate();

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< T >::Volume( "
             "const gkg::Vector3d< int32_t >& size, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::Volume< T >::Volume( const gkg::Vector2d< int32_t >& size,
                          int32_t sizeZ, int32_t sizeT )
                 : gkg::VolumeProxy< T >( size, sizeZ, sizeT )
{

  try
  {

    allocate();

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< T >::Volume( "
             "const gkg::Vector3d< int32_t >& size, "
             "int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::Volume< T >::Volume( const gkg::Volume< T >& other )
                 : gkg::VolumeProxy< T >( other ),
                   _items( other._items )
{

  try
  {

    updateLineAccessors();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Volume< T >::Volume( const gkg::Volume< T >& other )" );

}


template < class T >
inline
gkg::Volume< T >::~Volume()
{
}


template < class T >
inline
gkg::Volume< T >& gkg::Volume< T >::operator=( const gkg::Volume< T >& other )
{

  try
  {

    this->gkg::VolumeProxy< T >::operator=( other );
    _items = other._items;
    updateLineAccessors();

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< T >& gkg::Volume< T >::operator=( "
             "const gkg::Volume< T >& other )" );

}


template < class T >
inline
void gkg::Volume< T >::fill( const T& value )
{

  try
  {

    typename gkg::AutoMemoryMappedVector< T >::iterator
      i = _items.begin(),
      ie =_items.end();
    while ( i != ie )
    {

      *i = value;
      i ++;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Volume< T >::fill( const T& value )" );

}


template < class T >
inline
void gkg::Volume< T >::allocate()
{

  try
  {

    if ( this->_sizeXYZT >
         ( int64_t )std::numeric_limits< int64_t >::max() )
    {

      throw std::runtime_error(
        std::string( "attempt to allocate a volume which size is greater " ) +
        "than allowed by the system (" +
        gkg::StringConverter::toString( 
	  std::numeric_limits< int64_t >::max() ) + " bytes)" );

    }
    _items = gkg::AutoMemoryMappedVector< T >( ( size_t )this->_sizeXYZT );
    updateLineAccessors();

  }
  GKG_CATCH( "template < class T > void gkg::Volume< T >::allocate()" );

}


template < class T >
inline
void gkg::Volume< T >::reallocate( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                                   int32_t sizeT,
                                   bool copyData )
{

  try
  {

    if ( ( sizeX != this->_sizeX ) ||
         ( sizeY != this->_sizeY ) ||
         ( sizeZ != this->_sizeZ ) ||
         ( sizeT != this->_sizeT ) )
    {
       
      // allocating a new data
      gkg::AutoMemoryMappedVector< T >
      newItems( ( typename gkg::AutoMemoryMappedVector< T >::size_type )sizeX * 
                ( typename gkg::AutoMemoryMappedVector< T >::size_type )sizeY * 
                ( typename gkg::AutoMemoryMappedVector< T >::size_type )sizeZ * 
                ( typename gkg::AutoMemoryMappedVector< T >::size_type )sizeT );

      int64_t sizeXY = ( int64_t )sizeX * ( int64_t )sizeY;
      int64_t sizeXYZ = sizeXY * ( int64_t )sizeZ;
      int64_t sizeXYZT = sizeXYZ * ( int64_t )sizeT;

      if ( sizeXYZT > 
           ( int64_t )std::numeric_limits< int64_t >::max() )
      {

        throw std::runtime_error(
        std::string( "attempt to allocate a volume which size is greater " ) +
        "than allowed by the system (" +
        gkg::StringConverter::toString( 
	  std::numeric_limits< int64_t >::max() ) + " bytes)" );

      }

      if ( copyData )
      {

        int32_t minSizeX = std::min( sizeX, this->_sizeX );
        int32_t minSizeY = std::min( sizeY, this->_sizeY );
        int32_t minSizeZ = std::min( sizeZ, this->_sizeZ );
        int32_t minSizeT = std::min( sizeT, this->_sizeT );

        // preserving data
        int32_t x, y, z, t;
        for ( t = 0; t < minSizeT; t++ )
        {

          for ( z = 0; z < minSizeZ; z++ )
          {

            for ( y = 0; y < minSizeY; y++ )
            {

              for ( x = 0; x < minSizeX; x++ )
              {

                newItems[ x +
                          y * sizeX +
                          z * ( size_t )sizeXY +
                          t * ( size_t )sizeXYZ ] =
                _items[ x +
                        y * this->_sizeX +
                        z * ( size_t )this->_sizeXY +
                        t * ( size_t )this->_sizeXYZ ];

              }

            }

          }

        }

      }

      // copying new data to old one
      _items = newItems;

      // storing new dimensions
      this->_sizeX = sizeX;
      this->_sizeY = sizeY;
      this->_sizeZ = sizeZ;
      this->_sizeT = sizeT;
      this->_sizeXY = sizeXY;
      this->_sizeXYZ = sizeXYZ;
      this->_sizeXYZT = sizeXYZT;

    }

    updateLineAccessors();

    // updating header
    this->_header[ "sizeX" ] = this->_sizeX;
    this->_header[ "sizeY" ] = this->_sizeY;
    this->_header[ "sizeZ" ] = this->_sizeZ;
    this->_header[ "sizeT" ] = this->_sizeT;

  }
  GKG_CATCH( "template < class T > void gkg::Volume< T >::reallocate("
             " int sizeX, int sizeY, int sizeZ, int sizeT, "
             "bool copyData )" );

}


template < class T >
inline
void gkg::Volume< T >::reallocate( const gkg::Vector3d< int32_t >& size,
                                   int32_t sizeT,
                                   bool copyData )
{

  try
  {

    this->reallocate( size.x, size.y, size.z, sizeT, copyData );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Volume< T >::reallocate( "
             "const gkg::Vector3d< int32_t >& size, int32_t sizeT, "
             "bool copyData )" );

}


template < class T >
inline
void gkg::Volume< T >::reallocate( const gkg::Vector2d< int32_t >& size,
                                   int32_t sizeZ, int32_t sizeT,
                                   bool copyData )
{

  try
  {

    this->reallocate( size.x, size.y, sizeZ, sizeT, copyData );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Volume< T >::reallocate( "
             "const gkg::Vector2d< int32_t >& size, "
             "int32_t sizeZ, int32_t sizeT, "
             "bool copyData )" );

}


template < class T >
inline
typename gkg::Volume< T >::iterator gkg::Volume< T >::begin()
{

  try
  {

    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::Volume< T >::iterator gkg::Volume< T >::begin()" );

}


template < class T >
inline
typename gkg::Volume< T >::iterator gkg::Volume< T >::end()
{

  try
  {

    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::Volume< T >::iterator gkg::Volume< T >::end()" );

}


template < class T >
inline
typename gkg::Volume< T >::const_iterator gkg::Volume< T >::begin() const
{

  try
  {

    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::Volume< T >::const_iterator "
             "gkg::Volume< T >::begin() const" );

}


template < class T >
inline
typename gkg::Volume< T >::const_iterator gkg::Volume< T >::end() const
{

  try
  {

    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::Volume< T >::const_iterator "
             "gkg::Volume< T >::end() const" );


}


template < class T >
inline
const T& gkg::Volume< T >::operator()( int32_t x, int32_t y, int32_t z, 
                                       int32_t t ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( x < 0 ) || ( x >= this->_sizeX ) || 
         ( y < 0 ) || ( y >= this->_sizeY ) || 
         ( z < 0 ) || ( z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )", x, y, z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ x +
                     y * this->_sizeX +
                     z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ z ][ y ][ x ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::Volume< T >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t ) const" );


}


template < class T >
inline
T& gkg::Volume< T >::operator()( int32_t x, int32_t y, int32_t z, int32_t t )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( x < 0 ) || ( x >= this->_sizeX ) || 
         ( y < 0 ) || ( y >= this->_sizeY ) || 
         ( z < 0 ) || ( z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )", x, y, z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ x +
                     y * this->_sizeX +
                     z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ z ][ y ][ x ];

  }
  GKG_CATCH( "template < class T > "
             "T& gkg::Volume< T >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t )" );

}


template < class T >
inline
const T& gkg::Volume< T >::operator()( const gkg::Vector2d< int32_t >& site,
                                       int32_t z, int32_t t ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( site.x < 0 ) || ( site.x >= this->_sizeX ) || 
         ( site.y < 0 ) || ( site.y >= this->_sizeY ) || 
         ( z < 0 ) || ( z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )",
               site.x, site.y, z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ site.x +
                     site.y * this->_sizeX +
                     z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ z ][ site.y ][ site.x ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::Volume< T >::operator()( "
             "const gkg::Vector2d< int32_t >& site, "
             "int32_t z, int32_t t ) const" );


}


template < class T >
inline
T& gkg::Volume< T >::operator()( const gkg::Vector2d< int32_t >& site,
                                 int32_t z, int32_t t )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( site.x < 0 ) || ( site.x >= this->_sizeX ) || 
         ( site.y < 0 ) || ( site.y >= this->_sizeY ) || 
         ( z < 0 ) || ( z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )",
               site.x, site.y, z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ site.x +
                     site.y * this->_sizeX +
                     z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ z ][ site.y ][ site.x ];

  }
  GKG_CATCH( "template < class T > "
             "T& gkg::Volume< T >::operator()( "
             "const gkg::Vector2d< int32_t >& site, "
             "int32_t z, int32_t t )" );

}


template < class T >
inline
const T& gkg::Volume< T >::operator()( const gkg::Vector3d< int32_t >& site,
                                       int32_t t ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( site.x < 0 ) || ( site.x >= this->_sizeX ) || 
         ( site.y < 0 ) || ( site.y >= this->_sizeY ) || 
         ( site.z < 0 ) || ( site.z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )",
               site.x, site.y, site.z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ site.x +
                     site.y * this->_sizeX +
                     site.z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ site.z ][ site.y ][ site.x ];

  }
  GKG_CATCH( "template < class T > "
             "const T& gkg::Volume< T >::operator()( "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t t ) const" );


}


template < class T >
inline
T& gkg::Volume< T >::operator()( const gkg::Vector3d< int32_t >& site,
                                 int32_t t )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( site.x < 0 ) || ( site.x >= this->_sizeX ) || 
         ( site.y < 0 ) || ( site.y >= this->_sizeY ) || 
         ( site.z < 0 ) || ( site.z >= this->_sizeZ ) || 
         ( t < 0 ) || ( t >= this->_sizeT ) )
    {

      char message[ 256 ];
      sprintf( message, "invalid indices ( %d, %d, %d, %d )",
               site.x, site.y, site.z, t );
      throw std::runtime_error( message );

    }

#endif

    if ( _items.isMemoryMapped() )
    {

      return _items[ site.x +
                     site.y * this->_sizeX +
                     site.z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

    }

    return _lineAccessors[ t ][ site.z ][ site.y ][ site.x ];

  }
  GKG_CATCH( "template < class T > "
             "T& gkg::Volume< T >::operator()( "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t t )" );

}


template < class T >
inline
void gkg::Volume< T >::updateLineAccessors()
{

  try
  {

    // if memory mapped, cannot use line accessors
    if ( _items.isMemoryMapped() )
    {

      return;

    }

    // clearing previously allocated line accessors
    if ( !_lineAccessors.empty() )
    {

      _lineAccessors.clear();

    }

    // allocating line accessors
    _lineAccessors.resize( this->_sizeT );
    int32_t t = 0;
    int32_t z = 0;
    int32_t y = 0;
    for ( t = 0; t < this->_sizeT; t++ )
    {

      _lineAccessors[ t ].resize( this->_sizeZ );
      for ( z = 0; z < this->_sizeZ; z++ )
      {

        _lineAccessors[ t ][ z ].resize( this->_sizeY );
        for ( y = 0; y < this->_sizeY; y++ )
        {

          _lineAccessors[ t ][ z ][ y ] =
            &_items[ y * this->_sizeX +
                     z * ( size_t )this->_sizeXY +
                     t * ( size_t )this->_sizeXYZ ];

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::Volume< T >::updateLineAccessors()" );

}


template < class T >
inline
bool gkg::Volume< T >::isMemoryMapped() const
{

  try
  {

    return _items.isMemoryMapped();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "bool gkg::Volume< T >::isMemoryMapped() const" );

}


//
// specific case for boolean which are not stored like other types
//

namespace gkg
{

/*
template <>
inline
const bool& Volume< bool >::operator()( int32_t x, int32_t y, int32_t z, 
                                        int32_t t ) const
{

  try
  {

    return _items[ x +
                   y * this->_sizeX +
                   z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "const bool& Volume< bool >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t ) const" );

}


template <>
inline
bool& Volume< bool >::operator()( int32_t x, int32_t y, int32_t z, 
                                  int32_t t )
{

  try
  {

    return _items[ x +
                   y * this->_sizeX +
                   z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "bool& Volume< bool >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t )" );

}


template <>
inline
const bool& Volume< bool >::operator()( const Vector2d< int32_t >& site,
                                        int32_t z, int32_t t ) const
{

  try
  {

    return _items[ site.x +
                   site.y * this->_sizeX +
                   z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "const bool& Volume< bool >::operator()( "
             "const Vector2d< int32_t >& site, "
             "int32_t z, int32_t t ) const" );


}


template <>
inline
bool& Volume< bool >::operator()( const Vector2d< int32_t >& site,
                                  int32_t z, int32_t t )
{

  try
  {

    return _items[ site.x +
                   site.y * this->_sizeX +
                   z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "bool& Volume< bool >::operator()( "
             "const Vector2d< int32_t >& site, "
             "int32_t z, int32_t t )" );


}


template <>
inline
const bool& Volume< bool >::operator()( const Vector3d< int32_t >& site,
                                        int32_t t ) const
{

  try
  {

    return _items[ site.x +
                   site.y * this->_sizeX +
                   site.z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "const bool& Volume< bool >::operator()( "
             "const Vector3d< int32_t >& site, "
             "int32_t t ) const" );


}


template <>
inline
bool& Volume< bool >::operator()( const Vector3d< int32_t >& site,
                                  int32_t t )
{

  try
  {

    return _items[ site.x +
                   site.y * this->_sizeX +
                   site.z * ( size_t )this->_sizeXY +
                   t * ( size_t )this->_sizeXYZ ];

  }
  GKG_CATCH( "template <> "
             "bool& Volume< bool >::operator()( "
             "const Vector3d< int32_t >& site, "
             "int32_t t )" );


}
*/

template <>
inline
void Volume< bool >::updateLineAccessors()
{
}


}

#endif


