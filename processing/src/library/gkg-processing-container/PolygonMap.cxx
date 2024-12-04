#include <gkg-processing-container/PolygonMap.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


template < uint32_t D >
gkg::PolygonMap< D >::PolygonMap()
                     : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "polygon_size" ] = 
    gkg::Semantic( gkg::TypeOf< uint32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "rank_count" ] = 
    gkg::Semantic( gkg::TypeOf< int32_t >::getName(), true );
  _syntaxSet[ "__generic__" ][ "item_count" ] = 
    gkg::Semantic( gkg::TypeOf< std::vector< int32_t > >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "PolygonMap" ) );
  _header.addAttribute( "polygon_size", D );
  _header.addAttribute( "rank_count", 0 );

}


template < uint32_t D >
gkg::PolygonMap< D >::PolygonMap( const gkg::PolygonMap< D >& other )
                              : gkg::HeaderedObject( other ),
                                _polygons( other._polygons )
{


}


template < uint32_t D >
gkg::PolygonMap< D >::~PolygonMap()
{
}


template < uint32_t D >
gkg::PolygonMap< D >&
gkg::PolygonMap< D >::operator=( const gkg::PolygonMap< D >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    _polygons = other._polygons;

    return *this;

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "gkg::PolygonMap< D >& "
             "gkg::PolygonMap< D >::operator=( "
             "const gkg::PolygonMap< D >& other )" );

}


template < uint32_t D >
typename gkg::PolygonMap< D >::iterator
gkg::PolygonMap< D >::begin()
{

  return _polygons.begin();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::iterator
gkg::PolygonMap< D >::end()
{

  return _polygons.end();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_iterator
gkg::PolygonMap< D >::begin() const
{

  return _polygons.begin();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_iterator
gkg::PolygonMap< D >::end() const
{

  return _polygons.end();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::reverse_iterator
gkg::PolygonMap< D >::rbegin()
{

  return _polygons.rbegin();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::reverse_iterator
gkg::PolygonMap< D >::rend()
{

  return _polygons.rend();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_reverse_iterator
gkg::PolygonMap< D >::rbegin() const
{

  return _polygons.rbegin();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_reverse_iterator
gkg::PolygonMap< D >::rend() const
{

  return _polygons.rend();

}


template < uint32_t D >
typename gkg::PolygonMap< D >::iterator
gkg::PolygonMap< D >::lower_bound( int32_t rank )
{

  return _polygons.lower_bound( rank );

}


template < uint32_t D >
typename gkg::PolygonMap< D >::iterator
gkg::PolygonMap< D >::upper_bound( int32_t rank )
{

  return _polygons.upper_bound( rank );

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_iterator 
gkg::PolygonMap< D >::lower_bound( int32_t rank ) const
{

  return _polygons.lower_bound( rank );

}


template < uint32_t D >
typename gkg::PolygonMap< D >::const_iterator
gkg::PolygonMap< D >::upper_bound( int32_t rank ) const
{

  return _polygons.upper_bound( rank );

}


template < uint32_t D >
std::pair< typename gkg::PolygonMap< D >::iterator,
           typename gkg::PolygonMap< D >::iterator > 
gkg::PolygonMap< D >::equal_range( int32_t rank )
{

  return _polygons.equal_range( rank );

}


template < uint32_t D >
std::pair< typename gkg::PolygonMap< D >::const_iterator,
           typename gkg::PolygonMap< D >::const_iterator >
gkg::PolygonMap< D >::equal_range( int32_t rank ) const
{

  return _polygons.equal_range( rank );

}


template < uint32_t D >
void gkg::PolygonMap< D >::addPolygon( int32_t rank,
                                       const gkg::Polygon< D >& polygon,
                                       uint32_t offset )
{

  try
  {

    if ( offset == 0 )
    {

      _polygons[ rank ].push_back( polygon );

    }
    else
    {

      gkg::Polygon< D > newPolygon( polygon );
      uint32_t d;
      for ( d = 0; d < D; d++ )
      {

        newPolygon.indices[ d ] += offset;

      }
      _polygons[ rank ].push_back( newPolygon );

    }

    updateHeader();

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "void gkg::PolygonMap< D >::addPolygon( const R& rank, "
             "const gkg::Polygon< D >& polygon, "
             "uint32_t offset )" );

}


template < uint32_t D >
void gkg::PolygonMap< D >::addPolygons(
                          int32_t rank,
                          const std::set< gkg::Polygon< D >,
                                          gkg::PolygonCompare< D > >& polygons,
                          uint32_t offset )
{

  try
  {

    if ( offset == 0 )
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      typename std::set< gkg::Polygon< D >,
                         gkg::PolygonCompare< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        l.push_back( *p );
        ++ p;

      }

    }
    else
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      gkg::Polygon< D > newPolygon;
      uint32_t d;
      typename std::set< gkg::Polygon< D >,
                         gkg::PolygonCompare< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        for ( d = 0; d < D; d++ )
        {

          newPolygon.indices[ d ] = ( p->indices )[ d ] + offset;

        }
        l.push_back( newPolygon );
        ++ p;

      }

    }

    updateHeader();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMap< D >::addPolygons( "
             "int32_t rank, "
             "const std::set< gkg::Polygon< D >, "
             "                gkg::PolygonCompare< D > >& polygons, "
             "uint32_t offset )" );

}


template < uint32_t D >
void gkg::PolygonMap< D >::addPolygons(
                          int32_t rank,
                          const std::list< gkg::Polygon< D > >& polygons,
                          uint32_t offset )
{

  try
  {

    if ( offset == 0 )
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      typename std::list< gkg::Polygon< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        l.push_back( *p );
        ++ p;

      }

    }
    else
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      gkg::Polygon< D > newPolygon;
      uint32_t d;
      typename std::list< gkg::Polygon< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        for ( d = 0; d < D; d++ )
        {

          newPolygon.indices[ d ] = ( p->indices )[ d ] + offset;

        }
        l.push_back( newPolygon );
        ++ p;

      }

    }

    updateHeader();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMap< D >::addPolygons( "
             "int32_t rank, "
             "const std::list< gkg::Polygon< D > >& polygons, "
             "uint32_t offset )" );

}


template < uint32_t D >
void gkg::PolygonMap< D >::addPolygons(
                          int32_t rank,
                          const std::vector< gkg::Polygon< D > >& polygons,
                          uint32_t offset )
{

  try
  {

    if ( offset == 0 )
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      typename std::vector< gkg::Polygon< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        l.push_back( *p );
        ++ p;

      }

    }
    else
    {

      std::list< gkg::Polygon< D > >& l = _polygons[ rank ];

      gkg::Polygon< D > newPolygon;
      uint32_t d;
      typename std::vector< gkg::Polygon< D > >::const_iterator
        p = polygons.begin(),
        pe = polygons.end();
      while ( p != pe )
      {

        for ( d = 0; d < D; d++ )
        {

          newPolygon.indices[ d ] = ( p->indices )[ d ] + offset;

        }
        l.push_back( newPolygon );
        ++ p;

      }

    }

    updateHeader();

  }
  GKG_CATCH( "template < uint32_t D > "
             "void gkg::PolygonMap< D >::addPolygons( "
             "int32_t rank, "
             "const std::vector< gkg::Polygon< D > >& polygons, "
             "uint32_t offset )" );

}


template < uint32_t D >
bool gkg::PolygonMap< D >::hasPolygon( int32_t rank,
                                       const gkg::Polygon< D >& polygon ) const
{

  try
  {

    typename std::map< int32_t,
                       std::list< gkg::Polygon< D > > >::const_iterator
      l =  _polygons.find( rank );

    if ( l == _polygons.end() )
    {

      return false;


    }
    else
    {

      typename std::list< gkg::Polygon< D > >::const_iterator
        p = l->second.begin(),
        pe = l->second.end();

      while ( p != pe )
      {

        if ( *p == polygon )
        {

          return true;

        }
        ++ p;

      }

    }
    return true;

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "bool gkg::PolygonMap< D >::hasPolygon( "
             "int32_t rank, "
             "const gkg::Polygon< D >& polygon ) const" );

}


template < uint32_t D >
void gkg::PolygonMap< D >::removePolygon( int32_t rank,
                                          const gkg::Polygon< D >& polygon )
{

  try
  {

    typename std::map< int32_t,
                       std::list< gkg::Polygon< D > > >::iterator
      l =  _polygons.find( rank );

    if ( l == _polygons.end() )
    {

      throw std::runtime_error( "rank not present in the polygon map" );


    }
    else
    {

      l->second.remove( polygon );
      if ( _polygons.count( rank ) == 0U )
      {

        this->removePolygons( rank );

      }
      else
      {

        updateHeader();

      }

    }

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "void gkg::PolygonMap< D >::removePolygon( int32_t rank, "
             "const gkg::Polygon< D >& polygon )" );

}


template < uint32_t D >
void gkg::PolygonMap< D >::removePolygons( int32_t rank )
{

  try
  {

    _polygons.erase( rank );

    updateHeader();

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "void "
             "gkg::PolygonMap< D >::removePolygons( const R& rank )" );

}


template < uint32_t D >
int32_t gkg::PolygonMap< D >::getRankCount() const
{

  return ( int32_t )_polygons.size();

}


template < uint32_t D >
int32_t gkg::PolygonMap< D >::getPolygonCount( int32_t rank ) const
{

  try
  {

    typename std::map< int32_t,
                      std::list< gkg::Polygon< D > > >::const_iterator
      l =  _polygons.find( rank );

    if ( l == _polygons.end() )
    {

      return 0;

    }
    return ( int32_t )l->second.size();

  }
  GKG_CATCH( "template < uint32_t D > "
             "int32_t gkg::PolygonMap< D >::getPolygonCount( "
             "int32_t rank ) const" );

}


template < uint32_t D >
const std::list< gkg::Polygon< D > >&
gkg::PolygonMap< D >::getPolygons( int32_t rank ) const
{

  try
  {

    typename std::map< int32_t,
                       std::list< gkg::Polygon< D > > >::const_iterator
      l =  _polygons.find( rank );

    if ( l == _polygons.end() )
    {

      throw std::runtime_error( "rank not present in the polygon map" );

    }
    return l->second;

  }
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "const std::set< gkg::Polygon< D >, gkg::PolygonCompare< D > >& "
             "gkg::PolygonMap< D >::getPolygons( int32_t rank ) const" );

}


template < uint32_t D >
typename gkg::PolygonMap< D >::iterator
gkg::PolygonMap< D >::find( int32_t rank )
{

  return _polygons.find( rank );

}


template < uint32_t D >
void gkg::PolygonMap< D >::updateHeader()
{

  try
  {

    _header[ "rank_count" ] = this->getRankCount();

    std::vector< int32_t > itemCounts( this->getRankCount() );

    std::vector< int32_t >::iterator ic = itemCounts.begin(),
                                     ice = itemCounts.end();
    int32_t rank = 0;
    while ( ic != ice )
    {

      *ic = ( int32_t )_polygons[ rank ].size();
      ++ rank;
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
  GKG_CATCH( "template < uint32_t D > "
             "inline "
             "void gkg::PolygonMap< D >::updateHeader()" );

}


namespace std
{

ostream& operator<<( ostream& os, const gkg::PolygonMap< 2U >& thing )
{

  gkg::PolygonMap< 2U >::const_iterator
    r = thing.begin(),
    re = thing.end();

  while ( r != re )
  {

    os << r->first << " : ";

    std::list< gkg::Polygon< 2U > >::const_iterator
      p = r->second.begin(),
      pe = r->second.end();
    while ( p != pe )
    {

      os << *p << " ";
      ++ p;

    }    
    os << std::endl;
    ++ r;

  }
  return os;

}


ostream& operator<<( ostream& os, const gkg::PolygonMap< 3U >& thing )
{

  gkg::PolygonMap< 3U >::const_iterator
    r = thing.begin(),
    re = thing.end();

  while ( r != re )
  {

    os << r->first << " : ";

    std::list< gkg::Polygon< 3U > >::const_iterator
      p = r->second.begin(),
      pe = r->second.end();
    while ( p != pe )
    {

      os << *p << " ";
      ++ p;

    }    
    os << std::endl;
    ++ r;

  }
  return os;

}


ostream& operator<<( ostream& os, const gkg::PolygonMap< 4U >& thing )
{

  gkg::PolygonMap< 4U >::const_iterator
    r = thing.begin(),
    re = thing.end();

  while ( r != re )
  {

    os << r->first << " : ";

    std::list< gkg::Polygon< 4U > >::const_iterator
      p = r->second.begin(),
      pe = r->second.end();
    while ( p != pe )
    {

      os << *p << " ";
      ++ p;

    }    
    os << std::endl;
    ++ r;

  }
  return os;

}


}

template class gkg::PolygonMap< 2U >;
template class gkg::PolygonMap< 3U >;
template class gkg::PolygonMap< 4U >;



