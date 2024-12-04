#include <gkg-processing-numericalanalysis/HierarchicalClustering.h>
#include <gkg-processing-algobase/MaximumIndexFilter_i.h>
#include <gkg-core-exception/Exception.h>
#include <limits>


gkg::HierarchicalClustering::HierarchicalClustering(
                               const gkg::Matrix& matrix,
                               const gkg::TestFunction< double >& testFunction )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the node count
    ////////////////////////////////////////////////////////////////////////////

    // removing 1 because vertex indices start from 1
    _vertexCount = matrix.getSize1() - 1;


    ////////////////////////////////////////////////////////////////////////////
    // computing the number of elements that satisfy the test function
    ////////////////////////////////////////////////////////////////////////////

    int64_t itemCount = 0;

    int32_t size1 = matrix.getSize1();
    int32_t size2 = matrix.getSize2();
    int32_t i1 = 0;
    int32_t i2 = 0;

    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {


        if ( testFunction.getState( matrix( i1, i2 ) ) )
        {

          ++ itemCount;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating edges and weights
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::pair< int32_t, int32_t > > edges( itemCount );
    std::vector< float > weights( itemCount );


    ////////////////////////////////////////////////////////////////////////////
    // filling the edges and weights from sparse matrix item(s)
    ////////////////////////////////////////////////////////////////////////////

    int64_t itemIndex = 0;
    for ( i1 = 0; i1 < size1; ++ i1 )
    {

      for ( i2 = 0; i2 < size2; ++ i2 )
      {

        if ( testFunction.getState( matrix( i1, i2 ) ) )
        {

          edges[ itemIndex ].first = ( int32_t )i1 - 1;
          edges[ itemIndex ].second = ( int32_t )i2 - 1;
          weights[ itemIndex ] = ( float )matrix( i1, i2 );

          ++ itemIndex;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing connected component(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int32_t > connectedComponents;
    int32_t connectedComponentCount = this->computeConnectedComponents(
                                                          edges,
                                                          connectedComponents );
    _maximumVertexCount = 2 * _vertexCount - connectedComponentCount;


    ////////////////////////////////////////////////////////////////////////////
    // computing average link
    ////////////////////////////////////////////////////////////////////////////

    this->computeAverageLink( connectedComponentCount,
                              edges,
                              weights,
                              _heights,
                              _fathers );

  }
  GKG_CATCH( "gkg::HierarchicalClustering::HierarchicalClustering( "
             "const gkg::Matrix& sparseMatrix, "
             "const gkg::TestFunction< double >& testFunction )" );

}


gkg::HierarchicalClustering::HierarchicalClustering(
                               const gkg::SparseMatrix& sparseMatrix,
                               const gkg::TestFunction< double >& testFunction )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // computing the node count
    ////////////////////////////////////////////////////////////////////////////

    // removing 1 because vertex indices start from 1
    _vertexCount = sparseMatrix.getSize1() - 1;


    ////////////////////////////////////////////////////////////////////////////
    // computing the number of elements that satisfy the test function
    ////////////////////////////////////////////////////////////////////////////

    int64_t itemCount = 0;
    gkg::SparseMatrix::const_iterator1 i1;
    gkg::SparseMatrix::const_iterator2 i2;
    for ( i1 = sparseMatrix.begin1(); i1 != sparseMatrix.end1(); ++ i1 )
    {

      for ( i2 = i1.begin(); i2 != i1.end(); ++ i2 )
      {

        if ( testFunction.getState( *i2 ) )
        {

          ++ itemCount;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating edges and weights
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::pair< int32_t, int32_t > > edges( itemCount );
    std::vector< float > weights( itemCount );


    ////////////////////////////////////////////////////////////////////////////
    // filling the edges and weights from sparse matrix item(s)
    ////////////////////////////////////////////////////////////////////////////

    int64_t itemIndex = 0;
    for ( i1 = sparseMatrix.begin1(); i1 != sparseMatrix.end1(); ++ i1 )
    {

      for ( i2 = i1.begin(); i2 != i1.end(); ++ i2 )
      {

        if ( testFunction.getState( *i2 ) )
        {

          edges[ itemIndex ].first = ( int32_t )i2.index1() - 1;
          edges[ itemIndex ].second = ( int32_t )i2.index2() - 1;

          weights[ itemIndex ] = ( float )*i2;

          ++ itemIndex;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing connected component(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int32_t > connectedComponents;
    int32_t connectedComponentCount = this->computeConnectedComponents(
                                                          edges,
                                                          connectedComponents );
    _maximumVertexCount = 2 * _vertexCount - connectedComponentCount;


    ////////////////////////////////////////////////////////////////////////////
    // computing average link
    ////////////////////////////////////////////////////////////////////////////

    this->computeAverageLink( connectedComponentCount,
                              edges,
                              weights,
                              _heights,
                              _fathers );

  }
  GKG_CATCH( "gkg::HierarchicalClustering::HierarchicalClustering( "
             "const gkg::SparseMatrix& sparseMatrix, "
             "const gkg::TestFunction< double >& testFunction )" );

}


gkg::HierarchicalClustering::~HierarchicalClustering()
{
}


int32_t gkg::HierarchicalClustering::getMaximumVertexCount() const
{

  try
  {

    return _maximumVertexCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::HierarchicalClustering::getMaximumVertexCount() const" );

}


const std::vector< int32_t >& gkg::HierarchicalClustering::getFathers() const
{

  try
  {

    return _fathers;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::HierarchicalClustering::getFathers() const" );

}


void gkg::HierarchicalClustering::getChildren(
                  std::vector< std::pair< int32_t, int32_t > >& children ) const
{

  try
  {

    children.resize( _maximumVertexCount,
                     std::pair< int32_t, int32_t >( -1, -1 ) );

    int32_t index = 0;
    for ( index = 0; index < _maximumVertexCount; index++ )
    {

      const int32_t& father = this->_fathers[ index ];
      if ( father != index )
      {

        if ( children[ father ].first == - 1 )
        {

          children[ father ].first = index;

        }
        else
        {

          children[ father ].second = index;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::HierarchicalClustering::getChildren( "
             "std::vector< std::pair< int32_t, int32_t >& children ) const" );

}

int32_t gkg::HierarchicalClustering::getForest(
                               const std::vector< int32_t >& parcelVoxelCounts,
                               int32_t averageClusterSize,
                               int32_t isolatedClusterMinimumSize,
                               int32_t minimumClusterSize,
                               float minimumParcelSizeRatio,
                               std::vector< int32_t >& nodeClusterLabels )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // compute children
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::pair< int32_t, int32_t > > children;
    this->getChildren( children );


    ////////////////////////////////////////////////////////////////////////////
    // determine single vertices 
    ////////////////////////////////////////////////////////////////////////////

    std::list< int32_t > singleVertices;
    int32_t index = 0;
    for ( index = 0; index < _vertexCount; index++ )
    {

      if ( this->_fathers[ index ] == index )
      {

        singleVertices.push_back( index );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // determine top vertices of clusters
    ////////////////////////////////////////////////////////////////////////////

    std::list< int32_t > topVertices;
    for ( index = _vertexCount; index < _maximumVertexCount; index++ )
    {

      if ( this->_fathers[ index ] == index )
      {

        topVertices.push_back( index );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the number of vertex included in the weighted forest clustering
    ////////////////////////////////////////////////////////////////////////////

    //int32_t forestVertexCount = _vertexCount -
    //                            ( int32_t )singleVertices.size();


    ////////////////////////////////////////////////////////////////////////////
    // removing singular vertices (eg with only one child)
    ////////////////////////////////////////////////////////////////////////////

    for ( index = 0; index < _vertexCount; index++ )
    {

      if ( ( children[ index ].first != -1 ) &&
           ( children[ index ].second == -1 ) )
      {

        std::list< int32_t > queue;
        queue.push_back( index );
        while ( !queue.empty() )
        {

          // collecting current vertex index
          int32_t currentVertexIndex = queue.back();

          // removing it from queue
          queue.pop_back();

          // adding it to the single vertices
          singleVertices.push_back( currentVertexIndex );

          // setting its father
          this->_fathers[ currentVertexIndex ] = currentVertexIndex;

          // collecting children of current vertex index and adding them to the
          // queue if existing
          std::pair< int32_t, int32_t >&
            currentChildren = children[ currentVertexIndex ];

          if ( currentChildren.first != -1 )
          {

            queue.push_back( currentChildren.first );
            if ( currentChildren.second != -1 )
            {

              queue.push_back( currentChildren.second );

            }

          }

          // setting current children indices to invalid indices
          currentChildren.first = -1;
          currentChildren.second = -1;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing parcel size(s) for each intermediate father by summing the
    // parcel sizes of its children
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int32_t > parcelSizes( _maximumVertexCount );
    std::vector< float > parcelSizeRatios( _maximumVertexCount );
    std::vector< int32_t > minimumParcelSizes( _maximumVertexCount );

    for ( index = 0; index < _vertexCount; index++ )
    {

      // +1 in brackets because the parcel voxel counts starts with the 
      // background label 0, while the inner label starts at 0 
      parcelSizes[ index ] = parcelVoxelCounts[ index + 1 ];
      parcelSizeRatios[ index ] = 0.0f;
      minimumParcelSizes[ index ] = parcelSizes[ index ];

    }

    for ( index = _vertexCount; index < _maximumVertexCount; index++ )
    {

      const std::pair< int32_t, int32_t >&
        currentChildren = children[ index ];

      if ( currentChildren.first != -1 )
      {

        if ( currentChildren.second != -1 )
        {

          parcelSizes[ index ] = parcelSizes[ currentChildren.first ] +
                                 parcelSizes[ currentChildren.second ];
          minimumParcelSizes[ index ] = std::min(
                                        parcelSizes[ currentChildren.first ],
                                        parcelSizes[ currentChildren.second ] );
          parcelSizeRatios[ index ] = ( float )minimumParcelSizes[ index ] /
                                      ( float )parcelSizes[ index ];

        }
        else
        {

          std::cout << "maybe a problem" << std::endl;
          parcelSizes[ index ] = parcelSizes[ currentChildren.first ];
          minimumParcelSizes[ index ] = parcelSizes[ currentChildren.first ];
          parcelSizeRatios[ index ] = ( float )1.0;

        }

      }
      else
      {

        parcelSizes[ index ] = -1;
        parcelSizeRatios[ index ] = -1.0f;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing partition of the forest
    ////////////////////////////////////////////////////////////////////////////

    // allocating queues required to create partition
    std::list< int32_t > topVertexIndexQueue;
    std::list< float > depthQueue;
    std::list< int32_t > fatherIndexQueue;

    std::vector< int32_t > newVertexIndices( _maximumVertexCount, -1 );

    // initializing queues with top vertice(s)
    std::list< int32_t >::const_iterator
      t = topVertices.begin(),
      te = topVertices.end();
    while ( t != te )
    {

      topVertexIndexQueue.push_back( *t );
      depthQueue.push_back( 0.0f );
      fatherIndexQueue.push_back( -1 );

      ++ t;

    }

    // allocating the list of too small cluster to be discarded afterwards
    std::list< int32_t > tooSmallClusters;

    // final cluster nodes
    std::vector< int32_t > nodeIndices;
    std::vector< int32_t > nodeDepths;
    std::vector< int32_t > nodeFatherIndices;

    // to be completed to speed-up calculations
    // nodeIndices.reserve( );
    // nodeDepths.reserve( );
    // nodeFatherIndices.reserve( );

    int32_t numberOfSelectedNodesInTree = 0;
    while ( !topVertexIndexQueue.empty() )
    {

      // collecting top vertex index, its depth and its father index
      int32_t topVertexIndex = topVertexIndexQueue.back();
      float depth = depthQueue.back();
      int32_t fatherIndex = fatherIndexQueue.back();

      // removing it from the queue
      topVertexIndexQueue.pop_back();
      depthQueue.pop_back();
      fatherIndexQueue.pop_back();

      bool addNode = true;
      // if cluster is too small, add it to the list of clusters to be discarded
      if ( parcelSizes[ topVertexIndex ] < isolatedClusterMinimumSize )
      {

        tooSmallClusters.push_back( topVertexIndex );
        addNode = false;

      }
      else
      {

        if ( fatherIndex == -1 )
        {

          fatherIndex = numberOfSelectedNodesInTree;

        }
        else
        {

          if ( depth >= 1.0f )
          {

            int32_t fatherCount = ( int32_t )std::count( 
                                                       fatherIndexQueue.begin(),
                                                       fatherIndexQueue.end(),
                                                       fatherIndex );
            if ( fatherCount == 0 )
            {

              fatherCount = ( int32_t )std::count( nodeFatherIndices.begin(),
                                                   nodeFatherIndices.end(),
                                                   fatherIndex );

              if ( fatherCount == 0 )
              {

                addNode = false;
                int32_t fatherIndex = std::find( newVertexIndices.begin(),
                                                 newVertexIndices.end(),
                                                 fatherIndex ) -
                                      newVertexIndices.begin();
                int32_t nodeIndex = std::find( nodeIndices.begin(),
                                               nodeIndices.end(),
                                               fatherIndex ) -
                                    nodeIndices.begin();
                nodeIndices[ nodeIndex ] = topVertexIndex;
                newVertexIndices[ topVertexIndex ] = nodeIndex;
                newVertexIndices[ fatherIndex ] = -1;
                depth -= 1.0;

              }

            }

          }

        }

        if ( addNode )
        {

          nodeIndices.push_back( topVertexIndex );
          nodeDepths.push_back( depth );
          nodeFatherIndices.push_back( fatherIndex );
          newVertexIndices[ topVertexIndex ] = numberOfSelectedNodesInTree;
          ++ numberOfSelectedNodesInTree;

        }

      }

      const std::pair< int32_t, int32_t >&
        topVertexChildren = children[ topVertexIndex ];

      if ( ( topVertexChildren.first != -1 ) &&
           ( topVertexChildren.second != -1 ) )
      {

        if ( parcelSizes[ topVertexIndex ] > averageClusterSize )
        {
 
          topVertexIndexQueue.push_back( topVertexChildren.first );
          depthQueue.push_back( depth + 1.0f );
          fatherIndexQueue.push_back( newVertexIndices[ topVertexIndex ] );

          topVertexIndexQueue.push_back( topVertexChildren.second );
          depthQueue.push_back( depth + 1.0f );
          fatherIndexQueue.push_back( newVertexIndices[ topVertexIndex ] );

        }
        else
        {

          if ( parcelSizes[ topVertexIndex ] > isolatedClusterMinimumSize )
          {

            // check whether clusters possess major components
            std::vector< int32_t > descendantVertexIndices;

            this->getDescendantVertexIndices( topVertexIndex,
                                              children,
                                              descendantVertexIndices );
            int32_t descendantVertexCount =
                                      ( int32_t )descendantVertexIndices.size();

            int32_t index = 0;
            for ( index = 0; index < descendantVertexCount; index++ )
            {

              int32_t descendantVertexIndex = descendantVertexIndices[ index ];

              float parcelSizeRatio = parcelSizeRatios[ index ];
              int32_t minimumParcelSize = minimumParcelSizes[ index ];

              if ( ( parcelSizeRatio > minimumParcelSizeRatio ) &&
                   ( minimumParcelSize >= minimumClusterSize ) )
              {

                const std::pair< int32_t, int32_t >&
                  descendantVertexChildren = children[ descendantVertexIndex ];

                if ( descendantVertexChildren.first != -1 )
                {

                  nodeIndices.push_back( descendantVertexChildren.first );
                  nodeDepths.push_back( depth + 1.0f );
                  nodeFatherIndices.push_back(
                                           newVertexIndices[ topVertexIndex ] );
                  newVertexIndices[ descendantVertexChildren.first ] =
                                                    numberOfSelectedNodesInTree;
                  ++ numberOfSelectedNodesInTree;

                }

                if ( children[ descendantVertexIndex ].second != -1 )
                {

                  nodeIndices.push_back( descendantVertexChildren.second );
                  nodeDepths.push_back( depth + 1.0f );
                  nodeFatherIndices.push_back(
                                           newVertexIndices[ topVertexIndex ] );
                  newVertexIndices[ descendantVertexChildren.second ] =
                                                    numberOfSelectedNodesInTree;
                  ++ numberOfSelectedNodesInTree;

                }

                break;

              }

            }

          }

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // compute resulting forest 
    ////////////////////////////////////////////////////////////////////////////

    // copying resulting node fathers
    std::vector< int32_t > resultingNodeFatherIndices = nodeFatherIndices;

    // computing children
    std::vector< std::pair< int32_t, int32_t > >
      resultingChildren( numberOfSelectedNodesInTree,
                         std::pair< int32_t, int32_t >( -1, -1 ) );

    for ( index = 0; index < numberOfSelectedNodesInTree; index++ )
    {

      const int32_t& father = resultingNodeFatherIndices[ index ];
      if ( father != index )
      {

        if ( resultingChildren[ father ].first == - 1 )
        {

          resultingChildren[ father ].first = index;

        }
        else
        {

          resultingChildren[ father ].second = index;

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting cluster leaves
    ////////////////////////////////////////////////////////////////////////////

    std::vector< int32_t > leafIndices;
    leafIndices.reserve( numberOfSelectedNodesInTree );
    for ( index = 0; index < numberOfSelectedNodesInTree; index++ )
    {

      if ( resultingChildren[ index ].first == -1 )
      {

        leafIndices.push_back( index );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the final labels that for each input vertex will give an output
    // label (several input vertices will have the same output label and
    // correspond to the output clusters
    ////////////////////////////////////////////////////////////////////////////


    int32_t clusterCount = ( int32_t )leafIndices.size();
    int32_t finalClusterCount = clusterCount;

    nodeClusterLabels.resize( _vertexCount + 1, 0 );

    int32_t clusterIndex = 0;
    int32_t clusterLabel = 0;
    for ( clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++ )
    {

      const int32_t& leafIndex = leafIndices[ clusterIndex ];

      if ( nodeIndices[ leafIndex ] > 0 )
      {

        std::vector< int32_t > nodeLeafIndices;
        this->getLeafIndices( nodeIndices[ leafIndex ],
                              children,
                              nodeLeafIndices );
        int32_t nodeLeafIndexCount = ( int32_t )nodeLeafIndices.size();

        int32_t nodeLeafIndex = 0;
        for ( nodeLeafIndex = 0; nodeLeafIndex < nodeLeafIndexCount;
              nodeLeafIndex++ )
        {

          int32_t vertexIndex = nodeLeafIndices[ nodeLeafIndex ];
          if ( vertexIndex < _vertexCount )
          {

            // +1 because labels for clusters start from 1
            nodeClusterLabels[ vertexIndex + 1 ] = clusterLabel + 1;

          }


        }
        ++ clusterLabel;

      }
      else
      {

        -- finalClusterCount;

      }

    }

    return finalClusterCount;

  }
  GKG_CATCH( "int32_t gkg::HierarchicalClustering::getForest( "
             "const std::vector< int32_t >& parcelVoxelCounts, "
             "int32_t averageClusterSize, "
             "int32_t isolatedClusterMinimumSize, "
             "int32_t minimumClusterSize, "
             "float minimumParcelSizeRatio, "
             "std::vector< int32_t >& nodeClusterLabels )" );

}


int32_t gkg::HierarchicalClustering::computeConnectedComponents(
                      const std::vector< std::pair< int32_t, int32_t > >& edges,
                      std::vector< int32_t >& connectedComponents ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the edge count
    ////////////////////////////////////////////////////////////////////////////

    int32_t edgeCount = ( int32_t )edges.size();


    ////////////////////////////////////////////////////////////////////////////
    // allocating connected component(s)
    ////////////////////////////////////////////////////////////////////////////

    connectedComponents.resize( _vertexCount, -1 );

    ////////////////////////////////////////////////////////////////////////////
    // at the beginning, all the vertices are candidates
    ////////////////////////////////////////////////////////////////////////////

    int32_t remainingVertexCount = _vertexCount;

    ////////////////////////////////////////////////////////////////////////////
    // initializing the component id
    ////////////////////////////////////////////////////////////////////////////

    int32_t componentId = 0;

    ////////////////////////////////////////////////////////////////////////////
    // looping over vertices
    ////////////////////////////////////////////////////////////////////////////

    while ( remainingVertexCount > 0 )
    {

      // searching for the next available component
      int32_t nextVertex = 0;
      while ( connectedComponents[ nextVertex ] != -1 )
      {

        ++ nextVertex;

      }

      // setting the component id
      connectedComponents[ nextVertex ] = componentId;

      //
      int32_t su = 0;
      int32_t sv = 1;
      while ( sv > su )
      {

        su = sv;

        int32_t nextEdge = 0;
        for ( nextEdge = 0; nextEdge < edgeCount; nextEdge++ )
        {

          if ( connectedComponents[ edges[ nextEdge ].first ] == componentId )
          {

            connectedComponents[ edges[ nextEdge ].second ] = componentId;

          }

          if ( connectedComponents[ edges[ nextEdge ].second ] == componentId )
          {

            connectedComponents[ edges[ nextEdge ].first ] = componentId;

          }

        }


        sv = 0;
        for ( nextVertex = 0; nextVertex < _vertexCount; nextVertex++ )
        {

          sv += ( connectedComponents[ nextVertex ] == componentId );

        }

      }

      remainingVertexCount -= su;

      ++ componentId;

    }

    return componentId;

  }
  GKG_CATCH( "int32_t gkg::HierarchicalClustering::computeConnectedComponents( "
             "const std::vector< std::pair< int32_t, int32_t > >& edges, "
             "std::vector< int32_t >& connectedComponents ) const" );

}


void gkg::HierarchicalClustering::computeAverageLink(
                      int32_t connectedComponentCount,
                      std::vector< std::pair< int32_t, int32_t > >& edges,
                      std::vector< float >& weights,
                      std::vector< float >& heights,
                      std::vector< int32_t >& fathers ) const
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // allocating height(s), father(s) and pop(s) and initializing them
    ////////////////////////////////////////////////////////////////////////////

    heights.resize( 2 * _vertexCount,
                    std::numeric_limits< float >::infinity() );
    fathers.resize( 2 * _vertexCount );

    std::vector< int32_t >::iterator f = fathers.begin(),
                                     fe = fathers.end();
    int32_t index = 0;
    while ( f != fe )
    {

      *f = index;
      ++ index;

      ++ f;

    }
    std::vector< int32_t > pops( 2 * _vertexCount, 1 );


    ////////////////////////////////////////////////////////////////////////////
    // declaring an invalid pair for the coordinates
    ////////////////////////////////////////////////////////////////////////////

    std::pair< int32_t, int32_t > invalidPair( -1, -1 );


    ////////////////////////////////////////////////////////////////////////////
    // collecting the edge count
    ////////////////////////////////////////////////////////////////////////////

    int32_t edgeCount = ( int32_t )edges.size();


    ////////////////////////////////////////////////////////////////////////////
    // creating a filter to get the index of the maximum value within a vector
    ////////////////////////////////////////////////////////////////////////////

    gkg::MaximumIndexFilter< std::vector< float >, int32_t > maximumIndexFilter;


    ////////////////////////////////////////////////////////////////////////////
    // computing the average link
    ////////////////////////////////////////////////////////////////////////////

    int32_t mostConnectedEdgeIndex = 0;
    std::pair< int32_t, int32_t > currentEdge;
    int32_t edgeIndex = 0;
    float fFirst = 0.0;
    float fSecond = 0.0;
    int32_t first = 0;
    int32_t second = 0;

    int32_t q = 0;
    int32_t k = 0;
    for ( q = 0; q < _vertexCount - connectedComponentCount; q++ )
    {

      // collecting the edge index corresponding to the maximum connectivity 
      // value
      maximumIndexFilter.filter( weights, mostConnectedEdgeIndex );
      k = _vertexCount + q;

      // height corresponds to the importance of the connectivity
      heights[ k ] = weights[ mostConnectedEdgeIndex ];
      currentEdge = edges[ mostConnectedEdgeIndex ];



      // removing the current edge
      edges[ mostConnectedEdgeIndex ] = invalidPair;
      weights[ mostConnectedEdgeIndex ] =
                                      -std::numeric_limits< float >::infinity();


      // looping over edges and discarding the edge corresponding to the
      // symmetrical edge
      for ( edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++ )
      {

        if ( ( edges[ edgeIndex ].first == currentEdge.second ) &&
             ( edges[ edgeIndex ].second == currentEdge.first ) )
        {

          edges[ edgeIndex ] = invalidPair;
          weights[ edgeIndex ] = -std::numeric_limits< float >::infinity();

        }

        if ( ( edges[ edgeIndex ].first == currentEdge.first ) &&
             ( edges[ edgeIndex ].second == currentEdge.second ) )
        {

          edges[ edgeIndex ] = invalidPair;
          weights[ edgeIndex ] = -std::numeric_limits< float >::infinity();

        }

      }

      // merge the edges with third-part edges
      fathers[ currentEdge.first ] = k;
      fathers[ currentEdge.second ] = k;

      pops[ k ] = pops[ currentEdge.first ] + pops[ currentEdge.second ];

      fFirst = ( float )pops[ currentEdge.first ] / ( float )pops[ k ];
      fSecond = 1.0 - fFirst;

      // replace first or second coordinate by k
      for ( edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++ )
      {

        if ( edges[ edgeIndex ].first == currentEdge.first )
        {

          weights[ edgeIndex ] *= fFirst;
          edges[ edgeIndex ].first = k;

        }
        if ( edges[ edgeIndex ].second == currentEdge.first )
        {

          weights[ edgeIndex ] *= fFirst;
          edges[ edgeIndex ].second = k;

        }
        if ( edges[ edgeIndex ].first == currentEdge.second )
        {

          weights[ edgeIndex ] *= fSecond;
          edges[ edgeIndex ].first = k;

        }
        if ( edges[ edgeIndex ].second == currentEdge.second )
        {

          weights[ edgeIndex ] *= fSecond;
          edges[ edgeIndex ].second = k;

        }

      }

      // sorting edge according to their second coordinate
      // catch every edge index for which first is equal to k
      std::list< int32_t > edgeWithFirstEqualToKIndexList;

      for ( edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++ )
      {

        if ( edges[ edgeIndex ].first == k )
        {

          edgeWithFirstEqualToKIndexList.push_back( edgeIndex );

        }

      }
      int32_t edgeWithFirstEqualToKCount =
                               ( int32_t )edgeWithFirstEqualToKIndexList.size();

      std::vector< int32_t >
        edgeWithFirstEqualToKIndices( edgeWithFirstEqualToKCount );
      std::copy( edgeWithFirstEqualToKIndexList.begin(),
                 edgeWithFirstEqualToKIndexList.end(),
                 edgeWithFirstEqualToKIndices.begin() );
      
      std::vector< int32_t > seconds( edgeWithFirstEqualToKCount );
      std::vector< std::pair< int32_t, const std::vector< int32_t >* > >
        edgeIndices( edgeWithFirstEqualToKCount );

      std::vector< int32_t >::const_iterator
        e = edgeWithFirstEqualToKIndices.begin(),
        ee = edgeWithFirstEqualToKIndices.end();
      std::vector< int32_t >::iterator s = seconds.begin();
      std::vector< std::pair< int32_t,
                              const std::vector< int32_t >* > >::iterator
        i = edgeIndices.begin();
      edgeIndex = 0;
      while ( e != ee )
      {

        *s = edges[ *e ].second;
        i->first = edgeIndex;
        i->second = &seconds;

        ++ edgeIndex;
        ++ s;
        ++ e;
        ++ i;

      }

      std::vector< int32_t > copyOfSeconds = seconds;

      qsort( &copyOfSeconds[ 0 ], edgeWithFirstEqualToKCount, sizeof( int32_t ),
             coordinateCompareFunctor );

      qsort( &edgeIndices[ 0 ], edgeWithFirstEqualToKCount,
             sizeof( std::pair< int32_t, const std::vector< int32_t >* > ),
             edgeIndicesCompareFunctor );

      for ( edgeIndex = 0; edgeIndex < edgeWithFirstEqualToKCount -1;
            edgeIndex++ )
      {

        if ( copyOfSeconds[ edgeIndex ] == copyOfSeconds[ edgeIndex + 1 ] )
        {

          first = edgeWithFirstEqualToKIndices[
                                               edgeIndices[ edgeIndex ].first ];
          second = edgeWithFirstEqualToKIndices[
                                           edgeIndices[ edgeIndex + 1 ].first ];

          weights[ first ] += weights[ second ];
          weights[ second ] = -std::numeric_limits< float >::infinity();

          edges[ second ] = invalidPair;

        }

      }

      // sorting edge according to their first coordinate
      // catch every edge index for which second is equal to k
      std::list< int32_t > edgeWithSecondEqualToKIndexList;

      for ( edgeIndex = 0; edgeIndex < edgeCount; edgeIndex++ )
      {

        if ( edges[ edgeIndex ].second == k )
        {

          edgeWithSecondEqualToKIndexList.push_back( edgeIndex );

        }

      }
      int32_t edgeWithSecondEqualToKCount =
                              ( int32_t )edgeWithSecondEqualToKIndexList.size();

      std::vector< int32_t >
        edgeWithSecondEqualToKIndices( edgeWithSecondEqualToKCount );
      std::copy( edgeWithSecondEqualToKIndexList.begin(),
                 edgeWithSecondEqualToKIndexList.end(),
                 edgeWithSecondEqualToKIndices.begin() );
      
      std::vector< int32_t > firsts( edgeWithSecondEqualToKCount );
      edgeIndices.resize( edgeWithSecondEqualToKCount );

      e = edgeWithSecondEqualToKIndices.begin(),
      ee = edgeWithSecondEqualToKIndices.end();
      std::vector< int32_t >::iterator f = firsts.begin();
      i = edgeIndices.begin();
      edgeIndex = 0;
      while ( e != ee )
      {

        *f = edges[ *e ].first;
        i->first = edgeIndex;
        i->second = &firsts;

        ++ edgeIndex;
        ++ f;
        ++ e;
        ++ i;

      }

      std::vector< int32_t > copyOfFirsts = firsts;

      qsort( &copyOfFirsts[ 0 ], edgeWithSecondEqualToKCount, sizeof( int32_t ),
             coordinateCompareFunctor );
      qsort( &edgeIndices[ 0 ], edgeWithSecondEqualToKCount,
             sizeof( std::pair< int32_t, const std::vector< int32_t >* > ),
             edgeIndicesCompareFunctor );

      for ( edgeIndex = 0; edgeIndex < edgeWithSecondEqualToKCount -1;
            edgeIndex++ )
      {

        if ( copyOfFirsts[ edgeIndex ] == copyOfFirsts[ edgeIndex + 1 ] )
        {

          first = edgeWithSecondEqualToKIndices[
                                           edgeIndices[ edgeIndex ].first ];
          second = edgeWithSecondEqualToKIndices[
                                           edgeIndices[ edgeIndex + 1 ].first ];

          weights[ first ] += weights[ second ];
          weights[ second ] = -std::numeric_limits< float >::infinity();

          edges[ second ] = invalidPair;

        }

      }

    }

    int32_t h = 0;
    for ( h = 0; h < _maximumVertexCount; h++ )
    {

      if ( heights[ h ] < 0.0 )
      {

        heights[ h ] = 0.0;

      }
      else if ( heights[ h ] == std::numeric_limits< float >::infinity() )
      {

        heights[ h ] = heights[ _vertexCount ] + 1;

      } 

    }

  }
  GKG_CATCH( "void gkg::HierarchicalClustering::computeAverageLink( "
             "int32_t connectedComponentCount, "
             "std::vector< std::pair< int32_t, int32_t > >& edges, "
             "std::vector< float >& weights, "
             "std::vector< float >& heights, "
             "std::vector< int32_t >& fathers ) const" );

}


void gkg::HierarchicalClustering::getDescendantVertexIndices(
                   int32_t topVertexIndex,
                   const std::vector< std::pair< int32_t, int32_t > >& children,
                   std::vector< int32_t >& descendantVertexIndices ) const
{

  try
  {

    std::list< int32_t > descendantVertexIndexList;

    descendantVertexIndexList.push_back( topVertexIndex );

    std::list< int32_t > queue;
    queue.push_back( topVertexIndex );

    while ( !queue.empty() )
    {

      const int32_t& vertexIndex = queue.back();
      queue.pop_back();

      const std::pair< int32_t, int32_t >&
        vertexChildren = children[ vertexIndex ];

      if ( vertexChildren.first != -1 )
      {

        descendantVertexIndexList.push_back( vertexChildren.first );

        queue.push_back( vertexChildren.first );

      }

      if ( vertexChildren.second != -1 )
      {

        descendantVertexIndexList.push_back( vertexChildren.second );

        queue.push_back( vertexChildren.second );

      }

    }

    descendantVertexIndices.resize( descendantVertexIndexList.size() );
    std::copy( descendantVertexIndexList.begin(),
               descendantVertexIndexList.end(),
               descendantVertexIndices.begin() );


  }
  GKG_CATCH( "void gkg::HierarchicalClustering::getDescendantVertexIndices( "
             "int32_t topVertexIndex, "
             "const std::vector< std::pair< int32_t, int32_t > >& children, "
             "std::vector< int32_t >& descendantVertexIndices ) const" );

}


void gkg::HierarchicalClustering::getLeafIndices(
                   int32_t topVertexIndex,
                   const std::vector< std::pair< int32_t, int32_t > >& children,
                   std::vector< int32_t >& leafIndices ) const
{

  try
  {

    std::list< int32_t > leafIndexList;

    leafIndexList.push_back( topVertexIndex );

    std::list< int32_t > queue;
    queue.push_back( topVertexIndex );

    while ( !queue.empty() )
    {

      const int32_t& vertexIndex = queue.back();
      queue.pop_back();

      const std::pair< int32_t, int32_t >&
        vertexChildren = children[ vertexIndex ];

      if ( vertexChildren.first != -1 )
      {

        leafIndexList.push_back( vertexChildren.first );
        queue.push_back( vertexChildren.first );

      }

      if ( vertexChildren.second != -1 )
      {

        leafIndexList.push_back( vertexChildren.second );
        queue.push_back( vertexChildren.second );

      }

    }

    leafIndices.resize( leafIndexList.size() );
    std::copy( leafIndexList.begin(),
               leafIndexList.end(),
               leafIndices.begin() );

  }
  GKG_CATCH( "void gkg::HierarchicalClustering::getDescendantVertexIndices( "
             "int32_t topVertexIndex, "
             "const std::vector< std::pair< int32_t, int32_t > >& children, "
             "std::vector< int32_t >& descendantVertexIndices ) const" );

}


int32_t gkg::HierarchicalClustering::coordinateCompareFunctor( const void *a,
                                                               const void *b )
{

  try
  {

    return (  *( const int32_t* )a - *( const int32_t* )b );

  }
  GKG_CATCH( "int32_t gkg::HierarchicalClustering::coordinateCompareFunctor( "
             "const void *a, const void *b )" );

}


int32_t gkg::HierarchicalClustering::edgeIndicesCompareFunctor( const void *a,
                                                                const void *b )
{

  try
  {

    const std::pair< int32_t, const std::vector< int32_t >* >*
      A = ( const std::pair< int32_t, const std::vector< int32_t >* >* )a;
    const std::pair< int32_t, const std::vector< int32_t >* >*
      B = ( const std::pair< int32_t, const std::vector< int32_t >* >* )b;

    return ( ( *( A->second ) )[ A->first ] -
             ( *( B->second ) )[ B->first ] );

  }
  GKG_CATCH( "int32_t gkg::HierarchicalClustering::edgeIndicesCompareFunctor( "
             "const void *a, const void *b )" );

}
