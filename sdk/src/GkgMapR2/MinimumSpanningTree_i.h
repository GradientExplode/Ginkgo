#include "MinimumSpanningTree.h"
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


//
// template < class T > class MinimumSpanningTree::Edge
//

template < class T >
inline
gkg::MinimumSpanningTree< T >::Edge::Edge( int32_t theNode1,
                                           int32_t theNode2,
                                           double theWeight )
                                    : node1( theNode1 ),
                                      node2( theNode2 ),
                                      weight( theWeight )
{
}


template < class T >
inline
gkg::MinimumSpanningTree< T >::Edge::Edge(
                              const gkg::MinimumSpanningTree< T >::Edge& other )
                                    : node1( other.node1 ),
                                      node2( other.node2 ),
                                      weight( other.weight )
{
}


template < class T >
inline
typename gkg::MinimumSpanningTree< T >::Edge&
gkg::MinimumSpanningTree< T >::Edge::operator=(
                     const typename gkg::MinimumSpanningTree< T >::Edge& other )
{

  try
  {

    node1 = other.node1;
    node2 = other.node2;
    weight = other.weight;

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "typename gkg::MinimumSpanningTree< T >::Edge& "
             "gkg::MinimumSpanningTree< T >::Edge::Edge( "
             "const typename gkg::MinimumSpanningTree< T >::Edge& other )" );

}


template < class T >
inline
bool gkg::MinimumSpanningTree< T >::Edge::operator<( 
               const typename gkg::MinimumSpanningTree< T >::Edge& other ) const
{

  try
  {

    return ( this->weight > other.weight );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "bool gkg::MinimumSpanningTree< T >::Edge::operator<( "
             "const typename gkg::MinimumSpanningTree< T >::Edge& other ) "
             "const" );

}
                                      


//
// template < class T > class MinimumSpanningTree
//
template < class T >
inline
gkg::MinimumSpanningTree< T >::MinimumSpanningTree(
                                    const gkg::Volume< T >& volume,
                                    gkg::Neighborhood3d::Type neighborhoodType,
                                    int32_t nodeSpacing )
                              : _nodeSpacing( nodeSpacing )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the size of the tree
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > size;
    volume.getSize( size );
    size /= nodeSpacing;


    ////////////////////////////////////////////////////////////////////////////
    // creating weights between nodes
    ////////////////////////////////////////////////////////////////////////////

    int32_t nodeCount = size.x * size.y * size.z;

    _indexToVoxelLut.resize( nodeCount );

    std::list< Vector3d< int32_t > > neighborOffsets =
      gkg::Neighborhood3d::getInstance().getNeighborOffsets( neighborhoodType );
    int32_t neighborCount = ( int32_t )neighborOffsets.size();

    gkg::BoundingBox< int32_t > boundingBox( 0, size.x - 1,
                                             0, size.y - 1,
                                             0, size.z - 1 );                               


    std::vector< int32_t > neighborIndices( nodeCount * neighborCount, -1 );
    std::vector< float > edgeWeights( nodeCount * neighborCount, 0.0f );


    std::list< Vector3d< int32_t > >::const_iterator 
      n = neighborOffsets.begin(),
      ne = neighborOffsets.end();
    int32_t neighborIndex = 0;
    int32_t index = 0;
    gkg::Vector3d< int32_t > currentControlPoint;
    gkg::Vector3d< int32_t > neighborControlPoint;

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;

    std::ofstream os( "Cyril/mst_1.txt" );

    int32_t currentControlPointIndex = 0;
    for ( currentControlPoint.z = 0; currentControlPoint.z < size.z;
          currentControlPoint.z++ )
    {

      for ( currentControlPoint.y = 0; currentControlPoint.y < size.y; 
            currentControlPoint.y++ )
      {

        for ( currentControlPoint.x = 0; currentControlPoint.x < size.x;
              currentControlPoint.x++ )
        {

          n = neighborOffsets.begin();
          neighborIndex = 0;
          while ( n != ne )
          {

            neighborControlPoint.x = currentControlPoint.x + n->x;
            neighborControlPoint.y = currentControlPoint.y + n->y;
            neighborControlPoint.z = currentControlPoint.z + n->z;

            if ( boundingBox.contains( neighborControlPoint ) )
            {


              index =  this->getIndexFromVoxel( currentControlPoint, size ) +
                       neighborIndex * nodeCount;
              neighborIndices[ index ] =
                this->getIndexFromVoxel( neighborControlPoint, size );

              os << "currentControlPoint=" << currentControlPoint
                 << "neighborControlPoint=" << neighborControlPoint
                 << "  "
                 << "index=" << index 
                 << "  "
                 << "neighborIndices[ index ]="
                 << neighborIndices[ index ]
                 << " : ";

              for ( z = 0; z < nodeSpacing; z++ )
              {

                for ( y = 0; y < nodeSpacing; y++ )
                {

                  for ( x = 0; x < nodeSpacing; x++ )
                  {

                    edgeWeights[ index ] += std::fabs( 
                      ( float )volume(
                                   currentControlPoint.x * nodeSpacing + x,
                                   currentControlPoint.y * nodeSpacing + y,
                                   currentControlPoint.z * nodeSpacing + z ) -
                      ( float )volume(
                                   neighborControlPoint.x * nodeSpacing + x,
                                   neighborControlPoint.y * nodeSpacing + y,
                                   neighborControlPoint.z * nodeSpacing + z ) );


                  }

                }

              }
              os << "edgeWeights[ index ]=" << edgeWeights[ index ]
                 << std::endl;

            }

            ++ n;
            ++ neighborIndex;

          }

          ++ currentControlPointIndex;

        }

      }

    }
    os.close();

    os.open( "Cyril/mst_2.txt" );

    for ( int32_t ii = 0; ii < nodeCount * neighborCount; ii++ )
      os << neighborIndices[ ii ] << " " << edgeWeights[ ii ] << std::endl;

    os.close();

    ////////////////////////////////////////////////////////////////////////////
    // creating the Minimum Spanning Tree
    ////////////////////////////////////////////////////////////////////////////

    // allocating temporary structures used to create the MST
    std::vector< typename gkg::MinimumSpanningTree< T >::Edge > priorities;
    std::vector< bool > isNodeBelongingToMSTs( nodeCount, false );
    std::vector< int32_t > nodeLevels( nodeCount, 0 );

    priorities.reserve( neighborCount * nodeCount );

    // creating root and setting accordingly values of the temporary structures
    os.open( "Cyril/mst_3.txt" );

    gkg::Vector3d< int32_t > rootControlPoint( size.x / 2,
                                               size.y / 2,
                                               size.z / 2 );
    int32_t rootIndex = this->getIndexFromVoxel( rootControlPoint, size );

    os << "nodeCount=" << nodeCount << std::endl;
    os << "rootIndex=" << rootIndex << std::endl;

    nodeLevels[ rootIndex ] = 0;
    isNodeBelongingToMSTs[ rootIndex ] = true;

    _parentNodeIndices = std::vector< int32_t >( nodeCount, 0 );

    int32_t lastNodeIndex = rootIndex;
    typename gkg::MinimumSpanningTree< T >::Edge edge;  


    // looping (nodeCount-1) times to have all nodes added at the end
    int32_t nodeIndex = 0;
    float globalCost = 0.0f;
    for ( nodeIndex = 0; nodeIndex < nodeCount - 1; nodeIndex++ )
    {

      os << "nodeIndex=" << nodeIndex << std::endl;
      // adding edges of new node to the priority queue
      for ( neighborIndex = 0; neighborIndex < neighborCount; neighborIndex++ )
      {

        index = lastNodeIndex + neighborIndex * nodeCount;
        const int32_t& neighborNodeIndex = neighborIndices[ index ];

        os << "index=" << index << std::endl;
        os << "neighborNodeIndex=" << neighborNodeIndex << std::endl;

        if ( neighborNodeIndex >= 0 )
        {

          priorities.push_back(
            typename gkg::MinimumSpanningTree< T >::Edge(
                                                       lastNodeIndex,
                                                       neighborNodeIndex,
                                                       edgeWeights[ index ] ) );
          push_heap( priorities.begin(), priorities.end() );

        }

      }

      lastNodeIndex = -1;

      // finding a valid edge with the lowest weight (main step of Prim's 
      // algorithm)
      typename gkg::MinimumSpanningTree< T >::Edge 
        edgeWithLowestWeight( -1, -1, 0.0 );
      typename gkg::MinimumSpanningTree< T >::Edge
        newEdge( -1, -1, 0.0 );
      while ( lastNodeIndex == -1 )
      {

        // getting a copy of the edge having the lowest weight
        edgeWithLowestWeight = priorities.front();

        // removing it from the head and from the vector
        // swaps the value in the position first and the value in 
        // the position last-1 and makes the subrange [first, last-1) 
        // into a max heap. This has the effect of removing the first 
        // (largest) element from the heap defined by the range [first, last)
        pop_heap( priorities.begin(), priorities.end() );
        priorities.pop_back();

        // returning the next valid node or -1 if edge already exists
        lastNodeIndex = this->getNodeAndCreateNewEdge( edgeWithLowestWeight,
                                                       newEdge,
                                                       isNodeBelongingToMSTs );
        os << "lastNodeIndex=" << lastNodeIndex << std::endl;

      }

      // updaing the global cost and notifying the connection of the last node
      globalCost += newEdge.weight;
      isNodeBelongingToMSTs[ lastNodeIndex ] = true;

      // creating a new node level and keeing track of the parent information
      nodeLevels[ newEdge.node2 ] = nodeLevels[ newEdge.node1 ] + 1;
      _parentNodeIndices[ newEdge.node2 ] = newEdge.node1;

      if ( newEdge.node2 == 78020 )

        std::cout << "parentsMST[ 78020 ] = " << newEdge.node1 << std::endl;

      os << "newEdge.node2" << newEdge.node2 << " -> "
         << _parentNodeIndices[ newEdge.node2 ] << std::endl;

    }

    os.close();

    os.open( "Cyril/mst_4.txt" );

    for ( int32_t ii = 0; ii < nodeCount; ii++ )
      os << _parentNodeIndices[ ii ] << std::endl;

    os.close();

    ////////////////////////////////////////////////////////////////////////////
    // finding the correct ordering 
    ////////////////////////////////////////////////////////////////////////////

    int32_t maximumNodeLevel = 0;
    int32_t nodeLevel = 0;
    for ( nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++ )
    {

      nodeLevel = nodeLevels[ nodeIndex ];
      if ( nodeLevel > maximumNodeLevel )
      {

        maximumNodeLevel = nodeLevel;

      }

    }

    ++ maximumNodeLevel;

    std::vector< int32_t > nodeLevelOffsets( maximumNodeLevel, 0 );
    std::vector< int32_t > nodeLevelCounts( maximumNodeLevel, 0 );

    // counting the number of nodes in each level
    for ( nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++ )
    {

      nodeLevel = nodeLevels[ nodeIndex ];
      if ( nodeLevel < maximumNodeLevel - 1 )
      {
			
        ++ nodeLevelOffsets[ nodeLevel + 1 ]; 
  
      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing cumulative sum
    ////////////////////////////////////////////////////////////////////////////
    for ( nodeLevel = 1; nodeLevel < maximumNodeLevel; nodeLevel++ )
    {
		
      nodeLevelOffsets[ nodeLevel ] += nodeLevelOffsets[ nodeLevel - 1 ];

    }

    ////////////////////////////////////////////////////////////////////////////
    // filling the vector of node indices of the MST
    ////////////////////////////////////////////////////////////////////////////
    int32_t i = 0;
    _nodeIndices.resize( nodeCount, 0 );
    for ( nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++ )
    {

      i = nodeLevelOffsets[ nodeLevels[ nodeIndex ] ] +
          nodeLevelCounts[ nodeLevels[ nodeIndex ] ];
      ++ nodeLevelCounts[ nodeLevels[ nodeIndex ] ];
      _nodeIndices[ i ] = nodeIndex;
      _indexToVoxelLut[ nodeIndex ].z = nodeIndex / ( size.x * size.y );
      _indexToVoxelLut[ nodeIndex ].y = ( nodeIndex -
                                          _indexToVoxelLut[ nodeIndex ].z *
                                          size.x * size.y ) / size.x;
      _indexToVoxelLut[ nodeIndex ].x = ( nodeIndex - 
                                          _indexToVoxelLut[ nodeIndex ].z *
                                          size.x * size.y -
                                          _indexToVoxelLut[ nodeIndex ].y *
                                          size.x );

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "gkg::MinimumSpanningTree< T >::MinimumSpanningTree( "
             "const gkg::Volume< T >& volume, "
             "int32_t nodeSpacing )" );

}


template < class T >
inline
gkg::MinimumSpanningTree< T >::~MinimumSpanningTree()
{
}


template < class T >
inline
int32_t gkg::MinimumSpanningTree< T >::getNodeSpacing() const
{

  try
  {

    return _nodeSpacing;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::MinimumSpanningTree< T >::getNodeSpacing() const" );

}


template < class T >
inline
int32_t gkg::MinimumSpanningTree< T >::getNodeCount() const
{

  try
  {

    return ( int32_t )_nodeIndices.size();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::MinimumSpanningTree< T >::getNodeCount() const" );

}


template < class T >
inline
int32_t gkg::MinimumSpanningTree< T >::getNodeIndex( int32_t index ) const
{

  try
  {

    return _nodeIndices[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::MinimumSpanningTree< T >::getNodeIndex( "
             "int32_t index ) const" );

}


template < class T >
inline
int32_t gkg::MinimumSpanningTree< T >::getParentNodeIndex( int32_t index ) const
{

  try
  {

    return _parentNodeIndices[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::MinimumSpanningTree< T >::getParentNodeIndex( "
             "int32_t index ) const" );

}


template < class T >
inline
const gkg::Vector3d< int32_t >& 
gkg::MinimumSpanningTree< T >::getVoxelFromIndex( int32_t index ) const
{

  try
  {

    return _indexToVoxelLut[ index ];

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "const gkg::Vector3d< int32_t >& "
             "gkg::MinimumSpanningTree< T >::getVoxelFromIndex( "
             "int32_t index ) const" );

}


template < class T >
inline
int32_t gkg::MinimumSpanningTree< T >::getIndexFromVoxel(
                                   const gkg::Vector3d< int32_t >& controlPoint,
                                   const gkg::Vector3d< int32_t >& size ) const
{

  try
  {

    return ( controlPoint.z * size.y * size.x +
             controlPoint.y * size.x +
             controlPoint.x );

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t gkg::MinimumSpanningTree< T >::getIndexFromVoxel( "
             "const gkg::Vector3d< int32_t >& controlPoint, "
             "const gkg::Vector3d< int32_t >& size ) const" );

}


template < class T >
inline
int32_t 
gkg::MinimumSpanningTree< T >::getNodeAndCreateNewEdge(
                       const typename gkg::MinimumSpanningTree< T >::Edge& edge,
                       typename gkg::MinimumSpanningTree< T >::Edge& newEdge,
                       const std::vector< bool >& isNodeBelongingToMSTs ) const
{

  try
  {

    bool isNode1BelongingToMST = isNodeBelongingToMSTs[ edge.node1 ]; 
    bool isNode2BelongingToMST = isNodeBelongingToMSTs[ edge.node2 ]; 

    int32_t node = -1;

    if ( isNode1BelongingToMST ^ isNode2BelongingToMST )
    {
		
      if( isNode1BelongingToMST )
      {
			
        node = edge.node2;
        newEdge = typename gkg::MinimumSpanningTree< T >::Edge( edge.node1,
                                                                edge.node2,
                                                                edge.weight );
		
      }
      else 
      {
			
        node = edge.node1;
        newEdge = typename gkg::MinimumSpanningTree< T >::Edge( edge.node2,
                                                                edge.node1,
                                                                edge.weight );
		
      }

    }
    return node;

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "int32_t "
             "gkg::MinimumSpanningTree< T >::getNodeAndCreateNewEdge( "
             "const typename gkg::MinimumSpanningTree< T >::Edge& edge, "
             "typename gkg::MinimumSpanningTree< T >::Edge& newEdge, "
             "const std::vector< bool >& isNodeBelongingToMSTs ) const" );

}



