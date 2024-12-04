#include <gkg-processing-plugin-cgal/CGALPointCloudAlphaShapeMesherImplementation.h>
#include <gkg-processing-mesh/PointCloudAlphaShapeMesher.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-core-exception/Exception.h>


#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/compute_average_spacing.h>

#include <CGAL/Alpha_shape_3.h>
#include <CGAL/Alpha_shape_cell_base_3.h>
#include <CGAL/Alpha_shape_vertex_base_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Unique_hash_map.h>
#include <CGAL/Triangulation_utils_3.h>

#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/subdivision_method_3.h>

#include <CGAL/property_map.h>


#include <CGAL/Kernel_traits.h>


#include <CGAL/version_macros.h>

#include <boost/cstdint.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>


gkg::CGALPointCloudAlphaShapeMesherImplementation::
                                  CGALPointCloudAlphaShapeMesherImplementation( 
                   gkg::PointCloudAlphaShapeMesher* pointCloudAlphaShapeMesher )
                                : gkg::PointCloudAlphaShapeMesherImplementation(
                                                    pointCloudAlphaShapeMesher )
{
}


gkg::CGALPointCloudAlphaShapeMesherImplementation::
                                 ~CGALPointCloudAlphaShapeMesherImplementation()
{
}


void gkg::CGALPointCloudAlphaShapeMesherImplementation::computeAlphaShapeMesh(
                       const std::vector< gkg::Vector3d< float > >& pointCloud,
                       gkg::MeshMap< int32_t, float, 3U >& meshMap,
                       int32_t rank,
                       bool removingOutliers,
                       int32_t neighborCount,
                       float outlierDistanceScale,
                       bool simplifyingPointSet,
                       bool smoothingPointSet,
                       bool subdividingMesh,
                       int32_t subdivisionIterationCount,
                       const std::string& intermediateFileName,
                       bool verbose ) const
{

  try
  {

    typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
    typedef CGAL::Alpha_shape_vertex_base_3< Kernel > VertexBase3;
    typedef CGAL::Alpha_shape_cell_base_3< Kernel > CellBase3;
    typedef CGAL::Triangulation_data_structure_3< VertexBase3, CellBase3 >
                                                     TriangulationDataStructure;
    typedef CGAL::Delaunay_triangulation_3< Kernel,
                                            TriangulationDataStructure,
                                            CGAL::Fast_location >
                                                          DelaunayTriangulation;
    typedef CGAL::Alpha_shape_3< DelaunayTriangulation > AlphaShape3;
    typedef Kernel::Point_3 Point3;
    typedef AlphaShape3::Alpha_iterator Alpha_iterator;
    typedef AlphaShape3::Alpha_shape_vertices_iterator
                                                  Alpha_shape_vertices_iterator;
    typedef AlphaShape3::Alpha_shape_facets_iterator
                                                  Alpha_shape_facets_iterator;
    typedef AlphaShape3::Vertex_handle Vertex_handle;
    typedef AlphaShape3::Cell_handle Cell_handle;
    typedef AlphaShape3::size_type size_type;

    typedef typename CGAL::Surface_mesh< Point3 >::template 
      Property_map< CGAL::Surface_mesh< Point3 >::Vertex_index, Point3 >
       Point_map;


    ////////////////////////////////////////////////////////////////////////////
    // converting vector of gkg::Vector3d< float > to vector of CGAL Point3
    ////////////////////////////////////////////////////////////////////////////

    int32_t pointCount = ( int32_t )pointCloud.size();

    if ( verbose )
    {

      std::cout << "original point count : " << pointCount << std::endl;

    }

    if ( verbose )
    {

      std::cout << "converting Ginkgo to CGAL point cloud : "
                << std::flush;

    }
    std::vector< Point3 > cgalPoints;
    cgalPoints.reserve( pointCount );
    std::vector< gkg::Vector3d< float > >::const_iterator
      pc = pointCloud.begin(),
      pce = pointCloud.end();
    while ( pc != pce )
    {

      cgalPoints.push_back( Point3( pc->x, pc->y, pc->z ) );
      ++ pc;

    }

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // removing outliers
    ////////////////////////////////////////////////////////////////////////////

    if ( removingOutliers )
    {

      if ( verbose )
      {

        std::cout << "computing the average distance between neighbors of a "
                     "point : "
                  << std::flush;

      }

      double averageDistance =
         CGAL::compute_average_spacing< CGAL::Sequential_tag >( cgalPoints,
                                                                neighborCount );

      if ( verbose )
      {

        std::cout << averageDistance << " for " << neighborCount << " neighbors"
                  << std::endl;

      }

      if ( verbose )
      {

        std::cout << "removing outliers that goes beyond " 
                  << outlierDistanceScale << " * averageDistance : "
                  << std::flush;

      }

#if ( CGAL_VERSION_MAJOR >= 5 ) && ( CGAL_VERSION_MINOR >= 4 )

      std::vector< Point3 >::iterator
        firstRemovedPointIterator =
          CGAL::remove_outliers< CGAL::Parallel_if_available_tag >(
           cgalPoints,
            neighborCount,
            CGAL::parameters::threshold_percent( 100.0 ).threshold_distance(
                                     outlierDistanceScale * averageDistance ) );

#else

      std::vector< Point3 >::iterator
        firstRemovedPointIterator =
          CGAL::remove_outliers(
           cgalPoints,
            neighborCount,
            CGAL::parameters::threshold_percent( 100.0 ).threshold_distance(
                                     outlierDistanceScale * averageDistance ) );

#endif

      cgalPoints.erase( firstRemovedPointIterator, cgalPoints.end() );


      if ( verbose )
      {

        std::cout << "done " << std::endl;
        std::cout << "filtered point count after outlier removal : "
                  << cgalPoints.size() << std::endl;

      }

      if ( !intermediateFileName.empty() )
      {

        std::string fileName = intermediateFileName + 
                               "_pointcloud_after_outlier_removal.pointcloud";

        std::cout << "writing intermediate '"
                  << fileName
                  << "' point cloud : "
                  << std::flush;

        std::ofstream os( fileName.c_str() );
        std::vector< Point3 >::const_iterator
          p = cgalPoints.begin(),
          pe = cgalPoints.end();
        while ( p != pe )
        {

          os << p->x() << " " << p->y() << " " << p->z() << std::endl;
          ++ p;

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done " << std::endl;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // simplifying point cloud
    ////////////////////////////////////////////////////////////////////////////

    if ( simplifyingPointSet )
    {

      if ( verbose )
      {

        std::cout << "computing the average distance between neighbors of a "
                     "point : "
                  << std::flush;

      }

      double averageDistance =
         CGAL::compute_average_spacing< CGAL::Sequential_tag >( cgalPoints,
                                                                neighborCount );

      if ( verbose )
      {

        std::cout << averageDistance << " for " << neighborCount << " neighbors"
                  << std::endl;

      }


      if ( verbose )
      {

        std::cout << "simplifying point cloud keeping the points showing "
                  << "an average distance of  "
                  << outlierDistanceScale << " * averageDistance : "
                  << std::flush;

      }
      std::vector< Point3 >::iterator
        firstRemovedPointIterator = CGAL::grid_simplify_point_set(
                                       cgalPoints,
                                       outlierDistanceScale * averageDistance );

      cgalPoints.erase( firstRemovedPointIterator, cgalPoints.end() );


      if ( verbose )
      {

        std::cout << "done " << std::endl;
        std::cout << "point count after simplification : "
                  << cgalPoints.size() << std::endl;

      }

      if ( !intermediateFileName.empty() )
      {

        std::string fileName = intermediateFileName + 
                               "_pointcloud_after_simplification.pointcloud";

        std::cout << "writing intermediate '"
                  << fileName
                  << "' point cloud : "
                  << std::flush;

        std::ofstream os( fileName.c_str() );
        std::vector< Point3 >::const_iterator
          p = cgalPoints.begin(),
          pe = cgalPoints.end();
        while ( p != pe )
        {

          os << p->x() << " " << p->y() << " " << p->z() << std::endl;
          ++ p;

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done " << std::endl;

        }

      }

    }
 

    ////////////////////////////////////////////////////////////////////////////
    // smoothing point cloud
    ////////////////////////////////////////////////////////////////////////////

    if ( smoothingPointSet )
    {

      if ( verbose )
      {

        std::cout << "smoothing point cloud using the "
                  << neighborCount << "-neighborhood : "
                  << std::flush;

      }

      CGAL::jet_smooth_point_set< CGAL::Sequential_tag >( cgalPoints,
                                                          neighborCount );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

      if ( !intermediateFileName.empty() )
      {

        std::string fileName = intermediateFileName + 
                               "_pointcloud_after_smoothing.pointcloud";

        std::cout << "writing intermediate '"
                  << fileName
                  << "' point cloud : "
                  << std::flush;

        std::ofstream os( fileName.c_str() );
        std::vector< Point3 >::const_iterator
          p = cgalPoints.begin(),
          pe = cgalPoints.end();
        while ( p != pe )
        {

          os << p->x() << " " << p->y() << " " << p->z() << std::endl;
          ++ p;

        }
        os.close();

        if ( verbose )
        {

          std::cout << "done " << std::endl;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating Delaunay triangulation
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating Delaunay triangulation : "
                << std::flush;

    }

    DelaunayTriangulation delaunayTriangulation;

    std::vector< Point3 >::const_iterator
      p = cgalPoints.begin(),
      pe = cgalPoints.end();
    while ( p != pe )
    {

      delaunayTriangulation.insert( *p );
      ++ p;

    }

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing alpha shape
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing alpha shape n regularized mode : "
                << std::flush;

    }

    AlphaShape3 alphaShape3( delaunayTriangulation );

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    AlphaShape3::NT alphaSolid = alphaShape3.find_alpha_solid();
    if ( verbose )
    {

      std::cout << "minimum alpha value to get a solid through data points : "
                << alphaSolid << std::endl;

    }

    Alpha_iterator
      optimalAlphaShape3Iterator = alphaShape3.find_optimal_alpha( 1 );
    alphaShape3.set_alpha( *optimalAlphaShape3Iterator );
    if ( verbose )
    {

      std::cout << "optimal alpha value to get one connected component : "
                <<  *optimalAlphaShape3Iterator    << std::endl;

    }



    ////////////////////////////////////////////////////////////////////////////
    // collecting vertex and polygon lists from alpha shape
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "collecting vertex and polygon lists from alpha shape : "
                << std::flush;

    }

    CGAL::Unique_hash_map< Vertex_handle, size_type > V;
    size_type vertexCount = 0;

    std::list< gkg::Vector3d< float > > vertexList;
    Alpha_shape_vertices_iterator
      v = alphaShape3.alpha_shape_vertices_begin(),
      ve = alphaShape3.alpha_shape_vertices_end();
    while ( v != ve )
    {

      const Point3& point3 = ( *v )->point();

      V[ *v ] = vertexCount++;
      vertexList.push_back( gkg::Vector3d< float>( point3.x(),
                                                   point3.y(),
                                                   point3.z() ) );
      ++ v;

    }

    std::list< gkg::Polygon< 3U > > polygonList;
    Cell_handle c;
    int32_t i = 0;
    Alpha_shape_facets_iterator 
     f = alphaShape3.alpha_shape_facets_begin(),
     fe = alphaShape3.alpha_shape_facets_end();
    while ( f != fe )
    {

      c = f->first;
      i = f->second;

      // the following ensures that regular facets are output in ccw order
      if ( ( alphaShape3.classify( *f ) == AlphaShape3::REGULAR ) &&
           ( alphaShape3.classify(c) == AlphaShape3::INTERIOR ) )
      {

        c = c->neighbor( i );
        i = c->index( f->first );

      }
      int32_t i0 = CGAL::Triangulation_utils_3::vertex_triple_index( i, 0 );
      int32_t i1 = CGAL::Triangulation_utils_3::vertex_triple_index( i, 1 );
      int32_t i2 = CGAL::Triangulation_utils_3::vertex_triple_index( i, 2 );

      polygonList.push_back( gkg::Polygon< 3U >( V[ c->vertex( i0 ) ],
                                                 V[ c->vertex( i1 ) ],
                                                 V[ c->vertex( i2 ) ] ) );
      ++ f;

    }

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating surface mesh from vertex and polygon list(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating surface mesh from vertex and polygon list(s) : "
                << std::flush;

    }

    CGAL::Surface_mesh< Point3 > surfaceMesh;

    std::list< gkg::Vector3d< float > >::const_iterator
      vv = vertexList.begin(),
      vve = vertexList.end();
    while ( vv != vve )
    {

      surfaceMesh.add_vertex( Point3( vv->x, vv->y, vv->z ) );
      ++ vv;

    }

    std::list< gkg::Polygon< 3U > >::const_iterator
      pp = polygonList.begin(),
      ppe = polygonList.end();
    while ( pp != ppe )
    {

      surfaceMesh.add_face(
           CGAL::Surface_mesh< Point3 >::Vertex_index( pp->indices[ 0 ] ),
           CGAL::Surface_mesh< Point3 >::Vertex_index( pp->indices[ 1 ] ),
           CGAL::Surface_mesh< Point3 >::Vertex_index( pp->indices[ 2 ] ) );
      ++ pp;

    }

    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // subdividing mesh to get smaller surface elements
    ////////////////////////////////////////////////////////////////////////////

    if ( subdividingMesh )
    {

      if ( verbose )
      {

        std::cout << "subdividing mesh to get smaller surface elements ("
                  << subdivisionIterationCount << " iterations) : "
                  << std::flush;

      }

      // CGAL::Subdivision_method_3::CatmullClark_subdivision(
      //                     surfaceMesh,
      //                     CGAL::parameters::number_of_iterations( 1 ) );

      CGAL::Subdivision_method_3::Loop_subdivision(
          surfaceMesh,
          CGAL::parameters::number_of_iterations( subdivisionIterationCount ) );

      if ( verbose )
      {
 
        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating final MeshMap
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating final mesh map : "
                << std::flush;

    }


    // collecting vertices
    int32_t finalVertexCount = ( int32_t )surfaceMesh.number_of_vertices();
    std::vector< gkg::Vector3d< float > > vertices( finalVertexCount );

    Point_map pointMap( surfaceMesh.points() );
    std::vector< int32_t > reindex( finalVertexCount );

    int32_t index = 0;
    for ( CGAL::Surface_mesh< Point3 >::Vertex_index vi : 
                                                        surfaceMesh.vertices() )
    {

      gkg::Vector3d< float >& vertex = vertices[ index ];

      vertex.x = ( float )pointMap[ vi ].x();
      vertex.y = ( float )pointMap[ vi ].y();
      vertex.z = ( float )pointMap[ vi ].z();

      reindex[ vi ] = index;
      ++ index;

    }

    // collecting polygons
    int32_t finalPolygonCount = ( int32_t )surfaceMesh.number_of_faces();
    std::vector< gkg::Polygon< 3U > > polygons( finalPolygonCount );

    std::vector< int32_t > polygon;
    index = 0;
    for( CGAL::Surface_mesh< Point3 >::Face_index fi : surfaceMesh.faces() )
    {

      polygon.clear();

      for ( CGAL::Surface_mesh< Point3 >::Vertex_index vi :
                CGAL::vertices_around_face( surfaceMesh.halfedge( fi ),
                                            surfaceMesh ) )
      {

        polygon.push_back( reindex[ std::size_t( vi ) ] );

      }
      polygons[ index ].indices[ 0 ] = ( uint32_t )polygon[ 0 ];
      polygons[ index ].indices[ 1 ] = ( uint32_t )polygon[ 1 ];
      polygons[ index ].indices[ 2 ] = ( uint32_t )polygon[ 2 ];

      ++ index;

    }

    meshMap.vertices.addSites( rank, vertices );
    meshMap.polygons.addPolygons( rank, polygons );


    if ( verbose )
    {
 
      std::cout << "done " << std::endl;

    }


  }
  GKG_CATCH( "void gkg::CGALPointCloudAlphaShapeMesherImplementation::"
             "computeAlphaShapeMesh( "
             "const std::vector< gkg::Vector3d< float > >& pointCloud, "
             "gkg::MeshMap< int32_t, float, 3U >& meshMap, "
             "int32_t rank, "
             "bool removingOutliers, "
             "int32_t neighborCount, "
             "float outlierDistanceScale, "
             "bool simplifyingPointSet, "
             "bool smoothingPointSet, "
             "bool subdividingMesh, "
             "int32_t subdivisionIterationCount, "
             "const std::string& intermediateFileName, "
             "bool verbose ) const" );

}
