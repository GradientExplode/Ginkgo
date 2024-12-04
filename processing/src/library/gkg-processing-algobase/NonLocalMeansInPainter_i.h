#ifndef _gkg_processing_algobase_NonLocalMeansInPainter_i_h_
#define _gkg_processing_algobase_NonLocalMeansInPainter_i_h_


#include <gkg-processing-algobase/NonLocalMeansInPainter.h>
#include <gkg-processing-container/Volume_i.h>

#include <gkg-processing-algobase/NonLocalMeansContextMmap_i.h>
#include <gkg-processing-algobase/NonLocalMeansContext_i.h>
#include <gkg-processing-algobase/NLMContributionContext_i.h>
#include <gkg-communication-thread/ThreadedLoop.h>

#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <cstring>
#include <iomanip>
#include <vector>


#define LUT_MAXIMUM_EXPONENTIAL 30000U

template < class IN, class OUT >
inline
gkg::NonLocalMeansInPainter< IN, OUT >::NonLocalMeansInPainter( 
                         const gkg::RCPointer< gkg::NLMNoiseModel >& noiseModel,
                         int32_t halfSearchBlockSize, 
                         int32_t halfNeighborhoodSize, 
                         float degreeOfFiltering,
                         bool verbose )
                                       : _noiseModel( noiseModel ),
                                         _halfSearchBlockSize(
                                                          halfSearchBlockSize ),
                                         _halfNeighborhoodSize( 
                                                         halfNeighborhoodSize ),
                                         _degreeOfFiltering( 
                                                            degreeOfFiltering ),
                                         _verbose( verbose )
{
}


template < class IN, class OUT >
inline
gkg::NonLocalMeansInPainter< IN, OUT >::~NonLocalMeansInPainter()
{
}


template < class IN, class OUT >
inline
void gkg::NonLocalMeansInPainter< IN, OUT >::inPaint( 
                             const gkg::Volume< IN >& inputVolume,
                             const gkg::Volume< int16_t >& maskVolume,
                             const gkg::Volume< int16_t >& inPaintingMaskVolume,
                             gkg::Volume< OUT >& outputVolume ) const
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the input sizes and resolutions
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > inputSizes;
    int32_t inputSizeT = 1;
    inputVolume.getSize( inputSizes, inputSizeT );

    gkg::Vector3d< double > inputResolutions;
    double inputResolutionT = 1.0;
    inputVolume.getResolution( inputResolutions, inputResolutionT );


    ////////////////////////////////////////////////////////////////////////////
    // performing sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( !maskVolume.hasSameSize( inputVolume, true ) )
    {

      throw std::runtime_error( 
                           "mask has different x,y,z sizes from input volume" );

    }

    if ( !maskVolume.hasSameResolution( inputVolume, true ) )
    {

      throw std::runtime_error( 
                     "mask has different x,y,z resolutions from input volume" );

    }

    if ( !inPaintingMaskVolume.hasSameSize( inputVolume, true ) )
    {

      throw std::runtime_error( 
               "in-painting mask has different x,y,z sizes from input volume" );

    }

    if ( !inPaintingMaskVolume.hasSameResolution( inputVolume, true ) )
    {

      throw std::runtime_error( 
         "in-painting mask has different x,y,z resolutions from input volume" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating the output volume
    ////////////////////////////////////////////////////////////////////////////

    outputVolume.reallocate( inputSizes, inputSizeT );
    out.setResolution( inputResolutions, inputResolutionT );
    out.fill( ( OUT )0 );


    ////////////////////////////////////////////////////////////////////////////
    // computing the vector of offsets corresponding to the search window
    ////////////////////////////////////////////////////////////////////////////

    int32_t halfSizeAlongX = std::min( _halfSearchBlockSize,
                                       ( inputSizes.x - 1 ) / 2 );
    int32_t halfSizeAlongY = std::min( _halfSearchBlockSize,
                                       ( inputSizes.y - 1 ) / 2 );
    int32_t halfSizeAlongZ = std::min( _halfSearchBlockSize,
                                       ( inputSizes.z - 1 ) / 2 );
    int32_t blockWidth = -( 2 * halfSizeAlongX + 1 ) *
                          ( halfSizeAlongZ * ( 2 * halfSizeAlongY + 1 ) + 
                            halfSizeAlongY + halfSizeAlongX );

    std::vector< gkg::Vector3d< int32_t > > blockOffsets;
    blockOffsets.reserve( ( uint64_t )( 2 * halfSizeAlongZ * 1 ) *
                          ( uint64_t )( 2 * halfSizeAlongY * 1 ) *
                          ( uint64_t )( 2 * halfSizeAlongX * 1 ) );

    int32_t ox = 0;
    int32_t oy = 0;
    int32_t oz = 0;
    for ( oz = -halfSizeAlongZ; oz <= halfSizeAlongZ; oz++ )
    {

      for ( oy = -halfSizeAlongY; oy <= halfSizeAlongY; oy++ )
      {

        for ( ox = -halfSizeAlongX; ox <= halfSizeAlongX; ox++, blockWidth++ )
        {

          if ( blockWidth < 0 )
          {

            blockOffsets.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

          }

        }

      }

    }

    uint64_t blockCount = ( uint64_t )blockOffsets.size();


    ////////////////////////////////////////////////////////////////////////////
    // computing a LUT of exponential values
    ////////////////////////////////////////////////////////////////////////////


    std::vector< float > exponentialLut( LUT_MAXIMUM_EXPONENTIAL );

    uint32_t x = 0U;
    for ( x = 0U; x < LUT_MAXIMUM_EXPONENTIAL; x++ )
    {

      exponentialLut[ x ] = ( float )std::exp( -( double )i / 1000.0 );

    }
    float noiseStdDev = _noiseModel->getSigma();
    float neighborhoodWidth = ( float )( 2 * _halfNeighborhoodSize + 1 );
    float h = 2.0 * _degreeOfFiltering * noiseStdDev * noiseStdDev * 
              neighborhoodWidth * neighborhoodWidth * neighborhoodWidth;


    ////////////////////////////////////////////////////////////////////////////
    // performing in-painting in case input/output volumes are memory mapped
    ////////////////////////////////////////////////////////////////////////////
    if ( inputVolume.isMemoryMapped() || outputVolume.isMemoryMapped() )
    {

      gkg::Volume< float > outputSubVolume( inputSizes );
      gkg::Volume< float > M( inputSizes );
      gkg::Volume< float > Z( inputSizes );

      int32_t t = 0;
      for ( t = 0; t < inputSizeT; t++ )
      {

        if ( _verbose )
        {

          if ( t )
          {

            std::cout << gkg::Eraser( 7 );

          }
          std::cout << "t = " << std::setw( 3 ) << t << std::flush;

        }

        // resetting temporary volumes
        outputSubVolume.fill( 0.0f );
        M.fill( 0.0f );
        Z.fill( 0.0f );

        gkg::Volume< float > u( inputSizes );
        gkg::Volume< float > v( inputSizes );
        gkg::Volume< float > vtmp( inputSizes );

        uint64_t blockIndex = 0U;
        for ( blockIndex = 0U; blockIndex < blockCount; blockIndex++ )
        {

          const gkg::Vector3d< int32_t >& 
            blockOffset = blockOffsets[ blockIndex ]; 

          u.fill( 0.0f );
          v.fill( 0.0f );
          vtmp.fill( 0.0f );

          int32_t xMin = std::max( -blockOffset.x, 0 );
          int32_t yMin = std::max( -blockOffset.y, 0 );
          int32_t zMin = std::max( -blockOffset.z, 0 );

          int32_t xMax = std::min( inputSizes.x - blockOffset.x, 
                                   inputSizes.x );
          int32_t yMax = std::min( inputSizes.y - blockOffset.y,
                                   inputSizes.y );                 
          int32_t zMax = std::min( inputSizes.z - blockOffset.z,
                                   inputSizes.z );                   

          int32_t x = 0;
          int32_t y = 0;
          int32_t z = 0;                                   

          // computing volume of pixel values squared differences
          for ( z = zMin; z < zMax; z++ )                
          {

            for ( y = yMin; y < yMax; y++ )
            {

              for ( x = xMin; x < xMax; x++ )
              {

                float diff = inputVolume( x + blockOffset.x,
                                          x + blockOffset.y,
                                          x + blockOffset.z, t ) - 
                             inputVolume( x, y, k, t );
                u( x, y, z ) = diff * diff;

              }

            }

          }

          // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
          // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, 
          //                   with h_i = 1[-P,...,P]
          // h_i ~= moving average
          // recursive implementation: 
          //   v_n( z ) = v_n( z - 1 ) - u_n( z - 1 - P ) + u_n( z + P )
          // --> Along X
          int32_t xMinus1 = 0;
          int32_t xOld = 0;
          int32_t xNew = 0;
          int32_t xMaxMinusHalfNeighborhoodSize = xMax - _halfNeighborhoodSize;
          int32_t xMinPlusHalfNeighborhoodSize = xMin + _halfNeighborhoodSize;
          for ( z = zMin; z < zMax; z++ )
          {

            for ( y = yMin; y < yMax; y++ )
            {

              for ( x = xMin; x <= xMinPlusHalfNeighborhoodSize ; x++ )
              {

                v( xMin,  y, z ) += u( x, y, z );

              }

              xMinus1 = xMin;
              xNew = xMinPlusHalfNeighborhoodSize + 1;
              for ( x = xMin + 1; x <= xMinPlusHalfNeighborhoodSize; x++ )
              {

                v( x, y, z ) = v( xMinus1, ky, kz ) + u( xNew, ky, kz );
                ++ xMinus1;
                ++ xNew;
                

              }

              xOld = xMinus1 - _halfNeighborhoodSize;
              for ( x = xMinPlusHalfNeighborhoodSize + 1;
                    x < xMaxMinusHalfNeighborhoodSize; x++ )
              {

                v( x, y, z ) = v( xMinus1,  y, z ) - u( xOld,  y, z ) +
                               u( xNew,  y, z );
                ++ xMinus1;
                ++ xOld;
                ++ xNew;

              }

              for ( x = dmax; x < xMax; x++ )
              {

                v( x, y, z ) = v( xMinus1,  y, z ) - u( xOld,  y, z );
                ++ xMinus1;
                ++ xOld;


              }

            }

          }

          // --> Along Y
          int32_t yMinus1 = 0;
          int32_t yOld = 0;
          int32_t yNew = 0;
          int32_t yMaxMinusHalfNeighborhoodSize = yMax - _halfNeighborhoodSize;
          int32_t yMinPlusHalfNeighborhoodSize = yMin + _halfNeighborhoodSize;
          for ( z = zMin; z < zMax; z++ )
          {

      	    for ( y = yMin; y <= yMinPlusHalfNeighborhoodSize; y++ )
            {

      	      for ( x = xMin; x < xMax; x++ )
      	      {

                vtmp( x,yMin, z ) += v( x, y, z );

              }

            }

      	    yMinus1 = yMin;
            yNew = yMinPlusHalfNeighborhoodSize + 1;
      	    for ( y = yMin + 1; y <= yMinPlusHalfNeighborhoodSize; y++ )
      	    {

              for ( x = xMin; x < xMax; x++ )
              {

                vtmp( x, y, z ) = vtmp( x,yMinus1, z ) + v( x,yNew, z );

              }
              ++ yMinus1;
              ++ yNew;

            }

            yOld = yMinus1 - _halfNeighborhoodSize;
      	    for ( y = yMinPlusHalfNeighborhoodSize + 1;
                  y < yMaxMinusHalfNeighborhoodSize; y++ )
            {

              for ( x = xMin; x < xMax; x++ )
      	      {

      	        vtmp( x, y, z ) = vtmp( x,yMinus1, z ) - v( x,yOld, z ) +
                                  v( x,yNew, z );

              }
              ++ yMinus1;
              ++ yOld;
              ++ yNew;

            }

            for ( y = yMaxMinusHalfNeighborhoodSize; y < yMax; y++ )
            {

              for ( x = xMin; x < xMax; x++ )
              {

                vtmp( x, y, z ) = vtmp( x,yMinus1, z ) - v( x,yOld, z );

              }
              ++ yMinus1;
              ++ yOld;

            }

          }
          v.fill( 0.0f );  

          // --> Along Z
          int32_t zMinus1 = 0;
          int32_t zOld = 0;
          int32_t zNew = 0;
          int32_t zMaxMinusHalfNeighborhoodSize = zMax - _halfNeighborhoodSize;
          int32_t zMinPlusHalfNeighborhoodSize = zMin + _halfNeighborhoodSize;
          for ( z = zMin; z <= zMinPlusHalfNeighborhoodSize; z++ )
          {

      	    for ( y = yMin; y < yMax; y++ )
      	    {

      	      for ( x = xMin; x < xMax; x++ )
      	      {

      	        v( x, y, zMin ) += vtmp( x, y, z );

      	      }

      	    }

          }

          zMinus1 = zMin;
          zNew = zMinPlusHalfNeighborhoodSize + 1;
          for ( z = zMin + 1; z <= zMinPlusHalfNeighborhoodSize; z++ )
          {

            for ( y = yMin; y < yMax; y++ )
      	    {

              for ( x = xMin; x < xMax; x++ )
      	      {

      	        v( x, y, z ) += v( x, y, zMinus1 ) + vtmp( x, y, zNew );

              }

            }
            ++ zMinus1;
            ++ zNew;

          }

          zOld = zMinus1 - _halfNeighborhoodSize;
          for ( z = zMinPlusHalfNeighborhoodSize + 1;
                z < zMaxMinusHalfNeighborhoodSize; z++ )
          {

      	    for ( y = yMin; y < yMax; y++ )
      	    {

              for ( x = xMin; x < xMax; x++ )
              {

                v( x, y, z ) += v( x, y, zMinus1 ) - vtmp( x, y, zOld ) +
                                vtmp( x, y, zNew );

              }

      	    }
            ++ zMinus1;
            ++ zOld;
            ++ zNew;

          }

          for ( z = zMaxMinusHalfNeighborhoodSize; z < zMax; z++ )
          {

            for ( y = yMin; y < yMax; y++ )
            {

              for ( x = xMin; x < xMax; x++ )
      	      {

                v( x, y, z ) += v( x, y, zMinus1 ) - vtmp( x, y, zOld );

      	      }

      	    }
            ++ zMinus1;
            ++ zOld;

          }

          // update pixel values
          float w = 0.0f;
          float value = 0.0f;
          for ( z = zMin; z < zMax; z++ )                      
          {

            for ( y = yMin; y < yMax; y++ )                    
            {

              for ( x = xMin; x < xMax; x++ )                  
              {

                value = std::max( v( x, y, z ) / _h, 0.0f );
                if ( value >= 29.0f )
                {

                  w = 0.0f;

                }
                else
                {

                  int32_t x1 = ( int32_t )std::floor( value * 1000.0f );
                  float y1 = exponentialLut[ x1 ];

                  w = y1 + ( exponentialLut[ x1 + 1 ] - y1 ) *
                           ( value - ( float )x1 );

                }
                outputSubVolume( x, y, z ) += 
                  _noiseModel->getValue( w, 
                                         inputVolume( x + blockOffset.x,
                                                      y + blockOffset.y,
                                                      z + blockOffset.z,
                                                      t ) );
                outputSubVolume( x + blockOffset.x,
                                 y + blockOffset.y,
                                 z + blockOffset.z ) += 
                  _noiseModel->getValue( w,
                                         inputVolume( i,
                                                      j,
                                                      k,
                                                      t ) );
                M( x, y, z ) = std::max( M( x, y, z ), w );          
                M( x + blockOffset.x,
                   y + blockOffset.y,
                   z + blockOffset.z ) =  std::max( M( x + blockOffset.x,
                                                       y + blockOffset.y,
                                                       z + blockOffset.z ),
                                                    w );
                Z( x, y, z ) += w;
                Z( x + blockOffset.x,
                   y + blockOffset.y,
                   z + blockOffset.z ) += w;                            

              }

            }

          }

        }


        // add contribution of the noisy pixel and normalize
        float w = 0.0f;
        float N = 0.0f;
        float inputValue = 0.0f;
        float outputValue = 0.0f;
        float filteredValue = 0.0f;
        for ( z = 0; z < inputSizes.z; z++ )
        {

          for ( y = 0; y < inputSizes.y; y++ )
          {

            for ( x = 0; x < inputSizes.x; x++ )
            {

              inputValue = inputVolume( x, y, z, t );
              w = M( x, y, z );
              N = Z( x, y, z ) + w;
              outputValue = ( outputSubVolume( x, y, z ) +
                              _noiseModel->getValue( w, inputValue ) ) / N;
              filteredValue = _noiseModel->getFiltered( outputValue );
              outputVolume( x, y, z, t ) = ( OUT )( filteredValue >= 0.0f ) ?
                                                    filteredValue :
                                                    inputValue );

            }

          }

        }

      }

      if ( _verbose )
      {

        std::cout << gkg::Eraser( 7 ) << std::endl;

      }

    }
    ////////////////////////////////////////////////////////////////////////////
    // performing in-painting in case input/output volumes are stored in memory
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      int64_t inputSizeXYZ = inputVolume.getSizeXYZ();
      float* outputSubVolume = new float[ inputSizeXYZ ];
      float* Z = new float[ inputSizeXYZ ];
      float* M = new float[ inputSizeXYZ ];

      if ( !outputSubVolume || !Z || !M )
      {

        delete[] outputSubVolume;
        delete[] Z;
        delete[] M;
        throw std::runtime_error( "not enough memory" );

      }

      int32_t t = 0;
      for ( t = 0; t < inputSizeT; t++ )
      {

        if ( _verbose )
        {

          if ( t )
          {

            std::cout << gkg::Eraser( 7 );

          }
          std::cout << "t = " << std::setw( 3 ) << t << std::flush;

        }

        std::memset( outputSubVolume,
                     0, ( uint64_t )inputSizeXYZ *
                     sizeof( float ) );
        std::memset( Z,
                     0, ( uint64_t )inputSizeXYZ *
                     sizeof( float ) );
        std::memset( M,
                     0, ( uint64_t )inputSizeXYZ *
                     sizeof( float ) );

        gkg::NonLocalMeansInPainterInMemoryContext< IN >
          nonLocalMeansInPainterInMemoryContext( inputVolume,
                                                 outputVolume,
                                                 maskVolume,
                                                 inPaintingMaskVolume,
                                                 outputSubVolume,
                                                 M,
                                                 Z,
                                                 _noiseModel,
                                                 offsets,
                                                 _halfNeighborhoodSize,
                                                 _degreeOfFiltering,
                                                 t );
        gkg::ThreadedLoop< int64_t >
          threadedLoop( &nonLocalMeansInPainterInMemoryContext,
                        0,
                        ( int64_t )offsets.size() );
        threadedLoop.launch();

        gkg::NonLocalMeansInPainterContributionContext< IN, OUT >
          nonLocalMeansInPainterContributionContext( inputVolume,
                                                     outputVolume,
                                                     outputSubVolume,
                                                     M,
                                                     Z,
                                                     _noiseModel,
                                                     t );
        gkg::ThreadedLoop< int64_t >
          threadedLoop2( &nonLocalMeansInPainterContributionContext,
                         0,
                         inputSizeXYZ );
        threadedLoop2.launch();

      }

      if ( _verbose )
      {

        std::cout << gkg::Eraser( 7 ) << std::endl;

      }

      delete[] M;
      delete[] Z;
      delete[] outputSubVolume;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "inline "
             "void gkg::NonLocalMeansInPainter< IN, OUT >::inPaint( "
             "const gkg::Volume< IN >& in, "
             "const gkg::Volume< int16_t >& mask, "
             "const gkg::Volume< int16_t >& inPaintingMask, "
             "gkg::Volume< OUT >& out ) const" );

}


#undef LUT_MAXIMUM_EXPONENTIAL


#endif
