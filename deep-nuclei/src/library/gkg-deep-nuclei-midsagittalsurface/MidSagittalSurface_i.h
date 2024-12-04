#ifndef _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurface_i_h_
#define _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurface_i_h_


#include <gkg-deep-nuclei-midsagittalsurface/MidSagittalSurface.h>
#include <gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext2d_i.h>
#include <gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext3d_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


template < class T >
gkg::MidSagittalSurface< T >::MidSagittalSurface( int32_t slabWidth,
                                                  double sigma,
                                                  double minCorrelation,
                                                  double regularization )
                            : _slabWidth( slabWidth ),
                              _sigma( sigma ),
                              _minCorrelation( minCorrelation ),
                              _regularization( regularization )
{
}


template < class T >
gkg::MidSagittalSurface< T >::~MidSagittalSurface()
{
}


template < class T >
void gkg::MidSagittalSurface< T >::compute( const gkg::Volume< T >& volumeIn,
                                            const gkg::Volume< uint8_t >& mask,
                                            gkg::Volume< T >& volumeOut,
                                            bool output3D,
                                            bool verbose )
{

  try
  {

    int32_t x, y, z;
    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    int32_t minX = sizeX - 1;
    int32_t maxX = 0;
    int32_t halfSlabWidth = _slabWidth / 2 + _slabWidth % 2;
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );

    volumeIn.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "select points : " << std::flush;

    }

    // get min and max values in the X direction of the mask 
    // to provide an initial position of the slab center at ( max + min ) / 2
    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

	  if ( mask( x, y, z ) )
	  {
	  
	    if ( x < minX )
	    {
	    
	      minX = x;
	    
	    }
	    else if ( x > maxX )
	    {
	    
	      maxX = x;
	    
	    }
	  
	  }          

        }

      }

    }

    int32_t slice = ( minX + maxX ) / 2;
    int32_t sliceMin = slice - halfSlabWidth;
    int32_t sliceMax = slice + halfSlabWidth;

    if ( sliceMin > minX )
    {

      minX = sliceMin;

    }
    if ( sliceMax < maxX )
    {

      maxX = sliceMax;

    }

    // get slab
    int32_t i, n = maxX - minX + 1;
    gkg::Volume< T > slab( n, sizeY, sizeZ );
    slab.setResolution( resolution );

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = minX, i = 0; x <= maxX; i++, x++ )
        {
	  
          slab( i, y, z ) = volumeIn( x, y, z );

        }

      }

    }

    // smoothing and derivation along X
    gkg::Volume< T > smooth( n, sizeY, sizeZ );
    smooth.setResolution( resolution );
    gkg::Volume< T > gradient( n, sizeY, sizeZ );
    gradient.setResolution( resolution );
    gkg::DericheGaussianCoefficients< float > smoothingCoefficients(
                         _sigma,
                         gkg::DericheGaussianCoefficients< float >::Smoothing );
    gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  slab,
                                  smoothingCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::X_AXIS,
                                  smooth );
    gkg::DericheGaussianCoefficients< float > gradientCoefficients(
                          _sigma,
                          gkg::DericheGaussianCoefficients< float >::Gradient );
    gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  slab,
                                  gradientCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::X_AXIS,
                                  gradient );

    // select candidate points, i.e. best center of symmetry from
    // cross-correlation
    std::vector< gkg::Vector2d< int32_t > > controlPoints;
    std::vector< double > values;
    int32_t n2 = n / 2;
    int32_t n4 = n2 / 2;
    int32_t N = n - n2;
    float den = (float)n2;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        int32_t index = -1;
        float max = 0.0f;
        int32_t posMin = 0;
        int32_t posMax = n2 - 1; 

        for ( x = 0; x < N; x++, posMin++, posMax++ )
        {

          float mx = 0.0, mx2 = 0.0;
          float my = 0.0, my2 = 0.0;
          float mxy = 0.0;
          int32_t xmin = posMin;
          int32_t xmax = posMax;

          for ( i = 0; i < n2; i++, xmin++, xmax-- )
          {

            float v1 = (float)smooth( xmin, y, z );
            float v2 = (float)smooth( xmax, y, z );
            mx += v1;
            mx2 += v1 * v1;
            my += v2;
            my2 += v2 * v2;
            mxy += v1 * v2;

          }

          float correlation = ( mxy - mx * my / den ) /
                              std::sqrt( ( mx2 - mx * mx / den ) * 
                                         ( my2 - my * my / den ) );

          if ( correlation > max )
          {

            max = correlation;
            index = x;

          }

        }

        if ( ( index >= 0 ) && ( max >= _minCorrelation ) &&
             ( std::fabs( gradient( n4 + index, y, z ) ) < 1 ) )
        {

          int32_t pos = minX + n4 + index;

          if ( mask( pos, y, z ) )
          {

            controlPoints.push_back( gkg::Vector2d< int32_t >( y, z ) );
            values.push_back( double( pos ) );

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "evaluate surface : " << std::flush;

    }

    // fit a thin plate spline
    _spline.evaluate( controlPoints, values, _regularization );

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "fill output volume : " << std::flush;

    }
    
    if ( output3D )
    {
    
      fill3d( volumeIn, volumeOut );
    
    }
    else
    {
    
      fill2d( volumeIn, volumeOut );
    
    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::MidSagittalSurface< T >::compute( "
             "const gkg::Volume< T >& volumeIn, "
             "const gkg::Volume< uint8_t >& mask, "
             "gkg::Volume< T >& volumeOut, "
             "bool output3D, bool verbose )" );

}


template < class T >
void gkg::MidSagittalSurface< T >::fill2d( const gkg::Volume< T >& volumeIn,
                                           gkg::Volume< T >& volumeOut ) const
{

  try
  {

    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    gkg::Vector3d< double > resolution;
    
    volumeIn.getResolution( resolution );    
    volumeOut.reallocate( sizeY, sizeZ );
    volumeOut.setResolution( resolution );
    volumeOut.fill( 0 );

    gkg::MidSagittalSurfaceContext2d< T > context( volumeIn, 
                                                   volumeOut,
                                                   _spline );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sizeZ );

    threadedLoop.launch();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalSurface< T >::fill2d( "
             "const gkg::Volume< T >& volumeIn, "
             "gkg::Volume< T >& volumeOut ) const" );

}


template < class T >
void gkg::MidSagittalSurface< T >::fill3d( const gkg::Volume< T >& volumeIn,
                                           gkg::Volume< T >& volumeOut ) const
{

  try
  {

    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    gkg::Vector3d< double > resolution;
    
    volumeIn.getResolution( resolution );    
    volumeOut.reallocate( sizeX, sizeY, sizeZ );
    volumeOut.setResolution( resolution );
    volumeOut.fill( 0 );

    gkg::MidSagittalSurfaceContext3d< T > context( volumeIn, 
                                                   volumeOut,
                                                   _spline );
    gkg::ThreadedLoop< int32_t > threadedLoop( &context, 0, sizeZ );

    threadedLoop.launch();

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalSurface< T >::fill3d( "
             "const gkg::Volume< T >& volumeIn, "
             "gkg::Volume< T >& volumeOut ) const" );

}


#endif
