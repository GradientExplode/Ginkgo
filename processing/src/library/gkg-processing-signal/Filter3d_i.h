#ifndef _gkg_processing_signal_Filter3d_i_h_
#define _gkg_processing_signal_Filter3d_i_h_


#include <gkg-processing-signal/Filter3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <iomanip>



template < class IN, class MASK, class OUT >
inline
void gkg::Filter3d< IN, MASK, OUT >::filter(
            const gkg::Volume< IN >& in,
            const gkg::Volume< MASK >& mask,
            const gkg::TestFunction< MASK >& testFunction,
            const gkg::FilterFunction< std::vector< IN >, OUT >& filterFunction,
            gkg::Neighborhood3d::Type neighborhood3dType,
            gkg::Volume< OUT >& out,
            bool verbose ) const
{

  try
  {

    // sanity checks

    if ( mask.getSizeT() != 1 )
    {

      throw std::runtime_error( "mask should have T size equal to 1" );

    }

    int32_t sizeX = in.getSizeX();
    int32_t sizeY = in.getSizeY();
    int32_t sizeZ = in.getSizeZ();
    int32_t sizeT = in.getSizeT();
    if ( ( mask.getSizeX() != sizeX ) ||
         ( mask.getSizeY() != sizeY ) ||
         ( mask.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error( "inconsistent mask and input sizes" );

    }

    // allocating stuff
    gkg::Volume< OUT > tmpOut( sizeX, sizeY, sizeZ, sizeT );
    tmpOut.getHeader() = in.getHeader();
    tmpOut.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    // preparing neighbor list
    std::list< gkg::Vector3d< int32_t > >
      offsets = Neighborhood3d::getInstance().getNeighborOffsets(
                                                           neighborhood3dType );
    std::vector< IN > values;
    values.reserve( offsets.size() );

    // filtering
    int32_t x, y, z, t;

    gkg::Converter< IN, OUT > converter;
    gkg::BoundingBox< int32_t > box( 0, sizeX - 1,
                                     0, sizeY - 1,
                                     0, sizeZ - 1 );
                                 

    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose )
      {


        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " rank[ " << std::setw( 4 ) << t + 1 
                  << " / " << std::setw( 4 ) << sizeT
                  << " ]" << std::flush;

      }

      for ( z = 0; z < sizeZ; z++ )
      {

        if ( verbose )
        {


          if ( z != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << z + 1 
                    << " / " << std::setw( 4 ) << sizeZ
                    << " ]" << std::flush;

        }

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( testFunction.getState( mask( x, y, z ) ) )
            {

              values.clear();
              values.push_back( in( x, y, z, t ) );

              std::list< gkg::Vector3d< int32_t > >::const_iterator 
                o = offsets.begin(),
                oe = offsets.end();
              while ( o != oe )
              {

                gkg::Vector3d< int32_t > neighbor( x + o->x,
                                                   y + o->y,
                                                   z + o->z );
                if ( box.contains( neighbor.x, neighbor.y, neighbor.z ) )
                {

                  if ( mask( neighbor.x, neighbor.y, neighbor.z ) )
                  {

                    values.push_back( in( neighbor.x,
                                          neighbor.y,
                                          neighbor.z,
                                          t ) );

                  }

                }
                ++ o;

              }
              filterFunction.filter( values, tmpOut( x, y, z, t ) );

            }
            else
            {

              converter.convert( in( x, y, z, t ), tmpOut( x, y, z, t ) );

            }

          }

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 );

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << std::flush;

    }

    // copying back filtered volume
    out = tmpOut;

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "inline "
             "void gkg::Filter3d< IN, MASK, OUT >::filter( "
             "const gkg::Volume< IN >& in, "
             "const gkg::Volume< MASK >& mask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "const gkg::FilterFunction< IN, OUT >& filterFunction, "
             "gkg::Neighborhood3d::Type neighborhood3dType, "
             "gkg::Volume< OUT >& out, "
             "bool verbose = false ) const" );

}


#endif
