#ifndef _gkg_processing_algobase_IterativeVolumeAverage_i_h_
#define _gkg_processing_algobase_IterativeVolumeAverage_i_h_


#include <gkg-processing-algobase/IterativeVolumeAverage.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::IterativeVolumeAverage< T >::IterativeVolumeAverage()
                                : _volume( new gkg::Volume< T >() ),
                                  _volumeCount( 0 )
{
}


template < class T >
inline
gkg::IterativeVolumeAverage< T >::IterativeVolumeAverage( 
                                 const gkg::IterativeVolumeAverage< T >& other )
                                : _volume( other._volume ),
                                  _volumeCount( other._volumeCount )
{
}


template < class T >
inline
gkg::IterativeVolumeAverage< T >::IterativeVolumeAverage( 
                                                      gkg::Volume< T >& volume )
                                : _volume( new gkg::Volume< T >() ),
                                  _volumeCount( 0 )
{

  try
  {

    addVolume( volume );

  }
  GKG_CATCH( "template < class T > inline "
             "gkg::IterativeVolumeAverage< T >::IterativeVolumeAverage( "
             "const gkg::Volume< T >& volume )" );

}


template < class T >
inline
gkg::IterativeVolumeAverage< T >::~IterativeVolumeAverage()
{
}


template < class T >
inline
void gkg::IterativeVolumeAverage< T >::addVolume( gkg::Volume< T >& volume )
{

  try
  {

    int32_t sizeX = volume.getSizeX();
    int32_t sizeY = volume.getSizeY();
    int32_t sizeZ = volume.getSizeZ();
    int32_t sizeT = volume.getSizeT();

    if ( !volume.hasSameSize( *_volume ) )
    {

      gkg::Vector3d< double > resolution;
      double resolutionT;

      volume.getResolution( resolution, resolutionT );
      _volume->reallocate( sizeX, sizeY, sizeZ, sizeT );
      _volume->setResolution( resolution, resolutionT );
      _volume->fill( 0 );
      _volumeCount = 0;

    }

    uint32_t n = _volumeCount + 1;
    int32_t x, y, z, t;

    for ( t = 0; t < sizeT; t++ )
    {

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            T value = ( *_volume )( x, y, z, t );
            ( *_volume )( x, y, z, t ) = ( value * _volumeCount + 
                                           volume( x, y, z, t ) ) / n;

          }

        }

      }

    }

    _volumeCount++;

  }
  GKG_CATCH( "template < class T > inline "
             "void gkg::IterativeVolumeAverage< T >::addVolume( "
             "const gkg::Volume< T >& volume )" );

}


template < class T >
inline
gkg::RCPointer< gkg::Volume< T > > gkg::IterativeVolumeAverage< T >::getVolume()
{

  return _volume;

}


#endif
