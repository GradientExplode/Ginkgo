#ifndef _gkg_processing_container_CartesianField_i_h_
#define _gkg_processing_container_CartesianField_i_h_


#include <gkg-processing-container/CartesianField.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::CartesianField< T >::CartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ )
                         : _siteMap( siteMap ),
                           _rankSiteLut( siteMap ),
                           _offsetX( offsetX ),
                           _offsetY( offsetY ),
                           _offsetZ( offsetZ )
{

  try
  {

    allocateLut( sizeX, sizeY, sizeZ, resolution );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::CartesianField< T >::CartesianField( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, "
             "const gkg::Vector3d< double >& resolution, "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
             "int32_t offsetX, "
             "int32_t offsetY, "
             "int32_t offsetZ  )" );

}


template < class T >
inline
gkg::CartesianField< T >::CartesianField(
                               int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                               const gkg::Vector3d< double >& resolution,
                               const gkg::SiteMap< int32_t, int32_t >& siteMap,
                               const gkg::TextureMap< T >& items,
                               int32_t offsetX,
                               int32_t offsetY,
                               int32_t offsetZ )
                         : _siteMap( siteMap ),
                           _items( items ),
                           _rankSiteLut( siteMap ),
                           _offsetX( offsetX ),
                           _offsetY( offsetY ),
                           _offsetZ( offsetZ )
{

  try
  {

    allocateLut( sizeX, sizeY, sizeZ, resolution );

    // updating lut
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = _siteMap.getSites( 0 ).begin(),
      se = _siteMap.getSites( 0 ).end();
    typename gkg::TextureMap< T >::iterator t = _items.begin();

    while ( s != se )
    {

      _lut( s->x - _offsetX,
            s->y - _offsetY,
            s->z - _offsetZ ) = &t->second;
      ++ s;
      ++ t;

    }   

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::CartesianField< T >::CartesianField( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, "
             "const gkg::Vector3d< double >& resolution, "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
             "const gkg::TextureMap< T >& items, "
             "int32_t offsetX, "
             "int32_t offsetY, "
             "int32_t offsetZ )" );

}


template < class T >
inline
gkg::CartesianField< T >::CartesianField(
                                const gkg::SiteMap< int32_t, int32_t >& siteMap,
                                const gkg::TextureMap< T >& items )
                         : _siteMap( siteMap ),
                           _items( items ),
                           _rankSiteLut( siteMap )
{

  try
  {

    // assuming information is stored in the site map header
    int32_t sizeX = 1;
    int32_t sizeY = 1;
    int32_t sizeZ = 1;
    _offsetX = 0;
    _offsetY = 0;
    _offsetZ = 0;
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    if ( _siteMap.getHeader().hasAttribute( "sizeX" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "sizeX", sizeX );

      }
      catch( std::exception& )
      {

        double doubleSizeX = 0.0;
        _siteMap.getHeader().getAttribute( "sizeX", doubleSizeX );
        sizeX = ( int32_t )( doubleSizeX + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeX'" );

    }

    if ( _siteMap.getHeader().hasAttribute( "sizeY" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "sizeY", sizeY );

      }
      catch( std::exception& )
      {

        double doubleSizeY = 0.0;
        _siteMap.getHeader().getAttribute( "sizeY", doubleSizeY );
        sizeY = ( int32_t )( doubleSizeY + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeY'" );

    }

    if ( _siteMap.getHeader().hasAttribute( "sizeZ" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "sizeZ", sizeZ );

      }
      catch( std::exception& )
      {

        double doubleSizeZ = 0.0;
        _siteMap.getHeader().getAttribute( "sizeZ", doubleSizeZ );
        sizeZ = ( int32_t )( doubleSizeZ + 0.5 );

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeZ'" );

    }

    if ( _siteMap.getHeader().hasAttribute( "offsetX" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "offsetX", _offsetX );

      }
      catch( std::exception& )
      {

        double doubleOffsetX = 0.0;
        _siteMap.getHeader().getAttribute( "offsetX", doubleOffsetX );
        _offsetX = ( int32_t )( doubleOffsetX + 0.5 );

      }

    }

    if ( _siteMap.getHeader().hasAttribute( "offsetY" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "offsetY", _offsetY );

      }
      catch( std::exception& )
      {

        double doubleOffsetY = 0.0;
        _siteMap.getHeader().getAttribute( "offsetY", doubleOffsetY );
        _offsetY = ( int32_t )( doubleOffsetY + 0.5 );

      }

    }

    if ( _siteMap.getHeader().hasAttribute( "offsetZ" ) )
    {

      try
      {

        _siteMap.getHeader().getAttribute( "offsetZ", _offsetZ );

      }
      catch( std::exception& )
      {

        double doubleOffsetZ = 0.0;
        _siteMap.getHeader().getAttribute( "offsetZ", doubleOffsetZ );
        _offsetZ = ( int32_t )( doubleOffsetZ + 0.5 );

      }

    }

    if ( _siteMap.getHeader().hasAttribute( "resolutionX" ) )
    {

      _siteMap.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'resolutionX'" );

    }

    if ( _siteMap.getHeader().hasAttribute( "resolutionY" ) )
    {

      _siteMap.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'resolutionY'" );

    }

    if ( _siteMap.getHeader().hasAttribute( "resolutionZ" ) )
    {

      _siteMap.getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'resolutionZ'" );

    }


    // allocating lut
    allocateLut( sizeX, sizeY, sizeZ, resolution );


    // updating lut
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = _siteMap.getSites( 0 ).begin(),
      se = _siteMap.getSites( 0 ).end();
    typename gkg::TextureMap< T >::iterator t = _items.begin();

    while ( s != se )
    {

      _lut( s->x - _offsetX,
            s->y - _offsetY,
            s->z - _offsetZ ) = &t->second;
      ++ s;
      ++ t;

    }   


  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::CartesianField< T >::CartesianField( "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
             "const gkg::TextureMap< T >& items )" );

}


template < class T >
inline
gkg::CartesianField< T >::CartesianField( int32_t offsetX,
                                          int32_t offsetY,
                                          int32_t offsetZ )
                         : _offsetX( offsetX ),
                           _offsetY( offsetY ),
                           _offsetZ( offsetZ )
{
}


template < class T >
inline
gkg::CartesianField< T >::~CartesianField()
{
}


template < class T >
inline
void gkg::CartesianField< T >::allocate( 
                                    int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                                    const Vector3d< double >& resolution,
                                    int32_t offsetX,
                                    int32_t offsetY,
                                    int32_t offsetZ )
{

  try
  {

    _offsetX = offsetX;
    _offsetY = offsetY;
    _offsetZ = offsetZ;

    allocateLut( sizeX, sizeY, sizeZ, resolution );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::CartesianField< T >::allocate( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, "
             "const Vector3d< double >& resolution, "
             "int32_t offsetX = 0, "
             "int32_t offsetY = 0, "
             "int32_t offsetZ = 0 )" );

}


template < class T >
inline
void gkg::CartesianField< T >::setSiteMap( 
                               const gkg::SiteMap< int32_t, int32_t >& siteMap )
{

  try
  {

    _siteMap = siteMap;
    _rankSiteLut = gkg::RankSiteLut< int32_t, int32_t >( siteMap );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::CartesianField< T >::setSiteMap( "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap )" );

}


template < class T >
inline
void gkg::CartesianField< T >::setField( 
                                const gkg::SiteMap< int32_t, int32_t >& siteMap,
                                const gkg::TextureMap< T >& items )
{

  try
  {

    _siteMap = siteMap;
    _items = items;
    _rankSiteLut = gkg::RankSiteLut< int32_t, int32_t >( siteMap );

    // updating lut
    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = _siteMap.getSites( 0 ).begin(),
      se = _siteMap.getSites( 0 ).end();
    typename gkg::TextureMap< T >::iterator t = _items.begin();

    while ( s != se )
    {

      _lut( s->x - _offsetX,
            s->y - _offsetY,
            s->z - _offsetZ ) = &t->second;
      ++ s;
      ++ t;

    }   

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::CartesianField< T >::setField( "
             "const gkg::SiteMap< int32_t, int32_t >& siteMap, "
	     "const gkg::TetureMap< T >& items )" );

}


template < class T >
inline
typename gkg::CartesianField< T >::iterator gkg::CartesianField< T >::begin()
{

  return _items.begin();

}


template < class T >
inline
typename gkg::CartesianField< T >::iterator gkg::CartesianField< T >::end()
{

  return _items.end();

}


template < class T >
inline
typename gkg::CartesianField< T >::const_iterator 
gkg::CartesianField< T >::begin() const
{

  return _items.begin();

}


template < class T >
inline
typename gkg::CartesianField< T >::const_iterator 
gkg::CartesianField< T >::end() const
{

  return _items.end();

}


template < class T >
inline
typename gkg::CartesianField< T >::reverse_iterator 
gkg::CartesianField< T >::rbegin()
{

  return _items.rbegin();

}


template < class T >
inline
typename gkg::CartesianField< T >::reverse_iterator 
gkg::CartesianField< T >::rend()
{

  return _items.rend();

}


template < class T >
inline
typename gkg::CartesianField< T >::const_reverse_iterator 
gkg::CartesianField< T >::rbegin() const
{

  return _items.rbegin();

}


template < class T >
inline
typename gkg::CartesianField< T >::const_reverse_iterator 
gkg::CartesianField< T >::rend() const
{

  return _items.rend();

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getSizeX() const
{

  return _lut.getSizeX();

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getSizeY() const
{

  return _lut.getSizeY();

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getSizeZ() const
{

  return _lut.getSizeZ();

}


template < class T >
inline
gkg::Vector3d< int32_t > gkg::CartesianField< T >::getSize() const
{

  return gkg::Vector3d< int32_t >( _lut.getSizeX(),
                                   _lut.getSizeY(),
                                   _lut.getSizeZ() );

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getOffsetX() const
{

  return _offsetX;

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getOffsetY() const
{

  return _offsetY;

}


template < class T >
inline
int32_t gkg::CartesianField< T >::getOffsetZ() const
{

  return _offsetZ;

}


template < class T >
inline
gkg::Vector3d< int32_t > gkg::CartesianField< T >::getOffset() const
{

  return gkg::Vector3d< int32_t >( _offsetX,
                                   _offsetY,
                                   _offsetZ );

}


template < class T >
inline
double gkg::CartesianField< T >::getResolutionX() const
{

  try
  {

    double resolutionX = 1.0;
    _lut.getHeader().getAttribute( "resolutionX", resolutionX );
    return resolutionX;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::CartesianField< T >::getResolutionX() const" );

}


template < class T >
inline
double gkg::CartesianField< T >::getResolutionY() const
{

  try
  {

    double resolutionY = 1.0;
    _lut.getHeader().getAttribute( "resolutionY", resolutionY );
    return resolutionY;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::CartesianField< T >::getResolutionY() const" );

}


template < class T >
inline
double gkg::CartesianField< T >::getResolutionZ() const
{

  try
  {

    double resolutionZ = 1.0;
    _lut.getHeader().getAttribute( "resolutionZ", resolutionZ );
    return resolutionZ;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "double gkg::CartesianField< T >::getResolutionZ() const" );

}


template < class T >
inline
gkg::Vector3d< double > gkg::CartesianField< T >::getResolution() const
{

  try
  {

    return gkg::Vector3d< double >( this->getResolutionX(),
                                    this->getResolutionY(),
                                    this->getResolutionZ() );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::Vector3d< double > "
             "gkg::CartesianField< T >::getResolution() const" );

}


template < class T >
inline
void gkg::CartesianField< T >::setItem( const gkg::Vector3d< int32_t >& site,
                                        const T& thing )
{

  try
  {

    if ( !_siteMap.hasSite( 0, site ) )
    {

      throw std::runtime_error( "not a valid site" );

    }
    int32_t index = _rankSiteLut.getIndex( 0, site );
    T& item = _items.addTexture( index, thing );
    _lut( site.x - _offsetX,
          site.y - _offsetY,
          site.z - _offsetZ ) = &item;


  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::CartesianField< T >::setItem( "
             "const gkg::Vector3d< int32_t >& site, "
             "const T& thing )" );

}


template < class T >
inline
void gkg::CartesianField< T >::setItemFast( 
                                           const gkg::Vector3d< int32_t >& site,
                                           const T& thing )
{

  try
  {

    int32_t index = _rankSiteLut.getIndex( 0, site );
    T& item = _items.addTexture( index, thing );
    _lut( site.x - _offsetX,
          site.y - _offsetY,
          site.z - _offsetZ ) = &item;


  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::CartesianField< T >::setItemFast( "
             "const gkg::Vector3d< int32_t >& site, "
             "const T& thing )" );

}


template < class T >
inline
const T*
gkg::CartesianField< T >::getItem( int32_t x, int32_t y, int32_t z ) const
{

  try
  {

    return getItem( gkg::Vector3d< int32_t >( x, y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T* "
             "gkg::CartesianField< T >::getItem( "
             "int32_t x, int32_t y = 0, int32_t z ) const" );

}


template < class T >
inline
const T*
gkg::CartesianField< T >::getItem( const gkg::Vector2d< int32_t >& site,
                                   int32_t z ) const
{

  try
  {

    return getItem( gkg::Vector3d< int32_t >( site.x, site.y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T* "
             "gkg::CartesianField< T >::getItem( "
             "const gkg::Vector2d< int32_t >& site, int32_t z ) const" );

}


template < class T >
inline
const T*
gkg::CartesianField< T >::getItem( const gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    int32_t x = site.x - _offsetX;
    int32_t y = site.y - _offsetY;
    int32_t z = site.z - _offsetZ;

    if ( ( x >= 0 ) &&
         ( y >= 0 ) &&
         ( z >= 0 ) &&
         ( x < _lut.getSizeX() ) &&
         ( y < _lut.getSizeY() ) &&
         ( z < _lut.getSizeZ() ) )
    {

      return _lut( x, y, z );

    }

    return 0;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T* "
             "gkg::CartesianField< T >::getItem( "
             "const gkg::Vector3d< int32_t >& site ) const" );

}


template < class T >
inline
const T*
gkg::CartesianField< T >::getItemFast( int32_t x, int32_t y, int32_t z ) const
{

  try
  {

    return _lut( x - _offsetX, y - _offsetY, z - _offsetZ );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T* "
             "gkg::CartesianField< T >::getItemFast( "
             "int32_t x, int32_t y, int32_t z ) const" );

}


template < class T >
inline
const T*
gkg::CartesianField< T >::getItemFast(
                                    const gkg::Vector3d< int32_t >& site ) const
{

  try
  {

    return _lut( site.x - _offsetX, site.y - _offsetY, site.z - _offsetZ );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const T* "
             "gkg::CartesianField< T >::getItemFast( "
             "const gkg::Vector3d< int32_t >& site ) const" );

}


template < class T >
inline
T*
gkg::CartesianField< T >::getItem( int32_t x, int32_t y, int32_t z )
{

  try
  {

    return getItem( gkg::Vector3d< int32_t >( x, y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::CartesianField< T >::getItem( "
             "int32_t x, int32_t y = 0, int32_t z )" );

}


template < class T >
inline
T*
gkg::CartesianField< T >::getItem( const gkg::Vector2d< int32_t >& site,
                                   int32_t z )
{

  try
  {

    return getItem( gkg::Vector3d< int32_t >( site.x, site.y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::CartesianField< T >::getItem( "
             "const gkg::Vector2d< int32_t >& site, int32_t z )" );

}


template < class T >
inline
T*
gkg::CartesianField< T >::getItem( const gkg::Vector3d< int32_t >& site ) 
{

  try
  {

    int32_t x = site.x - _offsetX;
    int32_t y = site.y - _offsetY;
    int32_t z = site.z - _offsetZ;

    if ( ( x >= 0 ) &&
         ( y >= 0 ) &&
         ( z >= 0 ) &&
         ( x < _lut.getSizeX() ) &&
         ( y < _lut.getSizeY() ) &&
         ( z < _lut.getSizeZ() ) )
    {

      return _lut( x, y, z );

    }

    return 0;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::CartesianField< T >::getItem( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
T*
gkg::CartesianField< T >::getItemFast( int32_t x, int32_t y, int32_t z )
{

  try
  {

    return _lut( x - _offsetX, y - _offsetY, z - _offsetZ );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::CartesianField< T >::getItemFast( "
             "int32_t x, int32_t y, int32_t z )" );

}


template < class T >
inline
T*
gkg::CartesianField< T >::getItemFast( const gkg::Vector3d< int32_t >& site )
{

  try
  {

    return _lut( site.x - _offsetX, site.y - _offsetY, site.z - _offsetZ );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::CartesianField< T >::getItemFast( "
             "const gkg::Vector3d< int32_t >& site )" );

}


template < class T >
inline
gkg::SiteMap< int32_t, int32_t >& gkg::CartesianField< T >::getSiteMap()
{

  try
  {

    return _siteMap;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::SiteMap< int32_t, int32_t >& "
             "gkg::CartesianField< T >::getSiteMap()" );

}


template < class T >
inline
gkg::TextureMap< T >& gkg::CartesianField< T >::getItems()
{

  return _items;

}


template < class T >
inline
void 
gkg::CartesianField< T >::allocateLut(
                                    int32_t sizeX, int32_t sizeY, int32_t sizeZ,
                                    const gkg::Vector3d< double >& resolution )
{

  try
  {

    // adding size, resolution and offsets to site map
    _siteMap.getHeader().addAttribute( "sizeX", sizeX );
    _siteMap.getHeader().addAttribute( "sizeY", sizeY );
    _siteMap.getHeader().addAttribute( "sizeZ", sizeZ );
    _siteMap.getHeader().addAttribute( "resolutionX", resolution.x );
    _siteMap.getHeader().addAttribute( "resolutionY", resolution.y );
    _siteMap.getHeader().addAttribute( "resolutionZ", resolution.z );
    _siteMap.getHeader().addAttribute( "offsetX", _offsetX );
    _siteMap.getHeader().addAttribute( "offsetY", _offsetY );
    _siteMap.getHeader().addAttribute( "offsetZ", _offsetZ );

    // adding resolution to ODF texture map
    _items.getHeader().addAttribute( "resolutionX", resolution.x );
    _items.getHeader().addAttribute( "resolutionY", resolution.y );
    _items.getHeader().addAttribute( "resolutionZ", resolution.z );

    // allocation ODF lut
    _lut.reallocate( sizeX, sizeY, sizeZ );
    _lut.getHeader().addAttribute( "resolutionX", resolution.x );
    _lut.getHeader().addAttribute( "resolutionY", resolution.y );
    _lut.getHeader().addAttribute( "resolutionZ", resolution.z );
    _lut.fill( 0 );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void "
             "gkg::CartesianField< T >::allocateLut( "
             "int32_t sizeX, int32_t sizeY, int32_t sizeZ, "
             "const gkg::Vector3d< double >& resolution )" );

}


#endif
