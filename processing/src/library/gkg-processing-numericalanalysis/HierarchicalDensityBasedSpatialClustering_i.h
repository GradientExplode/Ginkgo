#include <gkg-processing-numericalanalysis/HierarchicalDensityBasedSpatialClustering.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <map>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <iostream>


#define NOISE        -2
#define UNCLASSIFIED -1





////////////////////////////////////////////////////////////////////////////////
// class CoreDistanceContext
////////////////////////////////////////////////////////////////////////////////


template < class I >
inline
gkg::CoreDistanceContext< I >::CoreDistanceContext(
                             gkg::CoreDistanceGauge& gauge,
                             const std::vector< I >& items,
                             const int32_t& neighborCount,
                             const gkg::DistanceOperator< I >& distanceOperator,
                             std::vector< double >& coreDistances,
                             const bool& verbose )
                              : gkg::LoopContext< int32_t >( &gauge ),
                                _items( items ),
                                _neighborCount( neighborCount ),
                                _distanceOperator( distanceOperator ),
                                _coreDistances( coreDistances ),
                                _verbose( verbose )
{

  try
  {


    // computing the gauge update period, eg 1% of the total number of items
    _gaugeUpdatePeriod = ( int32_t )( 0.01f * ( float )items.size() );


  }
  GKG_CATCH( "template < class I > "
             "inline "
             "gkg::CoreDistanceContext< I >::CoreDistanceContext( "
             "gkg::CoreDistanceGauge& gauge, "
             "const std::vector< I >& items, "
             "const int32_t& neighborCount, "
             "const gkg::DistanceOperator< I >& distanceOperator, "
             "std::vector< double >& coreDistances, "
             "const bool& verbose )" );


}


template < class I >
inline
void gkg::CoreDistanceContext< I >::doIt( int32_t startIndex,
                                          int32_t count )
{

  try
  {

    int32_t itemCount = ( int32_t )_items.size();
    std::vector< double > localCoreDistances( count, 0.0 );
    int32_t i = 0;
    int32_t n = 0;

    for ( i = 0; i < count; i++ )
    {


      if ( _verbose && _gauge )
      {
      
        if ( i && !( i % _gaugeUpdatePeriod ) )
        {

          lock();
          _gauge->add( _gaugeUpdatePeriod );
          unlock();

        }
      
      }

      std::vector< double > 
        kNNDistances( _neighborCount,
                      std::numeric_limits< double >::max() );

      for ( n = 0; n < itemCount; n++ )
      {

        if ( n == ( startIndex + i ) )
        {

          continue;

        }

        double distance = _distanceOperator.get( _items[ startIndex + i ],
                                                 _items[ n ] );


        int32_t neighborIndex = _neighborCount;
        // checking at which position in the nearest distances the current
        // distance would fit
        while ( ( neighborIndex >= 1 ) &&
                distance < kNNDistances[ neighborIndex - 1 ] )
        {

          -- neighborIndex;

        }

        // shifting elements in the array to make room for the current 
        // distance
        if ( neighborIndex < _neighborCount )
        {

          int32_t shiftIndex = 0;
          for ( shiftIndex = _neighborCount - 1;
                shiftIndex > neighborIndex; shiftIndex-- )
          {

            kNNDistances[ shiftIndex ] = kNNDistances[ shiftIndex - 1 ];

          }
          kNNDistances[ neighborIndex ] = distance;

        }

      }
      localCoreDistances[ i ] = kNNDistances[ _neighborCount - 1 ];

    }

    // now locking to fill the output structures
    lock();

    for ( i = 0; i < count; i++ )
    {

      _coreDistances[ startIndex + i ] = localCoreDistances[ i ];

    }

    unlock();

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::CoreDistanceContext::doIt( "
             "int32_t startIndex, int32_t count )" );


}

////////////////////////////////////////////////////////////////////////////////
// class MutualReachabilityContext
////////////////////////////////////////////////////////////////////////////////


template < class I >
inline
gkg::MutualReachabilityContext< I >::MutualReachabilityContext(
                             const int32_t& currentItem,
                             const std::vector< I >& items,
                             const gkg::BitSet& attachedItems,
                             const gkg::DistanceOperator< I >& distanceOperator,
                             const std::vector< double >& coreDistances,
                             std::vector< double >& nearestMRDDistances,
                             std::vector< int32_t >& nearestMRDNeighbors,
                             double& nearestMRDDistance,
                             int32_t& nearestMRDItem )
                                    : gkg::LoopContext< int32_t >( 0 ),
                                      _currentItem( currentItem ),
                                      _items( items ),
                                      _attachedItems( attachedItems ),
                                      _distanceOperator( distanceOperator ),
                                      _coreDistances( coreDistances ),
                                      _nearestMRDDistances(
                                                          nearestMRDDistances ),
                                      _nearestMRDNeighbors(
                                                          nearestMRDNeighbors ),
                                      _nearestMRDDistance( nearestMRDDistance ),
                                      _nearestMRDItem( nearestMRDItem )
{
}


template < class I >
inline
void gkg::MutualReachabilityContext< I >::doIt( int32_t startIndex,
                                                int32_t count )
{

  try
  {

    std::vector< double > localNearestMRDDistances( count );
    std::vector< int32_t > localNearestMRDNeighbors( count );
    int32_t n = 0;
    double distance = 0.0;
    double mutualReachabilityDistance = 0.0;


    for ( n = 0; n < count; n++ )
    {


      localNearestMRDDistances[ n ] = std::numeric_limits< double >::max();

    }

    for ( n = 0; n < count; n++ )
    {

      localNearestMRDDistances[ n ] = _nearestMRDDistances[ startIndex + n ];
      localNearestMRDNeighbors[ n ] = _nearestMRDNeighbors[ startIndex + n ];

      if ( _currentItem == startIndex + n )
      {

        continue;

      }
      if ( _attachedItems.get( startIndex + n ) )
      {

        continue;

      }

      distance = _distanceOperator.get( _items[ _currentItem ],
                                        _items[ startIndex + n ] );
      mutualReachabilityDistance = distance;

      if ( _coreDistances[ _currentItem ] > mutualReachabilityDistance )
      {

        mutualReachabilityDistance = _coreDistances[ _currentItem ];

      }

      if ( _coreDistances[ startIndex + n ] > mutualReachabilityDistance )
      {

        mutualReachabilityDistance = _coreDistances[ startIndex + n ];

      }

      if ( mutualReachabilityDistance < localNearestMRDDistances[ n ] )
      {

        localNearestMRDDistances[ n ] = mutualReachabilityDistance;
        localNearestMRDNeighbors[ n ] = _currentItem;

      }

    }

    // now locking to fill the output structures
    lock();

    for ( n = 0; n < count; n++ )
    {

      _nearestMRDDistances[ startIndex + n ] = localNearestMRDDistances[ n ];
      _nearestMRDNeighbors[ startIndex + n ] = localNearestMRDNeighbors[ n ];

      if ( _currentItem == startIndex + n )
      {

        continue;

      }
      if ( _attachedItems.get( startIndex + n ) )
      {

        continue;

      }

      if ( localNearestMRDDistances[ n ] <= _nearestMRDDistance )
      {

        _nearestMRDDistance = localNearestMRDDistances[ n ];
        _nearestMRDItem = startIndex + n;

      }

    }

    unlock();

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::MutualReachabilityContext::doIt( "
             "int32_t startIndex, int32_t count )" );


}

////////////////////////////////////////////////////////////////////////////////
// class HierarchicalDensityBasedSpatialClustering
////////////////////////////////////////////////////////////////////////////////

template < class I >
inline
gkg::HierarchicalDensityBasedSpatialClustering< I >::
                                      HierarchicalDensityBasedSpatialClustering(
                             const std::vector< I >& items,
                             const gkg::DistanceOperator< I >& distanceOperator,
                             int32_t minimumPointCount,
                             int32_t minimumClusterSize,
                             const std::vector< gkg::Constraint >* constraints,
                             bool verbose )
                                               :  _minimumPointCount(
                                                            minimumPointCount ),
                                                  _minimumClusterSize(
                                                           minimumClusterSize ),
                                                  _labels( items.size(),
                                                           UNCLASSIFIED )
{


  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    int32_t itemCount = ( int32_t )items.size();
    if ( itemCount < 2 )
    {

      throw std::runtime_error( "need at least 2 samples" );

    }

    if ( _minimumPointCount < 1 )
    {

      throw std::runtime_error( "minimum point count must be at least 1" );

    }

    if ( _minimumClusterSize < 1 )
    {

      throw std::runtime_error( "minimum cluster size must be at least 1" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the core distance(s) for each point in the data set, given some
    // value for k; each point's core distance will be it's distance to the kth 
    // nearest neighbor
    ////////////////////////////////////////////////////////////////////////////

    int32_t neighborCount = _minimumPointCount - 1;
    std::vector< double > coreDistances( itemCount, 0.0 );

    if ( _minimumPointCount > 1 )
    {


      if ( verbose )
      {

        std::cout << "computing core distance(s)" << std::flush;

      }

      // creating threaded gauge
      gkg::CoreDistanceGauge
        coreDistanceGauge( itemCount );

      // creating threaded loop context
      gkg::CoreDistanceContext< I > coreDistanceContext( coreDistanceGauge,
                                                         items,
                                                         neighborCount,
                                                         distanceOperator,
                                                         coreDistances,
                                                         verbose );

      // creating a threaded loop
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &coreDistanceContext,
                      0U,
                      itemCount );
      // launching the threaded loop
      threadedLoop.launch();

      if ( verbose )
      {

        std::cout << gkg::Eraser( 64 ) << std::flush;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the minimum spanning tree between the items from their 
    // distances
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "preparing MST" << std::flush;

    }

    int32_t i = 0;
    int32_t selfEdgeCapacity = itemCount;
    gkg::BitSet attachedItems;

    std::vector< double >
      nearestMRDDistances( itemCount - 1 + selfEdgeCapacity );
    std::vector< int32_t >
      nearestMRDNeighbors( itemCount - 1 + selfEdgeCapacity );

    for ( i = 0; i < itemCount - 1; i++ )
    {

      nearestMRDDistances[ i ] = std::numeric_limits< double >::max();

    }

    int32_t currentItem = itemCount - 1;
    int32_t attachedItemCount = 1;
    attachedItems.set( currentItem );


    if ( verbose )
    {

      std::cout << " [ " << std::setw( 15 ) << 0 
                << " / " << std::setw( 15 ) << itemCount
                << " ]" << std::flush;

    }

    int32_t nearestMRDItem = -1;
    double nearestMRDDistance = 0.0;

    int32_t displayIncrement = std::max( itemCount / 1000, 100 );
    while ( attachedItemCount < itemCount )
    {

      
      if ( verbose )
      {

        if ( ( ( attachedItemCount + 1 ) % displayIncrement == 0 ) ||
             ( ( attachedItemCount + 1 ) == itemCount ) )
        {

          std::cout << gkg::Eraser( 38 ) << std::flush;
          std::cout << " [ " << std::setw( 15 ) << attachedItemCount + 1 
                    << " / " << std::setw( 15 ) << itemCount
                    << " ]" << std::flush;


        }

      }

      nearestMRDItem = -1;
      nearestMRDDistance = std::numeric_limits< double >::max();


      // creating threaded loop context
      gkg::MutualReachabilityContext< I > 
        mutualReachabilityContext( currentItem,
                                   items,
                                   attachedItems,
                                   distanceOperator,
                                   coreDistances,
                                   nearestMRDDistances,
                                   nearestMRDNeighbors,
                                   nearestMRDDistance,
                                   nearestMRDItem );

      // creating a threaded loop
      gkg::ThreadedLoop< int32_t >
        threadedLoop( &mutualReachabilityContext,
                      0U,
                      itemCount );
      // launching the threaded loop
      threadedLoop.launch();

      attachedItems.set( nearestMRDItem );
      ++ attachedItemCount;
      currentItem = nearestMRDItem;

    }

    std::vector< int32_t > 
      otherVertexIndices( itemCount - 1 + selfEdgeCapacity );
    for ( i = 0; i < itemCount - 1; i++ )
    {

      otherVertexIndices[ i ] = i;

    }
    for ( i = itemCount - 1; i < itemCount * 2 - 1; i++ )
    {

      int32_t vertex = i - ( itemCount - 1 );
      nearestMRDNeighbors[ i ] = vertex;
      otherVertexIndices[ i ] = vertex;
      nearestMRDDistances[ i ] = coreDistances[ vertex ];

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 38 + 13 ) << std::flush;

    }



    if ( verbose )
    {

      std::cout << "computing MST" << std::flush;

    }

    gkg::UndirectedGraph mst( itemCount,
                              nearestMRDNeighbors,
                              otherVertexIndices,
                              nearestMRDDistances );
    mst.quickSortByEdgeWeight();


    if ( verbose )
    {

      std::cout << gkg::Eraser( 13 ) << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing hierarchy and cluster tree
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing cluster tree" << std::flush;

    }

    std::vector< double > itemNoiseLevels( itemCount );
    std::vector< int32_t > itemLastClusters( itemCount );
    std::vector< std::vector< int32_t > > hierarchy;

    std::vector< gkg::Cluster* > clusters;

    // allocating the previous and current cluster count of each point in the 
    // data set
    std::vector< int32_t > previousClusterLabels( mst.getVertexCount() );
    std::vector< int32_t > currentClusterLabels( mst.getVertexCount() );

    for ( i = 0; i < ( int32_t )currentClusterLabels.size(); i++ )
    {

      previousClusterLabels[ i ] = 1;
      currentClusterLabels[ i ] = 1;

    }

    int32_t clusterCounter = 0;
    clusters.push_back( 0 );
    clusters.push_back( new gkg::Cluster(
                                     clusterCounter,
                                     1,
                                     0,
                                     std::numeric_limits< double >::quiet_NaN(),
                                     mst.getVertexCount() ) );

    std::set< int32_t > clusterOne;
    clusterOne.insert( 1 );

    if ( constraints )
    {

      this->calculateConstraintsSatisfiedCount( *constraints,
                                                clusterOne,
                                                clusters,
                                                currentClusterLabels );

    }

    std::set< int32_t > affectedClusterLabels;
    std::set< int32_t > affectedVertices;

    int32_t hierarchyPosition = 0;
    int32_t nextClusterLabel = 2;
    bool isNextLevelSignificant = true;
    int32_t firstVertex = 0;
    int32_t secondVertex = 0;

    // the current edge being removed from the MST
    int32_t currentEdgeIndex = mst.getEdgeCount() - 1;
    double currentEdgeWeight = 0.0;
    int32_t examinedClusterLabel = 0;
    std::set< int32_t >::iterator previousLabel;
    while (currentEdgeIndex >= 0)
    {

      currentEdgeWeight = mst.getEdgeWeightAtIndex( currentEdgeIndex );
      std::vector< gkg::Cluster* > newClusters;

      while ( ( currentEdgeIndex >= 0 ) &&
              ( mst.getEdgeWeightAtIndex( currentEdgeIndex ) ==
                currentEdgeWeight ) )
      {

        firstVertex = mst.getFirstVertexAtIndex( currentEdgeIndex );
        secondVertex = mst.getSecondVertexAtIndex( currentEdgeIndex );

        mst.removeEdgeFromList( secondVertex, firstVertex );
        mst.removeEdgeFromList( firstVertex, secondVertex );

        if ( currentClusterLabels[ firstVertex ] == 0 )
        {

          -- currentEdgeIndex;
          continue;

        }

        affectedVertices.insert( firstVertex );
        affectedVertices.insert( secondVertex );
        affectedClusterLabels.insert( currentClusterLabels[ firstVertex ] );
        -- currentEdgeIndex;

      }
      if ( affectedClusterLabels.empty() )
      {

        continue;

      }

      while ( !affectedClusterLabels.empty() )
      {

        previousLabel = std::prev( affectedClusterLabels.end() );
        examinedClusterLabel = *previousLabel;
        affectedClusterLabels.erase( previousLabel );
        std::set< int32_t > examinedVertices;
        std::set< int32_t >::iterator av;
        for ( av = affectedVertices.begin();
              av != affectedVertices.end(); )
        {

          int32_t affectedVertex = *av;
          if ( currentClusterLabels[ affectedVertex ] == examinedClusterLabel )
          {

            examinedVertices.insert( affectedVertex );
            av = affectedVertices.erase( av );

          }
          else
          {

            ++ av;

          }

        }
        std::set< int32_t > firstChildCluster;
        std::list< int32_t > unexploredFirstChildClusterItems;
        int32_t childClusterCount = 0;
        while ( !examinedVertices.empty() )
        {

          std::set< int32_t > constructingSubCluster;
          std::list< int32_t > unexploredSubClusterItems;
          bool anyEdges = false;
          bool incrementedChildCount = false;
          std::set< int32_t >::iterator
            ev = std::prev( examinedVertices.end() );
          int32_t rootVertex = *ev;
          constructingSubCluster.insert( rootVertex );
          unexploredSubClusterItems.push_back( rootVertex );
          examinedVertices.erase( ev );
          while ( !unexploredSubClusterItems.empty())
          {

            int32_t vertexToExplore = *unexploredSubClusterItems.begin();
            unexploredSubClusterItems.erase(
                                            unexploredSubClusterItems.begin() );
            const std::vector< int32_t >&
              vertexToExploreEdgeList = mst.getEdgeListForVertex(
                                                              vertexToExplore );
            std::vector< int32_t >::const_iterator it;
            for ( it = vertexToExploreEdgeList.begin();
                  it != vertexToExploreEdgeList.end(); )
            {

              int32_t neighbor = *it;
              anyEdges = true;
              if ( constructingSubCluster.find( neighbor ) ==
                   constructingSubCluster.end() )
              {

                constructingSubCluster.insert( neighbor );
                unexploredSubClusterItems.push_back( neighbor );
                std::set< int32_t >::iterator
                  evn = examinedVertices.find( neighbor );
                if ( evn != examinedVertices.end() )
                {

                  examinedVertices.erase( evn );

                }

              }
              else
              {

                ++ it;

              }

            }
            if ( !incrementedChildCount &&
                 ( ( int32_t )constructingSubCluster.size() >=
                   _minimumClusterSize ) &&
                 anyEdges )
            {

              incrementedChildCount = true;
              ++ childClusterCount;

              // if this is the first valid child cluster, stop exploring it
              if ( firstChildCluster.empty() )
              {

                firstChildCluster = constructingSubCluster;
                unexploredFirstChildClusterItems = unexploredSubClusterItems;
                break;

              }

            }

          }

          // if there could be a split, and this child cluster is valid
          if ( ( childClusterCount >= 2 ) &&
               ( ( int32_t )constructingSubCluster.size() >= 
                 _minimumClusterSize ) &&
               anyEdges )
          {

            // checking this child cluster is not equal to the unexplored first
            // child cluster
            int32_t 
              firstChildClusterMember = *std::prev( firstChildCluster.end() );
            if ( constructingSubCluster.find( firstChildClusterMember ) !=
                 constructingSubCluster.end() )
            {

              -- childClusterCount;

            }
            // otherwise, creating a new cluster
            else
            {

              gkg::Cluster* newCluster = this->createNewCluster(
                                               clusterCounter,
                                               constructingSubCluster,
                                               currentClusterLabels,
                                               clusters[ examinedClusterLabel ],
                                               nextClusterLabel,
                                               currentEdgeWeight );

              newClusters.push_back( newCluster );
              clusters.push_back( newCluster );
              ++ nextClusterLabel;

            }

          }
          else if ( ( ( int32_t )constructingSubCluster.size() < 
                      _minimumClusterSize ) ||
                    !anyEdges )
          {

            this->createNewCluster( clusterCounter,
                                    constructingSubCluster,
                                    currentClusterLabels,
                                    clusters[ examinedClusterLabel ],
                                    0,
                                    currentEdgeWeight );

            std::set< int32_t >::iterator it;
            for ( it = constructingSubCluster.begin();
                  it != constructingSubCluster.end(); it++ )
            {

              int32_t item = *it;
              itemNoiseLevels[ item ] = currentEdgeWeight;
              itemLastClusters[ item ] = examinedClusterLabel;

            }

          }

        }

        if ( ( childClusterCount >= 2 ) &&
             ( currentClusterLabels[ *firstChildCluster.begin() ] ==
               examinedClusterLabel ) )
        {

          while ( !unexploredFirstChildClusterItems.empty())
          {

            int32_t vertexToExplore = *unexploredFirstChildClusterItems.begin();
            unexploredFirstChildClusterItems.pop_front();
            std::vector< int32_t >::const_iterator it;
            for ( it = mst.getEdgeListForVertex( vertexToExplore ).begin();
                  it != mst.getEdgeListForVertex( vertexToExplore ).end(); it++)
            {

              int32_t neighbor = *it;
              if ( firstChildCluster.find( neighbor ) ==
                   firstChildCluster.end() )
              {

                firstChildCluster.insert( neighbor );
                unexploredFirstChildClusterItems.push_back( neighbor );

              }

            }

          }
          gkg::Cluster* newCluster = this->createNewCluster(
                                               clusterCounter,
                                               firstChildCluster,
                                               currentClusterLabels,
                                               clusters[ examinedClusterLabel ],
                                               nextClusterLabel,
                                               currentEdgeWeight );
          newClusters.push_back( newCluster );
          clusters.push_back( newCluster );
          ++ nextClusterLabel;

        }

      }


      if ( isNextLevelSignificant || !newClusters.empty() )
      {

        std::vector< int32_t > lineContents( previousClusterLabels.size() );
        for ( i = 0; i < ( int32_t )previousClusterLabels.size(); i++ )
        {

          lineContents[ i ] = previousClusterLabels[ i ];

        }
        hierarchy.push_back( lineContents );
        ++ hierarchyPosition;

      }
      std::set< int32_t > newClusterLabels;

      std::vector< gkg::Cluster* >::iterator
        nc = newClusters.begin(),
        nce = newClusters.end();
      while ( nc != nce )
      {

        gkg::Cluster* newCluster = *nc;
        newCluster->setHierarchyPosition( hierarchyPosition );
        newClusterLabels.insert( newCluster->getLabel() );

        ++ nc;

      }
      if ( !newClusterLabels.empty() && constraints )
      {

        this->calculateConstraintsSatisfiedCount( *constraints,
                                                  newClusterLabels,
                                                  clusters, 
                                                  currentClusterLabels );

      }

      for ( i = 0; i < ( int32_t )previousClusterLabels.size(); i++ )
      {

        previousClusterLabels[ i ] = currentClusterLabels[ i ];

      }
      if ( newClusters.empty() )
      {

        isNextLevelSignificant = false;

      }
      else
      {

        isNextLevelSignificant = true;

      }

    }

    std::vector< int32_t > lineContents( previousClusterLabels.size() + 1U, 0);
    hierarchy.push_back( lineContents );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 22 ) << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // propagating tree
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "propagating tree" << std::flush;

    }

    this->propagateTree( clusters );


    if ( verbose )
    {

      std::cout << gkg::Eraser( 16 ) << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // collecting prominent cluster(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "collecting prominent cluster(s)" << std::flush;

    }

    this->findProminentClusters( clusters, hierarchy );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 31 ) << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing membership probabilities
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing membership probabilities" << std::flush;

    }

    _membershipProbabilities.resize( itemCount,
                                     std::numeric_limits< double >::max() );
    int32_t j = 0;
    i = 0;
    while ( i < itemCount )
    {

      if ( _membershipProbabilities[ i ] ==
           std::numeric_limits< double >::max() )
      {

        int32_t clusterId = _labels[ i ];
        std::vector< int32_t >::iterator
          it = _labels.begin() + i;
        std::vector< int32_t > indices;
        while ( ( it = std::find( it, _labels.end(), clusterId ) ) !=
                _labels.end() )
        {

          indices.push_back( std::distance( _labels.begin(), it ) );
          ++ it;
          if ( it == _labels.end() )
          {

            break;

          }

        }

        if ( clusterId <= 0 )
        {

          for ( j = 0; j < ( int32_t )indices.size(); j++ )
          {

            _membershipProbabilities[ indices[ j ] ] = 0;

          }
          ++ i;
          continue;

        }

        std::vector< double > tempCoreDistances( indices.size() );
        for ( j = 0; j < ( int32_t )indices.size(); j++ )
        {

          tempCoreDistances[ j ] = coreDistances[ j ];

        }
        double maximumCoreDistance = *max_element( tempCoreDistances.begin(),
                                                   tempCoreDistances.end() );
        for ( j = 0; j < ( int32_t )tempCoreDistances.size(); j++ )
        {

	  _membershipProbabilities[ indices[ j ] ] =
                              ( maximumCoreDistance - tempCoreDistances[ j ] ) /
                              maximumCoreDistance;

        }

      }

      ++ i;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 34 ) << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing outlier score(s) (produces the outlier score for each point in 
    // the data set, and returns a sorted list of outlier scores
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing outlier score(s)" << std::flush;

    }

    _outlierScores.reserve( itemCount );
    for ( i = 0; i < itemCount; i++ )
    {

      double epsilonMax = clusters[ itemLastClusters[ i ] ]->
                                           getPropagatedLowestChildDeathLevel();

      double epsilon = itemNoiseLevels[ i ];

      double score = 0;

      if ( epsilon != 0.0 )
      {

        score = 1 - ( epsilonMax / epsilon );

      }
      _outlierScores.push_back( gkg::OutlierScore( score,
                                                   coreDistances[ i ],
                                                   i ) );

    }
    // sorting the outlier scores
    std::sort( _outlierScores.begin(), _outlierScores.end() );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 26 ) << std::flush;

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the number of outlying items and the number of cluster(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "computing outlier and cluster count(s)" << std::flush;

    }

    std::set< int32_t > clusterSet;
    _noisyItemCount = 0U;
    for ( i = 0; i < itemCount; i++ )
    {

      if ( _labels[ i ] == UNCLASSIFIED )
      {

        ++ _noisyItemCount;

      }
      else
      {

        clusterSet.insert( _labels[ i ] );

      }

    }
    _clusterCount = clusterSet.size();

    if ( verbose )
    {

      std::cout << gkg::Eraser( 38 ) << std::flush;

    }


    ////////////////////////////////////////////////////////////////////////////
    // normalizing label(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "normalizing label(s)" << std::flush;

    }
    std::map< int32_t, int32_t > clusterMap;
    int32_t clusterIndex = 1;
    std::set< int32_t >::const_iterator
      cs = clusterSet.begin(),
      cse = clusterSet.end();
    while ( cs != cse )
    {

      clusterMap[ *cs ] = clusterIndex;

      ++ clusterIndex;
      ++ cs;

    }

    std::vector< std::list< int32_t > > integerClusters( _clusterCount );

    for ( i = 0; i < itemCount; i++ )
    {

      if ( _labels[ i ] != UNCLASSIFIED )
      {

        _labels[ i ] = clusterMap[ _labels[ i ] ];
        integerClusters[ _labels[ i ] - 1 ].push_back( i );

      }
      else
      {

        _labels[ i ] = NOISE;
        _isolatedItems.push_back( i );

      }

    }


    std::vector< std::list< int32_t > >::const_iterator
      ic = integerClusters.begin(),
      ice = integerClusters.end();
    while ( ic != ice )
    {

      if ( !ic->empty() )
      {

        _clusters.push_back( *ic );

      }
      ++ ic;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << std::flush;

    }


     ////////////////////////////////////////////////////////////////////////////
    // now free memory
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "freeing memory" << std::flush;

    }
    std::vector< gkg::Cluster* >::iterator
      c = clusters.begin(),
      ce = clusters.end();
    while ( c != ce )
    {

      delete *c;
      ++ c;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 14 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "HierarchicalDensityBasedSpatialClustering( "
             "const std::vector< I >& items, "
             "int32_t minimumPointCount, "
             "int32_t minimumClusterSize, "
             "const std::vector< gkg::Constraint >* constraints )" );

}


template < class I >
inline
gkg::HierarchicalDensityBasedSpatialClustering< I >::
                                    ~HierarchicalDensityBasedSpatialClustering()
{
}


template < class I >
inline
typename gkg::HierarchicalDensityBasedSpatialClustering< I >::const_iterator 
gkg::HierarchicalDensityBasedSpatialClustering< I >::begin() const
{

  try
  {

    return _clusters.begin();

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "typename gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "const_iterator "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::begin() "
             "const" );

}


template < class I >
inline
typename gkg::HierarchicalDensityBasedSpatialClustering< I >::const_iterator 
gkg::HierarchicalDensityBasedSpatialClustering< I >::end() const
{

  try
  {

    return _clusters.end();

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "typename gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "const_iterator "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "end() const" );

}





template < class I >
inline
int32_t 
gkg::HierarchicalDensityBasedSpatialClustering< I >::getClusterCount() const
{

  try
  {

    return _clusterCount;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "int32_t gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "getClusterCount() const" );

}


template < class I >
inline
int32_t 
gkg::HierarchicalDensityBasedSpatialClustering< I >::getNoisyItemCount() const
{

  try
  {

    return _noisyItemCount;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "int32_t gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "getNoisyItemCount() const" );

}


template < class I >
inline
const std::list< int32_t >& 
gkg::HierarchicalDensityBasedSpatialClustering< I >::getIsolatedItems() const
{

  try
  {

    return _isolatedItems;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "const std::list< int32_t >& "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "getIsolatedItems() const" );

}


template < class I >
inline
int32_t 
gkg::HierarchicalDensityBasedSpatialClustering< I >::getLabel( 
                                                           int32_t index ) const
{

  try
  {

    if ( ( index < 0 ) ||
         ( index >= ( int32_t )_labels.size() ) )
    {

      throw std::runtime_error( "index out of range" );

    }
    return _labels[ index ];

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "int32_t gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "getLabel( int32_t index ) const" );

}


template < class I >
inline
const std::vector< int32_t >& 
gkg::HierarchicalDensityBasedSpatialClustering< I >::getLabels() const
{

  try
  {

    return _labels;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "const std::vector< int32_t >& "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "getLabels() const" );

}


template < class I >
inline
bool 
gkg::HierarchicalDensityBasedSpatialClustering< I >::hasInfiniteStability()
                                                                           const
{

  try
  {

    return _hasInfiniteStability;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "bool "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "hasInfiniteStability() const" );

}


template < class I >
inline
void gkg::HierarchicalDensityBasedSpatialClustering< I >::displayResults() const
{

  try
  {

    std::cout << "HDBSCAN clustering: " << _clusterCount << " clusters found"
              << " with " << _noisyItemCount << " noise items" << std::endl;

    std::vector< int32_t >::const_iterator
      l = _labels.begin(),
      le = _labels.end();
    while ( l != le )
    {

      std::cout << *l << " ";
      ++ l;

    }
    std::cout << std::endl;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "displayResults() const" );

}


template < class I >
inline
void 
gkg::HierarchicalDensityBasedSpatialClustering< I >::
                                             calculateConstraintsSatisfiedCount(
                              const std::vector< gkg::Constraint >& constraints,
                              std::set< int32_t >& newClusterLabels,
                              std::vector< gkg::Cluster* >& clusters,
                              std::vector< int32_t >& clusterLabels )
{

  try
  {

    std::vector< gkg::Cluster > parents;

    std::set< int32_t >::const_iterator
      l = newClusterLabels.begin(),
      le = newClusterLabels.end();
    while ( l != le )
    {

      gkg::Cluster* parent = clusters[ *l ]->getParent();
      if ( parent && 
           !( std::find( parents.begin(), parents.end(), *parent ) !=
              parents.end() ) )
      {

        parents.push_back( *parent );

      }
      ++ l;

    }

    std::vector< gkg::Constraint >::const_iterator
      c = constraints.begin(),
      ce = constraints.end();
    while ( c != ce )
    {

      int32_t labelA = clusterLabels[ c->getItemA() ];
      int32_t labelB = clusterLabels[ c->getItemB() ];

      if ( ( c->getType() == gkg::Constraint::MustLink ) &&
           ( labelA == labelB ) )
      {

        if ( newClusterLabels.find( labelA ) != newClusterLabels.end() )
        {

          clusters[ labelA ]->addConstraintsSatisfied( 2 );

        }

      }
      else if ( ( c->getType() == gkg::Constraint::CannotLink ) &&
                ( ( labelA != labelB ) || labelA == 0 ) )
      {

        if ( ( labelA != 0 ) &&
             ( newClusterLabels.find( labelA ) != newClusterLabels.end() ) )
        {

          clusters[ labelA ]->addConstraintsSatisfied( 1 );

        }
        if ( ( labelB != 0 ) &&
             ( newClusterLabels.find( labelA ) != newClusterLabels.end() ) )
        {

          clusters[ labelB ]->addConstraintsSatisfied( 1 );

        }

        if ( labelA == 0)
        {

          std::vector< gkg::Cluster >::iterator
            p = parents.begin(),
            pe = parents.end();
          while ( p != pe )
          {

            if ( p->virtualChildClusterConstraintsItem( c->getItemA() ) )
            {

              p->addVirtualChildConstraintsSatisfied( 1 );
              break;

            }
            ++ p;

          }

        }

        if ( labelB == 0)
        {

          std::vector< gkg::Cluster >::iterator
            p = parents.begin(),
            pe = parents.end();
          while ( p != pe )
          {

            if ( p->virtualChildClusterConstraintsItem( c->getItemB() ) )
            {

              p->addVirtualChildConstraintsSatisfied( 1 );
              break;

            }
            ++ p;

          }

        }

      }

      ++ c;

    }


    std::vector< gkg::Cluster >::iterator
      p = parents.begin(),
      pe = parents.end();
    while ( p != pe )
    {

      p->releaseVirtualChildCluster();
      ++ p;

    }

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "calculateConstraintsSatisfiedCount( "
             "const std::vector< gkg:::Constraint >& constraints, "
             "std::set< int32_t >& newClusterLabels, "
             "std::vector< gkg::Cluster* >& clusters, "
             "std::vector< int32_t >& clusterLabels )" );

}


template < class I >
inline
gkg::Cluster* 
gkg::HierarchicalDensityBasedSpatialClustering< I >::createNewCluster(
                                          int32_t& clusterCounter,
                                          const std::set< int32_t >& items,
                                          std::vector< int32_t >& clusterLabels,
                                          gkg::Cluster* parent,
                                          int32_t clusterLabel,
                                          double edgeWeight )
{

  try
  {

    std::set< int32_t >::const_iterator 
      i = items.begin(),
      ie = items.end();
    while ( i != ie )
    {

      clusterLabels[ *i ] = clusterLabel;
      ++ i;

    }

    parent->detachPoints( ( int32_t )items.size(), edgeWeight );

    if ( clusterLabel != 0 )
    {

      return new gkg::Cluster( clusterCounter,
                               clusterLabel,
                               parent,
                               edgeWeight,
                               ( int32_t )items.size() );

    }

    parent->addItemsToVirtualChildCluster( items );
    return 0;

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "gkg::Cluster* "
             "gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "createNewCluster( "
             "int32_t& clusterCounter, "
             "const std::set< int32_t >& items, "
             "std::vector< int32_t >& clusterLabels, "
             "gkg::Cluster* parent, "
             "int32_t clusterLabel, "
             "double edgeWeight )" );

}


template < class I >
inline
void gkg::HierarchicalDensityBasedSpatialClustering< I >::propagateTree(
                                  const std::vector< gkg::Cluster* >& clusters )
{

  try
  {

    _hasInfiniteStability = false;
    gkg::BitSet addedToExaminationList;
    std::map< int32_t, gkg::Cluster* > clustersToExamine;

    std::vector< gkg::Cluster* >::const_iterator
      c = clusters.begin(),
      ce = clusters.end();
    while ( c != ce )
    {

      gkg::Cluster* cluster = *c;
      if ( cluster && !cluster->hasChildren() )
      {

        int32_t label = cluster->getLabel();
        clustersToExamine.erase( label );
        clustersToExamine.insert(
                        std::pair< int32_t, gkg::Cluster* >( label, cluster ) );
        addedToExaminationList.set( label );

      }

      ++ c;

    }

    // iterating through every cluster, propagating stability from children to 
    // parents
    while ( clustersToExamine.size() )
    {

      std::map< int32_t, gkg::Cluster* >::iterator
        ce = std::prev( clustersToExamine.end() );

      gkg::Cluster* currentCluster = ce->second;
      clustersToExamine.erase( ce->first );

      currentCluster->propagate();

      if ( currentCluster->getStability() ==
           std::numeric_limits< double >::infinity() )
      {

        _hasInfiniteStability = true;

      }

      if ( currentCluster->getParent() )
      {

        gkg::Cluster* parent = currentCluster->getParent();
        int32_t label = parent->getLabel();

        if ( !addedToExaminationList.get( label ) )
        {

          clustersToExamine.erase( label );

          clustersToExamine.insert(
                         std::pair< int32_t, gkg::Cluster* >( label, parent ) );
          addedToExaminationList.set( label );

        }

      }

    }

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "propagateTree( "
             "const std::vector< gkg::Cluster* >& clusters )" );

}


template < class I >
inline
void gkg::HierarchicalDensityBasedSpatialClustering< I >::findProminentClusters(
                        const std::vector< gkg::Cluster* >& clusters,
                        const std::vector< std::vector< int32_t > >& hierarchy )
{

  try
  {

    // taking the list of propagated clusters from the root cluster
    std::vector< gkg::Cluster* >
      solution = clusters[ 1 ]->getPropagatedDescendants();

    // storing all the hierarchy positions at which to find the birth points 
    // for the flat clustering
    std::map< int32_t, std::vector< int32_t > > significantHierarchyPositions;
    std::vector< gkg::Cluster* >::const_iterator
      c = solution.begin(),
      ce = solution.end();
    while ( c != ce )
    {

      int32_t hierarchyPosition = (*c)->getHierarchyPosition();
      if ( significantHierarchyPositions.count( hierarchyPosition ) > 0 )
      {

	significantHierarchyPositions[ hierarchyPosition ].push_back(
                                                             (*c)->getLabel() );

      }
      else
      {

	significantHierarchyPositions[ hierarchyPosition ].push_back(
                                                             (*c)->getLabel() );

      }
      ++ c;

    }

    // going through the hierarchy file, setting labels for the flat clustering
    while ( !significantHierarchyPositions.empty() )
    {

      std::map< int32_t, std::vector< int32_t > >::iterator
        entry = significantHierarchyPositions.begin();

      int32_t hierarchyPosition = entry->first;
      std::vector< int32_t > clusterList = entry->second;

      significantHierarchyPositions.erase( hierarchyPosition );

      const std::vector< int32_t >&
        lineContents = hierarchy[ hierarchyPosition ];

      int32_t i = 0;
      for ( i = 0; i < ( int32_t )lineContents.size(); i++ )
      {

        int32_t label = lineContents[ i ];
        if ( std::find(clusterList.begin(), clusterList.end(), label ) != 
             clusterList.end() )
        {

          _labels[ i ] = label;

        }

      }

    }

  }
  GKG_CATCH( "template < class I > "
             "inline "
             "void gkg::HierarchicalDensityBasedSpatialClustering< I >::"
             "findProminentClusters( "
             "const std::vector< gkg::Cluster* >& clusters, "
             "const std::vector< std::vector< int32_t > >& hierarchy )" );

}

