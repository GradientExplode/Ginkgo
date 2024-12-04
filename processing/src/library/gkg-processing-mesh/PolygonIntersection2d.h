#ifndef _gkg_processing_mesh_PolygonIntersection2d_h_
#define _gkg_processing_mesh_PolygonIntersection2d_h_


#include <gkg-processing-coordinates/Vector2d.h>
#include <vector>
#include <deque>


namespace gkg
{


bool getPolygonIntersection2d(
                  const std::vector< Vector2d< float > >& polygon1,
                  const std::vector< Vector2d< float > >& polygon2,
                  std::deque< std::vector< Vector2d< float > > > intersection );



}



#endif
