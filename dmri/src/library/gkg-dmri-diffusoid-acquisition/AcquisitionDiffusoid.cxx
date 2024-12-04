#include <gkg-dmri-diffusoid-acquisition/AcquisitionDiffusoid.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define EPSILON 1e-6


gkg::AcquisitionDiffusoid::AcquisitionDiffusoid(
                                   const gkg::OrientationSet* orientationSet,
                                   const gkg::SO3Interpolator* so3Interpolator,
                                   const gkg::Volume< float >& t2,
                                   const gkg::Volume< float >& dw,
                                   const gkg::Vector3d< int32_t >& site )
                          : gkg::Diffusoid( orientationSet, so3Interpolator )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      getReconstructionMatrix( dw,
                               *orientationSet,
                               reconstructionMatrix );

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float averageT2 = ( float )t2( site, 0 );
      for ( t = 1; t < t2Count; t++ )
      {

        averageT2 += ( float )t2( site, t );

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount * 2 );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
        normalizedSignal( t + orientationCount ) = normalizedSignal( t );

      }

      // processing attenuations
      gkg::Vector magnitudes;
      magnitudes = reconstructionMatrix.getComposition( normalizedSignal );
      int32_t o;
      for ( o = 0; o < _orientationSet->getCount(); o++ )
      {

        _magnitudes[ o ] = ( float )magnitudes( o );

      }

    }

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoid::AcquisitionDiffusoid( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site )" );

}


gkg::AcquisitionDiffusoid::AcquisitionDiffusoid(
                                   const gkg::OrientationSet* orientationSet,
                                   const gkg::SO3Interpolator* so3Interpolator,
                                   const gkg::Volume< float >& t2,
                                   const gkg::Volume< float >& dw,
                                   const gkg::Vector3d< float >& site )
                          : gkg::Diffusoid( orientationSet, so3Interpolator )
{

  try
  {

    if ( _orientationSet )
    {

      // building reconstruction matrix
      gkg::Matrix reconstructionMatrix;
      getReconstructionMatrix( dw,
                               *orientationSet,
                               reconstructionMatrix );

      // getting pointer to quick partial voluming resampler
      gkg::PartialVolumingQuickResampler< float >& resampler =
        gkg::PartialVolumingQuickResampler< float >::getInstance();

      // collecting resolution of data
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      if ( t2.getHeader().hasAttribute( "resolutionX" ) )
      {

        t2.getHeader().getAttribute( "resolutionX", resolution.x );

      }
      if ( t2.getHeader().hasAttribute( "resolutionY" ) )
      {

        t2.getHeader().getAttribute( "resolutionY", resolution.y );

      }
      if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
      {

        t2.getHeader().getAttribute( "resolutionZ", resolution.z );

      }

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float t2Value;
      float averageT2;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      averageT2 = ( float )t2Value;
      for ( t = 1; t < t2Count; t++ )
      {

        resampler.resample( t2,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            t2Value,
                            &resolution,
                            t );
        averageT2 += ( float )t2Value;

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount * 2 );
      float dwValue = 0.0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
        normalizedSignal( t + orientationCount ) = normalizedSignal( t );

      }

      // processing attenuations
      gkg::Vector magnitudes;
      magnitudes = reconstructionMatrix.getComposition( normalizedSignal );
      int32_t o;
      for ( o = 0; o < _orientationSet->getCount(); o++ )
      {

        _magnitudes[ o ] = ( float )magnitudes( o );

      }

    }

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoid::AcquisitionDiffusoid( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site )" );

}


gkg::AcquisitionDiffusoid::AcquisitionDiffusoid(
                                    const gkg::OrientationSet* orientationSet,
                                    const gkg::SO3Interpolator* so3Interpolator,
                                    const gkg::Volume< float >& t2,
                                    const gkg::Volume< float >& dw,
                                    const gkg::Vector3d< int32_t >& site,
                                    const gkg::Matrix& reconstructionMatrix )
                          : gkg::Diffusoid( orientationSet, so3Interpolator )
{

  try
  {

    if ( _orientationSet )
    {

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float averageT2 = ( float )t2( site, 0 );
      for ( t = 1; t < t2Count; t++ )
      {

        averageT2 += ( float )t2( site, t );

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount * 2 );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
        normalizedSignal( t + orientationCount ) = normalizedSignal( t );

      }

      // processing attenuations
      gkg::Vector magnitudes;
      magnitudes = reconstructionMatrix.getComposition( normalizedSignal );
      int32_t o;
      for ( o = 0; o < _orientationSet->getCount(); o++ )
      {

        _magnitudes[ o ] = ( float )magnitudes( o );

      }

    }

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoid::AcquisitionDiffusoid( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "const gkg::Matrix& reconstructionMatrix )" );

}


gkg::AcquisitionDiffusoid::AcquisitionDiffusoid(
                                    const gkg::OrientationSet* orientationSet,
                                    const gkg::SO3Interpolator* so3Interpolator,
                                    const gkg::Volume< float >& t2,
                                    const gkg::Volume< float >& dw,
                                    const gkg::Vector3d< float >& site,
                                    const gkg::Matrix& reconstructionMatrix )
                          : gkg::Diffusoid( orientationSet, so3Interpolator )
{

  try
  {

    if ( orientationSet )
    {

      // getting pointer to quick partial voluming resampler
      gkg::PartialVolumingQuickResampler< float >& resampler =
        gkg::PartialVolumingQuickResampler< float >::getInstance();

      // collecting resolution of data
      gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
      if ( t2.getHeader().hasAttribute( "resolutionX" ) )
      {

        t2.getHeader().getAttribute( "resolutionX", resolution.x );

      }
      if ( t2.getHeader().hasAttribute( "resolutionY" ) )
      {

        t2.getHeader().getAttribute( "resolutionY", resolution.y );

      }
      if ( t2.getHeader().hasAttribute( "resolutionZ" ) )
      {

        t2.getHeader().getAttribute( "resolutionZ", resolution.z );

      }

      int32_t t2Count = t2.getSizeT();
      int32_t orientationCount = dw.getSizeT();
      int32_t t = 0;

      // processing average T2 value
      float t2Value;
      float averageT2;
      resampler.resample( t2,
                          ( float )0,
                          gkg::Vector3d< double >( site.x, site.y, site.z ),
                          t2Value,
                          &resolution,
                          0 );

      averageT2 = ( float )t2Value;
      for ( t = 1; t < t2Count; t++ )
      {

        resampler.resample( t2,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            t2Value,
                            &resolution,
                            t );
        averageT2 += ( float )t2Value;

      }
      averageT2 /= t2Count;
      if ( averageT2 <= 0 )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount * 2 );
      float dwValue = 0.0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;
        normalizedSignal( t + orientationCount ) = normalizedSignal( t );

      }

      // processing attenuations
      gkg::Vector magnitudes;
      magnitudes = reconstructionMatrix.getComposition( normalizedSignal );
      int32_t o;
      for ( o = 0; o < _orientationSet->getCount(); o++ )
      {

        _magnitudes[ o ] = ( float )magnitudes( o );

      }

    }

  }
  GKG_CATCH( "gkg::AcquisitionDiffusoid::AcquisitionDiffusoid( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::SO3Interpolator* so3Interpolator, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "const gkg::Matrix& reconstructionMatrix )" );

}


gkg::AcquisitionDiffusoid::~AcquisitionDiffusoid()
{
}


#undef EPSILON
