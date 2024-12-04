#ifndef _gkg_processing_container_NewVolume_i_h_
#define _gkg_processing_container_NewVolume_i_h_


#include "NewVolume.h"
#include <gkg-processing-container/VolumeProxy_i.h>
#include "NewAutoMemoryMappedVector_i.h"
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <cstdio>


template < class T >
inline
gkg::NewVolume< T >::NewVolume( int32_t sizeX, int32_t sizeY, int32_t sizeZ, 
                          int32_t sizeT )
                 : gkg::VolumeProxy< T >( sizeX, sizeY, sizeZ, sizeT )
{

  try
  {

    allocate();
    std::cout << "----------------------------------->end of volume constructor" << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "gkg::NewVolume< T >::NewVolume( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::NewVolume< T >::NewVolume( const gkg::Vector3d< int32_t >& size,
                          int32_t sizeT )
                 : gkg::VolumeProxy< T >( size, sizeT )
{

  try
  {

    allocate();

  }
  GKG_CATCH( "template < class T > "
             "gkg::NewVolume< T >::NewVolume( "
             "const gkg::Vector3d< int32_t >& size, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::NewVolume< T >::NewVolume( const gkg::Vector2d< int32_t >& size,
                          int32_t sizeZ, int32_t sizeT )
                 : gkg::VolumeProxy< T >( size, sizeZ, sizeT )
{

  try
  {

    allocate();

  }
  GKG_CATCH( "template < class T > "
             "gkg::NewVolume< T >::NewVolume( "
             "const gkg::Vector3d< int32_t >& size, "
             "int32_t sizeZ, int32_t sizeT )" );

}

                   
template < class T >
inline
gkg::NewVolume< T >::NewVolume( const gkg::NewVolume< T >& other )
                 : gkg::VolumeProxy< T >( other ),
                   _items( other._items )
{

  try
  {

    updateLineAccessors();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::NewVolume< T >::NewVolume( const gkg::NewVolume< T >& other )" );

}


template < class T >
inline
gkg::NewVolume< T >::~NewVolume()
{
}


template < class T >
inline
gkg::NewVolume< T >& gkg::NewVolume< T >::operator=(
                                              const gkg::NewVolume< T >& other )
{

  try
  {

    this->gkg::VolumeProxy< T >::operator=( other );
    _items = other._items;
    updateLineAccessors();

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::NewVolume< T >& gkg::NewVolume< T >::operator=( "
             "const gkg::NewVolume< T >& other )" );

}


template < class T >
inline
void gkg::NewVolume< T >::fill( const T& value, bool verbose )
{

  try
  {

    typename gkg::NewAutoMemoryMappedVector< T >::iterator
      i = _items.begin(),
      ie =_items.end();
    int32_t count = 0;
    while ( i != ie )
    {

      if ( verbose )
      {

        std::cout << count << "/" <<  this->_sizeXYZT << " "
                  << std::flush;

      }
      *i = value;
      if ( verbose )
      {

        std::cout << "avant i ++ " << std::flush;

      }
      i ++;
      if ( verbose )
      {

        std::cout << "apres i ++ " << std::flush;

      }
      count ++;

    }
    std::cout << std::endl;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NewVolume< T >::fill( const T& value )" );

}


template < class T >
inline
void gkg::NewVolume< T >::allocate()
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
    _items = gkg::NewAutoMemoryMappedVector< T >( ( size_t )this->_sizeXYZT );
    updateLineAccessors();

  }
  GKG_CATCH( "template < class T > void gkg::NewVolume< T >::allocate()" );

}


template < class T >
inline
void gkg::NewVolume< T >::reallocate( int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                                   int32_t sizeT )
{

  try
  {

    if ( ( sizeX != this->_sizeX ) ||
         ( sizeY != this->_sizeY ) ||
         ( sizeZ != this->_sizeZ ) ||
         ( sizeT != this->_sizeT ) )
    {
       
      // allocating a new data
      gkg::NewAutoMemoryMappedVector< T >
        newItems( ( typename gkg::NewAutoMemoryMappedVector< T >::size_type )( 
                                                                   sizeX * 
                                                                   sizeY * 
                                                                   sizeZ * 
                                                                   sizeT ) );

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
  GKG_CATCH( "template < class T > void gkg::NewVolume< T >::reallocate("
             " int sizeX, int sizeY, int sizeZ, int sizeT )" );

}


template < class T >
inline
void gkg::NewVolume< T >::reallocate( const gkg::Vector3d< int32_t >& size,
                                   int32_t sizeT )
{

  try
  {

    this->reallocate( size.x, size.y, size.z, sizeT );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::NewVolume< T >::reallocate( "
             "const gkg::Vector3d< int32_t >& size, int32_t sizeT )" );

}


template < class T >
inline
void gkg::NewVolume< T >::reallocate( const gkg::Vector2d< int32_t >& size,
                                   int32_t sizeZ, int32_t sizeT )
{

  try
  {

    this->reallocate( size.x, size.y, sizeZ, sizeT );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::NewVolume< T >::reallocate( "
             "const gkg::Vector2d< int32_t >& size, "
             "int32_t sizeZ, int32_t sizeT )" );

}


template < class T >
inline
typename gkg::NewVolume< T >::iterator gkg::NewVolume< T >::begin()
{

  try
  {

    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::NewVolume< T >::iterator gkg::NewVolume< T >::begin()" );

}


template < class T >
inline
typename gkg::NewVolume< T >::iterator gkg::NewVolume< T >::end()
{

  try
  {

    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::NewVolume< T >::iterator gkg::NewVolume< T >::end()" );

}


template < class T >
inline
typename gkg::NewVolume< T >::const_iterator gkg::NewVolume< T >::begin() const
{

  try
  {

    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::NewVolume< T >::const_iterator "
             "gkg::NewVolume< T >::begin() const" );

}


template < class T >
inline
typename gkg::NewVolume< T >::const_iterator gkg::NewVolume< T >::end() const
{

  try
  {

    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "typename gkg::NewVolume< T >::const_iterator "
             "gkg::NewVolume< T >::end() const" );


}


template < class T >
inline
const T& gkg::NewVolume< T >::operator()( int32_t x, int32_t y, int32_t z, 
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
             "const T& gkg::NewVolume< T >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t ) const" );


}


template < class T >
inline
T& gkg::NewVolume< T >::operator()( int32_t x, int32_t y, int32_t z, int32_t t )
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
             "T& gkg::NewVolume< T >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t )" );

}


template < class T >
inline
const T& gkg::NewVolume< T >::operator()( const gkg::Vector2d< int32_t >& site,
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
             "const T& gkg::NewVolume< T >::operator()( "
             "const gkg::Vector2d< int32_t >& site, "
             "int32_t z, int32_t t ) const" );


}


template < class T >
inline
T& gkg::NewVolume< T >::operator()( const gkg::Vector2d< int32_t >& site,
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
             "T& gkg::NewVolume< T >::operator()( "
             "const gkg::Vector2d< int32_t >& site, "
             "int32_t z, int32_t t )" );

}


template < class T >
inline
const T& gkg::NewVolume< T >::operator()( const gkg::Vector3d< int32_t >& site,
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
             "const T& gkg::NewVolume< T >::operator()( "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t t ) const" );


}


template < class T >
inline
T& gkg::NewVolume< T >::operator()( const gkg::Vector3d< int32_t >& site,
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
             "T& gkg::NewVolume< T >::operator()( "
             "const gkg::Vector3d< int32_t >& site, "
             "int32_t t )" );

}


template < class T >
inline
void gkg::NewVolume< T >::updateLineAccessors()
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
             "void gkg::NewVolume< T >::updateLineAccessors()" );

}


template < class T >
inline
bool gkg::NewVolume< T >::isMemoryMapped() const
{

  try
  {

    return _items.isMemoryMapped();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "bool gkg::NewVolume< T >::isMemoryMapped() const" );

}


template < class T >
inline
void gkg::NewVolume< T >::setAutoMemoryVerbose()
{

  _items.setVerbose();


}


//
// specific case for boolean which are not stored like other types
//

namespace gkg
{

/*
template <>
inline
const bool& NewVolume< bool >::operator()( int32_t x, int32_t y, int32_t z, 
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
             "const bool& NewVolume< bool >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t ) const" );

}


template <>
inline
bool& NewVolume< bool >::operator()( int32_t x, int32_t y, int32_t z, 
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
             "bool& NewVolume< bool >::operator()( "
             "int32_t x, int32_t y, int32_t z, int32_t t )" );

}


template <>
inline
const bool& NewVolume< bool >::operator()( const Vector2d< int32_t >& site,
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
             "const bool& NewVolume< bool >::operator()( "
             "const Vector2d< int32_t >& site, "
             "int32_t z, int32_t t ) const" );


}


template <>
inline
bool& NewVolume< bool >::operator()( const Vector2d< int32_t >& site,
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
             "bool& NewVolume< bool >::operator()( "
             "const Vector2d< int32_t >& site, "
             "int32_t z, int32_t t )" );


}


template <>
inline
const bool& NewVolume< bool >::operator()( const Vector3d< int32_t >& site,
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
             "const bool& NewVolume< bool >::operator()( "
             "const Vector3d< int32_t >& site, "
             "int32_t t ) const" );


}


template <>
inline
bool& NewVolume< bool >::operator()( const Vector3d< int32_t >& site,
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
             "bool& NewVolume< bool >::operator()( "
             "const Vector3d< int32_t >& site, "
             "int32_t t )" );


}
*/

template <>
inline
void NewVolume< bool >::updateLineAccessors()
{
}


}

#endif


