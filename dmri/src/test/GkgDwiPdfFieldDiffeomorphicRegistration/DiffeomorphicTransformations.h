#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>


// Several functions to interpolate and symmetrise deformations 
// as well as make them diffeomorphic.
// Calculates Jacobian and harmonic Energy. 


void interpolation3DField( 
                   gkg::PdfCartesianField& warpedField,
                   gkg::PdfCartesianField& inputField,
                   std::list< gkg::Vector3d< int32_t > >& inputFieldSites,
                   gkg::Volume< float >& flowFieldX, 
                   gkg::Volume< float >& flowFieldY, 
                   gkg::Volume< float >& flowFieldZ, 
                   gkg::Vector3d< int32_t >& /*warpedFieldSize*/,
                   gkg::Vector3d< int32_t >& inputFieldSize,
                   bool addIntegerCoordinates,
                   int32_t index )
{

  try
  {

    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    int32_t inputSizeX = inputFieldSize.x;
    int32_t inputSizeY = inputFieldSize.y;
    int32_t inputSizeZ = inputFieldSize.z;

    float flowFieldValueX = 0.0f;
    float flowFieldValueY = 0.0f; 
    float flowFieldValueZ = 0.0f; 

    int32_t floorFlowFieldValueX = 0.0f; 
    int32_t floorFlowFieldValueY = 0.0f; 
    int32_t floorFlowFieldValueZ = 0.0f;

    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    // getting the number of values of PDFs
    int32_t valueCount = inputField.begin()->second.getValueCount();

    std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = inputFieldSites.begin(),
      se = inputFieldSites.end();  

    int32_t c = 0;

    while ( s != se )
    {

      c ++;
      if ( c % 100 == 0 )
        std::cout << "Site number : " << c << std::endl;

      const gkg::Vector3d< int32_t >& site = *s;
				
      flowFieldValueX = flowFieldX( i, j, k );
      flowFieldValueY = flowFieldY( i, j, k );
      flowFieldValueZ = flowFieldZ( i, j, k );

      floorFlowFieldValueX = std::floor( flowFieldValueX );
      floorFlowFieldValueY = std::floor( flowFieldValueY );
      floorFlowFieldValueZ = std::floor( flowFieldValueZ );
  
      dx = flowFieldValueX - floorFlowFieldValueX;
      dy = flowFieldValueY - floorFlowFieldValueY;
      dz = flowFieldValueZ - floorFlowFieldValueZ;
				
      if( addIntegerCoordinates )
      {

        floorFlowFieldValueX += j;
        floorFlowFieldValueY += i;
        floorFlowFieldValueZ += k;

      }

      if ( warpedField.getSiteMap().hasSite( 0, site ) )
      {

        gkg::ProbabilityDensityFunction inputPdf1;
        gkg::ProbabilityDensityFunction inputPdf2;
        gkg::ProbabilityDensityFunction inputPdf3;
        gkg::ProbabilityDensityFunction inputPdf4;
        gkg::ProbabilityDensityFunction inputPdf5;
        gkg::ProbabilityDensityFunction inputPdf6;
        gkg::ProbabilityDensityFunction inputPdf7;
        gkg::ProbabilityDensityFunction inputPdf8;

        gkg::ProbabilityDensityFunction& warpedPdf = 
                                          *( warpedField.getItemFast( site ) );

        gkg::Vector3d< int32_t > interpolationCoordinates( 0, 0, 0 );

        bool isInputPdf1 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf1 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf1 = true;

        }


        bool isInputPdf2 = false;


        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                             inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf2 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf2 = true;

        }


        bool isInputPdf3 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf3 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf3 = true;

        }


        bool isInputPdf4 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf4 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf4 = true;

        }


        bool isInputPdf5 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf5 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf5 = true;

        }


        bool isInputPdf6 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                              inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );
   
        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf6 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf6 = true;

        }


        bool isInputPdf7 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf7 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf7 = true;

        }


        bool isInputPdf8 = false;

        interpolationCoordinates.x = 
               std::min( std::max( floorFlowFieldValueY + 1, 0 ), 
                                                              inputSizeX - 1 );
        interpolationCoordinates.y =
               std::min( std::max( floorFlowFieldValueX + 1, 0 ), 
                                                              inputSizeY - 1 );
        interpolationCoordinates.z = 
               std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                              inputSizeZ - 1 );

        if ( inputField.getSiteMap().hasSite( 0, interpolationCoordinates ) )
        {

          inputPdf8 = *( inputField.getItemFast( interpolationCoordinates ) );

          isInputPdf8 = true;

        }

        int32_t l = 0;

        for ( l = index; l < valueCount; ++l )
        {

          float value = 0.0f;
          if ( isInputPdf1 )
          {

            value += ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) *
                       inputPdf1.getValue( l ); 

          }
          if ( isInputPdf2 )
          {

            value += ( 1.0f - dx ) * dy * ( 1.0f - dz ) *
                       inputPdf2.getValue( l ); 

          }
          if ( isInputPdf3 )
          {

            value += dx * ( 1.0f - dy ) * ( 1.0f - dz ) *
                       inputPdf3.getValue( l ); 

          }
          if ( isInputPdf4 )
          {

            value += ( 1.0f - dx ) * ( 1.0f - dy ) * dz *
                       inputPdf4.getValue( l ); 

          }
          if ( isInputPdf5 )
          {

            value += dx * dy * ( 1.0f - dz ) *
                       inputPdf5.getValue( l ); 

          }
          if ( isInputPdf6 )
          {

            value += ( 1.0f - dx ) * dy * dz *
                       inputPdf6.getValue( l ); 

          }
          if ( isInputPdf7 )
          {

            value += dx * ( 1.0f - dy ) * dz *
                       inputPdf7.getValue( l ); 

          }
          if ( isInputPdf8 )
          {

            value += dx * dy * dz *
                       inputPdf8.getValue( l ); 

          }


          warpedPdf.setValue( l, value );

        }

      }   

      ++ s;

    }
  
  }
  GKG_CATCH( "void interpolation3D( gkg::PdfCartesianField& warpedField,"
             "const gkg::PdfCartesianField& inputField,"
             "gkg::Volume< float >& flowFieldX,"
             "gkg::Volume< float >& flowFieldY," 
             "gkg::Volume< float >& flowFieldZ,"
             "gkg::Vector3d< int32_t >& warpedFieldSize,"
             "gkg::Vector3d< int32_t >& inputFieldSize,"
             "bool addIntegerCoordinates )" );
  
}



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
  
          dx = flowFieldValueX - floorFlowFieldValueX;
          dy = flowFieldValueY - floorFlowFieldValueY;
          dz = flowFieldValueZ - floorFlowFieldValueZ;
				
          if( addIntegerCoordinates )
          {

            floorFlowFieldValueX += j;
            floorFlowFieldValueY += i;
            floorFlowFieldValueZ += k;

          }

          warpedVolume( i, j, k ) = 
           ( 1.0f - dx ) * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) + 
           ( 1.0f - dx ) * dy * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           dx * ( 1.0f - dy ) * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           ( 1.0f - dx ) * ( 1.0f - dy ) * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                           inputSizeZ - 1 ) ) +
           dx * dy * ( 1.0f - dz ) * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ, 0 ), inputSizeZ - 1 ) ) +
           ( 1.0f - dx ) * dy * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                                                           inputSizeZ - 1 ) ) +
           dx * ( 1.0f - dy ) * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeY - 1 ),
            std::min( std::max( floorFlowFieldValueZ + 1, 0 ), 
                      inputSizeZ - 1 ) ) +
           dx * dy * dz * 
            inputVolume( 
            std::min( std::max( floorFlowFieldValueY + 1, 0 ), inputSizeX - 1 ),
            std::min( std::max( floorFlowFieldValueX + 1, 0 ), inputSizeY - 1 ),
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

          ratioCoordinatesX( i, j, k ) = j / ratioY;
          ratioCoordinatesY( i, j, k ) = i / ratioX;
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


void warpField( gkg::PdfCartesianField& warpedField,
                gkg::PdfCartesianField& firstField,
                std::list< gkg::Vector3d< int32_t > >& firstFieldSites,
                gkg::PdfCartesianField& /*secondField*/,
                gkg::Vector3d< int32_t >& fieldSize,
                gkg::Volume< float >& flowFieldX,
                gkg::Volume< float >& flowFieldY,
                gkg::Volume< float >& flowFieldZ,
                int32_t& index )
{

  try
  {	

    interpolation3DField( warpedField, 
                          firstField,
                          firstFieldSites, 
                          flowFieldX,
                          flowFieldY, 
                          flowFieldZ,  
                          fieldSize, 
                          fieldSize,
                          index, true );
	
  }
  GKG_CATCH( "void warpField( gkg::PdfCartesianfField& warpedField,"
             "gkg::PdfCartesianfField& firstField,"
             "gkg::PdfCartesianfField& secondField,"
             "gkg::Vector3d< int32_t >& fieldSize,"
             "gkg::Volume< float >& flowFieldX,"
             "gkg::Volume< float >& flowFieldY,"
             "gkg::Volume< float >& flowFieldZ,"
             "float& similarityEnergyBefore,"
             "float& similarityEnergyAfter )" );
	
}



















