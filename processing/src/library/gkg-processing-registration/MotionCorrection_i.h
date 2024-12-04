#ifndef _gkg_processing_registration_MotionCorrection_i_h_
#define _gkg_processing_registration_MotionCorrection_i_h_


#include <gkg-processing-registration/MotionCorrection.h>
#include <gkg-processing-registration/RegistrationAlgorithm_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-resampling/SplineResampler.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-transform/CompositeTransform3d.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


template < class T >
inline
gkg::MotionCorrection< T >::MotionCorrection(
    gkg::RCPointer< gkg::RegistrationAlgorithm< T, T > > registrationAlgorithm,
    int32_t resamplingOrder,
    const T& background,
    bool verbose )
                           : _registrationAlgorithm( registrationAlgorithm ),
                             _resamplingOrder( resamplingOrder ),
                             _background( background ),
                             _verbose( verbose )
{
}


template < class T >
inline
gkg::MotionCorrection< T >::~MotionCorrection()
{
}


template < class T >
inline
void gkg::MotionCorrection< T >::correct(
    gkg::RCPointer< gkg::Volume< T > > volume,
    int32_t referenceRank,
    const gkg::Vector& initialParameters,
    const T& lowerThreshold,
    std::vector< gkg::RCPointer< gkg::NonLinearTransform3d< float > > >&
                                                                  transform3ds,
    gkg::RCPointer< gkg::Transform3d< float > > furtherTransform3d,
    const gkg::Vector3d< int32_t >& furtherSliceCount )
{

  try
  {

    // collecting volume size and resolution
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume->getSize( size, sizeT );

    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume->getResolution( resolution, resolutionT );

    // creating a putative extended volume
    gkg::RCPointer< gkg::Volume< T > > extendedVolume;
    gkg::Vector3d< int32_t > extendedSize = size + furtherSliceCount * 2;
    if ( furtherSliceCount.x || furtherSliceCount.y || furtherSliceCount.z )
    {

      extendedVolume.reset( new gkg::Volume< T >( extendedSize, sizeT ) );
      extendedVolume->getHeader() = volume->getHeader();
      extendedVolume->getHeader()[ "sizeX" ] = extendedSize.x;
      extendedVolume->getHeader()[ "sizeY" ] = extendedSize.y;
      extendedVolume->getHeader()[ "sizeZ" ] = extendedSize.z;

    }
    else
    {

      extendedVolume = volume;

    }

    // sanity check
    if ( ( int32_t )transform3ds.size() != sizeT - 1 )
    {

      throw std::runtime_error(
                       "vector of 3D transform and volume size do not match" );

    }

    // allocating reference, floating and resampled volume(s)
    gkg::RCPointer< gkg::Volume< T > >
      reference( new gkg::Volume< T >( size.x, size.y, size.z ) );
    reference->getHeader().addAttribute( "resolutionX", resolution.x );
    reference->getHeader().addAttribute( "resolutionY", resolution.y );
    reference->getHeader().addAttribute( "resolutionZ", resolution.z );
    reference->getHeader().addAttribute( "resolutionT", resolutionT );

    gkg::RCPointer< gkg::Volume< T > >
      floating( new gkg::Volume< T >( size.x, size.y, size.z ) );
    floating->getHeader().addAttribute( "resolutionX", resolution.x );
    floating->getHeader().addAttribute( "resolutionY", resolution.y );
    floating->getHeader().addAttribute( "resolutionZ", resolution.z );
    floating->getHeader().addAttribute( "resolutionT", resolutionT );

    gkg::RCPointer< gkg::Volume< T > >
      resampled( new gkg::Volume< T >( extendedSize.x,
                                       extendedSize.y,
                                       extendedSize.z ) );
    resampled->getHeader().addAttribute( "resolutionX", resolution.x );
    resampled->getHeader().addAttribute( "resolutionY", resolution.y );
    resampled->getHeader().addAttribute( "resolutionZ", resolution.z );
    resampled->getHeader().addAttribute( "resolutionT", resolutionT );

    // filling reference volume
    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < size.z; site.z++ )
    {

      for ( site.y = 0; site.y < size.y; site.y++ )
      {

        for ( site.x = 0; site.x < size.x; site.x++ )
        {

          ( *reference )( site ) = ( *volume )( site, referenceRank );

        }

      }

    }

    // getting a pointer to the resampler
    gkg::Resampler< T >* resampler = gkg::ResamplerFactory< T >::getInstance().
                                              getResampler( _resamplingOrder );
    gkg::SplineResampler< T >* splineResampler =
      dynamic_cast< gkg::SplineResampler< T >* >( resampler );

    // allocating a thresholder to remove putative negative values when
    // using a spline resampler
    gkg::RCPointer< gkg::Thresholder< gkg::Volume< T > > > thresholder;
    gkg::RCPointer< gkg::GreaterOrEqualToFunction< T > >
                                                       greaterOrEqualToFunction;
    if ( splineResampler )
    {

      greaterOrEqualToFunction.reset( new gkg::GreaterOrEqualToFunction< T >( 
                                                                _background ) );
      thresholder.reset( new gkg::Thresholder< gkg::Volume< T > >(
                                     *greaterOrEqualToFunction, _background ) );

    }

    // looping over volumes, performing reference/floating registration,
    // and resampling on the fly
    int32_t t = 0;
    int32_t count = 0;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( t != referenceRank )
      {

        if ( _verbose )
        {

          if ( count != 0 )
          {

            std::cout << gkg::Eraser( 14 );

          }
          std::cout << " [ " << std::setw( 3 ) << count + 1 
                    << " / " << std::setw( 3 ) << sizeT - 1
                    << " ]" << std::flush;

        }

        // filling floating volume
        for ( site.z = 0; site.z < size.z; site.z++ )
        {

          for ( site.y = 0; site.y < size.y; site.y++ )
          {

            for ( site.x = 0; site.x < size.x; site.x++ )
            {

              ( *floating )( site ) = ( *volume )( site, t );

            }

          }

        }

        // performing registration between floating and reference volume(s)
        gkg::Vector copyOfInitialParameters( initialParameters );
        _registrationAlgorithm->match( reference,
                                       floating,
                                       copyOfInitialParameters,
                                       lowerThreshold,
                                       lowerThreshold,
                                       transform3ds[ count ] );

        // force reset of resampler in case of spline resampling
        if ( splineResampler )
        {

          splineResampler->reset();

        } 

        // composing motion correction and further 3D transform
        if ( !furtherTransform3d.isNull() )
        {

          gkg::CompositeTransform3d< float > compositeTransform3d;
          gkg::Translation3d< float >
            extendedTranslation3d( furtherSliceCount.x * resolution.x,
                                   furtherSliceCount.y * resolution.y,
                                   furtherSliceCount.z * resolution.z );

          compositeTransform3d.compose( *transform3ds[ count ] );
          compositeTransform3d.compose( extendedTranslation3d );
          compositeTransform3d.compose( *furtherTransform3d );

          // resampling floating volume
          resampler->resample( *floating,
                               compositeTransform3d,
                               _background,
                               *resampled,
                               false );

        }
        else
        {

          gkg::CompositeTransform3d< float > compositeTransform3d;
          gkg::Translation3d< float >
            extendedTranslation3d( furtherSliceCount.x * resolution.x,
                                   furtherSliceCount.y * resolution.y,
                                   furtherSliceCount.z * resolution.z );

          compositeTransform3d.compose( *transform3ds[ count ] );
          compositeTransform3d.compose( extendedTranslation3d );

          // resampling floating volume
          resampler->resample( *floating,
                               compositeTransform3d,
                               _background,
                               *resampled,
                               false );

        }

        // removing negative values when using spline order >= 3
        if ( splineResampler )
        {

          thresholder->threshold( *resampled, *resampled );

        } 

        // copying resampled volume back to the 4D volume
        for ( site.z = 0; site.z < extendedSize.z; site.z++ )
        {

          for ( site.y = 0; site.y < extendedSize.y; site.y++ )
          {

            for ( site.x = 0; site.x < extendedSize.x; site.x++ )
            {

              ( *extendedVolume )( site, t ) = ( *resampled )( site );

            }

          }

        }

        ++ count;

      }

    }
    std::cout << "step 5" << std::endl;
    // if volume was really extended
    if ( extendedVolume != volume )
    {

      *volume = *extendedVolume;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::MotionCorrection< T >::correct( "
             "gkg::RCPointer< gkg::Volume< T > > volume, "
             "int32_t referenceRank, "
             "const gkg::Vector& initialParameters, "
             "const T& lowerThreshold, "
             "std::vector< gkg::RCPointer< "
             "gkg::ParameterizedTransform3d< float > > >& transform3ds, "
             "gkg::RCPointer< gkg::Transform3d< float > > furtherTransform3d, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount )" );

}


template < class T >
inline
void gkg::MotionCorrection< T >::correct(
    gkg::RCPointer< gkg::Volume< T > > volume,
    gkg::RCPointer< gkg::Volume< T > > reference,
    const gkg::Vector& initialParameters,
    const T& lowerThreshold,
    std::vector< gkg::RCPointer< gkg::NonLinearTransform3d< float > > >&
                                                                  transform3ds,
    gkg::RCPointer< gkg::Transform3d< float > > furtherTransform3d,
    const gkg::Vector3d< int32_t >& furtherSliceCount )
{

  try
  {

    // collecting volume size and resolution
    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume->getSize( size, sizeT );

    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;
    volume->getResolution( resolution, resolutionT );

    // collecting reference size and resolution
    gkg::Vector3d< int32_t > referenceSize;
    int32_t referenceSizeT = 1;
    reference->getSize( referenceSize, referenceSizeT );

    gkg::Vector3d< double > referenceResolution;
    double referenceResolutionT = 1.0;
    reference->getResolution( referenceResolution, referenceResolutionT );

    // creating a putative extended volume
    gkg::RCPointer< gkg::Volume< T > > extendedVolume;
    gkg::Vector3d< int32_t > extendedSize = size + furtherSliceCount * 2;
    if ( furtherSliceCount.x || furtherSliceCount.y || furtherSliceCount.z )
    {

      extendedVolume.reset( new gkg::Volume< T >( extendedSize, sizeT ) );
      extendedVolume->getHeader() = volume->getHeader();
      extendedVolume->getHeader()[ "sizeX" ] = extendedSize.x;
      extendedVolume->getHeader()[ "sizeY" ] = extendedSize.y;
      extendedVolume->getHeader()[ "sizeZ" ] = extendedSize.z;

    }
    else
    {

      extendedVolume = volume;

    }

    // sanity check
    if ( ( referenceSize != size ) ||
         ( referenceResolution != resolution ) )
    {

      throw std::runtime_error( 
                    "incompatible reference versus volume size or resolution" );

    }
    if ( referenceSizeT != 1 )
    {

      throw std::runtime_error( "reference volume must be 3D and not 4D" );

    }

    if ( ( int32_t )transform3ds.size() != sizeT )
    {

      throw std::runtime_error(
                       "vector of 3D transform and volume size do not match" );

    }

    // allocating floating and resampled volume(s)
    gkg::RCPointer< gkg::Volume< T > >
      floating( new gkg::Volume< T >( size.x, size.y, size.z ) );
    floating->getHeader().addAttribute( "resolutionX", resolution.x );
    floating->getHeader().addAttribute( "resolutionY", resolution.y );
    floating->getHeader().addAttribute( "resolutionZ", resolution.z );
    floating->getHeader().addAttribute( "resolutionT", resolutionT );

    gkg::RCPointer< gkg::Volume< T > >
      resampled( new gkg::Volume< T >( extendedSize.x,
                                       extendedSize.y,
                                       extendedSize.z ) );
    resampled->getHeader().addAttribute( "resolutionX", resolution.x );
    resampled->getHeader().addAttribute( "resolutionY", resolution.y );
    resampled->getHeader().addAttribute( "resolutionZ", resolution.z );
    resampled->getHeader().addAttribute( "resolutionT", resolutionT );

    // getting a pointer to the resampler
    gkg::Resampler< T >* resampler = gkg::ResamplerFactory< T >::getInstance().
                                              getResampler( _resamplingOrder );
    gkg::SplineResampler< T >* splineResampler =
      dynamic_cast< gkg::SplineResampler< T >* >( resampler );

    // allocating a thresholder to remove putative negative values when
    // using a spline resampler
    gkg::RCPointer< gkg::Thresholder< gkg::Volume< T > > > thresholder;
    gkg::RCPointer< gkg::GreaterOrEqualToFunction< T > >
                                                       greaterOrEqualToFunction;
    if ( splineResampler )
    {

      greaterOrEqualToFunction.reset( new gkg::GreaterOrEqualToFunction< T >( 
                                                                _background ) );
      thresholder.reset( new gkg::Thresholder< gkg::Volume< T > >(
                                     *greaterOrEqualToFunction, _background ) );

    }

    // looping over volumes, performing reference/floating registration,
    // and resampling on the fly
    int32_t t = 0;
    gkg::Vector3d< int32_t > site;
    for ( t = 0; t < sizeT; t++ )
    {

      if ( _verbose )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 14 );

        }
        std::cout << " [ " << std::setw( 3 ) << t + 1 
                  << " / " << std::setw( 3 ) << sizeT
                  << " ]" << std::flush;

      }

      // filling floating volume
      for ( site.z = 0; site.z < size.z; site.z++ )
      {

        for ( site.y = 0; site.y < size.y; site.y++ )
        {

          for ( site.x = 0; site.x < size.x; site.x++ )
          {

            ( *floating )( site ) = ( *volume )( site, t );

          }

        }

      }

      // performing registration between floating and reference volume(s)
      gkg::Vector copyOfInitialParameters( initialParameters );
      _registrationAlgorithm->match( reference,
                                     floating,
                                     copyOfInitialParameters,
                                     lowerThreshold,
                                     lowerThreshold,
                                     transform3ds[ t ] );

      // force reset of resampler in case of spline resampling
      if ( splineResampler )
      {

        splineResampler->reset();

      } 

      // composing motion correction and further 3D transform
      if ( !furtherTransform3d.isNull() )
      {

        gkg::CompositeTransform3d< float > compositeTransform3d;
        gkg::Translation3d< float >
          extendedTranslation3d( furtherSliceCount.x * resolution.x,
                                 furtherSliceCount.y * resolution.y,
                                 furtherSliceCount.z * resolution.z );

        compositeTransform3d.compose( *transform3ds[ t ] );
        compositeTransform3d.compose( extendedTranslation3d );
        compositeTransform3d.compose( *furtherTransform3d );

        // resampling floating volume
        resampler->resample( *floating,
                             compositeTransform3d,
                             _background,
                             *resampled,
                             false );

      }
      else
      {

        gkg::CompositeTransform3d< float > compositeTransform3d;
        gkg::Translation3d< float >
          extendedTranslation3d( furtherSliceCount.x * resolution.x,
                                 furtherSliceCount.y * resolution.y,
                                 furtherSliceCount.z * resolution.z );

        compositeTransform3d.compose( *transform3ds[ t ] );
        compositeTransform3d.compose( extendedTranslation3d );

        // resampling floating volume
        resampler->resample( *floating,
                             compositeTransform3d,
                             _background,
                             *resampled,
                             false );

      }

      // removing negative values when using spline order >= 3
      if ( splineResampler )
      {

        thresholder->threshold( *resampled, *resampled );

      } 

      // copying resampled volume back to the 4D volume
      for ( site.z = 0; site.z < extendedSize.z; site.z++ )
      {

        for ( site.y = 0; site.y < extendedSize.y; site.y++ )
        {

          for ( site.x = 0; site.x < extendedSize.x; site.x++ )
          {

            ( *extendedVolume )( site, t ) = ( *resampled )( site );

          }

        }

      }

    }

    // if volume was really extended
    if ( extendedVolume != volume )
    {

      *volume = *extendedVolume;

    }

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::MotionCorrection< T >::correct( "
             "gkg::RCPointer< gkg::Volume< T > > volume, "
             "gkg::RCPointer< gkg::Volume< T > > reference, "
             "const gkg::Vector& initialParameters, "
             "const T& lowerThreshold, "
             "std::vector< gkg::RCPointer< "
             "gkg::ParameterizedTransform3d< float > > >& transform3ds ), "
             "gkg::RCPointer< gkg::Transform3d< float > > furtherTransform3d, "
             "const gkg::Vector3d< int32_t >& furtherSliceCount )" );

}


#endif
