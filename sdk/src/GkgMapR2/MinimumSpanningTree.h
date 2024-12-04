#ifndef _gkg_processing_graph_MinimumSpanningTree_h_
#define _gkg_processing_graph_MinimumSpanningTree_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <vector>


namespace gkg
{


template < class T >
class MinimumSpanningTree
{

  public:

    struct Edge
    {

      Edge( int32_t theNode1 = 0,
            int32_t theNode2 = 0,
            double theWeight = 0.0 );
      Edge( const Edge& other );

      Edge& operator=( const Edge& other );
      bool operator<( const Edge& other ) const;

      int32_t node1;
      int32_t node2;
      double weight;

    };

    MinimumSpanningTree( const Volume< T >& volume,
                         Neighborhood3d::Type neighborhoodType,
                         int32_t nodeSpacing );
    virtual ~MinimumSpanningTree();

    int32_t getNodeSpacing() const;

    int32_t getNodeCount() const;
    int32_t getNodeIndex( int32_t index ) const;
    int32_t getParentNodeIndex( int32_t index ) const;

    const Vector3d< int32_t >& getVoxelFromIndex( int32_t index ) const;

  protected:

    int32_t getIndexFromVoxel( const Vector3d< int32_t >& controlPoint,
                               const Vector3d< int32_t >& size ) const;

    int32_t 
      getNodeAndCreateNewEdge(
                     const Edge& edge,
                     Edge& newEdge,
                     const std::vector< bool >& isNodeBelongingToMSTs ) const;

    int32_t _nodeSpacing;
    std::vector< int32_t > _nodeIndices;
    std::vector< int32_t > _parentNodeIndices;

    std::vector< Vector3d< int32_t > > _indexToVoxelLut;

};



}


#endif
