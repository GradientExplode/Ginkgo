#ifndef _gkg_processing_container_ContinuousField_i_h_
#define _gkg_processing_container_ContinuousField_i_h_


#include <gkg-processing-container/ContinuousField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::ContinuousField< T >::ContinuousField( bool storeItems )
                          : _storeItems( storeItems )
{
}


template < class T >
inline
gkg::ContinuousField< T >::~ContinuousField()
{

  typename gkg::TextureMap< T* >::iterator
    t = _items.begin(),
    te = _items.end();
  while ( t != te )
  {

    delete t->second;
    ++ t;

  }

}


template < class T >
inline
typename gkg::ContinuousField< T >::iterator gkg::ContinuousField< T >::begin()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::iterator "
             "gkg::ContinuousField< T >::begin()" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::iterator gkg::ContinuousField< T >::end()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::iterator "
             "gkg::ContinuousField< T >::end()" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::const_iterator 
gkg::ContinuousField< T >::begin() const
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.begin();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::const_iterator "
             "gkg::ContinuousField< T >::begin() const" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::const_iterator 
gkg::ContinuousField< T >::end() const
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.end();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::const_iterator "
             "gkg::ContinuousField< T >::end() const" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::reverse_iterator 
gkg::ContinuousField< T >::rbegin()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.rbegin();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::reverse_iterator "
             "gkg::ContinuousField< T >::rbegin()" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::reverse_iterator 
gkg::ContinuousField< T >::rend()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.rend();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::reverse_iterator "
             "gkg::ContinuousField< T >::rend()" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::const_reverse_iterator 
gkg::ContinuousField< T >::rbegin() const
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.rbegin();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::const_reverse_iterator "
             "gkg::ContinuousField< T >::rbegin() const" );

}


template < class T >
inline
typename gkg::ContinuousField< T >::const_reverse_iterator 
gkg::ContinuousField< T >::rend() const
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items.rend();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::ContinuousField< T >::const_reverse_iterator "
             "gkg::ContinuousField< T >::rend() const" );

}


template < class T >
inline
bool gkg::ContinuousField< T >::areItemsStored() const
{

  return _storeItems;

}


template < class T >
inline
T* gkg::ContinuousField< T >::getItem( float x, float y, float z )
{

  try
  {

    return getItem( gkg::Vector3d< float >( x, y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::ContinuousField< T >::getItem( "
             "float x, float y = 0, float z )" );

}


template < class T >
inline
T* gkg::ContinuousField< T >::getItem( const gkg::Vector2d< float >& site,
                                       float z )
{

  try
  {

    return getItem( gkg::Vector3d< float >( site.x, site.y, z ) );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::ContinuousField< T >::getItem( "
             "const gkg::Vector2d< float >& site, int32_t z )" );

}


template < class T >
inline
T*
gkg::ContinuousField< T >::getItem( const gkg::Vector3d< float >& site )
{

  try
  {

    if ( _storeItems )
    {

      int32_t rankSiteIndex = _rankSiteLut.getIndex( 0, site );
      if ( _siteMap.hasSite( 0, site ) )
      {

        return _items.getTexture( rankSiteIndex );

      }
      T* item = newItem( site );
      _siteMap.addSite( 0, site );
      _items.addTexture( rankSiteIndex, item );
      return item;

    }
    else
    {

      return newItem( site );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "T* "
             "gkg::ContinuousField< T >::getItem( "
             "const gkg::Vector3d< float >& site )" );

}



template < class T >
inline
gkg::SiteMap< int32_t, float >& gkg::ContinuousField< T >::getSiteMap()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _siteMap;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::SiteMap< int32_t, float >& "
             "gkg::ContinuousField< T >::getSiteMap()" );

}


template < class T >
inline
gkg::TextureMap< T* >& gkg::ContinuousField< T >::getItems()
{

  try
  {

    if ( !_storeItems )
    {

      throw std::runtime_error(
                             "cannot use this method when no items is stored" );

    }
    return _items;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::TextureMap< T* >& gkg::ContinuousField< T >::getItems()" );

}


#endif
