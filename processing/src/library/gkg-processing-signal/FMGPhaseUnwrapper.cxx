#include <gkg-processing-signal/FMGPhaseUnwrapper.h>
#include <gkg-processing-signal/PhaseUnwrapperFactory.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-container/Volume_i.h>
#include <iostream>
#include <iomanip>
#include <cmath>

//#define WFGM_DEBUG

#ifdef WFGM_DEBUG

#include <gkg-processing-io/Writer_i.h>

#endif

RegisterPhaseUnwrapper( gkg, FMGPhaseUnwrapper, float );
RegisterPhaseUnwrapper( gkg, FMGPhaseUnwrapper, double );


#ifdef WFGM_DEBUG

static int32_t count = 0;

template < class T >
inline
void WriteData1( const std::string& name,  const gkg::Volume< T >& volume )
{


  gkg::Volume< T > volumeBis = volume;

  gkg::Writer::getInstance().write( name, volumeBis );

}


template < class T >
inline
void WriteData2( const std::string& operation, const gkg::Volume< T >& volume )
{


  gkg::Volume< T > volumeBis = volume;

  std::string prefix;
  if ( count < 10 )
    prefix = "00";
  else if ( count < 100 )
    prefix = "0";
  else
    prefix = "";
  
  gkg::Writer::getInstance().write( prefix +
                                    gkg::StringConverter::toString( count ) + 
                                    "_" + operation + "_gkg",
                                    volumeBis );
  count++;

}

#endif


template < class T >
gkg::FMGPhaseUnwrapper< T >::FMGPhaseUnwrapper()
                            : gkg::PhaseUnwrapper< T >(),
                               _planeAxis( gkg::FMGPhaseUnwrapper< T >::Z ),
                              _cycleCount( 2 ),
                              _coarsestSize( 3 ),
                              _preSmoothingIterationCount( 2 ),
                              _postSmoothingIterationCount( 2 ),
                              _useCongruence( false )
{
}


template < class T >
gkg::FMGPhaseUnwrapper< T >::~FMGPhaseUnwrapper()
{
}


template < class T >
std::string gkg::FMGPhaseUnwrapper< T >::getName() const
{

  return getStaticName();

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::setPlaneAxis(
                     typename gkg::FMGPhaseUnwrapper< T >::PlaneAxis planeAxis )
{

  try
  {

    _planeAxis = planeAxis;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::setPlaneAxis( "
             "gkg::FMGPhaseUnwrapper< T >::PlaneAxis planeAxis ) const" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::setCycleCount( int32_t count )
{

  try
  {

    if ( count <= 0 )
    {

      throw std::runtime_error( "count must be strictly positive" );

    }
    _cycleCount = count;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::setCycleCount( int count )" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::setCoarsestSize( int32_t size )
{

  try
  {

    if ( size < 3 )
    {

      throw std::runtime_error( "size must be at least equal to 3" );

    }
    _coarsestSize = size;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::setCoarsestSize( int size )" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::setPreSmoothingIterationCount( int32_t count )
{

  try
  {

    if ( count < 0 )
    {

      throw std::runtime_error( "count must be positive" );

    }
    _preSmoothingIterationCount = count;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::setPreSmoothingIterationCount("
             " int count )" );

}


template < class T >
void 
gkg::FMGPhaseUnwrapper< T >::setPostSmoothingIterationCount( int32_t count )
{

  try
  {

    if ( count < 0 )
    {

      throw std::runtime_error( "count must be positive" );

    }
    _postSmoothingIterationCount = count;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::setPostSmoothingIterationCount("
             " int count )" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::useCongruence( bool value )
{

  try
  {

    _useCongruence = value;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::useCongruence( "
             "bool value ) const" );

}


template < class T >
typename gkg::FMGPhaseUnwrapper< T >::PlaneAxis
gkg::FMGPhaseUnwrapper< T >::getPlaneAxis() const
{

  return _planeAxis;

}


template < class T >
int32_t gkg::FMGPhaseUnwrapper< T >::getCycleCount() const
{

  return _cycleCount;

}


template < class T >
int32_t gkg::FMGPhaseUnwrapper< T >::getCoarsestSize() const
{

  return _coarsestSize;

}


template < class T >
int32_t gkg::FMGPhaseUnwrapper< T >::getPreSmoothingIterationCount() const
{

  return _preSmoothingIterationCount;

}


template < class T >
int32_t gkg::FMGPhaseUnwrapper< T >::getPostSmoothingIterationCount() const
{

  return _postSmoothingIterationCount;

}


template < class T >
bool gkg::FMGPhaseUnwrapper< T >::getUseCongruence() const
{

  return _useCongruence;

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::unwrap( gkg::Volume< T >& phase,
                                          const gkg::Volume< T >& /*weights*/,
                                          bool verbose ) const
{

  try
  {

    unwrap( phase, verbose );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::unwrap( "
             "gkg::Volume< T >& phase, "
             "const gkg::Volume< T >& weights, "
             "bool verbose ) const" );

}

template < class T >
void gkg::FMGPhaseUnwrapper< T >::unwrap( gkg::Volume< T >& phase,
                                          bool verbose ) const
{

  try
  {

    if ( phase.getSizeT() != 1 )
    {

      throw std::runtime_error( "phase data has sizeT different from 1" );

    }


    int32_t sizeX = phase.getSizeX();
    int32_t lineCount = phase.getSizeY();
    int32_t sliceCount = phase.getSizeZ();
    int32_t x, line, slice;

    gkg::Volume< T > sliceWrappedPhase;
    gkg::Volume< T > sliceUnwrappedPhase;

    // 
    // unwrapping along Z axis: looping over axial slice(s)
    //

    if ( _planeAxis == gkg::FMGPhaseUnwrapper< T >::Z )
    {

      sliceWrappedPhase.reallocate(  sizeX, lineCount );
      sliceUnwrappedPhase.reallocate(  sizeX, lineCount );

      for ( slice = 0; slice < sliceCount; slice++ )
      {

        // extracting current axial slice
        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            sliceWrappedPhase( x, line ) = ( phase( x, line, slice ) + M_PI ) /
                                           ( 2 * M_PI );

          }

        }


        // initializing unwrapped phase slice from wrapped phase slice
        sliceUnwrappedPhase = sliceWrappedPhase;

        // unwrapping current slice
        unwrapSlice( sliceUnwrappedPhase );

        // making unwrapped phase slice congruent to wrapped phase slice
        if ( _useCongruence )
        {

          congruence( sliceWrappedPhase, sliceUnwrappedPhase );

        }

        // copying back unwrapped slice with offset correction
        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            phase( x, line, slice ) = sliceUnwrappedPhase( x, line ) *
                                      2 * M_PI -
                                      M_PI;

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
    // 
    // unwrapping along Y axis: looping over coronal slice(s)
    //

    else if ( _planeAxis == gkg::FMGPhaseUnwrapper< T >::Y )
    {

      sliceWrappedPhase.reallocate(  sizeX, sliceCount );
      sliceUnwrappedPhase.reallocate(  sizeX, sliceCount );

      for ( line = 0; line < lineCount; line++ )
      {

        // extracting current coronal slice
        for ( slice = 0; slice < sliceCount; slice++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            sliceWrappedPhase( x, slice ) = ( phase( x, line, slice ) + M_PI ) /
                                           ( 2 * M_PI );

          }

        }

        // initializing unwrapped phase slice from wrapped phase slice
        sliceUnwrappedPhase = sliceWrappedPhase;

        // unwrapping current slice
        unwrapSlice( sliceUnwrappedPhase );

        // making unwrapped phase slice congruent to wrapped phase slice
        if ( _useCongruence )
        {

          congruence( sliceWrappedPhase, sliceUnwrappedPhase );

        }

        // copying back unwrapped slice with offset correction
        for ( slice = 0; slice < sliceCount; slice++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            phase( x, line, slice ) = sliceUnwrappedPhase( x, slice ) *
                                      2 * M_PI -
                                      M_PI;

          }

        }


        if ( verbose )
        {

          if ( line != 0 )
          {

            std::cout << gkg::Eraser( 20 );

          }
          std::cout << " line[ " << std::setw( 4 ) << line + 1
                    << " / " << std::setw( 4 ) << lineCount
                    << " ]" << std::flush;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 20 ) << std::flush;

      }

    }
    // 
    // unwrapping along X axis: looping over sagittal slice(s)
    //
    else if ( _planeAxis == gkg::FMGPhaseUnwrapper< T >::X )
    {

      sliceWrappedPhase.reallocate(  lineCount, sliceCount );
      sliceUnwrappedPhase.reallocate(  lineCount, sliceCount );

      for ( x = 0; x < sizeX; x++ )
      {

        // extracting current sagittal slice
        for ( slice = 0; slice < sliceCount; slice++ )
        {

          for ( line = 0; line < lineCount; line++ )
          {

            sliceWrappedPhase( line, slice ) = ( phase( x, line, slice ) +
                                                 M_PI ) / ( 2 * M_PI );

          }

        }

        // initializing unwrapped phase slice from wrapped phase slice
        sliceUnwrappedPhase = sliceWrappedPhase;

        // unwrapping current slice
        unwrapSlice( sliceUnwrappedPhase );

        // making unwrapped phase slice congruent to wrapped phase slice
        if ( _useCongruence )
        {

          congruence( sliceWrappedPhase, sliceUnwrappedPhase );

        }

        // copying back unwrapped slice with offset correction
        for ( slice = 0; slice < sliceCount; slice++ )
        {

          for ( line = 0; line < lineCount; line++ )
          {

            phase( x, line, slice ) = sliceUnwrappedPhase( line, slice ) *
                                      2 * M_PI - M_PI;

          }

        }


        if ( verbose )
        {

          if ( x != 0 )
          {

            std::cout << gkg::Eraser( 17 );

          }
          std::cout << " x[ " << std::setw( 4 ) << x + 1
                    << " / " << std::setw( 4 ) << sizeX
                    << " ]" << std::flush;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 17 ) << std::flush;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::unwrap( "
             "gkg::Volume< T >& phase, bool verbose ) const" );

}


template < class T >
std::string gkg::FMGPhaseUnwrapper< T >::getStaticName()
{

  return "full-multigrid-phase-unwrapper";

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::unwrapSlice(
                                          gkg::Volume< T >& wrappedPhase ) const
{

  try
  {

    int32_t sizeX = wrappedPhase.getSizeX();
    int32_t lineCount = wrappedPhase.getSizeY();
    int32_t x, line;

    // processing X gradient
    gkg::Volume< T > gradientX( sizeX, lineCount );
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        if ( x < sizeX - 1 )
        {

          gradientX( x, line ) = wrapPhase( wrappedPhase( x + 1, line ) -
                                            wrappedPhase( x, line ) );

        }
        else
        {

          // manage boundary
          gradientX( x, line ) = wrapPhase( wrappedPhase( x - 1, line ) -
                                            wrappedPhase( x, line ) );

        }

      }

    }

    // processing Y gradient
    gkg::Volume< T > gradientY( sizeX, lineCount );
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        if ( line < lineCount - 1 )
        {

          gradientY( x, line ) = wrapPhase( wrappedPhase( x, line + 1 ) -
                                            wrappedPhase( x, line ) );

        }
        else
        {

          // manage boundary
          gradientY( x, line ) = wrapPhase( wrappedPhase( x, line - 1 ) -
                                            wrappedPhase( x, line ) );

        }

      }

    }

#ifdef WFGM_DEBUG

    gkg::Volume< T > copyWeights = weights;
    WriteData2( "starting_gradient_x", gradientX );
    WriteData2( "starting_gradient_y", gradientY );

#endif

    // initializing solution to 0.0
    gkg::Volume< T > unwrappedPhase( sizeX, lineCount );
    unwrappedPhase.fill( 0.0 );

    // performing cycles of unwrapping
    int32_t cycle;
    for ( cycle = 0; cycle < _cycleCount; cycle++ )
    {

      fullMultigridVCycle( unwrappedPhase,
                           gradientX, gradientY );

    }


    // copying back unwrapped data
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        wrappedPhase( x, line ) = unwrappedPhase( x, line );

      }

    }

#ifdef WFGM_DEBUG

    WriteData2( "final_unwrapped", wrappedPhase );

#endif

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::unwrapSlice( "
             "gkg::Volume< T >& slicePhase ) const" );

}


template < class T >
T gkg::FMGPhaseUnwrapper< T >::wrapPhase( const T& value ) const
{

  T r = value;
  while ( r > 0.5 )
  {

    r -= 1.0;

  }
  while ( r < -0.5 )
  {

    r += 1.0;

  }
  return r;

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::fullMultigridVCycle(
                                   gkg::Volume< T >& fineUnwrappedPhase,
                                   const gkg::Volume< T >& fineGradientX,
                                   const gkg::Volume< T >& fineGradientY ) const
{

  try
  {

    int32_t fineSizeX = fineGradientX.getSizeX();
    int32_t fineLineCount = fineGradientX.getSizeY();

    int32_t coarseSizeX = fineSizeX / 2;
    int32_t coarseLineCount = fineLineCount / 2;

    if ( ( coarseSizeX >= _coarsestSize ) && 
         ( coarseLineCount >= _coarsestSize ) )
    {

      gkg::Volume< T > coarseGradientX( coarseSizeX, coarseLineCount );
      gkg::Volume< T > coarseGradientY( coarseSizeX, coarseLineCount );
      gkg::Volume< T > coarseUnwrappedPhase( coarseSizeX, coarseLineCount );

      restrict( fineGradientX,
                fineGradientY,
                fineUnwrappedPhase,
                coarseGradientX,
                coarseGradientY );

#ifdef WFGM_DEBUG

      WriteData2( "restrict-coarse_gradient_x", coarseGradientX );
      WriteData2( "restrict-coarse_gradient_y", coarseGradientY );

#endif

      coarseUnwrappedPhase.fill( 0.0 );

      fullMultigridVCycle( coarseUnwrappedPhase,
                           coarseGradientX,
                           coarseGradientY );

#ifdef WFGM_DEBUG

      WriteData2( "full_multigrid_vcycle-coarse_unwrapped_phase",
                  coarseUnwrappedPhase );

#endif

      prolongate( coarseUnwrappedPhase,
                  fineUnwrappedPhase );

#ifdef WFGM_DEBUG

      WriteData2( "prolongate-fine_unwrapped_phase",
                  fineUnwrappedPhase );

#endif

    }
    vCycle( fineUnwrappedPhase,
            fineGradientX,
            fineGradientY );

#ifdef WFGM_DEBUG

    WriteData2( "vcycle-fine_unwrapped_phase",
                fineUnwrappedPhase );

#endif

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::fullMultigridVCycle( "
             "gkg::Volume< T >& unwrappedPhase, "
             "const gkg::Volume< T >& gradientX, "
             "const gkg::Volume< T >& gradientY ) const" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::vCycle(
                                   gkg::Volume< T >& fineUnwrappedPhase,
                                   const gkg::Volume< T >& fineGradientX,
                                   const gkg::Volume< T >& fineGradientY ) const
{

  try
  {

    int32_t fineSizeX = fineGradientX.getSizeX();
    int32_t fineLineCount = fineGradientX.getSizeY();

    int32_t coarseSizeX = fineSizeX / 2;
    int32_t coarseLineCount = fineLineCount / 2;

    if ( ( coarseSizeX >= _coarsestSize ) && 
         ( coarseLineCount >= _coarsestSize ) )
    {

      relax( fineGradientX,
             fineGradientY,
             fineUnwrappedPhase,
             _preSmoothingIterationCount );

#ifdef WFGM_DEBUG

      WriteData2( "relax-fine_unwrapped_phase",
                  fineUnwrappedPhase );

#endif

      gkg::Volume< T > coarseGradientX( coarseSizeX, coarseLineCount );
      gkg::Volume< T > coarseGradientY( coarseSizeX, coarseLineCount );
      gkg::Volume< T > coarseUnwrappedPhase( coarseSizeX, coarseLineCount );

      restrict( fineGradientX,
                fineGradientY,
                fineUnwrappedPhase,
                coarseGradientX,
                coarseGradientY );

#ifdef WFGM_DEBUG

      WriteData2( "restrict-coarse_gradient_x",
                  coarseWeightsX );
      WriteData2( "restrict-coarse_gradient_y",
                  coarseWeightsY );

#endif
      coarseUnwrappedPhase.fill( 0.0 );

      vCycle( coarseUnwrappedPhase,
              coarseGradientX,
              coarseGradientY );

#ifdef WFGM_DEBUG

      WriteData2( "vcycle-coarse_unwrapped_phase",
                  coarseUnwrappedPhase );

#endif

      prolongate( coarseUnwrappedPhase,
                  fineUnwrappedPhase );

#ifdef WFGM_DEBUG

      WriteData2( "prolongate-fine_unwrapped_phase",
                  fineUnwrappedPhase );

#endif

      relax( fineGradientX,
             fineGradientY,
             fineUnwrappedPhase,
             _postSmoothingIterationCount );

#ifdef WFGM_DEBUG

      WriteData2( "relax-fine_unwrapped_phase",
                  fineUnwrappedPhase );

#endif

    }
    else
    {

      relax( fineGradientX,
             fineGradientY,
             fineUnwrappedPhase,
             2 * fineSizeX * fineLineCount );

#ifdef WFGM_DEBUG

      WriteData2( "relax-fine_unwrapped_phase",
                  fineUnwrappedPhase );

#endif

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::vCycle( "
             "gkg::Volume< T >& unwrappedPhase, "
             "const gkg::Volume< T >& gradientX, "
             "const gkg::Volume< T >& gradientY ) const" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::restrict(
                                     const gkg::Volume< T >& fineGradientX,
                                     const gkg::Volume< T >& fineGradientY,
                                     const gkg::Volume< T >& fineUnwrappedPhase,
                                     gkg::Volume< T >& coarseGradientX,
                                     gkg::Volume< T >& coarseGradientY ) const
{

  try
  {

    int32_t fineSizeX = fineGradientX.getSizeX();
    int32_t fineLineCount = fineGradientX.getSizeY();
    int32_t coarseSizeX = coarseGradientX.getSizeX();
    int32_t coarseLineCount = coarseGradientX.getSizeY();
    int32_t x, line, xPrime, linePrime;
    T g1, g2, g3, g4, g5, g6, g7, g8, g9;
    T scale = 0.0; 
    int32_t x1, x2, x3, x4, x5, x6, x7, x8, x9;
    int32_t line1, line2, line3, line4, line5, line6, line7, line8, line9;

    // neighborhood for restriction operator:
    //
    //          N1 | N2 | N3
    //          ---+----+----
    //          N4 | N5 | N6
    //          ---+----+----
    //          N7 | N8 | N9

    // applying restriction to X gradient
    scale = ( ( T )fineSizeX - 1.0 ) / ( ( T )coarseSizeX - 1.0 );
    for ( line = 0; line < coarseLineCount; line++ )
    {

      for ( x = 0; x < coarseSizeX - 1; x++ )
      {

        xPrime = 2 * x;
        linePrime = 2 * line;

        x1 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line1 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x2 = xPrime;
        line2 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x3 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line3 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x4 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line4 = linePrime;

        x5 = xPrime;
        line5 = linePrime;

        x6 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line6 = linePrime;

        x7 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line7 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        x8 = xPrime;
        line8 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        x9 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line9 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        g1 = fineGradientX( x1, line1 ) - ( fineUnwrappedPhase( x2, line2 ) -
                                            fineUnwrappedPhase( x1, line1 ) );
        g2 = fineGradientX( x2, line2 ) - ( fineUnwrappedPhase( x3, line3 ) -
                                            fineUnwrappedPhase( x2, line2 ) );
        g3 = fineGradientX( x3, line3 ) - ( fineUnwrappedPhase( x3+1, line3 ) -
                                            fineUnwrappedPhase( x3, line3 ) );
        g4 = fineGradientX( x4, line4 ) - ( fineUnwrappedPhase( x5, line5 ) -
                                            fineUnwrappedPhase( x4, line4 ) );
        g5 = fineGradientX( x5, line5 ) - ( fineUnwrappedPhase( x6, line6 ) -
                                            fineUnwrappedPhase( x5, line5 ) );
        g6 = fineGradientX( x6, line6 ) - ( fineUnwrappedPhase( x6+1, line6 ) -
                                            fineUnwrappedPhase( x6, line6 ) );
        g7 = fineGradientX( x7, line7 ) - ( fineUnwrappedPhase( x8, line8 ) -
                                            fineUnwrappedPhase( x7, line7 ) );
        g8 = fineGradientX( x8, line8 ) - ( fineUnwrappedPhase( x9, line9 ) -
                                            fineUnwrappedPhase( x8, line8 ) );
        g9 = fineGradientX( x9, line9 ) - ( fineUnwrappedPhase( x9+1, line9 ) -
                                            fineUnwrappedPhase( x9, line9 ) );

        coarseGradientX( x, line ) = 0.25 * g5 +
                                     0.125 * ( g2 + g4 + g6 + g8 ) +
                                     0.0625 * ( g1 + g3 + g7 + g9 );
        coarseGradientX( x, line ) *= scale;

      }

    }

    for ( line = 0; line < coarseLineCount; line++ )
    {

      coarseGradientX( coarseSizeX - 1, line ) =
        -coarseGradientX( coarseSizeX - 2, line );

    }

    // applying restriction to Y gradient
    scale = ( ( T )fineLineCount - 1.0 ) / ( ( T )coarseLineCount - 1.0 );
    for ( line = 0; line < coarseLineCount - 1; line++ )
    {

      for ( x = 0; x < coarseSizeX; x++ )
      {

        xPrime = 2 * x;
        linePrime = 2 * line;

        x1 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line1 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x2 = xPrime;
        line2 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x3 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line3 = linePrime + ( linePrime > 0 ? -1 : +1 );

        x4 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line4 = linePrime;

        x5 = xPrime;
        line5 = linePrime;

        x6 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line6 = linePrime;

        x7 = xPrime + ( xPrime > 0 ? -1 : +1 );
        line7 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        x8 = xPrime;
        line8 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        x9 = xPrime + ( xPrime < fineSizeX - 1 ? +1 : -1 );
        line9 = linePrime + ( linePrime < fineLineCount - 1 ? +1 : -1 );

        g1 = fineGradientY( x1, line1 ) - ( fineUnwrappedPhase( x4, line4 ) -
                                            fineUnwrappedPhase( x1, line1 ) );
        g2 = fineGradientY( x2, line2 ) - ( fineUnwrappedPhase( x5, line5 ) -
                                            fineUnwrappedPhase( x2, line2 ) );
        g3 = fineGradientY( x3, line3 ) - ( fineUnwrappedPhase( x6, line6 ) -
                                            fineUnwrappedPhase( x3, line3 ) );
        g4 = fineGradientY( x4, line4 ) - ( fineUnwrappedPhase( x7, line7 ) -
                                            fineUnwrappedPhase( x4, line4 ) );
        g5 = fineGradientY( x5, line5 ) - ( fineUnwrappedPhase( x8, line8 ) -
                                            fineUnwrappedPhase( x5, line5 ) );
        g6 = fineGradientY( x6, line6 ) - ( fineUnwrappedPhase( x9, line9 ) -
                                            fineUnwrappedPhase( x6, line6 ) );
        g7 = fineGradientY( x7, line7 ) - ( fineUnwrappedPhase( x7, line7+1 ) -
                                            fineUnwrappedPhase( x7, line7 ) );
        g8 = fineGradientY( x8, line8 ) - ( fineUnwrappedPhase( x8, line8+1 ) -
                                            fineUnwrappedPhase( x8, line8 ) );
        g9 = fineGradientY( x9, line9 ) - ( fineUnwrappedPhase( x9, line9+1 ) -
                                            fineUnwrappedPhase( x9, line9 ) );

        coarseGradientY( x, line ) = 0.25 * g5 +
                                     0.125 * ( g2 + g4 + g6 + g8 ) +
                                     0.0625 * ( g1 + g3 + g7 + g9 );
        coarseGradientY( x, line ) *= scale;

      }

    }

    for ( x = 0; x < coarseSizeX; x++ )
    {

      coarseGradientY( x, coarseLineCount - 1 ) =
        -coarseGradientY( x, coarseLineCount - 2 );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::restrict( "
             "const gkg::Volume< T >& fineGradientX, "
             "const gkg::Volume< T >& fineGradientY, "
             "const gkg::Volume< T >& fineUnwrappedPhase, "
             "gkg::Volume< T >& coarseGradientX, "
             "gkg::Volume< T >& coarseGradientY ) const" );

}
 

template < class T >
void gkg::FMGPhaseUnwrapper< T >::prolongate(
                                   const gkg::Volume< T >& coarseUnwrappedPhase,
                                   gkg::Volume< T >& fineUnwrappedPhase ) const
{

  
  try
  {

    int32_t coarseSizeX = coarseUnwrappedPhase.getSizeX();
    int32_t coarseLineCount = coarseUnwrappedPhase.getSizeY();
    int32_t fineSizeX = fineUnwrappedPhase.getSizeX();
    int32_t fineLineCount = fineUnwrappedPhase.getSizeY();
    int32_t x, line, fineX, fineLine, xPrime, linePrime;
    int32_t x1, x2, x3, x4;
    int32_t line1, line2, line3, line4;
    T p1, p2, p3, p4;

    for ( line = 0; line < coarseLineCount; line++ )
    {

      for ( x = 0; x < coarseSizeX; x++ )
      {

        fineX = 2 * x;
        fineLine = 2 * line;

        x1 = x;
        line1 = line;

        x2 = x + ( x < coarseSizeX - 1 ? 1 : 0 );
        line2 = line;

        x3 = x;
        line3 = line + ( line < coarseLineCount - 1 ? 1 : 0 );

        x4 = x + ( x < coarseSizeX - 1 ? 1 : 0 );
        line4 = line + ( line < coarseLineCount - 1 ? 1 : 0 );

        p1 = coarseUnwrappedPhase( x1, line1 );
        p2 = 0.5 * ( coarseUnwrappedPhase( x1, line1 ) +
                     coarseUnwrappedPhase( x2, line2 ) );
        p3 = 0.5 * ( coarseUnwrappedPhase( x1, line1 ) +
                     coarseUnwrappedPhase( x3, line3 ) );
        p4 = 0.25 * ( coarseUnwrappedPhase( x1, line1 ) +
                      coarseUnwrappedPhase( x2, line2 ) +
                      coarseUnwrappedPhase( x3, line3 ) +
                      coarseUnwrappedPhase( x4, line4 ) );

        fineUnwrappedPhase( fineX, fineLine ) += p1;
        fineUnwrappedPhase( fineX + 1, fineLine ) += p2;
        fineUnwrappedPhase( fineX, fineLine + 1 ) += p3;
        fineUnwrappedPhase( fineX + 1, fineLine + 1 ) += p4;

        // fill in extra column when fineSizeX is odd
        if ( x == coarseSizeX - 1 )
        {

          for ( xPrime = 2 * coarseSizeX; xPrime < fineSizeX; xPrime++ )
          {

            fineUnwrappedPhase( xPrime, fineLine ) += p2;
            fineUnwrappedPhase( xPrime, fineLine + 1 ) += p4;

          }

        }

        // fill in extra column when fineLineCount is odd
        if ( line == coarseLineCount - 1 )
        {

          for ( linePrime = 2 * coarseLineCount; linePrime < fineLineCount;
                linePrime++ )
          {

            fineUnwrappedPhase( fineX, linePrime ) += p3;
            fineUnwrappedPhase( fineX + 1, linePrime ) += p4;

          }

        }

        // fill in lower right-hand corner
        if ( ( x == coarseSizeX - 1 ) && ( line == coarseLineCount - 1 ) )
        {

          for ( linePrime = 2 * coarseLineCount; linePrime < fineLineCount;
                linePrime++ )
          {

            for ( xPrime = 2 * coarseSizeX; xPrime < fineSizeX; xPrime++ )
            {

              fineUnwrappedPhase( xPrime, linePrime ) += p4;

            }

          }

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::prolongate( "
             "const gkg::Volume< T >& coarseUnwrappedPhase, "
             "gkg::Volume< T >& fineUnwrappedPhase ) const" );


}


// Black-red Gauss-Seidel relaxation: optimization of the functional:
// f(i,j) = 0.25*( f(i-1,j) + f(i+1,j) + f(i,j-1) + f(i,j+1) ) - 0.25*rho(i,j)
//  with
// rho(i,j) = ( dx(i+1,j) - dx(i,j) ) + ( dy(i,j+1) - dy(i,j) )
template < class T >
void gkg::FMGPhaseUnwrapper< T >::relax( const gkg::Volume< T >& gradientX,
                                         const gkg::Volume< T >& gradientY,
                                         gkg::Volume< T >& unwrappedPhase,
                                         int32_t iterationCount ) const
{

  try
  {

    int32_t sizeX = unwrappedPhase.getSizeX();
    int32_t lineCount = unwrappedPhase.getSizeY();
    int32_t x, line, iteration, pass, start;
    int32_t x1, x2, x3, x4;
    int32_t line1, line2, line3, line4;
    T gx, gy, rho;

    if ( ( sizeX * lineCount < 64 ) && ( iterationCount < 2 ) )
    {

      iterationCount = 2 * ( sizeX + lineCount );

    }

    for ( iteration = 0; iteration < iterationCount; iteration++ )
    {

      for ( pass = 0; pass < 2; pass++ )
      {

        start = pass;

        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = start; x < sizeX; x += 2 )
          {

            x1 = x + ( x > 0 ? -1 : +1 );
            line1 = line;

            x2 = x + ( x < sizeX - 1 ? +1 : -1 );
            line2 = line;

            x3 = x;
            line3 = line + ( line >0 ? -1 : +1 );

            x4 = x;
            line4 = line + ( line < lineCount - 1 ? +1 : -1 );

            // gradient derivatives
            gx = ( x > 0 ? gradientX( x - 1, line ) : -gradientX( x, line ) );
            gy = ( line > 0 ? gradientY( x, line- 1 ) : -gradientY( x, line ) );

            rho = ( gradientX( x, line ) - gx ) +
                  ( gradientY( x, line ) - gy );

            unwrappedPhase( x, line ) = 0.25 * ( unwrappedPhase( x1, line1 ) +
                                                 unwrappedPhase( x2, line2 ) +
                                                 unwrappedPhase( x3, line3 ) +
                                                 unwrappedPhase( x4, line4 ) -
                                                 rho );                         

          }
          start = ( start ? 0 : 1 );

        }

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::relax( "
             "const gkg::Volume< T >& gradientX, "
             "const gkg::Volume< T >& gradientY, "
             "gkg::Volume< T >& unwrappedPhase, "
             "int iterationCount ) const" );

}


template < class T >
void gkg::FMGPhaseUnwrapper< T >::congruence(
                                   const gkg::Volume< T >& sliceWrappedPhase,
                                   gkg::Volume< T >& sliceUnwrappedPhase ) const
{

  try
  {

    int32_t sizeX = sliceWrappedPhase.getSizeX();
    int32_t lineCount = sliceWrappedPhase.getSizeY();

    int32_t x, line;

    T rmin = sliceUnwrappedPhase( 0, 0 );
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        if ( rmin > sliceUnwrappedPhase( x, line ) )
        {

          rmin = sliceUnwrappedPhase( x, line );

        }

      }

    }

    int32_t npos = ( int32_t )( rmin < 0 ? -rmin + 2.0 : 0.0 );

    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        sliceUnwrappedPhase( x, line ) += npos;

      }

    }

    // finding best addition offset that minimizes discontinuities
    gkg::Volume< T > tmp( sizeX, lineCount );

    int32_t iteration;
    int32_t n, rCountMinimum = 4000000;
    T rr = 0, r = 0, rrMinimum = 0;
    for ( iteration = 0; iteration <= 10; iteration++ )
    {

      rr = 0.1 * iteration;

      // creating temporary surface by adding a constant rr
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          n = ( int32_t )( sliceUnwrappedPhase( x, line ) + rr );
          r = ( sliceUnwrappedPhase( x, line ) + rr ) - n -
              sliceWrappedPhase( x, line );
          if ( r < -0.5 )
          {

            tmp( x, line ) = n - 1 + sliceWrappedPhase( x, line );

          }
          else if ( r > 0.5 )
          {

            tmp( x, line ) = n + 1 + sliceWrappedPhase( x, line );

          }
          else
          {

            tmp( x, line ) = n + sliceWrappedPhase( x, line );

          }

        }

      }

      // measure discontinuities in the temporary phase
      int32_t rCount = 0;
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          if ( x > 0 )
          {

            r = tmp( x, line ) - tmp( x - 1, line );
            if ( ( r > 0.5 ) || ( r < -0.5 ) )
            {

              ++ rCount;

            }

          }          

          if ( line > 0 )
          {

            r = tmp( x, line ) - tmp( x, line - 1 );
            if ( ( r > 0.5 ) || ( r < -0.5 ) )
            {

              ++ rCount;

            }

          }          

        }

      }
      if ( ( iteration == 0 ) || ( rCount < rCountMinimum ) )
      {

        rCountMinimum = rCount;
        rrMinimum = rr;

      }

    }

    // adding final offset to phase slice
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        sliceUnwrappedPhase( x, line ) += rrMinimum;

        n = ( int32_t )sliceUnwrappedPhase( x, line );
        r = sliceUnwrappedPhase( x, line ) - n - sliceWrappedPhase( x, line );
        if ( r < -0.5 )
        {

          sliceUnwrappedPhase( x, line ) = n - 1 + sliceWrappedPhase( x, line );

        }
        else if ( r > 0.5 )
        {

          sliceUnwrappedPhase( x, line ) = n + 1 + sliceWrappedPhase( x, line );

        }
        else
        {

          sliceUnwrappedPhase( x, line ) = n + sliceWrappedPhase( x, line );

        }

        sliceUnwrappedPhase( x, line ) -= npos;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::FMGPhaseUnwrapper< T >::congruence( "
             "const gkg::Volume< T >& sliceWrappedPhase, "
             "gkg::Volume< T >& sliceUnwrappedPhase ) const" );


}


template class gkg::FMGPhaseUnwrapper< float >;
template class gkg::FMGPhaseUnwrapper< double >;
