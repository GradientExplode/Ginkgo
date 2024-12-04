#ifndef _gkg_processing_algobase_NonLocalMeansContext_i_h_
#define _gkg_processing_algobase_NonLocalMeansContext_i_h_


#include <gkg-processing-algobase/NonLocalMeansContext.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <algorithm>


template < class T >
inline
gkg::NonLocalMeansContext< T >::NonLocalMeansContext( 
                           const gkg::Volume< T >& in,
                           float* out,
                           float* M,
                           float* Z,
                           const gkg::NLMNoiseModel& noiseModel,
                           const std::vector< gkg::Vector3d< int32_t > >& block,
                           const int32_t& halfNeighborhoodSize, 
                           const float& degreeOfFiltering,
                           const int32_t& t )
                              : gkg::LoopContext< int32_t >(),
                                _in( in ),
                                _out( out ),
                                _M( M ),
                                _Z( Z ),
                                _noiseModel( noiseModel ),
                                _block( block ),
                                _halfNeighborhoodSize( halfNeighborhoodSize ),
                                _t( t )
{

  try
  {

    int32_t i;
    float n = float( 2 * _halfNeighborhoodSize + 1 );
    float sigma = _noiseModel.getSigma();
    _h = 2.0f * degreeOfFiltering * sigma * sigma * n * n * n;
   
    for ( i = 0; i < 30000; i++ )
    {

      _expLut[ i ] = float( std::exp( -double( i ) / 1000.0 ) );

    }

  }
  GKG_CATCH( "template < T > "
             "gkg::NonLocalMeansContext< T >::NonLocalMeansContext( "
             "const gkg::Volume< T >& in, float* out, float* M, float* Z, "
             "const gkg::NLMNoiseModel& noiseModel, "
             "const std::vector< Vector3d< int32_t > >& block, "
             "const int32_t& halfNeighborhoodSize, "
             "const float& degreeOfFiltering, const int32_t& t )" );

}


template < class T >
inline
void gkg::NonLocalMeansContext< T >::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    const int32_t sizeX = _in.getSizeX();
    const int32_t sizeY = _in.getSizeY();
    const int32_t sizeZ = _in.getSizeZ();
    const int32_t sizeXY = _in.getSizeXY();
    const int32_t sizeXYZ = _in.getSizeXYZ();
    const gkg::Vector3d< int32_t >* blockPtr = &_block[ startIndex ];
    float* O = new float[ sizeXYZ ];
    float* M = new float[ sizeXYZ ];
    float* Z = new float[ sizeXYZ ];
    float* v_n = new float[ sizeXYZ ];
    float* v_tmp = new float[ sizeXYZ ];
    const T* pI = &_in( 0, 0, 0, _t );

    if ( !O || !M || !Z || !v_n || !v_tmp )
    {

      delete[] O;
      delete[] M;
      delete[] Z;
      delete[] v_n;
      delete[] v_tmp;
      throw std::runtime_error( "not enough memory for thread buffers" );

    }

    std::memset( O, 0, sizeXYZ * sizeof( float ) );  
    std::memset( M, 0, sizeXYZ * sizeof( float ) );  
    std::memset( Z, 0, sizeXYZ * sizeof( float ) );  

    while ( count-- )
    {

      int32_t nx = blockPtr->x;
      int32_t ny = blockPtr->y;
      int32_t nz = blockPtr->z;

      std::memset( v_n, 0, sizeXYZ * sizeof( float ) );  
      std::memset( v_tmp, 0, sizeXYZ * sizeof( float ) );  

      // image of squared pixel values u_n( k ) = ( y( k + n ) - y( k ) )^2
      int32_t kx, ky, kz;                                   
      int32_t imin = std::max( -nx, 0 );                              
      int32_t jmin = std::max( -ny, 0 );                              
      int32_t kmin = std::max( -nz, 0 );                              
      int32_t imax = std::min( sizeX - nx, sizeX );
      int32_t jmax = std::min( sizeY - ny, sizeY );                 
      int32_t kmax = std::min( sizeZ - nz, sizeZ );                   

      int32_t n_off = nz * sizeXY + ny * sizeX + nx;
      int32_t delta_j = std::abs( nx );
      int32_t delta_k = std::abs( ny ) * sizeX;
      int32_t k_off0 = kmin * sizeXY + jmin * sizeX + imin;
      int32_t k_off = k_off0;
      float diff, diff2;

      const T* pI2 = pI + n_off;

      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      // --> squared difference values + convolution along X
      int32_t k_m1, k_old, k_new;
      int32_t range = imax - imin;
      int32_t dmax = range - _halfNeighborhoodSize;
      int32_t w_off;
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
      // --> convolution along Y
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
      std::memset( v_n, 0, sizeXYZ * sizeof( float ) );  
      // --> convolution along Z
      int32_t y_off, delta_z = _halfNeighborhoodSize * sizeXY;
      range = kmax - kmin;
      dmax = range - _halfNeighborhoodSize;
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

      // update pixel values                                  
      float* O2 = O + n_off;
      float* M2 = M + n_off;
      float* Z2 = Z + n_off;
      for ( k_off = k_off0, kz = kmin; kz < kmax; kz++, k_off += delta_k )
      {                                                       

        for ( ky = jmin; ky < jmax; ky++, k_off += delta_j )
        {                                                     

          for ( kx = imin; kx < imax; kx++, k_off++ )
          {                                                   

            float val = std::max( v_n[ k_off ] / _h, 0.0f );  
            float fWeight = expLut( val );             
            O[ k_off ] += _noiseModel.getValue( fWeight, pI2[ k_off ] );
            O2[ k_off ] += _noiseModel.getValue( fWeight, pI[ k_off ] );
            M[ k_off ] = std::max( M[ k_off ], fWeight );          
            M2[ k_off ] = std::max( M2[ k_off ], fWeight );        
            Z[ k_off ] += fWeight;                            
            Z2[ k_off ] += fWeight;                           

          }                                                   

        }

      }

      blockPtr++;

    }

    // update global volumes
    lock();

    int32_t N = sizeXYZ;
    float* pO_out = _out;
    float* pM_out = _M;
    float* pZ_out = _Z;
    float* pO = O;
    float* pM = M;
    float* pZ = Z;

    while ( N-- )
    {

      *pO_out += *pO++;
      *pM_out = std::max( *pM_out, *pM++ );
      *pZ_out += *pZ++;
      pO_out++;
      pM_out++;
      pZ_out++;

    }

    unlock();

    delete[] O;
    delete[] M;
    delete[] Z;
    delete[] v_n;
    delete[] v_tmp;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLocalMeansContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


template < class T >
inline
float gkg::NonLocalMeansContext< T >::expLut( float dif )
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


#endif
