#ifndef _gkg_processing_algobase_CenterOfGravity_i_h_
#define _gkg_processing_algobase_CenterOfGravity_i_h_


#include <gkg-processing-algobase/CenterOfGravity.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-cppext/Limits.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::Vector3d< float >
gkg::CenterOfGravity< T >::compute( const gkg::Volume< T >& volume ) const
{

  try
  {

    // The center of gravity (or center of mass) of a volume is defined as the
    // average of the voxel positions, weighted by their intensities
    //    CenterOfGravity = Sum( V(x,y,z) * pos ) / Sum( V(x,y,z) )

    gkg::Vector3d< float > centerOfGravity;

    double val = 0, total = 0, vx = 0, vy = 0, vz = 0, subTotal = 0;

    // Find min value of volume
    typename gkg::Volume< T >::const_iterator i  = volume.begin(),
                                              ie = volume.end();

    T minValue = std::numeric_limits< T >::max();

    while ( i != ie )
    {

      minValue = ( *i < minValue ) ? *i : minValue;
      ++ i;

    }

    // Find center of gravity
    int64_t n = 0, nlim;
    nlim = ( int64_t ) std::sqrt( ( double ) volume.getSizeXYZ() );

    if ( nlim < 1000 )
    {

      nlim = 1000;

    }

    const int32_t sizeX = volume.getSizeX();
    const int32_t sizeY = volume.getSizeY();
    const int32_t sizeZ = volume.getSizeZ();

    for ( int32_t z = 0; z < sizeZ; z++ )
    {

      for ( int32_t y = 0; y < sizeY; y++ )
      {

        for ( int32_t x = 0; x < sizeX; x++ )
        {

          val = ( double )( volume( x, y, z ) - minValue );
          vx += val * x;
          vy += val * y;
          vz += val * z;
          subTotal += val;
          n++;

          // Add sub total regularly
          if ( n > nlim )
          {

            n = 0;
            total += subTotal;
            centerOfGravity.x += vx;
            centerOfGravity.y += vy;
            centerOfGravity.z += vz;
            subTotal = 0;
            vx = 0;
            vy = 0;
            vz = 0;

          }

        }

      }

    }


    total += subTotal;
    centerOfGravity.x += vx;
    centerOfGravity.y += vy;
    centerOfGravity.z += vz;

    if ( std::fabs( total ) < 1e-5 )
    {

      std::cerr << "WARNING::in calculating COG, total = 0.0" << std::endl;
      total = 1.0;

    }

    centerOfGravity.x /= total;
    centerOfGravity.y /= total;
    centerOfGravity.z /= total;

    // Convert position in mm
    gkg::Vector3d< double > resolution;
    volume.getResolution( resolution );

    centerOfGravity.x *= resolution.x;
    centerOfGravity.y *= resolution.y;
    centerOfGravity.z *= resolution.z;

    return centerOfGravity;

  }
  GKG_CATCH( "template < class T > "
             "gkg::Vector3d< float > "
             "gkg::CenterOfGravity< T >::compute( "
             "const gkg::Volume< T >& volume ) const" );

}

#endif
