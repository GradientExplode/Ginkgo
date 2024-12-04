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
gkg::AhnPhaseCorrection< T >::AhnPhaseCorrection()
{
}


template < class T >
gkg::AhnPhaseCorrection< T >::~AhnPhaseCorrection()
{
}


template < class T >
void gkg::AhnPhaseCorrection< T >::estimate(
                                   const gkg::Volume< std::complex< T > >& data,
                                   int32_t rawLineCount,
                                   std::vector< T >& constantPhaseCorrection,
                                   std::vector< T >& linearPhaseCorrection,
                                   const T& sideExclusionPercentage,
                                   const T& magnitudeThresholdPercentage,
                                   int32_t centerExcludedPointCount,
                                   bool normalizeConstant,
                                   bool normalizeLinear ) const
{

  try
  {

    int32_t sizeX = data.getSizeX();
    int32_t x, line;

    // sanity checks
    if ( ( data.getSizeZ() != 1 ) || ( data.getSizeT() != 1 ) )
    {

      throw std::runtime_error(
                         "input data has sizeZ and/or sizeT different from 1" );

    }

    if ( ( int32_t )constantPhaseCorrection.size() != rawLineCount )
    {

      constantPhaseCorrection = std::vector< T >( rawLineCount );

    }

    if ( ( int32_t )linearPhaseCorrection.size() != rawLineCount )
    {

      linearPhaseCorrection = std::vector< T >( rawLineCount );

    }

    // building exclusion mask
    int32_t exclusionCount = std::max( ( int32_t )( sizeX * ( 1.0 -
                                                    sideExclusionPercentage / 
                                                    100.0 ) ),
                                       2 );

    std::vector< bool > mask( sizeX );

    for ( x = 0; x < sizeX; x++ )
    {

      mask[ x ] = true;

    }
    // exclude points at boundaries
    for ( x = 0; x < exclusionCount; x++ )
    {

      mask[ x ] = mask[ sizeX - 1 - x ] = false;

    }
    // exclude points at center
    for ( x = sizeX / 2 - centerExcludedPointCount / 2;
          x < sizeX / 2 + centerExcludedPointCount / 2; x++ )
    {

      mask[ x ] = false;

    }

    // we do a copy because Ahn algo modifies input data
    gkg::Volume< std::complex< T > > dataCopy( data );

    // applying Fourier transform along X_AXIS
    gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                               dataCopy, gkg::FourierTransform3d< T >::X_AXIS );

    // allocating vector for working
    std::vector< std::complex< T > > dataLine( sizeX );
    std::vector< T > magnitudeLine( sizeX );
    std::vector< std::complex< T > >  phaseDifference( sizeX - 1 );

    T magnitude;
    T magnitudePeak = 0.0;
    T magnitudeThreshold = 0.0;

    std::complex< T > nul( 0.0, 0.0 );

    for ( line = 0; line < rawLineCount; line++ )
    {

      ////////////////////////// linear phase correction ///////////////////////

      // finding peak of magnitude through masked line
      for ( x = 0; x < sizeX; x++ )
      {

        magnitudeLine[ x ] = std::sqrt( std::norm( dataCopy( x, line ) ) );
        if ( mask[ x ] )
        {

          if ( magnitudeLine[ x ] > magnitudePeak )
          {

            magnitudePeak = magnitudeLine[ x ];

          }

        }

      }

      // calculating magnitude threshold
      magnitudeThreshold = magnitudePeak * magnitudeThresholdPercentage / 100.0;

      // thresholding line
      for ( x = 0; x < sizeX; x++ )
      {

        if ( magnitudeLine[ x ] > magnitudeThreshold )
        {

          dataLine[ x ] = dataCopy( x, line );

        }
        else
        {

          dataLine[ x ] = nul;

        }

      }

#ifdef DEBUG_REFERENCE_SCAN

      if ( line == 63 )
      {

         std::ofstream os( "phase.txt" );
        for ( x = 0; x < sizeX; x++ )
        {

          os << std::arg( dataLine[ x ] ) << std::endl;

        }
        os.close();

      }

#endif

      // computing phase difference
      for ( x = 0; x < sizeX - 1; x++ )
      {

        phaseDifference[ x ] = dataLine[ x + 1 ] * std::conj( dataLine[ x ] );

      }

#ifdef DEBUG_REFERENCE_SCAN

      if ( line == 63 )
      {

         std::ofstream os( "phase_derivative.txt" );
        for ( x = 0; x < sizeX - 1; x++ )
        {

          os << std::arg( phaseDifference[ x ] ) << std::endl;

        }
        os.close();

      }

#endif

      // normalizing if required
      if ( normalizeLinear )
      {

        for ( x = 0; x < sizeX - 1; x++ )
        {

          magnitude = std::sqrt( std::norm( phaseDifference[ x ] ) );
          if ( magnitude > 1e-7 )
          {

            phaseDifference[ x ] /= magnitude;

          }

        }

      }

      // taking average phase difference value
      std::complex< T > averagePhaseDifference( 0.0, 0.0 );
      for ( x = 0; x < sizeX - 1; x++ )
      {

        averagePhaseDifference += phaseDifference[ x ];

      }
      averagePhaseDifference /= ( T )( sizeX - 1 );

      // calculating linear correction for current line
      linearPhaseCorrection[ line ] = - std::arg( averagePhaseDifference );

      ///////////////////////// constant phase correction //////////////////////

      // applying Ahn linear phase correction before constant coefficient can be
      // computed
      for ( x = 0; x < sizeX; x++ )
      {

        if ( magnitudeLine[ x ] > magnitudeThreshold )
        {

          dataLine[ x ] = dataLine[ x ] * 
                          std::polar( ( T )1.0, ( T )( -sizeX / 2 + x ) *
                                                linearPhaseCorrection[ line ] );

        }
        else
        {

          dataLine[ x ] = nul;

        }

      }

#ifdef DEBUG_REFERENCE_SCAN

      if ( line == 63 )
      {

        std::ofstream os( "phase_linear_corrected.txt" );
        for ( x = 0; x < sizeX; x++ )
        {

          os << std::arg( dataLine[ x ] ) << std::endl;

        }
        os.close();

      }

#endif

      // normalizing if required
      if ( normalizeConstant )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          magnitude = std::sqrt( std::norm( dataLine[ x ] ) );
          if ( magnitude > 1e-7 )
          {

            dataLine[ x ] /= magnitude;

          }

        }

      }

      // taking average phase difference value
      std::complex< T > averagePhase( 0.0, 0.0 );
      for ( x = 0; x < sizeX - 1; x++ )
      {

        averagePhase += dataLine[ x ];

      }
      averagePhase /= ( T )( sizeX - 1 );

      // calculating constant correction for current line
      constantPhaseCorrection[ line ] = - std::arg( averagePhase );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::AhnPhaseCorrection< T >::estimate( "
             "gkg::Volume< std::complex< T > >& data, "
             "int rawLineCount, "
             "std::vector< T >& constantPhaseCorrection, "
             "std::vector< T >& linearPhaseCorrection,   "
             "const T& sideExclusionPercentage, "
             "const T& magnitudeThresholdPercentage, "
             "int centerExcludedPointCount, "
             "bool normalizeConstant, "
             "bool normalizeLinear ) const " );

}


template < class T >
void gkg::AhnPhaseCorrection< T >::estimate(
                       const gkg::Volume< std::complex< T > >& data,
                       int32_t rawLineCount,
                       std::vector< std::vector< T > >& constantPhaseCorrection,
                       std::vector< std::vector< T > >& linearPhaseCorrection,
                       Type type,
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
    int32_t sliceCount = data.getSizeZ();

    // sanity checks
    if ( data.getSizeT() != 1 )
    {

      throw std::runtime_error( "input data has sizeT different from 1" );

    }

    // we allocate the result
    if ( ( int32_t )constantPhaseCorrection.size() != sliceCount )
    {

      constantPhaseCorrection = std::vector< std::vector< T > >( sliceCount );

    }
    if ( ( int32_t )linearPhaseCorrection.size() != sliceCount )
    {

      linearPhaseCorrection = std::vector< std::vector< T > >( sliceCount );

    }

    // looping over slice(s)
    if ( type == gkg::AhnPhaseCorrection< T >::ALL_SLICES )
    {

      gkg::Volume< std::complex< T > > sliceData( sizeX, lineCount );
      int32_t x, line, slice;
      for ( slice = 0; slice < sliceCount; slice++ )
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
                    << " / " << std::setw( 4 ) << sliceCount
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

      int32_t centralSlice = sliceCount / 2;

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
      for ( slice = 0; slice < sliceCount; slice++ )
      {

        if ( verbose )
        {

          if ( slice != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                    << " / " << std::setw( 4 ) << sliceCount
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
             "void gkg::ModulationTransferFunction< T >::estimate( "
             "const gkg::Volume< std::complex< T > >& data, "
             "int rawLineCount, "
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
void gkg::AhnPhaseCorrection< T >::apply(
                 gkg::Volume< std::complex< T > >& data,
                 const std::vector< std::vector< T > >& constantPhaseCorrection,
                 const std::vector< std::vector< T > >& linearPhaseCorrection,
                 int32_t rawLineCount,
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

    if ( rawLineCount > lineCount )
    {

      throw std::runtime_error( "inconsistent raw line count" );

    }

    if ( ( int32_t )constantPhaseCorrection.size() != sliceCount )
    {

      throw std::runtime_error( "constant phase correction vector and"
                                "data slice count inconsistency" );

    }

    if ( ( int32_t )linearPhaseCorrection.size() != sliceCount )
    {

      throw std::runtime_error( "linear phase correction vector and"
                                "data slice count inconsistency" );

    }

    for ( slice = 0; slice < sliceCount; slice ++ )
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
    for ( slice = 0; slice < sliceCount; slice++ )
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
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::Volume< std::complex< T > >& data, "
             "std::vector< std::vector< T > >& constantPhaseCorrection, "
             "std::vector< std::vector< T > >& linearPhaseCorrection, "
             "int rawLineCount, bool verbose ) const" );

}


template class gkg::AhnPhaseCorrection< float >;
template class gkg::AhnPhaseCorrection< double >;
