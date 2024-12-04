#include <gkg-mri-reconstruction-correction/RegridingCorrection.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Eraser.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>


#define NEIGHBORING_SIZE 4


template < class T >
gkg::RegridingCorrection< T >::RegridingCorrection( int32_t inSizeX )
{

  try
  {

    std::vector< T > inTrajectory( inSizeX );
    int32_t p;
    for ( p = 0; p < inSizeX; p++ )
    {

      inTrajectory[ p ] = ( T )p;

    }

    std::list< std::vector< T > > inTrajectories;
    inTrajectories.push_back( inTrajectory );

    initializeIn( inTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RegridingCorrection< T >::RegridingCorrection( "
             "int inSizeX )" );

}


template < class T >
gkg::RegridingCorrection< T >::RegridingCorrection(
                                          const std::vector< T >& inTrajectory )
{

  try
  {

    std::list< std::vector< T > > inTrajectories;
    inTrajectories.push_back( inTrajectory );

    initializeIn( inTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RegridingCorrection< T >::RegridingCorrection( "
             "const std::vector< T >& inTrajectory ) " );

}


template < class T >
gkg::RegridingCorrection< T >::RegridingCorrection(
                           const std::list< std::vector< T > >& inTrajectories )
{

  try
  {

    initializeIn( inTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "gkg::RegridingCorrection< T >::RegridingCorrection( "
             "const std::list< std::vector< T > >& inTrajectories ) " );

}


template < class T >
gkg::RegridingCorrection< T >::~RegridingCorrection()
{
}


template < class T >
int32_t gkg::RegridingCorrection< T >::getTrajectoryCount() const
{

  return ( int32_t )_effectiveSize.size();

}


template < class T >
T gkg::RegridingCorrection< T >::getEffectiveSize( int32_t trajectory ) const
{

  try
  {

    if ( ( trajectory < 0 ) || 
         ( trajectory >= ( int32_t )_effectiveSize.size() ) )
    {

      throw std::runtime_error( "bad trajectory index" );
      return ( T )0;

    }
    return _effectiveSize[ trajectory ];

  }
  GKG_CATCH( "template < class T > "
             "T gkg::RegridingCorrection< T >::getEffectiveSize( "
             "int trajectory ) const" );

}


template < class T >
T gkg::RegridingCorrection< T >::getLongestSize() const
{

  T longestSize = _effectiveSize[ 0 ];
  int32_t t;
  for ( t = 1; t < ( int32_t )_effectiveSize.size(); t++ )
  {

    if ( _effectiveSize[ t ] > longestSize )
    {

      longestSize = _effectiveSize[ t ];

    }

  }
  return longestSize;

}


template < class T >
void gkg::RegridingCorrection< T >::setOutputSampleCount( int32_t outSizeX,
                                                          const T& newEffSize )
{

  try
  {

    T longestSize = getLongestSize();
    int32_t newEffSizeInt;
    int32_t p, t;
    int32_t trajectoryCount = _inKSpace.getSizeY();

    if ( newEffSize > 0.0 )
    {

      newEffSizeInt = ( int32_t )( newEffSize + 0.5 );

    }
    else
    {

      newEffSizeInt = outSizeX;

    }

    int32_t minSizeX = std::min( outSizeX, newEffSizeInt );
    int32_t maxSizeX = std::max( outSizeX, newEffSizeInt );

    std::vector< T > outTrajectory( maxSizeX );
    for ( p = 0; p < minSizeX; p++ )
    {

      outTrajectory[ p ] = ( T )p * ( T )longestSize / ( newEffSize - 1.0 );

    }
    for ( p = minSizeX; p < maxSizeX; p++ )
    {

      outTrajectory[ p ] = 0.0;

    }

    std::list< std::vector< T > > outTrajectories;
    for ( t = 0; t < trajectoryCount; t++ )
    {

      outTrajectories.push_back( outTrajectory );

    }
    initializeOut( outTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::setOutputSampleCount( "
             "int outSizeX, "
             "const T& newEffSize ) " );

}


template < class T >
void gkg::RegridingCorrection< T >::setOutputTrajectory(
                                         const std::vector< T >& outTrajectory )
{

  try
  {

    int32_t t;
    int32_t trajectoryCount = _inKSpace.getSizeY();

    std::list< std::vector< T > > outTrajectories;
    for ( t = 0; t < trajectoryCount; t++ )
    {

      outTrajectories.push_back( outTrajectory );

    }
    initializeOut( outTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::setOutputTrajectory( "
             "const std::vector< T >& outTrajectory ) " );

}


template < class T >
void gkg::RegridingCorrection< T >::setOutputTrajectories(
                          const std::list< std::vector< T > >& outTrajectories )
{

  try
  {

    initializeOut( outTrajectories );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::setOutputTrajectories( "
             "const std::list< std::vector< T > >& outTrajectories ) " );

}


template < class T >
void gkg::RegridingCorrection< T >::apply(
                                     const std::vector< std::complex< T > >& in,
                                     std::vector< std::complex< T > >& out,
                                     int32_t trajectory ) const
{

  try
  {

    int32_t inSizeX = ( int32_t )in.size();
    int32_t outSizeX = ( int32_t )out.size();

    // sanity checks
    if ( inSizeX != _weighting.getSizeX() )
    {

      throw std::runtime_error(
                 "inconsistency between trajectories X size and input X size" );

    }

    if ( outSizeX != _weighting.getSizeY() )
    {

      throw std::runtime_error(
                "inconsistency between given output size and out buffer size" );

    }

    // regriding
    int32_t ki, ko;

    for ( ko = 0; ko < outSizeX; ko ++ )
    {

      out[ ko ] = std::complex< T >( 0.0 );
      for ( ki = _left( ko, trajectory ); ki < _right( ko, trajectory ); ki ++ )
      {

        out[ ko ] += _weighting( ki, ko, trajectory ) * in[ ki ];

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::apply( "
             "const std::vector< std::complex< T > >& in, "
             "std::vector< std::complex< T > >& out, "
             "int trajectory ) const" );

}


template < class T >
void gkg::RegridingCorrection< T >::apply(
                               const gkg::Volume< std::complex< T > >& volumeIn,
                               gkg::Volume< std::complex< T > >& volumeOut,
                               const std::vector< int32_t >& trajectories,
                               bool verbose ) const
{

  try
  {


    // sanity checks
    if ( volumeIn.getSizeT() != 1 )
    {

      throw std::runtime_error( "input volume has sizeT different from 1" );

    }

    if ( ( int32_t )trajectories.size() != volumeIn.getSizeY() )
    {

      throw std::runtime_error( "trajectory size inconsistency" );

    }

    int32_t sizeX = volumeIn.getSizeX();
    int32_t lineCount = volumeIn.getSizeY();
    int32_t sliceCount = volumeIn.getSizeZ();

    int32_t regridingSizeX = _weighting.getSizeY();

    std::vector< std::complex< T > > in( sizeX );
    std::vector< std::complex< T > > out( regridingSizeX );

    int32_t x, line, slice;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      // duplicating the current slice
      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          in[ x ] = volumeIn( x, line, slice );

        }
        apply( in, out, trajectories[ line ] );
        for ( x = 0; x < regridingSizeX; x++ )
        {

          volumeOut( x, line, slice ) = out[ x ];

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
             "void gkg::RegridingCorrection< T >::apply( "
             "const gkg::Volume< std::complex< T > >& in, "
             "gkg::Volume< std::complex< T > >& out, "
             "const std::vector< int >& trajectories, "
             "bool verbose ) const" );

}


template < class T >
void gkg::RegridingCorrection< T >::apply(
                               const gkg::Volume< std::complex< T > >& volumeIn,
                               gkg::Volume< std::complex< T > >& volumeOut,
                               bool verbose ) const
{

  try
  {

    int32_t t;
    int32_t trajectoryCount = volumeIn.getSizeY();
    std::vector< int32_t > trajectories( trajectoryCount );
    for ( t = 0; t < trajectoryCount; t++ )
    {

      trajectories[ t ] = 0;

    }
    apply( volumeIn, volumeOut, trajectories, verbose );

  }

  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::apply( "
             "const gkg::Volume< std::complex< T > >& in, "
             "gkg::Volume< std::complex< T > >& out, "
             "bool verbose ) const" );

}


template < class T >
void gkg::RegridingCorrection< T >::initializeIn(
                           const std::list< std::vector< T > >& inTrajectories )
{


  try
  {

    int32_t ki, t;
    int32_t inTrajectoryCount = ( int32_t )inTrajectories.size();

    if ( inTrajectoryCount < 1 )
    {

      throw std::runtime_error( "need at least one input trajectory" );

    }

    typename std::list< std::vector< T > >::const_iterator
      it = inTrajectories.begin(),
      ite = inTrajectories.end();
    int32_t inSizeX = ( int32_t )it->size();
    if ( inSizeX < 16 )
    {

      throw std::runtime_error( "not enough input trajectory samples (<16)" );

    }
    while ( it != ite )
    {

      if ( ( int32_t )it->size() != inSizeX )
      {

        throw std::runtime_error( 
                              "inconsistent input trajectory samples size(s)" );

      }
      ++ it;

    }
    
    // input normalization factor
    T inDeltaK = std::fabs( inTrajectories.front()[ inSizeX / 2 - 5 ] -
                            inTrajectories.front()[ inSizeX / 2 + 5 ] ) /
                 ( T )10.0;
    if ( inDeltaK == 0.0 )
    {

      inDeltaK = ( T )1.0;

    }

    // calculation of normalized input k-space values
    _effectiveSize = std::vector< T >( inTrajectoryCount );
    _inKSpace.reallocate( inSizeX, inTrajectoryCount );
    t = 0;
    it = inTrajectories.begin();
    while ( it != ite )
    {

      for ( ki = 0; ki < inSizeX; ki++ )
      {

        _inKSpace( ki, t ) = ( ( *it )[ ki ] - ( *it )[ 0 ] ) / inDeltaK;

      }
      _effectiveSize[ t ] = _inKSpace( inSizeX - 1, t );
      ++ it;
      ++ t;

    }

    // shifting input k-space coordinates such that top flat values are not
    // regrided
    for ( t = 0; t < inTrajectoryCount; t++ )
    {

      inDeltaK = std::ceil( _inKSpace( inSizeX / 2, t ) ) -
                 _inKSpace( inSizeX / 2, t );
      for ( ki = 0; ki < inSizeX;  ki++ )
      {

        _inKSpace( ki, t ) += inDeltaK;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::initializeIn( "
             "const std::list< std::vector< T > >& inTrajectories ) " );

}


template < class T >
void gkg::RegridingCorrection< T >::initializeOut(
                          const std::list< std::vector< T > >& outTrajectories )
{


  try
  {

    int32_t ki, ko, t;
    int32_t inSizeX = _inKSpace.getSizeX();
    int32_t inTrajectoryCount = _inKSpace.getSizeY();
    int32_t outTrajectoryCount = ( int32_t )outTrajectories.size();

    // sanity checks
    if ( inTrajectoryCount != outTrajectoryCount )
    {

      throw std::runtime_error( "there should be as much output trajectories "
                                "as input trajectories" );

    }

    typename std::list< std::vector< T > >::const_iterator
      ot = outTrajectories.begin(),
      ote = outTrajectories.end();
    int32_t outSizeX = ( int32_t )ot->size();
    if ( outSizeX < 16 )
    {

      throw std::runtime_error( "not enough output trajectory samples (<16)" );

    }
    while ( ot != ote )
    {

      if ( ( int32_t )ot->size() != outSizeX )
      {

        throw std::runtime_error( 
                             "inconsistent output trajectory samples size(s)" );

      }
      ++ ot;

    }


    // common input and output trajectory count
    int32_t trajectoryCount = inTrajectoryCount;    

    // calculation of normalized output k-space values
    _outKSpace.reallocate( outSizeX, trajectoryCount );
    t = 0;
    ot = outTrajectories.begin();
    while ( ot != ote )
    {

      for ( ko = 0; ko < outSizeX; ko++ )
      {

        _outKSpace( ko, t ) = ( ( *ot )[ ko ] - ( *ot )[ 0 ] );

      }
      ++ ot;
      ++ t;

    }

    // processing reinterpolation matrix
    _weighting.reallocate( inSizeX, outSizeX, trajectoryCount );
    for ( t = 0; t < trajectoryCount; t++ )
    {

      for ( ko = 0; ko < outSizeX;  ko++ )
      {

        _weighting( 0, ko, t ) = ( T )0;
        for ( ki = 1; ki < inSizeX;  ki++ )
        {

          _weighting( ki, ko, t ) = kernel( _inKSpace( ki, t ) - 
                                            _outKSpace( ko, t ) ) *
                                    ( _inKSpace( ki, t ) -
                                      _inKSpace( ki - 1, t ) );

        }

      }

    }

    // convolution range to avoid multiplication by zero(s)
    _left.reallocate( outSizeX, trajectoryCount );
    _right.reallocate( outSizeX, trajectoryCount );
    for ( t = 0; t < trajectoryCount; t++ )
    {

      for ( ko = 0; ko < outSizeX;  ko++ )
      {

        // left side
        _left( ko, t ) = 0;
        while ( ( std::fabs( _weighting( _left( ko, t ), ko, t ) ) < 0.001 ) &&
                ( _left( ko, t ) < inSizeX - 1 ) )
        {

          ++ _left( ko, t );

        }

        // right side
        _right( ko, t ) = inSizeX - 1;
        while ( ( std::fabs( _weighting( _right( ko, t ), ko, t ) ) < 0.001 ) &&
                ( _right( ko, t ) > 0 ) )
        {

          -- _right( ko, t );

        }


      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RegridingCorrection< T >::initializeOut( "
             "const std::list< std::vector< T > >& outTrajectories ) " );

}


template < class T >
T gkg::RegridingCorrection< T >::kernel( const T& value ) const
{

  // anti-folding
  T antifoldingValue = value * 0.9;

  if ( value == 0 )
  {

    return 1.0;

  }
  else if ( std::fabs( antifoldingValue ) <= NEIGHBORING_SIZE )
  {

    return ( std::sin( M_PI * antifoldingValue ) /
             ( M_PI * antifoldingValue ) ) * 0.5 *
           ( 1 + std::cos( M_PI * antifoldingValue / ( T )NEIGHBORING_SIZE ) );

  }
  else
  {

    return 0.0;

  }

}


template class gkg::RegridingCorrection< float >;
template class gkg::RegridingCorrection< double >;


#undef NEIGHBORING_SIZE
