#include <gkg-core-exception/Exception.h>
#include <gkg-processing-container/Volume_i.h>

//#define RAND_SAMPLES = 65

void dataCost( gkg::PdfCartesianField& reference, 
               gkg::PdfCartesianField& floating,
               std::set< gkg::Vector3d< int32_t >, 
                           gkg::Vector3dCompare< int32_t > > hasSiteReference,
               std::set< gkg::Vector3d< int32_t >, 
                           gkg::Vector3dCompare< int32_t > > hasSiteFloating,
               gkg::Vector3d< int32_t >& sizeVector,
               std::vector< float >& totalCost,
               float& regularizationWeighting,
               int32_t& currentHalfWidthSearchSpace,
               int32_t& currentControlPointsSpacing,
               float& currentSearchSpaceQuantisation )
{

  try
  {

    bool subPixel=false;
    if ( currentSearchSpaceQuantisation == 0.5 )
    {
	
      subPixel = true;
	
    }
	
    float regularizationWeighting1 = ( float )currentControlPointsSpacing / 
                                     ( regularizationWeighting * 
                                     ( float )currentSearchSpaceQuantisation );
	
    //float randv = ( ( float )std::rand() / float( RAND_MAX ) );
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
    int32_t j1 = 0;

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

    int32_t currentHalfWidthSearchSpace2 = 0;
    if ( subPixel )
    {

      currentHalfWidthSearchSpace2 = currentHalfWidthSearchSpace;

    }
    else
    {
	
      currentHalfWidthSearchSpace2 = currentHalfWidthSearchSpace * 
                                     ( int32_t )currentSearchSpaceQuantisation;

    }


    gkg::PdfCartesianField interpolatedFloating( 
                                   floating.getSizeX(),
                                   floating.getSizeY(),
                                   floating.getSizeZ(),
                                   floating.getResolution(),
                                   floating.getSiteMap(),
                                   floating.getItems(),
                                   floating.getOffsetX(),
                                   floating.getOffsetY(),
                                   floating.getOffsetZ(),
                                   floating.getOutputMultipleShellSampling() );

    int32_t interpolatedSizeX = sizeX;
    int32_t interpolatedSizeY = sizeY;
    int32_t interpolatedSizeZ = sizeZ;
    //int32_t interpolatedSize = interpolatedSizeX * 
    //                           interpolatedSizeY * 
    //                           interpolatedSizeZ;

    gkg::Volume< float > interpolationCoordinatesX;	
    gkg::Volume< float > interpolationCoordinatesY;	
    gkg::Volume< float > interpolationCoordinatesZ;	

    // subPixel not implemented for PDF fields yet
    /*
    if ( subPixel )
    {
		
      //interpolation with subsampling factor of 2
      interpolatedSizeX *= 2;
      interpolatedSizeY *= 2;
      interpolatedSizeZ *= 2;
      interpolatedSize = interpolatedSizeX * 
                         interpolatedSizeY * 
                         interpolatedSizeZ;

      interpolationCoordinatesX.reallocate( interpolatedSizeX,
                                            interpolatedSizeY,
                                            interpolatedSizeZ );

      interpolationCoordinatesY.reallocate( interpolatedSizeX,
                                            interpolatedSizeY,
                                            interpolatedSizeZ );

      interpolationCoordinatesZ.reallocate( interpolatedSizeX,
                                            interpolatedSizeY,
                                            interpolatedSizeZ );

      interpolatedFloating.reallocate( interpolatedSizeX,
                                       interpolatedSizeY,
                                       interpolatedSizeZ );
      interpolatedFloating.fill( 0.0f );

      for ( k = 0; k < interpolatedSizeZ; ++k )
      {
			
        for ( j = 0; j < interpolatedSizeY; ++j )
        {
				
          for ( i = 0; i < interpolatedSizeX; ++i )
          {
					
            interpolationCoordinatesX( i, j, k ) = 0.5 * ( float )j;
            interpolationCoordinatesY( i, j, k ) = 0.5 * ( float )i;
            interpolationCoordinatesZ( i, j, k ) = 0.5 * ( float )k;
									
          }
	
        }

      }

      gkg::Vector3d< int32_t > interpolatedSizeVector( interpolatedSizeX,
                                                       interpolatedSizeY,
                                                       interpolatedSizeZ );

      gkg::Vector3d< int32_t > sizeVector( sizeX, sizeY, sizeZ );

      interpolation3D( interpolatedFloating, 
                       floating, 
                       interpolationCoordinatesX,
                       interpolationCoordinatesY,
                       interpolationCoordinatesZ,
                       interpolatedSizeVector,
                       sizeVector,
                       false );

      for ( i = 0; i < cubedCurrentSearchSpace; ++i )
      {
			
        xs[ i ] *= 2.0;		
        ys[ i ] *= 2.0;
        zs[ i ] *= 2.0;
		
      }
	
    }
    */
	
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
	
    std::vector< float > cost1( cubedCurrentSearchSpace, 0.0f );

    float regularizationWeighting2 = regularizationWeighting1 / 
                                                              (float)maxSample;
    int32_t xx2,yy2,zz2;

    for ( i = 0; i < currentSize; ++i )
    {
		
      if ( ( i % ( int32_t )( currentSize / 25 ) ) == 0 )
      {
			
        std::cout << "x" << std::flush;

      }
		
      int32_t z1 = i / ( currentSizeX * currentSizeY );
      int32_t x1 = ( i - z1 * currentSizeX * currentSizeY ) / currentSizeX;
      int32_t y1 = i - z1 * currentSizeX * currentSizeY - x1 * currentSizeX;
		
      z1 *= currentControlPointsSpacing;
      x1 *= currentControlPointsSpacing;
      y1 *= currentControlPointsSpacing;
		
      //check image boundaries to save min/max computations
      bool boundaries = true;
 
      if ( subPixel )
      {
			
        if ( ( x1 * 2 + ( currentControlPointsSpacing - 1 ) * 2 + 
              currentHalfWidthSearchSpace2 >= interpolatedSizeY ) || 
             ( y1 * 2 + ( currentControlPointsSpacing - 1 ) * 2 + 
              currentHalfWidthSearchSpace2 >= interpolatedSizeX ) || 
             ( z1 * 2 + ( currentControlPointsSpacing - 1 ) * 2 + 
              currentHalfWidthSearchSpace2 >= interpolatedSizeZ ) )
        {
	
          boundaries = false;

        }

        if ( x1 * 2 - currentHalfWidthSearchSpace2 < 0 || 
             y1 * 2 - currentHalfWidthSearchSpace2 < 0 || 
             z1 * 2 - currentHalfWidthSearchSpace2 < 0 )
        {
	
          boundaries = false;

        }

      }
      else
      {
			
        if ( ( x1 + ( currentControlPointsSpacing - 1 ) + 
              currentHalfWidthSearchSpace2 >= interpolatedSizeY ) || 
            ( y1 + ( currentControlPointsSpacing - 1 ) + 
              currentHalfWidthSearchSpace2 >= interpolatedSizeX ) || 
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
        int32_t xx = ( i1 - zz * currentControlPointsSpacing * 
                             currentControlPointsSpacing ) / 
                                                   currentControlPointsSpacing;
        int32_t yy = i1 - zz * currentControlPointsSpacing * 
                               currentControlPointsSpacing - 
                                              xx * currentControlPointsSpacing;
			
        xx += x1;
        yy += y1;
        zz += z1;

        for ( l = 0; l < cubedCurrentSearchSpace; ++l )
        {
	
          if ( !boundaries )
          {
					
            if ( subPixel ) 
            {
						
              xx2 = std::max( 
                      std::min( xx * 2 + ( int32_t )xs[ l ], 
                                ( interpolatedSizeY - 1 ) ), 0 );
              yy2 = std::max( 
                      std::min( yy * 2 + ( int32_t )ys[ l ], 
                                ( interpolatedSizeX - 1 ) ), 0 );
              zz2 = std::max( 
                      std::min( zz * 2 + ( int32_t )zs[ l ], 
                               ( interpolatedSizeZ - 1 ) ), 0 );
		
            }
            else
            {

              xx2 = std::max( 
                      std::min( xx + ( int32_t )xs[ l ], 
                                ( interpolatedSizeY - 1 ) ), 0 );
              yy2 = std::max( 
                      std::min( yy + ( int32_t )ys[ l ], 
                                ( interpolatedSizeX - 1 ) ), 0 );
              zz2 = std::max( 
                      std::min( zz + ( int32_t )zs[ l ], 
                               ( interpolatedSizeZ - 1 ) ), 0 );
		
            }
 
          }			
          else
          {
				
            if ( subPixel )
            {

              xx2 = xx * 2 + ( int32_t )xs[ l ];
              yy2 = yy * 2 + ( int32_t )ys[ l ];
              zz2 = zz * 2 + ( int32_t )zs[ l ];
					
            }
            else
            {

              xx2 = xx + ( int32_t )xs[ l ];
              yy2 = yy + ( int32_t )ys[ l ];
              zz2 = zz + ( int32_t )zs[ l ];
					
            }
				
          }

          gkg::Vector3d< int32_t > xyz( yy, xx, zz );
          gkg::Vector3d< int32_t > xyz2( yy2, xx2, zz2 );


          std::set< gkg::Vector3d< int32_t >, 
                            gkg::Vector3dCompare< int32_t > >::iterator h1,h2;

          h1 = hasSiteReference.find( xyz );
          h2 = hasSiteFloating.find( xyz2 );

          if ( h1 != hasSiteReference.end() &&
               h2 != hasSiteFloating.end() )
          {

            gkg::ProbabilityDensityFunction* referencePdf = 
                                                  reference.getItemFast( *h1 );

            gkg::ProbabilityDensityFunction* floatingPdf = 
                                       interpolatedFloating.getItemFast( *h2 );

            cost1[ l ] += referencePdf->getDissimilarity( *floatingPdf,
                                                       0.0f, 100.0f );

          }

        }
      
      }
		
      for ( l = 0; l < cubedCurrentSearchSpace; ++l )
      {
	
        totalCost[ i + l * currentSize ] = regularizationWeighting2 * 
                                                                    cost1[ l ];
	
      }
	
    }

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


