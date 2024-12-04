#include <gkg-processing-mesh/IntersectionSet.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <cmath>


#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>


#define EPSILON 1e-20


//
// struct Intersection
//

gkg::Intersection::Intersection()
                  : arcLength( -1.0 )
{
}


gkg::Intersection::Intersection( 
                            float theArcLength,
                            const gkg::Vector3d< float >& thePoint,
                            gkg::RCPointer< gkg::EvolvingMesh > theEvolvingMesh,
                            const gkg::Polygon< 3U >* thePolygon )
                  : arcLength( theArcLength ),
                    point( thePoint ),
                    evolvingMesh( theEvolvingMesh ),
                    polygon( thePolygon )
{
}


gkg::Intersection::Intersection( const gkg::Intersection& other )
                  : arcLength( other.arcLength ),
                    point( other.point ),
                    evolvingMesh( other.evolvingMesh ),
                    polygon( other.polygon )
{
}


gkg::Intersection& 
gkg::Intersection::operator=( const gkg::Intersection& other )
{

  try
  {

    arcLength = other.arcLength;
    point = other.point;
    evolvingMesh = other.evolvingMesh;
    polygon = other.polygon;

    return *this;

  }
  GKG_CATCH( "gkg::Intersection& "
             "gkg::Intersection::operator=( const gkg::Intersection& other )" );

}



//
// class IntersectionSet
//

gkg::IntersectionSet::IntersectionSet( const gkg::SceneModeler& sceneModeler,
                                       const gkg::Vector3d< float >& from,
                                       const gkg::Vector3d< float >& to )
                     : gkg::RCObject()
{

  try
  {

    // first, collecting the voxels where the ray is passing through
    std::set< gkg::Vector3d< int32_t >,
              gkg::Vector3dCompare< int32_t > > voxels;
    sceneModeler.getBresenhamAlgorithm().getRayVoxels( from, to, voxels,
                                                       false );

    // calculating from->to vector and its norm
    gkg::Vector3d< float > fromTo = to - from;
    float fromToLength = fromTo.getNorm();

    // looping over voxels, membranes, and polygons to get the interseting
    // point & the arc length
    std::set< float > arcLengths;
    gkg::Vector3d< float > vertex1, vertex2, vertex3;
    gkg::Vector3d< float > intersectionPoint;
    gkg::Vector3d< float > fromIntersection;
    float fromIntersectionLength;
    bool existingIntersection = false;
    std::set< gkg::Vector3d< int32_t >,
              gkg::Vector3dCompare< int32_t > >::const_iterator
      v = voxels.begin(),
      ve = voxels.end();
    while ( v != ve )
    {

      // collecting the set of membranes crossing this voxel
      std::set< gkg::RCPointer< gkg::EvolvingMesh > >* evolvingMeshes =
        sceneModeler.getEvolvingMeshCache().getEvolvingMeshes( *v );

      // if the set of membranes is not empty
      if ( evolvingMeshes )
      {

        // looping over the evolving meshes
        std::set< gkg::RCPointer< gkg::EvolvingMesh > >::const_iterator
          m = evolvingMeshes->begin(),
          me = evolvingMeshes->end();

        while ( m != me )
        {

          // collecting the set of polygons corresponding to the current
          // membrane and contained inside the current voxel
          std::set< const gkg::Polygon< 3U >* >* 
            polygons = ( *m )->getPolygonCache().getPolygons( *v );

          // looping over the polygons
          std::set< const gkg::Polygon< 3U >* >::const_iterator
            p = polygons->begin(),
            pe = polygons->end();
          while ( p != pe )
          {

            // obtaining the current vertex positions of the polygon
            vertex1 = ( *m )->getCurrentVertex( ( *p )->indices[ 0 ] );
            vertex2 = ( *m )->getCurrentVertex( ( *p )->indices[ 1 ] );
            vertex3 = ( *m )->getCurrentVertex( ( *p )->indices[ 2 ] );

            // do the ray/triangle intersection to find the intersecting point
            existingIntersection = getRayToTriangleIntersection(
                                                            from,
                                                            to,
                                                            vertex1,
                                                            vertex2,
                                                            vertex3,
                                                            intersectionPoint );

            if ( existingIntersection )
            {

              // computing from->intersection vector
              fromIntersection = ( intersectionPoint - from );
              fromIntersectionLength = fromIntersection.getNorm();

              // check whether the intersecting point is in the segment of
              // [ from , to )
              // if it belongs to the segment: add the arc length, the point,
              // the polygon and the membrane to 'Intersection'
              if ( ( fromIntersectionLength < fromToLength ) &&
                   ( fromIntersection.dot( fromTo ) > 0.0 ) )
              {

                gkg::Intersection intersection( fromIntersectionLength,
                				intersectionPoint,
                				*m,
                				*p );

                arcLengths.insert( fromIntersectionLength );
                _intersections[ fromIntersectionLength ] = intersection;

              }

            }
            ++ p;

          }
          ++ m;

        }

      }
      ++ v;

    }

    // copying the arclength set to a vector
    _arcLengths.resize( arcLengths.size() );
    std::set< float >::const_iterator a = arcLengths.begin(),
                                      ae = arcLengths.end();
    int32_t index = 0;
    while ( a != ae )
    {

      _arcLengths[ index ] = *a;
      ++ a;
      ++ index;

    }

  }
  GKG_CATCH( "gkg::IntersectionSet::IntersectionSet( "
             "const gkg::SceneModeler& sceneModeler, "
             "const gkg::Vector3d< float >& from, "
             "const gkg::Vector3d< float >& to )" );

}


gkg::IntersectionSet::~IntersectionSet()
{
}


int32_t gkg::IntersectionSet::getCount() const
{

  try
  {

    return ( int32_t )_arcLengths.size();

  }
  GKG_CATCH( "int32_t gkg::IntersectionSet::getCount() const" );

}


const gkg::Intersection& 
gkg::IntersectionSet::getIntersection( int32_t index ) const
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "bad index" );

    }

#endif

    std::map< float, Intersection >::const_iterator
      i = _intersections.find( _arcLengths[ index ] );
    if ( i == _intersections.end() )
    {

      throw std::runtime_error( "intersection not found" );

    }
    return i->second;

  }
  GKG_CATCH( "const gkg::Intersection& "
             "gkg::IntersectionSet::getIntersection( int32_t index ) const" );

}


void gkg::IntersectionSet::eraseIntersection( int32_t index )
{

  try
  {

#ifdef GKG_DEBUG

    if ( ( index < 0 ) || ( index >= getCount() ) )
    {

      throw std::runtime_error( "bad index" );

    }

#endif

    _intersections.erase( _arcLengths[ index ] );
    _arcLengths.erase( _arcLengths.begin() + index );

  }
  GKG_CATCH( "void gkg::IntersectionSet::eraseIntersection( int32_t index )" );

}



bool gkg::IntersectionSet::getRayToTriangleIntersection(
                               const gkg::Vector3d< float >& from,
                               const gkg::Vector3d< float >& to,
                               const gkg::Vector3d< float >& vertex1,
                               const gkg::Vector3d< float >& vertex2,
                               const gkg::Vector3d< float >& vertex3, 
                               gkg::Vector3d< float >& intersectionPoint ) const
{

  try
  {

    gkg::Vector3d< float > u = vertex2 - vertex1;
    gkg::Vector3d< float > v = vertex3 - vertex1;
    gkg::Vector3d< float > n = u.cross( v );
    if ( n.getNorm2() < EPSILON )
    {

      return false;

    }
    n.normalize();

    gkg::Vector3d< float > rayDirection = to - from;
    gkg::Vector3d< float > w0 = from - vertex1;

    float a = -n.dot( w0 );
    float b = n.dot( rayDirection );
    // ray is parallel to triangle plane
    if ( b == 0.0f )
    {

      // ray lies in triangle plane
      if ( a == 0.0f )
      {

        return false;

      }
      // ray disjoint from plane
      else
      {

        return false;

      }

    }

    float r = a / b;
    if ( r < 0.0 )
    {

      return false;

    }

    // processing ray/triangle plane intersection
    intersectionPoint = from + rayDirection * r;

    // is intersection inside the triangle ?
    float uu = u.dot( u );
    float uv = u.dot( v );
    float vv = v.dot( v );
    gkg::Vector3d< float > w = intersectionPoint - vertex1;
    float wu = w.dot( u );
    float wv = w.dot( v );
    float D = uv * uv - uu * vv;
 
    // get and test parametric coords
    float s = ( uv * wv - vv * wu ) / D;

    // intersection is outside of the triangle
    if ( ( s < 0.0 ) || ( s > 1.0 ) )
    {

      return false;

    }
    float t = ( uv * wu - uu * wv ) / D;
    // intersection is outside of the triangle
    if ( ( t < 0.0 ) || ( ( s + t ) > 1.0 ) )
    {

      return false;

    }
 
    return true;

  }
  GKG_CATCH( "bool gkg::IntersectionSet::getRayToTriangleIntersection( "
             "const gkg::Vector3d< float >& from, "
             "const gkg::Vector3d< float >& to, "
             "const gkg::Vector3d< float >& vertex1, "
             "const gkg::Vector3d< float >& vertex2, "
             "const gkg::Vector3d< float >& vertex3, "
             "gkg::Vector3d< float >& intersectionPoint ) const" );

}


#undef EPSILON

