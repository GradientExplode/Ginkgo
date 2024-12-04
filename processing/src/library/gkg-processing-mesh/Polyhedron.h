#ifndef _gkg_processing_mesh_Polyhedron_h_
#define _gkg_processing_mesh_Polyhedron_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;


class Polyhedron
{

  public:

    typedef Vector3d< int32_t > Neighbors;
    typedef std::pair< Vector3d< float >, Neighbors > Vertex;

    typedef std::vector< Vertex >::iterator iterator;
    typedef std::vector< Vertex >::const_iterator const_iterator;

    Polyhedron();
    Polyhedron( const Polyhedron& other );
    virtual ~Polyhedron();


    Polyhedron& operator=( const Polyhedron& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    void reserve( int32_t vertexCount );
    void addVertex( const Vector3d< float >& position,
                    int32_t neighbor1,
                    int32_t neighbor2,
                    int32_t neighbor3 );
    void addVertex( const Vector3d< float >& position,
                    const Neighbors& neighbors );
    void addVertex( const Vertex& vertex );
    void setVertexPosition( int32_t index, const Vector3d< float >& position );
    void setVertexNeighbor( int32_t index,
                            int32_t neighborCoordinate,
                            int32_t neighborIndex );
    void clear();

    int32_t getVertexCount() const;
    const Vertex& getVertex( int32_t index ) const;
    Vertex& getVertex( int32_t index );

    void voxelize( const Vector3d< int32_t >& lowerVoxelBound,
                   const Vector3d< int32_t >& upperVoxelBound,
                   Volume< uint8_t >& grid ) const;

    void voxelize(
                  const Vector3d< int32_t >& gridSize,
                  const Vector3d< float >& gridResolution,
                  std::set< gkg::Vector3d< int32_t >,
                            gkg::Vector3dCompare< int32_t > >& voxelSet ) const;

    void display() const;

  protected:

    void split( Polyhedron& inputPolyhedron,
                Polyhedron** outputPolyhedrons,
                float position,
                int32_t maximumCoordinate ) const;

    std::vector< Vertex > _vertices;

};


}


#endif


