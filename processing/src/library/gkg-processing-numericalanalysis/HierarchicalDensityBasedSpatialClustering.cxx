#include <gkg-processing-numericalanalysis/HierarchicalDensityBasedSpatialClustering_i.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
// class HierarchicalDensityBasedSpatialClustering::UndirectedGraph
////////////////////////////////////////////////////////////////////////////////


gkg::UndirectedGraph::UndirectedGraph(
                                      int32_t vertexCount, 
                                      const std::vector< int32_t >& aVertices,
                                      const std::vector< int32_t >& bVertices,
                                      const std::vector< double >& edgeWeights )
                       : _vertexCount( vertexCount ),
                         _aVertices( aVertices ),
                         _bVertices( bVertices ),
                         _edgeWeights( edgeWeights ),
                         _edges( vertexCount )
{

  try
  {

    int32_t edgeWeightCount = ( int32_t )edgeWeights.size();
    int32_t i = 0;
    // the following code has been removed since it migh cause memory overload for
    // clustering of huge size(s)
    // for ( i = 0; i < vertexCount; i++ )
    // {
    //
    //   _edges[ i ].reserve( vertexCount );
    //
    // }
    for ( i = 0; i < edgeWeightCount; i++ )
    {

      _edges[ _aVertices[ i ] ].push_back( _bVertices[ i ] );
      if ( _aVertices[ i ] != _bVertices[ i ] )
      {

        _edges[ _bVertices[ i ] ].push_back( _aVertices[ i ] );

      }

    }

  }
  GKG_CATCH( "gkg::UndirectedGraph::UndirectedGraph("
             "int32_t vertexCount, "
             "const std::vector< int32_t >& aVertices, "
             "const std::vector< int32_t >& bVertices, "
             "const std::vector< double >& edgeWeights )" );

}


void gkg::UndirectedGraph::quickSortByEdgeWeight()
{

  try
  {

    int32_t edgeWeightCount = ( int32_t )_edgeWeights.size();
    if ( edgeWeightCount < 2 )
    {

      return;

    }
    std::vector< int32_t > startIndexStack( edgeWeightCount / 2 );
    std::vector< int32_t > endIndexStack( edgeWeightCount / 2 );

    startIndexStack[ 0 ] = 0;
    endIndexStack[ 0 ] = edgeWeightCount - 1;

    int32_t stackTop = 0;
    int32_t startIndex = 0;
    int32_t endIndex = 0;
    int32_t pivotIndex = 0;

    while ( stackTop >= 0 )
    {

      startIndex = startIndexStack[ stackTop ];
      endIndex = endIndexStack[ stackTop ];
      -- stackTop;

      pivotIndex = selectPivotIndex( startIndex, endIndex );
      pivotIndex = partition( startIndex, endIndex, pivotIndex );
      if ( pivotIndex > startIndex + 1 )
      {

        startIndexStack[ stackTop + 1 ] = startIndex;
        endIndexStack[ stackTop + 1 ] = pivotIndex - 1;
        ++ stackTop;

      }
      if ( pivotIndex < endIndex - 1 )
      {

        startIndexStack[ stackTop + 1 ] = pivotIndex + 1;
        endIndexStack[ stackTop + 1 ] = endIndex;
        ++ stackTop;

      }

    }

  }
  GKG_CATCH( "void gkg::UndirectedGraph::quickSortByEdgeWeight()" );

}


int32_t gkg::UndirectedGraph::getVertexCount() const
{

  try
  {

    return _vertexCount;

  }
  GKG_CATCH( "int32_t gkg::UndirectedGraph::getVertexCount() const" );

}


int32_t gkg::UndirectedGraph::getEdgeCount() const
{

  try
  {

    return ( int32_t )_edgeWeights.size();

  }
  GKG_CATCH( "int32_t gkg::UndirectedGraph::getEdgeCount() const" );

}


int32_t gkg::UndirectedGraph::getFirstVertexAtIndex( int32_t index ) const
{

  try
  {

    return _aVertices[ index ];

  }
  GKG_CATCH( "int32_t gkg::UndirectedGraph::getFirstVertexAtIndex( "
             "int32_t index ) const" );

}


int32_t gkg::UndirectedGraph::getSecondVertexAtIndex( int32_t index ) const
{

  try
  {

    return _bVertices[ index ];

  }
  GKG_CATCH( "int32_t gkg::UndirectedGraph::getSecondVertexAtIndex( "
             "int32_t index ) const" );

}


double gkg::UndirectedGraph::getEdgeWeightAtIndex( int32_t index ) const
{

  try
  {

    return _edgeWeights[ index ];

  }
  GKG_CATCH( "double gkg::UndirectedGraph::getEdgeWeightAtIndex( "
             "int32_t index ) const" );

}


const std::vector< int32_t >& 
gkg::UndirectedGraph::getEdgeListForVertex( int32_t index ) const
{

  try
  {

    return _edges[ index ];

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::UndirectedGraph::getEdgeListForVertex( "
             "int32_t index ) const" );

}


void gkg::UndirectedGraph::removeEdgeFromList( int32_t edgeIndex,
                                               int32_t index )
{

  try
  {

    std::vector< int32_t >& edges = _edges[ index ];
    std::vector< int32_t >::iterator
      e = std::find( edges.begin(), edges.end(), edgeIndex );
    if ( e != edges.end() )
    {

      edges.erase( e );

    }

  }
  GKG_CATCH( "void gkg::UndirectedGraph::"
             "removeEdgeFromList( int32_t edgeIndex, int32_t listIndex )" );

}



int32_t gkg::UndirectedGraph::selectPivotIndex( int32_t startIndex,
                                                int32_t endIndex ) const
{

  try
  {

    if ( ( startIndex - endIndex ) < 2 )
    {

      return startIndex;

    }

    double first = _edgeWeights[ startIndex ];
    double middle = _edgeWeights[ startIndex + ( endIndex - startIndex ) / 2 ];
    double last = _edgeWeights[ endIndex ];

    if ( first <= middle )
    {

      if ( middle <= last )
      {

        return startIndex + ( endIndex - startIndex ) / 2;

      }
      if ( last >= first )
      {

        return endIndex;

      }

      return startIndex;
    }

    if ( first <= last )
    {

      return startIndex;

    }

    if ( last >= middle )
    {

      return endIndex;

    }

    return startIndex + ( endIndex - startIndex ) / 2;

  }
  GKG_CATCH( "int32_t "
             "gkg::UndirectedGraph::selectPivotIndex( "
             "int32_t startIndex, "
             "int32_ endIndex )" );

}


int32_t gkg::UndirectedGraph::partition( int32_t startIndex,
                                         int32_t endIndex,
                                         int32_t pivotIndex )
{

  try
  {

    double pivotValue = _edgeWeights[ pivotIndex ];
    swapEdges( pivotIndex, endIndex );
    int32_t lowIndex = startIndex;
    int32_t i = 0;
    for ( i = startIndex; i < endIndex; i++ )
    {

      if ( _edgeWeights[ i ] < pivotValue )
      {

        swapEdges( i, lowIndex );
        ++ lowIndex;

      }

    }
    swapEdges( lowIndex, endIndex );
    return lowIndex;

  }
  GKG_CATCH( "int32_t "
             "gkg::UndirectedGraph::partition( "
             "int32_t startIndex, "
             "int32_ endIndex, "
             "int32_t pivotIndex )" );

}


void gkg::UndirectedGraph::swapEdges( int32_t firstIndex, int32_t secondIndex )
{

  try
  {

    if ( firstIndex == secondIndex )
    {

      return;

    }

    int32_t tempAVertex = _aVertices[ firstIndex];
    int32_t tempBVertex = _bVertices[ firstIndex ];
    double tempEdgeDistance = _edgeWeights[ firstIndex ];

    _aVertices[ firstIndex] = _aVertices[ secondIndex ];
    _bVertices[ firstIndex] = _bVertices[ secondIndex ];
    _edgeWeights[ firstIndex] = _edgeWeights[ secondIndex ];

    _aVertices[ secondIndex ] = tempAVertex;
    _bVertices[ secondIndex ] = tempBVertex;
    _edgeWeights[ secondIndex ] = tempEdgeDistance;

  }
  GKG_CATCH( "void "
             "gkg::UndirectedGraph::swapEdges( "
             "int32_t firstIndex, "
             "int32_ secondIndex )" );

}


////////////////////////////////////////////////////////////////////////////////
// class Cluster
////////////////////////////////////////////////////////////////////////////////

gkg::Cluster::Cluster( int32_t& clusterCounter )
             : _clusterCounter( clusterCounter )
{

  try
  {

    ++ _clusterCounter;

    _id = _clusterCounter;
    _birthLevel = 0.0;
    _deathLevel = 0.0;
    _itemCount = 0;
    _propagatedStability = 0.0;
    _constraintsSatisfiedCount = 0;
    _propagatedConstraintsSatisfiedCount = 0;

    _propagatedDescendants.resize( 0U );

    _propagatedLowestChildDeathLevel = std::numeric_limits< double >::max();
    _parent = 0;
    _stability = 0.0;
    _hasChildren = false;
    _label = 0;
    _hierarchyPosition = 0;

  }
  GKG_CATCH( "gkg::Cluster::Cluster( int32_t clusterCounter )" );

}


gkg::Cluster::Cluster( int32_t& clusterCounter,
                       int32_t label,
                       gkg::Cluster* parent,
                       double birthLevel,
                       int32_t itemCount )
             : _clusterCounter( clusterCounter )
{

  try
  {

    ++ _clusterCounter;

    _id = _clusterCounter;
    _birthLevel = birthLevel;
    _deathLevel = 0.0;
    _itemCount = itemCount;
    _propagatedStability = 0.0;
    _constraintsSatisfiedCount = 0;
    _propagatedConstraintsSatisfiedCount = 0;

    _propagatedDescendants.resize( 0U );

    _propagatedLowestChildDeathLevel = std::numeric_limits< double >::max();
    _parent = parent;
    _stability = 0.0;
    if ( _parent )
    {

      parent->setHasChildren( true );

    }
    _hasChildren = false;
    _label = label;
    _hierarchyPosition = 0;

  }
  GKG_CATCH( "gkg::Cluster::"
             "Cluster( "
             "int32_t& clusterCounter, "
             "int32_t label, "
             "gkg::Cluster* parent, "
             "double birthLevel, "
             "int32_t itemCount )" );

}



bool gkg::Cluster::operator==( const gkg::Cluster& other ) const
{

  try
  {

    return ( this->_id == other._id );


  }
  GKG_CATCH( "bool gkg::Cluster::operator==( "
             "const gkg::Cluster& other ) const" );

}


void gkg::Cluster::detachPoints( int32_t itemCount, double level )
{

  try
  {

    _itemCount -= itemCount;
    _stability += ( itemCount * ( 1 / level - 1 / _birthLevel ) );

    if ( _itemCount == 0 )
    {

      _deathLevel = level;

    }
    else if ( _itemCount < 0 )
    {

      throw std::invalid_argument( "cluster cannot have less than 0 points" );

    }

  }
  GKG_CATCH( "void gkg::Cluster::"
             "detachPoints( int32_t itemCount, double level )" );

}


void gkg::Cluster::propagate()
{

  try
  {

    if ( _parent )
    {

      if ( _propagatedLowestChildDeathLevel == 
           std::numeric_limits< double >::max() )
      {

        _propagatedLowestChildDeathLevel = _deathLevel;

      }

      if ( _propagatedLowestChildDeathLevel <
           _parent->getPropagatedLowestChildDeathLevel() )
      {

        _parent->setPropagatedLowestChildDeathLevel(
                                             _propagatedLowestChildDeathLevel );

      }

      if ( !_hasChildren )
      {

        _parent->addPropagatedConstraintsSatisfiedCount(
                                                   _constraintsSatisfiedCount );
        _parent->addPropagatedStability( _stability );
        _parent->addPropagatedDescendants( this );

      }
      else if ( _constraintsSatisfiedCount >
                _propagatedConstraintsSatisfiedCount )
      {

        _parent->addPropagatedConstraintsSatisfiedCount(
                                                   _constraintsSatisfiedCount );
        _parent->addPropagatedStability( _stability );
        _parent->addPropagatedDescendants( this );

      }
      else if ( _constraintsSatisfiedCount <
                _propagatedConstraintsSatisfiedCount )
      {

        _parent->addPropagatedConstraintsSatisfiedCount(
                                         _propagatedConstraintsSatisfiedCount );
        _parent->addPropagatedStability( _propagatedStability );
        _parent->addPropagatedDescendants( _propagatedDescendants );

      }
      else if ( _constraintsSatisfiedCount ==
                _propagatedConstraintsSatisfiedCount )
      {

        // choosing the parent over descendants if there is a tie in stability
        if ( _stability >= _propagatedStability )
        {

          _parent->addPropagatedConstraintsSatisfiedCount(
                                                   _constraintsSatisfiedCount );
          _parent->addPropagatedStability( _stability );
          _parent->addPropagatedDescendants( this );

        }
        else
        {

          _parent->addPropagatedConstraintsSatisfiedCount(
                                         _propagatedConstraintsSatisfiedCount );
          _parent->addPropagatedStability( _propagatedStability );
          _parent->addPropagatedDescendants( _propagatedDescendants );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::Cluster::propagate()" );

}


void gkg::Cluster::addItemsToVirtualChildCluster(
                                              const std::set< int32_t >& items )
{

  try
  {

    std::set< int32_t >::const_iterator
      i = items.begin(),
      ie = items.end();
    while ( i != ie )
    {

      _virtualChildCluster.insert( *i );
      ++ i;

    }

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addItemsToVirtualChildCluster( const std::set< int32_t >& "
             "items )" );

}


bool gkg::Cluster::virtualChildClusterConstraintsItem( int32_t itemIndex ) const
{

  try
  {

    return ( _virtualChildCluster.find( itemIndex ) !=
             _virtualChildCluster.end() );

  }
  GKG_CATCH( "bool gkg::Cluster::"
             "virtualChildClusterConstraintsItem( int32_t itemIndex ) const" );

}


void 
gkg::Cluster::addVirtualChildConstraintsSatisfied( int32_t constraintCount )
{

  try
  {

    _propagatedConstraintsSatisfiedCount += constraintCount;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addVirtualChildConstraintsSatisfied( int32_t constraintCount )" );

}


void gkg::Cluster::addConstraintsSatisfied( int32_t constraintCount )
{

  try
  {

    _constraintsSatisfiedCount += constraintCount;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addConstraintsSatisfied( int32_t constraintCount )" );

}


void gkg::Cluster::releaseVirtualChildCluster()
{

  try
  {

    _virtualChildCluster.clear();

  }
  GKG_CATCH( "void gkg::Cluster::releaseVirtualChildCluster()" );

}


int32_t gkg::Cluster::getClusterId() const
{

  try
  {

    return _id;

  }
  GKG_CATCH( "int32_t gkg::Cluster::getClusterId() const" );

}


gkg::Cluster* gkg::Cluster::getParent() const
{

  try
  {

    return _parent;

  }
  GKG_CATCH( "gkg::Cluster* "
             "gkg::Cluster::"
             "getParent() const" );

}


void gkg::Cluster::setHasChildren( bool value )
{

  try
  {

    _hasChildren = value;

  }
  GKG_CATCH( "void gkg::Cluster::setHasChildren( bool value )" );

}


bool gkg::Cluster::hasChildren() const
{

  try
  {

    return _hasChildren;

  }
  GKG_CATCH( "bool "
             "gkg::Cluster::"
             "hasChildren() const" );


}


void gkg::Cluster::setPropagatedLowestChildDeathLevel( double value )
{

  try
  {

    _propagatedLowestChildDeathLevel = value;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "setPropagatedLowestChildDeathLevel( double value )" );

}


double gkg::Cluster::getPropagatedLowestChildDeathLevel() const
{

  try
  {

    return _propagatedLowestChildDeathLevel;

  }
  GKG_CATCH( "double gkg::Cluster::"
             "getPropagatedLowestChildDeathLevel() const" );

}


void gkg::Cluster::addPropagatedConstraintsSatisfiedCount( int32_t count )
{

  try
  {

    _propagatedConstraintsSatisfiedCount += count;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addPropagatedConstraintsSatisfiedCount( int32_t count )" );


}


void gkg::Cluster::addPropagatedStability( double value )
{

  try
  {

    _propagatedStability += value;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addPropagatedStability( double value )" );


}


void gkg::Cluster::addPropagatedDescendants( gkg::Cluster* other )
{

  try
  {

    _propagatedDescendants.push_back( other );

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addPropagatedDescendants( "
             "gkg::Cluster* "
             "other )" );

}


void gkg::Cluster::addPropagatedDescendants(
                     const std::vector< gkg::Cluster* >& propagatedDescendants )
{

  try
  {

    _propagatedDescendants.insert( _propagatedDescendants.end(),
                                   propagatedDescendants.begin(),
                                   propagatedDescendants.end() );

  }
  GKG_CATCH( "void gkg::Cluster::"
             "addPropagatedDescendants( "
             "const std::vector< "
             "gkg::Cluster* >& "
             "propagatedDescendants )" );

}


void gkg::Cluster::setHierarchyPosition( int32_t hierarchyPosition )
{

  try
  {

    _hierarchyPosition = hierarchyPosition;

  }
  GKG_CATCH( "void gkg::Cluster::"
             "setHierarchyPosition( int32_t hierarchyPosition )" );

}


int32_t gkg::Cluster::getHierarchyPosition() const
{

  try
  {

    return _hierarchyPosition;

  }
  GKG_CATCH( "int32_t gkg::Cluster::"
             "getHierarchyPosition() const" );

}


int32_t gkg::Cluster::getLabel() const
{

  try
  {

    return _label;

  }
  GKG_CATCH( "int32_t "
             "gkg::Cluster::"
             "getLabel() const" );

}


double gkg::Cluster::getStability() const
{

  try
  {

    return _stability;

  }
  GKG_CATCH( "double "
             "gkg::Cluster::"
             "getStability() const" );

}


const std::vector< gkg::Cluster* >&
gkg::Cluster::getPropagatedDescendants() const
{

  try
  {

    return _propagatedDescendants;

  }
  GKG_CATCH( "const std::vector< "
             "gkg::Cluster* >& "
             "gkg::Cluster::"
             "getPropagatedDescendants() const" );

}


void gkg::Cluster::display() const
{

  try
  {

    std::cout << "cluster: "
              << "_id= " << _id << std::endl
              << "_birthLevel= " << _birthLevel << std::endl
              << "_deathLevel= " << _deathLevel << std::endl
              << "_itemCount= " << _itemCount << std::endl
              << "_propagatedStability= " << _propagatedStability << std::endl
              << "_constraintsSatisfiedCount= "
              << _constraintsSatisfiedCount << std::endl
              << "_propagatedConstraintsSatisfiedCount= "
              << _propagatedConstraintsSatisfiedCount << std::endl
              << "_virtualChildCluster= ";
    std::set< int32_t >::const_iterator
      v = _virtualChildCluster.begin(),
      ve = _virtualChildCluster.end();
    while ( v != ve )
    {

      std::cout << *v << " ";
      ++ v;

    }
    std::cout << std::endl
              << "_propagatedDescendants.size()= "
              << _propagatedDescendants.size() << std::endl
              << "_propagatedLowestChildDeathLevel= "
              << _propagatedLowestChildDeathLevel << std::endl;
    if ( _parent )
    {

      std::cout << "_parent->getClusterId()= " << _parent->getClusterId() << std::endl;

    }
    else
    {

      std::cout << "_parent= " << 0 << std::endl;

    }
    std::cout << "_stability= " << _stability << std::endl
              << "_hasChildren= " << _hasChildren << std::endl
              << "_label= " << _label << std::endl
              << "_hierarchyPosition= " << _hierarchyPosition << std::endl;
    std::cout << std::endl;

  }
  GKG_CATCH( "void gkg::Cluster::display() const" );


}


////////////////////////////////////////////////////////////////////////////////
// class CoreDistanceGauge
////////////////////////////////////////////////////////////////////////////////

gkg::CoreDistanceGauge::CoreDistanceGauge( int32_t maxCount )
                       : _count( 0 ),
                         _maxCount( maxCount ),
                         _alreadyDisplayed( false )
{
}


void gkg::CoreDistanceGauge::add( int32_t value )
{

  try
  {
  
    _count += value;

    display();  
    
  }
  GKG_CATCH( "void gkg::CoreDistanceGauge::add( int32_t value )" );

}


void gkg::CoreDistanceGauge::reset()
{

  try
  {

    _count = 0U;

    display();  
    
  }
  GKG_CATCH( "void gkg::CoreDistanceGauge::reset()" );

}


void gkg::CoreDistanceGauge::display()
{
 
  try
  {
   
    if ( _alreadyDisplayed )
    {

      std::cout << gkg::Eraser( 38 );

    }

    std::cout << " [ " << std::setw( 15 ) << _count 
              << " / " << std::setw( 15 ) << _maxCount
              << " ]" << std::flush;

    _alreadyDisplayed = true;
    
  }
  GKG_CATCH( "void gkg::CoreDistanceGauge::display()" );

}


