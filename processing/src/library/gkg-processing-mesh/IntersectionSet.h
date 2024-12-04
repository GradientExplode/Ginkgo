#ifndef _gkg_processing_mesh_IntersectionSet_h_
#define _gkg_processing_mesh_IntersectionSet_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-mesh/SceneModeler.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <list>
#include <set>
#include <map>


namespace gkg
{


//
// struct Intersection
//

struct Intersection
{

  public:

    Intersection();
    Intersection( float theArcLength,
                  const Vector3d< float >& thePoint,
		  RCPointer< EvolvingMesh > theEvolvingMesh,
                  const Polygon< 3U >* thePolygon );
    Intersection( const Intersection& other );
    
    Intersection& operator=( const Intersection& other );
    
    float arcLength;
    Vector3d< float > point;
    RCPointer< EvolvingMesh > evolvingMesh;
    const Polygon< 3U >* polygon;
    
};


//
// class IntersectionSet
//

class IntersectionSet : public RCObject
{

  public:

    IntersectionSet( const SceneModeler& sceneModeler,
                     const Vector3d< float >& from,
                     const Vector3d< float >& to );
    virtual ~IntersectionSet();

    int32_t getCount() const;
    const Intersection& getIntersection( int32_t index ) const;
    void eraseIntersection( int32_t index );

  protected:

    bool getRayToTriangleIntersection(
    			           const Vector3d< float >& from,
                                   const Vector3d< float >& to,
                                   const Vector3d< float >& vertex1,
			           const Vector3d< float >& vertex2,
			           const Vector3d< float >& vertex3, 
			           Vector3d< float >& intersectionPoint ) const;

    std::vector< float > _arcLengths;
    std::map< float, Intersection > _intersections;

};


}


#endif
