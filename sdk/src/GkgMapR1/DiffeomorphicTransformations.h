#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>

// Several functions to interpolate and symmetrise deformations 
// as well as make them diffeomorphic.
// Calculates Jacobian and harmonic Energy. 


void interpolation3D( gkg::Volume< float >& warpedVolume,
                      const gkg::Volume< float >& inputVolume,
                      gkg::Volume< float >& flowFieldX, 
                      gkg::Volume< float >& flowFieldY, 
                      gkg::Volume< float >& flowFieldZ, 
                      gkg::Vector3d< int32_t >& warpedVolumeSize,
                      gkg::Vector3d< int32_t >& inputVolumeSize,
                      bool addIntegerCoordinates )
{

  try
  {

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    int32_t warpedSizeX = warpedVolumeSize.x;
    int32_t warpedSizeY = warpedVolumeSize.y;
    int32_t warpedSizeZ = warpedVolumeSize.z;

    int32_t inputSizeX = inputVolumeSize.x;
    int32_t inputSizeY = inputVolumeSize.y;
    int32_t inputSizeZ = inputVolumeSize.z;

    float flowFieldValueX = 0.0f;
    float flowFieldValueY = 0.0f; 
    float flowFieldValueZ = 0.0f; 

    int32_t floorFlowFieldValueX = 0.0f; 
    int32_t floorFlowFieldValueY = 0.0f; 
    int32_t floorFlowFieldValueZ = 0.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    for( k = 0; k < warpedSizeZ; ++k )
    {
		
      for( j = 0; j < warpedSizeY; ++j )
      {
			
        for( i = 0; i < warpedSizeX; ++i )
        {
				
          flowFieldValueX = flowFieldX( i, j, k );
          flowFieldValueY = flowFieldY( i, j, k );
          flowFieldValueZ = flowFieldZ( i, j, k );

          floorFlowFieldValueX = std::floor( flowFieldValueX );
          floorFlowFieldValueY = std::floor( flowFieldValueY );
          floorFlowFieldValueZ = std::floor( flowFieldValueZ );
  
          dx = ( float )( flowFieldValueX - floorFlowFieldValueX );
          dy = ( float )( flowFieldValueY - floorFlowFieldValueY );
          dz = ( float )( flowFieldValueZ - floorFlowFieldValueZ );

				
          if( addIntegerCoordinates )
          {

            floorFlowFieldValueX += i;
            floorFlowFieldValueY += j;
            floorFlowFieldValueZ += k;

          }

          warpedVolume( i, j, k ) = 
           ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) + 
           ( 1.0f - dx ) * dy * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           dx * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           ( 1.0f - dx ) * ( 1.0f - dy ) * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                           inputSizeZ - 1 ) ) +
           dx * dy * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           ( 1.0f - dx ) * dy * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                           inputSizeZ - 1 ) ) +
           dx * ( 1.0f - dy ) * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                      inputSizeZ - 1 ) ) +
           dx * dy * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                            inputSizeZ - 1 ) );

        }

      }

    }
  
  }
  GKG_CATCH( "void interpolation3D("
             "gkg::RCPointer< gkg::Volume< float > >& warpedVolume,"
             "gkg::RCPointer< gkg::Volume< float > >& inputVolume,"
             "gkg::Volume< float >& flowField," 
             "gkg::Vector3d< int32_t >& warpedVolumeSize,"
             "gkg::Vector3d< int32_t >& inputVolumeSize,"
             "bool addIntegerCoordinates )" );
  
}



void upsampleDeformations( gkg::Volume< float >& currentDirectFlowFieldX,
                           gkg::Volume< float >& currentDirectFlowFieldY, 
                           gkg::Volume< float >& currentDirectFlowFieldZ, 
                           gkg::Volume< float >& scaledDirectFlowFieldX, 
                           gkg::Volume< float >& scaledDirectFlowFieldY, 
                           gkg::Volume< float >& scaledDirectFlowFieldZ, 
                           gkg::Vector3d< int32_t > currentSizeVector, 
                           gkg::Vector3d< int32_t > scaledSizeVector )
{

  try
  {
  
    int32_t currentSizeX = currentSizeVector.x;
    int32_t currentSizeY = currentSizeVector.y;
    int32_t currentSizeZ = currentSizeVector.z;

    int32_t scaledSizeX = scaledSizeVector.x;  
    int32_t scaledSizeY = scaledSizeVector.y;    
    int32_t scaledSizeZ = scaledSizeVector.z;      

    float ratioX = ( float )( currentSizeX / scaledSizeX );
    float ratioY = ( float )( currentSizeY / scaledSizeY );
    float ratioZ = ( float )( currentSizeZ / scaledSizeZ );

    gkg::Volume< float > ratioCoordinatesX( currentSizeVector );
    ratioCoordinatesX.fill( 0.0f );
    gkg::Volume< float > ratioCoordinatesY( currentSizeVector );
    ratioCoordinatesY.fill( 0.0f );
    gkg::Volume< float > ratioCoordinatesZ( currentSizeVector );
    ratioCoordinatesZ.fill( 0.0f );

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    for ( k = 0; k < currentSizeZ; ++k )
    {

      for ( j = 0; j < currentSizeY; ++j )
      {

        for ( i = 0; i < currentSizeX; ++i )
        {

          ratioCoordinatesX( i, j, k ) = i / ratioX;
          ratioCoordinatesY( i, j, k ) = j / ratioY;
          ratioCoordinatesZ( i, j, k ) = k / ratioZ;

        }
   
      }

    }
 
    interpolation3D( currentDirectFlowFieldX, 
                     scaledDirectFlowFieldX, 
                     ratioCoordinatesX,
                     ratioCoordinatesY,
                     ratioCoordinatesZ,
                     currentSizeVector,
                     scaledSizeVector, false );

    interpolation3D( currentDirectFlowFieldY, 
                     scaledDirectFlowFieldY, 
                     ratioCoordinatesX,
                     ratioCoordinatesY,
                     ratioCoordinatesZ,
                     currentSizeVector,
                     scaledSizeVector, false );

    interpolation3D( currentDirectFlowFieldZ, 
                     scaledDirectFlowFieldZ, 
                     ratioCoordinatesX,
                     ratioCoordinatesY,
                     ratioCoordinatesZ,
                     currentSizeVector,
                     scaledSizeVector, false );

  }
  GKG_CATCH( "void upsampleDeformations("
             "gkg::Volume< float >& currentDirectFlowField," 
             "gkg::Volume< float >&scaledDirectFlowField," 
             "gkg::Vector3d< int32_t > currentSizeVector," 
             "gkg::Vector3d< int32_t > scaledSizeVector )" );

}


void warpVolume( gkg::Volume< float >& warpedVolume,
                        gkg::Volume< float >& firstVolume,
                        gkg::Volume< float >& secondVolume,
                        gkg::Vector3d< int32_t >& volumeSize,
                        gkg::Volume< float >& flowFieldX,
                        gkg::Volume< float >& flowFieldY,
                        gkg::Volume< float >& flowFieldZ,
                        float& similarityEnergyBefore,
                        float& similarityEnergyAfter )
{

  try
  {

    int32_t sizeX = volumeSize.x;
    int32_t sizeY = volumeSize.y;
    int32_t sizeZ = volumeSize.z;
    int32_t size = sizeX * sizeY * sizeZ;	

    interpolation3D( warpedVolume, 
                     firstVolume, 
                     flowFieldX,
                     flowFieldY, 
                     flowFieldZ,  
                     volumeSize, 
                     volumeSize, true );
	
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    for( k = 0; k < sizeZ; ++k )
    {
		
      for( j = 0; j < sizeY; ++j )
      {
			
        for( i = 0; i < sizeX; ++i )
        {

          similarityEnergyBefore += std::pow( secondVolume( i, j, k ) - 
                                              firstVolume( i, j, k ), 2 );	
          similarityEnergyAfter += std::pow( secondVolume( i, j, k ) - 
                                             warpedVolume( i, j, k ), 2 );			
        }
		
      }
	
    }
	
    similarityEnergyBefore /= size;
    similarityEnergyAfter /= size;

  }
  GKG_CATCH( "static void warpVolume( "
             "gkg::RCPointer< gkg::Volume< float > >& warped,"
             "gkg::RCPointer< gkg::Volume< float > >& reference,"
             "gkg::RCPointer< gkg::Volume< float > >& floating,"
             "gkg::Volume< float >& scaledDirectFlowField )" );
	
}



















