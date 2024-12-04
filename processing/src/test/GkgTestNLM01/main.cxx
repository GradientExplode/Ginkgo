#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/NLMNoiseModelFactory.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Eraser.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cmath>


// Get volume difference


int32_t fBinaryDiff( gkg::Volume< float >& vol1,
                     gkg::Volume< float >& vol2 )
{

  int32_t N = vol1.getSizeX() * vol1.getSizeY() *
              vol1.getSizeZ() * vol1.getSizeT();
  int32_t cpt = 0;
  float* p1 = &vol1( 0 );
  float* p2 = &vol2( 0 );

  while ( N-- )
  {

    if ( *p1++ != *p2++ )
    {

      cpt++;

    }

  }

  return cpt;

}


int32_t u16BinaryDiff( gkg::Volume< uint16_t >& vol1,
                       gkg::Volume< uint16_t >& vol2 )
{

  int32_t N = vol1.getSizeX() * vol1.getSizeY() *
              vol1.getSizeZ() * vol1.getSizeT();
  int32_t cpt = 0;
  uint16_t* p1 = &vol1( 0 );
  uint16_t* p2 = &vol2( 0 );

  while ( N-- )
  {

    if ( *p1++ != *p2++ )
    {

      cpt++;

    }

  }

  return cpt;

}


// Non local means filtering


class NonLocalMeansFilter
{

  public:

    NonLocalMeansFilter( const gkg::NLMNoiseModel& noiseModel,
                         int32_t sizeX,
                         int32_t sizeY,
                         int32_t sizeZ,
                         int32_t halfSearchBlockSize,
                         int32_t halfNeighborhoodSize, 
                         float degreeOfFiltering );
    virtual ~NonLocalMeansFilter();

    void checkSquaredPixels( const gkg::Volume< uint16_t >& in ) const;
    void checkConvolutionX( const gkg::Volume< uint16_t >& in ) const;
    void checkConvolutionY( const gkg::Volume< uint16_t >& in ) const;
    void checkConvolutionZ( const gkg::Volume< uint16_t >& in ) const;
    void checkPixelValuesUpdate( const gkg::Volume< uint16_t >& in ) const;
    void checkContributionNormalization( 
                                      const gkg::Volume< uint16_t >& in ) const;

  private:

    float expLut( float dif ) const;

    const gkg::NLMNoiseModel& _noiseModel;
    int32_t _halfSearchBlockSize;
    int32_t _halfNeighborhoodSize;
    float _degreeOfFiltering;
    std::vector< gkg::Vector3d< int32_t > > _block;
    float _h;
    float _expLut[ 30000 ];

};


NonLocalMeansFilter::NonLocalMeansFilter( const gkg::NLMNoiseModel& noiseModel,
                                          int32_t sizeX,
                                          int32_t sizeY,
                                          int32_t sizeZ,
                                          int32_t halfSearchBlockSize, 
                                          int32_t halfNeighborhoodSize, 
                                          float degreeOfFiltering )
                   : _noiseModel( noiseModel ),
                     _halfSearchBlockSize( halfSearchBlockSize ),
                     _halfNeighborhoodSize( halfNeighborhoodSize ),
                     _degreeOfFiltering( degreeOfFiltering )
{

  int32_t i;
  float n = float( 2 * _halfNeighborhoodSize + 1 );
  float sigma = _noiseModel.getSigma();
  _h = 2.0 * degreeOfFiltering * sigma * sigma * n * n * n;
   
  for ( i = 0; i < 30000; i++ )
  {

    _expLut[ i ] = float( std::exp( -double( i ) / 1000.0 ) );

  }

  // length of search window
  int32_t x, y, z;
  int32_t hX = std::min( _halfSearchBlockSize, ( sizeX - 1 ) / 2 );
  int32_t hY = std::min( _halfSearchBlockSize, ( sizeY - 1 ) / 2 );
  int32_t hZ = std::min( _halfSearchBlockSize, ( sizeZ - 1 ) / 2 );
  int32_t bWidth = -( 2 * hX + 1 ) * ( hZ * ( 2 * hY + 1 ) + hY + hX );

  for ( z = -hZ; z <= hZ; z++ )
  {

    for ( y = -hY; y <= hY; y++ )
    {

      for ( x = -hX; x <= hX; x++, bWidth++ )
      {

        if ( bWidth < 0 )
        {

          _block.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

        }

      }

    }

  }

}


NonLocalMeansFilter::~NonLocalMeansFilter()
{
}


float NonLocalMeansFilter::expLut( float dif ) const
{

  if ( dif >= 29.0f )
  {

    return 0.0f;

  }

  float value = dif * 1000.0f;
  int32_t x = int32_t( std::floor( value ) );
  float y1 = _expLut[ x ];

  return y1 + ( *( _expLut + x + 1 ) - y1 ) * ( value - x );

}


void NonLocalMeansFilter::checkSquaredPixels( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  int32_t sizeXYZ = in.getSizeXYZ();
  gkg::Volume< float > u1( sizeX, sizeY, sizeZ );
  gkg::Volume< float > u2( sizeX, sizeY, sizeZ );
  int32_t chk[ Nb ];

  std::cout << "   number of blocks : " << Nb << std::endl;

  for ( t = 0; t < sizeT; t++ )
  {

    const uint16_t* pI = &in( 0, 0, 0, t );

    std::cout << "   squared pixels for t = "
              << std::setw( 3 ) << t << "  -  " << std::flush;

    std::memset( chk, 0, Nb * sizeof( int32_t ) );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      u1.fill( 0.0f );
      u2.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );                   

      // Non optimized version
      for ( kz = kmin; kz < kmax; kz++ )                
      {                                                 

        for ( ky = jmin; ky < jmax; ky++ )              
        {                                               

          for ( kx = imin; kx < imax; kx++ )            
          {                                             

            float diff = in( kx + nx, ky + ny, kz + nz, t ) - 
                         in( kx, ky, kz, t );
            u1( kx, ky, kz ) = diff * diff;

          }                                             

        }                                               

      }

      // Optimized version
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* pu2 = &u2( 0 );

      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = imin; kx < imax; kx++, k_off++ )
          {

            float diff = pI2[ k_off ] - pI[ k_off ];
            pu2[ k_off ] =  diff * diff;

          }

        }

      }

      // check differences
      chk[ b ] = fBinaryDiff( u1, u2 );

    }

    int32_t cpt = 0;
    int32_t mindiff = sizeXYZ;
    int32_t maxdiff = 0;
    for ( b = 0; b < Nb; b++ )
    {

      if ( chk[ b ] )
      {

        cpt++;

        if ( chk[ b ] < mindiff )
        {

          mindiff = chk[ b ];

        }
        else if ( chk[ b ] > maxdiff )
        {

          maxdiff = chk[ b ];
          
        }

      }

    }
    if ( cpt )
    {

      std::cout << cpt << " blocks are different" 
                << " with mindiff = " << mindiff 
                << " and maxdiff = " << maxdiff << std::endl;

    }
    else
    {

      std::cout << "all blocks are identical" << std::endl;

    }

  }

}


void NonLocalMeansFilter::checkConvolutionX( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  int32_t sizeXYZ = in.getSizeXYZ();
  gkg::Volume< float > u( sizeX, sizeY, sizeZ );
  gkg::Volume< float > v1( sizeX, sizeY, sizeZ );
  gkg::Volume< float > v2( sizeX, sizeY, sizeZ );
  int32_t chk[ Nb ];

  std::cout << "   number of blocks : " << Nb << std::endl;

  for ( t = 0; t < sizeT; t++ )
  {

    const uint16_t* pI = &in( 0, 0, 0, t );

    std::cout << "   convolution along X for t = "
              << std::setw( 3 ) << t << "  -  " << std::flush;

    std::memset( chk, 0, Nb * sizeof( int32_t ) );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      u.fill( 0.0f );
      v1.fill( 0.0f );
      v2.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* p_u = &u( 0 );
      float* v_n = &v2( 0 );
      float diff, diff2;

      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = imin; kx < imax; kx++, k_off++ )
          {

            diff = pI2[ k_off ] - pI[ k_off ];
            p_u[ k_off ] =  diff * diff;

          }

        }

      }

      // Convolution along X
      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )

      // Non optimized verions
      int32_t k_m1, k_old, k_new;
      int32_t dmax = imax - _halfNeighborhoodSize;
      int32_t kPlusHalf = imin + _halfNeighborhoodSize;
      for ( kz = kmin; kz < kmax; kz++ )
      {

        for ( ky = jmin; ky < jmax; ky++ )
        {

          for ( kx = imin; kx <= kPlusHalf ; kx++ )
          {

            v1( imin, ky, kz ) += u( kx, ky, kz );

          }

          k_m1 = imin;
          k_new = kPlusHalf + 1;
          for ( kx = imin + 1; kx <= kPlusHalf; kx++, k_m1++, k_new++ )
          {

            v1( kx, ky, kz ) = v1( k_m1, ky, kz ) + u( k_new, ky, kz );

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = kPlusHalf + 1;
                kx < dmax;
                kx++, k_m1++, k_old++, k_new++ )
          {

            v1( kx, ky, kz ) = v1( k_m1, ky, kz ) - 
                               u( k_old, ky, kz ) + u( k_new, ky, kz );

          }

          for ( kx = dmax; kx < imax; kx++, k_m1++, k_old++ )
          {

            v1( kx, ky, kz ) = v1( k_m1, ky, kz ) - u( k_old, ky, kz );

          }

        }

      }

      // Optimized version
      int32_t w_off;
      int32_t range = imax - imin;
      dmax = range - _halfNeighborhoodSize;
      k_off = k_off0;
      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = 0, w_off = k_off;
                kx <= _halfNeighborhoodSize;
                kx++, w_off++ )
          {

            diff = pI2[ w_off ] - pI[ w_off ];  
            v_n[ k_off ] += diff * diff;

          }

          k_m1 = k_off++;
          k_new = k_off + _halfNeighborhoodSize;
          for ( kx = 1;
                kx <= _halfNeighborhoodSize;
                kx++, k_off++, k_m1++, k_new++ )
          {

            diff = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] + diff * diff;

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = _halfNeighborhoodSize + 1;
                kx < dmax;
                kx++, k_off++, k_m1++, k_old++, k_new++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            diff2 = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff + diff2 * diff2;

          }

          for ( kx = dmax;
                kx < range;
                kx++, k_off++, k_m1++, k_old++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff;

          }

        }

      }

      // check differences
      chk[ b ] = fBinaryDiff( v1, v2 );

    }

    int32_t cpt = 0;
    int32_t mindiff = sizeXYZ;
    int32_t maxdiff = 0;
    for ( b = 0; b < Nb; b++ )
    {

      if ( chk[ b ] )
      {

        cpt++;

        if ( chk[ b ] < mindiff )
        {

          mindiff = chk[ b ];

        }
        else if ( chk[ b ] > maxdiff )
        {

          maxdiff = chk[ b ];
          
        }

      }

    }
    if ( cpt )
    {

      std::cout << cpt << " blocks are different" 
                << " with mindiff = " << mindiff 
                << " and maxdiff = " << maxdiff << std::endl;

    }
    else
    {

      std::cout << "all blocks are identical" << std::endl;

    }

  }

}


void NonLocalMeansFilter::checkConvolutionY( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  int32_t sizeXYZ = in.getSizeXYZ();
  gkg::Volume< float > v( sizeX, sizeY, sizeZ );
  gkg::Volume< float > vtmp1( sizeX, sizeY, sizeZ );
  gkg::Volume< float > vtmp2( sizeX, sizeY, sizeZ );
  int32_t chk[ Nb ];

  std::cout << "   number of blocks : " << Nb << std::endl;

  for ( t = 0; t < sizeT; t++ )
  {

    const uint16_t* pI = &in( 0, 0, 0, t );

    std::cout << "   convolution along Y for t = "
              << std::setw( 3 ) << t << "  -  " << std::flush;

    std::memset( chk, 0, Nb * sizeof( int32_t ) );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      v.fill( 0.0f );
      vtmp1.fill( 0.0f );
      vtmp2.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );                   
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* v_n = &v( 0 );
      float* v_tmp = &vtmp2( 0 );
      float diff, diff2;
      int32_t k_m1, k_old, k_new;
      int32_t w_off;
      int32_t range = imax - imin;
      int32_t dmax = range - _halfNeighborhoodSize;

      // Squared pixels + convolution along X
      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = 0, w_off = k_off;
                kx <= _halfNeighborhoodSize;
                kx++, w_off++ )
          {

            diff = pI2[ w_off ] - pI[ w_off ];  
            v_n[ k_off ] += diff * diff;

          }

          k_m1 = k_off++;
          k_new = k_off + _halfNeighborhoodSize;
          for ( kx = 1;
                kx <= _halfNeighborhoodSize;
                kx++, k_off++, k_m1++, k_new++ )
          {

            diff = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] + diff * diff;

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = _halfNeighborhoodSize + 1;
                kx < dmax;
                kx++, k_off++, k_m1++, k_old++, k_new++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            diff2 = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff + diff2 * diff2;

          }

          for ( kx = dmax;
                kx < range;
                kx++, k_off++, k_m1++, k_old++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff;

          }

        }

      }

      // Convolution along Y

      // Non optimized verion
      dmax = jmax - _halfNeighborhoodSize;
      int32_t kPlusHalf = jmin + _halfNeighborhoodSize;
      for ( kz = kmin; kz < kmax; kz++ )
      {

      	for ( ky = jmin; ky <= kPlusHalf; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp1( kx, jmin, kz ) += v( kx, ky, kz );

      	  }

      	}

      	k_m1 = jmin;
        k_new = kPlusHalf + 1;
      	for ( ky = jmin + 1; ky <= kPlusHalf; ky++, k_m1++, k_new++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp1( kx, ky, kz ) = vtmp1( kx, k_m1, kz ) + v( kx, k_new, kz );

      	  }

      	}

      	k_old = k_m1 - _halfNeighborhoodSize;
      	for ( ky = kPlusHalf + 1;
              ky < dmax;
              ky++, k_m1++, k_old++, k_new++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp1( kx, ky, kz ) = vtmp1( kx, k_m1, kz ) - 
                                  v( kx, k_old, kz ) + v( kx, k_new, kz );

      	  }

      	}

      	for ( ky = dmax; ky < jmax; ky++, k_m1++, k_old++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp1( kx, ky, kz ) = vtmp1( kx, k_m1, kz ) - v( kx, k_old, kz );

      	  }

      	}

      }

      // Optimized version
      int32_t x_off, delta_y = _halfNeighborhoodSize * sizeX;
      range = jmax - jmin;
      dmax = range - _halfNeighborhoodSize;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

      	for ( x_off = k_off, ky = 0;
              ky <= _halfNeighborhoodSize;
              ky++, x_off += delta_j )
      	{

      	  for ( kx = imin, w_off = k_off; kx < imax; kx++, x_off++, w_off++ )
      	  {

      	    v_tmp[ w_off ] += v_n[ x_off ];

      	  }

      	}

      	k_m1 = k_off;
      	k_off += sizeX;
      	k_new = k_off + delta_y;
      	for ( ky = 1;
              ky <= _halfNeighborhoodSize;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] + v_n[ k_new ];

      	  }

      	}

      	k_old = k_m1 - delta_y;
      	for ( ky = _halfNeighborhoodSize + 1;
              ky < dmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ] + v_n[ k_new ];

      	  }

      	}

      	for ( ky = dmax;
              ky < range;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ];

      	  }

      	}

      }

      // check differences
      chk[ b ] = fBinaryDiff( vtmp1, vtmp2 );

    }

    int32_t cpt = 0;
    int32_t mindiff = sizeXYZ;
    int32_t maxdiff = 0;
    for ( b = 0; b < Nb; b++ )
    {

      if ( chk[ b ] )
      {

        cpt++;

        if ( chk[ b ] < mindiff )
        {

          mindiff = chk[ b ];

        }
        else if ( chk[ b ] > maxdiff )
        {

          maxdiff = chk[ b ];
          
        }

      }

    }
    if ( cpt )
    {

      std::cout << cpt << " blocks are different" 
                << " with mindiff = " << mindiff 
                << " and maxdiff = " << maxdiff << std::endl;

    }
    else
    {

      std::cout << "all blocks are identical" << std::endl;

    }

  }

}


void NonLocalMeansFilter::checkConvolutionZ( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  int32_t sizeXYZ = in.getSizeXYZ();
  gkg::Volume< float > v( sizeX, sizeY, sizeZ );
  gkg::Volume< float > vtmp( sizeX, sizeY, sizeZ );
  gkg::Volume< float > v2( sizeX, sizeY, sizeZ );
  int32_t chk[ Nb ];

  std::cout << "   number of blocks : " << Nb << std::endl;

  for ( t = 0; t < sizeT; t++ )
  {

    const uint16_t* pI = &in( 0, 0, 0, t );

    std::cout << "   convolution along Z for t = "
              << std::setw( 3 ) << t << "  -  " << std::flush;

    std::memset( chk, 0, Nb * sizeof( int32_t ) );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      v.fill( 0.0f );
      vtmp.fill( 0.0f );
      v2.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );                   
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* v_n = &v( 0 );
      float* v_tmp = &vtmp( 0 );
      float diff, diff2;
      int32_t k_m1, k_old, k_new;
      int32_t w_off;
      int32_t range = imax - imin;
      int32_t dmax = range - _halfNeighborhoodSize;

      // Squared pixels + convolution along X
      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = 0, w_off = k_off;
                kx <= _halfNeighborhoodSize;
                kx++, w_off++ )
          {

            diff = pI2[ w_off ] - pI[ w_off ];  
            v_n[ k_off ] += diff * diff;

          }

          k_m1 = k_off++;
          k_new = k_off + _halfNeighborhoodSize;
          for ( kx = 1;
                kx <= _halfNeighborhoodSize;
                kx++, k_off++, k_m1++, k_new++ )
          {

            diff = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] + diff * diff;

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = _halfNeighborhoodSize + 1;
                kx < dmax;
                kx++, k_off++, k_m1++, k_old++, k_new++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            diff2 = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff + diff2 * diff2;

          }

          for ( kx = dmax;
                kx < range;
                kx++, k_off++, k_m1++, k_old++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff;

          }

        }

      }

      // Convolution along Y
      int32_t x_off, delta_y = _halfNeighborhoodSize * sizeX;
      range = jmax - jmin;
      dmax = range - _halfNeighborhoodSize;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

      	for ( x_off = k_off, ky = 0;
              ky <= _halfNeighborhoodSize;
              ky++, x_off += delta_j )
      	{

      	  for ( kx = imin, w_off = k_off; kx < imax; kx++, x_off++, w_off++ )
      	  {

      	    v_tmp[ w_off ] += v_n[ x_off ];

      	  }

      	}

      	k_m1 = k_off;
      	k_off += sizeX;
      	k_new = k_off + delta_y;
      	for ( ky = 1;
              ky <= _halfNeighborhoodSize;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] + v_n[ k_new ];

      	  }

      	}

      	k_old = k_m1 - delta_y;
      	for ( ky = _halfNeighborhoodSize + 1;
              ky < dmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ] + v_n[ k_new ];

      	  }

      	}

      	for ( ky = dmax;
              ky < range;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ];

      	  }

      	}

      }

      // Convolution along Z

      // Non optimized verion
      v.fill( 0.0f );  
      dmax = kmax - _halfNeighborhoodSize;
      int32_t kPlusHalf = kmin + _halfNeighborhoodSize;
      for ( kz = kmin; kz <= kPlusHalf; kz++ )
      {

      	for ( ky = jmin; ky < jmax; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    v( kx, ky, kmin ) += vtmp( kx, ky, kz );

      	  }

      	}

      }

      k_m1 = kmin;
      k_new = kPlusHalf + 1;
      for ( kz = kmin + 1; kz <= kPlusHalf; kz++, k_m1++, k_new++ )
      {

      	for ( ky = jmin; ky < jmax; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    v( kx, ky, kz ) += v( kx, ky, k_m1 ) + vtmp( kx, ky, k_new );

      	  }

      	}

      }

      k_old = k_m1 - _halfNeighborhoodSize;
      for ( kz = kPlusHalf + 1;
            kz < dmax;
            kz++, k_m1++, k_old++, k_new++ )
      {

      	for ( ky = jmin; ky < jmax; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    v( kx, ky, kz ) += v( kx, ky, k_m1 ) -
                               vtmp( kx, ky, k_old ) + vtmp( kx, ky, k_new );

      	  }

      	}

      }

      for ( kz = dmax; kz < kmax; kz++, k_m1++, k_old++ )
      {

      	for ( ky = jmin; ky < jmax; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    v( kx, ky, kz ) += v( kx, ky, k_m1 ) - vtmp( kx, ky, k_old );

      	  }

      	}

      }

      // Optimized version
      int32_t y_off, delta_z = _halfNeighborhoodSize * sizeXY;
      range = kmax - kmin;
      dmax = range - _halfNeighborhoodSize;
      v_n = &v2( 0 );
      for ( k_off = k_off0, x_off = k_off0, kz = 0;
            kz <= _halfNeighborhoodSize;
            kz++, x_off += delta_k )
      {

      	for ( y_off = k_off0, ky = jmin;
              ky < jmax;
              ky++, x_off += delta_j, y_off += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, x_off++, y_off++ )
      	  {

      	    v_n[ y_off ] += v_tmp[ x_off ];

      	  }

      	}

      }
      k_m1 = k_off;
      k_off += sizeXY;
      k_new = k_off + delta_z;
      for ( kz = 1;
            kz <= _halfNeighborhoodSize;
            kz++, k_off += delta_k, k_m1 += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] + v_tmp[ k_new ];

      	  }

      	}

      }
      k_old = k_m1 - delta_z;
      for ( kz = _halfNeighborhoodSize + 1;
            kz < dmax;
            kz++, k_off += delta_k, k_m1 += delta_k,
                  k_old += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ] + v_tmp[ k_new ];

      	  }

      	}

      }
      for ( kz = dmax;
            kz < range;
            kz++, k_off += delta_k, k_m1 += delta_k, k_old += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ];

      	  }

      	}

      }

      // check differences
      chk[ b ] = fBinaryDiff( v, v2 );

    }

    int32_t cpt = 0;
    int32_t mindiff = sizeXYZ;
    int32_t maxdiff = 0;
    for ( b = 0; b < Nb; b++ )
    {

      if ( chk[ b ] )
      {

        cpt++;

        if ( chk[ b ] < mindiff )
        {

          mindiff = chk[ b ];

        }
        else if ( chk[ b ] > maxdiff )
        {

          maxdiff = chk[ b ];
          
        }

      }

    }
    if ( cpt )
    {

      std::cout << cpt << " blocks are different" 
                << " with mindiff = " << mindiff 
                << " and maxdiff = " << maxdiff << std::endl;

    }
    else
    {

      std::cout << "all blocks are identical" << std::endl;

    }

  }

}


void NonLocalMeansFilter::checkPixelValuesUpdate( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  gkg::Volume< float > v( sizeX, sizeY, sizeZ );
  gkg::Volume< float > vtmp( sizeX, sizeY, sizeZ );
  gkg::Volume< float > volOut( sizeX, sizeY, sizeZ );
  gkg::Volume< float > M( sizeX, sizeY, sizeZ );
  gkg::Volume< float > Z( sizeX, sizeY, sizeZ );
  gkg::Volume< float > volOut2( sizeX, sizeY, sizeZ );
  gkg::Volume< float > M2( sizeX, sizeY, sizeZ );
  gkg::Volume< float > Z2( sizeX, sizeY, sizeZ );
  int32_t chk[ Nb ], chkM[ Nb ], chkZ[ Nb ];

  std::cout << "   number of blocks : " << Nb << std::endl;

  for ( t = 0; t < sizeT; t++ )
  {

    const uint16_t* pI = &in( 0, 0, 0, t );

    std::cout << "   pixel values update for t = "
              << std::setw( 3 ) << t << "  -  " << std::flush;

    volOut.fill( 0.0f );
    M.fill( 0.0f );
    Z.fill( 0.0f );
    volOut2.fill( 0.0f );
    M2.fill( 0.0f );
    Z2.fill( 0.0f );

    std::memset( chk, 0, Nb * sizeof( int32_t ) );
    std::memset( chkM, 0, Nb * sizeof( int32_t ) );
    std::memset( chkZ, 0, Nb * sizeof( int32_t ) );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      v.fill( 0.0f );
      vtmp.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );                   
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* v_n = &v( 0 );
      float* v_tmp = &vtmp( 0 );
      float diff, diff2;
      int32_t k_m1, k_old, k_new;
      int32_t w_off;
      int32_t range = imax - imin;
      int32_t dmax = range - _halfNeighborhoodSize;

      // Squared pixels + convolution along X
      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = 0, w_off = k_off;
                kx <= _halfNeighborhoodSize;
                kx++, w_off++ )
          {

            diff = pI2[ w_off ] - pI[ w_off ];  
            v_n[ k_off ] += diff * diff;

          }

          k_m1 = k_off++;
          k_new = k_off + _halfNeighborhoodSize;
          for ( kx = 1;
                kx <= _halfNeighborhoodSize;
                kx++, k_off++, k_m1++, k_new++ )
          {

            diff = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] + diff * diff;

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = _halfNeighborhoodSize + 1;
                kx < dmax;
                kx++, k_off++, k_m1++, k_old++, k_new++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            diff2 = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff + diff2 * diff2;

          }

          for ( kx = dmax;
                kx < range;
                kx++, k_off++, k_m1++, k_old++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff;

          }

        }

      }

      // Convolution along Y
      int32_t x_off, delta_y = _halfNeighborhoodSize * sizeX;
      range = jmax - jmin;
      dmax = range - _halfNeighborhoodSize;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

      	for ( x_off = k_off, ky = 0;
              ky <= _halfNeighborhoodSize;
              ky++, x_off += delta_j )
      	{

      	  for ( kx = imin, w_off = k_off; kx < imax; kx++, x_off++, w_off++ )
      	  {

      	    v_tmp[ w_off ] += v_n[ x_off ];

      	  }

      	}

      	k_m1 = k_off;
      	k_off += sizeX;
      	k_new = k_off + delta_y;
      	for ( ky = 1;
              ky <= _halfNeighborhoodSize;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] + v_n[ k_new ];

      	  }

      	}

      	k_old = k_m1 - delta_y;
      	for ( ky = _halfNeighborhoodSize + 1;
              ky < dmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ] + v_n[ k_new ];

      	  }

      	}

      	for ( ky = dmax;
              ky < range;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ];

      	  }

      	}

      }

      // Convolution along Z
      v.fill( 0.0f );  
      int32_t y_off, delta_z = _halfNeighborhoodSize * sizeXY;
      range = kmax - kmin;
      dmax = range - _halfNeighborhoodSize;
      v_n = &v( 0 );
      for ( k_off = k_off0, x_off = k_off0, kz = 0;
            kz <= _halfNeighborhoodSize;
            kz++, x_off += delta_k )
      {

      	for ( y_off = k_off0, ky = jmin;
              ky < jmax;
              ky++, x_off += delta_j, y_off += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, x_off++, y_off++ )
      	  {

      	    v_n[ y_off ] += v_tmp[ x_off ];

      	  }

      	}

      }
      k_m1 = k_off;
      k_off += sizeXY;
      k_new = k_off + delta_z;
      for ( kz = 1;
            kz <= _halfNeighborhoodSize;
            kz++, k_off += delta_k, k_m1 += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] + v_tmp[ k_new ];

      	  }

      	}

      }
      k_old = k_m1 - delta_z;
      for ( kz = _halfNeighborhoodSize + 1;
            kz < dmax;
            kz++, k_off += delta_k, k_m1 += delta_k,
                  k_old += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ] + v_tmp[ k_new ];

      	  }

      	}

      }
      for ( kz = dmax;
            kz < range;
            kz++, k_off += delta_k, k_m1 += delta_k, k_old += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ];

      	  }

      	}

      }

      // Update pixel values

      // Non optimized version
      float value = 0.0f, w = 0.0f;
      for ( kz = kmin; kz < kmax; kz++ )                      
      {                                                       

        for ( ky = jmin; ky < jmax; ky++ )                    
        {                                                     

          for ( kx = imin; kx < imax; kx++ )                  
          {                                                   

            value = std::max( v( kx, ky, kz ) / _h, 0.0f );  
            w = expLut( value ); //std::exp( -value );
            volOut( kx, ky, kz ) += 
                  _noiseModel.getValue( w, in( kx + nx, ky + ny, kz + nz, t ) );
            volOut( kx + nx, ky + ny, kz + nz ) += 
                                 _noiseModel.getValue( w, in( kx, ky, kz, t ) );
            M( kx, ky, kz ) = std::max( M( kx, ky, kz ), w );          
            M( kx + nx, ky + ny, kz + nz ) = 
                                  std::max( M( kx + nx, ky + ny, kz + nz ), w );
            Z( kx, ky, kz ) += w;                            
            Z( kx + nx, ky + ny, kz + nz ) += w;                            

          }                                                   

        }

      }

      // Optimized version
      float* pO = &volOut2( 0 );
      float* pM = &M2( 0 );
      float* pZ = &Z2( 0 );
      float* pO2 = pO + n_off;
      float* pM2 = pM + n_off;
      float* pZ2 = pZ + n_off;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {                                                       

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {                                                     

          for ( kx = imin; kx < imax; kx++, k_off++ )
          {                                                   

            float val = std::max( v_n[ k_off ] / _h, 0.0f );  
            float fWeight = expLut( val );             
            pO[ k_off ] += _noiseModel.getValue( fWeight, pI2[ k_off ] );
            pO2[ k_off ] += _noiseModel.getValue( fWeight, pI[ k_off ] );
            pM[ k_off ] = std::max( pM[ k_off ], fWeight );          
            pM2[ k_off ] = std::max( pM2[ k_off ], fWeight );        
            pZ[ k_off ] += fWeight;                            
            pZ2[ k_off ] += fWeight;                           

          }                                                   

        }

      }

      // check differences
      chk[ b ] = fBinaryDiff( volOut, volOut2 );
      chkM[ b ] = fBinaryDiff( M, M2 );
      chkZ[ b ] = fBinaryDiff( Z, Z2 );

    }

    int32_t cpt = 0, cptM = 0, cptZ = 0;
    for ( b = 0; b < Nb; b++ )
    {

      if ( chk[ b ] )
      {

        cpt++;

      }
      if ( chkM[ b ] )
      {

        cptM++;

      }
      if ( chkZ[ b ] )
      {

        cptZ++;

      }

    }
    if ( cpt )
    {

      std::cout << cpt << "volOut blocks KO - " << std::flush;

    }
    else
    {

      std::cout << "volOut OK - " << std::flush;

    }
    if ( cptM )
    {

      std::cout << cptM << "M blocks KO - " << std::flush;

    }
    else
    {

      std::cout << "M OK - " << std::flush;

    }
    if ( cptZ )
    {

      std::cout << cptZ << "Z blocks KO" << std::endl;

    }
    else
    {

      std::cout << "Z OK" << std::endl;

    }

  }

}


void NonLocalMeansFilter::checkContributionNormalization( 
                                       const gkg::Volume< uint16_t >& in ) const
{

  int32_t t, b, Nb = int32_t( _block.size() );
  int32_t sizeX = in.getSizeX();
  int32_t sizeY = in.getSizeY();
  int32_t sizeZ = in.getSizeZ();
  int32_t sizeT = in.getSizeT();
  int32_t sizeXY = in.getSizeXY();
  int32_t sizeXYZ = in.getSizeXYZ();
  gkg::Volume< float > v( sizeX, sizeY, sizeZ );
  gkg::Volume< float > vtmp( sizeX, sizeY, sizeZ );
  gkg::Volume< float > volOut( sizeX, sizeY, sizeZ );
  gkg::Volume< float > M( sizeX, sizeY, sizeZ );
  gkg::Volume< float > Z( sizeX, sizeY, sizeZ );
  gkg::Volume< uint16_t > out( sizeX, sizeY, sizeZ, sizeT );
  gkg::Volume< uint16_t > out2( sizeX, sizeY, sizeZ, sizeT );

  out.fill( 0 );
  out2.fill( 0 );

  std::cout << "   number of blocks : " << Nb << std::endl;
  std::cout << "   contribution and normalization - t = " << std::flush;

  for ( t = 0; t < sizeT; t++ )
  {

    if ( t )
    {

      std::cout << gkg::Eraser( 3 );

    }
    std::cout << std::setw( 3 ) << t << std::flush;

    const uint16_t* pI = &in( 0, 0, 0, t );

    volOut.fill( 0.0f );
    M.fill( 0.0f );
    Z.fill( 0.0f );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      v.fill( 0.0f );
      vtmp.fill( 0.0f );

      // image of squared pixel values u = ( y( k + n ) - y( k ) )^2
      int kx, ky, kz;                                   
      int imin = std::max( -nx, 0 );                              
      int jmin = std::max( -ny, 0 );                              
      int kmin = std::max( -nz, 0 );                              
      int imax = std::min( sizeX - nx, sizeX );
      int jmax = std::min( sizeY - ny, sizeY );                 
      int kmax = std::min( sizeZ - nz, sizeZ );                   
      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      const uint16_t* pI2 = pI + n_off;
      float* v_n = &v( 0 );
      float* v_tmp = &vtmp( 0 );
      float diff, diff2;
      int32_t k_m1, k_old, k_new;
      int32_t w_off;
      int32_t range = imax - imin;
      int32_t dmax = range - _halfNeighborhoodSize;

      // Squared pixels + convolution along X
      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      for ( kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {

          for ( kx = 0, w_off = k_off;
                kx <= _halfNeighborhoodSize;
                kx++, w_off++ )
          {

            diff = pI2[ w_off ] - pI[ w_off ];  
            v_n[ k_off ] += diff * diff;

          }

          k_m1 = k_off++;
          k_new = k_off + _halfNeighborhoodSize;
          for ( kx = 1;
                kx <= _halfNeighborhoodSize;
                kx++, k_off++, k_m1++, k_new++ )
          {

            diff = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] + diff * diff;

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = _halfNeighborhoodSize + 1;
                kx < dmax;
                kx++, k_off++, k_m1++, k_old++, k_new++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            diff2 = pI2[ k_new ] - pI[ k_new ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff + diff2 * diff2;

          }

          for ( kx = dmax;
                kx < range;
                kx++, k_off++, k_m1++, k_old++ )
          {

            diff = pI2[ k_old ] - pI[ k_old ];  
            v_n[ k_off ] = v_n[ k_m1 ] - diff * diff;

          }

        }

      }

      // Convolution along Y
      int32_t x_off, delta_y = _halfNeighborhoodSize * sizeX;
      range = jmax - jmin;
      dmax = range - _halfNeighborhoodSize;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {

      	for ( x_off = k_off, ky = 0;
              ky <= _halfNeighborhoodSize;
              ky++, x_off += delta_j )
      	{

      	  for ( kx = imin, w_off = k_off; kx < imax; kx++, x_off++, w_off++ )
      	  {

      	    v_tmp[ w_off ] += v_n[ x_off ];

      	  }

      	}

      	k_m1 = k_off;
      	k_off += sizeX;
      	k_new = k_off + delta_y;
      	for ( ky = 1;
              ky <= _halfNeighborhoodSize;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] + v_n[ k_new ];

      	  }

      	}

      	k_old = k_m1 - delta_y;
      	for ( ky = _halfNeighborhoodSize + 1;
              ky < dmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ] + v_n[ k_new ];

      	  }

      	}

      	for ( ky = dmax;
              ky < range;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

      	    v_tmp[ k_off ] = v_tmp[ k_m1 ] - v_n[ k_old ];

      	  }

      	}

      }

      // Convolution along Z
      v.fill( 0.0f );  
      int32_t y_off, delta_z = _halfNeighborhoodSize * sizeXY;
      range = kmax - kmin;
      dmax = range - _halfNeighborhoodSize;
      v_n = &v( 0 );
      for ( k_off = k_off0, x_off = k_off0, kz = 0;
            kz <= _halfNeighborhoodSize;
            kz++, x_off += delta_k )
      {

      	for ( y_off = k_off0, ky = jmin;
              ky < jmax;
              ky++, x_off += delta_j, y_off += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, x_off++, y_off++ )
      	  {

      	    v_n[ y_off ] += v_tmp[ x_off ];

      	  }

      	}

      }
      k_m1 = k_off;
      k_off += sizeXY;
      k_new = k_off + delta_z;
      for ( kz = 1;
            kz <= _halfNeighborhoodSize;
            kz++, k_off += delta_k, k_m1 += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] + v_tmp[ k_new ];

      	  }

      	}

      }
      k_old = k_m1 - delta_z;
      for ( kz = _halfNeighborhoodSize + 1;
            kz < dmax;
            kz++, k_off += delta_k, k_m1 += delta_k,
                  k_old += delta_k, k_new += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j,
                    k_old += delta_j, k_new += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++, k_new++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ] + v_tmp[ k_new ];

      	  }

      	}

      }
      for ( kz = dmax;
            kz < range;
            kz++, k_off += delta_k, k_m1 += delta_k, k_old += delta_k )
      {

      	for ( ky = jmin;
              ky < jmax;
              ky++, k_off += delta_j, k_m1 += delta_j, k_old += delta_j )
      	{

      	  for ( kx = imin; kx < imax; kx++, k_off++, k_m1++, k_old++ )
      	  {

            v_n[ k_off ] = v_n[ k_m1 ] - v_tmp[ k_old ];

      	  }

      	}

      }

      // Update pixel values
      float* pO = &volOut( 0 );
      float* pM = &M( 0 );
      float* pZ = &Z( 0 );
      float* pO2 = pO + n_off;
      float* pM2 = pM + n_off;
      float* pZ2 = pZ + n_off;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {                                                       

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {                                                     

          for ( kx = imin; kx < imax; kx++, k_off++ )
          {                                                   

            float val = std::max( v_n[ k_off ] / _h, 0.0f );  
            float fWeight = expLut( val );             
            pO[ k_off ] += _noiseModel.getValue( fWeight, pI2[ k_off ] );
            pO2[ k_off ] += _noiseModel.getValue( fWeight, pI[ k_off ] );
            pM[ k_off ] = std::max( pM[ k_off ], fWeight );          
            pM2[ k_off ] = std::max( pM2[ k_off ], fWeight );        
            pZ[ k_off ] += fWeight;                            
            pZ2[ k_off ] += fWeight;                           

          }                                                   

        }

      }

    }

    // Non optimized verion
    int32_t x, y, z;
    float N, valueOut, f;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          float value = float( in( x, y, z, t ) );
          float w = M( x, y, z );
          N = Z( x, y, z ) + w;
          valueOut = ( volOut( x, y, z ) + 
                       _noiseModel.getValue( w, value ) ) / N;
          f = _noiseModel.getFiltered( valueOut );
          out( x, y, z, t ) = uint16_t( ( f >= 0.0f ) ? f : value );

        }

      }

    }

    // Optimized version
    int32_t Ntot = sizeXYZ;
    uint16_t* pOut = &out2( 0, 0, 0, t );
    float* fpO = &volOut( 0 );
    float* pM = &M( 0 );
    float* pZ = &Z( 0 );
    while ( Ntot-- )
    {

      N = *pZ++ + *pM;
      valueOut = ( *fpO++ + _noiseModel.getValue( *pM++, float( *pI ) ) ) / N;
      f = _noiseModel.getFiltered( valueOut );
      *pOut++ = uint16_t( ( f >= 0.0f ) ? f : *pI );
      pI++;

    }

  }

  // check differences
  int32_t chk = u16BinaryDiff( out, out2 );
  if ( chk )
  {

    std::cout << chk << " - values are different" << std::endl;

  }
  else
  {

    std::cout << " - volumes are identical" << std::endl;

  }

}


// main


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string filename;
    gkg::Volume< uint16_t > volume;
    gkg::Volume< float > out1;
    gkg::Volume< float > out2;
    std::string noiseModel = "gaussian";
    int32_t halfSearchBlocSize = 8;
    int32_t halfNeighborhoodSize = 1;
    float degreeOfFiltering = 1.0f;
    float sigma = 1.0f;
    float n = 1.0f;

    gkg::Application application( argc, argv, "Unit test NLM" );
    application.addSingleOption( "-i",
                                 "input file name (must be UINT16)", 
                                 filename );
    application.initialize();

    std::cout << "========== NLM unit tests ==========" << std::endl;

    std::cout << "Reading file : " << filename;
    gkg::Reader::getInstance().read( filename, volume );
    std::cout << " - done" << std::endl;

    std::cout << "Sanity checks" << std::endl;
    if ( halfNeighborhoodSize >= halfSearchBlocSize )
    {

      std::cout << "  halfSearchBlocSize must be lower than halfSearchBlocSize"
                << " - abort" << std::endl;

      return EXIT_FAILURE;

    }

    std::cout << "Getting NLM " << noiseModel << " noise model" << std::endl;
    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    gkg::NLMNoiseModel* nlmNoiseModel = 
        gkg::NLMNoiseModelFactory::getInstance().createNLMNoiseModelFunction(
                                                                    noiseModel,
                                                                    sigma,
                                                                    n );

    std::cout << "Setting NLM filter" << std::endl;
    NonLocalMeansFilter NLMfilter( *nlmNoiseModel, sizeX, sizeY, sizeZ, 
                                   halfSearchBlocSize, halfNeighborhoodSize,
                                   degreeOfFiltering );

    std::cout << "=== Checking NLM image of squared pixels" << std::endl;
    NLMfilter.checkSquaredPixels( volume );

    std::cout << "=== Checking NLM convolution along X" << std::endl;
    NLMfilter.checkConvolutionX( volume );

    std::cout << "=== Checking NLM convolution along Y" << std::endl;
    NLMfilter.checkConvolutionY( volume );

    std::cout << "=== Checking NLM convolution along Z" << std::endl;
    NLMfilter.checkConvolutionZ( volume );

    std::cout << "=== Checking NLM pixel values update" << std::endl;
    NLMfilter.checkPixelValuesUpdate( volume );

    std::cout << "=== Checking NLM contribution and normalization" << std::endl;
    NLMfilter.checkContributionNormalization( volume );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
