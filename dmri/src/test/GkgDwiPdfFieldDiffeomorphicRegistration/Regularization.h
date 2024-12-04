/* Incremental diffusion regularisation of parametrised transformation
 using (globally optimal) belief-propagation on minimum spanning tree.
 Fast distance transform  uses squared differences.
 Similarity cost for each node and label has to be given as input.
*/

void minimumIndA( std::vector< float >& numbers, 
                  float& value, 
                  int32_t& index, 
                  int32_t& length )
{

  try
  {
	
    value=numbers[ 0 ];
    int32_t i = 0;
    index=0;
    for( i = 1; i < length; i++ )
    {
    
      if(numbers[ i ] < value )
      {

        index=i;
	value=numbers[i];
	
      }

    }

   }
   GKG_CATCH( "void minimumIndA( std::vector< float >& numbers, " 
              "float& value, int32_t& index, int32_t& length )" );

}


void dt1sq( std::vector< float >& val, 
            std::vector< int32_t >& ind,
            int32_t index,
            int32_t len,
            float offset,
            int32_t k,
            std::vector< int32_t >& v,
            std::vector< float >& z,
            std::vector< float >& f,
            std::vector< int32_t >& ind1 )
{

  try
  {
	
    float INF=1e10;

    int32_t j=0;
    z[ 0 ] = -INF;
    z[ 1 ] = INF;
    v[ 0 ] = 0;
 
    int32_t q = 0;
    for ( q = 1; q < len; ++q )
    {
	
      float s = ( ( val[ index + q * k ] + 
                    std::pow( ( float )q + offset, 2.0 ) ) -
                    ( val[ index + v[ j ] * k ] + 
                    std::pow( ( float )v[ j ] + offset, 2.0 ) ) ) / 
                                             ( 2.0 * ( float )( q - v[ j ] ) );
		
      while ( s <= z[ j ] )
      {
	
        --j;
		
	s = ( ( val[ index + q * k ] + 
                    std::pow( ( float )q + offset, 2.0 ) ) -
                    ( val[ index + v[ j ] * k ] + 
                    std::pow( ( float )v[ j ] + offset, 2.0 ) ) ) / 
                                             ( 2.0 * ( float )( q - v[ j ] ) );
      }
		
      ++j;
      v[ j ] = q;
      z[ j ] = s;
      z[ j + 1 ] = INF;
	
    }
	
    j = 0;
    for( q = 0; q < len; ++q )
    {
	
      f[ q ]=val[ index + q * k ];
      ind1[ q ]=ind[ index + q * k];
	
    } 
	
    for( q = 0; q < len; ++q )
    {
	
      while( z[ j + 1 ] < q )
      {
		
	++j;

      }
		
      ind[ index + q * k ] = ind1[ v[ j ] ];		
      val[ index + q * k ]= std::pow( 
                    ( float )q-( ( float )v[ j ] + offset), 2.0) + f[ v[ j ] ];
	
    }

  }
  GKG_CATCH( "void dt1sq( std::vector< float >& val," 
             "std::vector< int32_t >& ind,"
             "int32_t& index,"
             "int32_t& len,"
             "float& offset,"
             "int32_t& k,"
             "std::vector< int32_t >& v,"
             "std::vector< float >& z,"
             "std::vector< float >& f,"
             "std::vector< int32_t >& i1 )" );

}


void dt3x( std::vector< float >& r, 
           std::vector< int32_t >& indicesr,
           int32_t& rl,
           float& dx,
           float& dy,
           float& dz )
{

  try
  {
	
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    //rl is length of one side
    for( i = 0; i < rl * rl * rl ; ++i )
    {
	
      indicesr[ i ] = i;
	
    }
	
    std::vector< int32_t > v( rl, 0 );
    std::vector< float > z( rl + 1, 0 );
    std::vector< float > f( rl, 0 );
    std::vector< int32_t > i1( rl, 0 ); 

    for( k = 0; k < rl; ++k )
    {
	
      for( i = 0; i < rl; ++i )
      {
	
        dt1sq( r,
               indicesr, i + k * rl * rl,
               rl, -dx, rl, v, z, f, i1 );

      }

    }

    for( k = 0; k < rl; ++k )
    {
	
      for( j = 0; j < rl; ++j )
      {
	
        dt1sq( r,
               indicesr, j * rl + k * rl * rl,
               rl, -dy, 1, v, z, f, i1 );

      }

    }
	
    for( j = 0; j < rl; ++j )
    {
	
      for( i = 0; i < rl; ++i )
      {
	
        dt1sq( r,
               indicesr, i + j * rl,
               rl, -dz, rl * rl, v, z, f, i1 );

      }

    }
	
  }
  GKG_CATCH("void dt3x( std::vector< float >& r," 
            "std::vector< int32_t >& indicesr,"
            "int32_t& rl,"
            "float& dx,"
            "float& dy,"
            "float& dz )" );
	
}


void regularisation( gkg::Volume< float >& scaledFlowFieldX,
                     gkg::Volume< float >& scaledFlowFieldY,
                     gkg::Volume< float >& scaledFlowFieldZ,
                     gkg::Volume< float >& currentFlowFieldX,
                     gkg::Volume< float >& currentFlowFieldY,
                     gkg::Volume< float >& currentFlowFieldZ,
                     std::vector< float >& totalCost,
                     float& /*regularizationWeighting*/,
                     int32_t& currentHalfWidthSearchSpace,
                     int32_t& currentControlPointsSpacing,
                     float& currentSearchSpaceQuantisation,  
                     std::vector< int32_t >& orderedMST,
                     std::vector< int32_t >& parentsMST,
                     gkg::Vector3d< int32_t >& sizeVector,
                     std::set< gkg::Vector3d< int32_t >, 
                           gkg::Vector3dCompare< int32_t > > hasSiteFloating )
{

  try
  {
	
std::cout << "regularization 1" << std::endl;
	
    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    //int32_t size = sizeX * sizeY * sizeZ;

    int32_t currentSizeX = sizeX / currentControlPointsSpacing;
    int32_t currentSizeY = sizeY / currentControlPointsSpacing; 
    int32_t currentSizeZ = sizeZ / currentControlPointsSpacing; 
    int32_t currentSize = currentSizeX * currentSizeY * currentSizeZ;

    // dense displacement space
    int32_t currentSearchSpace = currentHalfWidthSearchSpace * 2 + 1;
    int32_t cubedCurrentSearchSpace = std::pow( currentSearchSpace, 3 );
    std::vector< float > xs( cubedCurrentSearchSpace );
    std::vector< float > ys( cubedCurrentSearchSpace );
    std::vector< float > zs( cubedCurrentSearchSpace );
	
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t l = 0;
    //int32_t i1 = 0;
    //int32_t j1 = 0;

std::cout << "regularization 2" << std::endl;

    for ( i = 0; i < currentSearchSpace; ++i )
    {
		
      for ( j = 0; j < currentSearchSpace; ++j )
      {
			
        for ( k = 0; k < currentSearchSpace; ++k )
        {
					
          xs[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( j - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

          ys[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( i - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

          zs[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( k - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

        }
		
      }
	
    }
	
std::cout << "regularization 3" << std::endl;

    std::vector< int32_t > selected( currentSize, 0 );
    std::vector< int32_t > indices( cubedCurrentSearchSpace, 0 ); 
    std::vector< int32_t > allIndices( currentSize*cubedCurrentSearchSpace, 0 );
    std::vector< float > cost1( cubedCurrentSearchSpace, 0 );

    //int32_t xs1 = 0;
    //int32_t ys1 = 0;
    //int32_t zs1 = 0;
    //int32_t xx = 0;
    //int32_t yy = 0;
    //int32_t zz = 0;
    //int32_t xx2 = 0;
    //int32_t yy2 = 0;
    //int32_t zz2 = 0;

    int32_t oroot = 0;
    int32_t ochild = 0;
    int32_t oparent = 0;
    int32_t x1 = 0;
    int32_t y1 = 0;
    int32_t z1 = 0;
    int32_t x2 = 0;
    int32_t y2 = 0;
    int32_t z2 = 0;

std::cout << "regularization 3_1" << std::endl;

    //calculate mst cost (loop on each control point)
    for( i = ( currentSize - 1 ); i > 0; --i )
    { 
		
      if( ( i % ( int32_t )( currentSize / 25 ) ) == 0 )
      {
	
        std::cout << "x" << std::flush;
		
      }
		
      ochild = orderedMST[ i ];
      oparent = parentsMST[ orderedMST[ i ] ];
      z1 = ochild / ( currentSizeX * currentSizeY );
      x1 = ( ochild - z1 * currentSizeX * currentSizeY ) / currentSizeX;
      y1 = ochild - z1 * currentSizeX * currentSizeY - x1 * currentSizeX;
      z2 = oparent / ( currentSizeX * currentSizeY );
      x2 = ( oparent - z2 * currentSizeX * currentSizeY ) / currentSizeX;
      y2 = oparent - z2 * currentSizeX * currentSizeY - x2 * currentSizeX;

      for( l = 0; l < cubedCurrentSearchSpace; ++l )
      {
	
        cost1[ l ] = totalCost[ ochild + l * currentSize ];
	
      }
		
      //important for INCREMENTAL regularisation (offset delta)
      float dx1 = ( currentFlowFieldX( y2, x2, z2 )  
                  - currentFlowFieldX( y1, x1, z1 ) ) / 
                                       ( float )currentSearchSpaceQuantisation;

      float dy1 = ( currentFlowFieldY( y2, x2, z2 ) 
                  - currentFlowFieldY( y1, x1, z1 ) ) / 
                                       ( float )currentSearchSpaceQuantisation;

      float dz1 = ( currentFlowFieldZ( y2, x2, z2 ) 
                  - currentFlowFieldZ( y1, x1, z1 ) ) / 
                                       ( float )currentSearchSpaceQuantisation;

		
      //fast distance transform see fastDT2.h
      dt3x( cost1, indices, currentSearchSpace, dx1, dy1, dz1);
		
      //argmin for each label
      for( l = 0; l < cubedCurrentSearchSpace; ++l )
      {

        allIndices[ y1 + x1 * currentSizeX + 
                    z1 * currentSizeX * currentSizeY + l * currentSize ] = 
                                                                  indices[ l ];
	
      }
		
      //add mincost to parent node
      for( l = 0; l < cubedCurrentSearchSpace; ++l )
      {
	
        totalCost[ oparent + l * currentSize ] += cost1[ l ];
		
      }
		
    }
	
std::cout << "regularization 4" << std::endl;

    //mst cost and select displacement for root node
    oroot = orderedMST[ i ];
    z1=oroot / ( currentSizeX * currentSizeY );
    x1 = ( oroot - z1 * currentSizeX * currentSizeY ) / currentSizeX;
    y1 = oroot - z1 * currentSizeX * currentSizeY - x1 * currentSizeX;

    for( l = 0; l < cubedCurrentSearchSpace; ++l )
    {
		
      cost1[ l ] = totalCost[ oroot + l * currentSize ];
	
    }

std::cout << "regularization 5" << std::endl;

    float value = 0.0f; 
    int32_t index = 0;
    minimumIndA( cost1, value, index, cubedCurrentSearchSpace );

    for( l = 0; l < cubedCurrentSearchSpace; ++l )
    {
	
      allIndices[ y1 + x1 * currentSizeX + z1 * currentSizeX * currentSizeY +
                                                         l * currentSize ] = l;
	
    }
	
std::cout << "regularization 6" << std::endl;

    selected[ y1 + x1 * currentSizeX + z1 * currentSizeX * currentSizeY ] = 
                                                                         index;

    gkg::Vector3d< int32_t > xyz( x1, y1, z1 );
    std::set< gkg::Vector3d< int32_t >, 
                            gkg::Vector3dCompare< int32_t > >::iterator h;

    h = hasSiteFloating.find( xyz );

    if ( h != hasSiteFloating.end() )
    {

      scaledFlowFieldX( x1, y1, z1 ) = 
                   xs[ index ] + currentFlowFieldX( y1, x1, z1 );
      scaledFlowFieldY( x1, y1, z1 ) = 
                   ys[ index ] + currentFlowFieldY( y1, x1, z1 );
      scaledFlowFieldZ( x1, y1, z1 ) = 
                   zs[ index ] + currentFlowFieldZ( y1, x1, z1 );
	
    }	

std::cout << "regularization 7" << std::endl;

    //select displacements and add to previous deformation field
    for( i = 1; i < currentSize; ++i )
    {

      ochild = orderedMST[ i ];
      oparent = parentsMST[ orderedMST[ i ] ];
      z1 = ochild / ( currentSizeX * currentSizeY );
      x1 = ( ochild - z1 * currentSizeX * currentSizeY ) / currentSizeX;
      y1 = ochild - z1 * currentSizeX * currentSizeY - x1 * currentSizeX;
      z2 = oparent / ( currentSizeX * currentSizeY );
      x2 = ( oparent - z2 * currentSizeX * currentSizeY ) / currentSizeX;
      y2 = oparent - z2 * currentSizeX * currentSizeY - x2 * currentSizeX;

      //select from argmin of based on parent selection
      index = allIndices[ y1 + x1 * currentSizeX + 
                              z1 * currentSizeX * currentSizeY + 
                          selected[ y2 + x2 * currentSizeX + 
                          z2 * currentSizeX * currentSizeY ] * 
                                  currentSizeX * currentSizeY * currentSizeZ ];
      selected[ y1 + x1 * currentSizeX + z1 * currentSizeX * currentSizeY ] = 
                                                                         index;

      gkg::Vector3d< int32_t > xyz( y1, x1, z1 );
      std::set< gkg::Vector3d< int32_t >, 
                            gkg::Vector3dCompare< int32_t > >::iterator h;

      h = hasSiteFloating.find( xyz );

      if ( h != hasSiteFloating.end() )
      {

        scaledFlowFieldX( y1, x1, z1 ) = 
                   xs[ index ] + currentFlowFieldX( y1, x1, z1 );
        scaledFlowFieldY( y1, x1, z1 ) = 
                   ys[ index ] + currentFlowFieldY( y1, x1, z1 );
        scaledFlowFieldZ( y1, x1, z1 ) = 
                   zs[ index ] + currentFlowFieldZ( y1, x1, z1 );

      }
		
    }

  }
  GKG_CATCH( "void regularisation( gkg::Volume< float >&"
             "scaledFlowFieldX,"
             "gkg::Volume< float >& scaledFlowFieldX,"
             "gkg::Volume< float >& scaledFlowFieldX,"
             "gkg::Volume< float >& currentFlowFieldX,"
             "gkg::Volume< float >& currentFlowFieldX,"
             "gkg::Volume< float >& currentFlowFieldX,"
             "std::vector< float >& totalCost,"
             "float& regularizationWeighting,"
             "int32_t& currentHalfWidthSearchSpace,"
             "int32_t& currentControlPointsSpacing,"
             "float& currentSearchSpaceQuantisation,"  
             "std::vector< int32_t >& orderedMST,"
             "std::vector< int32_t >& parentsMST,"
             "gkg::Vector3d< int32_t >& sizeVector )" );
	
}
