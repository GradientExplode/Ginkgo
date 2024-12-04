#include <gkg-mri-reconstruction-correction/ModulationTransferFunction.h>
#include <gkg-processing-signal/FourierTransform.h>
#include <gkg-processing-signal/FourierTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <fstream>


#define ZERO_FILLING_FACTOR    16
#define MAX_XCOUNT            512


// stuff dedicated to correct for wrong automatic correction(s)
static int32_t manualDisplayInfo = 0;
static int32_t currentSlice = 0;
static int32_t manualCorrectionCount = 0;
static int32_t manualSlices[ 100 ];
static float manualShifts[ 100 ];
static float manualPhases[ 100 ];


static void readManualCorrections()
{

  int32_t s;

  std::ifstream is( "corrections.txt" );

  if ( is )
  {

    is >> manualDisplayInfo >> manualCorrectionCount;
    for ( s = 0; s < manualCorrectionCount; s++ )
    {

      is >> manualSlices[ s ] >> manualShifts[ s ] >> manualPhases[ s ];

    }
    is.close();

  }
  else
  {

    manualDisplayInfo = 0;
    manualCorrectionCount = 0;

  }

}



template < class T >
bool replaceWithManualCorrection( int32_t slice, T& shift, T& phase )
{

  if ( manualCorrectionCount )
  {

    int32_t s;
    for ( s = 0; s < manualCorrectionCount; s++ )
    {

      if ( slice == manualSlices[ s ] )
      {

        shift = ( T )manualShifts[ s ];
        phase = ( T )manualPhases[ s ];
        if ( manualDisplayInfo )
        {

          std::cout << std::endl
                    << "slice : " << slice
                    << " shift : " << shift
                    << " phase : " << phase
                    << std::endl;

        }
        return true;

      }

    }

  }
  return false;
  
}

template < class T >
gkg::ModulationTransferFunction< T >::ModulationTransferFunction()
{
}


template < class T >
gkg::ModulationTransferFunction< T >::~ModulationTransferFunction()
{
}


//
// calculate the modulation transfer function (MTF) according to Bruder & al,
// Magnetic Resonance in Medecine, vol. 23, 311 (1992):
//  mtf( x ) = -odd( x, ghost ) / even( x, ghost )   (complex division)
// The result is stored in modulationTF( x, s ) for slice "s"
// Even data lines should be multiplyed by this MTF after Fourier transform 
// alongX and before Fourier transform along Y
//
template < class T >
void gkg::ModulationTransferFunction< T >::estimate(
                          const gkg::Volume< std::complex< T > >& data,
                          std::vector< std::complex< T > >& modulationTF,
                          const T& yScrolling,
                          T* outShift, T* outPhase, 
                          int32_t* outGhostLine ) const
{

  try
  {


    int32_t sizeX = data.getSizeX();
    int32_t sizeY = data.getSizeY();

    if ( ( data.getSizeZ() != 1 ) || ( data.getSizeT() != 1 ) )
    {

      throw std::runtime_error(
                         "input data has sizeZ and/or sizeT different from 1" );

    }

    // we allocate the result
    if ( ( int32_t )modulationTF.size() != sizeX )
    {

      modulationTF = std::vector< std::complex< T > >( sizeX  );

    }

    int32_t x, line;
    T shift = 0.0;
    T phase = 0.0;

    // strongest ghost line
    int32_t ghostLine = ( 0 - ( int32_t )( yScrolling * sizeY ) + sizeY ) % 
                        sizeY;
                        //findGhostLine1( even );


    if ( !replaceWithManualCorrection( currentSlice, shift, phase ) )
    {

      // we do a copy because MTF algo modifies input data
      gkg::Volume< std::complex< T > > dataCopy( data );

      gkg::Volume< std::complex< T > > even( sizeX, sizeY );
      gkg::Volume< std::complex< T > > odd( sizeX, sizeY );
      std::vector< std::complex< T > > cpy( ZERO_FILLING_FACTOR * sizeX );

      // applying Fourier transform along X_AXIS
      gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                               dataCopy, gkg::FourierTransform3d< T >::X_AXIS );

      // filling even and odd data
      for ( line = 0; line < sizeY; line++ )
      {

        if ( line % 2 )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            even( x, line ) = std::complex< T >( 0.0f, 0.0f );
            odd( x, line ) = dataCopy( x, line );

          }

        }
        else
        {
 
          for ( x = 0; x < sizeX; x++ )
          {

            even( x, line ) = dataCopy( x, line );
            odd( x, line ) = std::complex< T >( 0.0f, 0.0f );

          }

        }

      }

      // applying Fourier transform along Y_AXIS on even and odd data
      gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                   even, gkg::FourierTransform3d< T >::Y_AXIS );
      gkg::FourierTransform3d< T >::getInstance().applyCenteredInverse(
                                   odd, gkg::FourierTransform3d< T >::Y_AXIS );


      // average power on ghost line (needed for masking)
      T averagePower = 0.0f;
      for ( x = 0; x < sizeX; x++ )
      {

        // carreful : std::norm process the square of magnitude!
        averagePower += std::norm( even( x, ghostLine ) );

      }
      averagePower /= ( T )sizeX;
    
      // calculating MTF for the current slice
      for ( x = 0; x < sizeX; x++ )
      {

        // mtf( x ) = -conj(even)*odd / | -conj(even)*odd |
        modulationTF[ x ] = - std::conj( even( x, ghostLine ) ) *
                            odd( x, ghostLine );
        modulationTF[ x ] /= std::sqrt( std::norm( modulationTF[ x ] ) );

      }


      // smoothing MTF
      for ( x = 0; x < sizeX * ZERO_FILLING_FACTOR; x++ )
      {

        cpy[ x ] = std::complex< T >( 0.0f, 0.0f );

      }
      // filling centrally with MTF
      for ( x = 0; x < sizeX; x++ )
      {

        // masking regions of weak intensity
        if ( std::norm( even( x, ghostLine ) ) > averagePower / 25.0f )
        {

          cpy[ x + sizeX*ZERO_FILLING_FACTOR / 2 - sizeX / 2 ] =
            modulationTF[ x ];

        }

      }

      // calculating spectrum of MTF
      gkg::FourierTransform< T >::getInstance().applyCenteredDirect( cpy );


      // now, we get shift and phase
      shift = getShift( cpy, 1 );
      phase = std::arg( cpy[ ( int32_t )shift ] );
      shift -= ( T )sizeX * ZERO_FILLING_FACTOR / 2;

      if ( manualDisplayInfo )
      {

        std::cout << std::endl
                  << "slice : " << currentSlice
                  << " shift : " << shift
                  << " phase : " << phase
                  << std::endl;

      }

    }

    if ( outShift )
    {

      *outShift = shift / ZERO_FILLING_FACTOR;

    }

    if ( outPhase )
    {

      *outPhase = phase;

    }

    if ( outGhostLine )
    {

      *outGhostLine = ghostLine;

    }

    // doing a linear correction
    for ( x = 0; x < sizeX; x++ )
    {

      modulationTF[ x ] = std::polar(
                1.0f, 
                ( float )( 2 * M_PI * ( x - sizeX / 2 ) *
                           ( -shift / ZERO_FILLING_FACTOR ) / sizeX + phase ) );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::ModulationTransferFunction< T >::estimate( "
             "const gkg::Volume< std::complex< T > >& data, "
             "std::vector< std::complex< T > >& modulationTF, "
             "const T& yScrolling, "
             "T* outShift, T* outPhase ) const" );

}


template < class T >
void gkg::ModulationTransferFunction< T >::estimate(
                  const gkg::Volume< std::complex< T > >& data,
                  std::vector< std::vector< std::complex< T > > >& modulationTF,
                  const T& yScrolling,
                  bool verbose,
                  std::vector< T >* outShift,
                  std::vector< T >* outPhase,
                  std::vector< int32_t >* outGhostLine ) const
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
    if ( ( int32_t )modulationTF.size() != sliceCount )
    {

      modulationTF = std::vector< std::vector< std::complex< T > > >(
                                                          sliceCount );

    }

    // looping over slice(s)
    T shift, phase;
    int32_t ghostLine;

    gkg::Volume< std::complex< T > > sliceData( sizeX, lineCount );
    int32_t x, line, slice;

    readManualCorrections();

    for ( slice = 0; slice < sliceCount; slice++ )
    {

      currentSlice = slice;
      // duplicating the current slice
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          sliceData( x, line ) = data( x, line, slice );

        }

      }
      // processing the modulation transfer function for slice "slice"
      gkg::ModulationTransferFunction< T >::getInstance().estimate(
                                                          sliceData,
                                                          modulationTF[ slice ],
                                                          yScrolling,
                                                          &shift, &phase,
                                                          &ghostLine );

      if ( outShift )
      {

         ( *outShift )[ slice ] = shift;

      }
      if ( outPhase )
      {

         ( *outPhase )[ slice ] = phase;

      }
      if ( outGhostLine )
      {

         ( *outGhostLine )[ slice ] = ghostLine;

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
             "void gkg::ModulationTransferFunction< T >::estimate( "
             "const gkg::Volume< std::complex< T > >& data, "
             "std::vector< std::vector< std::complex< T > > >& modulationTF, "
             "const T& yScrolling, "
             "bool verbose, "
             "std::vector< T >* outShift, "
             "std::vector< T >* outPhase, "
             "std::vector< int >* outGhostLine ) const " );

}


template < class T >
void gkg::ModulationTransferFunction< T >::apply(
            gkg::Volume< std::complex< T > >& data,
            const std::vector< std::vector< std::complex< T > > >& modulationTF,
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

      throw std::runtime_error( "input data has sizeT different from 1" );

    }

    if ( rawLineCount > lineCount )
    {

      throw std::runtime_error( "inconsistent raw line count" );

    }

    if ( ( int32_t )modulationTF.size() != sliceCount )
    {

      throw std::runtime_error( "inconsistent MTF slice count" );

    }
    for (  slice = 0; slice < sliceCount; slice++ )
    {

      if ( ( int32_t )modulationTF[ slice ].size() != sizeX )
      {

        throw std::runtime_error( "inconsistent MTF X size" );

      }

    }

    // modulating each line with MTF
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 2; line < rawLineCount; line += 2 )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          data( x, line, slice ) *= modulationTF[ slice ][ x ];

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
             "void gkg::ModulationTransferFunction< T >::apply( "
             "gkg::Volume< std::complex< T > >& data, "
             "const std::vector< std::vector< std::complex< T > > >& "
             "modulationTF, "
             "int rawLineCount, "
             "bool verbose ) const " );

}


template < class T >
int32_t gkg::ModulationTransferFunction< T >::findGhostLine1(
                           const gkg::Volume< std::complex< T > >& image ) const
{

  int32_t sizeX = image.getSizeX();
  int32_t sizeY = image.getSizeY();

  T average = 0.0;

  int32_t x, y;
  for ( y = 0; y < sizeY; y++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      average += ( T )std::norm( image( x, y ) );

    }

  }
  average /= ( T )sizeX * ( T )sizeY;


  int32_t line;
  int32_t count;
  int32_t maxCount = 0;
  int32_t maxLine= 0;
  for ( y = 0; y < sizeY / 4; y++ )
  {

    line = y;
    count = 0;
    for ( x = 0; x < sizeX; x++ )
    {

      if ( ( T )std::norm( image( x, line ) ) > average / 2.0 )
      {

        ++ count;

      }

    }

    if ( count > maxCount )
    {

      maxCount = count;
      maxLine = line;

    }

  }

  return maxLine;  //( maxLine + sizeY / 2 ) % sizeY;

}


template < class T >
int32_t gkg::ModulationTransferFunction< T >::findGhostLine2(
                           const gkg::Volume< std::complex< T > >& image ) const
{

  int32_t sizeX = image.getSizeX();
  int32_t sizeY = image.getSizeY();

  T power = 0.0;
  T powerTotal = 0.0;
  T yAverage = 0.0;

  T maxPower = 0.0;
  //T minPower = 1e38;

  int32_t maxLine = 0;
  //int32_t minLine = 0;

  int32_t x, y;
  for ( y = 0; y < sizeY; y++ )
  {

    power = 0.0;

    for ( x = ( int32_t )( 0.45 * sizeX );
          x < ( int32_t )( 0.55 * sizeX ); 
          x++ )
    {

      power += ( T )std::norm( image( x, y ) );

    }
    yAverage += y * power;
    powerTotal += power;

    if ( power > maxPower )
    {

      maxPower = power;
      maxLine = y;

    }
    //else if ( power < minPower )
    //{

    //  minPower = power;
    //  minLine = y;

    //}

  }

  if ( powerTotal == 0.0 )
  {

    powerTotal = 1.0;

  }
  maxLine = ( int32_t )( yAverage / powerTotal );

  return ( maxLine + sizeY / 2 ) % sizeY;

}


template < class T >
T gkg::ModulationTransferFunction< T >::getShift(
               const std::vector< std::complex< T > >& vec, int32_t skip ) const
{

  int32_t x;
  int32_t size = ( int32_t )vec.size();
  T previousValue = 0.0f;
  T currentValue = 0.0f;
  T nextValue = 0.0f;
  int32_t xCount = 0;
  int32_t xTable[ MAX_XCOUNT ];
  T valueTable[ MAX_XCOUNT ];

  // searching all maximum
  for ( x = 1; x < size - 1; x++ )
  {

    nextValue = std::sqrt( std::norm( vec[ x ] ) );
    if ( currentValue >= previousValue &&
         currentValue > nextValue &&
         xCount < MAX_XCOUNT )
    {

      xTable[ xCount ] = x;
      valueTable[ xCount ] = currentValue;
      xCount ++;

    }
    previousValue = currentValue;
    currentValue = nextValue;

  }

  int32_t position = 0;
  // selecting highest maximum
  for ( x = skip; x < xCount; x++ )
  {

    if ( valueTable[ x ] > currentValue )
    {

      position = xTable[ x ];
      currentValue = valueTable[ x ];

    }

  }

  int32_t from = std::max( 0, position - 4 );
  int32_t to = std::min( size, position + 4 );
  float p = 0.0f;
  float z = 0.0f;
  for ( x = from; x < to; x++ )
  {

    p += std::sqrt( std::norm( vec[ x ] ) ) * ( float )x;
    z += std::sqrt( std::norm( vec[ x ] ) );

  }

  return p / z;

}


template class gkg::ModulationTransferFunction< float >;
template class gkg::ModulationTransferFunction< double >;


#undef ZERO_FILLING_FACTOR
#undef MAX_XCOUNT
