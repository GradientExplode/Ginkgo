#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>

// Minimum spanning tree calculation using Prim's algorithm.
// Average run-time should be of n*log(n) complexity.
// Uses heap data structure to speed-up finding the next lowest edge-weight.
// Edge-weights. 

class Edge
{

  public:
  
    double weight;
    int32_t vertex1;
    int32_t vertex2;
    Edge( double weight = 0.0, int32_t vertex1 = 0, int32_t vertex2 = 0 );
    bool operator<( const Edge & other ) const;

};

Edge::Edge(double weight,int32_t vertex1, int32_t vertex2 )
{

  try
  {
	
    this->weight = weight;
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;

  }
  GKG_CATCH("Edge::Edge(double weight,int32_t vertex1, int32_t vertex2 )");

}


bool Edge::operator<(const Edge & other) const
{

  try
  {

    return ( this->weight > other.weight);

  }
  GKG_CATCH("bool Edge::operator<(const Edge & other) const");

}


int32_t newEdge( Edge& edge, Edge& newEdge, std::vector< bool >& vertices )
{

  try
  {

    bool new1 = vertices[ edge.vertex1 ]; 
    bool new2 = vertices[ edge.vertex2 ];
    int32_t newVertex;

    if ( new1^new2 )
    {
		
      if( new1 )
      {
			
        newVertex = edge.vertex2;
        newEdge = Edge( edge.weight, edge.vertex1, edge.vertex2 );
		
      }
      else 
      {
			
        newVertex = edge.vertex1;
        newEdge = Edge( edge.weight, edge.vertex2, edge.vertex1 );
		
      }

    }
    else
    {
		
      newVertex=-1;
	
    }
    return newVertex;

  }
  GKG_CATCH("bool Edge::operator<(const Edge & other) const");

}


void primsGraph( gkg::PdfCartesianField& floating,
                 std::set< gkg::Vector3d< int32_t >, 
                           gkg::Vector3dCompare< int32_t > > hasSiteFloating, 
                 gkg::Vector3d< int32_t >& sizeVector,
                 std::vector< int32_t >& orderedMST,
                 std::vector< int32_t >& parentsMST, 
                 int32_t& currentControlPointsSpacing )
{

  try
  {

    int32_t currentSizeX = sizeVector.x / currentControlPointsSpacing;
    int32_t currentSizeY = sizeVector.y / currentControlPointsSpacing; 
    int32_t currentSizeZ = sizeVector.z / currentControlPointsSpacing;  

    int32_t numberOfVertices = currentSizeX * currentSizeY * currentSizeZ;
    int32_t numberOfNeighbours = 6;
    int32_t numberOfVerticesTimesNumberOfNeighbours = numberOfVertices *
                                                      numberOfNeighbours;
	
    std::vector< float > edgeCost( numberOfVerticesTimesNumberOfNeighbours, 
                                                                        0.0f ); 
    std::vector< int32_t > neighboursIndex( 
                                   numberOfVerticesTimesNumberOfNeighbours, 
                                                                          -1 );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t i1 = 0;
    int32_t j1 = 0;
    int32_t k1 = 0;
    int32_t neighbour = 0;
	
    int32_t dx[ 6 ] = {-1,1,0,0,0,0};
    int32_t dy[ 6 ] = {0,0,-1,1,0,0};
    int32_t dz[ 6 ] = {0,0,0,0,-1,1};
    int32_t xx,yy,zz,xx2,yy2,zz2;

    //calculate edge-weights based on similarity measure of groups of voxels 
    // (for each control point)

    for ( k = 0; k < currentSizeZ; ++k )
    {
 
      for ( j = 0; j < currentSizeY; ++j )
      {

        for ( i = 0; i < currentSizeX; ++i )
        {

          for ( neighbour = 0; neighbour < numberOfNeighbours; ++neighbour )
          {

            if ( ( ( i + dy[ neighbour ] ) >= 0 ) & 
                 ( ( i + dy[ neighbour ] ) < currentSizeX ) & 
                 ( ( j + dx[ neighbour ] ) >= 0 ) & 
                 ( ( j + dx[ neighbour ] ) < currentSizeY ) & 
                 ( ( k + dz[ neighbour ] ) >= 0 )& 
                 ( ( k + dz[ neighbour ]) < currentSizeZ ) )
            {
					
						
              neighboursIndex[ i + j*currentSizeX + 
                               k*currentSizeX*currentSizeY +
                               neighbour*numberOfVertices ] = 
                             i + dy[ neighbour ] + 
                             ( j + dx[ neighbour ] )*currentSizeX + 
                             ( k + dz[ neighbour ] )*currentSizeX*currentSizeY;

              for ( k1 = 0; k1 < currentControlPointsSpacing; ++k1 )
              {
							
                for ( j1 = 0; j1 < currentControlPointsSpacing; ++j1 )
                {
								
                  for ( i1 = 0; i1 < currentControlPointsSpacing; ++i1 )
                  {
									
                    xx = j * currentControlPointsSpacing + j1;
                    yy = i * currentControlPointsSpacing + i1;
                    zz = k * currentControlPointsSpacing + k1;
                    xx2 = ( j + dx[ neighbour ] ) * 
                                             currentControlPointsSpacing + j1;
                    yy2 = ( i + dy[ neighbour ] ) * 
                                             currentControlPointsSpacing + i1;
                    zz2 = ( k + dz[ neighbour ] ) * 
                                             currentControlPointsSpacing + k1;

                    gkg::Vector3d< int32_t > xyz( yy, xx, zz );
                    gkg::Vector3d< int32_t > xyz2( yy2, xx2, zz2 );

                    std::set< gkg::Vector3d< int32_t >, 
                              gkg::Vector3dCompare< int32_t > >::iterator h1,h2;

                    h1 = hasSiteFloating.find( xyz );
                    h2 = hasSiteFloating.find( xyz2 );

                    if ( h1 != hasSiteFloating.end() &&
                         h2 != hasSiteFloating.end() )
                    {

                      gkg::ProbabilityDensityFunction* pdf1 = 
                        floating.getItemFast( *h1 );

                      gkg::ProbabilityDensityFunction* pdf2 = 
                        floating.getItemFast( *h2 );

                      edgeCost[ i + j*currentSizeX + 
                                    k*currentSizeX*currentSizeY + 
                                    neighbour*numberOfVertices ] +=
                        pdf1->getDissimilarity( *pdf2, 0.0f, 100.0f );

                    }
                    /*
                    else          
                    {
                        
                      edgeCost[ i + j*currentSizeX + 
                                k*currentSizeX*currentSizeY + 
                                neighbour*numberOfVertices ] = INF;
                 
                    }  
                    */                   

                  }

                }

              }

            }
 
          }

        }

      }

    }   

    float centreX = ( float )( currentSizeY / 2 );
    float centreY = ( float )( currentSizeX / 2 );
    float centreZ = ( float )( currentSizeZ / 2 );
		
    int32_t root = centreY + 
                   centreX * currentSizeX + 
                   centreZ * currentSizeX * currentSizeY;
	
    std::vector< Edge > priority;
    std::vector< bool > vertices( numberOfVertices, false );
    std::vector< int32_t > level( numberOfVertices, 0 );

    level[ root ] = 0;
    vertices[ root ] = true;

    parentsMST.resize( numberOfVertices, -1 );


    int32_t last = root;
    Edge edgeOut = Edge( 0.0, -1, -1);
    Edge minEdge = Edge( 0.0, -1, -1);
    float cost = 0.0f;


    //run n-1 times to have all vertices added
    for( i = 0; i < numberOfVertices - 1; ++i )
    { 

      //add edges of new vertex to priority queue
      for( j = 0; j < numberOfNeighbours; ++j )
      {
			
        int32_t n = neighboursIndex[ last + j * numberOfVertices ];
	if( n >= 0 )
        {


          //std::cout << "edge cost = " 
          //          << edgeCost[ last + j * numberOfVertices ] 
          //          << std::endl;

          priority.push_back( 
                    Edge( edgeCost[ last + j * numberOfVertices ], last, n ) );

          // push_heap inserts the element at the position last-1 
          // into the max heap defined by the range [first, last-1]
	  push_heap( priority.begin(), priority.end() );

        }

      }

      last=-1;

      //find valid edge with lowest weight (main step of Prim's algorithm)
      while( last == -1 )
      {

        minEdge=priority.front(); // returns a ref to the 1st element 
        pop_heap( priority.begin(), priority.end() );

        // swaps the value in the position first and the value in 
        // the position last-1 and makes the subrange [first, last-1) 
        // into a max heap. This has the effect of removing the first 
        // (largest) element from the heap defined by the range [first, last)
        priority.pop_back();
 
        //is either vertex already part of MST?
        //bool new1 = vertices[ minEdge.vertex1 ]; 
        //bool new2 = vertices[ minEdge.vertex2 ];

        // return next valid vertex or -1 if edge exists already
        last = newEdge( minEdge, edgeOut, vertices );
                   //std::cout << "last : " << last << std::endl;

      }
		
      cost += edgeOut.weight;
      vertices[ last ] = true;


      level[ edgeOut.vertex2 ]= level[ edgeOut.vertex1 ] + 1;
      parentsMST[ edgeOut.vertex2 ] = edgeOut.vertex1;

    }
	

    // find correct ordering in constant time
    int32_t maxLevel=0;
    for( i = 0; i < numberOfVertices; ++i )
    {
		
      if( level[ i ] > maxLevel )
      {
			
        maxLevel=level[i];

      }

    }


    maxLevel++;
    std::vector< int32_t > levelOffset( maxLevel, 0 );
    std::vector< int32_t > levelCount( maxLevel, 0 );

    // counting the number of vertices in each level
    for( i = 0; i < numberOfVertices; ++i )
    {
		
      if( level[ i ] < maxLevel - 1 )
      {
			
        levelOffset[ level[ i ] + 1 ]++; 
  
      }

    }


    for(  i = 1; i < maxLevel; ++i )
    {
		
      levelOffset[ i ] += levelOffset[ i - 1 ]; //cumulative sum

    }

    for( i = 0; i < numberOfVertices; ++i )
    {
		
      int32_t num = levelOffset[ level[ i ] ] + levelCount[ level[ i ] ];
                   //std::cout << "num : " << num << std::endl;
      levelCount[ level[ i ] ]++;
      orderedMST[ num ] = i;

    }
	
	
  }
  GKG_CATCH("bool Edge::operator<(const Edge & other) const");	
	
}



