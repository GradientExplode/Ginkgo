#ifndef _gkg_processing_container_MeshMap_i_h_
#define _gkg_processing_container_MeshMap_i_h_


#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/RankSiteLut_i.h>
#include <gkg-processing-mesh/MeshAccumulator_i.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <list>


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMap< R, S, D, Compare >::MeshMap()
                                 : gkg::HeaderedObject()
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "vertex_map" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "polygon_map" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "MeshMap" ) );

}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMap< R, S, D, Compare >::MeshMap(
                              const gkg::SiteMap< R, S, Compare >& theVertices,
                              const gkg::PolygonMap< D >& thePolygons )
                                 : gkg::HeaderedObject(),
                                   vertices( theVertices ),
                                   polygons( thePolygons )
{

  // adding a generic syntax set
  _syntaxSet[ "__generic__" ][ "object_type" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "vertex_map" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );
  _syntaxSet[ "__generic__" ][ "polygon_map" ] = 
    gkg::Semantic( gkg::TypeOf< std::string >::getName(), true );

  // adding attributes to header
  _header.addAttribute( "object_type", std::string( "MeshMap" ) );

}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMap< R, S, D, Compare >::MeshMap(
                                 const gkg::MeshMap< R, S, D, Compare >& other )
                                 : gkg::HeaderedObject( other ),
                                   vertices( other.vertices ),
                                   polygons( other.polygons )
{
}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMap< R, S, D, Compare >::~MeshMap()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshMap< R, S, D, Compare >&
gkg::MeshMap< R, S, D, Compare >::operator=(
                                 const gkg::MeshMap< R, S, D, Compare >& other )
{

  try
  {

    this->gkg::HeaderedObject::operator=( other );
    vertices = other.vertices;
    polygons = other.polygons;

    return *this;

  }
  GKG_CATCH( "template < class R, class S, int32_t D, class Compare > "
             "inline "
             "gkg::MeshMap< R, S, D, Compare >& "
             "gkg::MeshMap< R, S, D, Compare >::operator=( "
             "const gkg::MeshMap< R, S, D, Compare >& other )" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMap< R, S, D, Compare >::add(
                                 const gkg::MeshMap< R, S, D, Compare >& other )
{

  try
  {

    // adding other vertices
    typename gkg::SiteMap< R, S, Compare >::const_iterator
      vl = other.vertices.begin(),
      vle = other.vertices.end();
    while ( vl != vle )
    {

      // collecting offset equal to site count of current mesh map
      uint32_t offset = ( uint32_t )vertices.getSiteCount( vl->first );

      // adding vertices from other to current
      vertices.addSites( vl->first, vl->second );

      // adding polygons with offset
      polygons.addPolygons( other.vertices.getIndex( vl->first ),
                            other.polygons.getPolygons(
                              other.vertices.getIndex( vl->first ) ),
                            offset );
      ++ vl;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void gkg::MeshMap< R, S, D, Compare >::add( "
             "const gkg::MeshMap< R, S, D, Compare >& other )" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshMap< R, S, D, Compare >::add(
               const gkg::MeshAccumulator< R, S, D, Compare >& meshAccumulator )
{

  try
  {

    // adding other vertices
    typename gkg::MeshAccumulator< R, S, D, Compare >::const_vertex_iterator
      vv = meshAccumulator.vbegin(),
      vve = meshAccumulator.vend();
    typename gkg::MeshAccumulator< R, S, D, Compare >::const_polygon_iterator
      pv = meshAccumulator.pbegin();
    while ( vv != vve )
    {

      // collecting offset equal to site count of current mesh map
      uint32_t offset = ( uint32_t )vertices.getSiteCount( vv->first );

      // adding vertices from other to current
      vertices.addSites( vv->first, vv->second );

      // adding polygons with offset (note: vv->first is equal to pv->first )
      polygons.addPolygons( vertices.getIndex( vv->first ),
                            pv->second,
                            offset );
      ++ vv;
      ++ pv;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void gkg::MeshMap< R, S, D, Compare >::add( "
             "const gkg::MeshAccumulator< R, S, D, Compare >& "
             "meshAccumulator )" );

}


template < class R, class S, uint32_t D, class Compare >
inline
std::list< gkg::Vector3d< S > > 
gkg::MeshMap< R, S, D, Compare >::getNormals( const R& rank ) const
{

  try
  {

    // building rank/site lookup table
    gkg::RankSiteLut< R, S, Compare > rankSiteLut( vertices );


    // allocating the vertext -> polygon normal set lookup table
    std::map< uint32_t,
              std::list<   Vector3d< S > > > vertexToPolygonNormal;  

    // allocating normal functor
    gkg::PolygonNormalFunctor< R, S, D, Compare > polygonNormalFunctor;

    // allocating the ouput list of normal(s)
    std::list< gkg::Vector3d< S > > normals;  
    
    gkg::Vector3d< S > polygonNormal;

    // filling vertex->polygon set lookup table
    uint32_t d = 0;
    typename std::list< gkg::Polygon< D > >::const_iterator
      p = this->polygons.getPolygons( vertices.getIndex( rank ) ).begin(),
      pe = this->polygons.getPolygons( vertices.getIndex( rank ) ).end();
    while ( p != pe )
    {

      polygonNormal = polygonNormalFunctor( *p, rankSiteLut );
      for ( d = 0; d < D; d++ )
      {

        vertexToPolygonNormal[ p->indices[ d ] ].push_back( polygonNormal );

      }
      ++ p;

    }

    // processing normals
    typename std::list< gkg::Vector3d< S > >::const_iterator
      v = this->vertices.getSites( rank ).begin(),
      ve = this->vertices.getSites( rank ).end();
    while ( v != ve )
    {

      typename 
      std::map< uint32_t, std::list< Vector3d< S > > >::const_iterator
        ns = vertexToPolygonNormal.find(
                               ( uint32_t )rankSiteLut.getIndex( rank, *v ) );

      if ( ns == vertexToPolygonNormal.end() )
      {

        throw std::runtime_error( "vertex index not found in polygon set LUT" );

      }

      const std::list< gkg::Vector3d< S > >& normalSet = ns->second;

      gkg::Vector3d< S > normal( ( S )0, ( S )0, ( S )0 );
      typename std::list<   Vector3d< S > >::const_iterator
        nn = normalSet.begin(),
        nne = normalSet.end();
      while ( nn != nne )
      {

        normal += *nn;
        ++ nn;

      }
      normal.normalize();

      normals.push_back( normal );

      ++ v;

    }

    return normals;

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "std::list< gkg::Vector3d< S > > "
             " gkg::MeshMap< R, S, D, Compare >::getNormals( "
             "const R& rank )" );

}


template < class R, class S, uint32_t D, class Compare >
inline
std::map< int32_t, std::set< int32_t > >
gkg::MeshMap< R, S, D, Compare >::getVertexNeighbors( 
                                        const R& rank, 
                                        const S& geodesicMaximumDistance ) const
{

  try
  {

    // building rank/site lookup table
    gkg::RankSiteLut< R, S, Compare > rankSiteLut( vertices );


    // filling 1st level neighbor set for each vertex
    std::map< int32_t, std::set< int32_t > > neighbor1stLevelIndexMap;

    uint32_t d = 0;
    typename std::list< gkg::Polygon< D > >::const_iterator
      p = this->polygons.getPolygons( vertices.getIndex( rank ) ).begin(),
      pe = this->polygons.getPolygons( vertices.getIndex( rank ) ).end();
    while ( p != pe )
    {

      for ( d = 1; d < D; d++ )
      {

        neighbor1stLevelIndexMap[ p->indices[ d - 1 ] ].insert(
                                                              p->indices[ d ] );
        neighbor1stLevelIndexMap[ p->indices[ d ] ].insert(
                                                           p->indices[ d -1 ] );

      }
      neighbor1stLevelIndexMap[ p->indices[ 0 ] ].insert(
                                                         p->indices[ D - 1U ] );
      neighbor1stLevelIndexMap[ p->indices[ D - 1U ] ].insert(
                                                              p->indices[ 0 ] );

      ++ p;

    }

    // filling neighbor set for each vertex corresponding to the maximum
    // prescribed geodesic distance
    std::map< int32_t, std::set< int32_t > > neighborIndexMap;
    std::map< int32_t, std::map< int32_t, S > > neighborDistanceMap;

    std::map< int32_t, std::set< int32_t > >::const_iterator
      i = neighbor1stLevelIndexMap.begin(),
      ie = neighbor1stLevelIndexMap.end();
    while ( i != ie )
    {

      std::set< int32_t >&
        neighborIndices = neighborIndexMap[ i->first ];
      std::map< int32_t, S >&
        neighborDistances = neighborDistanceMap[ i->first ];
      addNeighbors( i->first,
                    neighbor1stLevelIndexMap,
                    rankSiteLut,
                    neighborIndices,
                    neighborDistances,
                    geodesicMaximumDistance,
                    ( S )0.0 );

      ++ i;

    }

    return neighborIndexMap;

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "std::map< int32_t, std::set< int32_t > > "
             "gkg::MeshMap< R, S, D, Compare >::getVertexNeighbors( "
             "const R& rank, "
             "const S& geodesicMaximumDistance ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
std::map< int32_t, std::set< int32_t > >
gkg::MeshMap< R, S, D, Compare >::getVertexNthLevelNeighborhood( 
                                                    const R& rank,
                                                    int32_t level,
                                                    bool includingItself ) const
{

  try
  {

    // sanity check
    if ( level < 1 )
    {

      throw std::runtime_error( "level must be greater or equal to 1" );

    }

    // filling 1st level neighbor set for each vertex
    std::map< int32_t, std::set< int32_t > > firstLevelNeighborIndexMap;


    uint32_t d = 0;
    typename std::list< gkg::Polygon< D > >::const_iterator
      p = this->polygons.getPolygons( vertices.getIndex( rank ) ).begin(),
      pe = this->polygons.getPolygons( vertices.getIndex( rank ) ).end();
    while ( p != pe )
    {

      for ( d = 1; d < D; d++ )
      {

        firstLevelNeighborIndexMap[ p->indices[ d - 1 ] ].insert(
                                                              p->indices[ d ] );
        firstLevelNeighborIndexMap[ p->indices[ d ] ].insert(
                                                           p->indices[ d -1 ] );

      }
      firstLevelNeighborIndexMap[ p->indices[ 0 ] ].insert(
                                                         p->indices[ D - 1U ] );
      firstLevelNeighborIndexMap[ p->indices[ D - 1U ] ].insert(
                                                              p->indices[ 0 ] );

      ++ p;

    }

    if ( level == 1 )
    {

      if ( includingItself )
      {

        std::map< int32_t, std::set< int32_t > >::iterator
          nim = firstLevelNeighborIndexMap.begin(),
          nime = firstLevelNeighborIndexMap.end();

        while ( nim != nime )
        {

          const int32_t& currentVertexIndex = nim->first;
          std::set< int32_t >& currentNeighborIndices = nim->second;
          
          currentNeighborIndices.insert( currentVertexIndex );

          ++ nim;

        }

      }
      return firstLevelNeighborIndexMap;

    }

    std::map< int32_t, std::set< int32_t > >
      currentLevelNeighborIndexMap = firstLevelNeighborIndexMap;
    std::map< int32_t, std::set< int32_t > > nextLevelNeighborIndexMap;

    int32_t l = 0;
    for ( l = 2; l <= level; l++ )
    {

      nextLevelNeighborIndexMap = currentLevelNeighborIndexMap;

      std::map< int32_t, std::set< int32_t > >::iterator
        nim = currentLevelNeighborIndexMap.begin(),
        nime = currentLevelNeighborIndexMap.end();

      while ( nim != nime )
      {

        const int32_t& currentVertexIndex = nim->first;
        const std::set< int32_t >& currentNeighborIndices = nim->second;

        std::set< int32_t >::const_iterator
          ni = currentNeighborIndices.begin(),
          nie = currentNeighborIndices.end();
        while ( ni != nie )
        {

          nextLevelNeighborIndexMap[ currentVertexIndex ].merge(
                                            firstLevelNeighborIndexMap[ *ni ] );
          ++ ni;

        }
        
        // removing the current vertex index in case it is included
        if ( !includingItself )
        {

          nextLevelNeighborIndexMap[ currentVertexIndex ].erase(
                                                           currentVertexIndex );

        }

        ++ nim;

      }

      currentLevelNeighborIndexMap = nextLevelNeighborIndexMap;

    }

    return nextLevelNeighborIndexMap;

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "std::map< int32_t, std::set< int32_t > > "
             "gkg::MeshMap< R, S, D, Compare >::getVertexNthLevelNeighborhood( "
             "const R& rank, "
             "int32_t level, "
             "bool includingItself ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
std::map< int32_t, std::set< int32_t > >
gkg::MeshMap< R, S, D, Compare >::getVertexNthLevelNeighbors( 
                                                    const R& rank,
                                                    int32_t level ) const
{

  try
  {

    // sanity check
    if ( level < 1 )
    {

      throw std::runtime_error( "level must be greater or equal to 1" );

    }

    if ( level == 1 )
    {

      return this->getVertexNthLevelNeighborhood( rank, level, false );

    }

    std::map< int32_t, std::set< int32_t > >
      nthLevelMinus1Neighborhood = this->getVertexNthLevelNeighborhood(
                                                                      rank,
                                                                      level - 1,
                                                                      false );

    std::map< int32_t, std::set< int32_t > >
      nthLevelNeighborhood = this->getVertexNthLevelNeighborhood( rank,
                                                                  level,
                                                                  false );

    std::map< int32_t, std::set< int32_t > >::iterator
      n = nthLevelNeighborhood.begin(),
      ne = nthLevelNeighborhood.end();
    std::map< int32_t, std::set< int32_t > >::iterator
      nMinus1 = nthLevelMinus1Neighborhood.begin();
    while ( n != ne )
    {

      const int32_t& vertexIndex = n->first;
      std::set< int32_t >& neighborIndices = n->second;

      const std::set< int32_t >& toBeRemovedNeighborIndices = nMinus1->second;

      neighborIndices.erase( toBeRemovedNeighborIndices.begin(),
                             toBeRemovedNeighborIndices.end() );
      
      ++ nMinus1;
      ++ n;

    }
      
    return nthLevelNeighborhood;

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "std::map< int32_t, std::set< int32_t > > "
             "gkg::MeshMap< R, S, D, Compare >::getVertexNthLevelNeighbors( "
             "const R& rank, "
             "int32_t level ) const" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void
gkg::MeshMap< R, S, D, Compare >::addNeighbors(
       int32_t index,
       const std::map< int32_t, std::set< int32_t > >& neighbor1stLevelIndexMap,
       const gkg::RankSiteLut< R, S, Compare >& rankSiteLut,
       std::set< int32_t >& neighborIndices,
       std::map< int32_t, S >& neighborDistances,
       const S& geodesicMaximumDistance,
       const S& actualDistance ) const
{

  try
  {

    std::map< int32_t, std::set< int32_t > >::const_iterator
      n = neighbor1stLevelIndexMap.find( index );

    if ( n == neighbor1stLevelIndexMap.end() )
    {

      throw std::runtime_error( "key not present in the map" );

    }

    const std::set< int32_t >&
    neighbor1stLevelIndices = n->second;


    const gkg::RankSite< R, S, Compare >&
      rankSite1 = rankSiteLut.getRankSite( index );


    std::set< int32_t >::const_iterator
      i = neighbor1stLevelIndices.begin(),
      ie = neighbor1stLevelIndices.end();

    while ( i != ie )
    {

      if ( neighborIndices.find( *i ) == neighborIndices.end() )
      {

        const gkg::RankSite< R, S, Compare >&
          rankSite2 = rankSiteLut.getRankSite( *i );
        
        S distance = ( rankSite2.site - rankSite1.site ).getNorm();

        if ( ( actualDistance + distance ) <= geodesicMaximumDistance )
        {

          neighborIndices.insert( *i );
          neighborDistances[ *i ] = actualDistance + distance;
          addNeighbors( *i,
                        neighbor1stLevelIndexMap,
                        rankSiteLut,
                        neighborIndices,
                        neighborDistances,
                        geodesicMaximumDistance,
                        actualDistance + distance );

        }

      }
      ++ i;

    }

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void "
             "gkg::MeshMap< R, S, D, Compare >::addNeighbors( "
             "int32_t index, "
             "const std::map< int32_t, std::set< int32_t > >& "
             "neighbor1stLevelIndexMap, "
             "const gkg::RankSiteLut< R, S, Compare >& rankSiteLut, "
             "std::set< int32_t >& neighborIndices, "
             "std::map< int32_t, S >& neighborDistances, "
             "const S& geodesicMaximumDistance, "
             "const S& actualDistance ) const" );

}


namespace std
{


template < class R, class S, uint32_t D, class Compare >
ostream& 
operator<<( ostream& os, const gkg::MeshMap< R, S, D, Compare >& thing )
{

  os << "vertices : " << std::endl << thing.vertices << std::endl;
  os << "polygons : " << std::endl << thing.polygons << std::endl;
  return os;

}


}


#endif
