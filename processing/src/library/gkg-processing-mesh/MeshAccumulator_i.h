#ifndef _gkg_processing_mesh_MeshAccumulator_i_h_
#define _gkg_processing_mesh_MeshAccumulator_i_h_


#include <gkg-processing-mesh/MeshAccumulator.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshAccumulator< R, S, D, Compare >::MeshAccumulator()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
gkg::MeshAccumulator< R, S, D, Compare >::~MeshAccumulator()
{
}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::vertex_iterator
gkg::MeshAccumulator< R, S, D, Compare >::vbegin()
{

  return _vertices.begin();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::vertex_iterator
gkg::MeshAccumulator< R, S, D, Compare >::vend()
{

  return _vertices.end();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::const_vertex_iterator
gkg::MeshAccumulator< R, S, D, Compare >::vbegin() const
{

  return _vertices.begin();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::const_vertex_iterator
gkg::MeshAccumulator< R, S, D, Compare >::vend() const
{

  return _vertices.end();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::polygon_iterator
gkg::MeshAccumulator< R, S, D, Compare >::pbegin()
{

  return _polygons.begin();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::polygon_iterator
gkg::MeshAccumulator< R, S, D, Compare >::pend()
{

  return _polygons.end();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::const_polygon_iterator
gkg::MeshAccumulator< R, S, D, Compare >::pbegin() const
{

  return _polygons.begin();

}


template < class R, class S, uint32_t D, class Compare >
inline
typename gkg::MeshAccumulator< R, S, D, Compare >::const_polygon_iterator
gkg::MeshAccumulator< R, S, D, Compare >::pend() const
{

  return _polygons.end();

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshAccumulator< R, S, D, Compare >::reserve( const R& rank,
                                                        int32_t vertexCount,
                                                        int32_t polygonCount )
{

  try
  {

    _vertices[ rank ].reserve( vertexCount );
    _polygons[ rank ].reserve( polygonCount );

  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void gkg::MeshAccumulator< R, S, D, Compare >::reserve( "
             "const R& rank, "
             "int32_t vertexCount, "
             "int32_t polygonCount )" );

}


template < class R, class S, uint32_t D, class Compare >
inline
void gkg::MeshAccumulator< R, S, D, Compare >::add(
                               const gkg::MeshMap< R, S, D, Compare >& meshMap )
{

  try
  {

    // adding polygons of mesh map to accumulator with offset
    typename gkg::PolygonMap< D >::const_iterator
      lp = meshMap.polygons.begin(),
      lpe = meshMap.polygons.end();
    uint32_t d = 0;
    gkg::Polygon< D > polygon;
    while ( lp != lpe )
    {

      const R& rank = meshMap.vertices.getRank( lp->first );

      std::vector< gkg::Polygon< D > >& polygons = _polygons[ rank ];

      uint32_t offset = ( uint32_t )_vertices[ rank ].size();

      typename std::list< gkg::Polygon< D > >::const_iterator
        p = lp->second.begin(),
        pe = lp->second.end();
      while ( p != pe )
      {

        for ( d = 0; d < D; d++ )
        {

          polygon.indices[ d ] = p->indices[ d ] + offset;

        }
        polygons.push_back( polygon );
        ++ p;

      }
      ++ lp;

    }

    // adding vertices of mesh map to accumulator
    typename gkg::SiteMap< R, S, Compare >::const_iterator
      lv = meshMap.vertices.begin(),
      lve = meshMap.vertices.end();
    while ( lv != lve )
    {

      std::vector< gkg::Vector3d< S > >& vertices = _vertices[ lv->first ];

      typename std::list< gkg::Vector3d< S > >::const_iterator
        v = lv->second.begin(),
        ve = lv->second.end();
      while ( v != ve )
      {

        vertices.push_back( *v );
        ++ v;

      }
      ++ lv;

    }


  }
  GKG_CATCH( "template < class R, class S, uint32_t D, class Compare > "
             "inline "
             "void gkg::MeshAccumulator< R, S, D, Compare >::add( "
             "const gkg::MeshMap< R, S, D, Compare >& other )" );

}


#endif
