#ifndef _gkg_processing_container_PolygonMap_h_
#define _gkg_processing_container_PolygonMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-container/Polygon.h>
#include <list>
#include <set>
#include <vector>
#include <map>
#include <iostream>


namespace gkg
{


template < uint32_t D >
class PolygonMap : public HeaderedObject
{

  public:

    typedef typename std::map< int32_t,
               std::list< Polygon< D > > >::iterator iterator;
    typedef typename std::map< int32_t,
               std::list< Polygon< D > > >::const_iterator const_iterator;
    typedef typename std::map< int32_t,
               std::list< Polygon< D > > >::reverse_iterator reverse_iterator;
    typedef typename std::map< int32_t,
               std::list< Polygon< D > > >::const_reverse_iterator
                                                         const_reverse_iterator;

    PolygonMap();
    PolygonMap( const PolygonMap& other );
    virtual ~PolygonMap();

    PolygonMap< D >& operator=( const PolygonMap< D >& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator lower_bound( int32_t rank );
    iterator upper_bound( int32_t rank );

    const_iterator lower_bound( int32_t rank ) const;
    const_iterator upper_bound( int32_t rank ) const;

    std::pair< iterator, iterator > equal_range( int32_t rank );
    std::pair< const_iterator,
               const_iterator > equal_range( int32_t rank ) const;

    void addPolygon( int32_t rank, const Polygon< D >& polygon,
                     uint32_t offset = 0U );
    void addPolygons( int32_t rank,
                      const std::set< Polygon< D >,
                                      PolygonCompare< D > >& polygons,
                      uint32_t offset = 0U );
    void addPolygons( int32_t rank,
                      const std::list< Polygon< D > >& polygons,
                      uint32_t offset = 0U );
    void addPolygons( int32_t rank,
                      const std::vector< Polygon< D > >& polygons,
                      uint32_t offset = 0U );
    bool hasPolygon( int32_t rank, const Polygon< D >& polygon ) const;

    void removePolygon( int32_t rank, const Polygon< D >& polygon );
    void removePolygons( int32_t rank );

    int32_t getRankCount() const;
    int32_t getPolygonCount( int32_t rank ) const;
    const std::list< Polygon< D > >& getPolygons( int32_t rank ) const;

    iterator find( int32_t rank );

  protected:

    void updateHeader();

    std::map< int32_t,
              std::list< Polygon< D > > > _polygons;

};


typedef class PolygonMap< 2U > SegmentMap;
typedef class PolygonMap< 3U > TriangleMap;
typedef class PolygonMap< 4U > QuadMap;


}


namespace std
{


ostream& operator<<( ostream& os, const gkg::PolygonMap< 2U >& thing );
ostream& operator<<( ostream& os, const gkg::PolygonMap< 3U >& thing );
ostream& operator<<( ostream& os, const gkg::PolygonMap< 4U >& thing );


}


#endif
