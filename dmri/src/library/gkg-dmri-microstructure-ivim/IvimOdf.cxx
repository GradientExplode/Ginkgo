#include <gkg-dmri-microstructure-ivim/IvimOdf.h>
#include <gkg-dmri-microstructure-ivim/IvimNLPFunction.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-resampling/PartialVolumingQuickResampler.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-core-exception/Exception.h>
#include <cmath>


#define EPSILON 1e-6


gkg::IvimOdf::IvimOdf(
             const gkg::OrientationSet* orientationSet,
             const gkg::Volume< float >& t2,
             const gkg::Volume< float >& dw,
             const gkg::Vector3d< int32_t >& site,
             const gkg::Tensor& parenchymaTensor,
             int32_t shellCount,
             const std::vector< float >& shellBValues,
             const std::vector< int32_t >& shellIndexOffsets,
             const std::vector< int32_t >& shellOrientationCounts,
             int32_t /*ivimToParenchymaTransitionShellIndex*/,
             int32_t parenchymaGaussianToNonGaussianTransitionShellIndex,
             const gkg::Vector& initialParameters,
             double noiseVariance,
             const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& 
                                                           optimizerConstraints,
             const std::vector< double >& nlpOptimizerParameters )
              : gkg::OrientationDistributionFunction(
                                     gkg::OrientationDistributionFunction::Ivim,
                                     orientationSet )
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
      if ( averageT2 <= EPSILON )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount );
      for ( t = 0; t < orientationCount; t++ )
      {

        float dwValue = dw( site, t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

      }


      //////////////////////////////////////////////////////////////////////////
      // degenerated signal
      //////////////////////////////////////////////////////////////////////////
      if ( averageT2 <= EPSILON )
      {

        _values.resize( 9U );

        // setting IVIM fraction to zero
        _values[ 0 ] = 0.0;
        // setting IVIM diffusivity to 1e-8 m2/s
        _values[ 1 ] = 1e-8f;
        // setting parenchyma diffusivity to 0.7e-9 m2/s
        _values[ 2 ] = 0.7e-9f;
        // setting an isotropic parenchyma tensor
        _values[ 3 ] = _values[ 2 ] / 3.0f;
        _values[ 4 ] = 0.0f;
        _values[ 5 ] = 0.0f;
        _values[ 6 ] = _values[ 2 ] / 3.0f;
        _values[ 7 ] = 0.0f;
        _values[ 8 ] = _values[ 2 ] / 3.0f;

      }
      else
      {

        // computing the powder-average signal
        _powderAverage.resize( shellCount );

        int32_t shellIndex = 0;
        for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
        {

          int32_t orientationIndex = 0;
          for ( orientationIndex = 0;
                orientationIndex < shellOrientationCounts[ shellIndex ];
                orientationIndex++ )
          {

            _powderAverage[ shellIndex ] +=
              ( float )normalizedSignal( shellIndexOffsets[ shellIndex ] +
                                          orientationIndex );

          }
          _powderAverage[ shellIndex ] /=
                                 ( float )shellOrientationCounts[ shellIndex ];

        }

        // collecting parenchym diffusivity from estimated tensor
        float parenchymaDiffusivity =
                             parenchymaTensor.getApparentDiffusionCoefficient();

        // creating the NLP IVIM function
        gkg::IvimNLPFunction
          ivimNLPFunction( _powderAverage,
                           noiseVariance,
                           shellBValues,
                           parenchymaGaussianToNonGaussianTransitionShellIndex,
                           ( double )parenchymaDiffusivity );


        // getting a pointer to the numerical analysis factory
        gkg::NumericalAnalysisImplementationFactory* 
          factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

        // fitting the equation:
        //     A = fIVIM * exp( -b D* ) + ( 1 - fIVIM ) * exp( -b Dparenchyma )
        gkg::Vector parameters( initialParameters );
        factory->getConstrainedNonLinearProgrammingOptimumParameters(
                gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
                parameters,
                optimizerConstraints,
                ivimNLPFunction,
                nlpOptimizerParameters,
                false );

        // reallocating _values
        _values.resize( 9U );

        // setting IVIM fraction to zero
        _values[ 0 ] = ( float )parameters( 0 );
        // setting IVIM diffusivity to 1e-8 m2/s
        _values[ 1 ] = ( float )parameters( 1 );
        // setting parenchyma diffusivity to 0.7e-9 m2/s
        _values[ 2 ] = parenchymaDiffusivity;
        // setting the parenchyma tensor
        _values[ 3 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxx );
        _values[ 4 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxy );
        _values[ 5 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxz );
        _values[ 6 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dyy );
        _values[ 7 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dyz );
        _values[ 8 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dzz );

        // computing the fitted measurement volume
        _fittedMeasurements.reallocate( shellCount );
        ivimNLPFunction.getValuesAt( parameters, _fittedMeasurements );

      }

    }

  }
  GKG_CATCH( "gkg::IvimOdf::IvimOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< int32_t >& site, "
             "const Tensor& parenchymaTensor, "
             "int32_t shellCount, "
             "const std::vector< float >& shellBValues, "
             "const std::vector< int32_t >& shellIndexOffsets, "
             "const std::vector< int32_t >& shellOrientationCounts, "
             "int32_t /*ivimToParenchymaTransitionShellIndex*/, "
             "int32_t parenchymaGaussianToNonGaussianTransitionShellIndex, "
             "const gkg::Vector& initialParameters, "
             "double noiseVariance, "
             "const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& "
             "optimizerConstraints, "
             "const std::vector< double >& nlpOptimizerParameters )" );

}


gkg::IvimOdf::IvimOdf(
             const gkg::OrientationSet* orientationSet,
             const gkg::Volume< float >& t2,
             const gkg::Volume< float >& dw,
             const gkg::Vector3d< float >& site,
             const gkg::Tensor& parenchymaTensor,
             int32_t shellCount,
             const std::vector< float >& shellBValues,
             const std::vector< int32_t >& shellIndexOffsets,
             const std::vector< int32_t >& shellOrientationCounts,
             int32_t /*ivimToParenchymaTransitionShellIndex*/,
             int32_t parenchymaGaussianToNonGaussianTransitionShellIndex,
             const gkg::Vector& initialParameters,
             double noiseVariance,
             const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& 
                                                           optimizerConstraints,
             const std::vector< double >& nlpOptimizerParameters )
              : gkg::OrientationDistributionFunction(
                                     gkg::OrientationDistributionFunction::Ivim,
                                     orientationSet )
{

  try
  {

    if ( _orientationSet )
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
      if ( averageT2 <= EPSILON )
      {

        averageT2 = EPSILON;

      }

      // building normalized signal vector
      gkg::Vector normalizedSignal( orientationCount );
      float dwValue = 0;
      for ( t = 0; t < orientationCount; t++ )
      {

        resampler.resample( dw,
                            ( float )0,
                            gkg::Vector3d< double >( site.x, site.y, site.z ),
                            dwValue,
                            &resolution,
                            t );
        normalizedSignal( t ) = ( float )dwValue / ( float )averageT2;

      }

      //////////////////////////////////////////////////////////////////////////
      // degenerated signal
      //////////////////////////////////////////////////////////////////////////
      if ( averageT2 <= EPSILON )
      {

        _values.resize( 9U );

        // setting IVIM fraction to zero
        _values[ 0 ] = 0.0;
        // setting IVIM diffusivity to 1e-8 m2/s
        _values[ 1 ] = 1e-8f;
        // setting parenchyma diffusivity to 0.7e-9 m2/s
        _values[ 2 ] = 0.7e-9f;
        // setting an isotropic parenchyma tensor
        _values[ 3 ] = _values[ 2 ] / 3.0f;
        _values[ 4 ] = 0.0f;
        _values[ 5 ] = 0.0f;
        _values[ 6 ] = _values[ 2 ] / 3.0f;
        _values[ 7 ] = 0.0f;
        _values[ 8 ] = _values[ 2 ] / 3.0f;

      }
      else
      {

        // computing the powder-average signal
        _powderAverage.resize( shellCount );

        int32_t shellIndex = 0;
        for ( shellIndex = 0; shellIndex < shellCount; shellIndex++ )
        {

          int32_t orientationIndex = 0;
          for ( orientationIndex = 0;
                orientationIndex < shellOrientationCounts[ shellIndex ];
                orientationIndex++ )
          {

            _powderAverage[ shellIndex ] +=
              ( float )normalizedSignal( shellIndexOffsets[ shellIndex ] +
                                          orientationIndex );

          }
          _powderAverage[ shellIndex ] /=
                                 ( float )shellOrientationCounts[ shellIndex ];

        }

        // collecting parenchym diffusivity from estimated tensor
        float parenchymaDiffusivity =
                             parenchymaTensor.getApparentDiffusionCoefficient();

        // creating the NLP IVIM function
        gkg::IvimNLPFunction
          ivimNLPFunction( _powderAverage,
                           noiseVariance,
                           shellBValues,
                           parenchymaGaussianToNonGaussianTransitionShellIndex,
                           ( double )parenchymaDiffusivity );


        // getting a pointer to the numerical analysis factory
        gkg::NumericalAnalysisImplementationFactory* 
          factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

        // fitting the equation:
        //     A = fIVIM * exp( -b D* ) + ( 1 - fIVIM ) * exp( -b Dparenchyma )
        gkg::Vector parameters( initialParameters );
        factory->getConstrainedNonLinearProgrammingOptimumParameters(
                gkg::NumericalAnalysisImplementationFactory::NelderMeadSimplex2,
                parameters,
                optimizerConstraints,
                ivimNLPFunction,
                nlpOptimizerParameters,
                false );

        // reallocating _values
        _values.resize( 9U );

        // setting IVIM fraction to zero
        _values[ 0 ] = ( float )parameters( 0 );
        // setting IVIM diffusivity to 1e-8 m2/s
        _values[ 1 ] = ( float )parameters( 1 );
        // setting parenchyma diffusivity to the tensor ADC
        _values[ 2 ] = parenchymaDiffusivity;
        // setting the parenchyma tensor
        _values[ 3 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxx );
        _values[ 4 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxy );
        _values[ 5 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dxz );
        _values[ 6 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dyy );
        _values[ 7 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dyz );
        _values[ 8 ] = parenchymaTensor.getCoefficient( gkg::Tensor::Dzz );

        // computing the fitted measurement volume
        _fittedMeasurements.reallocate( shellCount );
        ivimNLPFunction.getValuesAt( parameters, _fittedMeasurements );

      }

    }

  }
  GKG_CATCH( "gkg::IvimOdf::IvimOdf( "
             "const gkg::OrientationSet* orientationSet, "
             "const gkg::Volume< float >& t2, "
             "const gkg::Volume< float >& dw, "
             "const gkg::Vector3d< float >& site, "
             "const Tensor& parenchymaTensor, "
             "int32_t shellCount, "
             "const std::vector< float >& shellBValues, "
             "const std::vector< int32_t >& shellIndexOffsets, "
             "const std::vector< int32_t >& shellOrientationCounts, "
             "int32_t /*ivimToParenchymaTransitionShellIndex*/, "
             "int32_t parenchymaGaussianToNonGaussianTransitionShellIndex, "
             "const gkg::Vector& initialParameters, "
             "double noiseVariance, "
             "const std::vector< gkg::RCPointer< gkg::OptimizerConstraint > >& "
             "optimizerConstraints, "
             "const std::vector< double >& nlpOptimizerParameters )" );

}


gkg::IvimOdf::~IvimOdf()
{
}


const float& 
gkg::IvimOdf::getIvimFraction() const
{

  try
  {

    return _values[ 0 ];

  }
  GKG_CATCH( "const float& "
             "gkg::IvimOdf::getIvimFraction() const" );

}


const float& 
gkg::IvimOdf::getIvimDiffusivity() const
{

  try
  {

    return _values[ 1 ];

  }
  GKG_CATCH( "const float& "
             "gkg::IvimOdf::getIvimDiffusivity() const" );

}


const float& 
gkg::IvimOdf::getParenchymaDiffusivity() const
{

  try
  {

    return _values[ 2 ];

  }
  GKG_CATCH( "const float& "
             "gkg::IvimOdf::getParenchymaDiffusivity() const" );

}


gkg::Tensor
gkg::IvimOdf::getParenchymaTensor() const
{

  try
  {

    return gkg::Tensor( _values[ 3 ],
                        _values[ 4 ],
                        _values[ 5 ],
                        _values[ 6 ],
                        _values[ 7 ],
                        _values[ 8 ] );

  }
  GKG_CATCH( "gkg::Tensor "
             "gkg::IvimOdf::getParenchymaTensor() const" );

}


gkg::Vector
gkg::IvimOdf::getFittedMeasurements() const
{

  try
  {

    return _fittedMeasurements;

  }
  GKG_CATCH( "gkg::Vector "
             "gkg::IvimOdf::getFittedMeasurements() const" );

}


std::vector< float > 
gkg::IvimOdf::getPowderAverage() const
{

  try
  {

    return _powderAverage;

  }
  GKG_CATCH( "std::vector< float > "
             "gkg::IvimOdf::getPowderAverage() const" );

}


#undef EPSILON
