#ifndef _gkg_processing_container_MeshMap_h_
#define _gkg_processing_container_MeshMap_h_


#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/PolygonMap.h>
#include <list>
#include <vector>
#include <set>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > class MeshAccumulator;
template < class R, class S, class Compare > class RankSiteLut;


template < class R, class S, uint32_t D, class Compare = std::less< R > >
struct MeshMap : public HeaderedObject
{

  public:

    MeshMap();
    MeshMap( const SiteMap< R, S, Compare >& theVertices,
             const PolygonMap< D >& thePolygons );
    MeshMap( const MeshMap< R, S, D, Compare >& other );
    virtual ~MeshMap();

    MeshMap< R, S, D, Compare >&
      operator=( const MeshMap< R, S, D, Compare >& other );

    void add( const MeshMap< R, S, D, Compare >& other );
    void add( const MeshAccumulator< R, S, D, Compare >& meshAccumulator );

    std::list< Vector3d< S > > getNormals( const R& rank ) const;
    std::map< int32_t, std::set< int32_t > >
      getVertexNeighbors( const R& rank,
                          const S& geodesicMaximumDistance ) const;

    std::map< int32_t, std::set< int32_t > >
      getVertexNthLevelNeighborhood( const R& rank,
                                     int32_t level,
                                     bool includingItself = false ) const;

    std::map< int32_t, std::set< int32_t > >
      getVertexNthLevelNeighbors( const R& rank,
                                  int32_t level ) const;


    SiteMap< R, S, Compare > vertices;
    PolygonMap< D > polygons;

  private:

    void addNeighbors(
       int32_t index,
       const std::map< int32_t, std::set< int32_t > >& neighbor1stLevelIndexMap,
       const RankSiteLut< R, S, Compare >& rankSiteLut,
       std::set< int32_t >& neighborIndices,
       std::map< int32_t, S >& neighborDistances,
       const S& geodesicMaximumDistance,
       const S& actualDistance ) const;


};


}


namespace std
{


template < class R, class S, int32_t D, class Compare >
ostream& 
operator<<( ostream& os, const gkg::MeshMap< R, S, D, Compare >& thing );


}


#endif
