#ifndef _gkg_processing_extrapolation_Extrapolator2d_i_h_
#define _gkg_processing_extrapolation_Extrapolator2d_i_h_


#include <gkg-processing-extrapolation/Extrapolator2d.h>
#include <gkg-processing-extrapolation/Extrapolator_i.h>
#include <gkg-processing-extrapolation/ExtrapolatorFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <cmath>
#include <numeric>
#include <iomanip>


template < class IN, class MASK, class OUT >
inline
gkg::Extrapolator2d< IN, MASK, OUT >::Extrapolator2d(
                         int32_t degree,
                         const gkg::ExtrapolatorFunction& extrapolatorFunction )
                                : gkg::Extrapolator< IN, MASK, OUT >(),
                                  _degree( degree ),
                                  _extrapolatorFunction( extrapolatorFunction )
{
}


template < class IN, class MASK, class OUT >
inline
gkg::Extrapolator2d< IN, MASK, OUT >::~Extrapolator2d()
{
}


template < class IN, class MASK, class OUT >
inline
void gkg::Extrapolator2d< IN, MASK, OUT >::extrapolate(
                                  const gkg::Volume< IN >& input,
                                  const gkg::Volume< MASK >& restrictedMask,
                                  const gkg::Volume< MASK >& enlargedMask,
                                  const gkg::TestFunction< MASK >& testFunction,
                                  gkg::Volume< OUT >& output,
                                  bool verbose ) const
{

  try
  {

    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    int32_t sizeX = input.getSizeX();
    int32_t sizeY = input.getSizeY();
    int32_t sizeZ = input.getSizeZ();
    int32_t sizeT = input.getSizeT();

    // sanity checks
    if ( ( restrictedMask.getSizeX() != sizeX ) || 
         ( restrictedMask.getSizeY() != sizeY ) || 
         ( restrictedMask.getSizeZ() != sizeZ ) || 
         ( restrictedMask.getSizeT() != sizeT ) ||
         ( enlargedMask.getSizeX() != sizeX ) || 
         ( enlargedMask.getSizeY() != sizeY ) || 
         ( enlargedMask.getSizeZ() != sizeZ ) || 
         ( enlargedMask.getSizeT() != sizeT ) )
    {

      throw std::runtime_error( "inconsistent input and mask size(s)" );

    }

    // duplicating input in case output reference is equal to input reference
    gkg::Volume< IN > tmpInput( input );

    // allocating output
    output = gkg::Volume< OUT >( sizeX, sizeY, sizeZ, sizeT );
    output.getHeader() = input.getHeader();
    output.getHeader()[ "item_type" ] = gkg::TypeOf< OUT >::getName();

    // generating basis vectors
    if ( verbose )
    {

      std::cout << "generating basis" << std::flush;

    }
    gkg::Volume< float > basis( sizeX, sizeY, _degree );

    int32_t d, x, y, n, q, r;
    for ( d = 0; d < _degree; d++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          n = ( int32_t )std::sqrt( ( double )_degree );
          q = d / n;
          r = d - q * n;
          basis( x, y, d ) = _extrapolatorFunction.getValue( y, q, sizeY ) *
                             _extrapolatorFunction.getValue( x, r, sizeX );

        }   

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 16 ) << std::flush;

    }

    // calculating dot products
    if ( verbose )
    {

      std::cout << "calculating dot products" << std::flush;

    }

    gkg::Vector beta( _degree );
    gkg::Matrix covar( _degree, _degree );

    gkg::Matrix U( _degree, _degree );
    gkg::Vector S( _degree );
    gkg::Matrix V( _degree, _degree );

    gkg::Vector alpha( _degree );

    int32_t d1, d2, z, t;
    double yModel, weight, outputValue;
    gkg::Converter< double, OUT > converter;
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

        // calculating covariance matrix
        covar.setZero();
        beta.setZero();
        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( testFunction.getState( restrictedMask( x, y, z, t ) ) )
            {

              yModel = tmpInput( x, y, z, t );
              for ( d1 = 0; d1 < _degree; d1++ )
              {

                weight = basis( x, y, d1 );
                for ( d2 = 0; d2 <= d1; d2++ )
                {

                  covar( d1, d2 ) += weight * basis( x, y, d2 );

                }
                beta( d1 ) += yModel * weight;

              }

            }

          }

        }


        for ( d2 = 1; d2 < _degree; d2++ )
        {

          for ( d1 = 0; d1 < d2; d1++ )
          {

            covar( d1, d2 ) = covar( d2, d1 );

          }

        }

        // resolving linear system:  covar * alpha = beta
        U = covar;
        factory->getSingularValueDecomposition( U, V, S );
        factory->getSingularValueSolution( U, S, V, beta, alpha );

        // applying result to get extrapolated output
        for ( y = 0; y < sizeY; y++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            if ( testFunction.getState( enlargedMask( x, y, z, t ) ) )
            {

              outputValue = 0.0;
              for ( d = 0; d < _degree; d++ )
              {

                outputValue += alpha( d ) * basis( x, y, d );

              }
              converter.convert( outputValue, output( x, y, z, t ) );

            }
            else
            {

              output( x, y, z, t ) = ( OUT )0;

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

      std::cout << gkg::Eraser( 44 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class IN, class MASK, class OUT > "
             "void gkg::Extrapolator2d< IN, MASK, OUT >::extrapolate( "
             "const gkg::Volume< IN >& input, "
             "const gkg::Volume< MASK >& restrictedMask, "
             "const gkg::Volume< MASK >& enlargedMask, "
             "const gkg::TestFunction< MASK >& testFunction, "
             "gkg::Volume< OUT >& ouput, "
             "bool verbose ) const" );

}


#endif
