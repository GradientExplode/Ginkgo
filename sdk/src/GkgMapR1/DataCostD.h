#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>

//#define RAND_SAMPLES = 65

static int32_t computingDisplacementCostsIndex = 6;
void dataCost( const gkg::Volume< float >& reference, 
               const gkg::Volume< float >& floating,
               gkg::Vector3d< int32_t >& sizeVector,
               std::vector< float >& totalCost,
               float& regularizationWeighting,
               int32_t& currentHalfWidthSearchSpace,
               int32_t& currentControlPointsSpacing,
               float& currentSearchSpaceQuantisation )
{

  try
  {

	
    float regularizationWeighting1 = ( float )currentControlPointsSpacing / 
                                     ( regularizationWeighting * 
                                     ( float )currentSearchSpaceQuantisation );
	
    float randv = ( ( float )std::rand() / float( RAND_MAX ) );
    int32_t sizeX = sizeVector.x;
    int32_t sizeY = sizeVector.y;
    int32_t sizeZ = sizeVector.z;
    int32_t size = sizeX * sizeY * sizeZ;

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
    int32_t i1 = 0;
    int32_t j1 = 0;

    for ( i = 0; i < currentSearchSpace; ++i )
    {
		
      for ( j = 0; j < currentSearchSpace; ++j )
      {
			
        for ( k = 0; k < currentSearchSpace; ++k )
        {
					
          xs[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( i - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

          ys[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( j - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

          zs[ i + j*currentSearchSpace + 
              k*currentSearchSpace*currentSearchSpace ] = 
          ( float )( ( k - currentHalfWidthSearchSpace ) * 
                                               currentSearchSpaceQuantisation );

        }
		
      }
	
    }


    std::ofstream os( std::string( "Kevin2/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_1.txt" );

    for ( int32_t d = 0; d < cubedCurrentSearchSpace; d++ )
    {

      os << xs[ d ] << " "
         << ys[ d ] << " "
         << zs[ d ] << std::endl;
         
    }

    os.close();




    int32_t currentHalfWidthSearchSpace2 = currentHalfWidthSearchSpace * 
                                     ( int32_t )currentSearchSpaceQuantisation;
	
    gkg::Volume< float > interpolatedFloating( floating );

    int32_t interpolatedSizeX = sizeX;
    int32_t interpolatedSizeY = sizeY;
    int32_t interpolatedSizeZ = sizeZ;
    int32_t interpolatedSize = interpolatedSizeX * 
                               interpolatedSizeY * 
                               interpolatedSizeZ;

    gkg::Volume< float > interpolationCoordinatesX;	
    gkg::Volume< float > interpolationCoordinatesY;	
    gkg::Volume< float > interpolationCoordinatesZ;	

	
    int32_t samples = 64;
    bool randomMode = samples < std::pow( currentControlPointsSpacing, 3 );
    //bool randomMode = true;
    int32_t maxSample;
    if ( randomMode )
    {
	
	maxSample = samples;

    }
    else
    {

      maxSample = std::pow( currentControlPointsSpacing, 3 );

    }
	


    os.open( std::string( "Kevin2/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_2.txt" );
    os << "currentHalfWidthSearchSpaceBis=" << currentHalfWidthSearchSpace2 << std::endl;
    os << "useRandomMode=" << randomMode << std::endl;
    os << "maximumSampleCount=" << maxSample << std::endl;

    std::vector< float > cost1( cubedCurrentSearchSpace, 0.0f );

    float regularizationWeighting2 = regularizationWeighting1 / 
                                                              (float)maxSample;

    os << "totalRegularizationWeight=" << regularizationWeighting2 << std::endl;
    os.close();

    int32_t xx2,yy2,zz2;


    std::cout << std::endl;

    for ( i = 0; i < currentSize; ++i )
    {
		
      if ( ( i % ( int32_t )( currentSize / 25 ) ) == 0 )
      {
			
        std::cout << "x" << std::flush;

      }
		
      int32_t z1 = i / ( currentSizeX * currentSizeY );
      int32_t y1 = ( i - z1 * currentSizeX * currentSizeY ) / currentSizeX;
      int32_t x1 = i - z1 * currentSizeX * currentSizeY - y1 * currentSizeX;
		
      z1 *= currentControlPointsSpacing;
      y1 *= currentControlPointsSpacing;
      x1 *= currentControlPointsSpacing;


      //check image boundaries to save min/max computations
      bool boundaries = true;
 
      
                      
      if ( ( x1 + ( currentControlPointsSpacing - 1 ) + 
            currentHalfWidthSearchSpace2 >= interpolatedSizeX ) || 
          ( y1 + ( currentControlPointsSpacing - 1 ) + 
            currentHalfWidthSearchSpace2 >= interpolatedSizeY ) || 
          ( z1 + ( currentControlPointsSpacing - 1 ) + 
            currentHalfWidthSearchSpace2 >= interpolatedSizeZ ) )
      {

        boundaries = false;

      }

      if ( x1 - currentHalfWidthSearchSpace2 < 0 || 
           y1 - currentHalfWidthSearchSpace2 < 0 || 
           z1 - currentHalfWidthSearchSpace2 < 0 )
      {

        boundaries = false;

      }
              
      
      int32_t c = 0;
      for (  c = 0; c < cubedCurrentSearchSpace; ++c )
      {

        cost1[ c ] = 0.0f;

      }

      for( j1 = 0; j1 < maxSample; ++j1 )
      {
	
        int32_t i1 = 0;
	
        //stochastic sampling for speed-up (~8x faster)		
        if ( randomMode )
        {

          i1 = ( int32_t )( std::rand() * 
                            std::pow( currentControlPointsSpacing, 3 ) / 
                                                           float( RAND_MAX ) );
 	
        }
        else
        {

          i1 = j1;      

        }
			
        int32_t zz = i1 / ( currentControlPointsSpacing * 
                        currentControlPointsSpacing );
        int32_t yy = ( i1 - zz * currentControlPointsSpacing * 
                             currentControlPointsSpacing ) / 
                                                   currentControlPointsSpacing;
        int32_t xx = i1 - zz * currentControlPointsSpacing * 
                               currentControlPointsSpacing - 
                                              yy * currentControlPointsSpacing;
			
        xx += x1;
        yy += y1;
        zz += z1;

        for ( l = 0; l < cubedCurrentSearchSpace; ++l )
        {
	
          if ( !boundaries )
          {
					
            

            xx2 = std::max( 
                    std::min( xx + ( int32_t )xs[ l ], 
                              ( interpolatedSizeX - 1 ) ), 0 );
            yy2 = std::max( 
                    std::min( yy + ( int32_t )ys[ l ], 
                              ( interpolatedSizeY - 1 ) ), 0 );
            zz2 = std::max( 
                    std::min( zz + ( int32_t )zs[ l ], 
                             ( interpolatedSizeZ - 1 ) ), 0 );
	
            
 
          }			
          else
          {
				
            

            xx2 = xx + ( int32_t )xs[ l ];
            yy2 = yy + ( int32_t )ys[ l ];
            zz2 = zz + ( int32_t )zs[ l ];
	                              
            
				
          }

          cost1[ l ] += std::abs( ( float )( reference( xx, yy, zz) - 
                                interpolatedFloating( xx2, yy2, zz2 ) ) );

        }
      
      }
		
      for ( l = 0; l < cubedCurrentSearchSpace; ++l )
      {
	
        totalCost[ i + l * currentSize ] = regularizationWeighting2 * 
                                                                    cost1[ l ];	
      }
	
    }



    gkg::Volume< float >
      displacementCostsVolume( currentSizeX,
                               currentSizeY,
                               currentSizeZ,
                               cubedCurrentSearchSpace );
    gkg::Volume< float >::iterator dout = displacementCostsVolume.begin();
    std::vector< float >::const_iterator din = totalCost.begin(),
                                         dine = totalCost.end();
    while ( din != dine )
    {

      *dout = *din;
      ++ dout;
      ++ din;

    }
    gkg::Writer::getInstance().write(

                      std::string( "Kevin2/" ) +
                      gkg::StringConverter::toString(
                                             computingDisplacementCostsIndex ) +
                      "_3.ima",
                      displacementCostsVolume );


++computingDisplacementCostsIndex;

  }
  GKG_CATCH( "void dataCost( gkg::Volume< float >& reference," 
             "gkg::Volume< float >& floating,"
             "gkg::Vector3d< int32_t >& sizeVector,"
             "std::vector< float >& totalCost,"
             "float& regularizationWeighting,"
             "int32_t& currentHalfWidthSearchSpace,"
             "int32_t& currentControlPointsSpacing,"
             "float& currentSearchSpaceQuantisation )" );

}


