#ifndef _gkg_processing_mesh_ConvexHull_h_
#define _gkg_processing_mesh_ConvexHull_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Polygon.h>
#include <vector>
#include <list>


namespace gkg
{


template < class R, class S, uint32_t D, class Compare > struct MeshMap;


class ConvexHull : public Singleton< ConvexHull >
{

  public:

    struct Vertex;
    struct Face;
    struct Edge;

    struct Vertex
    {

      Vertex( int32_t x,
              int32_t y,
              int32_t z,
              int32_t i );

      Vector3d< int32_t > vertex;
      int32_t index;
      Edge* duplicate;
      bool onHull;
      bool mark;

      Vertex* next;
      Vertex* previous;

    };

    struct Face
    {

      Face( Face** theFaces,
            Edge** theEdges,
            Vertex* vertex0,
            Vertex* vertex1,
            Vertex* vertex2,
            Face* fold );
      Face( Face** theFaces,
            Edge** theEdges,
            Edge* edge,
            Vertex* vertex );

      void linkAdjacentFace( Face* other );

      Edge* edges[ 3 ];
      Vertex* vertices[ 3 ];
      bool visible;

      Face* next;
      Face* previous;

    };

    struct Edge
    {

      Edge( Edge** theEdges );

      Face* adjacentFaces[ 2 ];
      Vertex* endPoints[ 2 ];
      Face* newFace;
      bool cDelete;

      Edge* next;
      Edge* previous;

    };

    virtual ~ConvexHull();

    template < class R, class Compare >
    void addConvexHull( const std::vector< Vector3d< float > >& points,
                        const R& rank,
                        MeshMap< R, float, 3U, Compare >& meshMap ) const;

  private:

    friend class Singleton< ConvexHull >;

    ConvexHull();

    void getConvexHull( const std::vector< Vector3d< float > >& points,
                        std::list< Vector3d< float > >& vertices,
                        std::list< Polygon< 3U > >& polygons ) const;
    int32_t getVolumeSign( Face* face, Vertex* vertex ) const;
    void addOne( Face** theFaces,
                 Edge** theEdges,
                 Vertex* vertex ) const;
    void cleanup( Face** theFaces,
                  Edge** theEdges,
                  Vertex** theIntegerVertices ) const;

};


}


#endif
