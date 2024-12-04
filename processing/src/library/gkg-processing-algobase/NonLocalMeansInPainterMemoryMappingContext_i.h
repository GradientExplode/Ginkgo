#ifndef _gkg_processing_algobase_NonLocalMeansContextMmap_i_h_
#define _gkg_processing_algobase_NonLocalMeansContextMmap_i_h_


#include <gkg-processing-algobase/NonLocalMeansContextMmap.h>
#include <gkg-processing-algobase/NLMNoiseModel.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <algorithm>


template < class IN, class OUT >
inline
gkg::NonLocalMeansContextMmap< IN, OUT >::NonLocalMeansContextMmap( 
                           const gkg::Volume< IN >& in,
                           gkg::Volume< OUT >& out,
                           gkg::Volume< float >& volOut,
                           gkg::Volume< float >& M,
                           gkg::Volume< float >& Z,
                           const gkg::NLMNoiseModel& noiseModel,
                           const std::vector< gkg::Vector3d< int32_t > >& block,
                           const int32_t& halfNeighborhoodSize, 
                           const float& degreeOfFiltering,
                           const int32_t& t )
                                        : _in( in ),
                                          _out( out ),
                                          _volOut( volOut ),
                                          _M( M ),
                                          _Z( Z ),
                                          _noiseModel( noiseModel ),
                                          _block( block ),
                                          _halfNeighborhoodSize(
                                                         halfNeighborhoodSize ),
                                          _t( t )
{

  try
  {

    int32_t i;
    float n = float( 2 * _halfNeighborhoodSize + 1 );
    float sigma = _noiseModel.getSigma();
    _h = 2.0 * degreeOfFiltering * sigma * sigma * n * n * n;
   
    for ( i = 0; i < 30000; i++ )
    {

      _expLut[ i ] = float( std::exp( -double( i ) / 1000.0 ) );

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "gkg::NonLocalMeansContextMmap< IN, OUT >::"
             "NonLocalMeansContextMmap( "
             "const gkg::Volume< IN >& in, gkg::Volume< OUT >& out, "
             "gkg::Volume< float >& volOut, gkg::Volume< float >& M, "
             "gkg::Volume< float >& Z, const gkg::NLMNoiseModel& noiseModel, "
             "const std::vector< Vector3d< int32_t > >& block, "
             "const int32_t& halfNeighborhoodSize, "
             "const float& degreeOfFiltering, const int32_t& t )" );

}


template < class IN, class OUT >
inline
gkg::NonLocalMeansContextMmap< IN, OUT >::~NonLocalMeansContextMmap()
{
}


template < class IN, class OUT >
inline
void gkg::NonLocalMeansContextMmap< IN, OUT >::execute()
{

  try
  {

    float w, value;
    int32_t x, y, z, b, Nb = int32_t( _block.size() );
    const int32_t sizeX = _in.getSizeX();
    const int32_t sizeY = _in.getSizeY();
    const int32_t sizeZ = _in.getSizeZ();
    gkg::Volume< float > u( sizeX, sizeY, sizeZ );
    gkg::Volume< float > v( sizeX, sizeY, sizeZ );
    gkg::Volume< float > vtmp( sizeX, sizeY, sizeZ );

    for ( b = 0; b < Nb; b++ )
    {

      int32_t nx = _block[ b ].x;
      int32_t ny = _block[ b ].y;
      int32_t nz = _block[ b ].z;

      u.fill( 0.0f );
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

      for ( kz = kmin; kz < kmax; kz++ )                
      {                                                 

        for ( ky = jmin; ky < jmax; ky++ )              
        {                                               

          for ( kx = imin; kx < imax; kx++ )            
          {                                             

            float diff = _in( kx + nx, ky + ny, kz + nz, _t ) - 
                         _in( kx, ky, kz, _t );
            u( kx, ky, kz ) = diff * diff;

          }                                             

        }                                               

      }                                                 

      // convolution : v_n = u_n * h, with h = 1_[-P,...,P]^3
      // separable : v_n = ( ( u_n * h_x ) * h_y ) * h_z, with h_i = 1[-P,...,P]
      // h_i ~= moving average
      // recursive implementation: 
      //   v_n( k ) = v_n( k - 1 ) - u_n( k - 1 - P ) + u_n( k + P )
      // --> Along X
      int32_t k_m1, k_old, k_new;
      int32_t dmax = imax - _halfNeighborhoodSize;
      int32_t kPlusHalf = imin + _halfNeighborhoodSize;
      for ( kz = kmin; kz < kmax; kz++ )
      {

        for ( ky = jmin; ky < jmax; ky++ )
        {

          for ( kx = imin; kx <= kPlusHalf ; kx++ )
          {

            v( imin, ky, kz ) += u( kx, ky, kz );

          }

          k_m1 = imin;
          k_new = kPlusHalf + 1;
          for ( kx = imin + 1; kx <= kPlusHalf; kx++, k_m1++, k_new++ )
          {

            v( kx, ky, kz ) = v( k_m1, ky, kz ) + u( k_new, ky, kz );

          }

          k_old = k_m1 - _halfNeighborhoodSize;
          for ( kx = kPlusHalf + 1;
                kx < dmax;
                kx++, k_m1++, k_old++, k_new++ )
          {

            v( kx, ky, kz ) = v( k_m1, ky, kz ) - 
                              u( k_old, ky, kz ) + u( k_new, ky, kz );

          }

          for ( kx = dmax; kx < imax; kx++, k_m1++, k_old++ )
          {

            v( kx, ky, kz ) = v( k_m1, ky, kz ) - u( k_old, ky, kz );

          }

        }

      }
      // --> Along Y
      dmax = jmax - _halfNeighborhoodSize;
      kPlusHalf = jmin + _halfNeighborhoodSize;
      for ( kz = kmin; kz < kmax; kz++ )
      {

      	for ( ky = jmin; ky <= kPlusHalf; ky++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp( kx, jmin, kz ) += v( kx, ky, kz );

      	  }

      	}

      	k_m1 = jmin;
        k_new = kPlusHalf + 1;
      	for ( ky = jmin + 1; ky <= kPlusHalf; ky++, k_m1++, k_new++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp( kx, ky, kz ) = vtmp( kx, k_m1, kz ) + v( kx, k_new, kz );

      	  }

      	}

      	k_old = k_m1 - _halfNeighborhoodSize;
      	for ( ky = kPlusHalf + 1;
              ky < dmax;
              ky++, k_m1++, k_old++, k_new++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp( kx, ky, kz ) = vtmp( kx, k_m1, kz ) - 
                                 v( kx, k_old, kz ) + v( kx, k_new, kz );

      	  }

      	}

      	for ( ky = dmax; ky < jmax; ky++, k_m1++, k_old++ )
      	{

      	  for ( kx = imin; kx < imax; kx++ )
      	  {

      	    vtmp( kx, ky, kz ) = vtmp( kx, k_m1, kz ) - v( kx, k_old, kz );

      	  }

      	}

      }
      v.fill( 0.0f );  
      // --> Along Z
      dmax = kmax - _halfNeighborhoodSize;
      kPlusHalf = kmin + _halfNeighborhoodSize;
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

      // update pixel values                                  
      for ( kz = kmin; kz < kmax; kz++ )                      
      {                                                       

        for ( ky = jmin; ky < jmax; ky++ )                    
        {                                                     

          for ( kx = imin; kx < imax; kx++ )                  
          {                                                   

            value = std::max( v( kx, ky, kz ) / _h, 0.0f );  
            w = expLut( value ); //std::exp( -value );
            _volOut( kx, ky, kz ) += 
                _noiseModel.getValue( w, _in( kx + nx, ky + ny, kz + nz, _t ) );
            _volOut( kx + nx, ky + ny, kz + nz ) += 
                               _noiseModel.getValue( w, _in( kx, ky, kz, _t ) );
            _M( kx, ky, kz ) = std::max( _M( kx, ky, kz ), w );          
            _M( kx + nx, ky + ny, kz + nz ) = 
                                 std::max( _M( kx + nx, ky + ny, kz + nz ), w );
            _Z( kx, ky, kz ) += w;                            
            _Z( kx + nx, ky + ny, kz + nz ) += w;                            

          }                                                   

        }

      }

    }

    // add contribution of the noisy pixel and normalize
    float N, valueOut, f;
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          value = _in( x, y, z, _t );
          w = _M( x, y, z );
          N = _Z( x, y, z ) + w;
          valueOut = ( _volOut( x, y, z ) + 
                       _noiseModel.getValue( w, value ) ) / N;
          f = _noiseModel.getFiltered( valueOut );
          _out( x, y, z, _t ) = OUT( ( f >= 0.0f ) ? f : value );

        }

      }

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::NonLocalMeansContextMmap< IN, OUT >::execute()" );

}


template < class IN, class OUT >
inline
float gkg::NonLocalMeansContextMmap< IN, OUT >::expLut( float dif )
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
