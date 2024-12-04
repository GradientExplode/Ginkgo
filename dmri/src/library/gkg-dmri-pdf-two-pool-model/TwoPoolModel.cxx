#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelLevenbergMarquardtFunction.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelNelderMeadSimplexFunction.h>
#include <gkg-dmri-qspace-sampling/QSpaceSamplingFactory.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>
#include <vector>


#define EPSILON 1e-30


gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( 13 ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 )
{

  try
  {

    // fast pool tensor
    _coefficients( 0 ) = 1.0;
    _coefficients( 1 ) = 0.0;
    _coefficients( 2 ) = 0.0;
    _coefficients( 3 ) = 1.0;
    _coefficients( 4 ) = 0.0;
    _coefficients( 5 ) = 1.0;

    // slow pool tensor
    _coefficients( 6 ) = 1.0;
    _coefficients( 7 ) = 0.0;
    _coefficients( 8 ) = 0.0;
    _coefficients( 9 ) = 1.0;
    _coefficients( 10 ) = 0.0;
    _coefficients( 11 ) = 1.0;

    // fast pool volume fraction
    _coefficients( 12 ) = 1.0;

    _diffusionTime = 1.0;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet )" );

}



gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Vector3d< int32_t >& site,
                                 gkg::TwoPoolModel::FitMethod fitMethod )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( 13 ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t dwCount = dw.getSizeT();
    int32_t t = 0;

    // building log vector of I0/I
    gkg::Vector normalizedSignal( dwCount );
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

      float dwValue;
      for ( t = 0; t < dwCount; t++ )
      {

        dwValue = dw( site, t );
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = dwValue / averageT2;

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // processing bi-exponential solution of the two pool model tensor 
      // coefficients
      gkg::Matrix qSpaceSamples;
      getQSpaceSamples( dw, qSpaceSamples );

      getBiexponentialSolution( qSpaceSamples,
                                normalizedSignal,
                                fitMethod );

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing the fast pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::FastDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDzz ) = 1.0;

      // initializing the slow pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::SlowDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDzz ) = 1.0;

      // initializing the fast pool volume fraction to 1.0
      _coefficients( gkg::TwoPoolModel::FastVolumeFraction ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::TwoPoolModel::FitMethod fitMethod )" );

}

gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Vector3d< float >& site,
                                 gkg::TwoPoolModel::FitMethod fitMethod )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( 13 ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 )
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
    int32_t dwCount = dw.getSizeT();
    int32_t t = 0;

    // building log vector of I0/I
    gkg::Vector normalizedSignal( dwCount );
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

      float dwValue;
      for ( t = 0; t < dwCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = dwValue / averageT2;

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // processing bi-exponential solution of the two pool model tensor 
      // coefficients
      gkg::Matrix qSpaceSamples;
      getQSpaceSamples( dw, qSpaceSamples );

      getBiexponentialSolution( qSpaceSamples,
                                normalizedSignal,
                                fitMethod );

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing the fast pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::FastDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDzz ) = 1.0;

      // initializing the slow pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::SlowDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDzz ) = 1.0;

      // initializing the fast pool volume fraction to 1.0
      _coefficients( gkg::TwoPoolModel::FastVolumeFraction ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "gkg::TwoPoolModel::FitMethod fitMethod )" );

}


gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Matrix& qSpaceSamples,
                                 const gkg::Vector3d< int32_t >& site,
                                 gkg::TwoPoolModel::FitMethod fitMethod )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( 13 ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 )
{

  try
  {

    int32_t t2Count = t2.getSizeT();
    int32_t dwCount = dw.getSizeT();
    int32_t t = 0;

    // building log vector of I0/I
    gkg::Vector normalizedSignal( dwCount );
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

      float dwValue;
      for ( t = 0; t < dwCount; t++ )
      {

        dwValue = dw( site, t );
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = dwValue / averageT2;

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // processing bi-exponential solution of the two pool model tensor 
      // coefficients
      getBiexponentialSolution( qSpaceSamples,
                                normalizedSignal,
                                fitMethod );

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing the fast pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::FastDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDzz ) = 1.0;

      // initializing the slow pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::SlowDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDzz ) = 1.0;

      // initializing the fast pool volume fraction to 1.0
      _coefficients( gkg::TwoPoolModel::FastVolumeFraction ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& qSpaceSamples, "
             "const gkg::Vector3d< int32_t >& site, "
             "gkg::TwoPoolModel::FitMethod fitMethod )" );

}

gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet,
                                 const gkg::Volume< float >& t2,
                                 const gkg::Volume< float >& dw,
                                 const gkg::Matrix& qSpaceSamples,
                                 const gkg::Vector3d< float >& site,
                                 gkg::TwoPoolModel::FitMethod fitMethod )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( 13 ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 )
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
    int32_t dwCount = dw.getSizeT();
    int32_t t = 0;

    // building log vector of I0/I
    gkg::Vector normalizedSignal( dwCount );
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

      float dwValue;
      for ( t = 0; t < dwCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        if ( dwValue > 0 )
        {

          normalizedSignal( t ) = dwValue / averageT2;

        }
        else
        {

          normalizedSignal( t ) = 0.0;

        }

      }

      // collecting diffusion time information
      getDiffusionTime( dw );

      // processing bi-exponential solution of the two pool model tensor 
      // coefficients
      getBiexponentialSolution( qSpaceSamples,
                                normalizedSignal,
                                fitMethod );

    }
    else
    {

      // collecting diffusion time information
      getDiffusionTime( dw );

      // initializing the fast pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::FastDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::FastDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::FastDzz ) = 1.0;

      // initializing the slow pool to a default isotropic tensor
      _coefficients( gkg::TwoPoolModel::SlowDxx ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDxy ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDxz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDyy ) = 1.0;
      _coefficients( gkg::TwoPoolModel::SlowDyz ) = 0.0;
      _coefficients( gkg::TwoPoolModel::SlowDzz ) = 1.0;

      // initializing the fast pool volume fraction to 1.0
      _coefficients( gkg::TwoPoolModel::FastVolumeFraction ) = 1.0;

    }

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

    // scaling coefficients to m^2/s
    doScaling();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Matrix& qSpaceSamples, "
             "const gkg::Vector3d< float >& site, "
             "gkg::TwoPoolModel::FitMethod fitMethod )" );

}


gkg::TwoPoolModel::TwoPoolModel( const gkg::OrientationSet* orientationSet,
                                 const gkg::Vector& coefficients,
                                 float diffusionTime )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( orientationSet ),
                    _coefficients( coefficients ),
                    _inverseCoefficients( 13 ),
                    _eigenValues( 6 ),
                    _eigenVectors( 3, 6 ),
                    _diffusionTime( diffusionTime )
{

  try
  {

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();

  }
  GKG_CATCH( "gkg::TwoPoolModel::TwoPoolModel( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Vector& coefficients, "
             "float diffusionTime  )" );

}


gkg::TwoPoolModel::TwoPoolModel( const gkg::TwoPoolModel& other )
                  : gkg::ProbabilityDensityFunction(),
                    _orientationSet( other._orientationSet ),
                    _coefficients( other._coefficients ),
                    _inverseCoefficients( other._inverseCoefficients ),
                    _eigenValues( other._eigenValues ),
                    _eigenVectors( other._eigenVectors ),
                    _diffusionTime( other._diffusionTime )
{
}


gkg::TwoPoolModel::~TwoPoolModel()
{
}


gkg::TwoPoolModel& 
gkg::TwoPoolModel::operator=( const gkg::TwoPoolModel& other )
{

  _coefficients = other._coefficients;
  _inverseCoefficients = other._inverseCoefficients;
  _eigenValues = other._eigenValues;
  _eigenVectors = other._eigenVectors;
  _diffusionTime = other._diffusionTime;
  _orientationSet = other._orientationSet;

  return *this;

}


void gkg::TwoPoolModel::setCoefficients(
                                       const std::vector< float >& coefficients,
                                       float diffusionTime )
{

  try
  {

    int32_t c = 0;
    for ( c = 0; c < 13; c++ )
    {

      _coefficients( c ) = coefficients[ c ];

    }
    _diffusionTime = diffusionTime;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();


  }
  GKG_CATCH( "void gkg::TwoPoolModel::setCoefficients( "
             "const std::vector< float >& coefficients, "
             "float diffusionTime )" );

}


void gkg::TwoPoolModel::setCoefficients( const gkg::Vector& coefficients,
                                         float diffusionTime )
{

  try
  {

    int32_t c = 0;
    for ( c = 0; c < 13; c++ )
    {

      _coefficients( c ) = coefficients( c );

    }
    _diffusionTime = diffusionTime;

    // performing eigen system and inverse tensor coefficients
    doEigenSystemAndInverseMatrix();


  }
  GKG_CATCH( "void gkg::TwoPoolModel::setCoefficients( "
             "const std::vector< float >& coefficients, "
             "float diffusionTime )" );

}


void gkg::TwoPoolModel::setOrientationSet(
                                     const gkg::OrientationSet* orientationSet )
{

  try
  {

    if ( orientationSet == 0 )
    {

      throw std::runtime_error( "nul orientation set pointer" );

    }
    _orientationSet = orientationSet;

  }
  GKG_CATCH( "void gkg::TwoPoolModel::setOrientationSet( "
             "const gkg::OrientationSet* orientationSet )" );

}


std::vector< float > gkg::TwoPoolModel::getOrientationProbabilities(
                                       gkg::TwoPoolModel::Pool poolIndex ) const
{

  try
  {


    int32_t orientationCount = _orientationSet->getCount();

    std::vector< float > probabilities( orientationCount );

    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      probabilities[ o ] = this->getDiffusionCoefficient(
                                      poolIndex,
                                      _orientationSet->getOrientation( o ) );


    }

    // normalizing
    float sum = 0.0;
    for ( o = 0; o < orientationCount; o++ )
    {

      sum += probabilities[ o ];

    }

    for ( o = 0; o < orientationCount; o++ )
    {

      probabilities[ o ] /= sum;

    }

    return probabilities;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::TwoPoolModel::getOrientationProbabilities( "
             "gkg::TwoPoolModel::Pool pool ) const" );


}


float gkg::TwoPoolModel::getDisplacementProbability(
                              const gkg::Vector3d< float >& displacement ) const
{

  try
  {

    float diffusionTimeFourPi = 4 * M_PI * _diffusionTime;
    float cubeOfDiffusionTimeFourPi = diffusionTimeFourPi *
                                      diffusionTimeFourPi *
                                      diffusionTimeFourPi;

    float fastDisplacementProbability = 
      ( 1.0 / std::sqrt( cubeOfDiffusionTimeFourPi *
                         getDeterminant( gkg::TwoPoolModel::Fast ) ) ) *
            std::exp( -getInverseDiffusionCoefficient( gkg::TwoPoolModel::Fast,
                                                       displacement ) /
                     ( 4 * _diffusionTime ) );
    float slowDisplacementProbability = 
      ( 1.0 / std::sqrt( cubeOfDiffusionTimeFourPi *
                         getDeterminant( gkg::TwoPoolModel::Slow ) ) ) *
            std::exp( -getInverseDiffusionCoefficient( gkg::TwoPoolModel::Slow,
                                                       displacement ) /
                     ( 4 * _diffusionTime ) );

    return this->getFastVolumeFraction() * fastDisplacementProbability +
           this->getSlowVolumeFraction() * slowDisplacementProbability;

  }
  GKG_CATCH( "float gkg::TwoPoolModel::getDisplacementProbability( "
             "const gkg::Vector3d< float >& displacement ) const" );

}


const gkg::Vector& gkg::TwoPoolModel::getCoefficients() const
{

  return _coefficients;

}


float gkg::TwoPoolModel::getCoefficient(
                    gkg::TwoPoolModel::CoefficientIndex coefficientIndex ) const
{

  return _coefficients( coefficientIndex );

}


float gkg::TwoPoolModel::getInverseCoefficient(
                    gkg::TwoPoolModel::CoefficientIndex coefficientIndex ) const
{

  return _inverseCoefficients( coefficientIndex );

}


float gkg::TwoPoolModel::getFastVolumeFraction() const
{

  return _coefficients( gkg::TwoPoolModel::FastVolumeFraction );

}


float gkg::TwoPoolModel::getSlowVolumeFraction() const
{

  return 1.0 - _coefficients( gkg::TwoPoolModel::FastVolumeFraction );

}


const gkg::Vector& gkg::TwoPoolModel::getEigenValues() const
{

  return _eigenValues;

}


float gkg::TwoPoolModel::getEigenValue(
                    gkg::TwoPoolModel::EigenSystemIndex eigenSystemIndex ) const
{

  return _eigenValues( eigenSystemIndex );

}


gkg::Vector3d< float >
gkg::TwoPoolModel::getEigenVector(
                    gkg::TwoPoolModel::EigenSystemIndex eigenSystemIndex ) const
{

  return gkg::Vector3d< float >(
                                ( float )_eigenVectors( 0, eigenSystemIndex ),
                                ( float )_eigenVectors( 1, eigenSystemIndex ),
                                ( float )_eigenVectors( 2, eigenSystemIndex ) );

}


float 
gkg::TwoPoolModel::getDeterminant( gkg::TwoPoolModel::Pool poolIndex ) const
{

  float determinant = 0.0;

  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    determinant = ( float )( _eigenValues( gkg::TwoPoolModel::FastMinimum ) *
                             _eigenValues( gkg::TwoPoolModel::FastMiddle ) *
                             _eigenValues( gkg::TwoPoolModel::FastMaximum ) );

  }
  else
  {

    determinant = ( float )( _eigenValues( gkg::TwoPoolModel::SlowMinimum ) *
                             _eigenValues( gkg::TwoPoolModel::SlowMiddle ) *
                             _eigenValues( gkg::TwoPoolModel::SlowMaximum ) );

  }

  return determinant;

}


float gkg::TwoPoolModel::getApparentDiffusionCoefficient(
                                       gkg::TwoPoolModel::Pool poolIndex ) const
{

  float adc = 0.0;
  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    adc = ( float )( _coefficients( gkg::TwoPoolModel::FastDxx ) +
                     _coefficients( gkg::TwoPoolModel::FastDyy ) +
                     _coefficients( gkg::TwoPoolModel::FastDzz ) ) / 3.0;

  }
  else
  {

    adc = ( float )( _coefficients( gkg::TwoPoolModel::SlowDxx ) +
                     _coefficients( gkg::TwoPoolModel::SlowDyy ) +
                     _coefficients( gkg::TwoPoolModel::SlowDzz ) ) / 3.0;

  }
  return ( adc >= 0.0 ? adc : 0.0 );

}


float gkg::TwoPoolModel::getNorm2( gkg::TwoPoolModel::Pool poolIndex ) const
{

  float norm2 = 0.0;

  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    norm2 = ( float )( _coefficients( gkg::TwoPoolModel::FastDxx ) * 
                       _coefficients( gkg::TwoPoolModel::FastDxx ) +
                       _coefficients( gkg::TwoPoolModel::FastDyy ) *
                       _coefficients( gkg::TwoPoolModel::FastDyy ) +
                       _coefficients( gkg::TwoPoolModel::FastDzz ) *
                       _coefficients( gkg::TwoPoolModel::FastDzz ) +
                       2 * ( _coefficients( gkg::TwoPoolModel::FastDxy ) *
                             _coefficients( gkg::TwoPoolModel::FastDxy ) +
                             _coefficients( gkg::TwoPoolModel::FastDxz ) *
                             _coefficients( gkg::TwoPoolModel::FastDxz ) +
                             _coefficients( gkg::TwoPoolModel::FastDyz ) *
                             _coefficients( gkg::TwoPoolModel::FastDyz ) ) );

  }
  else
  {

    norm2 = ( float )( _coefficients( gkg::TwoPoolModel::SlowDxx ) * 
                       _coefficients( gkg::TwoPoolModel::SlowDxx ) +
                       _coefficients( gkg::TwoPoolModel::SlowDyy ) *
                       _coefficients( gkg::TwoPoolModel::SlowDyy ) +
                       _coefficients( gkg::TwoPoolModel::SlowDzz ) *
                       _coefficients( gkg::TwoPoolModel::SlowDzz ) +
                       2 * ( _coefficients( gkg::TwoPoolModel::SlowDxy ) *
                             _coefficients( gkg::TwoPoolModel::SlowDxy ) +
                             _coefficients( gkg::TwoPoolModel::SlowDxz ) *
                             _coefficients( gkg::TwoPoolModel::SlowDxz ) +
                             _coefficients( gkg::TwoPoolModel::SlowDyz ) *
                             _coefficients( gkg::TwoPoolModel::SlowDyz ) ) );

  }
  return norm2;

}



float gkg::TwoPoolModel::getNorm( gkg::TwoPoolModel::Pool poolIndex ) const
{

  return  std::sqrt( getNorm2( poolIndex ) );

}


float gkg::TwoPoolModel::getFractionalAnisotropy(
                                       gkg::TwoPoolModel::Pool poolIndex ) const
{

  float adc = getApparentDiffusionCoefficient( poolIndex );
  float mdc = std::sqrt( getNorm2( poolIndex ) / 3.0f );
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


float gkg::TwoPoolModel::getLambdaParallel(
                                       gkg::TwoPoolModel::Pool poolIndex ) const
{

  float lambdaParallel = 0.0;

  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    lambdaParallel = ( float )_eigenValues( gkg::TwoPoolModel::FastMaximum );

  }
  else
  {

    lambdaParallel = ( float )_eigenValues( gkg::TwoPoolModel::SlowMaximum );

  }

  return ( lambdaParallel >= 0.0 ? lambdaParallel : 0.0 );

}


float gkg::TwoPoolModel::getLambdaTransverse(
                                       gkg::TwoPoolModel::Pool poolIndex ) const
{

  float lambdaTransverse = 0.0;

  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    lambdaTransverse = ( float )( 0.5f *
                            ( _eigenValues( gkg::TwoPoolModel::FastMinimum ) +
                              _eigenValues( gkg::TwoPoolModel::FastMiddle ) ) );

  }
  else
  {

    lambdaTransverse = ( float )( 0.5f *
                            ( _eigenValues( gkg::TwoPoolModel::SlowMinimum ) +
                              _eigenValues( gkg::TwoPoolModel::SlowMiddle ) ) );

  }


  return ( lambdaTransverse >= 0.0 ? lambdaTransverse : 0.0 );

}


float gkg::TwoPoolModel::getDiffusionCoefficient(
                        gkg::TwoPoolModel::Pool poolIndex,
                        const gkg::Vector3d< float >& unitaryOrientation ) const
{

  float diffusionCoefficient = 0.0;
  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    diffusionCoefficient = ( float )(
                    _coefficients( gkg::TwoPoolModel::FastDxx ) *
                    unitaryOrientation.x * unitaryOrientation.x +
                    _coefficients( gkg::TwoPoolModel::FastDyy ) *
                    unitaryOrientation.y * unitaryOrientation.y +
                    _coefficients( gkg::TwoPoolModel::FastDzz ) *
                    unitaryOrientation.z * unitaryOrientation.z +
                    2.0 * ( _coefficients( gkg::TwoPoolModel::FastDxy ) *
                            unitaryOrientation.x * unitaryOrientation.y +
                            _coefficients( gkg::TwoPoolModel::FastDxz ) *
                            unitaryOrientation.x * unitaryOrientation.z +
                            _coefficients( gkg::TwoPoolModel::FastDyz ) *
                            unitaryOrientation.y * unitaryOrientation.z ) );

  }
  else
  {

    diffusionCoefficient = ( float )(
                    _coefficients( gkg::TwoPoolModel::SlowDxx ) *
                    unitaryOrientation.x * unitaryOrientation.x +
                    _coefficients( gkg::TwoPoolModel::SlowDyy ) *
                    unitaryOrientation.y * unitaryOrientation.y +
                    _coefficients( gkg::TwoPoolModel::SlowDzz ) *
                    unitaryOrientation.z * unitaryOrientation.z +
                    2.0 * ( _coefficients( gkg::TwoPoolModel::SlowDxy ) *
                            unitaryOrientation.x * unitaryOrientation.y +
                            _coefficients( gkg::TwoPoolModel::SlowDxz ) *
                            unitaryOrientation.x * unitaryOrientation.z +
                            _coefficients( gkg::TwoPoolModel::SlowDyz ) *
                            unitaryOrientation.y * unitaryOrientation.z ) );

  }
  return diffusionCoefficient;

}


float gkg::TwoPoolModel::getInverseDiffusionCoefficient(
                              gkg::TwoPoolModel::Pool poolIndex,
                              const gkg::Vector3d< float >& displacement ) const
{

  float inverseDiffusionCoefficient = 0.0;
  if ( poolIndex == gkg::TwoPoolModel::Fast )
  {

    inverseDiffusionCoefficient =
         ( float )( _inverseCoefficients( gkg::TwoPoolModel::FastDxx ) *
                    displacement.x * displacement.x +
                    _inverseCoefficients( gkg::TwoPoolModel::FastDyy ) *
                    displacement.y * displacement.y +
                    _inverseCoefficients( gkg::TwoPoolModel::FastDzz ) *
                    displacement.z * displacement.z +
                    2.0 * ( _inverseCoefficients( gkg::TwoPoolModel::FastDxy ) *
                            displacement.x * displacement.y +
                            _inverseCoefficients( gkg::TwoPoolModel::FastDxz ) *
                            displacement.x * displacement.z +
                            _inverseCoefficients( gkg::TwoPoolModel::FastDyz ) *
                            displacement.y * displacement.z ) );

  }
  else
  {

    inverseDiffusionCoefficient =
         ( float )( _inverseCoefficients( gkg::TwoPoolModel::SlowDxx ) *
                    displacement.x * displacement.x +
                    _inverseCoefficients( gkg::TwoPoolModel::SlowDyy ) *
                    displacement.y * displacement.y +
                    _inverseCoefficients( gkg::TwoPoolModel::SlowDzz ) *
                    displacement.z * displacement.z +
                    2.0 * ( _inverseCoefficients( gkg::TwoPoolModel::SlowDxy ) *
                            displacement.x * displacement.y +
                            _inverseCoefficients( gkg::TwoPoolModel::SlowDxz ) *
                            displacement.x * displacement.z +
                            _inverseCoefficients( gkg::TwoPoolModel::SlowDyz ) *
                            displacement.y * displacement.z ) );

  }

  return inverseDiffusionCoefficient;

}


float 
gkg::TwoPoolModel::getAttenuation( const gkg::Vector3d< float >& orientation,
                                   float bValue ) const
{

  try
  {

    double ox = orientation.x;
    double oy = orientation.y;
    double oz = orientation.z;
    double fDxx = this->_coefficients( 0 );
    double fDxy = this->_coefficients( 1 );
    double fDxz = this->_coefficients( 2 );
    double fDyy = this->_coefficients( 3 );
    double fDyz = this->_coefficients( 4 );
    double fDzz = this->_coefficients( 5 );
 
    double sDxx = this->_coefficients( 6 );
    double sDxy = this->_coefficients( 7 );
    double sDxz = this->_coefficients( 8 );
    double sDyy = this->_coefficients( 9 );
    double sDyz = this->_coefficients( 10 );
    double sDzz = this->_coefficients( 11 );

    double f = this->_coefficients( 12 );

    return ( float )( f * 
                      std::exp( -bValue * ( ox * ox * fDxx +
                                            2.0 * ox * oy * fDxy +
                                            2.0 * ox * oz * fDxz +
                                            oy * oy * fDyy +
                                            2.0 * oy * oz * fDyz +
                                            oz * oz * fDzz ) ) +
                      ( 1.0 - f ) *
                      std::exp( -bValue * ( ox * ox * sDxx +
                                            2.0 * ox * oy * sDxy +
                                            2.0 * ox * oz * sDxz +
                                            oy * oy * sDyy +
                                            2.0 * oy * oz * sDyz +
                                            oz * oz * sDzz ) ) );

  }
  GKG_CATCH( "float "
             "gkg::TwoPoolModel::getAttenuation( "
             "const gkg::Vector3d< float >& orientation, "
             "float bValue ) const" );

}


float gkg::TwoPoolModel::getDiffusionTime() const
{

  return _diffusionTime;

}


void gkg::TwoPoolModel::getDiffusionTime(
                                     const gkg::HeaderedObject& headeredObject )
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
  GKG_CATCH( "void gkg::TwoPoolModel::getDiffusionTime( "
             "const gkg::HeaderedObject& headeredObject )" );

}


void gkg::TwoPoolModel::getQSpaceSamples( 
                                      const gkg::HeaderedObject& headeredObject,
                                      gkg::Matrix& qSpaceSamples )
{

  try
  {

    // reading Q-space sampling information from headered object
    gkg::QSpaceSampling*
     qSpaceSampling = gkg::QSpaceSamplingFactory::getInstance().create(
                             *( ( gkg::HeaderedObject* )&headeredObject ) );

    // getting the pointer to the concrete Q-space sampling instance
    gkg::MultipleShellQSpaceSampling*
      multipleShellQSpaceSampling =
       dynamic_cast< gkg::MultipleShellQSpaceSampling* >( qSpaceSampling );

    if ( !multipleShellQSpaceSampling )
    {

      throw std::runtime_error( "unable to read the Q-space sampling as a "
                                "multiple same shell Q-space sampling" );

    }

    // generating samples from this two-pool model on two shells
    int32_t shellCount = multipleShellQSpaceSampling->getShellCount();
    int32_t s = 0;
    std::vector< int32_t > orientationCounts( shellCount );
    int32_t qSpaceSampleCount = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      orientationCounts[ s ] = multipleShellQSpaceSampling->getOrientationSet(
                                                                 s ).getCount();
      qSpaceSampleCount += orientationCounts[ s ];

    }

    qSpaceSamples.reallocate( qSpaceSampleCount, 4 );
    int32_t o = 0;

    int32_t q = 0;
    for ( s = 0; s < shellCount; s++ )
    {

      double bValue = multipleShellQSpaceSampling->getBValue( s );
      for ( o = 0; o < orientationCounts[ s ]; o++ )
      {

        const gkg::Vector3d< float >&
          orientation = multipleShellQSpaceSampling->getOrientation( q );

        qSpaceSamples( q, 0 ) = ( double )orientation.x;
        qSpaceSamples( q, 1 ) = ( double )orientation.y;
        qSpaceSamples( q, 2 ) = ( double )orientation.z;
        qSpaceSamples( q, 3 ) = bValue;
        ++ q;

      }

    }


  }
  GKG_CATCH( "void gkg::TwoPoolModel::getQSpaceSamples( " 
             "const gkg::HeaderedObject& headeredObject, "
             "gkg::Matrix& qSpaceSamples )" );

}


void gkg::TwoPoolModel::getBiexponentialSolution( 
                                        const gkg::Matrix& qSpaceSamples,
                                        const gkg::Vector& normalizedSignal,
                                        gkg::TwoPoolModel::FitMethod fitMethod )
{

  try
  {


    // getting numerical analysis implementation factory
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

    if ( fitMethod == gkg::TwoPoolModel::LevenbergMarquardt )
    {

      gkg::Vector initialParameters( 13 );
      initialParameters( 0 )  = 0.00127;
      initialParameters( 1 )  = 0.0;
      initialParameters( 2 )  = 0.0;
      initialParameters( 3 )  = 0.00127;
      initialParameters( 4 )  = 0.0;
      initialParameters( 5 )  = 0.00127;

      initialParameters( 6 )  = 0.00027;
      initialParameters( 7 )  = 0.0;
      initialParameters( 8 )  = 0.0;
      initialParameters( 9 )  = 0.00027;
      initialParameters( 10 ) = 0.0;
      initialParameters( 11 ) = 0.00027;

      initialParameters( 12 ) = 0.67;

      gkg::Matrix covariance;
      double chiSquarePerDegreeOfFreedom = 0.0;

      gkg::TwoPoolModelLevenbergMarquardtFunction
      twoPoolModelFunction( initialParameters );

      gkg::Vector weights( normalizedSignal.getSize() );
      weights.fill( 1.0 );

      factory->getLevenbergMarquardtFit( qSpaceSamples,
                                         normalizedSignal,
                                         weights,
                                         twoPoolModelFunction,
                                         covariance,
                                         chiSquarePerDegreeOfFreedom,
                                         10000,
                                         1e-6,
                                         1e-6 );

      _coefficients = twoPoolModelFunction.getParameters();
      double fastADC = ( _coefficients( 0 ) +
                         _coefficients( 3 ) +
                         _coefficients( 5 ) ) / 3.0;
      double slowADC = ( _coefficients( 6 ) +
                         _coefficients( 9 ) +
                         _coefficients( 11 ) ) / 3.0;
                       
      if ( slowADC > fastADC )
      {

        std::swap( _coefficients( 0 ), _coefficients( 6 ) );
        std::swap( _coefficients( 3 ), _coefficients( 9 ) );
        std::swap( _coefficients( 5 ), _coefficients( 11 ) );
        _coefficients( 12 ) = 1.0 - _coefficients( 12 );


      }

    }
    else if ( fitMethod == gkg::TwoPoolModel::NelderMeadSimplex )
    {

      _coefficients( 0 )  = 0.00127;
      _coefficients( 1 )  = 0.0;
      _coefficients( 2 )  = 0.0;
      _coefficients( 3 )  = 0.00127;
      _coefficients( 4 )  = 0.0;
      _coefficients( 5 )  = 0.00127;

      _coefficients( 6 )  = 0.00127;
      _coefficients( 7 )  = 0.0;
      _coefficients( 8 )  = 0.0;
      _coefficients( 9 )  = 0.00127;
      _coefficients( 10 ) = 0.0;
      _coefficients( 11 ) = 0.00127;

      _coefficients( 12 ) = 0.001;


      gkg::Vector deltaParameters( 13 );
      deltaParameters( 0 )  = 0.0001;
      deltaParameters( 1 )  = 0.0001;
      deltaParameters( 2 )  = 0.0001;
      deltaParameters( 3 )  = 0.0001;
      deltaParameters( 4 )  = 0.0001;
      deltaParameters( 5 )  = 0.0001;

      deltaParameters( 6 )  = 0.0001;
      deltaParameters( 7 )  = 0.0001;
      deltaParameters( 8 )  = 0.0001;
      deltaParameters( 9 )  = 0.0001;
      deltaParameters( 10 ) = 0.0001;
      deltaParameters( 11 ) = 0.0001;

      deltaParameters( 12 ) = 0.00005;

      gkg::TwoPoolModelNelderMeadSimplexFunction
        twoPoolModelFunction( qSpaceSamples,
                              normalizedSignal );

/*
      std::cout << "q-space samples:" << std::endl;
      std::cout << qSpaceSamples << std::endl;

      std::cout << "normalized signal:" << std::endl;
      std::cout << normalizedSignal << std::endl;
*/
      double testSize = 0.0;
      int32_t maximumIterationCount = 10000;
      double maximumTestSize = 1e-5;


      factory->getNelderMeadSimplexOptimumParameters( _coefficients,
                                                      deltaParameters,
                                                      testSize,
                                                      twoPoolModelFunction,
                                                      maximumIterationCount,
                                                      maximumTestSize,
                                                      false );

      _coefficients( 6 ) /= 5.0;
      _coefficients( 7 ) /= 5.0;
      _coefficients( 8 ) /= 5.0;
      _coefficients( 9 ) /= 5.0;
      _coefficients( 10 ) /= 5.0;
      _coefficients( 11 ) /= 5.0;

      _coefficients( 12 ) *= 670.0;

      double fastADC = ( _coefficients( 0 ) +
                         _coefficients( 3 ) +
                         _coefficients( 5 ) ) / 3.0;
      double slowADC = ( _coefficients( 6 ) +
                         _coefficients( 9 ) +
                         _coefficients( 11 ) ) / 3.0;
                       
      if ( slowADC > fastADC )
      {

        std::swap( _coefficients( 0 ), _coefficients( 6 ) );
        std::swap( _coefficients( 3 ), _coefficients( 9 ) );
        std::swap( _coefficients( 5 ), _coefficients( 11 ) );
        _coefficients( 12 ) = 1.0 - _coefficients( 12 );


      }

    }

  }
  GKG_CATCH( "void gkg::TwoPoolModel::getBiexponentialSolution( "
             "const gkg::Matrix& qSpaceSamples, "
             "const gkg::Vector& normalizedSignal, "
             "gkg::TwoPoolModel::FitMethod fitMethod )" );


}


void gkg::TwoPoolModel::doEigenSystemAndInverseMatrix()
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // computing fast pool eigen system and inverse matrix
    gkg::Matrix FastD( 3, 3 );

    FastD( 0, 0 ) = _coefficients( gkg::TwoPoolModel::FastDxx );
    FastD( 0, 1 ) = _coefficients( gkg::TwoPoolModel::FastDxy );
    FastD( 0, 2 ) = _coefficients( gkg::TwoPoolModel::FastDxz );
    FastD( 1, 0 ) = _coefficients( gkg::TwoPoolModel::FastDxy );
    FastD( 1, 1 ) = _coefficients( gkg::TwoPoolModel::FastDyy );
    FastD( 1, 2 ) = _coefficients( gkg::TwoPoolModel::FastDyz );
    FastD( 2, 0 ) = _coefficients( gkg::TwoPoolModel::FastDxz );
    FastD( 2, 1 ) = _coefficients( gkg::TwoPoolModel::FastDyz );
    FastD( 2, 2 ) = _coefficients( gkg::TwoPoolModel::FastDzz );

    gkg::Vector FastEigenValues( 3 );
    gkg::Matrix FastEigenVectors( 3, 3 );
    factory->getEigenSystem( FastD, FastEigenValues, FastEigenVectors );
    factory->sortEigenSystem( FastEigenValues, FastEigenVectors, 
             gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

    _eigenValues( 0 ) = FastEigenValues( 0 );
    _eigenValues( 1 ) = FastEigenValues( 1 );
    _eigenValues( 2 ) = FastEigenValues( 2 );


    _eigenVectors( 0, 0 ) = FastEigenVectors( 0, 0 );
    _eigenVectors( 0, 1 ) = FastEigenVectors( 0, 1 );
    _eigenVectors( 0, 2 ) = FastEigenVectors( 0, 2 );

    _eigenVectors( 1, 0 ) = FastEigenVectors( 1, 0 );
    _eigenVectors( 1, 1 ) = FastEigenVectors( 1, 1 );
    _eigenVectors( 1, 2 ) = FastEigenVectors( 1, 2 );

    _eigenVectors( 2, 0 ) = FastEigenVectors( 2, 0 );
    _eigenVectors( 2, 1 ) = FastEigenVectors( 2, 1 );
    _eigenVectors( 2, 2 ) = FastEigenVectors( 2, 2 );

    if ( _eigenValues( gkg::TwoPoolModel::FastMinimum ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::FastMinimum ) = EPSILON;

    }
    if ( _eigenValues( gkg::TwoPoolModel::FastMiddle ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::FastMiddle ) = EPSILON;

    }
    if ( _eigenValues( gkg::TwoPoolModel::FastMaximum ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::FastMaximum ) = EPSILON;

    }

    gkg::Matrix inverseFastD( 3, 3 );

    gkg::Vector inverseFastEigenValues( 3 );
    inverseFastEigenValues( 0 ) = 1.0 / _eigenValues( 0 );
    inverseFastEigenValues( 1 ) = 1.0 / _eigenValues( 1 );
    inverseFastEigenValues( 2 ) = 1.0 / _eigenValues( 2 );

    inverseFastD = FastEigenVectors.getComposition(
                 inverseFastEigenValues.getDiagonal().getComposition(
                   FastEigenVectors.getTransposition() ) );

    _inverseCoefficients( gkg::TwoPoolModel::FastDxx ) = inverseFastD( 0, 0 );
    _inverseCoefficients( gkg::TwoPoolModel::FastDxy ) = inverseFastD( 0, 1 );
    _inverseCoefficients( gkg::TwoPoolModel::FastDxz ) = inverseFastD( 0, 2 );
    _inverseCoefficients( gkg::TwoPoolModel::FastDyy ) = inverseFastD( 1, 1 );
    _inverseCoefficients( gkg::TwoPoolModel::FastDyz ) = inverseFastD( 1, 2 );
    _inverseCoefficients( gkg::TwoPoolModel::FastDzz ) = inverseFastD( 2, 2 );

    // computing slow pool eigen system and inverse matrix
    gkg::Matrix SlowD( 3, 3 );

    SlowD( 0, 0 ) = _coefficients( gkg::TwoPoolModel::SlowDxx );
    SlowD( 0, 1 ) = _coefficients( gkg::TwoPoolModel::SlowDxy );
    SlowD( 0, 2 ) = _coefficients( gkg::TwoPoolModel::SlowDxz );
    SlowD( 1, 0 ) = _coefficients( gkg::TwoPoolModel::SlowDxy );
    SlowD( 1, 1 ) = _coefficients( gkg::TwoPoolModel::SlowDyy );
    SlowD( 1, 2 ) = _coefficients( gkg::TwoPoolModel::SlowDyz );
    SlowD( 2, 0 ) = _coefficients( gkg::TwoPoolModel::SlowDxz );
    SlowD( 2, 1 ) = _coefficients( gkg::TwoPoolModel::SlowDyz );
    SlowD( 2, 2 ) = _coefficients( gkg::TwoPoolModel::SlowDzz );

    gkg::Vector SlowEigenValues( 3 );
    gkg::Matrix SlowEigenVectors( 3, 3 );
    factory->getEigenSystem( SlowD, SlowEigenValues, SlowEigenVectors );
    factory->sortEigenSystem( SlowEigenValues, SlowEigenVectors, 
             gkg::NumericalAnalysisImplementationFactory::AbsoluteDescending );

    _eigenValues( 3 ) = SlowEigenValues( 0 );
    _eigenValues( 4 ) = SlowEigenValues( 1 );
    _eigenValues( 5 ) = SlowEigenValues( 2 );


    _eigenVectors( 0, 3 ) = SlowEigenVectors( 0, 0 );
    _eigenVectors( 0, 4 ) = SlowEigenVectors( 0, 1 );
    _eigenVectors( 0, 5 ) = SlowEigenVectors( 0, 2 );

    _eigenVectors( 1, 3 ) = SlowEigenVectors( 1, 0 );
    _eigenVectors( 1, 4 ) = SlowEigenVectors( 1, 1 );
    _eigenVectors( 1, 5 ) = SlowEigenVectors( 1, 2 );

    _eigenVectors( 2, 3 ) = SlowEigenVectors( 2, 0 );
    _eigenVectors( 2, 4 ) = SlowEigenVectors( 2, 1 );
    _eigenVectors( 2, 5 ) = SlowEigenVectors( 2, 2 );

    if ( _eigenValues( gkg::TwoPoolModel::SlowMinimum ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::SlowMinimum ) = EPSILON;

    }
    if ( _eigenValues( gkg::TwoPoolModel::SlowMiddle ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::SlowMiddle ) = EPSILON;

    }
    if ( _eigenValues( gkg::TwoPoolModel::SlowMaximum ) < 0.0 )
    {

      _eigenValues( gkg::TwoPoolModel::SlowMaximum ) = EPSILON;

    }

    gkg::Matrix inverseSlowD( 3, 3 );

    gkg::Vector inverseSlowEigenValues( 3 );
    inverseSlowEigenValues( 0 ) = 1.0 / _eigenValues( 3 );
    inverseSlowEigenValues( 1 ) = 1.0 / _eigenValues( 4 );
    inverseSlowEigenValues( 2 ) = 1.0 / _eigenValues( 5 );

    inverseSlowD = SlowEigenVectors.getComposition(
                 inverseSlowEigenValues.getDiagonal().getComposition(
                   SlowEigenVectors.getTransposition() ) );

    _inverseCoefficients( gkg::TwoPoolModel::SlowDxx ) = inverseSlowD( 0, 0 );
    _inverseCoefficients( gkg::TwoPoolModel::SlowDxy ) = inverseSlowD( 0, 1 );
    _inverseCoefficients( gkg::TwoPoolModel::SlowDxz ) = inverseSlowD( 0, 2 );
    _inverseCoefficients( gkg::TwoPoolModel::SlowDyy ) = inverseSlowD( 1, 1 );
    _inverseCoefficients( gkg::TwoPoolModel::SlowDyz ) = inverseSlowD( 1, 2 );
    _inverseCoefficients( gkg::TwoPoolModel::SlowDzz ) = inverseSlowD( 2, 2 );

    // computing slow pool eigen system and inverse matrix
    _inverseCoefficients( gkg::TwoPoolModel::FastVolumeFraction ) = 
      1.0 - _inverseCoefficients( gkg::TwoPoolModel::FastVolumeFraction );


  }
  GKG_CATCH( "void gkg::TwoPoolModel::doEigenSystemAndInverseMatrix()" );

}


void gkg::TwoPoolModel::doScaling()
{

  try
  {

    // we assume that the b-value is given in s/mm^2, so use 1e6 factor
    // to restore the b-value in s/m^2
    // Thus, diffusion coefficients are given in m^2/s, normally close
    // to 2.2e-9 m^2/s for water, at 25 degrees
    _coefficients( gkg::TwoPoolModel::FastDxx ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::FastDxy ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::FastDxz ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::FastDyy ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::FastDyz ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::FastDzz ) *= 1e-6;

    _coefficients( gkg::TwoPoolModel::SlowDxx ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::SlowDxy ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::SlowDxz ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::SlowDyy ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::SlowDyz ) *= 1e-6;
    _coefficients( gkg::TwoPoolModel::SlowDzz ) *= 1e-6;

    _inverseCoefficients( gkg::TwoPoolModel::FastDxx ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::FastDxy ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::FastDxz ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::FastDyy ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::FastDyz ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::FastDzz ) *= 1e+6;

    _inverseCoefficients( gkg::TwoPoolModel::SlowDxx ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::SlowDxy ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::SlowDxz ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::SlowDyy ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::SlowDyz ) *= 1e+6;
    _inverseCoefficients( gkg::TwoPoolModel::SlowDzz ) *= 1e+6;

    _eigenValues( gkg::TwoPoolModel::FastMinimum ) *= 1e-6;
    _eigenValues( gkg::TwoPoolModel::FastMiddle ) *= 1e-6;
    _eigenValues( gkg::TwoPoolModel::FastMaximum ) *= 1e-6;

    _eigenValues( gkg::TwoPoolModel::SlowMinimum ) *= 1e-6;
    _eigenValues( gkg::TwoPoolModel::SlowMiddle ) *= 1e-6;
    _eigenValues( gkg::TwoPoolModel::SlowMaximum ) *= 1e-6;

  }
  GKG_CATCH( "void gkg::TwoPoolModel::doScaling()" );

}





#undef EPSILON
