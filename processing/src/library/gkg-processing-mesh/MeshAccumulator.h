#ifndef _gkg_processing_mesh_MeshAccumulator_h_
#define _gkg_processing_mesh_MeshAccumulator_h_


#include <gkg-processing-container/MeshMap.h>
#include <map>
#include <vector>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare = std::less< R > >
class MeshAccumulator
{

  public:

    typedef typename std::map< R,
                               std::vector< Vector3d< S > >,
                               Compare >::iterator vertex_iterator;
    typedef typename std::map< R,
                               std::vector< Vector3d< S > >,
                               Compare >::const_iterator const_vertex_iterator;
    typedef typename std::map< R,
                               std::vector< Polygon< D > >,
                               Compare >::iterator polygon_iterator;
    typedef typename std::map< R,
                               std::vector< Polygon< D > >,
                               Compare >::const_iterator const_polygon_iterator;

    MeshAccumulator();
    virtual ~MeshAccumulator();

    vertex_iterator vbegin();
    vertex_iterator vend();

    const_vertex_iterator vbegin() const;
    const_vertex_iterator vend() const;

    polygon_iterator pbegin();
    polygon_iterator pend();

    const_polygon_iterator pbegin() const;
    const_polygon_iterator pend() const;

    void reserve( const R& rank,
                  int32_t vertexCount,
                  int32_t polygonCount );

    void add( const MeshMap< R, S, D, Compare >& other );


  protected:

    std::map< R, std::vector< Vector3d< S > >, Compare > _vertices;
    std::map< R, std::vector< Polygon< D > >, Compare > _polygons;

};


}


#endif
