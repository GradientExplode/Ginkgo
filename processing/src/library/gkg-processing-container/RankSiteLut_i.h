#ifndef _gkg_processing_container_RankSiteLut_i_h_
#define _gkg_processing_container_RankSiteLut_i_h_


#include <gkg-processing-container/RankSiteLut.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-core-exception/Exception.h>


//
// class RankSite
//

template < class R, class S, class RankCompare >
inline
gkg::RankSite< R, S, RankCompare >::RankSite()
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSite< R, S, RankCompare >::RankSite( const R& theRank,
                                              const Vector3d< S >& theSite )
                                   : rank( theRank ),
                                     site( theSite )
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSite< R, S, RankCompare >::RankSite(
                               const gkg::RankSite< R, S, RankCompare >& other )
                                   : rank( other.rank ),
                                     site( other.site )
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSite< R, S, RankCompare >&
gkg::RankSite< R, S, RankCompare >::operator=(
                               const gkg::RankSite< R, S, RankCompare >& other )
{

  rank = other.rank;
  site = other.site;
  return *this;

}


namespace std
{


template < class R, class S, class RankCompare >
ostream& 
operator<<( ostream& os, const gkg::RankSite< R, S, RankCompare >& thing )
{

  os << "( " << thing.rank << ", " << thing.site << " )";
  return os;

}


}


//
// class RankSiteLut
//


template < class R, class S, class RankCompare >
inline
gkg::RankSiteLut< R, S, RankCompare >::RankSiteLut()
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSiteLut< R, S, RankCompare >::RankSiteLut(
                              const gkg::SiteMap< R, S, RankCompare >& siteMap )
{

  try
  {

    int32_t index = 0;

    typename gkg::SiteMap< R, S, RankCompare >::const_iterator
      r = siteMap.begin(),
      re = siteMap.end();

    while ( r != re )
    {

      typename std::list< Vector3d< S > >::const_iterator
        s = r->second.begin(),
        se = r->second.end();

      while ( s != se )
      {

        gkg::RankSite< R, S, RankCompare > rankSite( r->first, *s );
        _indices[ rankSite ] = index;
        _rankSites[ index ] = rankSite;
        ++ index;

        ++ s;

      }

      ++ r;

    }

  }
  GKG_CATCH( "template < class R, class S, class RankCompare > "
             "gkg::RankSiteLut< R, S, RankCompare >::RankSiteLut( "
             "const gkg::SiteMap< R, S, RankCompare >& siteMap )" );

}


template < class R, class S, class RankCompare >
inline
gkg::RankSiteLut< R, S, RankCompare >::RankSiteLut(
                            const gkg::RankSiteLut< R, S, RankCompare >& other )
                                      : _indices( other._indices ),
                                        _rankSites( other._rankSites )
                                        
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSiteLut< R, S, RankCompare >::~RankSiteLut()
{
}


template < class R, class S, class RankCompare >
inline
gkg::RankSiteLut< R, S, RankCompare >&
gkg::RankSiteLut< R, S, RankCompare >::operator=(
                            const gkg::RankSiteLut< R, S, RankCompare >& other )
{

  try
  {

    _indices = other._indices;
    _rankSites = other._rankSites;

    return *this;

  }
  GKG_CATCH( "template < class R, class S, class RankCompare > "
             "gkg::RankSiteLut< R, S, RankCompare >& "
             "gkg::RankSiteLut< R, S, RankCompare >::operator=( "
             "const gkg::RankSiteLut< R, S, RankCompare >& other )" );

}


template < class R, class S, class RankCompare >
inline
int32_t gkg::RankSiteLut< R, S, RankCompare >::getCount() const
{

  return ( int32_t )_indices.size();

}


template < class R, class S, class RankCompare >
inline
int32_t gkg::RankSiteLut< R, S, RankCompare >::getIndex(
                                          const R& rank,
                                          const gkg::Vector3d< S >& site ) const
{

  try
  {

    return getIndex( gkg::RankSite< R, S, RankCompare >( rank, site ) );

  }
  GKG_CATCH( "template < class R, class S, class RankCompare > "
             "int32_t gkg::RankSiteLut< R, S, RankCompare >::getIndex( "
             "const R& rank, "
             "const gkg::Vector3d< S >& site ) const" );

}


template < class R, class S, class RankCompare >
inline
int32_t gkg::RankSiteLut< R, S, RankCompare >::getIndex(
                      const gkg::RankSite< R, S, RankCompare >& rankSite ) const
{

  try
  {

    typename std::map< RankSite< R, S, RankCompare >,
                       int32_t,
                       RankSiteCompare< R, S, RankCompare > >::const_iterator
      i = _indices.find( rankSite );
    if ( i == _indices.end() )
    {

      throw std::runtime_error( "no (rank,site) found in the index lut" );

    }
    else
    {

      return i->second;

    }

  }
  GKG_CATCH( "template < class R, class S, class RankCompare > "
             "int32_t gkg::RankSiteLut< R, S, RankCompare >::getIndex( "
             "const gkg::RankSite< R, S, RankCompare >& rankSite ) const" );

}


template < class R, class S, class RankCompare >
inline
const gkg::RankSite< R, S, RankCompare >&
gkg::RankSiteLut< R, S, RankCompare >::getRankSite( int32_t index ) const
{

  try
  {

    typename std::map< int32_t, RankSite< R, S, RankCompare > >::const_iterator
      r = _rankSites.find( index );
    if ( r == _rankSites.end() )
    {

      throw std::runtime_error( "no index found in the rank/site lut" );

    }
    else
    {

      return r->second;

    }

  }
  GKG_CATCH( "template < class R, class S, class RankCompare > "
             "const gkg::RankSite< R, S, RankCompare >& "
             "gkg::RankSiteLut< R, S, RankCompare >::getRankSite( "
             "int32_t index ) const" );

}


#endif
