#ifndef _gkg_processing_extrapolation_RecursiveExtrapolator3d_i_h_
#define _gkg_processing_extrapolation_RecursiveExtrapolator3d_i_h_


#include <gkg-processing-extrapolation/RecursiveExtrapolator3d.h>
#include <gkg-processing-extrapolation/Extrapolator_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/FilterFunction_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <iomanip>
#include <list>


template < class IN, class MASK, class OUT >
inline
gkg::RecursiveExtrapolator3d< IN, MASK, OUT >::RecursiveExtrapolator3d(
            const gkg::FilterFunction< std::vector< IN >, OUT >& filterFunction,
            gkg::Neighborhood3d::Type neighborhood3dType )
                                     : gkg::Extrapolator< IN, MASK, OUT >(),
                                       _filterFunction( filterFunction ),
                                       _neighborhood3dType( neighborhood3dType )
{
}


template < class IN, class MASK, class OUT >
inline
gkg::RecursiveExtrapolator3d< IN, MASK, OUT >::~RecursiveExtrapolator3d()
{
}


template < class IN, class MASK, class OUT >
inline
void gkg::RecursiveExtrapolator3d< IN, MASK, OUT >::extrapolate(
                                  const gkg::Volume< IN >& input,
                                  const gkg::Volume< MASK >& restrictedMask,
                                  const gkg::Volume< MASK >& enlargedMask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  gkg::Volume< OUT >& output,
                                  bool verbose ) const
{

  try
  {

    int32_t sizeX = input.getSizeX();
    int32_t sizeY = input.getSizeY();
    int32_t sizeZ = input.getSizeZ();
    int32_t sizeT = input.getSizeT();

    // sanity checks
    if ( ( restrictedMask.getSizeX() != sizeX ) || 
         ( restrictedMask.getSizeY() != sizeY ) || 
         ( restrictedMask.getSizeZ() != sizeZ ) ||
         ( enlargedMask.getSizeX() != sizeX ) || 
         ( enlargedMask.getSizeY() != sizeY ) || 
         ( enlargedMask.getSizeZ() != sizeZ ) )
    {

      throw std::runtime_error( "inconsistent input and mask size(s)" );

    }

    if ( ( ( restrictedMask.getSizeT() != sizeT ) && 
           ( restrictedMask.getSizeT() != 1 ) ) ||
         ( ( enlargedMask.getSizeT() != sizeT ) && 
           ( enlargedMask.getSizeT() != 1 ) ) )
    {

      throw std::runtime_error(
        "mask size should have T size equal to 1 or equal to input T size" );

    }

    // duplicating input in case output reference is equal to input reference
    gkg::Volume< IN > tmpInput( input );

    // copying input to output
    gkg::Volume< OUT > tmpOutput( sizeX, sizeY, sizeZ, sizeT );
    tmpOutput.getHeader() = tmpInput.getHeader();
    tmpOutput.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    // preparing neighbor offsets
    std::list< gkg::Vector3d< int32_t > > offsets =
      gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                                          _neighborhood3dType );
    std::vector< IN > values;
    values.reserve( offsets.size() );

    gkg::Volume< uint8_t > newRestrictedMask( sizeX, sizeY, sizeZ );
    int32_t x, y, z, t, maskT;
    gkg::BoundingBox< int32_t > box( 0, sizeX - 1,
                                 0, sizeY - 1,
                                 0, sizeZ - 1 );
    gkg::Converter< IN, OUT > converter;
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

      if ( restrictedMask.getSizeT() == 1 )
      {

        maskT = 0;

      }
      else
      {

        maskT = t;

      }

      // building hole point list and mask, and filling temporary output
      // values for non hole points
      std::list< gkg::Vector3d< int32_t > > holePoints;
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( !testFunction.getState( restrictedMask( x, y, z, maskT ) ) &&
                 testFunction.getState( enlargedMask( x, y, z, maskT ) ) )
            {

              holePoints.push_back( gkg::Vector3d< int32_t >( x, y, z ) );

            }
            else
            {

              converter.convert( tmpInput( x, y, z, t ),
                                 tmpOutput( x, y, z, t ) ); 

            }
           

          }

        }

      }

      // copying restricted mask
      for ( z = 0; z < sizeZ; z++ )
      {

        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            newRestrictedMask( x, y, z ) = restrictedMask( x, y, z, maskT );

          }

        }

      }

      // filling holes
      while ( !holePoints.empty() )
      {

        // updating front of the restricted mask
        std::list< gkg::Vector3d< int32_t > > keptPoints;
        std::list< gkg::Vector3d< int32_t > > removedPoints;       
        std::list< gkg::Vector3d< int32_t > >::const_iterator
          h = holePoints.begin(),
          he = holePoints.end();
        while ( h != he )
        {

          gkg::Vector3d< int32_t > site = *h;
          std::list< gkg::Vector3d< int32_t > >::const_iterator 
	                                               o = offsets.begin(),
                                                       oe = offsets.end();
          values.clear();
          while ( o != oe )
          {

            gkg::Vector3d< int32_t > neighbor( site );
            neighbor += *o;
            if ( box.contains( neighbor.x, neighbor.y, neighbor.z ) )
            {

              if ( testFunction.getState( newRestrictedMask( neighbor.x,
                                                             neighbor.y,
                                                             neighbor.z ) ) )
              {

                values.push_back( tmpOutput( neighbor.x,
                                             neighbor.y,
                                             neighbor.z,
                                             t ) );

              }

            }
            ++ o;

          }
          if ( !values.empty() )
          {

            _filterFunction.filter( values,
                                    tmpOutput( site.x, site.y, site.z, t ) );
            removedPoints.push_back( site );

          }
          else
          {

            keptPoints.push_back( *h );

          }

          ++ h;

        }

        // updating restricted mask
        std::list< gkg::Vector3d< int32_t > >::const_iterator
          r = removedPoints.begin(),
          re = removedPoints.end();
        while ( r != re )
        {

          newRestrictedMask( r->x, r->y, r->z ) = 1U;
          ++ r;

        }

        // updating hole point list
        if ( keptPoints.size() == holePoints.size() )
        {

          // no points belongs to the boundary, so they are isolated points;
          // replacing by its original value
          std::list< gkg::Vector3d< int32_t > >::const_iterator
            k = keptPoints.begin(),
            ke = keptPoints.end();
          while ( k != ke )
          {

            converter.convert( tmpInput( k->x, k->y, k->z, t ),
                               tmpOutput( k->x, k->y, k->z, t ) );
            ++ k;

          }
          holePoints.clear();

        }
        else
        {

          // replacing hole points with kept points
          holePoints = keptPoints;

        }

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << std::flush;

    }
    output = tmpOutput;

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "void gkg::RecursiveExtrapolator3d< IN, MASK, OUT >::extrapolate( "
             "const gkg::Volume< IN >& input, "
             "const gkg::Volume< MASK >& restrictedMask, "
             "const gkg::Volume< MASK >& enlargedMask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "gkg::Volume< OUT >& ouput, "
             "bool verbose ) const" );

}


#endif
