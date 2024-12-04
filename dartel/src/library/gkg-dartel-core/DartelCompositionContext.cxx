#include <gkg-dartel-core/DartelCompositionContext.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


// circulant boundary
static int32_t bound( int32_t i, int32_t m )
{

  return ( i >= 0 ) ? i % ( m ) : ( ( m ) + i % ( m ) ) % m;

}


template < class T >
gkg::DartelCompositionContext< T >::DartelCompositionContext( int32_t sX, 
                                                              int32_t sY,
                                                              int32_t sZ,
                                                              T *pB,
                                                              T *pA,
                                                              T *pC )
                                  : gkg::LoopContext< int32_t >(),
                                    _sizeX( sX ),
                                    _sizeY( sY ),
                                    _sizeZ( sZ ),
                                    _m( sX * sY * sZ ),
                                    _A( pA ),
                                    _B( pB ),
                                    _C( pC )
{
}


template < class T >
void gkg::DartelCompositionContext< T >::doIt( int32_t startIndex,
                                               int32_t count )
{

  try
  {

    T* Ax = _A + startIndex;
    T* Ay = Ax + _m;
    T* Az = Ay + _m;
    T* Bx = _B;
    T* By = _B + _m;
    T* Bz = By + _m;
    T* Cx = _C + startIndex;
    T* Cy = Cx + _m;
    T* Cz = Cy + _m;

    while ( count-- )
    {

      double x, y, z;
      double k000, k100, k010, k110, k001, k101, k011, k111;
      double dx1, dx2, dy1, dy2, dz1, dz2;
      int32_t ix, iy, iz, ix1, iy1, iz1;
      int32_t o000, o100, o010, o110, o001, o101, o011, o111;
      int32_t tmpz, tmpy;

      x = *Ax++;
      y = *Ay++;
      z = *Az++;
      ix = int32_t( floor( x ) );
      dx1 = x - ix;
      dx2 = 1.0 - dx1;
      iy = int32_t( floor( y ) );
      dy1 = y - iy;
      dy2 = 1.0 - dy1;
      iz = int32_t( floor( z ) );
      dz1 = z - iz;
      dz2 = 1.0 - dz1;
      ix = bound( ix, _sizeX );
      iy = bound( iy, _sizeY );
      iz = bound( iz, _sizeZ );
      ix1 = bound( ix + 1, _sizeX );
      iy1 = bound( iy + 1, _sizeY );
      iz1 = bound( iz + 1, _sizeZ );

      tmpz = _sizeY * iz;
      tmpy = _sizeX * ( iy + tmpz );
      o000 = ix + tmpy;
      o100 = ix1 + tmpy;
      tmpy = _sizeX * ( iy1 + tmpz );
      o010 = ix + tmpy;
      o110 = ix1 + tmpy;
      tmpz = _sizeY * iz1;
      tmpy = _sizeX * ( iy + tmpz );
      o001 = ix + tmpy;
      o101 = ix1 + tmpy;
      tmpy = _sizeX * ( iy1 + tmpz );
      o011 = ix + tmpy;
      o111 = ix1 + tmpy;

      k000 = Bx[ o000 ];
      k100 = Bx[ o100 ];
      k010 = Bx[ o010 ];
      k110 = Bx[ o110 ];
      k001 = Bx[ o001 ];
      k101 = Bx[ o101 ];
      k011 = Bx[ o011 ];
      k111 = Bx[ o111 ];

      k100 -= floor( ( k100 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k010 -= floor( ( k010 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k110 -= floor( ( k110 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k001 -= floor( ( k001 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k101 -= floor( ( k101 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k011 -= floor( ( k011 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      k111 -= floor( ( k111 - k000 ) / _sizeX + 0.5 ) * _sizeX;
      *Cx++ = ( ( k000 * dx2 + k100 * dx1 ) * dy2 +
                ( k010 * dx2 + k110 * dx1 ) * dy1 ) * dz2 +
    	      ( ( k001 * dx2 + k101 * dx1 ) * dy2 +
                ( k011 * dx2 + k111 * dx1 ) * dy1 ) * dz1;

      k000 = By[ o000 ];
      k100 = By[ o100 ];
      k010 = By[ o010 ];
      k110 = By[ o110 ];
      k001 = By[ o001 ];
      k101 = By[ o101 ];
      k011 = By[ o011 ];
      k111 = By[ o111 ];

      k100 -= floor( ( k100 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k010 -= floor( ( k010 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k110 -= floor( ( k110 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k001 -= floor( ( k001 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k101 -= floor( ( k101 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k011 -= floor( ( k011 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      k111 -= floor( ( k111 - k000 ) / _sizeY + 0.5 ) * _sizeY;
      *Cy++ = ( ( k000 * dx2 + k100 * dx1 ) * dy2 +
                ( k010 * dx2 + k110 * dx1 ) * dy1 ) * dz2 +
    	      ( ( k001 * dx2 + k101 * dx1 ) * dy2 +
                ( k011 * dx2 + k111 * dx1 ) * dy1 ) * dz1;

      k000 = Bz[ o000 ];
      k100 = Bz[ o100 ];
      k010 = Bz[ o010 ];
      k110 = Bz[ o110 ];
      k001 = Bz[ o001 ];
      k101 = Bz[ o101 ];
      k011 = Bz[ o011 ];
      k111 = Bz[ o111 ];

      k100 -= floor( ( k100 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k010 -= floor( ( k010 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k110 -= floor( ( k110 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k001 -= floor( ( k001 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k101 -= floor( ( k101 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k011 -= floor( ( k011 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      k111 -= floor( ( k111 - k000 ) / _sizeZ + 0.5 ) * _sizeZ;
      *Cz++ = ( ( k000 * dx2 + k100 * dx1 ) * dy2 +
                ( k010 * dx2 + k110 * dx1 ) * dy1 ) * dz2 +
    	      ( ( k001 * dx2 + k101 * dx1 ) * dy2 +
                ( k011 * dx2 + k111 * dx1 ) * dy1 ) * dz1;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::DartelCompositionContext< T >::doIt( "
             "int32_t startIndex, int32_t count )" );
  
}


template class gkg::DartelCompositionContext< float >;
template class gkg::DartelCompositionContext< double >;
