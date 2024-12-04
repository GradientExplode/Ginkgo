#include <gkg-mri-reconstruction-correction/AhnPhaseCorrection3d.h>
#include <gkg-mri-reconstruction-correction/AhnPhaseCorrection.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


//#define DEBUG_REFERENCE_SCAN 1

#ifdef DEBUG_REFERENCE_SCAN

#include <fstream>

#endif


template < class T >
gkg::AhnPhaseCorrection3d< T >::AhnPhaseCorrection3d()
{
}


template < class T >
gkg::AhnPhaseCorrection3d< T >::~AhnPhaseCorrection3d()
{
}


template < class T >
void gkg::AhnPhaseCorrection3d< T >::estimate(
                       const gkg::Volume< std::complex< T > >& data,
                       int32_t rawLineCount,
                       int32_t rawSliceCount,
                       std::vector< std::vector< T > >& constantPhaseCorrection,
                       std::vector< std::vector< T > >& linearPhaseCorrection,
                       gkg::AhnPhaseCorrection3d< T >::Type type,
                       bool verbose,
                       const T& sideExclusionPercentage,
                       const T& magnitudeThresholdPercentage,
                       int32_t centerExcludedPointCount,
                       bool normalizeConstant,
                       bool normalizeLinear ) const
{

  try
  {

    int32_t sizeX = data.getSizeX();
    int32_t lineCount = data.getSizeY();

    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "input data has sizeT different from 1" );

    }

    // we allocate the result
    if ( ( int32_t )constantPhaseCorrection.size() != rawSliceCount )
    {

      constantPhaseCorrection = std::vector< std::vector< T > >( 
                                                                rawSliceCount );

    }
    if ( ( int32_t )linearPhaseCorrection.size() != rawSliceCount )
    {

      linearPhaseCorrection = std::vector< std::vector< T > >( rawSliceCount );

    }

    // looping over slice(s)
    if ( type == gkg::AhnPhaseCorrection3d< T >::ALL_SLICES )
    {

      gkg::Volume< std::complex< T > > sliceData( sizeX, lineCount );
      int32_t x, line, slice;
      for ( slice = 0; slice < rawSliceCount; slice++ )
      {

        // duplicating the current slice
        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            sliceData( x, line ) = data( x, line, slice );

          }

        }
        // processing the modulation transfer function for slice "slice"
        gkg::AhnPhaseCorrection< T >::getInstance().estimate(
                                               sliceData,
                                               rawLineCount,
                                               constantPhaseCorrection[ slice ],
                                               linearPhaseCorrection[ slice ],
                                               sideExclusionPercentage,
                                               magnitudeThresholdPercentage,
                                               centerExcludedPointCount,
                                               normalizeConstant,
                                               normalizeLinear );

        if ( verbose )
        {

          if ( slice != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                    << " / " << std::setw( 4 ) << rawSliceCount
                    << " ]" << std::flush;

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 ) << std::flush;

      }

    }
    else
    {


      int32_t x, line, slice;

      int32_t centralSlice = rawSliceCount / 2;

      // duplicating the central slice
      gkg::Volume< std::complex< T > > sliceData( sizeX, lineCount );
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          sliceData( x, line ) = data( x, line, centralSlice );

        }

      }

      // processing the modulation transfer function for central slice
      gkg::AhnPhaseCorrection< T >::getInstance().estimate(
                                        sliceData,
                                        rawLineCount,
                                        constantPhaseCorrection[ centralSlice ],
                                        linearPhaseCorrection[ centralSlice ],
                                        sideExclusionPercentage,
                                        magnitudeThresholdPercentage,
                                        centerExcludedPointCount,
                                        normalizeConstant,
                                        normalizeLinear );

      // propagating result to other slices
      for ( slice = 0; slice < rawSliceCount; slice++ )
      {

        if ( verbose )
        {

          if ( slice != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                    << " / " << std::setw( 4 ) << rawSliceCount
                    << " ]" << std::flush;

        }

        if ( slice != centralSlice )
        {

          constantPhaseCorrection[ slice ] =
            constantPhaseCorrection[ centralSlice ];
          linearPhaseCorrection[ slice ] =
            linearPhaseCorrection[ centralSlice ];

        }

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AhnPhaseCorrection3d< T >::estimate( "
             "const gkg::Volume< std::complex< T > >& data, "
             "int rawLineCount, "
             "int rawSliceCount, "
             "std::vector< std::vector< T > >& constantPhaseCorrection, "
             "std::vector< std::vector< T > >& linearPhaseCorrection, "
             "Type type,"
             "bool verbose,"
             "const T& sideExclusionPercentage, "
             "const T& magnitudeThresholdPercentage, "
             "int centerExcludedPointCount, "
             "bool normalizeConstant, "
             "bool normalizeLinear ) const" );

}


template < class T >
void gkg::AhnPhaseCorrection3d< T >::apply(
                 gkg::Volume< std::complex< T > >& data,
                 const std::vector< std::vector< T > >& constantPhaseCorrection,
                 const std::vector< std::vector< T > >& linearPhaseCorrection,
                 int32_t rawLineCount,
                 int32_t rawSliceCount,
                 bool verbose ) const
{

  try
  {

    int32_t sizeX = data.getSizeX();
    int32_t lineCount = data.getSizeY();
    int32_t sliceCount = data.getSizeZ();
    int32_t x, line, slice;

    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "T size should be equal to 1" );

    }

    if ( rawSliceCount > sliceCount )
    {

      throw std::runtime_error( "inconsistent raw slice count" );

    }

    if ( rawLineCount > lineCount )
    {

      throw std::runtime_error( "inconsistent raw line count" );

    }

    if ( ( int32_t )constantPhaseCorrection.size() != rawSliceCount )
    {

      throw std::runtime_error( "constant phase correction vector and"
                                "data slice count inconsistency" );

    }

    if ( ( int32_t )linearPhaseCorrection.size() != rawSliceCount )
    {

      throw std::runtime_error( "linear phase correction vector and"
                                "data slice count inconsistency" );

    }

    for ( slice = 0; slice < rawSliceCount; slice ++ )
    {

      if ( ( int32_t )constantPhaseCorrection[ slice ].size() != rawLineCount )
      {

        throw std::runtime_error( "constant phase correction vector and Y "
                                  "volume size inconsistency" );

      }
      if ( ( int32_t )linearPhaseCorrection[ slice ].size() != rawLineCount )
      {

        throw std::runtime_error( "linear phase correction vector and Y "
                                  "volume size inconsistency" );

      }

    }

    // correcting the whole volume
    for ( slice = 0; slice < rawSliceCount; slice++ )
    {

      for ( line = 0; line < rawLineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          data( x, line, slice ) *=
                        std::polar( ( T )1.0, ( T )( -sizeX / 2 + x ) *
                                     linearPhaseCorrection[ slice ][ line ] +
                                     constantPhaseCorrection[ slice ][ line ] );

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << rawSliceCount
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AhnPhaseCorrection3d< T >::apply( "
             "gkg::Volume< std::complex< T > >& data, "
             "std::vector< std::vector< T > >& constantPhaseCorrection, "
             "std::vector< std::vector< T > >& linearPhaseCorrection, "
             "int rawLineCount, int rawSliceCount, bool verbose ) const" );

}


template class gkg::AhnPhaseCorrection3d< float >;
template class gkg::AhnPhaseCorrection3d< double >;
