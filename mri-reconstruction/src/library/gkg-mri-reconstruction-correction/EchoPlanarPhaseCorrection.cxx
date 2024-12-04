#include <gkg-mri-reconstruction-correction/EchoPlanarPhaseCorrection.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>


//#define DEBUG_REFERENCE_SCAN 1

#ifdef DEBUG_REFERENCE_SCAN

#include <fstream>

#endif


template < class T >
gkg::EchoPlanarPhaseCorrection< T >::EchoPlanarPhaseCorrection()
{
}


template < class T >
gkg::EchoPlanarPhaseCorrection< T >::~EchoPlanarPhaseCorrection()
{
}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::estimate(
                             const std::vector< T >& ahnConstantPhaseCorrection,
                             const std::vector< T >& ahnLinearPhaseCorrection,
                             int32_t rawLineCount,
                             std::vector< T >& epiConstantPhaseCorrection,
                             std::vector< T >& epiLinearPhaseCorrection,
                             const std::vector< int32_t >& flip,
                             const T& bestky,
                             int32_t fitPointCount,
                             int32_t constantFitOrder,
                             int32_t linearFitOrder ) const
{

  try
  {

    // sanity checks
    if ( ( int32_t )ahnConstantPhaseCorrection.size() != rawLineCount )
    {

      throw std::runtime_error( "Ahn constant phase correction vector and Y "
                                "raw size inconsistency" );

    }

    if ( ( int32_t )ahnLinearPhaseCorrection.size() != rawLineCount )
    {

      throw std::runtime_error( "Ahn linear phase correction vector and Y "
                                "raw size inconsistency" );

    }

    if ( ( int32_t )flip.size() != rawLineCount )
    {

      throw std::runtime_error( "flip vector and Y raw size inconsistency" );

    }

    // collecting positive line points and negative line points
    std::vector< int32_t > positivePointIndices( fitPointCount );
    std::vector< int32_t > negativePointIndices( fitPointCount );
    collectPointIndices( positivePointIndices,
                         negativePointIndices,
                         rawLineCount,
                         flip,
                         bestky );


#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "positive point indices : ";
    for ( int32_t p = 0; p < ( int32_t )positivePointIndices.size(); p++ )
    {

      std::cout << positivePointIndices[ p ]  << " ";

    }
    std::cout << std::endl;
    std::cout << "negative point indices : ";
    for ( int32_t p = 0; p < ( int32_t )negativePointIndices.size(); p++ )
    {

      std::cout << negativePointIndices[ p ]  << " ";

    }
    std::cout << std::endl;

#endif

    // unwraping Ahn constant phase correction
    std::vector< T > 
      unwrappedAhnConstantPhaseCorrection = ahnConstantPhaseCorrection;
    unwrap( unwrappedAhnConstantPhaseCorrection, bestky );

    // positive linear readout fit

#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "positive linear fit :" << std::endl;

#endif

    std::vector< T > positiveLinearA = fit( positivePointIndices,
                                            ahnLinearPhaseCorrection,
                                            bestky,
                                            linearFitOrder );

    // negative linear readout fit

#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "negative linear fit :" << std::endl;

#endif

    std::vector< T > negativeLinearA = fit( negativePointIndices,
                                            ahnLinearPhaseCorrection,
                                            bestky,
                                            linearFitOrder );

    // positive constant readout fit

#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "positive constant fit :" << std::endl;

#endif

    std::vector< T > positiveConstantA = fit(
                                            positivePointIndices,
                                            unwrappedAhnConstantPhaseCorrection,
                                            bestky,
                                            constantFitOrder );

    // negative constant readout fit

#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "negative constant fit :" << std::endl;

#endif

    std::vector< T > negativeConstantA = fit(
                                            negativePointIndices,
                                            unwrappedAhnConstantPhaseCorrection,
                                            bestky,
                                            constantFitOrder );

    // generating new linear coefficients using the fit
    getEpiPhaseCorrection( positiveLinearA,
                           negativeLinearA,
                           rawLineCount,
                           epiLinearPhaseCorrection,
                           flip,
                           bestky,
                           linearFitOrder );

    // generating new constant coefficients using the fit
    getEpiPhaseCorrection( positiveConstantA,
                           negativeConstantA,
                           rawLineCount,
                           epiConstantPhaseCorrection,
                           flip,
                           bestky,
                           constantFitOrder );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::EchoPlanarPhaseCorrection< T >::estimate( "
             "const std::vector< T >& ahnConstantPhaseCorrection, "
             "const std::vector< T >& ahnLinearPhaseCorrection, "
             "int rawLineCount, "
             "std::vector< T >& epiConstantPhaseCorrection, "
             "std::vector< T >& epiLinearPhaseCorrection, "
             "const std::vector< int >& flip, "
             "const T& bestky, "
             "int fitPointCount, "
             "int constantFitOrder, "
             "int linearFitOrder ) const" );

}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::estimate(
              const std::vector< std::vector< T > >& ahnConstantPhaseCorrection,
              const std::vector< std::vector< T > >& ahnLinearPhaseCorrection,
              int32_t rawLineCount,
              std::vector< std::vector< T > >& epiConstantPhaseCorrection,
              std::vector< std::vector< T > >& epiLinearPhaseCorrection,
              bool verbose,
              const std::vector< int32_t >& flip,
              const T& bestky,
              int32_t fitPointCount,
              int32_t constantFitOrder,
              int32_t linearFitOrder ) const
{

  try
  {

    int32_t sliceCount = ( int32_t )ahnConstantPhaseCorrection.size();

    // sanity checks
    if ( ( int32_t )ahnLinearPhaseCorrection.size() != sliceCount )
    {

      throw std::runtime_error( "Inconsistent constant  and linear Ahn "
                                "phase corrections" );

    }


    // we allocate the result
    if ( ( int32_t )epiConstantPhaseCorrection.size() != sliceCount )
    {

      epiConstantPhaseCorrection = std::vector< std::vector< T > >(
                                                                   sliceCount );

    }
    if ( ( int32_t )epiLinearPhaseCorrection.size() != sliceCount )
    {

      epiLinearPhaseCorrection = std::vector< std::vector< T > >( sliceCount );

    }


    // looping over slice(s)
    int32_t slice;  
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      gkg::EchoPlanarPhaseCorrection< T >::getInstance().estimate(
                                            ahnConstantPhaseCorrection[ slice ],
                                            ahnLinearPhaseCorrection[ slice ],
                                            rawLineCount,
                                            epiConstantPhaseCorrection[ slice ],
                                            epiLinearPhaseCorrection[ slice ],
                                            flip,
                                            bestky,
                                            fitPointCount,
                                            constantFitOrder,
                                            linearFitOrder );

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
             "void gkg::EchoPlanarPhaseCorrection< T >::estimate( "
             "const std::vector< std::vector< T > >& "
             "ahnConstantPhaseCorrection, "
             "const std::vector< std::vector< T > >& ahnLinearPhaseCorrection, "
             "int rawLineCount, "
             "std::vector< std::vector< T > >& epiConstantPhaseCorrection, "
             "std::vector< std::vector< T > >& epiLinearPhaseCorrection, "
             "bool verbose, "
             "const std::vector< int >& flip, "
             "const T& bestky, "
             "int fitPointCount, "
             "int constantFitOrder, "
             "int linearFitOrder ) const" );

}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::apply(
              gkg::Volume< std::complex< T > >& data,
              const std::vector< std::vector< T > >& epiConstantPhaseCorrection,
              const std::vector< std::vector< T > >& epiLinearPhaseCorrection,
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

    if ( ( int32_t )epiConstantPhaseCorrection.size() != sliceCount )
    {

      throw std::runtime_error( "EPI constant phase correction vector and"
                                "data slice count inconsistency" );

    }

    if ( ( int32_t )epiLinearPhaseCorrection.size() != sliceCount )
    {

      throw std::runtime_error( "EPI linear phase correction vector and"
                                "data slice count inconsistency" );

    }

    for ( slice = 0; slice < sliceCount; slice ++ )
    {

      if ( ( int32_t )epiConstantPhaseCorrection[ slice ].size() != 
           rawLineCount )
      {

        throw std::runtime_error( "EPI constant phase correction vector and Y "
                                  "volume size inconsistency" );

      }
      if ( ( int32_t )epiLinearPhaseCorrection[ slice ].size() != rawLineCount )
      {

        throw std::runtime_error( "EPI linear phase correction vector and Y "
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
                                  epiLinearPhaseCorrection[ slice ][ line ] +
                                  epiConstantPhaseCorrection[ slice ][ line ] );

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
             "std::vector< std::vector< T > >& epiConstantPhaseCorrection, "
             "std::vector< std::vector< T > >& epiLinearPhaseCorrection,"
             "int rawLineCount, "
             "bool verbose ) const" );

}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::collectPointIndices(
                  std::vector< int32_t >& positivePointIndices,
                  std::vector< int32_t >& negativePointIndices,
                  int32_t rawLineCount,
                  const std::vector< int32_t >& flip,
                  const T& bestky ) const
{

  try
  {

    int32_t line, p;
    int32_t fitPointCount = ( int32_t )positivePointIndices.size();

    // flag indicating that each line is free
    std::vector< bool > lineFree( rawLineCount );
    for ( line = 0; line < rawLineCount; line ++ )
    {

      lineFree[ line ] = true;

    }

    // flag indicating that each line is free
    for ( p = 0; p < fitPointCount; p++ )
    {

      positivePointIndices[ p ] = -1;
      negativePointIndices[ p ] = -1;

    }

    // selecting point to use for fit
    int32_t bestPositiveIndex;
    int32_t bestNegativeIndex;
    T bestPositiveDistance;
    T bestNegativeDistance;
    for ( p = 0; p < fitPointCount; p++ )
    {

      bestPositiveIndex = bestNegativeIndex = -1;
      bestPositiveDistance = bestNegativeDistance = 2048.0;

      for ( line = 0; line < rawLineCount; line++ )
      {

        if ( lineFree[ line ] )
        {

          if ( flip[ line ] > 0 )
          {

            if ( std::fabs( line - bestky ) <=
                 std::fabs( bestPositiveDistance ) )
            {

              bestPositiveIndex = line;
              bestPositiveDistance = ( T )line - bestky;

            }

          }
          else if ( flip[ line ] < 0 )
          {

            if ( std::fabs( line - bestky ) <=
                 std::fabs( bestNegativeDistance ) )
            {

              bestNegativeIndex = line;
              bestNegativeDistance = ( T )line - bestky;

            }

          }

        }

      }

      // check that valid indices have been found
      if ( ( bestPositiveIndex < 0 ) || ( bestNegativeIndex < 0 ) )
      {

        throw std::runtime_error( "no fit point found" );

      }

      // mark the lines chosen above as used
      lineFree[ bestPositiveIndex ] = false;
      lineFree[ bestNegativeIndex ] = false;

      // save points selected for this loop
      positivePointIndices[ p ] = bestPositiveIndex;
      negativePointIndices[ p ] = bestNegativeIndex;

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::EchoPlanarPhaseCorrection< T >::collectPointIndices( "
             "std::vector< int >& positivePointIndices, "
             "std::vector< int >& negativePointIndices, "
             "int rawLineCount, "
             "const std::vector< int >& flip, "
             "const T& bestky ) const" );

}


template < class T >
std::vector< T > gkg::EchoPlanarPhaseCorrection< T >::fit(
                                     const std::vector< int32_t >& pointIndices,
                                     const std::vector< T >& phaseCorrection,
                                     const T& bestky,
                                     int32_t fitOrder ) const
{

  try
  {

    int32_t p;
    int32_t fitPointCount = ( int32_t )pointIndices.size();

    // positive lines readout fit
    gkg::Vector x( fitPointCount );
    gkg::Vector y( fitPointCount );

    for ( p = 0; p < fitPointCount; p++ )
    {

      x( p ) = bestky - ( T )pointIndices[ p ];
      y( p ) = phaseCorrection[ pointIndices[ p ] ];
      
    }
    gkg::Vector a( fitOrder );

    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    factory->getSingularValueNthOrderFit( x, y, a );

    std::vector< T > result( fitOrder );
    int32_t o;
    for ( o = 0; o < fitOrder; o++ )
    {

      result[ o ] = a( o );

    }


#ifdef DEBUG_REFERENCE_SCAN

    std::cout << "y = " << a( 0 ) << " + " << a( 1 ) << " * x" << std::endl;
    for ( p = 0; p < fitPointCount; p++ )
    {

      std::cout << pointIndices[ p ]
                << " : ( " << x( p ) << ", " << y( p ) << " ) -> "
                << a( 0 ) + a( 1 ) * x( p ) << std::endl;

    }

#endif

    return result;

  }
  GKG_CATCH( "template < class T > "
             "std::vector< T > gkg::EchoPlanarPhaseCorrection< T >::fit( "
             "const std::vector< int >& pointIndex, "
             "const std::vector< T >& phaseCorrection, "
             "const T& bestky, "
             "int fitOrder ) const" );

}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::getEpiPhaseCorrection(
                                           const std::vector< T >& positiveA,
                                           const std::vector< T >& negativeA,
                                           int32_t rawLineCount,
                                           std::vector< T >& epiPhaseCorrection,
                                           const std::vector< int32_t >& flip,
                                           const T& bestky,
                                           int32_t fitOrder ) const
{

  try
  {


    // allocating vector with raw Y size
    if ( ( int32_t )epiPhaseCorrection.size() != rawLineCount )
    {

      epiPhaseCorrection = std::vector< T >( rawLineCount );

    }

    // processing new coefficients using the positive and negative fit
    int32_t line, o;
    T tmp1, tmp2;
    for ( line = 0; line < rawLineCount; line++ )
    {

      T y = bestky - ( T )line;

      tmp1 = 0.0;
      for ( o = 0; o < fitOrder; o++ )
      {

        if ( ( o == 0 ) && ( std::fabs( y ) < 1e-6 ) )
        {

          // check to be sure we do not call pow( 0, 0 )
          tmp2 = 1.0;
 
        }
        else
        {

          tmp2 = std::pow( y, ( T )o );

        }
        if ( flip[ line ] >= 0 )
        {

          tmp1 += positiveA[ o ] * tmp2;

        }
        else
        {

          tmp1 += negativeA[ o ] * tmp2;

        }

      }
      epiPhaseCorrection[ line ] = tmp1;

    }    

  }
  GKG_CATCH( "template < class T > "
             "void gkg::EchoPlanarPhaseCorrection< T >::getEpiPhaseCorrection( "
             "const std::vector< T >& positiveA, "
             "const std::vector< T >& negativeA, "
             "int rawLineCount, "
             "std::vector< T >& epiPhaseCorrection, "
             "const std::vector< int >& flip, "
             "const T& bestky, "
             "int fitOrder ) const" );

}


template < class T >
void gkg::EchoPlanarPhaseCorrection< T >::unwrap(
                                                 std::vector< T >& wrappedPhase,
                                                 const T& bestky ) const
{

  try
  {

    int32_t rawLineCount = ( int32_t )wrappedPhase.size();
    int32_t middlePoint = ( int32_t )std::ceil( bestky );
    T jump;

    int32_t line;
    for ( line = middlePoint; line < rawLineCount - 1; line++ )
    {

      jump = wrappedPhase[ line ] - wrappedPhase[ line + 1 ];
      if ( jump >= 3.5 )
      {

        wrappedPhase[ line + 1 ] += 2.0 * M_PI;

      }
      else if ( jump <= -3.5 )
      {

        wrappedPhase[ line + 1 ] -= 2.0 * M_PI;

      }

    }

    for ( line = middlePoint; line > 0; line-- )
    {

      jump = wrappedPhase[ line ] - wrappedPhase[ line - 1 ];
      if ( jump >= 3.5 )
      {

        wrappedPhase[ line - 1 ] += 2.0 * M_PI;

      }
      else if ( jump <= -3.5 )
      {

        wrappedPhase[ line - 1 ] -= 2.0 * M_PI;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::EchoPlanarPhaseCorrection< T >::getEpiPhaseCorrection( "
             "std::vector< T >& wrappedPhase, "
             "const T& bestky ) const" );

}


template class gkg::EchoPlanarPhaseCorrection< float >;
template class gkg::EchoPlanarPhaseCorrection< double >;
