#include <gkg-processing-mesh/Polyhedron.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


template < class T >
inline const T& getCoordinate( const gkg::Vector3d< T >& v, int32_t coordinate )
{

  if ( coordinate == 0 )
  {

    return v.x;

  }
  else if ( coordinate == 1 )
  {

    return v.y;

  }

  return v.z;

}


template < class T >
inline T& getCoordinate( gkg::Vector3d< T >& v, int32_t coordinate )
{

  if ( coordinate == 0 )
  {

    return v.x;

  }
  else if ( coordinate == 1 )
  {

    return v.y;

  }

  return v.z;

}



gkg::Polyhedron::Polyhedron()
{
}


gkg::Polyhedron::Polyhedron( const gkg::Polyhedron& other )
                : _vertices( other._vertices )
{
}


gkg::Polyhedron::~Polyhedron()
{
}


gkg::Polyhedron& gkg::Polyhedron::operator=( const gkg::Polyhedron& other )
{

  try
  {

    _vertices = other._vertices;

    return *this;

  }
  GKG_CATCH( "gkg::Polyhedron& "
             "gkg::Polyhedron::operator=( const gkg::Polyhedron& other )" );

}


gkg::Polyhedron::iterator gkg::Polyhedron::begin()
{

  try
  {

    return _vertices.begin();

  }
  GKG_CATCH( "gkg::Polyhedron::iterator gkg::Polyhedron::begin()" );

}


gkg::Polyhedron::iterator gkg::Polyhedron::end()
{

  try
  {

    return _vertices.end();

  }
  GKG_CATCH( "gkg::Polyhedron::iterator gkg::Polyhedron::end()" );

}


gkg::Polyhedron::const_iterator gkg::Polyhedron::begin() const
{

  try
  {

    return _vertices.begin();

  }
  GKG_CATCH( "gkg::Polyhedron::const_iterator gkg::Polyhedron::begin() const" );

}


gkg::Polyhedron::const_iterator gkg::Polyhedron::end() const
{

  try
  {

    return _vertices.end();

  }
  GKG_CATCH( "gkg::Polyhedron::const_iterator gkg::Polyhedron::end() const" );

}


void gkg::Polyhedron::reserve( int32_t vertexCount )
{

  try
  {

    _vertices.reserve( vertexCount );

  }
  GKG_CATCH( "void gkg::Polyhedron::reserve( int32_t vertexCount )" );


}


void gkg::Polyhedron::addVertex( const gkg::Vector3d< float >& position,
                                 int32_t neighbor1,
                                 int32_t neighbor2,
                                 int32_t neighbor3 )
{

  try
  {

    _vertices.push_back( gkg::Polyhedron::Vertex(
                                      position,
                                      gkg::Vector3d< int32_t >( neighbor1,
                                                                neighbor2,
                                                                neighbor3 ) ) );

  }
  GKG_CATCH( "void gkg::Polyhedron::addVertex( "
             "const gkg::Vector3d< float >& position, "
             "int32_t neighbor1, "
             "int32_t neighbor2, "
             "int32_t neighbor3 )" );

}


void gkg::Polyhedron::addVertex( const gkg::Vector3d< float >& position,
                                 const gkg::Polyhedron::Neighbors& neighbors )
{

  try
  {

    _vertices.push_back( gkg::Polyhedron::Vertex( position,
                                                  neighbors ) );

  }
  GKG_CATCH( "void gkg::Polyhedron::addVertex( "
             "const gkg::Vector3d< float >& position, "
             "const gkg::Polyhedron::Neighbors& neighbors )" );

}


void gkg::Polyhedron::addVertex( const gkg::Polyhedron::Vertex& vertex )
{

  try
  {

    _vertices.push_back( vertex );

  }
  GKG_CATCH( "void gkg::Polyhedron::addVertex( "
             "const gkg::Polyhedron::Vertex& vertex )" );

}


void gkg::Polyhedron::setVertexPosition(
                                        int32_t index,
                                        const gkg::Vector3d< float >& position )
{

  try
  {

    if ( index >= this->getVertexCount() )
    {

      throw std::runtime_error( "index is greater or equal to vertex count" );

    }
    _vertices[ index ].first = position;

  }
  GKG_CATCH( "void gkg::Polyhedron::setVertexPosition( "
             "int32_t index, "
             "const gkg::Vector3d< float >& position )" );

}


void gkg::Polyhedron::setVertexNeighbor( int32_t index,
                                         int32_t neighborCoordinate,
                                         int32_t neighborIndex )
{

  try
  {

    if ( index >= this->getVertexCount() )
    {

      throw std::runtime_error( "index is greater or equal to vertex count" );

    }
    getCoordinate( _vertices[ index ].second, neighborCoordinate ) =
                                                                  neighborIndex;

  }
  GKG_CATCH( "void gkg::Polyhedron::setVertexNeighbor( "
             "int32_t index, "
             "int32_t neighborCoordinate, "
             "int32_t neighborIndex )" );

}


void gkg::Polyhedron::clear()
{

  try
  {

    _vertices.clear();

  }
  GKG_CATCH( "void gkg::Polyhedron::clear()" );

}


int32_t gkg::Polyhedron::getVertexCount() const
{

  try
  {

    return ( int32_t )_vertices.size();

  }
  GKG_CATCH( "int32_t gkg::Polyhedron::getVertexCount() const" );

}


const gkg::Polyhedron::Vertex& gkg::Polyhedron::getVertex( int32_t index ) const
{

  try
  {

    return _vertices[ index ];

  }
  GKG_CATCH( "const gkg::Polyhedron::Vertex& "
             "gkg::Polyhedron::getVertex( int32_t index ) const" );

}


gkg::Polyhedron::Vertex& gkg::Polyhedron::getVertex( int32_t index )
{

  try
  {

    return _vertices[ index ];

  }
  GKG_CATCH( "gkg::Polyhedron::Vertex& "
             "gkg::Polyhedron::getVertex( int32_t index )" );

}


void gkg::Polyhedron::voxelize( const gkg::Vector3d< int32_t >& lowerVoxelBound,
                                const gkg::Vector3d< int32_t >& upperVoxelBound,
                                gkg::Volume< uint8_t >& grid ) const
{

  try
  {


    // getting grid resolution
    gkg::Vector3d< double > gridResolution( 1.0, 1.0, 1.0 );
    grid.getResolution( gridResolution );

    gkg::Polyhedron* polyhedronChildren[ 2 ];

    // computing grid size(s)
    gkg::Vector3d< int32_t > gridSize;
    gridSize.x = upperVoxelBound.x - lowerVoxelBound.x;
    gridSize.y = upperVoxelBound.y - lowerVoxelBound.y;
    gridSize.z = upperVoxelBound.z - lowerVoxelBound.z;

    if ( ( gridSize.x <= 0 ) ||
         ( gridSize.y <= 0 ) ||
         ( gridSize.z <= 0 ) )
    {

      throw std::runtime_error( "bad lower and upper bound(s)" );

    }
         
    std::vector< std::pair< gkg::Polyhedron, 
                            std::pair< gkg::Vector3d< int32_t >,
                                       gkg::Vector3d< int32_t > > > >
      stack( ( int32_t )( std::ceil( ::log2( gridSize.x ) ) +
                          std::ceil( ::log2( gridSize.y ) ) +
                          std::ceil( ::log2( gridSize.z ) ) +
                          1 ) );

    int32_t stackIndex = 0;

    stack[ stackIndex ].first = *this;
    stack[ stackIndex ].second.first = lowerVoxelBound;
    stack[ stackIndex ].second.second = upperVoxelBound;

    ++ stackIndex;

    int32_t maximumSize = 0;
    int32_t maximumCoordinate = 0;
    int32_t currentSize = 0;
    while ( stackIndex > 0 )
    {

      // pop the stack and skip the leaf if empty
      -- stackIndex;

      if ( stack[ stackIndex ].first.getVertexCount() != 0 )
      {


        // find the longest axis along which to split 
        maximumSize = 0;
        maximumCoordinate = 0;

        currentSize = stack[ stackIndex ].second.second.x -
                      stack[ stackIndex ].second.first.x;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 0;

        }

        currentSize = stack[ stackIndex ].second.second.y -
                      stack[ stackIndex ].second.first.y;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 1;

        }

        currentSize = stack[ stackIndex ].second.second.z -
                      stack[ stackIndex ].second.first.z;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 2;

        }

        // if all three axes are only one voxel long, reduce the single voxel to
        // the destination grid
        if ( maximumSize == 1 )
        {

          grid( stack[ stackIndex ].second.first.x - lowerVoxelBound.x,
                stack[ stackIndex ].second.first.y - lowerVoxelBound.y,
                stack[ stackIndex ].second.first.z - lowerVoxelBound.z ) = 1U;

        }
        else
        {

          // split the poly and push children to the stack
          polyhedronChildren[ 0 ] = &stack[ stackIndex ].first;
          polyhedronChildren[ 1 ] = &stack[ stackIndex + 1 ].first;

          float position = ( float )getCoordinate( gridResolution,
                                                   maximumCoordinate ) *
                           ( float )( getCoordinate(
                                               stack[ stackIndex ].second.first,
                                               maximumCoordinate ) +
                                      maximumSize / 2 );

          this->split( stack[ stackIndex ].first,
                       polyhedronChildren,
                       position,
                       maximumCoordinate );
                       
          stack[ stackIndex + 1 ].second = stack[ stackIndex ].second;

          getCoordinate( stack[ stackIndex ].second.second,
                         maximumCoordinate ) -= maximumSize -
                                                maximumSize / 2;
          getCoordinate( stack[ stackIndex + 1 ].second.first,
                         maximumCoordinate ) += maximumSize / 2;

          stackIndex += 2;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::Polyhedron::voxelize( "
             "const gkg::Vector3d< int32_t >& lowerVoxelBound, "
             "const gkg::Vector3d< int32_t >& upperVoxelBound, "
             "gkg::Volume< uint8_t >& grid ) const" );

}


void gkg::Polyhedron::voxelize(
                   const gkg::Vector3d< int32_t >& gridSize,
                   const gkg::Vector3d< float >& gridResolution,
                   std::set< gkg::Vector3d< int32_t >,
                             gkg::Vector3dCompare< int32_t > >& voxelSet ) const
{

  try
  {

    gkg::Polyhedron* polyhedronChildren[ 2 ];

    std::vector< std::pair< gkg::Polyhedron, 
                            std::pair< gkg::Vector3d< int32_t >,
                                       gkg::Vector3d< int32_t > > > >
      stack( ( int32_t )( std::ceil( ::log2( gridSize.x ) ) +
                          std::ceil( ::log2( gridSize.y ) ) +
                          std::ceil( ::log2( gridSize.z ) ) +
                          1 ) );

    int32_t stackIndex = 0;

    stack[ stackIndex ].first = *this;
    stack[ stackIndex ].second.first.x = 0;
    stack[ stackIndex ].second.first.y = 0;
    stack[ stackIndex ].second.first.z = 0;
    stack[ stackIndex ].second.second.x = gridSize.x - 1;
    stack[ stackIndex ].second.second.y = gridSize.y - 1;
    stack[ stackIndex ].second.second.z = gridSize.z - 1;

    ++ stackIndex;

    int32_t maximumSize = 0;
    int32_t maximumCoordinate = 0;
    int32_t currentSize = 0;
    while ( stackIndex > 0 )
    {

      // pop the stack and skip the leaf if empty
      -- stackIndex;

      if ( stack[ stackIndex ].first.getVertexCount() != 0 )
      {


        // find the longest axis along which to split 
        maximumSize = 0;
        maximumCoordinate = 0;

        currentSize = stack[ stackIndex ].second.second.x -
                      stack[ stackIndex ].second.first.x;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 0;

        }

        currentSize = stack[ stackIndex ].second.second.y -
                      stack[ stackIndex ].second.first.y;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 1;

        }

        currentSize = stack[ stackIndex ].second.second.z -
                      stack[ stackIndex ].second.first.z;
        if ( currentSize > maximumSize )
        {

          maximumSize = currentSize;
          maximumCoordinate = 2;

        }

        // if all three axes are only one voxel long, reduce the single voxel to
        // the destination grid
        if ( maximumSize == 1 )
        {

          voxelSet.insert( stack[ stackIndex ].second.first );

        }
        else
        {

          // split the poly and push children to the stack
          polyhedronChildren[ 0 ] = &stack[ stackIndex ].first;
          polyhedronChildren[ 1 ] = &stack[ stackIndex + 1 ].first;

          float position = ( float )getCoordinate( gridResolution,
                                                   maximumCoordinate ) *
                           ( float )( getCoordinate(
                                               stack[ stackIndex ].second.first,
                                               maximumCoordinate ) +
                                      maximumSize / 2 );

          this->split( stack[ stackIndex ].first,
                       polyhedronChildren,
                       position,
                       maximumCoordinate );
                       
          stack[ stackIndex + 1 ].second = stack[ stackIndex ].second;

          getCoordinate( stack[ stackIndex ].second.second,
                         maximumCoordinate ) -= maximumSize -
                                                maximumSize / 2;
          getCoordinate( stack[ stackIndex + 1 ].second.first,
                         maximumCoordinate ) += maximumSize / 2;

          stackIndex += 2;

        }

      }

    }

  }
  GKG_CATCH( "void gkg::Polyhedron::voxelize( "
             "const gkg::Vector3d< int32_t >& gridSize, "
             "const gkg::Vector3d< float >& gridResolution, "
             "std::set< gkg::Vector3d< int32_t >, "
             "gkg::Vector3dCompare< int32_t > >& voxelSet ) const" );

}


void gkg::Polyhedron::display() const
{

  try
  {

    gkg::Polyhedron::const_iterator
      v = _vertices.begin(),
      ve = _vertices.end();
    while ( v != ve )
    {

      std::cout << "position=" << v->first << " neighbors=" << v->second
                << std::endl;
      ++ v;

    }


  }
  GKG_CATCH( "void gkg::Polyhedron::display() const" );

}


void gkg::Polyhedron::split( gkg::Polyhedron& inputPolyhedron,
                             gkg::Polyhedron** outputPolyhedrons,
                             float position,
                             int32_t maximumCoordinate ) const
{

  try
  {

    int32_t vertexCount = inputPolyhedron.getVertexCount();

    if ( vertexCount == 0 )
    {

      return;

    }

    // calculate signed distances to the clip plane
    int32_t rightCount = 0;

    std::vector< int32_t > sides( vertexCount, 0 );
    std::vector< float > signedDistances( vertexCount );

    int32_t vertexIndex = 0;
    for ( vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++ )
    {

      signedDistances[ vertexIndex ] =
                  position -
                  getCoordinate( inputPolyhedron.getVertex( vertexIndex ).first,
                                 maximumCoordinate );
      if ( signedDistances[ vertexIndex ] < 0.0 )
      {

        sides[ vertexIndex ] = 1;
        ++ rightCount;

      }

    }
    
    // return if the poly lies entirely on one side of it
    if ( rightCount == 0 )
    {

      *( outputPolyhedrons[ 0 ] )= inputPolyhedron;
      outputPolyhedrons[ 1 ]->clear();
      return;

    }
    if ( rightCount == vertexCount )
    {

      *( outputPolyhedrons[ 1 ] )= inputPolyhedron;
      outputPolyhedrons[ 0 ]->clear();
      return;

    }

    // check edges and insert new vertices on the bisected edges
    int32_t oldVertexCount = vertexCount;
    int32_t neighborCoordinate = 0;
    int32_t nextVertexIndex = 0;
    int32_t nextNeighborCoordinate = 0;
    gkg::Vector3d< float > newPosition;

    for ( vertexIndex = 0; vertexIndex < oldVertexCount; vertexIndex++ )
    {

      if ( !sides[ vertexIndex ] )
      {

        for ( neighborCoordinate = 0; neighborCoordinate < 3;
              neighborCoordinate++ )
        {

          nextVertexIndex = getCoordinate(
                                inputPolyhedron.getVertex( vertexIndex ).second,
                                neighborCoordinate );

          if ( sides[ nextVertexIndex ] )
          {

            float weight1 = -signedDistances[ nextVertexIndex ];
            float weight2 = signedDistances[ vertexIndex ];
            float sumOfWeights = weight1 + weight2;

            newPosition =
                       inputPolyhedron.getVertex( vertexIndex ).first *
                       ( weight1 / sumOfWeights ) +
                       inputPolyhedron.getVertex( nextVertexIndex ).first *
                       ( weight2 / sumOfWeights );

            inputPolyhedron.addVertex( newPosition,
                                       vertexIndex,
                                       0,
                                       0 );
            inputPolyhedron.setVertexNeighbor( vertexIndex,
                                               neighborCoordinate,
                                               vertexCount );
            ++ vertexCount;

            sides.push_back( 0 );
            inputPolyhedron.addVertex( newPosition,
                                       nextVertexIndex,
                                       0,
                                       0 );
            sides.push_back( 1 );
            for ( nextNeighborCoordinate = 0; nextNeighborCoordinate < 3;
                  nextNeighborCoordinate++ )
            {

              if ( getCoordinate( inputPolyhedron.getVertex(
                                                       nextVertexIndex ).second,
                                  nextNeighborCoordinate ) == vertexIndex )
              {

                break;

              }

            }
            getCoordinate( inputPolyhedron.getVertex( nextVertexIndex ).second,
                           nextNeighborCoordinate ) = vertexCount;

            ++ vertexCount;

          }

        }

      }

    }


    // for each new vertex, search around the faces for its new neighbors
    // and doubly-link everything
    int32_t newVertexIndex = 0;
    for ( newVertexIndex = oldVertexCount; newVertexIndex < vertexCount;
          newVertexIndex++ )
    {

      vertexIndex = newVertexIndex;
      nextVertexIndex = getCoordinate(
                                inputPolyhedron.getVertex( vertexIndex ).second,
                                0 );

      do
      {

        for ( neighborCoordinate = 0; neighborCoordinate < 3;
              neighborCoordinate++ )
        {

          if ( getCoordinate( inputPolyhedron.getVertex(
                                                   nextVertexIndex ).second,
                              neighborCoordinate ) == vertexIndex )
          {

            break;

          }

        }

        vertexIndex = nextVertexIndex;
        nextNeighborCoordinate = ( neighborCoordinate + 1 ) % 3;
        nextVertexIndex = getCoordinate( inputPolyhedron.getVertex(
                                                           vertexIndex ).second,
                                          nextNeighborCoordinate );

      }
      while ( vertexIndex < oldVertexCount );

      inputPolyhedron.setVertexNeighbor( newVertexIndex,
                                         2,
                                         vertexIndex );
      inputPolyhedron.setVertexNeighbor( vertexIndex,
                                         1,
                                         newVertexIndex );

    }

    // copy and compress vertices into their new buffers
    oldVertexCount = vertexCount;

    outputPolyhedrons[ 0 ]->clear();
    outputPolyhedrons[ 1 ]->clear();

    outputPolyhedrons[ 0 ]->reserve( oldVertexCount );
    outputPolyhedrons[ 1 ]->reserve( oldVertexCount );

    int32_t side = 0;
    for ( vertexIndex = 0; vertexIndex < oldVertexCount; vertexIndex++ )
    {

      side = sides[ vertexIndex ];
      sides[ vertexIndex ] = outputPolyhedrons[ side ]->getVertexCount();
      outputPolyhedrons[ side ]->addVertex(
                                     inputPolyhedron.getVertex( vertexIndex ) );

    }

    int32_t vertexCount0 = outputPolyhedrons[ 0 ]->getVertexCount();
    int32_t neighborVertexIndex = 0;
    for ( vertexIndex = 0; vertexIndex < vertexCount0; vertexIndex++ )
    {

      for ( neighborCoordinate = 0; neighborCoordinate < 3;
            neighborCoordinate++ )
      {

        neighborVertexIndex =
          sides[ getCoordinate(
                        outputPolyhedrons[ 0 ]->getVertex( vertexIndex ).second,
                        neighborCoordinate ) ];
        outputPolyhedrons[ 0 ]->setVertexNeighbor( vertexIndex,
                                                   neighborCoordinate,
                                                   neighborVertexIndex );

      }

    }

    int32_t vertexCount1 = outputPolyhedrons[ 1 ]->getVertexCount();
    for ( vertexIndex = 0; vertexIndex < vertexCount1; vertexIndex++ )
    {

      for ( neighborCoordinate = 0; neighborCoordinate < 3;
            neighborCoordinate++ )
      {

        neighborVertexIndex =
          sides[ getCoordinate(
                        outputPolyhedrons[ 1 ]->getVertex( vertexIndex ).second,
                        neighborCoordinate ) ];
        outputPolyhedrons[ 1 ]->setVertexNeighbor( vertexIndex,
                                                   neighborCoordinate,
                                                   neighborVertexIndex );

      }

    }

  }
  GKG_CATCH( "void gkg::Polyhedron::split( "
             "const gkg::Polyhedron& inputPolyhedron, "
             "gkg::Polyhedron** outputPolyhedrons, "
             "float position, "
             "int32_t maximumCoordinate ) const" );

}
 
