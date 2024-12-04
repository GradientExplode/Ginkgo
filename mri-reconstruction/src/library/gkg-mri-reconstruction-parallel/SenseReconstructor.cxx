#include <gkg-mri-reconstruction-parallel/SenseReconstructor.h>
#include <gkg-processing-numericalanalysis/IllMatrixInverser.h>
#include <gkg-processing-numericalanalysis/CNormWeighting.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/CVector.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-processing-resampling/Spreader.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


template < class T >
gkg::SenseReconstructor< T >::SenseReconstructor()
                             : _quickResampler( 0 ),
                               _spreader( 0 ),
                               _illMatrixInverser( 0 ),
                               _cNormWeighting( 0 )
{
}


template < class T >
gkg::SenseReconstructor< T >::~SenseReconstructor()
{
}


template < class T >
void gkg::SenseReconstructor< T >::setB1SensitivityMap(
                      const gkg::Volume< std::complex< T > >& b1SensitivityMap )
{

  try
  {

    _b1SensitivityMap = b1SensitivityMap;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::setB1SensitivityMap( "
             "const Volume< std:complex< T > >& b1SensitivityMap )" );

}


template < class T >
void gkg::SenseReconstructor< T >::setQuickResampler(
                const gkg::QuickResampler< std::complex< T > >& quickResampler )
{

  try
  {

    _quickResampler = &quickResampler;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::setQuickResampler( "
             "const gkg::QuickResampler< std::complex< T > >& "
             "quickResampler )" );

}


template < class T >
void gkg::SenseReconstructor< T >::setSpreader(
                            const gkg::Spreader< std::complex< T > >& spreader )
{

  try
  {

    _spreader = &spreader;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::setSpreader( "
             "const gkg::Spreader< std::complex< T > >& spreader )" );

}


template < class T >
void gkg::SenseReconstructor< T >::setIllMatrixInverser(
                               const gkg::IllMatrixInverser& illMatrixInverser )
{

  try
  {

    _illMatrixInverser = &illMatrixInverser;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::setIllMatrixInverser( "
             "const gkg::IllMatrixInverser& illMatrixInverser )" );

}


template < class T >
void gkg::SenseReconstructor< T >::setCNormWeighting(
                                     const gkg::CNormWeighting& cNormWeighting )
{

  try
  {

    _cNormWeighting = &cNormWeighting;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::setCNormWeighting( "
             "const gkg::CNormWeighting& cNormWeighting )" );

}


template < class T >
void gkg::SenseReconstructor< T >::reconstruct(
                                const gkg::Volume< std::complex< T > >& raw,
                                gkg::Volume< std::complex< T > >& reconstructed,
                                bool verbose ) const
{

  try
  {

    // sanity check
    if ( reconstructed.getSizeT() != 1 )
    {

      throw std::runtime_error(
                      "T size of the reconstructed output must be equal to 1" );

    }
    if ( !_quickResampler )
    {

      throw std::runtime_error( "not a valid quick resampler" );

    }
    if ( !_spreader )
    {

      throw std::runtime_error( "not a valid spreader" );

    }
    if ( !_illMatrixInverser )
    {

      throw std::runtime_error( "not a valid ill matrix inverser" );

    }

    int32_t rawSizeX = raw.getSizeX();
    int32_t rawSizeY = raw.getSizeY();
    int32_t rawSizeZ = raw.getSizeZ();
    int32_t coilCount = raw.getSizeT();

    int32_t reconstructedSizeX = reconstructed.getSizeX();
    int32_t reconstructedSizeY = reconstructed.getSizeY();
    int32_t reconstructedSizeZ = reconstructed.getSizeZ();

    gkg::Vector3d< double > outResolution( 1.0, 1.0, 1.0 );
/*
    if ( reconstructed.getHeader().hasAttribute( "resolutionX" ) )
    {

      reconstructed.getHeader().getAttribute( "resolutionX",
                                              outResolution.x );

    }
    if ( reconstructed.getHeader().hasAttribute( "resolutionY" ) )
    {

      reconstructed.getHeader().getAttribute( "resolutionY",
                                              outResolution.y );

    }
    if ( reconstructed.getHeader().hasAttribute( "resolutionZ" ) )
    {

      reconstructed.getHeader().getAttribute( "resolutionZ",
                                              outResolution.z );

    }
*/
    // clearing unfolded volume
    reconstructed.fill( std::complex< T >( 0.0, 0.0 ) );

    int32_t x, y, z, c, ux, uy, uz, index;


    std::vector< T > unfoldedXPositions;
    std::vector< T > unfoldedYPositions;
    std::vector< T > unfoldedZPositions;

    int32_t unfoldedXPositionCount =
      this->ParallelReconstructor< T >::_xReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedYPositionCount =
      this->ParallelReconstructor< T >::_yReductionStrategy.
                                                     getUnfoldedPositionCount();
    int32_t unfoldedZPositionCount =
      this->ParallelReconstructor< T >::_zReductionStrategy.
                                                     getUnfoldedPositionCount();

    gkg::CMatrix S( coilCount,
                    unfoldedXPositionCount *
                    unfoldedYPositionCount * 
                    unfoldedZPositionCount );
    gkg::CVector folded( coilCount );
    gkg::CVector unfolded( unfoldedXPositionCount *
                           unfoldedYPositionCount * 
                           unfoldedZPositionCount );
    gkg::Vector3d< double > outLocation;
    std::complex< T > value;

    for ( z = 0; z < rawSizeZ; z++ )
    {

      if ( verbose )
      {


        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << z + 1
                  << " / " << std::setw( 4 ) << rawSizeZ
                  << " ]" << std::flush;

      }

      this->ParallelReconstructor< T >::_zReductionStrategy.
                          getUnfoldedPositions( ( T )z,
                                                reconstructedSizeZ,
                                                unfoldedZPositions );

      for ( y = 0; y < rawSizeY; y++ )
      {

        this->ParallelReconstructor< T >::_yReductionStrategy.
                            getUnfoldedPositions( ( float )y,
                                                  reconstructedSizeY,
                                                  unfoldedYPositions );

        for ( x = 0; x < rawSizeX; x++ )
        {

          this->ParallelReconstructor< T >::_xReductionStrategy.
                             getUnfoldedPositions( ( float )x,
                                                    reconstructedSizeX,
                                                    unfoldedXPositions );

          index = 0;
          for ( uz = 0; uz < unfoldedZPositionCount; uz++ )
          {

            for ( uy = 0; uy < unfoldedYPositionCount; uy++ )
            {

              for ( ux = 0; ux < unfoldedXPositionCount; ux++ )
              {

                outLocation.x = ( double )unfoldedXPositions[ ux ];
                outLocation.y = ( double )unfoldedYPositions[ uy ];
                outLocation.z = ( double )unfoldedZPositions[ uz ];

                for ( c = 0; c < coilCount; c++ )
                {

                  _quickResampler->resample( _b1SensitivityMap,
                                             std::complex< T >( 0.0, 0.0 ),
                                             outLocation,
                                             value,
                                             &outResolution,
                                             c );
                  S( c, index ) = value;
                  folded( c ) = raw( x, y, z, c );

                }
                ++ index;

              }

            }

          }

          if ( _cNormWeighting )
          {

            S = _cNormWeighting->getWeighting().getComposition( S );
            folded = _cNormWeighting->getWeighting().getComposition( folded );

          }
          _illMatrixInverser->inverse( S, folded, unfolded );

          index = 0;
          for ( uz = 0; uz < unfoldedZPositionCount; uz++ )
          {

            for ( uy = 0; uy < unfoldedYPositionCount; uy++ )
            {

              for ( ux = 0; ux < unfoldedXPositionCount; ux++ )
              {

                outLocation.x = ( double )unfoldedXPositions[ ux ];
                outLocation.y = ( double )unfoldedYPositions[ uy ];
                outLocation.z = ( double )unfoldedZPositions[ uz ];
                value = std::complex< T >( unfolded( index ) );

                if ( std::abs( _b1SensitivityMap( ( int32_t )outLocation.x,
                                                  ( int32_t )outLocation.y,
                                                  ( int32_t )outLocation.z ) )
                     != 0 )
                {

                  _spreader->spread( value,
                                     outLocation,
                                     reconstructed,
                                     &outResolution );

                }
                ++ index;

              }

            }

          }

        }

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::SenseReconstructor< T >::reconstruct( "
             "const gkg::Volume< std::complex< T > >& raw, "
             "gkg::Volume< std::complex< T > >& reconstructed, "
             "bool verbose ) const" );

}


template class gkg::SenseReconstructor< float >;
template class gkg::SenseReconstructor< double >;
