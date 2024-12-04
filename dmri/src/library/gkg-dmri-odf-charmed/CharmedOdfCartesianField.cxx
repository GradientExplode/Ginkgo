#include <gkg-dmri-odf-charmed/CharmedOdfCartesianField.h>
#include <gkg-dmri-odf-charmed/CharmedOdf.h>
#include <gkg-dmri-odf-charmed/CharmedOdfCartesianFieldFunctor.h>
#include <gkg-dmri-odf-charmed/CharmedMCMCFunction.h>
#include <gkg-dmri-odf-charmed/CharmedMCMCParameterSet.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::CharmedOdfCartesianField::CharmedOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                            : gkg::OdfCartesianField( t2, dw, mask,
                                                      outputOrientationSet )
{

  try
  {

    // getting the CHARMED type, one or two compartment(s)
    if ( specificStringParameters[ 0 ] == "one-compartment" )
    {

      _type = gkg::CharmedOdfCartesianField::OneCompartment;

    }
    else if ( specificStringParameters[ 0 ] == "two-compartments" )
    {

      _type = gkg::CharmedOdfCartesianField::TwoCompartments;

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting ODF type and functor
    _items.getHeader().addAttribute( "odf_type",
                                    gkg::TypeOf< gkg::CharmedOdf >::getName() );

    this->_functor = new gkg::CharmedOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::CharmedOdfCartesianField::"
             "CharmedOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::CharmedOdfCartesianField::CharmedOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< float >& boundingBox,
                 const gkg::Vector3d< float >& superResolution,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        superResolution,
                                                        outputOrientationSet )
{

  try
  {

    // getting the CHARMED type, one or two compartment(s)
    if ( specificStringParameters[ 0 ] == "one-compartment" )
    {

      _type = gkg::CharmedOdfCartesianField::OneCompartment;

    }
    else if ( specificStringParameters[ 0 ] == "two-compartments" )
    {

      _type = gkg::CharmedOdfCartesianField::TwoCompartments;

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       superResolution,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::CharmedOdf >::getName() );

    this->_functor = new gkg::CharmedOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::CharmedOdfCartesianField::"
             "CharmedOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< float >& boundingBox, "
             "const gkg::Vector3d< float >& superResolution, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::CharmedOdfCartesianField::CharmedOdfCartesianField(
                 gkg::RCPointer< gkg::Volume< float > > t2,
                 gkg::RCPointer< gkg::Volume< float > > dw,
                 gkg::RCPointer< gkg::Volume< int16_t > > mask,
                 const gkg::BoundingBox< int32_t >& boundingBox,
                 const gkg::OrientationSet* outputOrientationSet,
                 const std::vector< double >& specificScalarParameters,
                 const std::vector< std::string >& specificStringParameters,
                 bool verbose )
                              : gkg::OdfCartesianField( t2, dw, mask,
                                                        boundingBox,
                                                        outputOrientationSet )
{

  try
  {

    // getting the CHARMED type, one or two compartment(s)
    if ( specificStringParameters[ 0 ] == "one-compartment" )
    {

      _type = gkg::CharmedOdfCartesianField::OneCompartment;

    }
    else if ( specificStringParameters[ 0 ] == "two-compartments" )
    {

      _type = gkg::CharmedOdfCartesianField::TwoCompartments;

    }

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // computing tensor cartesian field
    std::vector< double > dtiSpecificScalarParameters;
    std::vector< std::string > dtiSpecificStringParameters( 1 );
    dtiSpecificStringParameters[ 0 ] = "robust_positive_definite";
    _tensorCartesianField.reset( new gkg::TensorCartesianField(
                                       t2,
                                       dw,
                                       mask,
                                       boundingBox,
                                       dtiSpecificScalarParameters,
                                       dtiSpecificStringParameters,
                                       verbose ) );

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                            "odf_type",
                            gkg::TypeOf< gkg::CharmedOdf >::getName() );

    this->_functor = new gkg::CharmedOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction(  verbose );

    // computing ODFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::CharmedOdfCartesianField::"
             "CharmedOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::CharmedOdfCartesianField::~CharmedOdfCartesianField()
{
}

gkg::CharmedOdfCartesianFieldType gkg::CharmedOdfCartesianField::getType() const
{

  try
  {

    return _type;

  }
  GKG_CATCH( "gkg::CharmedOdfCartesianFieldType "
             "gkg::CharmedOdfCartesianField::getType() const" );

}

const gkg::Vector& gkg::CharmedOdfCartesianField::getInitialParameters() const
{

  try
  {

    return _initialParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::CharmedOdfCartesianField::getInitialParameters() const" );

}


const gkg::Vector& gkg::CharmedOdfCartesianField::getLowerParameters() const
{

  try
  {

    return _lowerParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::CharmedOdfCartesianField::getLowerParameters() const" );

}


const gkg::Vector& gkg::CharmedOdfCartesianField::getUpperParameters() const
{

  try
  {

    return _upperParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::CharmedOdfCartesianField::getUpperParameters() const" );

}


const gkg::Vector& gkg::CharmedOdfCartesianField::getDeltaParameters() const
{

  try
  {

    return _deltaParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::CharmedOdfCartesianField::getDeltaParameters() const" );

}


const std::vector< bool >& gkg::CharmedOdfCartesianField::getFixed() const
{

  try
  {

    return _fixed;

  }
  GKG_CATCH( "const std::vector< bool >& "
             "gkg::CharmedOdfCartesianField::getFixed() const" );

}


double gkg::CharmedOdfCartesianField::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( "double "
             "gkg::CharmedOdfCartesianField::getNoiseStandardDeviation() "
             "const" );

}


int32_t gkg::CharmedOdfCartesianField::getQuickSearchStepCount() const
{

  try
  {

    return _quicksearchStepCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getQuickSearchStepCount() "
             "const" );

}


int32_t gkg::CharmedOdfCartesianField::getNLPMaximumIterationCount() const
{

  try
  {

    return _nlpMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getNLPMaximumIterationCount() "
             "const" );

}


double gkg::CharmedOdfCartesianField::getNLPMaximumSizeOfTheSimplex() const
{

  try
  {

    return _nlpMaximumSizeOfTheSimplex;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getNLPMaximumSizeOfTheSimplex() "
             "const" );

}


int32_t gkg::CharmedOdfCartesianField::getMCMCBurninCount() const
{

  try
  {

    return _mcmcBurninCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getMCMCBurninCount() const" );

}


int32_t gkg::CharmedOdfCartesianField::getMCMCSampleCount() const
{

  try
  {

    return _mcmcSampleCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getMCMCSampleCount() const" );

}


int32_t gkg::CharmedOdfCartesianField::getMCMCIntervalCount() const
{

  try
  {

    return _mcmcIntervalCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getMCMCIntervalCount() const" );

}


int32_t gkg::CharmedOdfCartesianField::getMCMCMaximumIterationCount() const
{

  try
  {

    return _mcmcMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::CharmedOdfCartesianField::getMCMCMaximumIterationCount() const" );

}

//////////////////////////////////////////////////////////////
void gkg::CharmedOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "one-compartment";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "only one string parameter required" );

      }

      if ( ( specificStringParameters[ 0 ] != "one-compartment" ) &&
           ( specificStringParameters[ 0 ] != "two-compartments" ) )
      {

        throw std::runtime_error( 
         "CHARMED type must be one of one-compartment/two-compartments" );

      }

    }

    if ( specificScalarParameters.empty() )
    {

      if ( specificStringParameters[ 0 ] == "one-compartment" )
      {

        specificScalarParameters.resize( 42 );

        // <initial_radius>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialRadius ] = 1e-6;
        // <initial_isotropic_fraction>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialIsotropicFraction ] = 0.2;
        // <initial_isotropic_diffusivity>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialIsotropicDiffusivity ] = 3.0e-9;
        // <initial_parallel_diffusivity>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialParallelDiffusivity ] = 1.0e-9;
        // <initial_perpendicular_diffusivity>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialPerpendicularDiffusivity ] = 1.e-9;
        // <initial_restricted_fraction>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::
          OneCompartment_InitialRestrictedFraction ] = 0.5;
        // <initial_theta>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::InitialTheta ] = M_PI;
        // <initial_phi>
        specificScalarParameters[
          gkg::CharmedOdfCartesianField::InitialPhi ] = M_PI/2.0;

      }
      else if ( specificStringParameters[ 0 ] == "two-compartments" )
      {

        specificScalarParameters.resize( 48 );

      }

    }
    else
    {

      if ( specificStringParameters[ 0 ] == "one-compartment" )
      {

      }
      else if ( specificStringParameters[ 0 ] == "two-compartments" )
      {

      }

    }


//////////////////////////////
    if ( specificScalarParameters.empty() )
    {
      
      specificScalarParameters.resize( 52 );
      
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::RestrictedCompartmentsCount ] = 1;

      // <initial_radius>
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::InitialRadius ] = 1e-6;
      // <initial_isotropic_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialIsotropicFraction ] = 0.2;
      // <initial_isotropic_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialIsotropicDiffusivity ] = 3.0e-9;
      // <initial_parallel_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialParallelDiffusivity ] = 1.0e-9;
      // <initial_perpendicular_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialPerpendicularDiffusivity ] = 1.e-9;
      // <initial_restricted_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialRestrictedFraction ] = 0.5;
      // <initial_theta_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialTheta ] = M_PI;
      // <initial_phi_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::InitialPhi ] = M_PI/2.0;        
        
      // <lower_radius>
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::LowerRadius ] = 0.0;
      // <lower_isotropic_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerIsotropicFraction ] = 0.0;  
      // <lower_isotropic_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerIsotropicDiffusivity ] = 0.0;  
/*      // <lower_lambda_parallel>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerLambdaParallel ] = 0.0;
      // <lower_lambda_transverse>  
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerLambdaTransverse ] = 0.0;  */      
      // <lower_parallel_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerParallelDiffusivity ] = 0.0;
      // <lower_perpendicular_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerPerpendicularDiffusivity ] = 0.0;
      // <lower_restricted_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerRestrictedFraction ] = 0.0;
      // <lower_theta_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerTheta ] = 0.0;
      // <lower_phi_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::LowerPhi ] = 0.0;  

      // <upper_radius>
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::UpperRadius ] = 1e-3;
      // <upper_isotropic_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperIsotropicFraction ] = 1.0;    
      // <upper_isotropic_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperIsotropicDiffusivity ] = 1.0e-8;  
/*      // <upper_lambda_parallel>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperLambdaParallel ] = 5.0e-9;
      // <upper_lambda_transverse>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperLambdaTransverse ] = 5.0e-9;  */      
      // <upper_parallel_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperParallelDiffusivity ] = 5.0e-9;
      // <upper_perpendicular_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperPerpendicularDiffusivity ] = 5.0e-9;
      // <upper_restricted_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperRestrictedFraction ] = 1.0;
      // <upper_theta_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperTheta ] = 2 * M_PI;
      // <upper_phi_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::UpperPhi ] = M_PI;  

      // <delta_radius>
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::DeltaRadius ] = 0.05e-6;
      // <delta_isotropic_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaIsotropicFraction ] = 0.05;    
      // <delta_isotropic_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaIsotropicDiffusivity ] = 0.5e-9; 
/*      // <delta_lambda_parallel>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaLambdaParallel ] = 0.25e-9;
      // <delta_lambda_transverse>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaLambdaTransverse ] = 0.25e-9; */       
      // <delta_parallel_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaParallelDiffusivity ] = 0.25e-9;
      // <delta_perpendicular_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaPerpendicularDiffusivity ] = 0.25e-9;
      // <delta_restricted_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaRestrictedFraction ] = 0.05;
      // <delta_theta_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaTheta ] = 0.1;
      // <delta_phi_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::DeltaPhi ] = 0.1;  

      // <fixed_radius>
      specificScalarParameters[
        gkg::CharmedOdfCartesianField::FixedRadius ] = true;
      // <fixed_isotropic_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedIsotropicFraction ] = false;
      // <fixed_isotropic_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedIsotropicDiffusivity ] = true; 
/*      // <fixed_lambda_parallel>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedLambdaParallel ] = true;
      // <fixed_lambda_transverse>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedLambdaTransverse ] = true; */       
      // <fixed_parallel_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedParallelDiffusivity ] = false;
      // <fixed_perpendicular_diffusivity>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedPerpendicularDiffusivity ] = true;
      // <fixed_restricted_fraction>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedRestrictedFraction ] = false;
      // <fixed_theta_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedTheta ] = false;
      // <fixed_phi_1>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::FixedPhi ] = false;  

      // <noise_standard_deviation>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::NoiseStandardDeviation ] = 50;

      // <apply_quicksearch_optimizer>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::ApplyQuickSearchOptimizer ] = 1;
      // <quicksearch_step_count>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::QuickSearchStepCount ] = 10;

      // <apply_nlp_optimizer>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::ApplyNlpOptimizer ] = 1;
      // <nlp_maximum_iteration_count>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::NlpMaximumIterationCount ] = 1000;
      // <nlp_maximum_size_of_the_simplex>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::NlpMaximumSizeOfTheSimplex ] = 0.001;

      // <apply_mcmc_optimizer>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::ApplyMcmcOptimizer ] = 1;
      // <mcmc_burnin>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::McmcBurninCount ] = 10000;
      // <mcmc_sample_count>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::McmcSampleCount ] = 100;
      // <mcmc_interval_count>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::McmcIntervalCount ] = 1000;
      // <mcmc_maximum_iteration_count>
      specificScalarParameters[ 
        gkg::CharmedOdfCartesianField::McmcMaximumIterationCount ] = 50000;

    }
    else
    {

      // check(s) for restricted compartments count////////////////////////////
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::RestrictedCompartmentsCount ] < 1
           && specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::RestrictedCompartmentsCount ] > 3 )
      {

        throw std::runtime_error(
                          "restricted compartments count must be between " 
                          " 1 and 3 " );

      }
      
      // check(s) for radius ///////////////////////////////////
      // <lower_radius>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerRadius ] < 0.0 )
      {

        throw std::runtime_error(
                          "radius must be positive" );

      }
      
      // <lower_radius> < <upper_radius>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerRadius ] > 
           specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::UpperRadius ])
      {

        throw std::runtime_error(
                          "lower radius is greater than "
                          "upper radius" );

      }

      // <initial_radius>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialRadius ] < 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerRadius ] ) ||
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialRadius ] > 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperRadius ] ) )
      {
        std::cout << "yo " << specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperRadius ] << std::endl;
        throw std::runtime_error( 
                           "initial radius must be between "
                           "lower and upper bounds" );

      }

      // <delta_radius>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::DeltaRadius ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperRadius ] ) -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerRadius ] ) )
      {

        throw std::runtime_error( 
                           "delta radius must be lower "
                           "than upper bound minus lower bound" );

      }

      
      // check(s) for isotropic fraction /////////////////////////////////////
      // <lower_isotropic_fraction>
      if ( specificScalarParameters[ 
             gkg::CharmedOdfCartesianField::LowerIsotropicFraction ] < 0.0 )
      {

        throw std::runtime_error(
                               "lower isotropic fraction must be positive" );

      }
      // <upper_isotropic_fraction>
      if ( specificScalarParameters[ 
             gkg::CharmedOdfCartesianField::UpperIsotropicFraction ] > 1.0 )
      {

        throw std::runtime_error(
                  "upper isotropic fraction must be lower or equal to 1.0" );

      }
      // <lower_isotropic_fraction> < <upper_isotropic_fraction>
      if ( specificScalarParameters[ LowerIsotropicFraction ] >
           specificScalarParameters[ UpperIsotropicFraction ] )
      {

        throw std::runtime_error( "lower isotropic fraction is greater than "
                                  "upper isotropic fraction" );

      }
      // <initial_isotropic_fraction>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialIsotropicFraction ] < 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerIsotropicFraction ] ) ||
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialIsotropicFraction ] > 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperIsotropicFraction ] ) )
      {

        throw std::runtime_error( 
        "initial isotropic fraction must be between lower and upper bounds" );

      }
      // <delta_isotropic_fraction>
      if ( specificScalarParameters[ 
             gkg::CharmedOdfCartesianField::DeltaIsotropicFraction ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperIsotropicFraction ] -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerIsotropicFraction ] ) )
      {

        throw std::runtime_error( 
          "delta isotropic fraction must be lower than upper bound minus "
          "lower bound" );
      }
 

      // check for combined isotropic & intracellular ////////////////////////
      if ( ( specificScalarParameters[ 
              gkg::CharmedOdfCartesianField::InitialIsotropicFraction ] +
             specificScalarParameters[ 
              gkg::CharmedOdfCartesianField::InitialRestrictedFraction ] )
                                                                      > 1.0 )
      {

        throw std::runtime_error( 
                          "the sum of isotropic & restricted fractions is "
                          "greater than 1.0" );

      }
      

      // check(s) for parallel_diffusivity ///////////////////////////////////
      // <lower_parallel_diffusivity>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerParallelDiffusivity ] < 0.0 )
      {

        throw std::runtime_error(
                          "parallel diffusivity must be positive" );

      }
      
      // <lower_parallel_diffusivity> < <upper_parallel_diffusivity>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerParallelDiffusivity ] > 
           specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::UpperParallelDiffusivity ])
      {

        throw std::runtime_error(
                          "lower parallel diffusivity is greater than "
                          "upper parallel diffusivity" );

      }

      // <initial_parallel_diffusivity>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialParallelDiffusivity ] < 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerParallelDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialParallelDiffusivity ] > 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperParallelDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "initial parallel diffusivity must be between "
                           "lower and upper bounds" );

      }

      // <delta_parallel_diffusivity>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::DeltaParallelDiffusivity ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperParallelDiffusivity ] ) -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerParallelDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "delta parallel diffusivity must be lower "
                           "than upper bound minus lower bound" );

      }

      // check(s) for perpendicular_diffusivity /////////////////////////////////
      // <initial_perpendicular_diffusivity>
      if ( specificScalarParameters[ 
         gkg::CharmedOdfCartesianField::InitialPerpendicularDiffusivity ] < 0.0 )
      {

        throw std::runtime_error(
                         "perpendicular diffusivity fraction must be positive" );

      }
      
      // <lower_perpendicular_diffusivity> < <upper_perpendicular_diffusivity>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerPerpendicularDiffusivity ] > 
           specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::UpperPerpendicularDiffusivity ])
      {

        throw std::runtime_error(
                          "lower perpendicular diffusivity is greater than "
                          "upper perpendicular diffusivity" );

      }

      // <initial_perpendicular_diffusivity>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialPerpendicularDiffusivity ] < 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerPerpendicularDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialPerpendicularDiffusivity ] > 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperPerpendicularDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "initial parallel diffusivity must be between "
                           "lower and upper bounds" );

      }

      // <delta_perpendicular_diffusivity>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::DeltaPerpendicularDiffusivity ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperPerpendicularDiffusivity ] ) -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerPerpendicularDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "delta perpendicular diffusivity must be lower than "
                           "upper bound minus lower bound" );

      }      
      
      // check(s) for isotropic_diffusivity /////////////////////////////////
      // <initial_isotropic_diffusivity>
      if ( specificScalarParameters[ 
         gkg::CharmedOdfCartesianField::InitialIsotropicDiffusivity ] < 0.0 )
      {

        throw std::runtime_error(
                         "isotropic diffusivity fraction must be positive" );

      }
      
      // <lower_isotropic_diffusivity> < <upper_isotropic_diffusivity>
      if ( specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::LowerIsotropicDiffusivity ] > 
           specificScalarParameters[ 
          gkg::CharmedOdfCartesianField::UpperIsotropicDiffusivity ])
      {

        throw std::runtime_error(
                          "lower isotropic diffusivity is greater than "
                          "upper isotropic diffusivity" );

      }

      // <initial_isotropic_diffusivity>
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialIsotropicDiffusivity ] < 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerIsotropicDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::InitialIsotropicDiffusivity ] > 
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperIsotropicDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "initial parallel diffusivity must be between "
                           "lower and upper bounds" );

      }
// 
//       // <delta_isotropic_diffusivity>
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::DeltaIsotropicDiffusivity ] >= 
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::UpperIsotropicDiffusivity ] ) -
//              specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::LowerIsotropicDiffusivity ] ) )
//       {
// 
//         throw std::runtime_error( 
//                            "delta isotropic diffusivity must be lower than "
//                            "upper bound minus lower bound" );
// 
//       }
      
      // check(s) for lambda_parallel /////////////////////////////////      
      // <lower_lambda_parallel> < <upper_lambda_parallel>
//       if ( specificScalarParameters[ 
//           gkg::CharmedOdfCartesianField::LowerLambdaParallel ] > 
//            specificScalarParameters[ 
//           gkg::CharmedOdfCartesianField::UpperLambdaParallel ])
//       {
// 
//         throw std::runtime_error(
//                           "lower lambda parallel is greater than "
//                           "upper lambda parallel" );
// 
//       }

//       // <initial_lambda_parallel>
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::InitialLambdaParallel ] < 
//              specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::LowerLambdaParallel ] ) ||
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::InitialLambdaParallel ] > 
//              specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::UpperLambdaParallel ] ) )
//       {
// 
//         throw std::runtime_error( 
//                            "initial parallel diffusivity must be between "
//                            "lower and upper bounds" );
// 
//       }

      // <delta_lambda_parallel>
/*      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::DeltaLambdaParallel ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperLambdaParallel ] ) -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerLambdaParallel ] ) )
      {

        throw std::runtime_error( 
                           "delta lambda parallel must be lower than "
                           "upper bound minus lower bound" );

      } */   
      
      // check(s) for lambda_transverse /////////////////////////////////
      
      // <lower_lambda_transverse> < <upper_lambda_transverse>
//       if ( specificScalarParameters[ 
//           gkg::CharmedOdfCartesianField::LowerLambdaTransverse ] > 
//            specificScalarParameters[ 
//           gkg::CharmedOdfCartesianField::UpperLambdaTransverse ])
//       {
// 
//         throw std::runtime_error(
//                           "lower lambda transverse is greater than "
//                           "upper lambda transverse" );
// 
//       }

//       // <initial_lambda_transverse>
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::InitialLambdaTransverse ] < 
//              specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::LowerLambdaTransverse ] ) ||
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::InitialLambdaTransverse ] > 
//              specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::UpperLambdaTransverse ] ) )
//       {
// 
//         throw std::runtime_error( 
//                            "initial parallel diffusivity must be between "
//                            "lower and upper bounds" );
// 
//       }

      // <delta_lambda_transverse>
/*      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::DeltaLambdaTransverse ] >= 
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::UpperLambdaTransverse ] ) -
             specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::LowerLambdaTransverse ] ) )
      {

        throw std::runtime_error( 
                           "delta lambda transverse must be lower than "
                           "upper bound minus lower bound" );

      }   */    
//       // check for scalar vector size ////////////////////////////////////////
//       if ( specificScalarParameters.size() == 46U
//            || specificScalarParameters.size() == 61U
//            || specificScalarParameters.size() == 76U )
//       {
        
        //One Restricted Compartment /////////////////////////////////////////

        // check(s) for restricted_fraction_1 /////////////////////////////////
        // <initial_restricted_fraction_1>
        if ( specificScalarParameters[ 
         gkg::CharmedOdfCartesianField::InitialRestrictedFraction ] < 0.0 )
 	{

 	  throw std::runtime_error(
 			   "restricted fraction ( 1 ) must be positive" );

 	}
      
 	// <upper_restricted_fraction_1>
 	if ( specificScalarParameters[
 	       gkg::CharmedOdfCartesianField::UpperRestrictedFraction ]
 									> 1.0 )
 	{

 	  throw std::runtime_error(
 		 "upper restricted fraction ( 1 ) must be lower or equal to 1.0" );

 	}
 
 	// <lower_restricted_fraction_1> < <upper_restricted_fraction_1>
 	if ( specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::LowerRestrictedFraction ] >
 	     specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::UpperRestrictedFraction ])
 	{

 	  throw std::runtime_error(
 			    "lower restricted fraction ( 1 ) is greater than "
 			    "upper restricted fraction ( 1 )" );

 	}

 	// <initial_restricted_fraction_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction ] <
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction ] ) ||
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction ] >
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction ] ) )
 	{

 	  throw std::runtime_error(
 			     "initial restricted fraction ( 1 ) must be between "
 			     "lower and upper bounds" );

 	}

 	// <delta_restricted_fraction_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::DeltaRestrictedFraction ] >=
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction ] ) -
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction ] ) )
 	{

 	  throw std::runtime_error(
 			     "delta restricted fraction ( 1 ) must be lower than "
 			     "upper bound minus lower bound" );

 	}

 	// check(s) for theta_1 /////////////////////////////////
 	// <initial_theta_1>
 	if ( specificScalarParameters[
 	   gkg::CharmedOdfCartesianField::InitialTheta ] < 0.0 )
 	{

 	  throw std::runtime_error(
 			   "theta ( 1 ) must be positive" );

 	}

 	// <upper_theta_1>
 	if ( specificScalarParameters[
 	       gkg::CharmedOdfCartesianField::UpperTheta ] > 2*M_PI )
 	{

 	  throw std::runtime_error(
 		 "upper theta ( 1 ) must be lower or equal to 2 pi" );

 	}
 
 	// <lower_theta_1> < <upper_theta_1>
 	if ( specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::LowerTheta ] >
 	     specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::UpperTheta ])
 	{

 	  throw std::runtime_error(
 			    "lower theta ( 1 ) is greater than "
 			    "upper theta ( 1 )" );

 	}

 	// <initial_theta_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialTheta ] <
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerTheta ] ) ||
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialTheta ] >
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperTheta ] ) )
 	{

 	  throw std::runtime_error(
 			     "initial theta ( 1 ) must be between "
 			     "lower and upper bounds" );

 	}

 	// <delta_theta_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::DeltaTheta ] >=
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperTheta ] ) -
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerTheta ] ) )
 	{

 	  throw std::runtime_error(
 			     "delta theta ( 1 ) must be lower than "
 			     "upper bound minus lower bound" );

 	}

 	// check(s) for phi_1 /////////////////////////////////
 	// <initial_phi_1>
 	if ( specificScalarParameters[
 	   gkg::CharmedOdfCartesianField::InitialPhi ] < 0.0 )
 	{

 	  throw std::runtime_error(
 			   "phi ( 1 ) must be positive" );

 	}
 
 	// <upper_phi_1>
 	if ( specificScalarParameters[
 	       gkg::CharmedOdfCartesianField::UpperPhi ] > M_PI )
 	{

 	  throw std::runtime_error(
 		 "upper phi ( 1 ) must be lower or equal to pi" );

 	}
 
 	// <lower_phi_1> < <upper_phi_1>
 	if ( specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::LowerPhi ] >
 	     specificScalarParameters[
 	    gkg::CharmedOdfCartesianField::UpperPhi ])
 	{

 	  throw std::runtime_error(
 			    "lower phi ( 1 ) is greater than "
 			    "upper phi ( 1 )" );

 	}

 	// <initial_phi_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialPhi ] <
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerPhi ] ) ||
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::InitialPhi ] >
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperPhi ] ) )
 	{

 	  throw std::runtime_error(
 			     "initial phi ( 1 ) must be between "
 			     "lower and upper bounds" );

 	}

 	// <delta_phi_1>
 	if ( ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::DeltaPhi ] >=
 	     ( specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::UpperPhi ] ) -
 	       specificScalarParameters[
 		 gkg::CharmedOdfCartesianField::LowerPhi ] ) )
 	{

 	  throw std::runtime_error(
 			     "delta phi ( 1 ) must be lower than "
 			     "upper bound minus lower bound" );

 	}
      
       
//       } 
      
//       if( specificScalarParameters.size() == 61U
//           || specificScalarParameters.size() == 76U ) 
//       {
//         
//         //Two Restricted Compartments /////////////////////////////////////////
// 
//  	// check(s) for restricted_fraction_2 /////////////////////////////////
//  	// <initial_restricted_fraction_2>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialRestrictedFraction_2 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "restricted fraction ( 2 ) must be positive" );
// 
//  	}
//  
//  	// <upper_restricted_fraction_2>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperRestrictedFraction_2 ]
//  									> 1.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper restricted fraction ( 2 ) must be lower or equal to 1.0" );
// 
//  	}
//  
//  	// <lower_restricted_fraction_2> < <upper_restricted_fraction_2>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerRestrictedFraction_2 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperRestrictedFraction_2 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower restricted fraction ( 2 ) is greater than "
//  			    "upper restricted fraction ( 2 )" );
// 
//  	}
// 
//  	// <initial_restricted_fraction_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction_2 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction_2 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction_2 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial restricted fraction ( 2 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_restricted_fraction_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaRestrictedFraction_2 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction_2 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta restricted fraction ( 2 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
// 
//  	// check(s) for theta_2 /////////////////////////////////
//  	// <initial_theta_2>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialTheta_2 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "theta ( 2 ) must be positive" );
// 
//  	}
//  
//  	// <upper_theta_2>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperTheta_2 ] > 2*M_PI )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper theta ( 2 ) must be lower or equal to 2 pi" );
// 
//  	}
//  
//  	// <lower_theta_2> < <upper_theta_2>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerTheta_2 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperTheta_2 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower theta ( 2 ) is greater than "
//  			    "upper theta ( 2 )" );
// 
//  	}
// 
//  	// <initial_theta_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialTheta_2 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerTheta_2 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialTheta_2 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperTheta_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial theta ( 2 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_theta_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaTheta_2 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperTheta_2 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerTheta_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta theta ( 2 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
// 
//  	// check(s) for phi_2 /////////////////////////////////
//  	// <initial_phi_2>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialPhi_2 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "phi ( 2 ) must be positive" );
// 
//  	}
//  
//  	// <upper_phi_2>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperPhi_2 ] > M_PI )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper phi ( 2 ) must be lower or equal to pi" );
// 
//  	}
//  
//  	// <lower_phi_2> < <upper_phi_2>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerPhi_2 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperPhi_2 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower phi ( 2 ) is greater than "
//  			    "upper phi ( 2 )" );
// 
//  	}
// 
//  	// <initial_phi_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialPhi_2 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerPhi_2 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialPhi_2 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperPhi_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial phi ( 2 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_phi_2>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaPhi_2 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperPhi_2 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerPhi_2 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta phi ( 2 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
//         
//       }  
//       
//       if( specificScalarParameters.size() == 76U )
//       {
//         
//         //Three Restricted Compartments ///////////////////////////////////////
// 
//  	// check(s) for restricted_fraction_3 /////////////////////////////////
//  	// <initial_restricted_fraction_3>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialRestrictedFraction_3 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "restricted fraction ( 3 ) must be positive" );
// 
//  	}
//  
//  	// <upper_restricted_fraction_3>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperRestrictedFraction_3 ]
//  									> 1.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper restricted fraction ( 3 ) must be lower or equal to 3.0" );
// 
//  	}
//  
//  	// <lower_restricted_fraction_3> < <upper_restricted_fraction_3>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerRestrictedFraction_3 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperRestrictedFraction_3 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower restricted fraction ( 3 ) is greater than "
//  			    "upper restricted fraction ( 3 )" );
// 
//  	}
// 
//  	// <initial_restricted_fraction_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction_3 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction_3 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialRestrictedFraction_3 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial restricted fraction ( 3 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_restricted_fraction_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaRestrictedFraction_3 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperRestrictedFraction_3 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerRestrictedFraction_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta restricted fraction ( 3 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
// 
//  	// check(s) for theta_3 /////////////////////////////////
//  	// <initial_theta_3>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialTheta_3 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "theta ( 3 ) must be positive" );
// 
//  	}
//  
//  	// <upper_theta_3>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperTheta_3 ] > 2*M_PI )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper theta ( 3 ) must be lower or equal to 2 pi" );
// 
//  	}
//  
//  	// <lower_theta_3> < <upper_theta_3>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerTheta_3 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperTheta_3 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower theta ( 3 ) is greater than "
//  			    "upper theta ( 3 )" );
// 
//  	}
// 
//  	// <initial_theta_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialTheta_3 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerTheta_3 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialTheta_3 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperTheta_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial theta ( 3 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_theta_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaTheta_3 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperTheta_3 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerTheta_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta theta ( 3 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
// 
//  	// check(s) for phi_3 /////////////////////////////////
//  	// <initial_phi_3>
//  	if ( specificScalarParameters[
//  	   gkg::CharmedOdfCartesianField::InitialPhi_3 ] < 0.0 )
//  	{
// 
//  	  throw std::runtime_error(
//  			   "phi ( 3 ) must be positive" );
// 
//  	}
//  
//  	// <upper_phi_3>
//  	if ( specificScalarParameters[
//  	       gkg::CharmedOdfCartesianField::UpperPhi_3 ] > M_PI )
//  	{
// 
//  	  throw std::runtime_error(
//  		 "upper phi ( 3 ) must be lower or equal to pi" );
// 
//  	}
//  
//  	// <lower_phi_3> < <upper_phi_3>
//  	if ( specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::LowerPhi_3 ] >
//  	     specificScalarParameters[
//  	    gkg::CharmedOdfCartesianField::UpperPhi_3 ])
//  	{
// 
//  	  throw std::runtime_error(
//  			    "lower phi ( 3 ) is greater than "
//  			    "upper phi ( 3 )" );
// 
//  	}
// 
//  	// <initial_phi_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialPhi_3 ] <
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerPhi_3 ] ) ||
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::InitialPhi_3 ] >
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperPhi_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "initial phi ( 3 ) must be between "
//  			     "lower and upper bounds" );
// 
//  	}
// 
//  	// <delta_phi_3>
//  	if ( ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::DeltaPhi_3 ] >=
//  	     ( specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::UpperPhi_3 ] ) -
//  	       specificScalarParameters[
//  		 gkg::CharmedOdfCartesianField::LowerPhi_3 ] ) )
//  	{
// 
//  	  throw std::runtime_error(
//  			     "delta phi ( 3 ) must be lower than "
//  			     "upper bound minus lower bound" );
// 
//  	}
//         
//       }  

      if ( specificScalarParameters.size() != 52 )
      {

        throw std::runtime_error( "invalid scalar parameter count" );

      }      

      // check(s) for fixed //////////////////////////////////////////////////
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRadius ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRadius ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "radius fraction must be either 0 or 1");
// 
//       }
//       
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedIsotropicFraction ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedIsotropicFraction ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                             "fixed isotropic fraction must be either 0 or 1");
// 
//       }
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedIsotropicDiffusivity ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedIsotropicDiffusivity ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed isotropic diffusivity must be either 0 or 1");
// 
//       }
/*      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedLambdaParallel ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedLambdaParallel ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                          "fixed lambda parallel must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedLambdaTransverse ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedLambdaTransverse ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                          "fixed lambda transverse must be either 0 or 1");

      } */     
/*      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedParallelDiffusivity ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedParallelDiffusivity ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                          "fixed parallel diffusivity must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedPerpendicularDiffusivity ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedPerpendicularDiffusivity ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                       "fixed perpendicular diffusivity must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedRestrictedFraction ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedRestrictedFraction ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                       "fixed restricted fraction ( 1 ) must be either 0 or 1");

      } 
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedTheta ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedTheta ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                       "fixed theta ( 1 ) must be either 0 or 1");

      } 
      if ( ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedPhi ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::CharmedOdfCartesianField::FixedPhi ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                       "fixed phi ( 1 ) must be either 0 or 1");

      } */      
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRestrictedFraction_2 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRestrictedFraction_2 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed restricted fraction ( 2 ) must be either 0 or 1");
// 
//       } 
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedTheta_2 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedTheta_2 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed theta ( 2 ) must be either 0 or 1");
// 
//       } 
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedPhi_2 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedPhi_2 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed phi ( 2 ) must be either 0 or 1");
// 
//       }  
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRestrictedFraction_3 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedRestrictedFraction_3 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed restricted fraction ( 3 ) must be either 0 or 1");
// 
//       } 
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedTheta_3 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedTheta_3 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed theta ( 3 ) must be either 0 or 1");
// 
//       } 
//       if ( ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedPhi_3 ] 
//              != 0.0 ) &&
//            ( specificScalarParameters[ 
//                gkg::CharmedOdfCartesianField::FixedPhi_3 ] 
//              != 1.0 ) )
//       {
// 
//         throw std::runtime_error(
//                        "fixed phi ( 3 ) must be either 0 or 1");
// 
//       }        
      // check for noise standard deviation //////////////////////////////////
      if ( specificScalarParameters[
             gkg::CharmedOdfCartesianField::NoiseStandardDeviation ] <= 0.0 )
      {

        throw std::runtime_error(
                       "noise standard deviation must be strictly positive" );

      }

      // check(s) for QuickSeach parameter ///////////////////////////////////
      if ( specificScalarParameters[
              gkg::CharmedOdfCartesianField::QuickSearchStepCount ] <= 0.0 )
      {

        throw std::runtime_error(
                         "QuickSearch step count must be strictly positive" );

      }

      // check(s) for NLP parameter //////////////////////////////////////////
      if ( specificScalarParameters[
            gkg::CharmedOdfCartesianField::NlpMaximumIterationCount ] <= 0.0 )
      {

        throw std::runtime_error(
                    "NLP maximum iteration count must be strictly positive" );

      }
      if ( specificScalarParameters[
          gkg::CharmedOdfCartesianField::NlpMaximumSizeOfTheSimplex ] <= 0.0 )
      {

        throw std::runtime_error(
                "NLP maximum size of the simplex must be strictly positive" );

      }

      // check(s) for MCMCM parameter ////////////////////////////////////////
      if ( specificScalarParameters[
              gkg::CharmedOdfCartesianField::McmcBurninCount ] <= 0.0 )
      {

        throw std::runtime_error(
                              "MCMC burnin count must be strictly positive" );

      }

      if ( specificScalarParameters[
              gkg::CharmedOdfCartesianField::McmcSampleCount ] <= 0.0 )
      {

        throw std::runtime_error(
                              "MCMC sample count must be strictly positive" );

      }

      if ( specificScalarParameters[
              gkg::CharmedOdfCartesianField::McmcIntervalCount ] <= 0.0 )
      {

        throw std::runtime_error(
                            "MCMC interval count must be strictly positive" );

      }

      if ( specificScalarParameters[
           gkg::CharmedOdfCartesianField::McmcMaximumIterationCount ] <= 0.0 )
      {

        throw std::runtime_error(
                   "MCMC maximum iteration count must be strictly positive" );

      }

    }

  }
  GKG_CATCH( "void gkg::CharmedOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::CharmedOdfCartesianField::initialize( 
                    const std::vector< double >& specificScalarParameters,
                    const std::vector< std::string >& specificStringParameters )
{

  try
  {

    _restrictedCompartmentsCount = specificScalarParameters[ 
                                                 RestrictedCompartmentsCount ];
                                                
    // collecting parameter(s) to compute Charmed model
    
    if ( _restrictedCompartmentsCount == 2 )
    {

      _initialParameters.reallocate( 11 );  
      _lowerParameters.reallocate( 11 ); 
      _upperParameters.reallocate( 11 ); 
      _deltaParameters.reallocate( 11 ); 
      _fixed.resize( 11U );       
      
    }
    else if ( _restrictedCompartmentsCount == 3 )
    {

      _initialParameters.reallocate( 14 );  
      _lowerParameters.reallocate( 14 ); 
      _upperParameters.reallocate( 14 ); 
      _deltaParameters.reallocate( 14 ); 
      _fixed.resize( 14U );        
      
    }
    else
    {
      
      _initialParameters.reallocate( 8 );  
      _lowerParameters.reallocate( 8 ); 
      _upperParameters.reallocate( 8 ); 
      _deltaParameters.reallocate( 8 ); 
      _fixed.resize( 8U );    
      
    }  
    
    if ( _restrictedCompartmentsCount > 0 )
    {  
      // collecting initial parameter(s)
      _initialParameters( 0 ) = specificScalarParameters[
                                           InitialRadius ];
      _initialParameters( 1 ) = specificScalarParameters[
                                           InitialIsotropicFraction ];
      _initialParameters( 2 ) = specificScalarParameters[
                                           InitialIsotropicDiffusivity ];
/*      _initialParameters( 1 ) = 0.0; // will be set to tensor cartesian field
      _initialParameters( 2 ) = 0.0;        */                                  
      _initialParameters( 3 ) = specificScalarParameters[
                                           InitialParallelDiffusivity ];
      _initialParameters( 4 ) = specificScalarParameters[
                                           InitialPerpendicularDiffusivity ];
      _initialParameters( 5 ) = specificScalarParameters[
                                           InitialRestrictedFraction ];
      _initialParameters( 6 ) = specificScalarParameters[
                                           InitialTheta ];
      _initialParameters( 7 ) = specificScalarParameters[
                                           InitialPhi ];                                           

      // collecting lower parameter(s)
      _lowerParameters( 0 ) = specificScalarParameters[
                                           LowerRadius ];
      _lowerParameters( 1 ) = specificScalarParameters[
                                           LowerIsotropicFraction ];
      _lowerParameters( 2 ) = specificScalarParameters[
                                           LowerIsotropicDiffusivity ];
/*      _lowerParameters( 1 ) = specificScalarParameters[
                                           LowerLambdaParallel ];
      _lowerParameters( 2 ) = specificScalarParameters[
                                           LowerLambdaTransverse ]; */                                          
      _lowerParameters( 3 ) = specificScalarParameters[
                                           LowerParallelDiffusivity ];
      _lowerParameters( 4 ) = specificScalarParameters[
                                           LowerPerpendicularDiffusivity ];
      _lowerParameters( 5 ) = specificScalarParameters[
                                           LowerRestrictedFraction ];
      _lowerParameters( 6 ) = specificScalarParameters[
                                           LowerTheta ];
      _lowerParameters( 7 ) = specificScalarParameters[
                                           LowerPhi ]; 
                                           
      // collecting upper parameter(s)
      _upperParameters( 0 ) = specificScalarParameters[
                                           UpperRadius ];
      _upperParameters( 1 ) = specificScalarParameters[
                                           UpperIsotropicFraction ];
      _upperParameters( 2 ) = specificScalarParameters[
                                           UpperIsotropicDiffusivity ];
//       _upperParameters( 1 ) = specificScalarParameters[
//                                            UpperLambdaParallel ];
//       _upperParameters( 2 ) = specificScalarParameters[
//                                            UpperLambdaTransverse ];
      _upperParameters( 3 ) = specificScalarParameters[
                                           UpperParallelDiffusivity ];
      _upperParameters( 4 ) = specificScalarParameters[
                                           UpperPerpendicularDiffusivity ];
      _upperParameters( 5 ) = specificScalarParameters[
                                           UpperRestrictedFraction ];
      _upperParameters( 6 ) = specificScalarParameters[
                                           UpperTheta ];
      _upperParameters( 7 ) = specificScalarParameters[
                                           UpperPhi ];     
                                           
      // collecting delta parameter(s)
      _deltaParameters( 0 ) = specificScalarParameters[
                                           DeltaRadius ];
      _deltaParameters( 1 ) = specificScalarParameters[
                                           DeltaIsotropicFraction ];
      _deltaParameters( 2 ) = specificScalarParameters[
                                           DeltaIsotropicDiffusivity ];
//       _deltaParameters( 1 ) = specificScalarParameters[
//                                            DeltaLambdaParallel ];
//       _deltaParameters( 2 ) = specificScalarParameters[
//                                            DeltaLambdaTransverse ];
      _deltaParameters( 3 ) = specificScalarParameters[
                                           DeltaParallelDiffusivity ];
      _deltaParameters( 4 ) = specificScalarParameters[
                                           DeltaPerpendicularDiffusivity ];
      _deltaParameters( 5 ) = specificScalarParameters[
                                           DeltaRestrictedFraction ];
      _deltaParameters( 6 ) = specificScalarParameters[
                                           DeltaTheta ];
      _deltaParameters( 7 ) = specificScalarParameters[
                                           DeltaPhi ];                                                

      // collecting fixed parameter(s)
      _fixed[ 0 ] = ( specificScalarParameters[
                      FixedRadius ] > 0.0 ? true : false );
      _fixed[ 1 ] = ( specificScalarParameters[
                      FixedIsotropicFraction ] > 0.0 ? true : false );
      _fixed[ 2 ] = ( specificScalarParameters[
                      FixedIsotropicDiffusivity ] > 0.0 ? true : false );
/*      _fixed[ 1 ] = ( specificScalarParameters[
                      FixedLambdaParallel ] > 0.0 ? true : false );
      _fixed[ 2 ] = ( specificScalarParameters[
                      FixedLambdaTransverse ] > 0.0 ? true : false );  */    
      _fixed[ 3 ] = ( specificScalarParameters[
                      FixedParallelDiffusivity ] > 0.0 ? true : false );
      _fixed[ 4 ] = ( specificScalarParameters[
                      FixedPerpendicularDiffusivity ] > 0.0 ? true : false );
      _fixed[ 5 ] = ( specificScalarParameters[
                      FixedRestrictedFraction ] > 0.0 ? true : false );
      _fixed[ 6 ] = ( specificScalarParameters[
                      FixedTheta ] > 0.0 ? true : false );
      _fixed[ 7 ] = ( specificScalarParameters[
                      FixedPhi ] > 0.0 ? true : false );
    }
    if ( _restrictedCompartmentsCount > 1 )
    {  
      // collecting initial parameter(s)
      _initialParameters( 8 ) = specificScalarParameters[
                                           InitialRestrictedFraction ];
      _initialParameters( 9 ) = specificScalarParameters[
                                           InitialTheta ];
      _initialParameters( 10 ) = specificScalarParameters[
                                           InitialPhi ];                                           

      // collecting lower parameter(s)
      _lowerParameters( 8 ) = specificScalarParameters[
                                           LowerRestrictedFraction ];
      _lowerParameters( 9 ) = specificScalarParameters[
                                           LowerTheta ];
      _lowerParameters( 10 ) = specificScalarParameters[
                                           LowerPhi ];

                                           
      // collecting upper parameter(s)
      _upperParameters( 8 ) = specificScalarParameters[
                                           UpperRestrictedFraction ];
      _upperParameters( 9 ) = specificScalarParameters[
                                           UpperTheta ];
      _upperParameters( 10 ) = specificScalarParameters[
                                           UpperPhi ];     
                                           
      // collecting delta parameter(s)
      _deltaParameters( 8 ) = specificScalarParameters[
                                           DeltaRestrictedFraction ];
      _deltaParameters( 9 ) = specificScalarParameters[
                                           DeltaTheta ];
      _deltaParameters( 10 ) = specificScalarParameters[
                                           DeltaPhi ];                                                

      // collecting fixed parameter(s)
      _fixed[ 8 ] = ( specificScalarParameters[
                      FixedRestrictedFraction ] > 0.0 ? true : false );
      _fixed[ 9 ] = ( specificScalarParameters[
                      FixedTheta ] > 0.0 ? true : false );
      _fixed[ 10 ] = ( specificScalarParameters[
                      FixedPhi ] > 0.0 ? true : false );
      
    } 
    
    if ( _restrictedCompartmentsCount > 2 )
    {  
      // collecting initial parameter(s)
      _initialParameters( 11 ) = specificScalarParameters[
                                           InitialRestrictedFraction ];
      _initialParameters( 12 ) = specificScalarParameters[
                                           InitialTheta ];
      _initialParameters( 13 ) = specificScalarParameters[
                                           InitialPhi ];                                           

      // collecting lower parameter(s)
      _lowerParameters( 11 ) = specificScalarParameters[
                                           LowerRestrictedFraction ];
      _lowerParameters( 12 ) = specificScalarParameters[
                                           LowerTheta ];
      _lowerParameters( 13 ) = specificScalarParameters[
                                           LowerPhi ];

                                           
      // collecting upper parameter(s)
      _upperParameters( 11 ) = specificScalarParameters[
                                           UpperRestrictedFraction ];
      _upperParameters( 12 ) = specificScalarParameters[
                                           UpperTheta ];
      _upperParameters( 13 ) = specificScalarParameters[
                                           UpperPhi ];     
                                           
      // collecting delta parameter(s)
      _deltaParameters( 11 ) = specificScalarParameters[
                                           DeltaRestrictedFraction ];
      _deltaParameters( 12 ) = specificScalarParameters[
                                           DeltaTheta ];
      _deltaParameters( 13 ) = specificScalarParameters[
                                           DeltaPhi ];                                                

      // collecting fixed parameter(s)
      _fixed[ 11 ] = ( specificScalarParameters[
                      FixedRestrictedFraction ] > 0.0 ? true : false );
      _fixed[ 12 ] = ( specificScalarParameters[
                      FixedTheta ] > 0.0 ? true : false );
      _fixed[ 13 ] = ( specificScalarParameters[
                      FixedPhi ] > 0.0 ? true : false );
      
    }    
    // collecting noise standard deviation
    _noiseStandardDeviation = specificScalarParameters[
                                                     NoiseStandardDeviation ];
                                         
    // collecting QuickSearch parameters
    _applyQuickSearchOptimizer = ( specificScalarParameters[
                           ApplyQuickSearchOptimizer ] > 0.0 ? true : false );
    _quicksearchStepCount = ( int32_t )( specificScalarParameters[
                                               QuickSearchStepCount ] + 0.5 );

    // collecting NLP parameters
    _applyNlpOptimizer = ( specificScalarParameters[
                                   ApplyNlpOptimizer ] > 0.0 ? true : false );
    _nlpMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                           NlpMaximumIterationCount ] + 0.5 );
    _nlpMaximumSizeOfTheSimplex = specificScalarParameters[
                                                 NlpMaximumSizeOfTheSimplex ];

    // collecting MCMC parameters
    _applyMcmcOptimizer = ( specificScalarParameters[
                                  ApplyMcmcOptimizer ] > 0.0 ? true : false );
    _mcmcBurninCount = ( int32_t )( specificScalarParameters[
                                          McmcBurninCount ] + 0.5 );
    _mcmcSampleCount = ( int32_t )( specificScalarParameters[
                                          McmcSampleCount ] + 0.5 );
    _mcmcIntervalCount = ( int32_t )( specificScalarParameters[
                                          McmcIntervalCount ] + 0.5 );
    _mcmcMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                          McmcMaximumIterationCount ] + 0.5 );


  }
  GKG_CATCH( "void gkg::CharmedOdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters )" );

}


void 
gkg::CharmedOdfCartesianField::prepareMCMCFunction( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "preparing Monte-Carlo-Markov-Chain function" << std::flush;

    }

    _charmedMCMCFunction.reset( new gkg::CharmedMCMCFunction(
                                    this->_dw,
                                    this->_outputOrientationSet,
                                    this->_noiseStandardDeviation,
                                    this->_restrictedCompartmentsCount ) );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 43 ) << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::CharmedOdfCartesianField::prepareMCMCFunction( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::CharmedOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {
    
    gkg::Vector initialParameters( _initialParameters );

    gkg::RCPointer< gkg::CharmedMCMCParameterSet > charmedMCMCParameterSet; 

    charmedMCMCParameterSet.reset( new gkg::CharmedMCMCParameterSet(
                                    this->_lowerParameters,
                                    this->_upperParameters,
                                    this->_fixed,
                                    this->_deltaParameters ) );

    // setting the principal orientation to the first eigenvector of the 
    // diffusion tensor model
//     const Vector3d< float >& principalOrientation =
//                   _tensorCartesianField->getItem( lutSite )->getEigenVector(
//                                                          gkg::Tensor::Maximum );
//     initialParameters( 3 ) = principalOrientation.x;
//     initialParameters( 4 ) = principalOrientation.y;
//     initialParameters( 5 ) = principalOrientation.z;
    
    _charmedMCMCFunction->setTensor( _tensorCartesianField->getItem( lutSite )->
                                     getCoefficients() );
    _charmedMCMCFunction->setEigenVector( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getEigenVector( gkg::Tensor::Maximum ) );
    _charmedMCMCFunction->setLambdaParallel( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getLambdaParallel( ) ); 
    _charmedMCMCFunction->setLambdaTransverse( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getLambdaTransverse( ) ); 

    gkg::CharmedOdf charmedOdf( &_outputOrientationSet,
                            *_t2, *_dw,
                            originSite,
                            _charmedMCMCFunction,
                            charmedMCMCParameterSet,
                            initialParameters,
                            _applyQuickSearchOptimizer,
                            _quicksearchStepCount,
                            _applyNlpOptimizer,
                            _nlpMaximumIterationCount,
                            _nlpMaximumSizeOfTheSimplex,
                            _applyMcmcOptimizer,
                            _mcmcBurninCount,
                            _mcmcSampleCount,
                            _mcmcIntervalCount,
                            _mcmcMaximumIterationCount );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              charmedOdf );

    }
    else
    {

      return charmedOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::CharmedOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::CharmedOdfCartesianField::newItem( const gkg::Vector3d< float >& originSite,
                                      const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::Vector initialParameters( _initialParameters );
    gkg::RCPointer< gkg::CharmedMCMCParameterSet > charmedMCMCParameterSet; 

    charmedMCMCParameterSet.reset( new gkg::CharmedMCMCParameterSet(
                                    this->_lowerParameters,
                                    this->_upperParameters,
                                    this->_fixed,
                                    this->_deltaParameters ) );

    // setting the principal orientation to the first eigenvector of the 
    // diffusion tensor model
//     const Vector3d< float >& principalOrientation =
//                   _tensorCartesianField->getItem( lutSite )->getEigenVector(
//                                                          gkg::Tensor::Maximum );
//     initialParameters( 3 ) = principalOrientation.x;
//     initialParameters( 4 ) = principalOrientation.y;
//     initialParameters( 5 ) = principalOrientation.z;

    _charmedMCMCFunction->setTensor( _tensorCartesianField->getItem( lutSite )->
                                    getCoefficients() );  
    _charmedMCMCFunction->setEigenVector( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getEigenVector( gkg::Tensor::Maximum ) ); 
    _charmedMCMCFunction->setLambdaParallel( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getLambdaParallel( ) ); 
    _charmedMCMCFunction->setLambdaTransverse( 
                                     _tensorCartesianField->getItem( lutSite )->
                                     getLambdaTransverse( ) ); 
    
    gkg::CharmedOdf charmedOdf( &_outputOrientationSet,
                            *_t2, *_dw,
                            originSite,
                            _charmedMCMCFunction,
                            charmedMCMCParameterSet,
                            initialParameters,
                            _applyQuickSearchOptimizer,
                            _quicksearchStepCount,
                            _applyNlpOptimizer,
                            _nlpMaximumIterationCount,
                            _nlpMaximumSizeOfTheSimplex,
                            _applyMcmcOptimizer,
                            _mcmcBurninCount,
                            _mcmcSampleCount,
                            _mcmcIntervalCount,
                            _mcmcMaximumIterationCount );

    if ( _functor )
    {

      return _functor->apply( lutSite,
                              charmedOdf );

    }
    else
    {

      return charmedOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::CharmedOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
