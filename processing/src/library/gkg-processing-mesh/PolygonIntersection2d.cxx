#include <gkg-processing-mesh/PolygonIntersection2d.h>
#include <gkg-core-exception/Exception.h>


#include <boost/geometry.hpp>
#include <boost/geometry/core/point_type.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/linestring.hpp>


BOOST_GEOMETRY_REGISTER_POINT_2D( gkg::Vector2d< float >, float,
                                  boost::geometry::cs::cartesian, x, y )

BOOST_GEOMETRY_REGISTER_LINESTRING( std::vector< gkg::Vector2d< float > > )


bool gkg::getPolygonIntersection2d(
              const std::vector< gkg::Vector2d< float > >& polygon1,
              const std::vector< gkg::Vector2d< float > >& polygon2,
              std::deque< std::vector< gkg::Vector2d< float > > > intersection )
{

  try
  {


#if BOOST_VERSION >= 105800 && \
    ( !defined( linux ) || ( defined( __GNUC__ ) && __GNUC__ >= 4 ) )

    boost::geometry::intersection( polygon1,
                                   polygon2,
                                   intersection );

#endif

    return !intersection.empty();


  }
  GKG_CATCH( "bool gkg::getPolygonIntersection2d( "
             "const std::vector< gkg::Vector2d< float > >& polygon1, "
             "const std::vector< gkg::Vector2d< float > >& polygon2, "
             "std::deque< std::vector< gkg::Vector2d< float > > > "
             "intersection )" );

}


