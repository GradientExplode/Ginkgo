#include <gkg-processing-resampling/Resampler.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>



template < class T >
gkg::Resampler< T >::Resampler()
{
}


template < class T >
gkg::Resampler< T >::~Resampler()
{
}


template < class T >
void 
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,     
                               const gkg::Transform3d< float >& transform3d,
                               const T& outBackground,
                               gkg::Volume< T >& outVolume,
                               bool verbose )
{

  try
  {

    if ( inVolume.getSizeT() != 1 )
    {

      throw std::runtime_error(  "input volume must have a T size equal to 1" );

    }

    if ( outVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "output volume must have a T size equal to 1" );

    }

    gkg::Vector3d< double > inResolution;
    inVolume.getHeader().getAttribute( "resolutionX", inResolution.x );
    inVolume.getHeader().getAttribute( "resolutionY", inResolution.y );
    inVolume.getHeader().getAttribute( "resolutionZ", inResolution.z );

    gkg::Vector3d< double > outResolution;
    outVolume.getHeader().getAttribute( "resolutionX", outResolution.x );
    outVolume.getHeader().getAttribute( "resolutionY", outResolution.y );
    outVolume.getHeader().getAttribute( "resolutionZ", outResolution.z );

    gkg::CompositeTransform3d< float > normTransform3d;

    gkg::RCPointer< gkg::Scaling3d< float > > scaling;
    scaling.reset( new gkg::Scaling3d< float >( ( float )inResolution.x,
                                                ( float )inResolution.y,
                                                ( float )inResolution.z ) );
    if ( !scaling.isNull() )
    {

      normTransform3d.compose( scaling, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate Scaling3d instance" );

    }

    gkg::RCPointer< gkg::Transform3d< float > > cloneTransform3d =
                                                            transform3d.clone();

    if ( !cloneTransform3d.isNull() )
    {

      normTransform3d.compose( cloneTransform3d, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate cloneTransform3d instance" );

    }

    outVolume.fill( outBackground );

#ifdef RESAMPLER_USE_THREADED_LOOP
    gkg::ResamplerContext< T, float > context( this, inVolume, normTransform3d,
                                               outBackground, outVolume,
                                               outResolution, verbose );
    int32_t maxThreadCount = ( inVolume.isMemoryMapped() ||
                               outVolume.isMemoryMapped() ) ? 1 : 0;

    gkg::ThreadedLoop< int32_t > threadedLoop(
        &context,
        0,                     // the start index of the global loop
        outVolume.getSizeZ(),  // the count to be done
        maxThreadCount,        // max number of threads (0 means one per CPU)
        1,                     // number of threads launched on each CPU
        false );               // verbose mode

    threadedLoop.launch( true, true );

#else

    int32_t outSizeX = outVolume.getSizeX();
    int32_t outSizeY = outVolume.getSizeY();
    int32_t outSizeZ = outVolume.getSizeZ();

    typename gkg::Volume< T >::iterator o = outVolume.begin();
    gkg::Vector3d< float > outLoc( 0.0, 0.0, 0.0 );
    int32_t x, y, z;
    for ( z = 0; z < outSizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 16 );

        }
        std::cout << "z[ " << std::setw( 4 ) << z + 1
                  << " / " << std::setw( 4 ) << outSizeZ
                  << " ]" << std::flush;

      }
      for ( y = 0; y < outSizeY; y++ )
      {

        for ( x = 0; x < outSizeX; x++ )
        {

          doResample( inVolume, normTransform3d, outBackground, outLoc, *o );
          ++ o;
          outLoc.x += ( float )outResolution.x;

        }
        outLoc.y += ( float )outResolution.y;
        outLoc.x = 0.0;

      }
      outLoc.z += ( float )outResolution.z;
      outLoc.y = 0.0;

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 16 );

    }

#endif

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "     
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "bool verbose )" );

}

#ifdef RESAMPLER_USE_THREADED_LOOP

template < class T >
void 
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,     
                               const gkg::Transform3d< float >& transform3d,
                               const T& outBackground,
                               gkg::Volume< T >& outVolume,
                               gkg::Vector3d< double >& outResolution,
                               int32_t zStartIndex,
                               int32_t zCount,
                               bool /* verbose */ )
{

  try
  {

    const int32_t outSizeX = outVolume.getSizeX();
    const int32_t outSizeY = outVolume.getSizeY();

    int32_t zStopIndex = zStartIndex + zCount;
    gkg::Vector3d< float > outLoc( 0.0, 0.0, 0.0 );

    // Use a for loop to compute outLoc.z instead of multiplication
    // Reason: slight difference using double multiplication,
    // and we wanted to compare results with previous implementation
    // using for loop in z...
    for ( int32_t z = 0; z < zStartIndex; z++ )
    {

      outLoc.z += outResolution.z;

    }

    int32_t x, y, z;
    for ( z = zStartIndex; z < zStopIndex; z++ )
    {

      for ( y = 0; y < outSizeY; y++ )
      {

        for ( x = 0; x < outSizeX; x++ )
        {

          doResample( inVolume, transform3d, outBackground, outLoc, 
	              outVolume( x, y, z ) );
          outLoc.x += ( float )outResolution.x;

        }
        outLoc.y += ( float )outResolution.y;
        outLoc.x = 0.0;

      }
      outLoc.z += ( float )outResolution.z;
      outLoc.y = 0.0;

    }

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "int32_t zStartIndex, int32_t zCount, "
             "bool verbose )" );

}

#endif


template < class T >
void
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,
                               const gkg::Transform3d< double >& transform3d,
                               const T& outBackground,
                               gkg::Volume< T >& outVolume,
                               bool verbose )
{

  try
  {

    if ( inVolume.getSizeT() != 1 )
    {

      throw std::runtime_error(  "input volume must have a T size equal to 1" );

    }

    if ( outVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "output volume must have a T size equal to 1" );

    }

    gkg::Vector3d< double > inResolution;
    inVolume.getHeader().getAttribute( "resolutionX", inResolution.x );
    inVolume.getHeader().getAttribute( "resolutionY", inResolution.y );
    inVolume.getHeader().getAttribute( "resolutionZ", inResolution.z );

    gkg::Vector3d< double > outResolution;
    outVolume.getHeader().getAttribute( "resolutionX", outResolution.x );
    outVolume.getHeader().getAttribute( "resolutionY", outResolution.y );
    outVolume.getHeader().getAttribute( "resolutionZ", outResolution.z );

    gkg::CompositeTransform3d< double > normTransform3d;

    gkg::RCPointer< gkg::Scaling3d< double > > scaling;
    scaling.reset( new gkg::Scaling3d< double >( inResolution ) );

    if ( !scaling.isNull() )
    {

      normTransform3d.compose( scaling, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate Scaling3d instance" );

    }

    gkg::RCPointer< gkg::Transform3d< double > > cloneTransform3d =
                                                            transform3d.clone();

    if ( !cloneTransform3d.isNull() )
    {

      normTransform3d.compose( cloneTransform3d, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate cloneTransform3d instance" );

    }

    outVolume.fill( outBackground );


#ifdef RESAMPLER_USE_THREADED_LOOP
    gkg::ResamplerContext< T, double > context( this, inVolume, normTransform3d,
                                        outBackground, outVolume, outResolution,
                                        verbose );
    int32_t maxThreadCount = ( inVolume.isMemoryMapped() ||
                               outVolume.isMemoryMapped() ) ? 1 : 0;

    gkg::ThreadedLoop< int32_t > threadedLoop(
        &context,
        0,                    // the start index of the global loop
        outVolume.getSizeZ(), // the count to be done
        maxThreadCount,       // max number of threads (0 means one per CPU)
        1,                    // number of threads launched on each CPU
        false );              // verbose mode

    threadedLoop.launch( true, true );

#else
  
    int32_t outSizeX = outVolume.getSizeX();
    int32_t outSizeY = outVolume.getSizeY();
    int32_t outSizeZ = outVolume.getSizeZ();

    typename gkg::Volume< T >::iterator o = outVolume.begin();
    gkg::Vector3d< double > outLoc( 0.0, 0.0, 0.0 );
    int32_t x, y, z;
    for ( z = 0; z < outSizeZ; z++ )
    {

      if ( verbose )
      {

        if ( z != 0 )
        {

          std::cout << gkg::Eraser( 16 );

        }
        std::cout << "z[ " << std::setw( 4 ) << z + 1 
                  << " / " << std::setw( 4 ) << outSizeZ
                  << " ]" << std::flush;

      }
      for ( y = 0; y < outSizeY; y++ )
      {

        for ( x = 0; x < outSizeX; x++ )
        {

          doResample( inVolume, normTransform3d, outBackground, outLoc, *o );
          ++ o;
          outLoc.x += outResolution.x;

        }
        outLoc.y += outResolution.y;
        outLoc.x = 0.0;

      }
      outLoc.z += outResolution.z;
      outLoc.y = 0.0;

    } 
    if ( verbose )
    {

      std::cout << gkg::Eraser( 16 );

    }

#endif

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "     
             "const gkg::Transform3d< double >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "bool verbose )" );

}


#ifdef RESAMPLER_USE_THREADED_LOOP

template < class T >
void
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,
                               const gkg::Transform3d< double >& transform3d,
                               const T& outBackground,
                               gkg::Volume< T >& outVolume,
                               gkg::Vector3d< double >& outResolution,
                               int32_t zStartIndex,
                               int32_t zCount,
                               bool /* verbose */ )
{

  try
  {

    const int32_t outSizeX = outVolume.getSizeX();
    const int32_t outSizeY = outVolume.getSizeY();

    int32_t zStopIndex = zStartIndex + zCount;
    gkg::Vector3d< double > outLoc( 0.0, 0.0, 0.0 );

    // Use a for loop to compute outLoc.z instead of multiplication
    // Reason: slight difference using double multiplication,
    // and we wanted to compare results with previous implementation
    // using for loop in z...
    for ( int32_t z = 0; z < zStartIndex; z++ )
    {

      outLoc.z += outResolution.z;

    }

    int32_t x, y, z;
    for ( z = zStartIndex; z < zStopIndex; z++ )
    {

      for ( y = 0; y < outSizeY; y++ )
      {

        for ( x = 0; x < outSizeX; x++ )
        {

          doResample( inVolume, transform3d, outBackground, outLoc, 
	              outVolume( x, y, z ) );
          outLoc.x += ( float )outResolution.x;

        }
        outLoc.y += ( float )outResolution.y;
        outLoc.x = 0.0;

      }
      outLoc.z += ( float )outResolution.z;
      outLoc.y = 0.0;

    }

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( "
             "const gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, gkg::Volume< T >& outVolume, "
             "int32_t zStartIndex, int32_t zCount, "
             "bool verbose )" );

}

#endif

template < class T >
void
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,
                               const gkg::Transform3d< float >& transform3d,
                               const T& outBackground,
                               const gkg::Vector3d< float >& outLocation,
                               T& outValue,
                               const gkg::Vector3d< double >* inResolution )
{

  try
  {

    if ( inVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "input volume must have a T size equal to 1" );

    }

    gkg::Vector3d< double > resolution;

    if ( inResolution )
    {

      resolution = *inResolution;

    }
    else
    {

      inVolume.getHeader().getAttribute( "resolutionX", resolution.x );
      inVolume.getHeader().getAttribute( "resolutionY", resolution.y );
      inVolume.getHeader().getAttribute( "resolutionZ", resolution.z );

    }

    gkg::CompositeTransform3d< float > normTransform3d;

    gkg::RCPointer< gkg::Scaling3d< float > > scaling;
    scaling.reset( new gkg::Scaling3d< float >( ( float )resolution.x,
                                                ( float )resolution.y,
                                                ( float )resolution.z ) );

    if ( !scaling.isNull() )
    {

      normTransform3d.compose( scaling, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate Scaling3d instance" );

    }

    gkg::RCPointer< gkg::Transform3d< float > > cloneTransform3d =
                                                            transform3d.clone();

    if ( !cloneTransform3d.isNull() )
    {

      normTransform3d.compose( cloneTransform3d, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate cloneTransform3d instance" );

    }

    doResample( inVolume, normTransform3d, outBackground, outLocation,
                outValue );

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( const "
             "gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< float >& transform3d, "
             "const T& outBackground, "
             "const gkg::Vector3d< float >& outLocation, T& outValue )" );

}


template < class T >
void
gkg::Resampler< T >::resample( const gkg::Volume< T >& inVolume,
                               const gkg::Transform3d< double >& transform3d,
                               const T& outBackground,
                               const gkg::Vector3d< double >& outLocation,
                               T& outValue,
                               const gkg::Vector3d< double >* inResolution )
{

  try
  {

    if ( inVolume.getSizeT() != 1 )
    {

      throw std::runtime_error( "input volume must have a T size equal to 1" );

    }

    gkg::Vector3d< double > resolution;

    if ( inResolution )
    {

      resolution = *inResolution;

    }
    else
    {

      inVolume.getHeader().getAttribute( "resolutionX", resolution.x );
      inVolume.getHeader().getAttribute( "resolutionY", resolution.y );
      inVolume.getHeader().getAttribute( "resolutionZ", resolution.z );

    }

    gkg::CompositeTransform3d< double > normTransform3d;

    gkg::RCPointer< gkg::Transform3d< double > > cloneTransform3d =
                                                            transform3d.clone();

    if ( !cloneTransform3d.isNull() )
    {

      normTransform3d.compose( cloneTransform3d , false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate cloneTransform3d instance" );

    }

    gkg::RCPointer< gkg::Scaling3d< double > > scaling;
    scaling.reset( new gkg::Scaling3d< double >( resolution ) );

    if ( !scaling.isNull() )
    {

      normTransform3d.compose( scaling, false );

    }
    else
    {

      throw std::runtime_error(  "cannot allocate Scaling3d instance" );

    }

    doResample( inVolume, normTransform3d, outBackground, outLocation,
                outValue );

  }
  GKG_CATCH( "template < class T > void gkg::Resampler< T >::resample( const "
             "gkg::Volume< T >& inVolume, "
             "const gkg::Transform3d< double >& transform3d, "
             "const T& outBackground, "
             "const gkg::Vector3d< double >& outLocation, T& outValue )" );

}


#ifdef RESAMPLER_USE_THREADED_LOOP
template < class T1, class T2 >
inline
gkg::ResamplerContext< T1, T2 >::ResamplerContext(
                                      gkg::Resampler< T1 >* resampler,
                                      const gkg::Volume< T1 >& inVolume,
                                      const gkg::Transform3d< T2 >& transform3d,
                                      const T1& outBackground,
                                      gkg::Volume< T1 >& outVolume,
                                      gkg::Vector3d< double >& outResolution,
                                      bool verbose )
                               : gkg::LoopContext< int32_t >(),
                                 _resampler( resampler ),
                                 _inVolume( inVolume ),
                                 _transform3d( transform3d ),
                                 _outBackground( outBackground ),
                                 _outVolume( outVolume ),
                                 _outResolution( outResolution ),
                                 _verbose( verbose )

{
}


template < class T1, class T2 >
inline
void gkg::ResamplerContext< T1, T2 >::doIt( int32_t startIndex, int32_t count )
{

  try
  {

    _resampler->resample( _inVolume, _transform3d, _outBackground,
                          _outVolume, _outResolution,
                          startIndex, count, _verbose );

  }
  GKG_CATCH( "template < class T1, class T2 > inline"
             "void gkg::ResamplerContext< T1, T2 >::doIt( "
             "int32_t startIndex, int32_t count )" );

}
#endif

template class gkg::Resampler< int8_t >;
template class gkg::Resampler< uint8_t >;
template class gkg::Resampler< int16_t >;
template class gkg::Resampler< uint16_t >;
template class gkg::Resampler< int32_t >;
template class gkg::Resampler< uint32_t >;
template class gkg::Resampler< int64_t >;
template class gkg::Resampler< uint64_t >;
template class gkg::Resampler< float  >;
template class gkg::Resampler< double >;
template class gkg::Resampler< std::complex< float > >;
template class gkg::Resampler< std::complex< double > >;
template class gkg::Resampler< gkg::RGBComponent >;

