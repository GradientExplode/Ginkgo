#ifndef _gkg_processing_mesh_QuickConvexHull_h_
#define _gkg_processing_mesh_QuickConvexHull_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Polygon.h>
#include <vector>
#include <list>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > struct MeshMap;


class QuickConvexHull : public Singleton< QuickConvexHull >
{

  public:

    virtual ~QuickConvexHull();

    template < class R, class Compare >
    void addConvexHull( const std::vector< Vector3d< float > >& points,
                         const R& rank,
                         MeshMap< R, float, 3U, Compare >& meshMap ) const;

/*
    template < class R, class Compare >
    void addConvexHull( const std::list< Vector3d< float > >& points,
                         const R& rank,
                         MeshMap< R, float, 3U, Compare >& meshMap ) const;
*/

  private:

    friend class Singleton< QuickConvexHull >;

    QuickConvexHull();

    void getConvexHull( const std::vector< Vector3d< float > >& points,
                         std::list< Vector3d< float > >& vertices,
                         std::list< Polygon< 3U > >& polygons ) const;

/*
    void getConvexHull( const std::list< Vector3d< float > >& points,
                         std::list< Vector3d< float > >& vertices,
                         std::list< Polygon< 3U > >& polygons ) const;
*/

};


}


#endif
