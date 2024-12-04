#ifndef _gkg_processing_mesh_QuickConvexHull_i_h_
#define _gkg_processing_mesh_QuickConvexHull_i_h_


#include <gkg-processing-mesh/QuickConvexHull.h>
#include <gkg-processing-container/MeshMap_i.h>


template < class R, class Compare >
inline
void gkg::QuickConvexHull::addConvexHull(
                       const std::vector< Vector3d< float > >& points,
                       const R& rank,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // extracting vertices and polygons of the convex hull
    std::list< gkg::Vector3d< float > > vertices;
    std::list< gkg::Polygon< 3U > > polygons;
    getConvexHull( points, vertices, polygons );      

    // creating mesh map for convex hull
    gkg::MeshMap< R, float, 3U, Compare > convexHull;

    // adding vertices to convex hull mesh map
    convexHull.vertices.addSites( rank, vertices );

    // adding polygons to convex hull mesh map
    convexHull.polygons.addPolygons( convexHull.vertices.getIndex( rank ),
                                     polygons );

    // performing fusion between given mesh map and convex hull mesh map
    meshMap.add( convexHull );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "void gkg::QuickConvexHull::addConvexHull( "
             "const std::vector< Vector3d< float > >& points, "
             "const R& rank, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}

/*
template < class R, class Compare >
inline
void gkg::QuickConvexHull::addQuickConvexHull(
                       const std::list< Vector3d< float > >& points,
                       const R& rank,
                       gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const
{

  try
  {

    // extracting vertices and polygons of the convex hull
    std::list< gkg::Vector3d< float > > vertices;
    std::list< gkg::Polygon< 3U > > polygons;
    getConvexHull( points, vertices, polygons );      

    // creating mesh map for convex hull
    gkg::MeshMap< R, float, 3U, Compare > convexHull;

    // adding vertices to convex hull mesh map
    convexHull.vertices.addSites( rank, vertices );

    // adding polygons to convex hull mesh map
    convexHull.polygons.addPolygons( convexHull.vertices.getIndex( rank ),
                                     polygons );

    // performing fusion between given mesh map and convex hull mesh map
    meshMap.add( convexHull );

  }
  GKG_CATCH( "template < class R, class Compare > "
             "void gkg::QuickConvexHull::addConvexHull( "
             "const std::list< Vector3d< float > >& points, "
             "const R& rank, "
             "gkg::MeshMap< R, float, 3U, Compare >& meshMap ) const" );

}
*/

#endif
