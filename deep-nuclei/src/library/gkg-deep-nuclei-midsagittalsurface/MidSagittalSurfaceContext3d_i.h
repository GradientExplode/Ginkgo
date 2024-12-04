#ifndef _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurfaceContext3d_i_h_
#define _gkg_deep_nuclei_midsagittalsurface_MidSagittalSurfaceContext3d_i_h_


#include <gkg-deep-nuclei-midsagittalsurface/MidSagittalSurfaceContext3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::MidSagittalSurfaceContext3d< T >::MidSagittalSurfaceContext3d(
                                 const gkg::Volume< T >& volumeIn,
                                 gkg::Volume< T >& volumeOut,
                                 const gkg::ThinPlateSpline< int32_t >& spline )
                                     : gkg::LoopContext< int32_t >(),
                                       _volumeIn( volumeIn ),
                                       _volumeOut( volumeOut ),
                                       _spline( spline )
{

  try
  {

    _sizeX = _volumeIn.getSizeX();
    _sizeY = _volumeIn.getSizeY();

  }
  GKG_CATCH( "template < class T > "
             "gkg::MidSagittalSurfaceContext3d::MidSagittalSurfaceContext3d( "
             "const gkg::Volume< T >& volumeIn, gkg::Volume< T >& volumeOut, "
             "const gkg::ThinPlateSpline< int32_t >& spline )" );

}


template < class T >
void gkg::MidSagittalSurfaceContext3d< T >::doIt( int32_t startIndex, 
                                                  int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;
    int32_t x, y, z;

    for ( z = startIndex; z < stopIndex; z++ )
    {
    
      for ( y = 0; y < _sizeY; y++ )
      {

        x = int32_t( _spline.getValueAt( y, z ) + 0.5 );

        if ( ( x > 0 ) && ( x < _sizeX ) )
        {

          _volumeOut( x, y, z ) = _volumeIn( x, y, z );

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::MidSagittalSurfaceContext3d< T >::doIt( "
             "int32_t startIndex, int32_t count )" );

}


#endif
