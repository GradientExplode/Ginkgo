#ifndef _gkg_processing_algobase_ConditionalMask_i_h_
#define _gkg_processing_algobase_ConditionalMask_i_h_


#include <gkg-processing-algobase/ConditionalMask.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


//
// class ConditionalMask< IN, MASK >
//

template < class IN, class MASK >
inline
void gkg::ConditionalMask< IN, MASK >::mask( 
                     IN& in,
                     const MASK& mask,
                     const gkg::TestFunction< MASK >& testFunction,
                     const gkg::FilterFunction< IN, IN >& filterFunction ) const
{

  if ( testFunction.getState( mask ) )
  {

    filterFunction.filter( in, in );

  }

}


//
// class ConditionalMask< std::vector< IN >, MASK >
//

template < class IN, class MASK >
inline
void gkg::ConditionalMask< std::vector< IN >, MASK >::mask( 
                     std::vector< IN >& in,
                     const std::vector< MASK >& mask,
                     const gkg::TestFunction< MASK >& testFunction,
                     const gkg::FilterFunction< IN, IN >& filterFunction ) const
{

  try
  {

    if ( in.size() != mask.size() )
    {

      throw std::runtime_error( "incompatible input vector and mask vector" );

    }

    typename std::vector< IN >::iterator i = in.begin(), ie = in.end();
    typename std::vector< MASK >::const_iterator m = mask.begin();
    while ( i != ie )
    {

      if ( testFunction.getState( *m ) )
      {

        filterFunction.filter( *i, *i );

      }
      ++ i;
      ++ m;

    }

  }
  GKG_CATCH( "template < class IN, class MASK > "
             "inline "
             "void gkg::ConditionalMask< std::vector< IN >, MASK >::mask( "
             "std::vector< IN >& in, "
             "const std::vector< MASK >& mask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "const gkg::FilterFunction< IN, IN >& filterFunction ) const" );

}


//
// class ConditionalMask< Volume< IN >, MASK >
//

template < class IN, class MASK >
inline
void gkg::ConditionalMask< gkg::Volume< IN >, MASK >::mask( 
                     gkg::Volume< IN >& in,
                     const gkg::Volume< MASK >& mask,
                     const gkg::TestFunction< MASK >& testFunction,
                     const gkg::FilterFunction< IN, IN >& filterFunction ) const
{

  try
  {

    int32_t sizeX = in.getSizeX();
    int32_t sizeY = in.getSizeY();
    int32_t sizeZ = in.getSizeZ();
    int32_t sizeT = in.getSizeT();

    // sanity checks
    if ( ( mask.getSizeX() != sizeX ) || 
         ( mask.getSizeY() != sizeY ) || 
         ( mask.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error( "inconsistent input and mask size(s)" );

    }

    if ( ( mask.getSizeT() != sizeT ) && 
         ( mask.getSizeT() != 1 ) )
    {

      throw std::runtime_error(
        "mask size should have T size equal to 1 or equal to input T size" );

    }

    int32_t x, y, z, t, maskT;

    for ( t = 0; t < sizeT; t++ )
    {

      if ( mask.getSizeT() == 1 )
      {

        maskT = 0;

      }
      else
      {

        maskT = t;

      }

      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( testFunction.getState( mask( x, y, z, maskT ) ) )
            {

              filterFunction.filter( in( x, y, z, t ), in( x, y, z, t ) );

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class IN, class MASK > "
             "inline "
             "void gkg::ConditionalMask< gkg::Volume< IN >, MASK >::mask( "
             "gkg::Volume< IN >& in, "
             "const gkg::Volume< MASK >& mask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "const gkg::FilterFunction< IN, IN >& filterFunction ) const" );

}


#endif

