#include <gkg-processing-graph/EuclideanMinimumSpanningTree.h>
#include <gkg-core-exception/Exception.h>
#include <utility>
#include <algorithm>
#include <map>


//
// class EuclideanMinimumSpanningTree::Node
//

gkg::EuclideanMinimumSpanningTree::Node::Node(
                                gkg::EuclideanMinimumSpanningTree::Node* parent,
                                const gkg::Vector3d< float >& position,
                                int32_t index,
                                float pathDistance )
                                        : _parent( parent ),
                                          _position( position ),
                                          _index( index ),
                                          _pathDistance( pathDistance ),
                                          _isRoot( false ),
                                          _hasChildren( false )
{

  try
  {

    parent->addChild( this );  

  }
  GKG_CATCH( "gkg::EuclideanMinimumSpanningTree::Node::Node( "
             "gkg::EuclideanMinimumSpanningTree::Node* parent, "
             "const gkg::Vector3d< float >& position, "
             "int32_t index, "
             "float pathDistance )" );

}


gkg::EuclideanMinimumSpanningTree::Node::Node(
                                const gkg::Vector3d< float >& position,
                                int32_t index,
                                float pathDistance )
                                        : _parent( 0 ),
                                          _position( position ),
                                          _index( index ),
                                          _pathDistance( pathDistance ),
                                          _isRoot( true ),
                                          _hasChildren( false )
{
}


gkg::EuclideanMinimumSpanningTree::Node::Node(
                          const gkg::EuclideanMinimumSpanningTree::Node& other )
                                        : _parent( other._parent ),
                                          _position( other._position ),
                                          _index( other._index ),
                                          _pathDistance( other._pathDistance ),
                                          _isRoot( other._isRoot ),
                                          _hasChildren( other._hasChildren ),
                                          _children( other._children )
{
}


gkg::EuclideanMinimumSpanningTree::Node::~Node()
{
}


gkg::EuclideanMinimumSpanningTree::Node&
gkg::EuclideanMinimumSpanningTree::Node::operator=(
                          const gkg::EuclideanMinimumSpanningTree::Node& other )
{

  try
  {

    _parent = other._parent;
    _position = other._position;
    _index = other._index;
    _pathDistance = other._pathDistance;
    _isRoot = other._isRoot;
    _hasChildren = other._hasChildren;
    _children = other._children;

    return *this;

  }
  GKG_CATCH( "gkg::EuclideanMinimumSpanningTree::Node& "
             "gkg::EuclideanMinimumSpanningTree::Node::operator=( "
             "const gkg::EuclideanMinimumSpanningTree::Node& other )" );

}


const gkg::Vector3d< float >& 
gkg::EuclideanMinimumSpanningTree::Node::getPosition() const
{

  try
  {

    return _position;

  }
  GKG_CATCH( "const gkg::Vector3d< float >& "
             "gkg::EuclideanMinimumSpanningTree::Node::getPosition() const" );

}


int32_t gkg::EuclideanMinimumSpanningTree::Node::getIndex() const
{

  try
  {

    return _index;

  }
  GKG_CATCH( "int32_t "
             "gkg::EuclideanMinimumSpanningTree::Node::getIndex() const" );

}


const std::vector< gkg::EuclideanMinimumSpanningTree::Node >& 
gkg::EuclideanMinimumSpanningTree::Node::getChildren() const
{

  try
  {

    return _children;

  }
  GKG_CATCH( "const std::vector< gkg::EuclideanMinimumSpanningTree::Node >& "
             "gkg::EuclideanMinimumSpanningTree::Node::getChildren() const" );

}


float gkg::EuclideanMinimumSpanningTree::Node::getPathDistance() const
{

  try
  {

    return _pathDistance;

  }
  GKG_CATCH( "float "
             "gkg::EuclideanMinimumSpanningTree::Node::getPathDistance() "
             "const" );

}


bool gkg::EuclideanMinimumSpanningTree::Node::isRoot() const
{

  try
  {

    return _isRoot;

  }
  GKG_CATCH( "bool gkg::EuclideanMinimumSpanningTree::Node::isRoot() const" );

}


bool gkg::EuclideanMinimumSpanningTree::Node::hasChildren() const
{

  try
  {

    return _hasChildren;

  }
  GKG_CATCH( "bool "
             "gkg::EuclideanMinimumSpanningTree::Node::hasChildren() const" );

}


void gkg::EuclideanMinimumSpanningTree::Node::addChild(
                                gkg::EuclideanMinimumSpanningTree::Node* child )
{

  try
  {

    _children.push_back( *child );
    _hasChildren = true;

  }
  GKG_CATCH( "void gkg::EuclideanMinimumSpanningTree::Node::addChild( "
             "gkg::EuclideanMinimumSpanningTree::Node* child )" );

}




//
// class EuclideanMinimumSpanningTree::Node
//


gkg::EuclideanMinimumSpanningTree::EuclideanMinimumSpanningTree(
                         const std::vector< gkg::Vector3d< float > >& positions,
                         float balancingFactor )
{

  try
  {

    int32_t positionCount = ( int32_t )positions.size();

    std::map< int32_t, int32_t > positionIndices;
    positionIndices.insert( std::pair< int32_t, int32_t >( 0, 0 ) );

    gkg::Vector3d< float > rootPosition = positions[ 0 ];

    gkg::EuclideanMinimumSpanningTree::Node rootNode( rootPosition, 0, 0.0f );
    _nodes.push_back( rootNode );

    // initializing vector of open points
    std::vector< int32_t > openPositions( positionCount, 0 );
    int32_t p = 0;
    for ( p = 1; p < positionCount; p++ )
    {

      openPositions[ p ] = p;

    }

    // initializing distance to root point
    std::vector< float > distances( positionCount, 0.0f );
    for ( p = 0; p < positionCount; p++ )
    {

      distances[ p ] = ( positions[ p ] - rootPosition ).getNorm();

    }

    std::vector< int32_t > closestPositionInTree( positionCount, 0 );

    int32_t positionIndicesIndex = 0;
    int32_t d = 0;
    while ( !distances.empty() )
    {

      // getting argmin of distances and remove corresponding distance
      int32_t minimumIndex = std::distance(
                                           distances.begin(), 
                                           std::min_element( distances.begin(), 
                                                             distances.end()) );
      distances.erase( distances.begin() + minimumIndex ); 

      // popping element with index minimumIndex out of open list
      int32_t positionIndex = openPositions[ minimumIndex ];
      openPositions.erase( openPositions.begin() + minimumIndex ); 

      // getting closest point in tree
      int32_t closestPositionIndex = closestPositionInTree[ positionIndex ];  

      gkg::Vector3d< float > position = positions[ positionIndex ];

      gkg::EuclideanMinimumSpanningTree::Node&
        parentNode = _nodes[ positionIndices[ closestPositionIndex ] ];

      float actualDistance = ( position - parentNode.getPosition() ).getNorm();   

      float pathDistance = actualDistance + parentNode.getPathDistance();

      gkg::EuclideanMinimumSpanningTree::Node 
        node( &parentNode, position, positionIndex, pathDistance );

      // add to closed list
      if ( positionIndices.find( positionIndex ) == positionIndices.end())
      {

        ++ positionIndicesIndex;
        _nodes.push_back( node );
        positionIndices.insert(
                        std::pair< int32_t, int32_t >( positionIndex,
                                                       positionIndicesIndex ) );

      } 

      std::vector< float > weightedDistances;
      for ( p = 0; p < ( int32_t )openPositions.size(); p++ )
      {

        weightedDistances.push_back( 
                      ( positions[ openPositions[ p ] ] - position ).getNorm() +
                      balancingFactor * pathDistance );

      }

      std::vector< int32_t > indices( distances.size(), 0 );
      for ( d = 0; d < ( int32_t )distances.size(); d++ )
      {

        if ( distances[ d ] > weightedDistances[ d ] )
        {

          indices[ d ] = 1;
          distances[ d ] = weightedDistances[ d ];

        }

      }

      std::vector< bool > changedValues( closestPositionInTree.size(), 0 );
      for ( d = 0; d < ( int32_t )openPositions.size(); d++ )
      {

        changedValues[ openPositions[ d ] ] = indices[ d ];

      }

      for ( d = 0; d < ( int32_t )changedValues.size(); d++ )
      {

        if ( changedValues[ d ] == 1 )
        {

          closestPositionInTree[ d ] = positionIndex;

        }

      } 

    }

  }
  GKG_CATCH( "gkg::EuclideanMinimumSpanningTree::EuclideanMinimumSpanningTree( "
             "const std::vector< gkg::Vector3d< float > >& points, "
             "float balancingFactor )" );

}


gkg::EuclideanMinimumSpanningTree::~EuclideanMinimumSpanningTree()
{
}


const std::vector< gkg::EuclideanMinimumSpanningTree::Node >& 
gkg::EuclideanMinimumSpanningTree::getNodes() const
{

  try
  {

    return _nodes;

  }
  GKG_CATCH( "const std::vector< gkg::EuclideanMinimumSpanningTree::Node >& "
             "gkg::EuclideanMinimumSpanningTree::getNodes() const" );

}
