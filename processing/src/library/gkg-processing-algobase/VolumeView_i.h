#ifndef _gkg_processing_algobase_VolumeView_i_h_
#define _gkg_processing_algobase_VolumeView_i_h_


#include <gkg-processing-algobase/VolumeView.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


template < class T >
inline
gkg::VolumeView< T >::VolumeView( const gkg::Volume< T >& volume )
                     : _volume( volume )
{
}


template < class T >
inline
gkg::VolumeView< T >::~VolumeView()
{
}


template < class T >
inline
void gkg::VolumeView< T >::extract( 
                                 const gkg::BoundingBox< int32_t >& boundingBox,
                                 gkg::Volume< T >& out ) const
{

  try
  {

    gkg::BoundingBox< int32_t > bbox( _volume );

    if ( !bbox.contains( boundingBox ) )
    {

      throw std::runtime_error( "not a valid bounding box" );

    }

    int32_t offsetX = boundingBox.getLowerX();
    int32_t offsetY = boundingBox.getLowerY();
    int32_t offsetZ = boundingBox.getLowerZ();
    int32_t offsetT = boundingBox.getLowerT();

    int32_t subSizeX = boundingBox.getUpperX() - offsetX + 1;
    int32_t subSizeY = boundingBox.getUpperY() - offsetY + 1;
    int32_t subSizeZ = boundingBox.getUpperZ() - offsetZ + 1;
    int32_t subSizeT = boundingBox.getUpperT() - offsetT + 1;

    out.reallocate( subSizeX, subSizeY, subSizeZ, subSizeT );

    int32_t x, y, z, t;
    for ( t = 0; t < subSizeT; t++ )
    {

      for ( z = 0; z < subSizeZ; z++ )
      {

        for ( y = 0; y < subSizeY; y++ )
        {

          for ( x = 0; x < subSizeX; x++ )
          {

            out( x, y, z, t ) = _volume( offsetX + x,
                                         offsetY + y,
                                         offsetZ + z,
                                         offsetT + t );

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::VolumeView< T >::extract( "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "gkg::Volume< T >& out ) const" );

}


#endif
