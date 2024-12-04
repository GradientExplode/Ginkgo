/* several functions to interpolate and symmetrise deformations (as well as make them diffeomorphic)
 calculates Jacobian and harmonic Energy */

void filter1( gkg::Volume< float >& volumeIn,
              gkg::Volume< float >& volumeOut,
              gkg::Vector3d< int32_t >& sizeVector,
              float* filter,
              int32_t length,
              int32_t dimension )
{

  try
  {
                
    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t i1 = 0;
    int32_t j1 = 0;
    int32_t k1 = 0;
    int32_t l = 0;
    int32_t halfWidth = ( int32_t )( ( length - 1 ) / 2 );
	
    volumeOut.fill( 0.0f );
	
    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          for ( l = 0; l < length; ++l )
          {

            if ( dimension == 1 )
            {

              volumeOut( i, j, k ) += filter[ l ] * 
                                      volumeIn( std::max( 
                                                  std::min( 
                                                    i + l - halfWidth, 
                                                    sizeX - 1 ), 0 ), j, k ); 

            }
            else if ( dimension == 2 )
            {

              volumeOut( i, j, k ) += filter[ l ] * 
                                      volumeIn( i, std::max( 
                                                      std::min( 
                                                        j + l - halfWidth, 
                                                      sizeY - 1 ), 0 ), k );

            }
            else if ( dimension == 3 )
            {

              volumeOut( i, j, k ) += filter[ l ] * 
                                      volumeIn( i, j, std::max( 
                                                        std::min( 
                                                          k + l - halfWidth, 
                                                            sizeZ - 1 ), 0 ) );

            }   

          }

	}

      }

    }

  }
  GKG_CATCH( "void filter1( gkg::Volume< float >& volumeIn,"
             "gkg::Volume< float >& volumeOut,"
             "gkg::Vector3d< int32_t >& sizeVector,"
             "float* filter,"
             "int32_t length,"
             "int32_t dimension )" );

}


void fastInverse( gkg::Volume< float >& inverseFlowFieldX,
                  gkg::Volume< float >& inverseFlowFieldY,
                  gkg::Volume< float >& inverseFlowFieldZ,
                  gkg::Volume< float >& flowFieldX,
                  gkg::Volume< float >& flowFieldY,
                  gkg::Volume< float >& flowFieldZ,
                  gkg::Vector3d< int32_t >& sizeVector )
{

  try
  {
 
    inverseFlowFieldX.fill( 0.0f );
    inverseFlowFieldY.fill( 0.0f );
    inverseFlowFieldZ.fill( 0.0f );
 
    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;

    gkg::Volume< float > newFlowFieldX( sizeVector );
    gkg::Volume< float > newFlowFieldY( sizeVector );
    gkg::Volume< float > newFlowFieldZ( sizeVector );
    gkg::Volume< float > newInverseFlowFieldX( sizeVector );
    gkg::Volume< float > newInverseFlowFieldY( sizeVector );
    gkg::Volume< float > newInverseFlowFieldZ( sizeVector );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t it = 0;

    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          newInverseFlowFieldX( i, j, k ) = -flowFieldX( i, j, k );   
          newInverseFlowFieldY( i, j, k ) = -flowFieldY( i, j, k );   
          newInverseFlowFieldZ( i, j, k ) = -flowFieldZ( i, j, k );   

	}

      }

    }

    for( it = 0; it < 10; ++it )
    {
	
      interpolation3D( newFlowFieldX, 
                       newInverseFlowFieldX, 
                       inverseFlowFieldX, 
                       inverseFlowFieldY, 
                       inverseFlowFieldZ, 
                       sizeVector, sizeVector, true );
      interpolation3D( newFlowFieldY, 
                       newInverseFlowFieldY, 
                       inverseFlowFieldX, 
                       inverseFlowFieldY, 
                       inverseFlowFieldZ, 
                       sizeVector, sizeVector, true );
      interpolation3D( newFlowFieldZ, 
                       newInverseFlowFieldZ, 
                       inverseFlowFieldX, 
                       inverseFlowFieldY, 
                       inverseFlowFieldZ, 
                       sizeVector, sizeVector, true );

      for( k = 0; k < sizeZ; ++k )
      {
	
        for ( j = 0; j < sizeY; ++j )
        {

          for ( i = 0; i < sizeX; ++i )
          {
	
            inverseFlowFieldX( i, j, k ) = newFlowFieldX( i, j, k );
            inverseFlowFieldY( i, j, k ) = newFlowFieldY( i, j, k );
            inverseFlowFieldZ( i, j, k ) = newFlowFieldZ( i, j, k );

          }

        }

      }

    }

  }
  GKG_CATCH( "void fastInverse( gkg::Volume< float >& inverseFlowFieldX,"
             "gkg::Volume< float >& inverseFlowFieldY,"
             "gkg::Volume< float >& inverseFlowFieldZ,"
             "gkg::Volume< float >& flowFieldX,"
             "gkg::Volume< float >& flowFieldY,"
             "gkg::Volume< float >& flowFieldZ,"
             "gkg::Vector3d< int32_t >& sizeVector )" );

}



void combineDeformation( gkg::Volume< float >& flowFieldX3,
                         gkg::Volume< float >& flowFieldY3,  
                         gkg::Volume< float >& flowFieldZ3,  
                         gkg::Volume< float >& flowFieldX1,  
                         gkg::Volume< float >& flowFieldY1,  
                         gkg::Volume< float >& flowFieldZ1,  
                         gkg::Volume< float >& flowFieldX2,  
                         gkg::Volume< float >& flowFieldY2,  
                         gkg::Volume< float >& flowFieldZ2,  
                         gkg::Vector3d< int32_t > sizeVector )
{

  try
  {

    gkg::Volume< float > flowFieldX( sizeVector );
    gkg::Volume< float > flowFieldY( sizeVector );
    gkg::Volume< float > flowFieldZ( sizeVector );

    interpolation3D( flowFieldX, 
                     flowFieldX1, 
                     flowFieldX2, 
                     flowFieldY2,
                     flowFieldZ2, 
                     sizeVector, 
                     sizeVector, true );

    interpolation3D( flowFieldY, 
                     flowFieldY1, 
                     flowFieldX2, 
                     flowFieldY2,
                     flowFieldZ2, 
                     sizeVector, 
                     sizeVector, true );

    interpolation3D( flowFieldZ, 
                     flowFieldZ1, 
                     flowFieldX2, 
                     flowFieldY2,
                     flowFieldZ2, 
                     sizeVector, 
                     sizeVector, true );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    for( k = 0; k < sizeVector.z; ++k )
    {
	
      for ( j = 0; j < sizeVector.y; ++j )
      {

        for ( i = 0; i < sizeVector.x; ++i )
        {
	
          flowFieldX3( i, j, k ) = flowFieldX( i, j, k ) +
                                                        flowFieldX2( i, j, k );
          flowFieldY3( i, j, k ) = flowFieldY( i, j, k ) + 
                                                        flowFieldY2( i, j, k );
          flowFieldZ3( i, j, k ) = flowFieldZ( i, j, k ) + 
                                                        flowFieldZ2( i, j, k );

        }

      }

    }
  
  }
  GKG_CATCH( "combineDeformation( gkg::Volume< float >& flowFieldX3,"
             "gkg::Volume< float >& flowFieldY3," 
             "gkg::Volume< float >& flowFieldZ3," 
             "gkg::Volume< float >& flowFieldX1," 
             "gkg::Volume< float >& flowFieldY1," 
             "gkg::Volume< float >& flowFieldZ1," 
             "gkg::Volume< float >& flowFieldX2," 
             "gkg::Volume< float >& flowFieldY2," 
             "gkg::Volume< float >& flowFieldZ2," 
             "gkg::Vector3d< int32_t > sizeVector )" );

}

void diffeomorphic( gkg::Volume< float >& scaledFlowFieldX,
                    gkg::Volume< float >& scaledFlowFieldY,
                    gkg::Volume< float >& scaledFlowFieldZ,
                    gkg::Vector3d< int32_t >& sizeVector,
                    int32_t expSteps,
                    int32_t& factor )
{

  try
  {


    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

    gkg::Volume< float > flowFieldX1( sizeVector );
    gkg::Volume< float > flowFieldY1( sizeVector );
    gkg::Volume< float > flowFieldZ1( sizeVector );

    gkg::Volume< float > flowFieldX2( sizeVector );
    gkg::Volume< float > flowFieldY2( sizeVector );
    gkg::Volume< float > flowFieldZ2( sizeVector );
	
    float inverseFactor = 1.0f / ( float )factor;
    float coeff = 1.0f / ( float )std::pow( 2.0, expSteps );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    int32_t it = 0;

    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          flowFieldX1( i, j, k ) = 
                           coeff * scaledFlowFieldX( i, j, k ) * inverseFactor;
          flowFieldY1( i, j, k ) = 
                           coeff * scaledFlowFieldY( i, j, k ) * inverseFactor;
          flowFieldZ1( i, j, k ) = 
                           coeff * scaledFlowFieldZ( i, j, k ) * inverseFactor;

        }

      }

    }

	
    for( it = 0; it < expSteps; ++it )
    {
	
      combineDeformation( flowFieldX2,
                          flowFieldY2, 
                          flowFieldZ2,
                          flowFieldX1,
                          flowFieldY1,
                          flowFieldZ1,
                          flowFieldX1,
                          flowFieldY1,
                          flowFieldZ1,
                          sizeVector );


		
      for( k = 0; k < sizeZ; ++k )
      {
	
        for ( j = 0; j < sizeY; ++j )
        {

          for ( i = 0; i < sizeX; ++i )
          {
	
            flowFieldX1( i, j, k ) = flowFieldX2( i, j, k );
            flowFieldY1( i, j, k ) = flowFieldY2( i, j, k );
            flowFieldZ1( i, j, k ) = flowFieldZ2( i, j, k );
	

          }
	
        }

      }
	
    }
	
    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {
	
          scaledFlowFieldX( i, j, k ) = 
                                      flowFieldX2( i, j, k ) * ( float )factor;
          scaledFlowFieldY( i, j, k ) = 
                                      flowFieldY2( i, j, k ) * ( float )factor;
          scaledFlowFieldZ( i, j, k ) = 
                                      flowFieldZ2( i, j, k ) * ( float )factor;

        }

      }
	
    }

  }
  GKG_CATCH( "void diffeomorphic( gkg::Volume< float >& scaledFlowFieldX,"
             "gkg::Volume< float >& scaledFlowFieldY,"
             "gkg::Volume< float >& scaledFlowFieldZ,"
             "gkg::Vector3d< int32_t >& sizeVector,"
             "int32_t& expSteps,"
             "int32_t& factor )" );

}


void symmetricMapping( gkg::Volume< float >& directFlowFieldX,
                       gkg::Volume< float >& directFlowFieldY,
                       gkg::Volume< float >& directFlowFieldZ,
                       gkg::Volume< float >& inverseFlowFieldX,
                       gkg::Volume< float >& inverseFlowFieldY,
                       gkg::Volume< float >& inverseFlowFieldZ,
                       gkg::Vector3d< int32_t >& sizeVector,
                       int32_t& factor )
{

  try
  {

    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

    float inverseFactor = 1.0f / ( float )factor;

    gkg::Volume< float > symFlowFieldX1( sizeVector );
    gkg::Volume< float > symFlowFieldY1( sizeVector );
    gkg::Volume< float > symFlowFieldZ1( sizeVector );
    gkg::Volume< float > symFlowFieldX2( sizeVector );
    gkg::Volume< float > symFlowFieldY2( sizeVector );
    gkg::Volume< float > symFlowFieldZ2( sizeVector );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          symFlowFieldX1( i, j, k ) = directFlowFieldX( i, j, k ) * 
                                                     ( inverseFactor * 0.5f );
          symFlowFieldY1( i, j, k ) = directFlowFieldY( i, j, k ) * 
                                                     ( inverseFactor * 0.5f ); 
          symFlowFieldZ1( i, j, k ) = directFlowFieldZ( i, j, k ) * 
                                                     ( inverseFactor * 0.5f ); 
          symFlowFieldX2( i, j, k ) = inverseFlowFieldX( i, j, k ) * 
                                                     ( inverseFactor * 0.5f ); 
          symFlowFieldY2( i, j, k ) = inverseFlowFieldY( i, j, k ) * 
                                                     ( inverseFactor * 0.5f ); 
          symFlowFieldZ2( i, j, k ) = inverseFlowFieldZ( i, j, k ) * 
                                                     ( inverseFactor * 0.5f );  

	}

      }

    }

    gkg::Volume< float > inverseFlowFieldX1( sizeVector );
    gkg::Volume< float > inverseFlowFieldY1( sizeVector );
    gkg::Volume< float > inverseFlowFieldZ1( sizeVector );
    gkg::Volume< float > inverseFlowFieldX2( sizeVector );
    gkg::Volume< float > inverseFlowFieldY2( sizeVector );
    gkg::Volume< float > inverseFlowFieldZ2( sizeVector );

    fastInverse( inverseFlowFieldX1,
                 inverseFlowFieldY1,
                 inverseFlowFieldZ1,
                 symFlowFieldX1,
                 symFlowFieldY1,
                 symFlowFieldZ1, 
                 sizeVector );

    fastInverse( inverseFlowFieldX2,
                 inverseFlowFieldY2,
                 inverseFlowFieldZ2,
                 symFlowFieldX2,
                 symFlowFieldY2,
                 symFlowFieldZ2, 
                 sizeVector );
	
    combineDeformation( directFlowFieldX, 
                        directFlowFieldY, 
                        directFlowFieldZ, 
                        inverseFlowFieldX2, 
                        inverseFlowFieldY2, 
                        inverseFlowFieldZ2, 
                        symFlowFieldX1, 
                        symFlowFieldY1, 
                        symFlowFieldZ1, 
                        sizeVector );

    combineDeformation( inverseFlowFieldX, 
                        inverseFlowFieldY, 
                        inverseFlowFieldZ, 
                        inverseFlowFieldX1, 
                        inverseFlowFieldY1, 
                        inverseFlowFieldZ1, 
                        symFlowFieldX2, 
                        symFlowFieldY2, 
                        symFlowFieldZ2, 
                        sizeVector );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {
	
          directFlowFieldX( i, j, k ) *= ( float )factor;
          directFlowFieldY( i, j, k ) *= ( float )factor;
          directFlowFieldZ( i, j, k ) *= ( float )factor;
          inverseFlowFieldX( i, j, k ) *= ( float )factor;
          inverseFlowFieldY( i, j, k ) *= ( float )factor;	
          inverseFlowFieldZ( i, j, k ) *= ( float )factor;

        }

      }
	
    }

  }
  GKG_CATCH( "void symmetricMapping( gkg::Volume< float >& directFlowFieldX,"
             "gkg::Volume< float >& directFlowFieldY,"
             "gkg::Volume< float >& directFlowFieldZ,"
             "gkg::Volume< float >& inverseFlowFieldX,"
             "gkg::Volume< float >& inverseFlowFieldY,"
             "gkg::Volume< float >& inverseFlowFieldZ,"
             "gkg::Vector3d< int32_t >& sizeVector,"
             "int32_t& factor )" );	
	
}


float jacobian( gkg::Volume< float >& flowFieldX,
                gkg::Volume< float >& flowFieldY,
                gkg::Volume< float >& flowFieldZ,  
                gkg::Vector3d< int32_t > sizeVector,
                int32_t factor )
{

  try
  {
	
    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

    float inverseFactor = 1.0f / (float)factor;
    float jmean = 0.0f;
    float jstd = 0.0f;
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    float grad[ 3 ] = { -0.5f, 0.0f, 0.5f };

    gkg::Volume< float > Jac( sizeVector );
    Jac.fill( 0.0f );
    gkg::Volume< float > J11( sizeVector );
    J11.fill( 0.0f );
    gkg::Volume< float > J12( sizeVector );
    J12.fill( 0.0f );
    gkg::Volume< float > J13( sizeVector );
    J13.fill( 0.0f );
    gkg::Volume< float > J21( sizeVector );
    J21.fill( 0.0f );
    gkg::Volume< float > J22( sizeVector );
    J22.fill( 0.0f );
    gkg::Volume< float > J23( sizeVector );
    J23.fill( 0.0f );
    gkg::Volume< float > J31( sizeVector );
    J31.fill( 0.0f );
    gkg::Volume< float > J32( sizeVector );
    J32.fill( 0.0f );
    gkg::Volume< float > J33( sizeVector );
    J33.fill( 0.0f );
	
	
    float neg=0; 
    float Jmin=1; 
    float Jmax=1; 
    float J;
    float count=0; 
    float frac;
	
    filter1( flowFieldX, J11, sizeVector, grad, 3, 2 );
    filter1( flowFieldX, J12, sizeVector, grad, 3, 1 );
    filter1( flowFieldX, J13, sizeVector, grad, 3, 3 );

    filter1( flowFieldY, J21, sizeVector, grad, 3, 2 );
    filter1( flowFieldY, J22, sizeVector, grad, 3, 1 );
    filter1( flowFieldY, J23, sizeVector, grad, 3, 3 );

    filter1( flowFieldZ, J31, sizeVector, grad, 3, 2 );
    filter1( flowFieldZ, J32, sizeVector, grad, 3, 1 );
    filter1( flowFieldZ, J33, sizeVector, grad, 3, 3 );
	
    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {
		
          J11( i, j, k ) *= inverseFactor;
          J11( i, j, k ) += 1.0f;
          J12( i, j, k ) *= inverseFactor;
          J13( i, j, k ) *= inverseFactor;
          J21( i, j, k ) *= inverseFactor;
          J22( i, j, k ) *= inverseFactor;
          J22( i, j, k ) += 1.0f;
          J23( i, j, k ) *= inverseFactor;
          J31( i, j, k ) *= inverseFactor;
          J32( i, j, k ) *= inverseFactor;
          J33( i, j, k ) *= inverseFactor;
          J33( i, j, k ) += 1.0f;

	}	
	
      }

    }
 
    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          J = J11( i, j, k ) * ( J22( i, j, k ) * J33( i, j, k ) - 
                                 J23( i, j, k ) * J32( i, j, k ) ) -
              J21( i, j, k ) * ( J12( i, j, k ) * J33( i, j, k ) - 
                                 J13( i, j, k ) * J32( i, j, k ) ) +
              J31( i, j, k ) * ( J12( i, j, k ) * J23( i, j, k ) - 
                                 J13( i, j, k ) * J22( i, j, k ) );

          jmean+=J;

          if( J > Jmax )
          {
	
            Jmax = J;

          }
          if( J < Jmin )
          {
	
            Jmin = J;

          }
          if( J < 0 )
          {
	
            neg++;

          }

          count++;
          Jac( i, j, k ) = J;

	}	
	
      }

    }
	
    jmean /= size;

    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          jstd += std::pow( Jac( i, j, k ) - jmean, 2.0 );

	}	
	
      }

    }

    jstd /= ( size - 1 );
    jstd = std::sqrt( jstd );
    frac = neg / count;

    std::cout << "Jacobian of deformations| Mean (std): "
              << ( int32_t )( jmean * 1000 ) / 1000.0 
              << " (" << ( int32_t )( jstd * 1000 ) / 1000.0 << ")\n";
    std::cout << "Range: [" << Jmin << ", " << Jmax 
              << "] Negative fraction: "<< frac << "\n";


  }
  GKG_CATCH( "float jacobian( gkg::Volume< float >& flowFieldX,"
             "gkg::Volume< float >& flowFieldY,"
             "gkg::Volume< float >& flowFieldZ,"  
             "gkg::Vector3d< int32_t > sizeVector,"
             "int32_t factor )" );
		
}


float harmonicEnergy( gkg::Volume< float > flowFieldX,
                      gkg::Volume< float > flowFieldY,
                      gkg::Volume< float > flowFieldZ,
                      gkg::Vector3d< int32_t > sizeVector )
{

  try
  {

    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

    float grad[3]={-0.5,0.0,0.5};
    gkg::Volume< float > volumeOut( sizeVector );
    float energy = 0.0f;
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    filter1( flowFieldX, volumeOut, sizeVector, grad, 3, 1 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }
    filter1( flowFieldX, volumeOut, sizeVector, grad, 3, 2 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }
    filter1( flowFieldX, volumeOut, sizeVector, grad, 3, 3 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldY, volumeOut, sizeVector, grad, 3, 1 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldY, volumeOut, sizeVector, grad, 3, 2 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldY, volumeOut, sizeVector, grad, 3, 3 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldZ, volumeOut, sizeVector, grad, 3, 1 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldZ, volumeOut, sizeVector, grad, 3, 2 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }


    filter1( flowFieldZ, volumeOut, sizeVector, grad, 3, 3 );


    for( k = 0; k < sizeZ; ++k )
    {
	
      for ( j = 0; j < sizeY; ++j )
      {

        for ( i = 0; i < sizeX; ++i )
        {

          energy += volumeOut( i, j, k ) * volumeOut( i, j, k );

	}	
	
      }

    }

    energy /= ( float )( size );
    return energy;

  }
  GKG_CATCH( "float harmonicEnergy( gkg::Volume< float > flowFieldX,"
             "gkg::Volume< float > flowFieldY,"
             "gkg::Volume< float > flowFieldZ,"
             "gkg::Vector3d< int32_t > sizeVector )" );

}





