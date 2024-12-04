#include <gkg-processing-graph/DijkstraAlgorithm.h>
#include <gkg-core-exception/Exception.h>
#include <limits>


gkg::DijkstraAlgorithm::DijkstraAlgorithm(
   const std::vector< std::list< std::pair< int32_t, float > > >& 
                                                                  adjacencyList,
   const std::list< int32_t >& sourceVertices )
{
  
  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // initializing vectors
    ////////////////////////////////////////////////////////////////////////////

    int32_t vertexCount = ( int32_t )adjacencyList.size();

    _distances = std::vector< float >( vertexCount,
                                        std::numeric_limits< float >::max() );
    std::vector< bool > isVisited( vertexCount, false );
    std::vector< int32_t > path( vertexCount, -1 );

    std::list< int32_t >::const_iterator
      sv = sourceVertices.begin(),
      sve = sourceVertices.end();
    while ( sv != sve )
    {

      _distances[ *sv ] = 0;
      ++ sv;

    }
    
    int32_t i = 0;
    for( i = 0; i < vertexCount; i++ )                           
    {
      
      // get index of the non-visited vertex with the minimum distance
      int32_t minDistance = std::numeric_limits< int32_t >::max();
      int32_t selectedVertexIndex = 0;

      std::vector< float >::const_iterator
        d = _distances.begin(),
        de = _distances.end();
      std::vector< bool >::const_iterator
        iv = isVisited.begin();
      int32_t vertexIndex = 0;
      while ( d != de )
      {

        if( !( *iv ) && ( *d <= minDistance ) )      
        {

          minDistance = *d;
          selectedVertexIndex = vertexIndex;

        }
        ++ vertexIndex;
        ++ d;
        ++ iv;

      }

      // marking selected vertex as visited
      isVisited[ selectedVertexIndex ] = true; 

      std::list< std::pair< int32_t, float > >::const_iterator
        n = adjacencyList[ selectedVertexIndex ].begin(),
        ne = adjacencyList[ selectedVertexIndex ].end();
      while( n != ne )
      {

        // Updating the minimum distance of other vertices
        if( !isVisited[ n->first ] &&
            ( ( _distances[ selectedVertexIndex ] + n->second ) <
                _distances[ n->first ] ) )
        {

          _distances[ n->first ] = _distances[ selectedVertexIndex ] +
                                   n->second;
          path[ n->first ] = selectedVertexIndex;

        }
        ++ n;

      }

    }
    
  }
  GKG_CATCH( "gkg::DijkstraAlgorithm::DijkstraAlgorithm( "
             "const std::vector< std::list< std::pair< int32_t, float > > >& "
             "adjacencyList, "
             "const std::list< int32_t >& sourceVertices )" );

}


gkg::DijkstraAlgorithm::~DijkstraAlgorithm()
{
}


const std::vector< float >& gkg::DijkstraAlgorithm::getDistances() const
{

  try
  {

    return _distances;

  }
  GKG_CATCH( "const std::vector< float >& "
             "gkg::DijkstraAlgorithm::getDistances() const" );

}
