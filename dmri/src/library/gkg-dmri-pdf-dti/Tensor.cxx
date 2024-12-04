#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-transform/Rotation3dFunctions.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <vector>


#define EPSILON 1e-30
#define DEFAULT_CONVERGENCE_STEP_COUNT    10
#define DEFAULT_TRIAL_COUNT               10
#define TINY_NUMBER                    1e-10
#define SMALL_NUMBER                    1e-6


gkg::Tensor::Tensor()
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             0 ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    _coefficients( 0 ) = 1.0;
    _coefficients( 1 ) = 0.0;
    _coefficients( 2 ) = 0.0;
    _coefficients( 3 ) = 1.0;
    _coefficients( 4 ) = 0.0;
    _coefficients( 5 ) = 1.0;

    _diffusionTime = 1.0;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor()" );

}


gkg::Tensor::Tensor( float dxx,
                     float dxy,
                     float dxz,
                     float dyy,
                     float dyz,
                     float dzz,
                     float diffusionTime )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             0 ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    _coefficients( 0 ) = ( double )dxx;
    _coefficients( 1 ) = ( double )dxy;
    _coefficients( 2 ) = ( double )dxz;
    _coefficients( 3 ) = ( double )dyy;
    _coefficients( 4 ) = ( double )dyz;
    _coefficients( 5 ) = ( double )dzz;

    _diffusionTime = diffusionTime;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "float dxx, "
             "float dxy, "
             "float dxz, "
             "float dyy, "
             "float dyz, "
             "float dzz, "
             "float diffusionTime )" );

}


gkg::Tensor::Tensor( 
             const std::map< float, OrientationSet >* outputMultiShellSampling )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    _coefficients( 0 ) = 1.0;
    _coefficients( 1 ) = 0.0;
    _coefficients( 2 ) = 0.0;
    _coefficients( 3 ) = 1.0;
    _coefficients( 4 ) = 0.0;
    _coefficients( 5 ) = 1.0;

    _diffusionTime = 1.0;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, OrientationSet >* "
             "outputMultiShellSampling )" );

}



gkg::Tensor::Tensor(
                  const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                  const gkg::Vector3d< double >* outputCartesianGridResolution )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    _coefficients( 0 ) = 1.0;
    _coefficients( 1 ) = 0.0;
    _coefficients( 2 ) = 0.0;
    _coefficients( 3 ) = 1.0;
    _coefficients( 4 ) = 0.0;
    _coefficients( 5 ) = 1.0;

    _diffusionTime = 1.0;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution )" );

}



gkg::Tensor::Tensor(  
              const std::map< float, OrientationSet >* outputMultiShellSampling,
              const gkg::Volume< float >& t2,
              const gkg::Volume< float >& dw,
              const gkg::Vector3d< int32_t >& site,
              gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        dwValue = dw( site, t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // building orientation matrix
      gkg::Matrix U( orientationCount, 6 );
      gkg::Vector S( 6 );
      gkg::Matrix V( 6, 6 );
      getOrientationSVDDecomposition( dw, U, S, V );

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        // building b-matrix
        gkg::Matrix B( orientationCount + 1, 6 );
        getBMatrix( dw, B );
        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, OrientationSet >* "
             "outputMultiShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}

gkg::Tensor::Tensor(  
              const gkg::Vector3d< int32_t >* outputCartesianGridSize,
              const gkg::Vector3d< double >* outputCartesianGridResolution,
              const gkg::Volume< float >& t2,
              const gkg::Volume< float >& dw,
              const gkg::Vector3d< int32_t >& site,
              gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        dwValue = dw( site, t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // building orientation matrix
      gkg::Matrix U( orientationCount, 6 );
      gkg::Vector S( 6 );
      gkg::Matrix V( 6, 6 );
      getOrientationSVDDecomposition( dw, U, S, V );

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        // building b-matrix
        gkg::Matrix B( orientationCount + 1, 6 );
        getBMatrix( dw, B );
        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor(
         const std::map< float, gkg::OrientationSet >* outputMultiShellSampling,
         const gkg::Volume< float >& t2,
         const gkg::Volume< float >& dw,
         const gkg::Vector3d< float >& site,
         gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
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

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

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

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // building orientation matrix
      gkg::Matrix U( orientationCount, 6 );
      gkg::Vector S( 6 );
      gkg::Matrix V( 6, 6 );
      getOrientationSVDDecomposition( dw, U, S, V );

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        // building b-matrix
        gkg::Matrix B( orientationCount + 1, 6 );
        getBMatrix( dw, B );
        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultiShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor(
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
                   const gkg::Volume< float >& t2,
                   const gkg::Volume< float >& dw,
                   const gkg::Vector3d< float >& site,
                   gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
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

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

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

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // building orientation matrix
      gkg::Matrix U( orientationCount, 6 );
      gkg::Vector S( 6 );
      gkg::Matrix V( 6, 6 );
      getOrientationSVDDecomposition( dw, U, S, V );

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        // building b-matrix
        gkg::Matrix B( orientationCount + 1, 6 );
        getBMatrix( dw, B );
        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor( 
         const std::map< float, gkg::OrientationSet >* outputMultiShellSampling,
         const gkg::Volume< float >& t2,
         const gkg::Volume< float >& dw,
         const gkg::Matrix& B,
         const gkg::Matrix& U,
         const gkg::Vector& S,
         const gkg::Matrix& V,
         const gkg::Vector3d< int32_t >& site,
         gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        dwValue = dw( site, t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 / float( dwValue ) );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // sanity check for orientation matrix
      if ( ( U.getSize1() != orientationCount ) ||
           ( U.getSize2() != 6 ) ||
           ( S.getSize() != 6 ) ||
           ( V.getSize1() != 6 ) ||
           ( V.getSize2() != 6 ) )
      {

        throw std::runtime_error( "U, S, V size incompatible with DWI data" );

      }

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );


      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultiShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& B, "
             "const gkg::Matrix& U, "
             "const gkg::Vector& S, "
             "const gkg::Matrix& V, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor( 
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
                   const gkg::Volume< float >& t2,
                   const gkg::Volume< float >& dw,
                   const gkg::Matrix& B,
                   const gkg::Matrix& U,
                   const gkg::Vector& S,
                   const gkg::Matrix& V,
                   const gkg::Vector3d< int32_t >& site,
                   gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t orientationCount = dw.getSizeT();
    int32_t t = 0;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

    // processing average T2 value
    float averageT2 = ( float )t2( site, 0 );
    for ( t = 1; t < t2Count; t++ )
    {

      averageT2 += ( float )t2( site, t );

    }
    averageT2 /= t2Count;

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {

      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        dwValue = dw( site, t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 / float( dwValue ) );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // sanity check for orientation matrix
      if ( ( U.getSize1() != orientationCount ) ||
           ( U.getSize2() != 6 ) ||
           ( S.getSize() != 6 ) ||
           ( V.getSize1() != 6 ) ||
           ( V.getSize2() != 6 ) )
      {

        throw std::runtime_error( "U, S, V size incompatible with DWI data" );

      }

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );


      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& B, "
             "const gkg::Matrix& U, "
             "const gkg::Vector& S, "
             "const gkg::Matrix& V, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor(
         const std::map< float, gkg::OrientationSet >* outputMultiShellSampling,
         const gkg::Volume< float >& t2,
         const gkg::Volume< float >& dw,
         const gkg::Matrix& B,
         const gkg::Matrix& U,
         const gkg::Vector& S,
         const gkg::Matrix& V,
         const gkg::Vector3d< float >& site,
         gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
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

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

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

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {


      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // sanity check for orientation matrix
      if ( ( U.getSize1() != orientationCount ) ||
           ( U.getSize2() != 6 ) ||
           ( S.getSize() != 6 ) ||
           ( V.getSize1() != 6 ) ||
           ( V.getSize2() != 6 ) )
      {

        throw std::runtime_error( "U, S, V size incompatible with DWI data" );

      }

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultiShellSampling, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& B, "
             "const gkg::Matrix& U, "
             "const gkg::Vector& S, "
             "const gkg::Matrix& V, "
             "const gkg::Vector3d< float >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor(
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
                   const gkg::Volume< float >& t2,
                   const gkg::Volume< float >& dw,
                   const gkg::Matrix& B,
                   const gkg::Matrix& U,
                   const gkg::Vector& S,
                   const gkg::Matrix& V,
                   const gkg::Vector3d< float >& site,
                   gkg::Tensor::FitMethod fitMethod )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( 6 ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 )
{

  try
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

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // building time series
    gkg::Vector timeSeries( orientationCount + 1 );
    timeSeries.fill( 0 );

    // building log vector of I0/I
    gkg::Vector normalizedSignal( orientationCount );
    normalizedSignal.fill( 0 );

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

    // processing log vector of I0/I
    if ( averageT2 > 0.0 )
    {


      timeSeries( 0 ) = averageT2;

      float dwValue;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        timeSeries( t + 1 ) = ( double )dwValue;
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = std::log( averageT2 ) - std::log( dwValue );
          if ( std::isinf( normalizedSignal( t ) ) )
          {

            normalizedSignal( t ) = 10.0;

          }

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // sanity check for orientation matrix
      if ( ( U.getSize1() != orientationCount ) ||
           ( U.getSize2() != 6 ) ||
           ( S.getSize() != 6 ) ||
           ( V.getSize1() != 6 ) ||
           ( V.getSize2() != 6 ) )
      {

        throw std::runtime_error( "U, S, V size incompatible with DWI data" );

      }

      // processing linear square fit of tensor coefficients
      factory->getSingularValueSolution( U, S, V, normalizedSignal,
                                         _coefficients );

      // if robust fit required, performing robust positive definite tensor
      if ( fitMethod == gkg::Tensor::RobustPositiveDefinite )
      {

        getRobustPositiveDefiniteTensor( B, timeSeries );

      }

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing to a default isotropic tensor
      _coefficients( gkg::Tensor::Dxx ) = 1.0;
      _coefficients( gkg::Tensor::Dxy ) = 0.0;
      _coefficients( gkg::Tensor::Dxz ) = 0.0;
      _coefficients( gkg::Tensor::Dyy ) = 1.0;
      _coefficients( gkg::Tensor::Dyz ) = 0.0;
      _coefficients( gkg::Tensor::Dzz ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& B, "
             "const gkg::Matrix& U, "
             "const gkg::Vector& S, "
             "const gkg::Matrix& V, "
             "const gkg::Vector3d< float >& site, "
             "gkg::Tensor::FitMethod fitMethod )" );

}


gkg::Tensor::Tensor( 
         const std::map< float, gkg::OrientationSet >* outputMultiShellSampling,
         const gkg::Vector& coefficients,
         float diffusionTime )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputMultiShellSampling ),
              _coefficients( coefficients ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 ),
              _diffusionTime( diffusionTime )
{

  try
  {

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const std::map< float, gkg::OrientationSet >* "
             "outputMultiShellSampling, "
             "const gkg::Vector& coefficients, "
             "float diffusionTime )" );

}


gkg::Tensor::Tensor( 
                   const gkg::Vector3d< int32_t >* outputCartesianGridSize,
                   const gkg::Vector3d< double >* outputCartesianGridResolution,
                   const gkg::Vector& coefficients,
                   float diffusionTime )
            : gkg::ProbabilityDensityFunction(
                             gkg::ProbabilityDensityFunction::SecondOrderTensor,
                             outputCartesianGridSize,
                             outputCartesianGridResolution ),
              _coefficients( coefficients ),
              _inverseCoefficients( 6 ),
              _eigenValues( 3 ),
              _eigenVectors( 3, 3 ),
              _diffusionTime( diffusionTime )
{

  try
  {

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::Tensor::Tensor( "
             "const gkg::Vector3d< int32_t >* outputCartesianGridSize, "
             "const gkg::Vector3d< double >* outputCartesianGridResolution, "
             "const gkg::Vector& coefficients, "
             "float diffusionTime )" );

}


gkg::Tensor::Tensor( const gkg::Tensor& other )
            : gkg::ProbabilityDensityFunction( other ),
              _coefficients( other._coefficients ),
              _inverseCoefficients( other._inverseCoefficients ),
              _eigenValues( other._eigenValues ),
              _eigenVectors( other._eigenVectors ),
              _diffusionTime( other._diffusionTime )              
{
}


gkg::Tensor::~Tensor()
{
}


gkg::Tensor& gkg::Tensor::operator=( const gkg::Tensor& other )
{

  _coefficients = other._coefficients;
  _inverseCoefficients = other._inverseCoefficients;
  _eigenValues = other._eigenValues;
  _eigenVectors = other._eigenVectors;
  _diffusionTime = other._diffusionTime;

  return *this;

}


float gkg::Tensor::getDisplacementProbability(
                              const gkg::Vector3d< float >& displacement ) const
{

  try
  {

    float diffusionTimeFourPi = 4 * M_PI * _diffusionTime;

    return  ( 1.0 / std::sqrt( diffusionTimeFourPi * diffusionTimeFourPi *
                               diffusionTimeFourPi * getDeterminant() ) ) *
            std::exp( -getInverseDiffusionCoefficient( displacement ) /
                     ( 4 * _diffusionTime ) );

  }
  GKG_CATCH( "float gkg::Tensor::getDisplacementProbability( "
             "const gkg::Vector3d< float >& displacement ) const" );

}


const gkg::Vector& gkg::Tensor::getCoefficients() const
{

  return _coefficients;

}


float gkg::Tensor::getCoefficient(
                          gkg::Tensor::CoefficientIndex coefficientIndex ) const
{

  return _coefficients( coefficientIndex );

}


float gkg::Tensor::getInverseCoefficient(
                          gkg::Tensor::CoefficientIndex coefficientIndex ) const
{

  return _inverseCoefficients( coefficientIndex );

}


const gkg::Vector& gkg::Tensor::getEigenValues() const
{

  return _eigenValues;

}


float gkg::Tensor::getEigenValue(
                          gkg::Tensor::EigenSystemIndex eigenSystemIndex ) const
{

  return _eigenValues( eigenSystemIndex );

}


gkg::Vector3d< float >
gkg::Tensor::getEigenVector(
                          gkg::Tensor::EigenSystemIndex eigenSystemIndex ) const
{

  return gkg::Vector3d< float >(
                                ( float )_eigenVectors( 0, eigenSystemIndex ),
                                ( float )_eigenVectors( 1, eigenSystemIndex ),
                                ( float )_eigenVectors( 2, eigenSystemIndex ) );

}


float gkg::Tensor::getDeterminant() const
{

  return ( float )( _eigenValues( gkg::Tensor::Minimum ) *
                    _eigenValues( gkg::Tensor::Middle ) *
                    _eigenValues( gkg::Tensor::Maximum ) );

}


float gkg::Tensor::getApparentDiffusionCoefficient() const
{

  float adc = ( float )( _coefficients( gkg::Tensor::Dxx ) +
                         _coefficients( gkg::Tensor::Dyy ) +
                         _coefficients( gkg::Tensor::Dzz ) ) / 3.0;
  return ( adc >= 0.0 ? adc : 0.0 );

}


float gkg::Tensor::getNorm2() const
{

  return ( float )( _coefficients( gkg::Tensor::Dxx ) * 
                    _coefficients( gkg::Tensor::Dxx ) +
                    _coefficients( gkg::Tensor::Dyy ) *
                    _coefficients( gkg::Tensor::Dyy ) +
                    _coefficients( gkg::Tensor::Dzz ) *
                    _coefficients( gkg::Tensor::Dzz ) +
                    2 * ( _coefficients( gkg::Tensor::Dxy ) *
                          _coefficients( gkg::Tensor::Dxy ) +
                          _coefficients( gkg::Tensor::Dxz ) *
                          _coefficients( gkg::Tensor::Dxz ) +
                          _coefficients( gkg::Tensor::Dyz ) *
                          _coefficients( gkg::Tensor::Dyz ) ) );

}



float gkg::Tensor::getNorm() const
{

  return  std::sqrt( getNorm2() );

}


float gkg::Tensor::getFractionalAnisotropy() const
{

  float adc = getApparentDiffusionCoefficient();
  float mdc = std::sqrt( getNorm2() / 3.0f );
  float anisotropy = 0.0;

  if ( mdc > 0.0 )
  {

    float fraction = ( adc * adc ) / ( mdc * mdc );	
    anisotropy = ( fraction <= 1.0 ) ? std::sqrt( 1.5 * ( 1.0 - fraction ) )
                                     : 0.0;

  }

  if ( anisotropy < 0.0 )
  {

    anisotropy = 0.0;

  }
  if ( anisotropy > 1.0 )
  {

    anisotropy = 1.0;

  }

  return anisotropy;

}


float gkg::Tensor::getLambdaParallel() const
{

  float lambdaParallel = ( float )_eigenValues( gkg::Tensor::Maximum );

  return ( lambdaParallel >= 0.0 ? lambdaParallel : 0.0 );

}


float gkg::Tensor::getLambdaTransverse() const
{

  float lambdaTransverse = ( float )( 0.5f *
                                      ( _eigenValues( gkg::Tensor::Minimum ) +
                                        _eigenValues( gkg::Tensor::Middle ) ) );

  return ( lambdaTransverse >= 0.0 ? lambdaTransverse : 0.0 );

}


float gkg::Tensor::getDiffusionCoefficient(
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  return ( float )( _coefficients( gkg::Tensor::Dxx ) *
                    unitaryOrientation.x * unitaryOrientation.x +
                    _coefficients( gkg::Tensor::Dyy ) *
                    unitaryOrientation.y * unitaryOrientation.y +
                    _coefficients( gkg::Tensor::Dzz ) *
                    unitaryOrientation.z * unitaryOrientation.z +
                    2.0 * ( _coefficients( gkg::Tensor::Dxy ) *
                            unitaryOrientation.x * unitaryOrientation.y +
                            _coefficients( gkg::Tensor::Dxz ) *
                            unitaryOrientation.x * unitaryOrientation.z +
                            _coefficients( gkg::Tensor::Dyz ) *
                            unitaryOrientation.y * unitaryOrientation.z ) );

}


float gkg::Tensor::getInverseDiffusionCoefficient(
                              const gkg::Vector3d< float >& displacement ) const
{

  return ( float )( _inverseCoefficients( gkg::Tensor::Dxx ) *
                    displacement.x * displacement.x +
                    _inverseCoefficients( gkg::Tensor::Dyy ) *
                    displacement.y * displacement.y +
                    _inverseCoefficients( gkg::Tensor::Dzz ) *
                    displacement.z * displacement.z +
                    2.0 * ( _inverseCoefficients( gkg::Tensor::Dxy ) *
                            displacement.x * displacement.y +
                            _inverseCoefficients( gkg::Tensor::Dxz ) *
                            displacement.x * displacement.z +
                            _inverseCoefficients( gkg::Tensor::Dyz ) *
                            displacement.y * displacement.z ) );

}


gkg::Vector gkg::Tensor::getRotatedDiffusionWeightedSignal(
                                     const gkg::OrientationSet& orientationSet,
                                     const gkg::Matrix& rotationMatrix,
                                     double bValue,
                                     float S0 ) const
{

  try
  {

    // rotating tensor
    gkg::Matrix currentTensor( 3, 3 );
    currentTensor( 0, 0 ) = _coefficients( gkg::Tensor::Dxx );
    currentTensor( 0, 1 ) = _coefficients( gkg::Tensor::Dxy );
    currentTensor( 1, 0 ) = _coefficients( gkg::Tensor::Dxy );
    currentTensor( 0, 2 ) = _coefficients( gkg::Tensor::Dxz );
    currentTensor( 2, 0 ) = _coefficients( gkg::Tensor::Dxz );
    currentTensor( 1, 1 ) = _coefficients( gkg::Tensor::Dyy );
    currentTensor( 1, 2 ) = _coefficients( gkg::Tensor::Dyz );
    currentTensor( 2, 1 ) = _coefficients( gkg::Tensor::Dyz );
    currentTensor( 2, 2 ) = _coefficients( gkg::Tensor::Dzz );

    currentTensor = rotationMatrix.getComposition( currentTensor ).
                      getComposition( rotationMatrix.getTransposition() );

    gkg::Vector rotatedCoefficients ( 6 );
    rotatedCoefficients( gkg::Tensor::Dxx ) = currentTensor( 0, 0 );
    rotatedCoefficients( gkg::Tensor::Dxy ) = currentTensor( 0, 1 );
    rotatedCoefficients( gkg::Tensor::Dxz ) = currentTensor( 0, 2 );
    rotatedCoefficients( gkg::Tensor::Dyy ) = currentTensor( 1, 1 );
    rotatedCoefficients( gkg::Tensor::Dyz ) = currentTensor( 1, 2 );
    rotatedCoefficients( gkg::Tensor::Dzz ) = currentTensor( 2, 2 );

    gkg::Tensor rotatedTensor( 0, rotatedCoefficients );

    int32_t orientationCount = orientationSet.getCount();
    int32_t o = 0;
    gkg::Vector signal( orientationCount );
    float apparentDiffusionCoefficient = 0.0f;
    for ( o = 0; o < orientationCount; o++ )
    {

      apparentDiffusionCoefficient = rotatedTensor.getDiffusionCoefficient(
                                           orientationSet.getOrientation( o ) );
      signal( o ) = S0 * std::exp( -bValue * apparentDiffusionCoefficient );

    }

    return signal;

  }
  GKG_CATCH( "gkg::Vector gkg::Tensor::getRotatedDiffusionWeightedSignal( "
             "const gkg::OrientationSet& orientationSet, "
             "const gkg::Matrix& rotationMatrix, "
             "double bValue, "
             "float S0 ) const" );

}


gkg::Rotation3d< float > gkg::Tensor::getRotation3d() const
{

  try
  {


    // checking the trieder is direct, else inverting the last axis
    gkg::Vector3d< float > u = this->getEigenVector( gkg::Tensor::Maximum );
    gkg::Vector3d< float > v = this->getEigenVector( gkg::Tensor::Middle );
    gkg::Vector3d< float > w = this->getEigenVector( gkg::Tensor::Minimum );
    if ( std::fabs( u.cross( v ).dot( w ) - 1.0 ) > 1.0 )
    {

      w *= -1.0f;

    }

    return gkg::Rotation3d< float >( gkg::getXYZAnglesFromTrieder( u, v, w ) );

  }
  GKG_CATCH( "gkg::Rotation3d< float > gkg::Tensor::getRotation3d() const" );

}


float gkg::Tensor::getDiffusionTime() const
{

  return _diffusionTime;

}


void gkg::Tensor::getDiffusionTime( const gkg::HeaderedObject& headeredObject )
{

  try
  {

    // building orientation matrix
    double diffusionTime = 1.0;
    if ( headeredObject.getHeader().hasAttribute( "diffusion_time" ) )
    {

      headeredObject.getHeader().getAttribute( "diffusion_time",
                                               diffusionTime );

    }
    else
    {

      //throw std::runtime_error( "missing attribute 'diffusion_time'" );

    }

    _diffusionTime = ( float )diffusionTime;

  }
  GKG_CATCH( "void gkg::Tensor::getDiffusionTime( "
             "const gkg::HeaderedObject& headeredObject )" );

}


void gkg::Tensor::getBMatrix( const gkg::HeaderedObject& headeredObject,
                              gkg::Matrix& B )
{

  try
  {

    // getting orientation count
    int32_t orientationCount = 0;

    if ( headeredObject.getHeader().hasAttribute( "sizeT" ) )
    {

      headeredObject.getHeader().getAttribute( "sizeT", orientationCount );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeT'" );

    }

    // building orientation matrix
    std::vector< double > bValues( orientationCount );
    try
    {

      if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
      {

        headeredObject.getHeader().getAttribute( "bvalues", bValues );

      }
      else
      {

        throw std::runtime_error( "missing attribute 'bvalues'" );

      }

    }
    catch ( std::exception& )
    {

      std::vector< float > floatBValues( orientationCount );
      if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
      {

        headeredObject.getHeader().getAttribute( "bvalues", floatBValues );

      }
      else
      {

        throw std::runtime_error( "missing attribute 'bvalues'" );

      }
      std::vector< float >::const_iterator
        i = floatBValues.begin(),
        ie = floatBValues.end();
      std::vector< double >::iterator
        o = bValues.begin();
      while ( i != ie )
      {

        *o = ( double )*i;
        ++ o;
        ++ i;

      }

    }

    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    if ( headeredObject.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }

    B( 0, 0 ) = 0;
    B( 0, 1 ) = 0;
    B( 0, 2 ) = 0;
    B( 0, 3 ) = 0;
    B( 0, 4 ) = 0;
    B( 0, 5 ) = 0;

    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      gkg::Vector3d< float > orientation( ( float )tmp[ 0 ]->getScalar(),
                                          ( float )tmp[ 1 ]->getScalar(),
                                          ( float )tmp[ 2 ]->getScalar() );
                                          

      B( o + 1, 0 ) = bValues[ o ] * 1e6 * orientation.x * orientation.x;
      B( o + 1, 1 ) = bValues[ o ] * 1e6 *  orientation.x * orientation.y;
      B( o + 1, 2 ) = bValues[ o ] * 1e6 *  orientation.x * orientation.z;
      B( o + 1, 3 ) = bValues[ o ] * 1e6 *  orientation.y * orientation.y;
      B( o + 1, 4 ) = bValues[ o ] * 1e6 *  orientation.y * orientation.z;
      B( o + 1, 5 ) = bValues[ o ] * 1e6 *  orientation.z * orientation.z;

    }

  }
  GKG_CATCH( "void gkg::Tensor::getBMatrix( "
             "const gkg::HeaderedObject& headeredObject, "
             "gkg::Matrix& B )" );

}


void gkg::Tensor::getOrientationSVDDecomposition(
                                      const gkg::HeaderedObject& headeredObject,
                                      gkg::Matrix& U,
                                      gkg::Vector& S,
                                      gkg::Matrix& V )
{

  try
  {

    // getting orientation count
    int32_t orientationCount = 0;

    if ( headeredObject.getHeader().hasAttribute( "sizeT" ) )
    {

      headeredObject.getHeader().getAttribute( "sizeT", orientationCount );

    }
    else
    {

      throw std::runtime_error( "missing attribute 'sizeT'" );

    }

    // building orientation matrix
    std::vector< double > bValues( orientationCount );

    try
    {

      if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
      {

        headeredObject.getHeader().getAttribute( "bvalues", bValues );

      }
      else
      {

        throw std::runtime_error( "missing attribute 'bvalues'" );

      }

    }
    catch ( std::exception& )
    {

      std::vector< float > floatBValues( orientationCount );
      if ( headeredObject.getHeader().hasAttribute( "bvalues" ) )
      {

        headeredObject.getHeader().getAttribute( "bvalues", floatBValues );

      }
      else
      {

        throw std::runtime_error( "missing attribute 'bvalues'" );

      }

      std::vector< float >::const_iterator
        i = floatBValues.begin(),
        ie = floatBValues.end();
      std::vector< double >::iterator
        o = bValues.begin();
      while ( i != ie )
      {

        *o = ( double )*i;
        ++ o;
        ++ i;

      }

    }

    gkg::GenericObjectList genericObjectListOrientations( orientationCount );
    if ( headeredObject.getHeader().hasAttribute(
                                           "diffusion_gradient_orientations" ) )
    {

      headeredObject.getHeader().getAttribute(
                                              "diffusion_gradient_orientations",
                                              genericObjectListOrientations );

    }
    else
    {

      throw std::runtime_error(
                        "missing attribute 'diffusion_gradient_orientations'" );

    }

    int32_t o;
    for ( o = 0; o < U.getSize1(); o++ )
    {

      gkg::GenericObjectList tmp =
      genericObjectListOrientations[ o ]->getValue< gkg::GenericObjectList >();
      gkg::Vector3d< float > orientation( ( float )tmp[ 0 ]->getScalar(),
                                          ( float )tmp[ 1 ]->getScalar(),
                                          ( float )tmp[ 2 ]->getScalar() );
                                          

      U( o, 0 ) = bValues[ o ] * orientation.x * orientation.x;
      U( o, 1 ) = 2 * bValues[ o ] * orientation.x * orientation.y;
      U( o, 2 ) = 2 * bValues[ o ] * orientation.x * orientation.z;
      U( o, 3 ) = bValues[ o ] * orientation.y * orientation.y;
      U( o, 4 ) = 2 * bValues[ o ] * orientation.y * orientation.z;
      U( o, 5 ) = bValues[ o ] * orientation.z * orientation.z;

    }

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // performing SVD decomposition
    factory->getSingularValueDecomposition( U, V, S );

  }
  GKG_CATCH( "void gkg::Tensor::getOrientationSVDDecomposition( "
             "const gkg::HeaderedObject& headeredObject, "
             "gkg::Matrix& U, "
             "gkg::Vector& S, "
             "gkg::Matrix& V )" );

}


void gkg::Tensor::doEigenSystemAndInverseMatrix()
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    gkg::Matrix D( 3, 3 );

    D( 0, 0 ) = _coefficients( gkg::Tensor::Dxx );
    D( 0, 1 ) = _coefficients( gkg::Tensor::Dxy );
    D( 0, 2 ) = _coefficients( gkg::Tensor::Dxz );
    D( 1, 0 ) = _coefficients( gkg::Tensor::Dxy );
    D( 1, 1 ) = _coefficients( gkg::Tensor::Dyy );
    D( 1, 2 ) = _coefficients( gkg::Tensor::Dyz );
    D( 2, 0 ) = _coefficients( gkg::Tensor::Dxz );
    D( 2, 1 ) = _coefficients( gkg::Tensor::Dyz );
    D( 2, 2 ) = _coefficients( gkg::Tensor::Dzz );

    factory->getEigenSystem( D, _eigenValues, _eigenVectors );
    factory->sortEigenSystem( _eigenValues, _eigenVectors, 
             gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

    if ( _eigenValues( gkg::Tensor::Minimum ) < 0.0 )
    {

      _eigenValues( gkg::Tensor::Minimum ) = EPSILON;

    }
    if ( _eigenValues( gkg::Tensor::Middle ) < 0.0 )
    {

      _eigenValues( gkg::Tensor::Middle ) = EPSILON;

    }
    if ( _eigenValues( gkg::Tensor::Maximum ) < 0.0 )
    {

      _eigenValues( gkg::Tensor::Maximum ) = EPSILON;

    }

#if 0
    gkg::Matrix inverseD( 3, 3 );

    gkg::Vector inverseEigenValues( 3 );
    inverseEigenValues( gkg::Tensor::Maximum ) =
                                     1.0 / _eigenValues( gkg::Tensor::Maximum );
    inverseEigenValues( gkg::Tensor::Middle ) =
                                     1.0 / _eigenValues( gkg::Tensor::Middle );
    inverseEigenValues( gkg::Tensor::Minimum ) =
                                     1.0 / _eigenValues( gkg::Tensor::Minimum );

    inverseD = _eigenVectors.getComposition(
                 inverseEigenValues.getDiagonal().getComposition(
                   _eigenVectors.getTransposition() ) );
		   
    _inverseCoefficients( gkg::Tensor::Dxx ) = inverseD( 0, 0 );
    _inverseCoefficients( gkg::Tensor::Dxy ) = inverseD( 0, 1 );
    _inverseCoefficients( gkg::Tensor::Dxz ) = inverseD( 0, 2 );
    _inverseCoefficients( gkg::Tensor::Dyy ) = inverseD( 1, 1 );
    _inverseCoefficients( gkg::Tensor::Dyz ) = inverseD( 1, 2 );
    _inverseCoefficients( gkg::Tensor::Dzz ) = inverseD( 2, 2 );
#endif
    double l1 = 1.0 / _eigenValues( gkg::Tensor::Maximum );
    double l2 = 1.0 / _eigenValues( gkg::Tensor::Middle );
    double l3 = 1.0 / _eigenValues( gkg::Tensor::Minimum );

    _inverseCoefficients( gkg::Tensor::Dxx ) = l1 * _eigenVectors( 0, 0 ) * 
                                                    _eigenVectors( 0, 0 ) +
					       l2 * _eigenVectors( 0, 1 ) * 
                                                    _eigenVectors( 0, 1 ) +
					       l3 * _eigenVectors( 0, 2 ) * 
                                                    _eigenVectors( 0, 2 );
    _inverseCoefficients( gkg::Tensor::Dxy ) = l1 * _eigenVectors( 0, 0 ) * 
                                                    _eigenVectors( 1, 0 ) +
					       l2 * _eigenVectors( 0, 1 ) * 
                                                    _eigenVectors( 1, 1 ) +
					       l3 * _eigenVectors( 1, 2 ) * 
                                                    _eigenVectors( 0, 2 );
    _inverseCoefficients( gkg::Tensor::Dxz ) = l1 * _eigenVectors( 0, 0 ) * 
                                                    _eigenVectors( 2, 0 ) +
					       l2 * _eigenVectors( 0, 1 ) * 
                                                    _eigenVectors( 2, 1 ) +
					       l3 * _eigenVectors( 0, 2 ) * 
                                                    _eigenVectors( 2, 2 );
    _inverseCoefficients( gkg::Tensor::Dyy ) = l1 * _eigenVectors( 1, 0 ) * 
                                                    _eigenVectors( 1, 0 ) +
					       l2 * _eigenVectors( 1, 1 ) * 
                                                    _eigenVectors( 1, 1 ) +
					       l3 * _eigenVectors( 1, 2 ) * 
                                                    _eigenVectors( 1, 2 );
    _inverseCoefficients( gkg::Tensor::Dyz ) = l1 * _eigenVectors( 1, 0 ) * 
                                                    _eigenVectors( 2, 0 ) +
					       l2 * _eigenVectors( 1, 1 ) * 
                                                    _eigenVectors( 2, 1 ) +
					       l3 * _eigenVectors( 1, 2 ) * 
                                                    _eigenVectors( 2, 2 );
    _inverseCoefficients( gkg::Tensor::Dzz ) = l1 * _eigenVectors( 2, 0 ) * 
                                                    _eigenVectors( 2, 0 ) +
					       l2 * _eigenVectors( 2, 1 ) * 
                                                    _eigenVectors( 2, 1 ) +
					       l3 * _eigenVectors( 2, 2 ) * 
                                                    _eigenVectors( 2, 2 );

  }
  GKG_CATCH( "void gkg::Tensor::doEigenSystemAndInverseMatrix()" );

}


void gkg::Tensor::doScaling()
{

  try
  {

    // we assume that the b-value is given in s/mm^2, so use 1e6 factor
    // to restore the b-value in s/m^2
    // Thus, diffusion coefficients are given in m^2/s, normally close
    // to 2.2e-9 m^2/s for water, at 25 degrees
    _coefficients( gkg::Tensor::Dxx ) *= 1e-6;
    _coefficients( gkg::Tensor::Dxy ) *= 1e-6;
    _coefficients( gkg::Tensor::Dxz ) *= 1e-6;
    _coefficients( gkg::Tensor::Dyy ) *= 1e-6;
    _coefficients( gkg::Tensor::Dyz ) *= 1e-6;
    _coefficients( gkg::Tensor::Dzz ) *= 1e-6;

    _inverseCoefficients( gkg::Tensor::Dxx ) *= 1e+6;
    _inverseCoefficients( gkg::Tensor::Dxy ) *= 1e+6;
    _inverseCoefficients( gkg::Tensor::Dxz ) *= 1e+6;
    _inverseCoefficients( gkg::Tensor::Dyy ) *= 1e+6;
    _inverseCoefficients( gkg::Tensor::Dyz ) *= 1e+6;
    _inverseCoefficients( gkg::Tensor::Dzz ) *= 1e+6;

    _eigenValues( gkg::Tensor::Minimum ) *= 1e-6;
    _eigenValues( gkg::Tensor::Middle ) *= 1e-6;
    _eigenValues( gkg::Tensor::Maximum ) *= 1e-6;

  }
  GKG_CATCH( "void gkg::Tensor::doScaling()" );

}



void gkg::Tensor::getRobustPositiveDefiniteTensor(
                                           const gkg::Matrix& B,   
                                           const gkg::Vector& /* timeSeries */ )
{

  try
  {

    int32_t orientationCount = B.getSize1() - 1;

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // allocating structures
    gkg::Matrix F( 3, 3 );
    gkg::Matrix D( 3, 3 );
    gkg::Matrix Hplus( 3, 3 );
    gkg::Matrix Hminus( 3, 3 );
    gkg::Matrix oldD( 3, 3 );

    std::vector< gkg::Matrix > tempF( 2 );
    std::vector< gkg::Matrix > tempD( 2 );
    std::vector< gkg::Matrix > tempHplus( 2 );
    std::vector< gkg::Matrix > tempHminus( 2 );

    int32_t i;
    for ( i = 0; i < 2; i++ )
    {

      tempF[ i ] = gkg::Matrix( 3, 3 );
      tempD[ i ] = gkg::Matrix( 3, 3 );
      tempHplus[ i ] = gkg::Matrix( 3, 3 );
      tempHminus[ i ] = gkg::Matrix( 3, 3 );

    }

    // performing eigensystem
    D( 0, 0 ) = _coefficients( gkg::Tensor::Dxx );
    D( 0, 1 ) = _coefficients( gkg::Tensor::Dxy );
    D( 0, 2 ) = _coefficients( gkg::Tensor::Dxz );
    D( 1, 0 ) = _coefficients( gkg::Tensor::Dxy );
    D( 1, 1 ) = _coefficients( gkg::Tensor::Dyy );
    D( 1, 2 ) = _coefficients( gkg::Tensor::Dyz );
    D( 2, 0 ) = _coefficients( gkg::Tensor::Dxz );
    D( 2, 1 ) = _coefficients( gkg::Tensor::Dyz );
    D( 2, 2 ) = _coefficients( gkg::Tensor::Dzz );

    factory->getEigenSystem( D, _eigenValues, _eigenVectors );
    factory->sortEigenSystem( _eigenValues, _eigenVectors, 
            gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

    // computing initial estimate from linear model
    if ( _eigenValues( gkg::Tensor::Maximum ) < 0 )
    {

      double adc = ( std::fabs( _eigenValues( gkg::Tensor::Maximum ) ) +
                     std::fabs( _eigenValues( gkg::Tensor::Middle ) ) +
                     std::fabs( _eigenValues( gkg::Tensor::Minimum ) ) ) / 3.0;
      D( 0, 0 ) = adc;
      D( 0, 1 ) = 0;
      D( 0, 2 ) = 0;
      D( 1, 0 ) = 0;
      D( 1, 1 ) = adc;
      D( 1, 2 ) = 0;
      D( 2, 0 ) = 0;
      D( 2, 1 ) = 0;
      D( 2, 2 ) = adc;

    }
    else
    {

      double mu = 0.2 * _eigenValues( gkg::Tensor::Maximum );
      if ( _eigenValues( gkg::Tensor::Middle ) < mu )
      {

       _eigenValues( gkg::Tensor::Middle ) = mu;

      }
      if ( _eigenValues( gkg::Tensor::Minimum ) < mu )
      {

         _eigenValues( gkg::Tensor::Minimum ) = mu;

      }
      D = _eigenVectors.getComposition( _eigenValues.getDiagonal()
                                       ).getComposition(
          _eigenVectors.getTransposition() );

    }

    // initializing weight(s) to 1.0
    gkg::Vector weights( orientationCount + 1 );
    weights.fill( 1.0 );

    // computing non-gradient intensities
    double ED = 0.0, oldED = 0.0;
    //double J = computeJ( D, B, weights, timeSeries, F, ED );

    tempF[ 0 ] = F;
    tempD[ 0 ] = D;
    tempHplus[ 0 ] = Hplus;
    tempHminus[ 0 ] = Hminus;

    oldD = D;
    oldED = ED;

    double deltaTau = computeDeltaTau( D, F );

    if ( deltaTau <= TINY_NUMBER )
    {

       return;

    }

    bool hasConverged = false;
    int32_t convergenceStep = 0;
    int32_t convergenceStepCount = DEFAULT_CONVERGENCE_STEP_COUNT;
    int32_t trialStep = 0;
    int32_t trialCount = 0;
    while ( ( convergenceStep < convergenceStepCount ) &&
            ( hasConverged == false ) && 
            ( trialCount < DEFAULT_TRIAL_COUNT ) )
    {

      trialStep = 1;
      trialCount = 0;
      while ( trialStep && ( trialCount < DEFAULT_TRIAL_COUNT ) )
      {

        computeHplusHminus( deltaTau, F, D, Hplus, Hminus );
        computeNewD( Hplus, Hminus, D );
        //J = computeJ( D, B, weights, timeSeries, F, ED );

        if ( ED < oldED )
        {

          oldED = ED;
          trialStep = 0;

          tempF[ 1 ] = F;
          tempD[ 1 ] = D;
          tempHplus[ 1 ] = Hplus;
          tempHminus[ 1 ] = Hminus;

        }
        else
        {

          F = tempF[ 0 ];
          D = tempD[ 0 ];
          Hplus = tempHplus[ 0 ];
          Hminus = tempHminus[ 0 ];

          deltaTau /= 2;
          ++ trialCount;

        }

      }

      if ( trialCount < 10 )
      {

        double originalDeltaTau = deltaTau;
        int32_t adjustStep = 1;

        for ( i = 0; i < 2; i++ )
        {

          if ( i == 0 )
          {

            deltaTau *= 2.0;

          }
          else
          {

            deltaTau /= 2.0;

          }

          if ( ( adjustStep == 1 ) &&
               ( ( i != 0 ) || ( trialCount < 2 ) ) )
          {

            F = tempF[ 0 ];
            D = tempD[ 0 ];
            Hplus = tempHplus[ 0 ];
            Hminus = tempHminus[ 0 ];

            computeHplusHminus( deltaTau, F, D, Hplus, Hminus );
            computeNewD( Hplus, Hminus, D );
            //J = computeJ( D, B, weights, timeSeries, F, ED );

            if ( ED < oldED )
            {

              adjustStep = 0;
              tempF[ 0 ] = F;
              tempD[ 0 ] = D;
              tempHplus[ 0 ] = Hplus;
              tempHminus[ 0 ] = Hminus;

              oldED = ED;

            }

          }

        }

        if ( adjustStep != 0 )
        {

          ED = oldED;
          deltaTau = originalDeltaTau;

          F = tempF[ 1 ];
          D = tempD[ 1 ];
          Hplus = tempHplus[ 1 ];
          Hminus = tempHminus[ 1 ];

          tempF[ 0 ] = F;
          tempD[ 0 ] = D;
          tempHplus[ 0 ] = Hplus;
          tempHminus[ 0 ] = Hminus;

        }

        if ( convergenceStep != 0 )
        {

          hasConverged = testConvergence( D, oldD );

        }

        oldD = D;

        ++ convergenceStep;

      }
      else
      {

        F = tempF[ 0 ];
        D = tempD[ 0 ];
        Hplus = tempHplus[ 0 ];
        Hminus = tempHminus[ 0 ];

        ED = oldED;

      }

    }
    //std::cout << "convergenceStep=" << convergenceStep << std::endl;

    ED = oldED;

    // collecting result of robust positive definite fit
    _coefficients( gkg::Tensor::Dxx ) = D( 0, 0 );
    _coefficients( gkg::Tensor::Dxy ) = D( 0, 1 );
    _coefficients( gkg::Tensor::Dxz ) = D( 0, 2 );
    _coefficients( gkg::Tensor::Dyy ) = D( 1, 1 );
    _coefficients( gkg::Tensor::Dyz ) = D( 1, 2 );
    _coefficients( gkg::Tensor::Dzz ) = D( 2, 2 );

  }
  GKG_CATCH( "void gkg::Tensor::getRobustPositiveDefiniteTensor( "
             "const gkg::Matrix& B, "
             "const gkg::Vector& timeSeries )" );

}


// computing estimate of T2-weighted value J
double gkg::Tensor::computeJ( const gkg::Matrix& D,
                              const gkg::Matrix& B,
                              const gkg::Vector& weights,
                              const gkg::Vector& timeSeries,
                              gkg::Matrix& F,
                              double& ED ) const
{

  try
  {

    int32_t orientationCount = B.getSize1() - 1;

    int32_t o = 0;

    gkg::Vector vD( 6 );
    vD( 0 ) = D( 0, 0 );
    vD( 1 ) = 2 * D( 0, 1 );
    vD( 2 ) = 2 * D( 0, 2 );
    vD( 3 ) = D( 1, 1 );
    vD( 4 ) = 2 * D( 1, 2 );
    vD( 5 ) = D( 2, 2 );

    // processing large dot product of b and D at qth gradient
    // J = Sum( wq Iq exp( -bq D ) ) / Sum( wq exp(-2bq D ) )
    // estimate of b0 intensity without noise and applied gradient
    double sum0 = 0.0;
    double sum1 = 0.0;
    gkg::Vector BD = B.getComposition( vD );

    gkg::Vector expBD( orientationCount + 1 );
    gkg::Vector weightedExpBD( orientationCount + 1 );
    for ( o = 0; o < orientationCount + 1; o++ )
    {

      expBD( o ) = std::exp( - BD( o ) );
      weightedExpBD( o ) = weights( o ) * expBD( o );

      sum0 += weightedExpBD( o ) * timeSeries( o );
      sum1 += weightedExpBD( o ) * expBD( o );

    }

    double J = sum0 / sum1;

    // compute error functional E(D,J) and gradient F of E with respect to D
    // E( D, J ) = 1/2 Sum[ wq ( J exp( -bq.D ) - Iq )^2 ]
    // F =  - Sum[ wq ( J exp( -bq.D ) - Iq ) bq ]
    gkg::Vector temporaryF( 6 );
    temporaryF.fill( 0 );

    double residual, squareResidual, weightedResidual;
    int32_t i;
    for ( o = 0; o < orientationCount + 1; o++ )
    {

      residual = J * expBD( o ) - timeSeries( o );
      squareResidual = residual * residual;
      weightedResidual = -weights( o ) * residual;

      for ( i = 0; i < 6; i++ )
      {

        temporaryF( i ) += weightedResidual * B( o, i );

      }
      sum0 += weights( o ) * squareResidual;

    }

    F( 0, 0 ) = temporaryF( 0 );
    F( 0, 1 ) = temporaryF( 1 );
    F( 0, 2 ) = temporaryF( 2 );
    F( 1, 0 ) = temporaryF( 1 );
    F( 1, 1 ) = temporaryF( 3 );
    F( 1, 2 ) = temporaryF( 4 );
    F( 2, 0 ) = temporaryF( 2 );
    F( 2, 1 ) = temporaryF( 4 );
    F( 2, 2 ) = temporaryF( 5 );

    ED = sum0 / 2;

    return J;

  }
  GKG_CATCH( "double gkg::Tensor::computeJ( "
             "const gkg::Matrix& D, "
             "const gkg::Matrix& B, "
             "const gkg::Vector& weights, "
             "const gkg::Vector& timeSeries, "
             "gkg::Matrix& F, "
             "double& ED ) const" );

}


// computing initial step size for gradient descent
double gkg::Tensor::computeDeltaTau( const gkg::Matrix& D,
                                     const gkg::Matrix& F ) const
{

  try
  {

    double deltaTau;

    gkg::Matrix Dsquare = D.getComposition( D );
    gkg::Matrix FDsquare = F.getComposition( Dsquare );
    gkg::Matrix DsquareF = Dsquare.getComposition( F );
    gkg::Matrix G = FDsquare.getComposition( DsquareF );

    double sum0 = std::fabs( D( 0, 0 ) ) +
                  std::fabs( D( 0, 1 ) ) +
                  std::fabs( D( 0, 2 ) ) +
                  std::fabs( D( 1, 0 ) ) +
                  std::fabs( D( 1, 1 ) ) +
                  std::fabs( D( 1, 2 ) ) +
                  std::fabs( D( 2, 0 ) ) +
                  std::fabs( D( 2, 1 ) ) +
                  std::fabs( D( 2, 2 ) );
    double sum1 = std::fabs( G( 0, 0 ) ) +
                  std::fabs( G( 0, 1 ) ) +
                  std::fabs( G( 0, 2 ) ) +
                  std::fabs( G( 1, 0 ) ) +
                  std::fabs( G( 1, 1 ) ) +
                  std::fabs( G( 1, 2 ) ) +
                  std::fabs( G( 2, 0 ) ) +
                  std::fabs( G( 2, 1 ) ) +
                  std::fabs( G( 2, 2 ) );

    if ( sum1 != 0 )
    {

      deltaTau = 0.01 * sum0 / sum1;

    }
    else
    {

      deltaTau = 0.0;

    }

    return deltaTau;

  }
  GKG_CATCH( "double gkg::Tensor::computeDeltaTau( "
             "const gkg::Matrix& D, const gkg::Matrix& F ) const" );

}


// computing Hplus and Hminus as a function of deltaTau
// H+ = I + 0.5 * deltaTau * F * D
// H- = I - 0.5 * deltaTau * F * D
void gkg::Tensor::computeHplusHminus( double deltaTau,
                                      const gkg::Matrix& F,
                                      const gkg::Matrix& D,
                                      gkg::Matrix& Hplus,
                                      gkg::Matrix& Hminus ) const
{

  try
  {

    double halfDeltaTau = deltaTau * 0.5;

    gkg::Matrix FD = F.getComposition( D );

    int32_t i1, i2;
    for ( i1 = 0; i1 < 3; i1++ )
    {

      for ( i2 = 0; i2 < 3; i2++ )
      {

        FD( i1, i2 ) = halfDeltaTau * FD( i1, i2 );

      }

    }

    for ( i1 = 0; i1 < 3; i1++ )
    {

      for ( i2 = 0; i2 < 3; i2++ )
      {

        if ( i1 == i2 )
        {

          Hplus( i1, i2 ) = 1 + FD( i1, i2 );
          Hminus( i1, i2 ) = 1 - FD( i1, i2 );

        }
        else
        {

          Hplus( i1, i2 ) = FD( i1, i2 );
          Hminus( i1, i2 ) = FD( i1, i2 );

        }

      }

    }

  }
  GKG_CATCH( "void gkg::Tensor::computeHplusHminus( "
             "double deltaTau, "
             "const gkg::Matrix& F, "
             "const gkg::Matrix& D, "
             "gkg::Matrix& Hplus, "
             "gkg::Matrix& Hminus ) const" );

}


// compute new D matrix
// D( tau + deltaTau ) = H- H+^-1 D( tau ) H+^-1 H-
//                     = A        D( tau ) A^T
void gkg::Tensor::computeNewD( const gkg::Matrix& Hplus,
                               const gkg::Matrix& Hminus,
                               gkg::Matrix& D ) const
{

  try
  {

    // invertion of Hplus matrix
    gkg::Matrix inverseHplus( 3, 3 );

    double denominator = ( Hplus( 0, 0 ) * Hplus( 1, 1 ) * Hplus( 2, 2 ) -
                           Hplus( 0, 0 ) * Hplus( 1, 2 ) * Hplus( 1, 2 ) -
                           Hplus( 0, 1 ) * Hplus( 0, 1 ) * Hplus( 2, 2 ) +
                           2.0 * Hplus( 0, 2 ) * Hplus( 0, 1 ) * Hplus( 1, 2 ) -
                           Hplus( 0, 2 ) * Hplus( 0, 2 ) * Hplus( 1, 1 ) );

    inverseHplus( 0, 0 ) = +( Hplus( 1, 1 ) * Hplus( 2, 2 ) -
                              Hplus( 1, 2 ) * Hplus( 1, 2 ) ) / denominator;
    inverseHplus( 0, 1 ) = -( Hplus( 0, 1 ) * Hplus( 2, 2 ) -
                              Hplus( 0, 2 ) * Hplus( 1, 2 ) ) / denominator;
    inverseHplus( 0, 2 ) = +( Hplus( 0, 1 ) * Hplus( 1, 2 ) -
                              Hplus( 0, 2 ) * Hplus( 1, 1 ) ) / denominator;
    inverseHplus( 1, 1 ) = +( Hplus( 0, 0 ) * Hplus( 2, 2 ) -
                              Hplus( 0, 2 ) * Hplus( 0, 2 ) ) / denominator;
    inverseHplus( 1, 2 ) = -( Hplus( 0, 0 ) * Hplus( 1, 2 ) -
                              Hplus( 0, 2 ) * Hplus( 0, 1 ) ) / denominator;
    inverseHplus( 2, 2 ) = +( Hplus( 0, 0 ) * Hplus( 1, 1 ) -
                              Hplus( 0, 1 ) * Hplus( 0, 1 ) ) / denominator;

    inverseHplus( 1, 0 ) = inverseHplus( 0, 1 );
    inverseHplus( 2, 0 ) = inverseHplus( 0, 2 );
    inverseHplus( 2, 1 ) = inverseHplus( 1, 2 );

    // computing new D
    gkg::Matrix A = Hminus.getComposition( inverseHplus );
    D = A.getComposition( D ).getComposition( A.getTransposition() );

  }
  GKG_CATCH( "void gkg::Tensor::computeNewD( "
             "const gkg::Matrix& Hplus, "
             "const gkg::Matrix& Hminus, "
             "gkg::Matrix& D ) const" );

}


bool gkg::Tensor::testConvergence( const gkg::Matrix& D,
                                   const gkg::Matrix& oldD ) const
{

  try
  {

    if ( ( std::fabs( D( 0, 0 ) - oldD( 0, 0 ) ) +
           std::fabs( D( 0, 1 ) - oldD( 0, 1 ) ) +
           std::fabs( D( 0, 2 ) - oldD( 0, 2 ) ) +
           std::fabs( D( 1, 1 ) - oldD( 1, 1 ) ) +
           std::fabs( D( 1, 2 ) - oldD( 1, 2 ) ) +
           std::fabs( D( 2, 2 ) - oldD( 2, 2 ) ) ) < SMALL_NUMBER )
    {

      return true;

    }
    return false;        

  }
  GKG_CATCH( "bool gkg::Tensor::testConvergence( "
             "const gkg::Matrix& D, "
             "const gkg::Matrix& oldD ) const" );

}


#undef EPSILON
#undef DEFAULT_CONVERGENCE_STEP_COUNT
#undef DEFAULT_TRIAL_COUNT
#undef TINY_NUMBER
#undef SMALL_NUMBER
