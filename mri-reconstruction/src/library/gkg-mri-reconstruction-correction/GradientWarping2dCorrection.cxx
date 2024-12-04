#include <gkg-mri-reconstruction-correction/GradientWarping2dCorrection.h>
#include <gkg-mri-reconstruction-correction/GradientCoil.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/RealPartConverter_i.h>
#include <gkg-processing-algobase/ImagPartConverter_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <algorithm>


//
//
// The input to this routine is three gradwarp points which represent the 
// end-points (or verticies) of a plane in a 3-D coordinate system as shown 
// below:
//
//            ^ y
//            |
//            |       P0(x0,y0,z0)
//            |         *-----------* P1(x1,y1,z1)
//            |         |           |
//            |         |           |
//            |         |           |
//            |         |           |
//            |         |           |
//            |         *-----------*              
//            |       P2(x2, y2, z2)
//            |
//            |
//            +-----------------------------> x
//           / (0,0,0)
//          /
//         /
//        /
//       /
//      /  
//     z
//
//


#define GW2D_ORDER 6
#define GW2D_LOG_INTERPOLATION_SIZE 10
#define GW2D_INTERPOLATION_SIZE    ( 1 << GW2D_LOG_INTERPOLATION_SIZE )


static uint32_t fracExponent = ( 127 + 23 ) - GW2D_LOG_INTERPOLATION_SIZE;
static uint32_t fracMask = ( 1 << GW2D_LOG_INTERPOLATION_SIZE ) - 1U;


inline
float interpolate( float pixel0,
                   float pixel1,
                   const std::vector< float >& lineIn,
                   const gkg::Volume< float >& interpolationTable )
{

  float* pixelFlt = &pixel1;
  uint32_t pixelInt = *( uint32_t* )pixelFlt;
  uint32_t exponent = pixelInt >> 23;
  exponent &= 0xff;

  uint32_t shift = fracExponent - exponent;
  int32_t interpolationIndex = ( int32_t )( ( pixelInt >> shift ) & fracMask );
  int32_t inputIndex = ( int32_t )pixel1;

  float accumulator = 0.0;
  int32_t i;
  for ( i = 0; i < 4; i++ )
  {

    accumulator += lineIn[ inputIndex + i - 1 ] *
                   interpolationTable( interpolationIndex, i );

  }

  return accumulator * ( pixel1 - pixel0 );

}


template < class T >
gkg::GradientWarping2dCorrection< T >::GradientWarping2dCorrection()
{
}


template < class T >
gkg::GradientWarping2dCorrection< T >::~GradientWarping2dCorrection()
{
}


template < class T >
void gkg::GradientWarping2dCorrection< T >::estimate(
      const gkg::GradientCoil< T >& gradientCoil,
      const std::vector< typename 
                         gkg::GradientWarping2dCorrection< T >::Plane >& planes,
      std::vector< typename
                   gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds,
      int32_t sizeX,
      int32_t sizeY,
      bool verbose ) const
{

  try
  {

    int32_t slice, sliceCount = ( int32_t )planes.size();

    wedge2ds = std::vector< typename
                          gkg::GradientWarping2dCorrection< T >::Wedge2d >(
                                                                   sliceCount );

    std::vector< float > wedgeX;
    std::vector< float > wedgeY;
    std::vector< float > wedgeZ;

    for ( slice = 0; slice < sliceCount; slice ++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

      wedge2ds[ slice ].row = std::vector< float >( GRADIENT_COIL_WEDGE_COUNT );
      wedge2ds[ slice ].column =
        std::vector< float >( GRADIENT_COIL_WEDGE_COUNT );

      //
      // vector in magnitude coordinate along frequency axis
      //
      gkg::Vector3d< T >
        deltaFrequency = planes[ slice ].point2 - planes[ slice ].point1;
      // 10.0 in order to convert mm -> cm for gradient warping model
      deltaFrequency /= ( T )( sizeX - 1 );

      //
      // vector in magnitude coordinate along phase axis
      //
      gkg::Vector3d< T >
        deltaPhase = planes[ slice ].point3 - planes[ slice ].point1;
      // 10.0 in order to convert mm -> cm for gradient warping model
      deltaPhase /= ( T )( sizeY - 1 );


      //
      // getting gradient wedges in the magnet x, y, z directions
      //
      gradientCoil.getXYZWedges( deltaPhase,
                                 deltaFrequency,
                                 planes[ slice ].point1,
                                 wedgeX,
                                 wedgeY,
                                 wedgeZ );


      //
      // getting normalized vector(s)
      //
      T deltaFrequencyNorm = deltaFrequency.getNorm();
      if ( !deltaFrequencyNorm )
      {

        deltaFrequencyNorm = ( T )1.0;

      }
      gkg::Vector3d< T > normalizedDeltaFrequency( deltaFrequency );
      normalizedDeltaFrequency /= deltaFrequencyNorm;

      T deltaPhaseNorm = deltaPhase.getNorm();
      if ( !deltaPhaseNorm )
      {

        deltaPhaseNorm = ( T )1.0;

      }
      gkg::Vector3d< T > normalizedDeltaPhase( deltaPhase );
      normalizedDeltaPhase /= deltaPhaseNorm;

      //
      // rotating row wedge
      //
      int32_t o1, o2, count;
      count = 0;
      for ( o1 = 0; o1 < GW2D_ORDER; o1++ )
      {

        for ( o2 = 0; o2 < GW2D_ORDER - o1; o2++ )
        {

          wedge2ds[ slice ].row[ count ] =
            normalizedDeltaPhase.dot( gkg::Vector3d< T >( wedgeX[ count ],
                                                          wedgeY[ count ],
                                                          wedgeZ[ count ] ) ) /
            deltaPhaseNorm;
          ++ count;                                           

        }

      }
      wedge2ds[ slice ].row[ 1 ] += 1.0;

      //
      // rotating column wedge (with transposition included)
      //
      count = 0;
      int32_t countbis = 0;
      for ( o1 = 0; o1 < GW2D_ORDER; o1++ )
      {

        countbis = o1;
        for ( o2 = 0; o2 < GW2D_ORDER - o1; o2++ )
        {

          wedge2ds[ slice ].column[ countbis ] =
            normalizedDeltaFrequency.dot(
                                  gkg::Vector3d< T >( wedgeX[ count ],
                                                      wedgeY[ count ],
                                                      wedgeZ[ count ] ) ) /
            deltaFrequencyNorm;
          ++ count;
          countbis += GW2D_ORDER - o2;

        }

      }
      wedge2ds[ slice ].column[ 1 ] += 1.0;

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "void gkg::GradientWarping2dCorrection< T >::estimate( "
             "const gkg::GradientCoil< T >& gradientCoil, "
             "const std::vector< typename "
             "gkg::GradientWarping2dCorrection< T >::Plane >& planes, "
             "std::vector< typename "
             "gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds, "
             "int sizeX, "
             "int sizeY, "
             "bool verbose ) const" );

}



template < class T >
void gkg::GradientWarping2dCorrection< T >::apply(
      gkg::Volume< T >& data,
      const std::vector< typename 
         gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds,
      bool swapXY,
      bool verbose ) const
{

  try
  {

    int32_t sliceCount = ( int32_t )data.getSizeZ();

    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "input data has sizeT different from 1" );

    }


    if ( ( int32_t )wedge2ds.size() != sliceCount )
    {

      throw std::runtime_error(
                    "inconsistent gradient warping 2d correction slice count" );

    }

    // allocating temporary buffer for current slice
    int32_t sizeX = ( swapXY ? data.getSizeY() : data.getSizeX() );
    int32_t sizeY = ( swapXY ? data.getSizeX() : data.getSizeY() );
    gkg::Volume< T > sliceInYX( sizeY, sizeX );
    gkg::Volume< T > sliceOutYX( sizeY, sizeX );
    gkg::Volume< T > sliceInXY( sizeX, sizeY );
    gkg::Volume< T > sliceOutXY( sizeX, sizeY );

    // pre-processing interpolation table
    uint32_t resolutionInt = ( 127 - GW2D_LOG_INTERPOLATION_SIZE ) << 23;
    uint32_t* resolutionIntPtr = &resolutionInt;
    float resolution = *( ( float* )resolutionIntPtr );


    gkg::Volume< float > interpolationTable( GW2D_INTERPOLATION_SIZE, 4 );

    int32_t i;
    float delta;
    for ( i = 0; i < GW2D_INTERPOLATION_SIZE; i++ )
    {

      delta = ( float )i * resolution;
      interpolationTable( i, 0 ) = 2.0 * delta * delta - delta -
                                   delta * delta * delta;
      interpolationTable( i, 1 ) = 1.0 - 2.0 * delta * delta +
                                   delta * delta * delta;
      interpolationTable( i, 2 ) = delta + delta * delta -
                                   delta * delta * delta;
      interpolationTable( i, 3 ) = delta * delta * delta - delta * delta;
         
    }

    // pre-processing power of 2 table(s)
    int32_t powerTableSize = std::max( sizeX, sizeY );
    std::vector< float > power1Table( powerTableSize );
    std::vector< float > power2Table( powerTableSize );
    std::vector< float > power3Table( powerTableSize );
    std::vector< float > power4Table( powerTableSize );
    std::vector< float > power5Table( powerTableSize );

    for ( i = 0; i < powerTableSize - 2; i++ )
    {

      power1Table[ i ] = ( 2.0 + ( float )i );
      power2Table[ i ] = power1Table[ i ] * ( 2.0 + ( float )i );
      power3Table[ i ] = power2Table[ i ] * ( 2.0 + ( float )i );
      power4Table[ i ] = power3Table[ i ] * ( 2.0 + ( float )i );
      power5Table[ i ] = power4Table[ i ] * ( 2.0 + ( float )i );

    }
    power1Table[ powerTableSize - 2 ] = 0.0;
    power2Table[ powerTableSize - 2 ] = 0.0;
    power3Table[ powerTableSize - 2 ] = 0.0;
    power4Table[ powerTableSize - 2 ] = 0.0;
    power5Table[ powerTableSize - 2 ] = 0.0;
    power1Table[ powerTableSize - 1 ] = 0.0;
    power2Table[ powerTableSize - 1 ] = 0.0;
    power3Table[ powerTableSize - 1 ] = 0.0;
    power4Table[ powerTableSize - 1 ] = 0.0;
    power5Table[ powerTableSize - 1 ] = 0.0;

    // looping over slice(s)
    int32_t x, y, slice;
    for ( slice = 0; slice < sliceCount; slice ++ )
    {

      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

      // copying current slice with transposition
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( swapXY )
          {

            sliceInYX( y, x ) = data( sizeY - 1 - y, x, slice );

          }
          else
          {

            sliceInYX( y, x ) = data( x, y, slice );

          }

        }

      }

      // warping column(s)
      warp2d( sliceInYX,
              wedge2ds[ slice ].column,
              interpolationTable,
              power1Table,
              power2Table,
              power3Table,
              power4Table,
              power5Table,
              sliceOutYX );

      // copying current slice with transposition
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          sliceInXY( x, y ) = sliceOutYX( y, x );

        }

      }

      // warping row(s)
      warp2d( sliceInXY,
              wedge2ds[ slice ].row,
              interpolationTable,
              power1Table,
              power2Table,
              power3Table,
              power4Table,
              power5Table,
              sliceOutXY );

      // copying back wraped slice
      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( swapXY )
          {

            data( sizeY - 1 - y, x, slice ) = sliceOutXY( x, y );

          }
          else
          {

            data( x, y, slice ) = sliceOutXY( x, y );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientWarping2dCorrection< T >::apply( "
             "gkg::Volume< T >& data, "
             "const std::vector< typename  "
             "gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds ) "
             "const" );

}


template < class T >
void gkg::GradientWarping2dCorrection< T >::apply(
      gkg::Volume< std::complex< T > >& data,
      const std::vector< typename 
         gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds,
      bool swapXY,
      bool verbose ) const
{

  try
  {

    gkg::Volume< T > realData;
    gkg::Volume< T > imagData;

    gkg::RealPartConverter< gkg::Volume< std::complex< T > >,
                            gkg::Volume< T > > realPartConverter;
    gkg::ImagPartConverter< gkg::Volume< std::complex< T > >,
                            gkg::Volume< T > > imagPartConverter;

    realPartConverter.convert( data, realData );
    imagPartConverter.convert( data, imagData );

    this->apply( realData, wedge2ds, swapXY, verbose );
    this->apply( imagData, wedge2ds, swapXY, verbose );


    typename gkg::Volume< std::complex< T > >::iterator d = data.begin(),
                                                        de = data.end();
    typename gkg::Volume< T >::iterator r = realData.begin();
    typename gkg::Volume< T >::iterator i = imagData.begin();
    while ( d != de )
    {

      *d = std::complex< T >( *r, *i );
      ++ d;
      ++ r;
      ++ i;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientWarping2dCorrection< T >::apply( "
             "gkg::Volume< std::complex< T > >& data, "
             "const std::vector< typename  "
             "gkg::GradientWarping2dCorrection< T >::Wedge2d >& wedge2ds ) "
             "const" );

}


template < class T >
void gkg::GradientWarping2dCorrection< T >::warp2d(
                       const gkg::Volume< T >& sliceIn,
                       const std::vector< float >& wedge,
                       const gkg::Volume< float >& interpolationTable,
                       const std::vector< float >& power1Table,
                       const std::vector< float >& power2Table,
                       const std::vector< float >& power3Table,
                       const std::vector< float >& power4Table,
                       const std::vector< float >& power5Table,
                       gkg::Volume< T >& sliceOut ) const
{

  try
  {

    int32_t sizeX = sliceIn.getSizeX();
    int32_t sizeY = sliceIn.getSizeY();

    int32_t x, y;

    uint32_t roundInt = ( 126 - GW2D_LOG_INTERPOLATION_SIZE ) << 23;
    uint32_t* roundIntPtr = &roundInt;
    float round = *( ( float* )roundIntPtr );

    float c00 = ( float )wedge[  0 ] + 1.0 + round;
    float c10 = ( float )wedge[  1 ];
    float c20 = ( float )wedge[  2 ];
    float c30 = ( float )wedge[  3 ];
    float c40 = ( float )wedge[  4 ];
    float c50 = ( float )wedge[  5 ];
    float c01 = ( float )wedge[  6 ];
    float c11 = ( float )wedge[  7 ];
    float c21 = ( float )wedge[  8 ];
    float c31 = ( float )wedge[  9 ];
    float c41 = ( float )wedge[ 10 ];
    float c02 = ( float )wedge[ 11 ];
    float c12 = ( float )wedge[ 12 ];
    float c22 = ( float )wedge[ 13 ];
    float c32 = ( float )wedge[ 14 ];
    float c03 = ( float )wedge[ 15 ];
    float c13 = ( float )wedge[ 16 ];
    float c23 = ( float )wedge[ 17 ];
    float c04 = ( float )wedge[ 18 ];
    float c14 = ( float )wedge[ 19 ];
    float c05 = ( float )wedge[ 20 ];

    float p1, p2, p3, p4, p5;

    std::vector< float > lineIn( sizeX + 3 );
    std::vector< float > lineOut( sizeX );

    std::vector< float > params( 8 );
    for ( y = 0; y < sizeY; y++ )
    {

      p1 = ( float )y;
      p2 = p1 * p1;
      p3 = p2 * p1;
      p4 = p3 * p1;
      p5 = p4 * p1;

      params[ 0 ] = c05 * p5 + c04 * p4 + c03 * p3 + c02 * p2 + c01 * p1 + c00;
      params[ 1 ] = c14 * p4 + c13 * p3 + c12 * p2 + c11 * p1 + c10;
      params[ 2 ] = c23 * p3 + c22 * p2 + c21 * p1 + c20;
      params[ 3 ] = c32 * p2 + c31 * p1 + c30;
      params[ 4 ] = c41 * p1 + c40;
      params[ 5 ] = c50;
      params[ 6 ] = 1.0;
      params[ 7 ] = ( float )sizeX + 1.0 - round;

      // copying input line and replicating first point once on the left
      // and last point twice on the right
      for ( x = 0; x < sizeX; x++ )
      {

        lineIn[ x + 1 ] = ( float )sliceIn( x, y );

      }
      lineIn[ 0 ] = lineIn[ 1 ];
      lineIn[ sizeX + 2 ] = lineIn[ sizeX + 1 ] = lineIn[ sizeX ];

      warp1d( lineIn,
              params,
              interpolationTable,
              power1Table,
              power2Table,
              power3Table,
              power4Table,
              power5Table,
              lineOut );

      // copying back warped line to output slice
      for ( x = 0; x < sizeX; x++ )
      {

        sliceOut( x, y ) = ( T )lineOut[ x ];

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientWarping2dCorrection< T >::warp2d( "
             "const gkg::Volume< T >& sliceIn, "
             "const std::vector< float >& wedge, "
             "const gkg::Volume< float >& interpolationTable, "
             "const std::vector< float >& power1Table, "
             "const std::vector< float >& power2Table, "
             "const std::vector< float >& power3Table, "
             "const std::vector< float >& power4Table, "
             "const std::vector< float >& power5Table, "
             "gkg::Volume< T >& sliceOut ) const" );

}


template < class T >
void gkg::GradientWarping2dCorrection< T >::warp1d(
                       const std::vector< float >& lineIn,
                       const std::vector< float >& params,
                       const gkg::Volume< float >& interpolationTable,
                       const std::vector< float >& power1Table,
                       const std::vector< float >& power2Table,
                       const std::vector< float >& power3Table,
                       const std::vector< float >& power4Table,
                       const std::vector< float >& power5Table,
                       std::vector< float >& lineOut ) const
{

  try
  {

    float pixel0 = params[ 0 ];
    if ( pixel0 < params[ 6 ] )
    {

      pixel0 = params[ 6 ];

    }
    else if ( pixel0 > params[ 7 ] )
    {

      pixel0 = params[ 7 ];

    }

    float pixel1 = params[ 0 ] +
                   params[ 1 ] +
                   params[ 2 ] +
                   params[ 3 ] +
                   params[ 4 ] +
                   params[ 5 ];
    if ( pixel1 < params[ 6 ] )
    {

      pixel1 = params[ 6 ];

    }
    else if ( pixel1 > params[ 7 ] )
    {

      pixel1 = params[ 7 ];

    }

    lineOut[ 0 ] = interpolate( pixel0,
                                pixel0,
                                lineIn,
                                interpolationTable );
    lineOut[ 1 ] = interpolate( pixel0,
                                pixel1,
                                lineIn,
                                interpolationTable );

    int32_t i;
    for ( i = 0; i < ( int32_t )lineOut.size() - 2; i++ )
    {

      pixel0 = pixel1;
      pixel1 = params[ 5 ] * power5Table[ i ] +
               params[ 4 ] * power4Table[ i ] +
               params[ 3 ] * power3Table[ i ] +
               params[ 2 ] * power2Table[ i ] +
               params[ 1 ] * power1Table[ i ] +
               params[ 0 ];
      if ( pixel1 < params[ 6 ] )
      {

        pixel1 = params[ 6 ];

      }
      else if ( pixel1 > params[ 7 ] )
      {

        pixel1 = params[ 7 ];

      }


      lineOut[ 2 + i ] = interpolate( pixel0,
                                      pixel1,
                                      lineIn,
                                      interpolationTable );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::GradientWarping2dCorrection< T >::warp1d( "
             "const std::vector< T >& lineIn, "
             "float params[ 8 ], "
             "const gkg::Volume< float >& interpolationTable, "
             "const std::vector< float >& power1Table, "
             "const std::vector< float >& power2Table, "
             "const std::vector< float >& power3Table, "
             "const std::vector< float >& power4Table, "
             "const std::vector< float >& power5Table, "
             "std::vector< T >& lineOut ) const" );

}


template class gkg::GradientWarping2dCorrection< float >;
template class gkg::GradientWarping2dCorrection< double >;


#undef GW2D_ORDER
#undef GW2D_LOG_INTERPOLATION_SIZE
#undef GW2D_INTERPOLATION_SIZE
