#ifndef _gkg_processing_container_SiteMapFactory_i_h_
#define _gkg_processing_container_SiteMapFactory_i_h_


#include <gkg-processing-container/SiteMapFactory.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>
#include <list>
#include <map>


//
// function toSiteHelper
//

template < class S >
inline
void gkg::toSiteHelper( const gkg::Vector3d< int32_t >& p,
                        const gkg::Vector3d< double >& /*resolution*/,
                        gkg::Vector3d< S >& site )
{

  site.x = ( S )p.x;
  site.y = ( S )p.y;
  site.z = ( S )p.z;

}


// specialization(s)

namespace gkg
{


template <>
inline
void toSiteHelper( const Vector3d< int32_t >& p,
                   const Vector3d< double >& resolution,
                   Vector3d< float >& site )
{

  site.x = ( float )( p.x * resolution.x );
  site.y = ( float )( p.y * resolution.y );
  site.z = ( float )( p.z * resolution.z );

}


template <>
inline
void toSiteHelper( const Vector3d< int32_t >& p,
                   const Vector3d< double >& resolution,
                   Vector3d< double >& site )
{

  site.x = ( double )( p.x * resolution.x );
  site.y = ( double )( p.y * resolution.y );
  site.z = ( double )( p.z * resolution.z );

}


}


//
// class SiteMapFactory
//

template < class L, class S, class Compare >
inline
gkg::SiteMapFactory< L, S, Compare >::SiteMapFactory()
{
}


template < class L, class S, class Compare >
inline
gkg::SiteMapFactory< L, S, Compare >::~SiteMapFactory()
{
}


template < class L, class S, class Compare >
inline
gkg::SiteMap< L, S, Compare >
gkg::SiteMapFactory< L, S, Compare >::create(
                      const gkg::Volume< L >& volume,
                      const gkg::TestFunction< L >& inBackgroundFunction ) const
{

  try
  {

    if ( volume.getSizeT() != 1 )
    {

       throw std::runtime_error( "T size is not equal to 1" );

    }

    gkg::SiteMap< L, S, Compare > siteMap;

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    if ( volume.getHeader().hasAttribute( "resolutionX" ) )
    {

      volume.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( volume.getHeader().hasAttribute( "resolutionY" ) )
    {

      volume.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( volume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      volume.getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    siteMap.getHeader().addAttribute( "resolutionX", resolution.x );
    siteMap.getHeader().addAttribute( "resolutionY", resolution.y );
    siteMap.getHeader().addAttribute( "resolutionZ", resolution.z );

    gkg::Vector3d< int32_t > p;
    gkg::Vector3d< S > site;
    std::map< L, std::list< gkg::Vector3d< S > >, Compare > sites;

    for ( p.z = 0; p.z < sizeZ; p.z++ )
    {

      for ( p.y = 0; p.y < sizeY; p.y++ )
      {

        for ( p.x = 0; p.x < sizeX; p.x++ )
        {

          L label = volume( p );
          if ( !inBackgroundFunction.getState( label ) )
          {

             gkg::toSiteHelper< S >( p, resolution, site );
             sites[ label ].push_back( site );

          }

        }

      }

    }

    typename 
    std::map< L, std::list< gkg::Vector3d< S > >, Compare >::const_iterator
      l = sites.begin(),
      le = sites.end();
    while ( l != le )
    {

      siteMap.addSites( l->first, l->second );
      ++ l;

    }

    return siteMap;

  }
  GKG_CATCH( "template < class L, class S, class Compare > "
             "inline "
             "gkg::SiteMap< L, S, Compare > "
             "gkg::SiteMapFactory< L, S, Compare >::create( "
             "const gkg::Volume< L >& volume, "
             "const gkg::TestFunction< L >& inBackgroundFunction ) const" );

}


template < class L, class S, class Compare >
inline
gkg::SiteMap< L, S, Compare >
gkg::SiteMapFactory< L, S, Compare >::create(
                            const gkg::Volume< L >& volume,
                            const gkg::TestFunction< L >& inBackgroundFunction,
                            const std::set< L, Compare >& selectedLabels ) const
{

  try
  {

    if ( volume.getSizeT() != 1 )
    {

       throw std::runtime_error( "T size is not equal to 1" );

    }

    gkg::SiteMap< L, S, Compare > siteMap;

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    if ( volume.getHeader().hasAttribute( "resolutionX" ) )
    {

      volume.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( volume.getHeader().hasAttribute( "resolutionY" ) )
    {

      volume.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( volume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      volume.getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    siteMap.getHeader().addAttribute( "resolutionX", resolution.x );
    siteMap.getHeader().addAttribute( "resolutionY", resolution.y );
    siteMap.getHeader().addAttribute( "resolutionZ", resolution.z );

    gkg::Vector3d< int32_t > p;
    gkg::Vector3d< S > site;
    std::map< L, std::list< gkg::Vector3d< S > >, Compare > sites;

    for ( p.z = 0; p.z < sizeZ; p.z++ )
    {

      for ( p.y = 0; p.y < sizeY; p.y++ )
      {

        for ( p.x = 0; p.x < sizeX; p.x++ )
        {

          L label = volume( p );
          if ( !inBackgroundFunction.getState( label ) &&
               ( selectedLabels.find( label ) != selectedLabels.end() ) )
          {

             gkg::toSiteHelper< S >( p, resolution, site );
             sites[ label ].push_back( site );

          }

        }

      }

    }

    typename 
    std::map< L, std::list< gkg::Vector3d< S > >, Compare >::const_iterator
      l = sites.begin(),
      le = sites.end();
    while ( l != le )
    {

      siteMap.addSites( l->first, l->second );
      ++ l;

    }

    return siteMap;

  }
  GKG_CATCH( "template < class L, class S, class Compare > "
             "inline "
             "gkg::SiteMap< L, S, Compare > "
             "gkg::SiteMapFactory< L, S, Compare >::create( "
             "const gkg::Volume< L >& volume, "
             "const gkg::TestFunction< L >& inBackgroundFunction, "
             "const std::set< L, Compare >& selectedLabels ) const" );

}



template < class L, class S, class Compare >
inline
gkg::SiteMap< L, S, Compare >
gkg::SiteMapFactory< L, S, Compare >::create(
                             const gkg::Volume< L >& volume,
                             const gkg::TestFunction< L >& inBackgroundFunction,
                             const L& selectedLabel ) const
{

  try
  {

    if ( volume.getSizeT() != 1 )
    {

       throw std::runtime_error( "T size is not equal to 1" );

    }

    gkg::SiteMap< L, S, Compare > siteMap;

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    if ( volume.getHeader().hasAttribute( "resolutionX" ) )
    {

      volume.getHeader().getAttribute( "resolutionX", resolution.x );

    }
    if ( volume.getHeader().hasAttribute( "resolutionY" ) )
    {

      volume.getHeader().getAttribute( "resolutionY", resolution.y );

    }
    if ( volume.getHeader().hasAttribute( "resolutionZ" ) )
    {

      volume.getHeader().getAttribute( "resolutionZ", resolution.z );

    }
    siteMap.getHeader().addAttribute( "resolutionX", resolution.x );
    siteMap.getHeader().addAttribute( "resolutionY", resolution.y );
    siteMap.getHeader().addAttribute( "resolutionZ", resolution.z );

    gkg::Vector3d< int32_t > p;
    gkg::Vector3d< S > site;
    std::list< gkg::Vector3d< S > > sites;

    for ( p.z = 0; p.z < sizeZ; p.z++ )
    {

      for ( p.y = 0; p.y < sizeY; p.y++ )
      {

        for ( p.x = 0; p.x < sizeX; p.x++ )
        {

          L label = volume( p );
          if ( !inBackgroundFunction.getState( label ) &&
               ( label == selectedLabel ) )
          {

             gkg::toSiteHelper< S >( p, resolution, site );
             sites.push_back( site );

          }

        }

      }

    }

    siteMap.addSites( selectedLabel, sites );

    return siteMap;

  }
  GKG_CATCH( "template < class L, class S, class Compare > "
             "inline "
             "gkg::SiteMap< L, S, Compare > "
             "gkg::SiteMapFactory< L, S, Compare >::create( "
             "const gkg::Volume< L >& volume, "
             "const gkg::TestFunction< L >& inBackgroundFunction, "
             "const L& selectedLabel ) const" );

}


#endif
