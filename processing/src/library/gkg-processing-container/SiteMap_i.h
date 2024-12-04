#ifndef _gkg_processing_container_SiteMap_i_h_
#define _gkg_processing_container_SiteMap_i_h_


#include <gkg-processing-container/SiteMap.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, class Compare >
inline
gkg::SiteMap< R, S, Compare >::SiteMap()
                              : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "rank_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "rank_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "SiteMap" ) );
  _header.addAttribute( "item_type", gkg::TypeOf< S >::getName() );
  _header.addAttribute( "rank_type", gkg::TypeOf< R >::getName() );
  _header.addAttribute( "rank_count", 0 );

}


template < class R, class S, class Compare >
inline
gkg::SiteMap< R, S, Compare >::SiteMap(
                                    const gkg::SiteMap< R, S, Compare >& other )
                              : gkg::HeaderedObject( other ),
                                _sites( other._sites ),
                                _ranks( other._ranks ),
                                _rankToIndexLut( other._rankToIndexLut ),
                                _indexToRankLut( other._indexToRankLut )
{


}


template < class R, class S, class Compare >
inline
gkg::SiteMap< R, S, Compare >::~SiteMap()
{
}


template < class R, class S, class Compare >
inline
gkg::SiteMap< R, S, Compare >&
gkg::SiteMap< R, S, Compare >::operator=(
                                    const gkg::SiteMap< R, S, Compare >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _sites = other._sites;
    _ranks = other._ranks;
    _rankToIndexLut = other._rankToIndexLut;
    _indexToRankLut = other._indexToRankLut;

    return *this;

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "gkg::SiteMap< R, S, Compare >& "
             "gkg::SiteMap< R, S, Compare >::operator=( "
             "const gkg::SiteMap< R, S, Compare >& other )" );

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::iterator
gkg::SiteMap< R, S, Compare >::begin()
{

  return _sites.begin();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::iterator
gkg::SiteMap< R, S, Compare >::end()
{

  return _sites.end();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_iterator
gkg::SiteMap< R, S, Compare >::begin() const
{

  return _sites.begin();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_iterator
gkg::SiteMap< R, S, Compare >::end() const
{

  return _sites.end();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::reverse_iterator
gkg::SiteMap< R, S, Compare >::rbegin()
{

  return _sites.rbegin();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::reverse_iterator
gkg::SiteMap< R, S, Compare >::rend()
{

  return _sites.rend();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_reverse_iterator
gkg::SiteMap< R, S, Compare >::rbegin() const
{

  return _sites.rbegin();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_reverse_iterator
gkg::SiteMap< R, S, Compare >::rend() const
{

  return _sites.rend();

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::iterator
gkg::SiteMap< R, S, Compare >::lower_bound( const R& rank )
{

  return _sites.lower_bound( rank );

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::iterator
gkg::SiteMap< R, S, Compare >::upper_bound( const R& rank )
{

  return _sites.upper_bound( rank );

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_iterator 
gkg::SiteMap< R, S, Compare >::lower_bound( const R& rank ) const
{

  return _sites.lower_bound( rank );

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::const_iterator
gkg::SiteMap< R, S, Compare >::upper_bound( const R& rank ) const
{

  return _sites.upper_bound( rank );

}


template < class R, class S, class Compare >
inline
std::pair< typename gkg::SiteMap< R, S, Compare >::iterator,
           typename gkg::SiteMap< R, S, Compare >::iterator > 
gkg::SiteMap< R, S, Compare >::equal_range( const R& rank )
{

  return _sites.equal_range( rank );

}


template < class R, class S, class Compare >
inline
std::pair< typename gkg::SiteMap< R, S, Compare >::const_iterator,
           typename gkg::SiteMap< R, S, Compare >::const_iterator >
gkg::SiteMap< R, S, Compare >::equal_range( const R& rank ) const
{

  return _sites.equal_range( rank );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::addSite( const R& rank,
                                             const gkg::Vector3d< S >& site )
{

  try
  {

    if ( _sites.find( rank ) == _sites.end() )
    {

      _ranks.insert( rank );

    }
    _sites[ rank ].push_back( site );

    updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::addSite( const R& rank, "
             "const gkg::Vector3d< S >& site )" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::addSites(
                            const R& rank,
                            const std::set< gkg::Vector3d< S >,
                                            gkg::Vector3dCompare< S > >& sites )
{

  try
  {

    if ( _sites.find( rank ) == _sites.end() )
    {

      _ranks.insert( rank );

    }

    std::list< gkg::Vector3d< S > >& l = _sites[ rank ];

    typename std::set< gkg::Vector3d< S >,
                       gkg::Vector3dCompare< S > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      l.push_back( *s );
      ++ s;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::addSites( "
             "const R& rank, "
             "const std::set< gkg::Vector3d< S >, "
             "                gkg::Vector3dCompare< S > >& sites )" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::addSites(
                            const R& rank,
                            const std::list< gkg::Vector3d< S > >& sites )
{

  try
  {

    if ( _sites.find( rank ) == _sites.end() )
    {

      _ranks.insert( rank );

    }

    std::list< gkg::Vector3d< S > >& l = _sites[ rank ];

    typename std::list< gkg::Vector3d< S > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      l.push_back( *s );
      ++ s;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::addSites( "
             "const R& rank, "
             "const std::list< gkg::Vector3d< S > >& sites )" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::addSites(
                            const R& rank,
                            const std::vector< gkg::Vector3d< S > >& sites )
{

  try
  {

    if ( _sites.find( rank ) == _sites.end() )
    {

      _ranks.insert( rank );

    }

    std::list< gkg::Vector3d< S > >& l = _sites[ rank ];

    typename std::vector< gkg::Vector3d< S > >::const_iterator
      s = sites.begin(),
      se = sites.end();
    while ( s != se )
    {

      l.push_back( *s );
      ++ s;

    }

    updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::addSites( "
             "const R& rank, "
             "const std::vector< gkg::Vector3d< S > >& sites )" );

}


template < class R, class S, class Compare >
inline
bool gkg::SiteMap< R, S, Compare >::hasSite(
                                          const R& rank,
                                          const gkg::Vector3d< S >& site ) const
{

  try
  {

    typename std::map< R,
                       std::list< gkg::Vector3d< S > >,
                       Compare >::const_iterator
      l =  _sites.find( rank );

    if ( l == _sites.end() )
    {

      return false;


    }
    else
    {

      typename std::list< gkg::Vector3d< S > >::const_iterator
        s = l->second.begin(),
        se = l->second.end();
      while ( s != se )
      {

        if ( *s == site )
        {

          return true;

        }
        ++ s;

      }

    }
    return false;

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "bool gkg::SiteMap< R, S, Compare >::hasSite( "
             "const R& rank, "
             "const gkg::Vector3d< S >& site ) const" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::removeSite( const R& rank,
                                                const gkg::Vector3d< S >& site )
{

  try
  {

    typename std::map< R,
                      std::list< gkg::Vector3d< S > >,
                      Compare >::iterator
      l =  _sites.find( rank );

    if ( l == _sites.end() )
    {

      throw std::runtime_error( "rank not present in the site map" );


    }
    else
    {

      l->second.remove( site );
      if ( _sites.count( rank ) == 0U )
      {

        this->removeSites( rank );

      }
      else
      {

        updateHeader();

      }

    }

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::removeSite( const R& rank, "
             "const gkg::Vector3d< S >& site )" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::removeSites( const R& rank )
{

  try
  {

    _sites.erase( rank );
    _ranks.erase( rank );

    updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void "
             "gkg::SiteMap< R, S, Compare >::removeSites( const R& rank )" );

}


template < class R, class S, class Compare >
inline
int32_t
gkg::SiteMap< R, S, Compare >::getRankCount() const
{

  return ( int32_t )_ranks.size();

}


template < class R, class S, class Compare >
inline
const std::set< R, Compare >&
gkg::SiteMap< R, S, Compare >::getRanks() const
{

  return _ranks;

}


template < class R, class S, class Compare >
inline
int32_t gkg::SiteMap< R, S, Compare >::getSiteCount( const R& rank ) const
{

  try
  {

    typename std::map< R,
                      std::list< gkg::Vector3d< S > >,
                      Compare >::const_iterator
      l =  _sites.find( rank );

    if ( l == _sites.end() )
    {

      return 0;

    }
    return ( int32_t )l->second.size();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "int32_t "
             "gkg::SiteMap< R, S, Compare >::getSiteCount( "
             "const R& rank ) const" );

}


template < class R, class S, class Compare >
inline
const std::list< gkg::Vector3d< S > >&
gkg::SiteMap< R, S, Compare >::getSites( const R& rank ) const
{

  try
  {

    typename std::map< R,
                      std::list< gkg::Vector3d< S > >,
                      Compare >::const_iterator
      l =  _sites.find( rank );

    if ( l == _sites.end() )
    {

      throw std::runtime_error( "rank not present in the site map" );

    }
    return l->second;

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "const std::list< gkg::Vector3d< S > >& "
             "gkg::SiteMap< R, S, Compare >::getSites( const R& rank ) const" );

}


template < class R, class S, class Compare >
inline
typename gkg::SiteMap< R, S, Compare >::iterator
gkg::SiteMap< R, S, Compare >::find( const R& rank )
{

  return _sites.find( rank );

}

template < class R, class S, class Compare >
inline
int32_t gkg::SiteMap< R, S, Compare >::getIndex( const R& rank ) const
{

  try
  {

    typename std::map< R, int32_t, Compare >::const_iterator
      rti = _rankToIndexLut.find( rank );
    if ( rti != _rankToIndexLut.end() )
    {

       return rti->second;

    }
    else
    {

      throw std::runtime_error( "rank not present in site map" );

    }

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "int32_t gkg::SiteMap< R, S, Compare >::getIndex( "
             "const R& rank ) const" );

}


template < class R, class S, class Compare >
inline
const R& gkg::SiteMap< R, S, Compare >::getRank( int32_t index ) const
{

  try
  {

    typename std::map< int32_t, R >::const_iterator
      itr = _indexToRankLut.find( index );
    if ( itr != _indexToRankLut.end() )
    {

       return itr->second;

    }
    else
    {

      throw std::runtime_error( "index not available in site map" );

    }

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "const R& gkg::SiteMap< R, S, Compare >::getRank( "
             "int32_t index ) const" );

}


template < class R, class S, class Compare >
inline
const std::map< R, int32_t, Compare >& 
gkg::SiteMap< R, S, Compare >::getRankToIndexLut() const
{

  return _rankToIndexLut;

}


template < class R, class S, class Compare >
inline
const std::map< int32_t, R >& 
gkg::SiteMap< R, S, Compare >::getIndexToRankLut() const
{

  return _indexToRankLut;

}


template < class R, class S, class Compare >
inline
gkg::BoundingBox< S >
gkg::SiteMap< R, S, Compare >::getBoundingBox( const R& rank ) const
{

  try
  {

    const std::list< gkg::Vector3d< S > >& sites = this->getSites( rank );

    gkg::Vector3d< S > lowerBound;
    gkg::Vector3d< S > upperBound;

    typename std::list< gkg::Vector3d< S > >::const_iterator
      s = sites.begin(),
      se = sites.end();

    lowerBound = upperBound = *s;
    ++ s;

    while ( s != se )
    {

      // lower bound
      if ( s->x < lowerBound.x )
      {

        lowerBound.x = s->x;

      }
      if ( s->y < lowerBound.y )
      {

        lowerBound.y = s->y;

      }
      if ( s->z < lowerBound.z )
      {

        lowerBound.z = s->z;

      }

      // upper bound
      if ( s->x > upperBound.x )
      {

        upperBound.x = s->x;

      }
      if ( s->y > upperBound.y )
      {

        upperBound.y = s->y;

      }
      if ( s->z > upperBound.z )
      {

        upperBound.z = s->z;

      }

      ++ s;

    }

    return gkg::BoundingBox< S >( lowerBound.x, upperBound.x,
                                  lowerBound.y, upperBound.y,
                                  lowerBound.z, upperBound.z );

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "gkg::BoundingBox< S > "
             "gkg::SiteMap< R, S, Compare >::getBoundingBox( "
             "const R& rank ) const" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::clear()
{

  try
  {

    _sites.clear();
    _ranks.clear();
    _rankToIndexLut.clear();
    _indexToRankLut.clear();

    this->updateHeader();

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::clear()" );

}


template < class R, class S, class Compare >
inline
void gkg::SiteMap< R, S, Compare >::updateHeader()
{

  try
  {

    _header[ "rank_count" ] = ( int32_t )_ranks.size();

    std::vector< int32_t > itemCounts( _ranks.size() );
    _rankToIndexLut.clear();
    _indexToRankLut.clear();

    std::vector< int32_t >::iterator ic = itemCounts.begin();
    typename std::set< R, Compare >::const_iterator r = _ranks.begin(),
                                                    re = _ranks.end();
    int32_t index = 0;
    while ( r != re )
    {

      *ic = ( int32_t )_sites[ *r ].size();
      _rankToIndexLut[ *r ] = index;
      _indexToRankLut[ index ] = *r;
      ++ index;
      ++ r;
      ++ ic;

    }

    if ( _header.hasAttribute( "item_count" ) )
    {

      _header[ "item_count" ] = itemCounts;

    }
    else
    {

      _header.addAttribute( "item_count", itemCounts );

    }

  }
  GKG_CATCH( "template < class R, class S, class Compare > "
             "inline "
             "void gkg::SiteMap< R, S, Compare >::updateHeader()" );

}


namespace std
{


template < class R, class S, class Compare >
ostream& operator<<( ostream& os, const gkg::SiteMap< R, S, Compare >& thing )
{

  typename gkg::SiteMap< R, S, Compare >::const_iterator
    r = thing.begin(),
    re = thing.end();

  while ( r != re )
  {

    os << r->first << " : ";

    typename std::list< gkg::Vector3d< S > >::const_iterator
      v = r->second.begin(),
      ve = r->second.end();
    while ( v != ve )
    {

      os << *v << " ";
      ++ v;

    }    
    ++ r;
    if ( r != re )
    {

      os << std::endl;

    }

  }
  return os;

}


}

#endif
