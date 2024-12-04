#include <gkg-processing-signal/Remez.h>
#include <algorithm>
#include <cmath>
#include <iostream>


#define MAXIMUM_ITERATION_COUNT       40


gkg::Remez::Remez( gkg::Remez::FilterType filterType, int gridDensity )
{

  _filterType = filterType;

  if ( gridDensity < GKG_MINIMUM_REMEZ_GRID_SIZE )
  {

    std::cerr << "gkg::Remez::Remez: gridDensity < 16 -> replace by 16 "
              << std::endl;

  }
  _gridDensity = std::max( GKG_MINIMUM_REMEZ_GRID_SIZE, gridDensity );

}


bool gkg::Remez::getOptimalFilter( std::vector< double >& response,
                                   int32_t sampleCount,
                                   const std::vector< double >& frequencies,
                                   const std::vector< double >& magnitudes,
                                   const std::vector< double >& weights ) const
{

  int32_t tapsCount = sampleCount + 1;
  int32_t i, j, k, b;

  //
  // sanity checks
  //
  if ( frequencies.size() % 2 )
  {

    std::cerr << "gkg::Remez::getOptimalFilter: must have an even "
              << "number of band edges"
              << std::endl;
    return false;

  }

  if ( frequencies.size() != magnitudes.size() )
  {

    std::cerr << "gkg::Remez::getOptimalFilter: frequencies and  "
              << "magnitudes should have same item count"
              << std::endl;
    return false;

  }

  if ( weights.size() != ( magnitudes.size() / 2 ) )
  {

    std::cerr << "gkg::Remez::getOptimalFilter: weights item count "
              << "should be half frequencies item count"
              << std::endl;
    return false;

  }


  size_t f;
  std::vector< double > localFrequencies;
  localFrequencies = frequencies;
  for ( f = 0; f < localFrequencies.size(); f++ )
  {

    localFrequencies[ f ] /= 2.0f;

  }

  //
  // calculate band & extrema count
  //
  int32_t bandCount = ( int32_t )frequencies.size() / 2;
  int32_t extremaCount = tapsCount / 2;
  if ( ( tapsCount % 2 ) && ( _filterType == gkg::Remez::BandPass ) )
  {

    extremaCount ++;

  }

  //
  // predict dense grid size in advance for memory allocation
  //
  int32_t gridSize = 0;
  for ( i = 0; i < bandCount; i++ )
  {

    gridSize += ( int32_t )( 2 * extremaCount * _gridDensity *
                            ( localFrequencies[ 2 * i + 1 ] - 
                              localFrequencies[ 2 * i ] ) + 0.5 );

  }
  if ( _filterType != gkg::Remez::BandPass )
  {

    gridSize --;

  }

  //
  // create dense frequency grid
  //
  double deltaFrequency = 0.5 / ( _gridDensity * extremaCount );
  // for Differentiator and Hilbert, 
  //   grid[ 0 ] = max( deltaFrequency, frequencies[ 0 ] )
  double grid0 = ( ( _filterType != gkg::Remez::BandPass ) &&
                  ( deltaFrequency > localFrequencies[ 0 ] ) ) ?
                  deltaFrequency :
                  localFrequencies[ 0 ];

  std::vector< double > D( gridSize );
  std::vector< double > W( gridSize );
  std::vector< double > grid( gridSize );
  double lowerFrequency = 0;
  double upperFrequency = 0;
  for ( b = 0, j = 0; b < bandCount; b++ )
  {

    lowerFrequency = ( b == 0 ? grid0 : localFrequencies[ 2 * b ] );
    upperFrequency = localFrequencies[ 2 * b + 1 ];
    k = ( int )( ( upperFrequency - lowerFrequency ) / deltaFrequency + 0.5f  );
    for ( i = 0; i < k; i++ )
    {

      D[ j ] = magnitudes[ 2 * b ] + i * ( magnitudes[ 2 * b + 1 ] -
                                           magnitudes[ 2 * b ] ) / ( k - 1 );
      W[ j ] = weights[ b ];
      grid[ j ] = lowerFrequency;
      lowerFrequency += deltaFrequency;
      j ++;

    }
    grid[ j - 1 ] = upperFrequency;

  }

  // if not bandpass filter, last grid point cannot be 0.5, but if there are
  // even taps count, leave the last grid point at 0.5
  if ( ( _filterType != gkg::Remez::BandPass ) &&
       ( grid[ gridSize - 1 ] > ( 0.5f - deltaFrequency ) ) && 
       ( tapsCount % 2 ) )
  {

    grid[ gridSize - 1 ] = 0.5f - deltaFrequency;

  }

  //
  // initialize extrema frequency indexes evenly througout the dense grid
  //
  std::vector< int32_t > extremaIndexes( extremaCount + 1 );
  for ( i = 0; i <= extremaCount; i++ )
  {

    extremaIndexes[ i ] = i * ( gridSize - 1 ) / extremaCount;

  }

  //
  // fix grid if Differentiator type
  //
  if ( _filterType == gkg::Remez::Differentiator )
  {

    for ( i = 0; i < gridSize; i++ )
    {

      if ( D[ i ] > 0.0001 )
      {

        W[ i ] = W[ i ] / grid[ i ];

      }

    }

  }

  //
  // for odd or non-passband filters, alter the D[] and W[] according to
  // Parks-McClellan algorithm
  //
  if ( _filterType == gkg::Remez::BandPass )
  {

    if ( tapsCount % 2 == 0 )
    {

      for ( i = 0; i < gridSize; i++ )
      {

        D[ i ] /= std::cos( M_PI * grid[ i ] );
        W[ i ] *= std::cos( M_PI * grid[ i ] );

      }

    }

  }
  else
  {

    if ( tapsCount % 2 )
    {

      for ( i = 0; i < gridSize; i++ )
      {

        D[ i ] /= std::sin( 2 * M_PI * grid[ i ] );
        W[ i ] *= std::sin( 2 * M_PI * grid[ i ] );

      }

    }
    else
    {

      for ( i = 0; i < gridSize; i++ )
      {

        D[ i ] /= std::sin( M_PI * grid[ i ] );
        W[ i ] *= std::sin( M_PI * grid[ i ] );

      }

    }

  }

  //
  // perform Remez' exchange algorithm
  //
  int32_t iteration;
  std::vector< double > ad( extremaCount + 1 );
  std::vector< double > x( extremaCount + 1 );
  std::vector< double > y( extremaCount + 1 );
  std::vector< double > error( gridSize );

  for ( iteration = 0; iteration < MAXIMUM_ITERATION_COUNT; iteration++ )
  {


    getParameters( extremaIndexes,
                   grid,
                   D,
                   W,
                   ad,
                   x,
                   y );
    getError( grid,
              D,
              W,
              ad,
              x,
              y,
              error );    
    if ( getExtrema( extremaIndexes, error ) == false )
    {

      return false;

    }
    if ( hasConverged( extremaIndexes, error ) )
    {

      break;

    }

  }

  getParameters( extremaIndexes,
                 grid,
                 D,
                 W,
                 ad,
                 x,
                 y );

  //
  // find the taps of the filter for use with frequency sampling; if odd or
  // non-passband filter, fix the taps according to Parks-McClellan
  //
  std::vector< double > taps( extremaCount + 1 );
  for ( i = 0; i <= tapsCount / 2; i++ )
  {

    if ( _filterType == gkg::Remez::BandPass )
    {


      if ( tapsCount % 2 )
      {

        taps[ i ] = getA( ( double )i / tapsCount, ad, x, y );

      }
      else
      {

        taps[ i ] = getA( ( double )i / tapsCount, ad, x, y ) *
                    std::cos( M_PI * ( double )i / tapsCount );

      }

    }
    else
    {

      if ( tapsCount % 2 )
      {

        taps[ i ] = getA( ( double )i / tapsCount, ad, x, y ) *
                    std::sin( 2 * M_PI * ( double )i / tapsCount );

      }
      else
      {

        taps[ i ] = getA( ( double )i / tapsCount, ad, x, y ) *
                    std::sin( M_PI * ( double )i / tapsCount );

      }

    }

  }

  //
  // frequency sampling design with calculated taps
  //
  getFrequencySamples( response, taps, tapsCount );

  return true;

}


bool gkg::Remez::getOptimalFilter(
                                 std::vector< std::complex< double > >& response,
                                 int32_t sampleCount,
                                 const std::vector< double >& frequencies,
                                 const std::vector< double >& magnitudes,
                                 const std::vector< double >& weights ) const
{

  std::vector< double > realResponse;
  if ( getOptimalFilter( realResponse,
                         sampleCount,
                         frequencies,
                         magnitudes,
                         weights ) == false )
  {

    return false;

  }

  response = std::vector< std::complex< double > >( realResponse.size() );
  int32_t i;
  for ( i = 0; i < ( int32_t )response.size(); i++ )
  {

    response[ i ] = std::complex< double >( realResponse[ i ], 0.0f );

  }

  return true;

}


void gkg::Remez::getParameters( const std::vector< int32_t >& extremaIndexes,
                                const std::vector< double >& grid,
                                const std::vector< double >& D,
                                const std::vector< double >& W,
                                std::vector< double >& ad,
                                std::vector< double >& x,
                                std::vector< double >& y ) const
{

  int32_t i, j, k, ld;
  double numerator, denominator, sign, delta;

  int32_t extremaCount = ( int32_t )x.size() - 1;

  //
  // find x
  //
  for ( i = 0; i <= extremaCount; i++ )
  {

    x[ i ] = std::cos( 2 * M_PI * grid[ extremaIndexes[ i ] ] );

  }

  //
  // calculate ad (Oppenheim & Schafer eq 7.132 )
  //
  // skips around to avoid round errors
  ld = ( extremaCount - 1 ) / 15 + 1;
  for ( i = 0; i <= extremaCount; i++ )
  {

    denominator = 1.0f;
    for ( j = 0; j < ld; j++ )
    {

      for ( k = j; k <= extremaCount; k += ld )
      {

        if ( k != i )
        {

          denominator *= 2.0f * ( x[ i ] - x[ k ] );

        }

      }

    }
    if ( std::fabs( denominator ) < 0.00001f )
    {

      denominator = 0.00001f;

    }
    ad[ i ] = 1.0f / denominator;

  }

  //
  // calculate delta (Oppenheim & Schafer eq 7.131 )
  //
  numerator = 0.0f;
  denominator = 0.0f;
  sign = 1.0f;
  for ( i = 0; i <= extremaCount; i++ )
  {

    numerator += ad[ i ] * D[ extremaIndexes[ i ] ];
    denominator += sign * ad[ i ] / W[ extremaIndexes[ i ] ];
    sign = -sign;

  }
  delta = numerator / denominator;
  sign = 1.0f;

  //
  // calculate y (Oppenheim & Schafer eq 7.133b )
  //
  for ( i = 0; i <= extremaCount; i++ )
  {

    y[ i ] = D[ extremaIndexes[ i ] ] - sign * delta / W[ extremaIndexes[ i ] ];
    sign = -sign;

  }

}


void gkg::Remez::getError( const std::vector< double >& grid,
                           const std::vector< double >& D,
                           const std::vector< double >& W,
                           const std::vector< double >& ad,
                           const std::vector< double >& x,
                           const std::vector< double >& y,
                           std::vector< double >& error ) const
{

  int32_t j;
  int32_t gridSize = ( int32_t )grid.size();

  for ( j = 0; j < gridSize; j++ )
  {

    error[ j ] = W[ j ] * ( D[ j ] - getA( grid[ j ], ad, x, y ) );

  }

}


bool gkg::Remez::getExtrema( std::vector< int32_t >& extremaIndexes,
                             const std::vector< double >& error ) const
{

  int32_t i, j, k, l, extra;
  bool up, alt;

  int32_t extremaCount = ( int32_t )extremaIndexes.size() - 1;
  int32_t gridSize = ( int32_t )error.size();

  //
  // allocate space for found extrema indexes
  //
  std::vector< int32_t > foundExtremaIndexes( 2 * extremaCount );
  k = 0;

  //
  // check for extremum at 0
  //
  if ( ( ( error[ 0 ] > 0.0f ) && ( error[ 0 ] > error[ 1 ] ) ) ||
       ( ( error[ 0 ] < 0.0f ) && ( error[ 0 ] < error[ 1 ] ) ) )
  {

    foundExtremaIndexes[ k ] = 0;
    k ++;

  }

  //
  // check for extrema inside dense grid
  //
  for ( i = 1; i < gridSize - 1; i++ )
  {

    if ( ( ( error[ i ] >= error[ i - 1 ] ) &&
           ( error[ i ] > error[ i + 1 ] ) &&
           ( error[ i ] > 0.0f ) ) ||
         ( ( error[ i ] <= error[ i - 1 ] ) &&
           ( error[ i ] < error[ i + 1 ] ) &&
           ( error[ i ] < 0.0f ) ) )
    {

      // we sometimes get too many extremal frequencies
      if ( k >= 2 * extremaCount )
      {

        std::cerr << "gkg::Remez::getExtrema: too many extrema frequencies"
                  << std::endl;
        return false;

      }
      foundExtremaIndexes[ k ] = i;
      k ++;

    }    

  }

  //
  // check for extremum at 0.5
  //
  if ( ( ( error[ gridSize - 1 ] > 0.0f ) &&
         ( error[ gridSize - 1 ] > error[ gridSize - 2 ] ) ) ||
       ( ( error[ gridSize - 1 ] < 0.0f ) &&
         ( error[ gridSize - 1 ] < error[ gridSize - 2 ] ) ) )
  {

    // we sometimes get too many extrema frequencies
    if ( k >= 2 * extremaCount )
    {

      std::cerr << "gkg::Remez::getExtrema: too many extrema frequencies"
                << std::endl;
      return false;

    }
    foundExtremaIndexes[ k ] = gridSize - 1;
    k ++;

  }    

  // we sometimes get not enough extrema frequencies
  if ( k < extremaCount + 1 )
  {

    std::cerr << "gkg::Remez::getExtrema: not enough extrema frequencies"
              << std::endl;
    return false;

  }

  //
  // remove extra extrema
  //
  extra = k - ( extremaCount + 1 );
  if ( extra < 0 )
  {

    std::cerr << "gkg::Remez::getExtrema: extra < 0"
              << std::endl;
    return false;

  }

  while ( extra > 0 )
  {

    if ( error[ foundExtremaIndexes[ 0 ] ] > 0.0f )
    {

      up = true;

    }
    else
    {

      up = false;

    }
    l = 0;
    alt = true;
    // at the end of the loop, all extrema are alternating
    for ( j = 1; j < k; j++ )
    {

      if ( std::fabs( error[ foundExtremaIndexes[ j ] ] ) <
           std::fabs( error[ foundExtremaIndexes[ l ] ] ) )
      {

        // new smallest error
        l = j;

      }
      if ( up && ( error[ foundExtremaIndexes[ j ] ] < 0.0f ) )
      {

        // switch to minima
        up = false;

      }
      else if ( ( !up ) && ( error[ foundExtremaIndexes[ j ] ] > 0.0f ) )
      {

        // switch to maxima
        up = true;

      }
      else
      {

        // break now and you will delete the smallest overall extrema;
        // if you want to delete the smallest of the pair of non-alternating
        // extrema, you must do:
        //   if ( std::fabs( error[ foundExtremaIndexes[ j ] ] ) <
        //        std::fabs( error[ foundExtremaIndexes[ j - 1 ] ] )
        //     l = j;
        //   else
        //     l = j - 1;

        // found two non-alternating extrema and delete the smallest of them
        alt = false;
        break;

      }

    }

    //
    // if there is only ne extrema and all are alternating, delete the smallest
    // of the first/last extrema
    //
    if ( alt && ( extra == 1 ) )
    {

      if ( std::fabs( error[ foundExtremaIndexes[ k - 1 ] ] ) <
           std::fabs( error[ foundExtremaIndexes[ 0 ] ] ) )
      {

        // delete last extrema
        l = k - 1;

      }
      else
      {

        // delete first extrema
        l = 0;

      }

    }
    for ( j = l; j < k - 1; j++ )
    {

      foundExtremaIndexes[ j ] = foundExtremaIndexes[ j + 1 ];

    }
    k --;
    extra --;

  }

  for ( i = 0; i <= extremaCount; i++ )
  {

    extremaIndexes[ i ] = foundExtremaIndexes[ i ];

  }

  return true;

}


bool gkg::Remez::hasConverged( const std::vector< int32_t >& extremaIndexes,
                               const std::vector< double >& error ) const
{

  int32_t i;
  double minimum = std::fabs( error[ extremaIndexes[ 0 ] ] );
  double maximum = std::fabs( error[ extremaIndexes[ 0 ] ] );
  double current = 0;
  int32_t extremaCount = ( int32_t )extremaIndexes.size() - 1;

  for ( i = 1; i <= extremaCount; i++ )
  {

    current = std::fabs( error[ extremaIndexes[ i ] ] );
    if ( current < minimum )
    {

      minimum = current;

    }
    if ( current > maximum )
    {

      maximum = current;

    }

  }
  return ( ( maximum - minimum ) / maximum ) < 0.0001f;

}


double gkg::Remez::getA( double frequency,
                         const std::vector< double >& ad,
                         const std::vector< double >& x,
                         const std::vector< double >& y ) const
{

  int32_t i;
  double c;

  int32_t extremaCount = ( int32_t )x.size() - 1;
  double numerator = 0.0f;
  double denominator = 0.0f;

  for ( i = 0; i <= extremaCount; i++ )
  {

    c = std::cos( 2 * M_PI * frequency ) - x[ i ];
    if ( std::fabs( c ) < 1.0e-7 )
    {

      numerator = y[ i ];
      denominator = 1.0f;
      break;

    }
    c = ad[ i ] / c;
    numerator += c * y[ i ];
    denominator += c;

  }
  return numerator / denominator;

}


void gkg::Remez::getFrequencySamples( std::vector< double >& response,
                                      const std::vector< double >& taps,
                                      int32_t tapsCount ) const
{

  int32_t n, k;
  double deltaX, value;

  double M = ( tapsCount - 1.0f ) / 2.0f;

  response = std::vector< double >( tapsCount );

  if ( _filterType == gkg::Remez::BandPass )
  {

    if ( tapsCount % 2 )
    {

      for ( n = 0; n < tapsCount; n++ )
      {

        deltaX = 2.0f * M_PI * ( n - M ) / tapsCount;
        value = taps[ 0 ];
        for ( k = 1; k <= ( int32_t )M; k++ )
        {

          value += 2.0f * taps[ k ] * std::cos( k * deltaX );

        }
        response[ n ] = value / tapsCount;

      }

    }
    else
    {

      for ( n = 0; n < tapsCount; n++ )
      {

        deltaX = 2.0f * M_PI * ( n - M ) / tapsCount;
        value = taps[ 0 ];
        for ( k = 1; k <= ( tapsCount / 2 - 1 ); k++ )
        {

          value += 2.0f * taps[ k ] * std::cos( k * deltaX );

        }
        response[ n ] = value / tapsCount;

      }

    }

  }
  else
  {

    if ( tapsCount % 2 )
    {

      for ( n = 0; n < tapsCount; n++ )
      {

        deltaX = 2.0f * M_PI * ( n - M ) / tapsCount;
        value = 0.0f;
        for ( k = 1; k <= ( int32_t )M; k++ )
        {

          value += 2.0f * taps[ k ] * std::sin( k * deltaX );

        }
        response[ n ] = value / tapsCount;

      }

    }
    else
    {

      for ( n = 0; n < tapsCount; n++ )
      {

        deltaX = 2.0f * M_PI * ( n - M ) / tapsCount;
        value = taps[ tapsCount / 2 ] * std::sin( M_PI * ( n - M ) );
        for ( k = 1; k <= ( tapsCount / 2 - 1 ); k++ )
        {

          value += 2.0f * taps[ k ] * std::sin( k * deltaX );

        }
        response[ n ] = value / tapsCount;

      }

    }

  }

}
