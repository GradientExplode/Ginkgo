#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianField.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdf.h>
#include <gkg-dmri-microstructure-noddi/NoddiOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCBinghamFunction.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCWatsonParameterSet.h>
#include <gkg-dmri-microstructure-noddi/NoddiMCMCBinghamParameterSet.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::NoddiOdfCartesianField::NoddiOdfCartesianField(
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

    // getting the sphere distribution function
    if ( specificStringParameters[ 0 ] == "watson" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Watson;

    }
    else if ( specificStringParameters[ 0 ] == "bingham" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Bingham;

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
                                     gkg::TypeOf< gkg::NoddiOdf >::getName() );

    this->_functor = new gkg::NoddiOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::NoddiOdfCartesianField::"
             "NoddiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::NoddiOdfCartesianField::NoddiOdfCartesianField(
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

    // getting the sphere distribution function
    if ( specificStringParameters[ 0 ] == "watson" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Watson;

    }
    else if ( specificStringParameters[ 0 ] == "bingham" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Bingham;

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
                            gkg::TypeOf< gkg::NoddiOdf >::getName() );

    this->_functor = new gkg::NoddiOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::NoddiOdfCartesianField::"
             "NoddiOdfCartesianField( "
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


gkg::NoddiOdfCartesianField::NoddiOdfCartesianField(
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

    // getting the sphere distribution function
    if ( specificStringParameters[ 0 ] == "watson" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Watson;

    }
    else if ( specificStringParameters[ 0 ] == "bingham" )
    {

      _sphereDistributionFunction = gkg::NoddiOdfCartesianField::Bingham;

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
                            gkg::TypeOf< gkg::NoddiOdf >::getName() );

    this->_functor = new gkg::NoddiOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction(  verbose );

    // computing ODFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::NoddiOdfCartesianField::"
             "NoddiOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::NoddiOdfCartesianField::~NoddiOdfCartesianField()
{
}



gkg::NoddiOdfCartesianField::SphereDistributionFunction 
gkg::NoddiOdfCartesianField::getSphereDistributionFunction() const
{

  try
  {

    return _sphereDistributionFunction;

  }
  GKG_CATCH( "gkg::NoddiOdfCartesianField::SphereDistributionFunction "
             "gkg::NoddiOdfCartesianField::getSphereDistributionFunction() "
             "const" );

}


const gkg::Vector& gkg::NoddiOdfCartesianField::getInitialParameters() const
{

  try
  {

    return _initialParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::NoddiOdfCartesianField::getInitialParameters() const" );

}


const gkg::Vector& gkg::NoddiOdfCartesianField::getLowerParameters() const
{

  try
  {

    return _lowerParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::NoddiOdfCartesianField::getLowerParameters() const" );

}


const gkg::Vector& gkg::NoddiOdfCartesianField::getUpperParameters() const
{

  try
  {

    return _upperParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::NoddiOdfCartesianField::getUpperParameters() const" );

}


const gkg::Vector& gkg::NoddiOdfCartesianField::getDeltaParameters() const
{

  try
  {

    return _deltaParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::NoddiOdfCartesianField::getDeltaParameters() const" );

}


const std::vector< bool >& gkg::NoddiOdfCartesianField::getFixed() const
{

  try
  {

    return _fixed;

  }
  GKG_CATCH( "const std::vector< bool >& "
             "gkg::NoddiOdfCartesianField::getFixed() const" );

}


double gkg::NoddiOdfCartesianField::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( "double "
             "gkg::NoddiOdfCartesianField::getNoiseStandardDeviation() "
             "const" );

}


int32_t gkg::NoddiOdfCartesianField::getQuickSearchStepCount() const
{

  try
  {

    return _quicksearchStepCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getQuickSearchStepCount() "
             "const" );

}


int32_t gkg::NoddiOdfCartesianField::getNLPMaximumIterationCount() const
{

  try
  {

    return _nlpMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getNLPMaximumIterationCount() "
             "const" );

}


double gkg::NoddiOdfCartesianField::getNLPMaximumSizeOfTheSimplex() const
{

  try
  {

    return _nlpMaximumSizeOfTheSimplex;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getNLPMaximumSizeOfTheSimplex() "
             "const" );

}


int32_t gkg::NoddiOdfCartesianField::getMCMCBurninCount() const
{

  try
  {

    return _mcmcBurninCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getMCMCBurninCount() const" );

}


int32_t gkg::NoddiOdfCartesianField::getMCMCSampleCount() const
{

  try
  {

    return _mcmcSampleCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getMCMCSampleCount() const" );

}


int32_t gkg::NoddiOdfCartesianField::getMCMCIntervalCount() const
{

  try
  {

    return _mcmcIntervalCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getMCMCIntervalCount() const" );

}


int32_t gkg::NoddiOdfCartesianField::getMCMCMaximumIterationCount() const
{

  try
  {

    return _mcmcMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::NoddiOdfCartesianField::getMCMCMaximumIterationCount() const" );

}


void gkg::NoddiOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "watson";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "only one string parameter required" );

      }

      if ( ( specificStringParameters[ 0 ] != "watson" ) &&
           ( specificStringParameters[ 0 ] != "bingham" ) )
      {

        throw std::runtime_error( 
         "spherical distribution type must be one of watson/bingham" );

      }

    }

    if ( specificScalarParameters.empty() )
    {

      if ( specificStringParameters[ 0 ] == "watson" )
      {

        specificScalarParameters.resize( 42 );

        // <initial_isotropic_fraction>
        specificScalarParameters[
          gkg::NoddiOdfCartesianField::W_InitialIsotropicFraction ] = 0.0;
        // <initial_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_InitialIntracellularFraction ] = 0.5;
        // <initial_kappa>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_InitialKappa ] = 0.0;
        // <initial_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_InitialParallelDiffusivity ] = 1.7e-9;
        // <initial_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_InitialIsotropicDiffusivity ] = 3e-9;
        // <initial_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_InitialStationaryFraction ] = 0.0;

        // <lower_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerIsotropicFraction ] = 0.0;
        // <lower_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerIntracellularFraction ] = 0.0;
        // <lower_kappa>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerKappa ] = 0.0;
        // <lower_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerParallelDiffusivity ] = 0.0;
        // <lower_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerIsotropicDiffusivity ] = 0.0;
        // <lower_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_LowerStationaryFraction ] = 0.0;

        // <upper_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperIsotropicFraction ] = 1.0;
        // <upper_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperIntracellularFraction ] = 1.0;
        // <upper_kappa>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperKappa ] = 100.0;
        // <upper_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperParallelDiffusivity ] = 3.0e-9;
        // <upper_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperIsotropicDiffusivity ] = 6.0e-9;
        // <upper_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_UpperStationaryFraction ] = 1.0;

        // <delta_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaIsotropicFraction ] = 0.05;
        // <delta_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaIntracellularFraction ] = 0.05;
        // <delta_kappa>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaKappa ] = 5.0;
         // <delta_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaParallelDiffusivity ] = 0.25e-9;
        // <delta_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaIsotropicDiffusivity ] = 0.50e-9;
        // <delta_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_DeltaStationaryFraction ] = 0.05;

        // <fixed_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedIsotropicFraction ] = false;
        // <fixed_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedIntracellularFraction ] = false;
        // <fixed_kappa>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedKappa ] = false;
        // <fixed_principal_orientation>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedPrincipalOrientation ] = false;
        // <fixed_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedParallelDiffusivity ] = true;
        // <fixed_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedIsotropicDiffusivity ] = true;
        // <fixed_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_FixedStationaryFraction ] = true;

        // <noise_standard_deviation>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_NoiseStandardDeviation ] = 50;

        // <apply_quicksearch_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_ApplyQuickSearchOptimizer ] = 1;
        // <quicksearch_step_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_QuickSearchStepCount ] = 10;

        // <apply_nlp_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_ApplyNlpOptimizer ] = 1;
        // <nlp_maximum_iteration_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_NlpMaximumIterationCount ] = 1000;
        // <nlp_maximum_size_of_the_simplex>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_NlpMaximumSizeOfTheSimplex ] = 0.001;

        // <apply_mcmc_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_ApplyMcmcOptimizer ] = 1;
        // <mcmc_burnin>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_McmcBurninCount ] = 10000;
        // <mcmc_sample_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_McmcSampleCount ] = 100;
        // <mcmc_interval_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_McmcIntervalCount ] = 1000;
        // <mcmc_maximum_iteration_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::W_McmcMaximumIterationCount ] = 50000;


      }
      else if ( specificStringParameters[ 0 ] == "bingham" )
      {

        specificScalarParameters.resize( 48 );

        // <initial_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialIsotropicFraction ] = 0.0;
        // <initial_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialIntracellularFraction ] = 0.5;
        // <initial_kappa1>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialKappa1 ] = 0;
        // <initial_kappa2>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialKappa2 ] = 0;
        // <initial_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialParallelDiffusivity ] = 1.7e-9;
        // <initial_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialIsotropicDiffusivity ] =
                                                                        3.0e-9;
        // <initial_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_InitialStationaryFraction ] = 0.0;

        // <lower_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerIsotropicFraction ] = 0.0;
        // <lower_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerIntracellularFraction ] = 0.0;
        // <lower_kappa1>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerKappa1 ] = 0.0;
        // <lower_kappa2>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerKappa2 ] = 0.0;
        // <lower_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerParallelDiffusivity ] = 0.0;
        // <lower_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerIsotropicDiffusivity ] = 0.0;
        // <lower_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_LowerStationaryFraction ] = 0.0;

        // <upper_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_UpperIsotropicFraction ] = 1.0;
        // <upper_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_UpperIntracellularFraction ] = 1.0;
        // <upper_kappa1>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_UpperKappa1 ] = 100.0;
        // <upper_kappa2>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_UpperKappa2 ] = 100.0;
        // <upper_parallel_diffusivity>
        specificScalarParameters[
          gkg::NoddiOdfCartesianField::B_UpperParallelDiffusivity ] = 3.0e-9;
        // <upper_isotropic_diffusivity>
        specificScalarParameters[
          gkg::NoddiOdfCartesianField::B_UpperIsotropicDiffusivity ] = 6.0e-9;
        // <upper_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_UpperStationaryFraction ] = 1.0;

        // <delta_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_DeltaIsotropicFraction ] = 0.05;
        // <delta_intracellular_fraction>
        specificScalarParameters[
          gkg::NoddiOdfCartesianField::B_DeltaIntracellularFraction ] = 0.05;
        // <delta_kappa1>
        specificScalarParameters[
          gkg::NoddiOdfCartesianField::B_DeltaKappa1 ] = 5.0;
        // <delta_kappa2>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_DeltaKappa2 ] = 5.0;
        // <delta_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_DeltaParallelDiffusivity ] = 0.25e-9;
        // <delta_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_DeltaIsotropicDiffusivity ] = 0.50e-9;
        // <delta_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_DeltaStationaryFraction ] = 0.05;

        // <fixed_isotropic_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedIsotropicFraction ] = false;
        // <fixed_intracellular_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedIntracellularFraction ] = false;
        // <fixed_kappa1>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedKappa1 ] = false;
        // <fixed_kappa2>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedKappa2 ] = false;
        // <fixed_principal_orientation>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedPrincipalOrientation ] = false;
        // <fixed_secondary_orientation>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedSecondaryOrientation ] = false;
        // <fixed_parallel_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedParallelDiffusivity ] = true;
        // <fixed_isotropic_diffusivity>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedIsotropicDiffusivity ] = true;
        // <fixed_stationary_fraction>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_FixedStationaryFraction ] = true;

        // <noise_standard_deviation>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_NoiseStandardDeviation ] = 50;

        // <apply_quicksearch_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_ApplyQuickSearchOptimizer ] = 1;
        // <quicksearch_step_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_QuickSearchStepCount ] = 10;

        // <apply_nlp_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_ApplyNlpOptimizer ] = 1;
        // <nlp_maximum_iteration_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_NlpMaximumIterationCount ] = 1000;
        // <nlp_maximum_size_of_the_simplex>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_NlpMaximumSizeOfTheSimplex ] = 0.001;

        // <apply_mcmc_optimizer>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_ApplyMcmcOptimizer ] = 1;
        // <mcmc_burnin>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_McmcBurninCount ] = 10000;
        // <mcmc_sample_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_McmcSampleCount ] = 100;
        // <mcmc_interval_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_McmcIntervalCount ] = 1000;
        // <mcmc_maximum_iteration_count>
        specificScalarParameters[ 
          gkg::NoddiOdfCartesianField::B_McmcMaximumIterationCount ] = 50000;


      }

    }
    else
    {

      if ( specificStringParameters[ 0 ] == "watson" )
      {

        // check for scalar vector size ////////////////////////////////////////
        if ( specificScalarParameters.size() != 42U )
        {

          throw std::runtime_error( "invalid scalar parameter count" );

        }

        // check(s) for isotropic fraction /////////////////////////////////////
        // <lower_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerIsotropicFraction ] < 0.0 )
        {

          throw std::runtime_error(
                                 "lower isotropic fraction must be positive" );

        }
        // <upper_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_UpperIsotropicFraction ] > 1.0 )
        {

          throw std::runtime_error(
                    "upper isotropic fraction must be lower or equal to 1.0" );

        }
        // <lower_isotropic_fraction> < <upper_isotropic_fraction>
        if ( specificScalarParameters[ W_LowerIsotropicFraction ] >
             specificScalarParameters[ W_UpperIsotropicFraction ] )
        {

          throw std::runtime_error( "lower isotropic fraction is greater than "
                                    "upper isotropic fraction" );

        }
        // <initial_isotropic_fraction>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialIsotropicFraction ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerIsotropicFraction ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialIsotropicFraction ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperIsotropicFraction ] ) )
        {

          throw std::runtime_error( 
          "initial isotropic fraction must be between lower and upper bounds" );

        }
        // <delta_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_DeltaIsotropicFraction ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperIsotropicFraction ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerIsotropicFraction ] ) )
        {

          throw std::runtime_error( 
            "delta isotropic fraction must be lower than upper bound minus "
            "lower bound" );
        }


        // check(s) for intracellular fraction /////////////////////////////////
        // <lower_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerIntracellularFraction ]
                                                                        < 0.0 )
        {

          throw std::runtime_error(
                              "lower intracellular fraction must be positive" );

        }
        // <upper_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_UpperIntracellularFraction ] 
                                                                        > 1.0 )
        {

          throw std::runtime_error(
                 "upper intracellular fraction must be lower or equal to 1.0" );

        }
        // <lower_intracellular_fraction> < <upper_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerIntracellularFraction ] > 
             specificScalarParameters[
               gkg::NoddiOdfCartesianField::W_UpperIntracellularFraction ] )
        {

          throw std::runtime_error( "lower intracellular fraction is greater "
                                    "than upper intracellular fraction" );

        }
        // <initial_intracellular_fraction>
        if ( ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_InitialIntracellularFraction ] < 
               specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerIntracellularFraction ] ) ||
             ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_InitialIntracellularFraction ] > 
               specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_UpperIntracellularFraction ] ) )
        {

          throw std::runtime_error( 
               "initial intracellular fraction must be between lower and upper "
               "bounds" );

        }
        // <delta_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_DeltaIntracellularFraction ] >= 
             ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_UpperIntracellularFraction ] -
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_LowerIntracellularFraction ] ) )
        {

          throw std::runtime_error( 
            "delta intracellular fraction must be lower than upper bound minus "
            "lower bound" );
          
        }
        // check for combined isotropic & intracellular ////////////////////////
        if ( ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_InitialIsotropicFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_InitialIntracellularFraction ] )
                                                                        > 1.0 )
        {

          throw std::runtime_error( 
                            "the sum of isotropic & intracellular fractions is "
                            "greater than 1.0" );

        }
        
        // check(s) for kappa //////////////////////////////////////////////////
        // <lower_kappa>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerKappa ] < 0.0 )
        {

          throw std::runtime_error( "lower kappa must be positive" );

        }
        // <lower_kappa> < <upper_kappa>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerKappa ] > 
             specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_UpperKappa ] )
        {

          throw std::runtime_error( "lower kappa is greater "
                                    "than upper kappa" );

        }
        // <initial_kappa>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialKappa ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerKappa ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialKappa ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperKappa ] ) )
        {

          throw std::runtime_error( 
                      "initial kappa must be between lower and upper bounds" );

        }
        // <delta_kappa>
        if ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_DeltaKappa ] >= 
             ( specificScalarParameters[
                 gkg::NoddiOdfCartesianField::W_UpperKappa ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerKappa ] ) )
        {

          throw std::runtime_error( 
            "delta kappa must be lower than upper bound minus "
            "lower bound" );
        }

        // check(s) for parallel_diffusivity ///////////////////////////////////
        // <lower_parallel_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::W_LowerParallelDiffusivity ] < 0.0 )
        {

          throw std::runtime_error(
                            "parallel diffusivity must be positive" );

        }
        
        // <lower_parallel_diffusivity> < <upper_parallel_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::W_LowerParallelDiffusivity ] > 
             specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::W_UpperParallelDiffusivity ])
        {

          throw std::runtime_error(
                            "lower parallel diffusivity is greater than "
                            "upper parallel diffusivity" );

        }

        // <initial_parallel_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialParallelDiffusivity ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerParallelDiffusivity ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialParallelDiffusivity ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperParallelDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "initial parallel diffusivity must be between "
                             "lower and upper bounds" );

        }

        // <delta_parallel_diffusivity>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_DeltaParallelDiffusivity ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperParallelDiffusivity ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerParallelDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "delta parallel diffusivity must be lower "
                             "than upper bound minus lower bound" );

        }
     
        
        // check(s) for isotropic_diffusivity /////////////////////////////////
        // <initial_isotropic_diffusivity>
        if ( specificScalarParameters[ 
           gkg::NoddiOdfCartesianField::W_InitialIsotropicDiffusivity ] < 0.0 )
        {

          throw std::runtime_error(
                           "isotropic diffusivity fraction must be positive" );

        }
        
        // <lower_isotropic_diffusivity> < <upper_isotropic_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::W_LowerIsotropicDiffusivity ] > 
             specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::W_UpperIsotropicDiffusivity ])
        {

          throw std::runtime_error(
                            "lower isotropic diffusivity is greater than "
                            "upper isotropic diffusivity" );

        }

        // <initial_isotropic_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialIsotropicDiffusivity ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerIsotropicDiffusivity ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialIsotropicDiffusivity ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperIsotropicDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "initial parallel diffusivity must be between "
                             "lower and upper bounds" );

        }

        // <delta_isotropic_diffusivity>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_DeltaIsotropicDiffusivity ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperIsotropicDiffusivity ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerIsotropicDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "delta isotropic diffusivity must be lower than "
                             "upper bound minus lower bound" );

        }

        // check(s) for stationary fraction /////////////////////////////////////
        // <lower_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_LowerStationaryFraction ] < 0.0 )
        {

          throw std::runtime_error(
                                 "lower stationary fraction must be positive" );

        }
        // <upper_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_UpperStationaryFraction ] > 1.0 )
        {

          throw std::runtime_error(
                    "upper stationary fraction must be lower or equal to 1.0" );

        }
        // <lower_stationary_fraction> < <upper_stationary_fraction>
        if ( specificScalarParameters[ W_LowerStationaryFraction ] >
             specificScalarParameters[ W_UpperStationaryFraction ] )
        {

          throw std::runtime_error( "lower stationary fraction is greater than "
                                    "upper stationary fraction" );

        }
        // <initial_stationary_fraction>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialStationaryFraction ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerStationaryFraction ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_InitialStationaryFraction ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperStationaryFraction ] ) )
        {

          throw std::runtime_error( 
                      "initial stationary fraction must be between lower and"
                                                               "upper bounds" );

        }

        // <delta_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::W_DeltaStationaryFraction ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_UpperStationaryFraction ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_LowerStationaryFraction ] ) )
        {

          throw std::runtime_error( 
            "delta stationary fraction must be lower than upper bound minus "
                                                                "lower bound" );
        }

        // check for combined isotropic, intracellular & stationary/////////////
 
       if ( ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_InitialIsotropicFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_InitialIntracellularFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::W_InitialStationaryFraction ] )
                                                                        > 1.0 )
        {

          throw std::runtime_error( 
                            "the sum of initial isotropic, intracellular & "
                            " stationary fractions is greater than 1.0 ");

        }
        



        // check(s) for fixed //////////////////////////////////////////////////
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIsotropicFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIsotropicFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                              "fixed isotropic fraction must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIntracellularFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIntracellularFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                         "fixed intracellular fraction must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedKappa ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedKappa ] 
               != 1.0 ) )
        {

          throw std::runtime_error( "fixed kappa must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedPrincipalOrientation ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedPrincipalOrientation ] 
               != 1.0 ) )
        {

          throw std::runtime_error( 
                           "fixed principal orientation must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedParallelDiffusivity ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedParallelDiffusivity ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                            "fixed parallel diffusivity must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIsotropicDiffusivity ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedIsotropicDiffusivity ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                           "fixed isotropic diffusivity must be either 0 or 1");

        }

        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedStationaryFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::W_FixedStationaryFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                             "fixed stationary fraction must be either 0 or 1");

        }


       // check for noise standard deviation //////////////////////////////////
        if ( specificScalarParameters[
               gkg::NoddiOdfCartesianField::W_NoiseStandardDeviation ] <= 0.0 )
        {

          throw std::runtime_error(
                         "noise standard deviation must be strictly positive" );

        }

        // check(s) for QuickSeach parameter ///////////////////////////////////
        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::W_QuickSearchStepCount ] <= 0.0 )
        {

          throw std::runtime_error(
                           "QuickSearch step count must be strictly positive" );

        }

        // check(s) for NLP parameter //////////////////////////////////////////
        if ( specificScalarParameters[
              gkg::NoddiOdfCartesianField::W_NlpMaximumIterationCount ] <= 0.0 )
        {

          throw std::runtime_error(
                      "NLP maximum iteration count must be strictly positive" );

        }
        if ( specificScalarParameters[
            gkg::NoddiOdfCartesianField::W_NlpMaximumSizeOfTheSimplex ] <= 0.0 )
        {

          throw std::runtime_error(
                  "NLP maximum size of the simplex must be strictly positive" );

        }

        // check(s) for MCMCM parameter ////////////////////////////////////////
        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::W_McmcBurninCount ] <= 0.0 )
        {

          throw std::runtime_error(
                                "MCMC burnin count must be strictly positive" );

        }

        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::W_McmcSampleCount ] <= 0.0 )
        {

          throw std::runtime_error(
                                "MCMC sample count must be strictly positive" );

        }

        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::W_McmcIntervalCount ] <= 0.0 )
        {

          throw std::runtime_error(
                              "MCMC interval count must be strictly positive" );

        }

        if ( specificScalarParameters[
             gkg::NoddiOdfCartesianField::W_McmcMaximumIterationCount ] <= 0.0 )
        {

          throw std::runtime_error(
                     "MCMC maximum iteration count must be strictly positive" );

        }

      }
      else if ( specificStringParameters[ 0 ] == "bingham" )
      {

         // check for scalar vector size ////////////////////////////////////////
        if ( specificScalarParameters.size() != 48U )
        {

          throw std::runtime_error( "invalid scalar parameter count" );

        }

        // check(s) for isotropic fraction /////////////////////////////////////
        // <lower_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerIsotropicFraction ] < 0.0 )
        {

          throw std::runtime_error(
                                 "lower isotropic fraction must be positive" );

        }
        // <upper_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperIsotropicFraction ] > 1.0 )
        {

          throw std::runtime_error(
                    "upper isotropic fraction must be lower or equal to 1.0" );

        }
        // <lower_isotropic_fraction> < <upper_isotropic_fraction>
        if ( specificScalarParameters[ B_LowerIsotropicFraction ] >
             specificScalarParameters[ B_UpperIsotropicFraction ] )
        {

          throw std::runtime_error( "lower isotropic fraction is greater than "
                                    "upper isotropic fraction" );

        }
        // <initial_isotropic_fraction>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialIsotropicFraction ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerIsotropicFraction ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialIsotropicFraction ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperIsotropicFraction ] ) )
        {

          throw std::runtime_error( 
          "initial isotropic fraction must be between lower and upper bounds" );

        }
        // <delta_isotropic_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_DeltaIsotropicFraction ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperIsotropicFraction ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerIsotropicFraction ] ) )
        {

          throw std::runtime_error( 
            "delta isotropic fraction must be lower than upper bound minus"
            " lower bound" );
        }

        // check(s) for intracellular fraction /////////////////////////////////
        // <lower_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerIntracellularFraction ]
                                                                        < 0.0 )
        {

          throw std::runtime_error(
                              "lower intracellular fraction must be positive" );

        }
        // <upper_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperIntracellularFraction ] 
                                                                        > 1.0 )
        {

          throw std::runtime_error(
                 "upper intracellular fraction must be lower or equal to 1.0" );

        }
        // <lower_intracellular_fraction> < <upper_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerIntracellularFraction ] > 
             specificScalarParameters[
               gkg::NoddiOdfCartesianField::B_UpperIntracellularFraction ] )
        {

          throw std::runtime_error( "lower intracellular fraction is greater "
                                    "than upper intracellular fraction" );

        }
        // <initial_intracellular_fraction>
        if ( ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_InitialIntracellularFraction ] < 
               specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerIntracellularFraction ] ) ||
             ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_InitialIntracellularFraction ] > 
               specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperIntracellularFraction ] ) )
        {

          throw std::runtime_error( 
               "initial intracellular fraction must be between lower and upper "
               "bounds" );

        }
        // <delta_intracellular_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_DeltaIntracellularFraction ] >= 
             ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_UpperIntracellularFraction ] -
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_LowerIntracellularFraction ] ) )
        {

          throw std::runtime_error( 
            "delta intracellular fraction must be lower than upper bound minus "
            "lower bound" );
          
        }
        // check for combined isotropic & intracellular ////////////////////////
        if ( ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_InitialIsotropicFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_InitialIntracellularFraction ] )
                                                                        > 1.0 )
        {

          throw std::runtime_error( 
                            "the sum of isotropic & intracellular fractions is "
                            "greater than 1.0" );

        }
        
        // check(s) for kappa1 /////////////////////////////////////////////////
        // <lower_kappa1>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerKappa1 ] < 0.0 )
        {

          throw std::runtime_error( "lower kappa1 must be positive" );

        }
        // <lower_kappa1> < <upper_kappa1>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerKappa1 ] > 
             specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperKappa1 ] )
        {

          throw std::runtime_error( "lower kappa1 is greater "
                                    "than upper kappa1" );

        }
        // <initial_kappa1>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialKappa1 ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerKappa1 ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialKappa1 ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperKappa1 ] ) )
        {

          throw std::runtime_error( 
                      "initial kappa1 must be between lower and upper bounds" );

        }
        // <delta_kappa1>
        if ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_DeltaKappa1 ] >= 
             ( specificScalarParameters[
                 gkg::NoddiOdfCartesianField::B_UpperKappa1 ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerKappa1 ] ) )
        {

          throw std::runtime_error( 
            "delta kappa1 must be lower than upper bound minus "
            "lower bound" );
        }

        // check(s) for kappa2 /////////////////////////////////////////////////
        // <lower_kappa2>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerKappa2 ] < 0.0 )
        {

          throw std::runtime_error( "lower kappa2 must be positive" );

        }
        // <lower_kappa2> < <upper_kappa2>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerKappa2 ] > 
             specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperKappa2 ] )
        {

          throw std::runtime_error( "lower kappa2 is greater "
                                    "than upper kappa1" );

        }
        // <initial_kappa2>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialKappa2 ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerKappa2 ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialKappa2 ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperKappa2 ] ) )
        {

          throw std::runtime_error( 
                      "initial kappa2 must be between lower and upper bounds" );

        }
        // <delta_kappa2>
        if ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_DeltaKappa2 ] >= 
             ( specificScalarParameters[
                 gkg::NoddiOdfCartesianField::B_UpperKappa2 ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerKappa2 ] ) )
        {

          throw std::runtime_error( 
            "delta kappa2 must be lower than upper bound minus lower bound" );
        }

        // check(s) for parallel_diffusivity ///////////////////////////////////
        // <lower_parallel_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::B_LowerParallelDiffusivity ] < 0.0 )
        {

          throw std::runtime_error(
                            "parallel diffusivity must be positive" );

        }
        
        // <lower_parallel_diffusivity> < <upper_parallel_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::B_LowerParallelDiffusivity ] > 
             specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::B_UpperParallelDiffusivity ])
        {

          throw std::runtime_error(
                            "lower parallel diffusivity is greater than "
                            "upper parallel diffusivity" );

        }

        // <initial_parallel_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialParallelDiffusivity ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerParallelDiffusivity ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialParallelDiffusivity ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperParallelDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "initial parallel diffusivity must be between "
                             "lower and upper bounds" );

        }

        // <delta_parallel_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_DeltaParallelDiffusivity ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperParallelDiffusivity ] ) -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerParallelDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "delta parallel diffusivity must be lower "
                             "than upper bound minus lower bound" );

        }
     
        
        // check(s) for isotropic_diffusivity /////////////////////////////////
        // <initial_isotropic_diffusivity>
        if ( specificScalarParameters[ 
           gkg::NoddiOdfCartesianField::B_InitialIsotropicDiffusivity ] < 0.0 )
        {

          throw std::runtime_error(
                           "isotropic diffusivity fraction must be positive" );

        }
        
        // <lower_isotropic_diffusivity> < <upper_isotropic_diffusivity>
        if ( specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::B_LowerIsotropicDiffusivity ] > 
             specificScalarParameters[ 
            gkg::NoddiOdfCartesianField::B_UpperIsotropicDiffusivity ])
        {

          throw std::runtime_error(
                            "lower isotropic diffusivity is greater than "
                            "upper isotropic diffusivity" );

        }

        // <initial_isotropic_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialIsotropicDiffusivity ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerIsotropicDiffusivity ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialIsotropicDiffusivity ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperIsotropicDiffusivity ] ) )
        {

          throw std::runtime_error( 
                             "initial parallel diffusivity must be between "
                             "lower and upper bounds" );

        }

        // <delta_isotropic_diffusivity>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_DeltaIsotropicDiffusivity ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperIsotropicDiffusivity ] ) -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerIsotropicDiffusivity ] ) )
        {

          throw std::runtime_error( 
                               "delta isotropic diffusivity must be lower than "
                               "upper bound minus lower bound" );

        }

       // check(s) for stationary fraction /////////////////////////////////////
        // <lower_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_LowerStationaryFraction ] < 0.0 )
        {

          throw std::runtime_error(
                                 "lower stationary fraction must be positive" );

        }
        // <upper_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_UpperStationaryFraction ] > 1.0 )
        {

          throw std::runtime_error(
                    "upper stationary fraction must be lower or equal to 1.0" );

        }
        // <lower_stationary_fraction> < <upper_stationary_fraction>
        if ( specificScalarParameters[ B_LowerStationaryFraction ] >
             specificScalarParameters[ B_UpperStationaryFraction ] )
        {

          throw std::runtime_error( "lower stationary fraction is greater than "
                                    "upper stationary fraction" );

        }
        // <initial_stationary_fraction>
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialStationaryFraction ] < 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerStationaryFraction ] ) ||
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_InitialStationaryFraction ] > 
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperStationaryFraction ] ) )
        {

          throw std::runtime_error( 
                      "initial stationary fraction must be between lower and" 
                                                               "upper bounds" );

        }
        // <delta_stationary_fraction>
        if ( specificScalarParameters[ 
               gkg::NoddiOdfCartesianField::B_DeltaStationaryFraction ] >= 
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_UpperStationaryFraction ] -
               specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_LowerStationaryFraction ] ) )
        {

          throw std::runtime_error( 
            "delta stationary fraction must be lower than upper bound minus"
                                                              " lower bound" );
        }

        // check for combined isotropic, intracellular & stationary  ///////////
        if ( ( specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_InitialIsotropicFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_InitialIntracellularFraction ] +
               specificScalarParameters[ 
                gkg::NoddiOdfCartesianField::B_InitialStationaryFraction ] )
                                                                        > 1.0 )
        {

          throw std::runtime_error( 
                            "the sum of initial isotropic, intracellular & "
                            " stationary fractions is greater than 1.0 ");
        }


        // check(s) for fixed //////////////////////////////////////////////////
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIsotropicFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIsotropicFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                              "fixed isotropic fraction must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIntracellularFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIntracellularFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                         "fixed intracellular fraction must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedKappa1 ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedKappa1 ] 
               != 1.0 ) )
        {

          throw std::runtime_error( "fixed kappa1 must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedKappa2 ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedKappa2 ] 
               != 1.0 ) )
        {

          throw std::runtime_error( "fixed kappa2 must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedPrincipalOrientation ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedPrincipalOrientation ] 
               != 1.0 ) )
        {

          throw std::runtime_error( 
                           "fixed principal orientation must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedSecondaryOrientation ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedSecondaryOrientation ] 
               != 1.0 ) )
        {

          throw std::runtime_error( 
                           "fixed secondary orientation must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedParallelDiffusivity ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedParallelDiffusivity ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                            "fixed parallel diffusivity must be either 0 or 1");

        }
        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIsotropicDiffusivity ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedIsotropicDiffusivity ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                           "fixed isotropic diffusivity must be either 0 or 1");

        }

        if ( ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedStationaryFraction ] 
               != 0.0 ) &&
             ( specificScalarParameters[ 
                 gkg::NoddiOdfCartesianField::B_FixedStationaryFraction ] 
               != 1.0 ) )
        {

          throw std::runtime_error(
                             "fixed stationary fraction must be either 0 or 1");

        }



        // check for noise standard deviation //////////////////////////////////
        if ( specificScalarParameters[
               gkg::NoddiOdfCartesianField::B_NoiseStandardDeviation ] <= 0.0 )
        {

          throw std::runtime_error(
                         "noise standard deviation must be strictly positive" );

        }

        // check(s) for QuickSeach parameter ///////////////////////////////////
        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::B_QuickSearchStepCount ] <= 0.0 )
        {

          throw std::runtime_error(
                           "QuickSearch step count must be strictly positive" );

        }

        // check(s) for NLP parameter //////////////////////////////////////////
        if ( specificScalarParameters[
              gkg::NoddiOdfCartesianField::B_NlpMaximumIterationCount ] <= 0.0 )
        {

          throw std::runtime_error(
                      "NLP maximum iteration count must be strictly positive" );

        }
        if ( specificScalarParameters[
            gkg::NoddiOdfCartesianField::B_NlpMaximumSizeOfTheSimplex ] <= 0.0 )
        {

          throw std::runtime_error(
                  "NLP maximum size of the simplex must be strictly positive" );

        }

        // check(s) for MCMCM parameter ////////////////////////////////////////
        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::B_McmcBurninCount ] <= 0.0 )
        {

          throw std::runtime_error(
                                "MCMC burnin count must be strictly positive" );

        }

        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::B_McmcSampleCount ] <= 0.0 )
        {

          throw std::runtime_error(
                                "MCMC sample count must be strictly positive" );

        }

        if ( specificScalarParameters[
                gkg::NoddiOdfCartesianField::B_McmcIntervalCount ] <= 0.0 )
        {

          throw std::runtime_error(
                              "MCMC interval count must be strictly positive" );

        }

        if ( specificScalarParameters[
             gkg::NoddiOdfCartesianField::B_McmcMaximumIterationCount ] <= 0.0 )
        {

          throw std::runtime_error(
                     "MCMC maximum iteration count must be strictly positive" );

        }
        
      }

    }

  }
  GKG_CATCH( "void gkg::NoddiOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::NoddiOdfCartesianField::initialize( 
              const std::vector< double >& specificScalarParameters,
              const std::vector< std::string >& /* specificStringParameters */ )
{

  try
  {

    // collecting parameter(s) to compute NODDI model
    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {

      // collecting initial parameter(s)
      _initialParameters.reallocate( 9 );
      _initialParameters( 0 ) = specificScalarParameters[
                                           W_InitialIsotropicFraction ];
      _initialParameters( 1 ) = specificScalarParameters[
                                           W_InitialIntracellularFraction ];
      _initialParameters( 2 ) = specificScalarParameters[
                                           W_InitialKappa ];
      _initialParameters( 3 ) = 0.0; // will be set to firt eigenvector of DTI
      _initialParameters( 4 ) = 0.0;
      _initialParameters( 5 ) = 0.0;
      _initialParameters( 6 ) = specificScalarParameters[
                                           W_InitialParallelDiffusivity ];
      _initialParameters( 7 ) = specificScalarParameters[
                                           W_InitialIsotropicDiffusivity ];
      _initialParameters( 8 ) = specificScalarParameters[
                                           W_InitialStationaryFraction ];


      // collecting lower parameter(s)
      _lowerParameters.reallocate( 9 );
      _lowerParameters( 0 ) = specificScalarParameters[
                                               W_LowerIsotropicFraction ];
      _lowerParameters( 1 ) = specificScalarParameters[
                                               W_LowerIntracellularFraction ];
      _lowerParameters( 2 ) = specificScalarParameters[
                                               W_LowerKappa ];
      _lowerParameters( 3 ) = -1.0;
      _lowerParameters( 4 ) = -1.0;
      _lowerParameters( 5 ) = -1.0;
      _lowerParameters( 6 ) = specificScalarParameters[
                                               W_LowerParallelDiffusivity ];
      _lowerParameters( 7 ) = specificScalarParameters[
                                               W_LowerIsotropicDiffusivity ];
      _lowerParameters( 8 ) = specificScalarParameters[
                                               W_LowerStationaryFraction ];


      // collecting upper parameter(s)
      _upperParameters.reallocate( 9 );
      _upperParameters( 0 ) = specificScalarParameters[
                                               W_UpperIsotropicFraction ];
      _upperParameters( 1 ) = specificScalarParameters[
                                               W_UpperIntracellularFraction ];
      _upperParameters( 2 ) = specificScalarParameters[
                                               W_UpperKappa ];
      _upperParameters( 3 ) = 1.0;
      _upperParameters( 4 ) = 1.0;
      _upperParameters( 5 ) = 1.0;
      _upperParameters( 6 ) = specificScalarParameters[
                                               W_UpperParallelDiffusivity ];
      _upperParameters( 7 ) = specificScalarParameters[
                                               W_UpperIsotropicDiffusivity ];
      _upperParameters( 8 ) = specificScalarParameters[
                                               W_UpperStationaryFraction ];


      // collecting delta parameter(s)
      _deltaParameters.reallocate( 9 );
      _deltaParameters( 0 ) = specificScalarParameters[
                                               W_DeltaIsotropicFraction ];
      _deltaParameters( 1 ) = specificScalarParameters[
                                               W_DeltaIntracellularFraction ];
      _deltaParameters( 2 ) = specificScalarParameters[
                                               W_DeltaKappa ];
      _deltaParameters( 3 ) = 0.02;
      _deltaParameters( 4 ) = 0.02;
      _deltaParameters( 5 ) = 0.02;
      _deltaParameters( 6 ) = specificScalarParameters[
                                               W_DeltaParallelDiffusivity ];
      _deltaParameters( 7 ) = specificScalarParameters[
                                               W_DeltaIsotropicDiffusivity ];
      _deltaParameters( 8 ) = specificScalarParameters[
                                               W_DeltaStationaryFraction ];


      // collecting fixed parameter(s)
      _fixed.resize( 9U );
      _fixed[ 0 ] = ( specificScalarParameters[
                      W_FixedIsotropicFraction ] > 0.0 ? true : false );
      _fixed[ 1 ] = ( specificScalarParameters[
                      W_FixedIntracellularFraction ] > 0.0 ? true : false );
      _fixed[ 2 ] = ( specificScalarParameters[
                      W_FixedKappa ] > 0.0 ? true : false );
      _fixed[ 3 ] = ( specificScalarParameters[
                      W_FixedPrincipalOrientation ] > 0.0 ? true : false );
      _fixed[ 4 ] = _fixed[ 3 ];
      _fixed[ 5 ] = _fixed[ 3 ];
      _fixed[ 6 ] = ( specificScalarParameters[
                      W_FixedParallelDiffusivity ] > 0.0 ? true : false );
      _fixed[ 7 ] = ( specificScalarParameters[
                      W_FixedIsotropicDiffusivity ] > 0.0 ? true : false );
      _fixed[ 8 ] = ( specificScalarParameters[
                      W_FixedStationaryFraction ] > 0.0 ? true : false );
      


      // collecting noise standard deviation
      _noiseStandardDeviation = specificScalarParameters[
                                                     W_NoiseStandardDeviation ];

      // collecting QuickSearch parameters
      _applyQuickSearchOptimizer = ( specificScalarParameters[
                           W_ApplyQuickSearchOptimizer ] > 0.0 ? true : false );
      _quicksearchStepCount = ( int32_t )( specificScalarParameters[
                                               W_QuickSearchStepCount ] + 0.5 );

      // collecting NLP parameters
      _applyNlpOptimizer = ( specificScalarParameters[
                                   W_ApplyNlpOptimizer ] > 0.0 ? true : false );
      _nlpMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                           W_NlpMaximumIterationCount ] + 0.5 );
      _nlpMaximumSizeOfTheSimplex = specificScalarParameters[
                                                 W_NlpMaximumSizeOfTheSimplex ];

      // collecting MCMC parameters
      _applyMcmcOptimizer = ( specificScalarParameters[
                                  W_ApplyMcmcOptimizer ] > 0.0 ? true : false );
      _mcmcBurninCount = ( int32_t )( specificScalarParameters[
                                          W_McmcBurninCount ] + 0.5 );
      _mcmcSampleCount = ( int32_t )( specificScalarParameters[
                                          W_McmcSampleCount ] + 0.5 );
      _mcmcIntervalCount = ( int32_t )( specificScalarParameters[
                                          W_McmcIntervalCount ] + 0.5 );
      _mcmcMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                          W_McmcMaximumIterationCount ] + 0.5 );

    }
    else if ( _sphereDistributionFunction ==
              gkg::NoddiOdfCartesianField::Bingham )
    {

      // collecting initial parameter(s)
      _initialParameters.reallocate( 13 );
      _initialParameters( 0 ) = specificScalarParameters[
                                           B_InitialIsotropicFraction ];
      _initialParameters( 1 ) = specificScalarParameters[
                                           B_InitialIntracellularFraction ];
      _initialParameters( 2 ) = specificScalarParameters[
                                           B_InitialKappa1 ];
      _initialParameters( 3 ) = specificScalarParameters[
                                           B_InitialKappa2 ];
      _initialParameters( 4 ) = 0.0; // will be set to firt eigenvector of DTI
      _initialParameters( 5 ) = 0.0;
      _initialParameters( 6 ) = 0.0;
      _initialParameters( 7 ) = 0.0; // will be set to second eigenvector of DTI
      _initialParameters( 8 ) = 0.0;
      _initialParameters( 9 ) = 0.0;
      _initialParameters( 10 ) = specificScalarParameters[
                                           B_InitialParallelDiffusivity ];
      _initialParameters( 11 ) = specificScalarParameters[
                                           B_InitialIsotropicDiffusivity ];
      _initialParameters( 12 ) = specificScalarParameters[
                                           B_InitialStationaryFraction ];

      // collecting lower parameter(s)
      _lowerParameters.reallocate( 13 );
      _lowerParameters( 0 ) = specificScalarParameters[
                                               B_LowerIsotropicFraction ];
      _lowerParameters( 1 ) = specificScalarParameters[
                                               B_LowerIntracellularFraction ];
      _lowerParameters( 2 ) = specificScalarParameters[
                                               B_LowerKappa1 ];
      _lowerParameters( 3 ) = specificScalarParameters[
                                               B_LowerKappa2 ];
      _lowerParameters( 4 ) = -1.0;
      _lowerParameters( 5 ) = -1.0;
      _lowerParameters( 6 ) = -1.0;
      _lowerParameters( 7 ) = -1.0;
      _lowerParameters( 8 ) = -1.0;
      _lowerParameters( 9 ) = -1.0;
      _lowerParameters( 10 ) = specificScalarParameters[
                                               B_LowerParallelDiffusivity ];
      _lowerParameters( 11 ) = specificScalarParameters[
                                               B_LowerIsotropicDiffusivity ];
      _lowerParameters( 12 ) = specificScalarParameters[
                                               B_LowerStationaryFraction ];


      // collecting upper parameter(s)
      _upperParameters.reallocate( 13 );
      _upperParameters( 0 ) = specificScalarParameters[
                                               B_UpperIsotropicFraction ];
      _upperParameters( 1 ) = specificScalarParameters[
                                               B_UpperIntracellularFraction ];
      _upperParameters( 2 ) = specificScalarParameters[
                                               B_UpperKappa1 ];
      _upperParameters( 3 ) = specificScalarParameters[
                                               B_UpperKappa2 ];
      _lowerParameters( 4 ) = +1.0;
      _lowerParameters( 5 ) = +1.0;
      _lowerParameters( 6 ) = +1.0;
      _lowerParameters( 7 ) = +1.0;
      _lowerParameters( 8 ) = +1.0;
      _lowerParameters( 9 ) = +1.0;
      _upperParameters( 10 ) = specificScalarParameters[
                                               B_UpperParallelDiffusivity ];
      _upperParameters( 11 ) = specificScalarParameters[
                                               B_UpperIsotropicDiffusivity ];
      _upperParameters( 12 ) = specificScalarParameters[
                                               B_UpperStationaryFraction ];


      // collecting delta parameter(s)
      _deltaParameters.reallocate( 13 );
      _deltaParameters( 0 ) = specificScalarParameters[
                                               B_DeltaIsotropicFraction ];
      _deltaParameters( 1 ) = specificScalarParameters[
                                               B_DeltaIntracellularFraction ];
      _deltaParameters( 2 ) = specificScalarParameters[
                                               B_DeltaKappa1 ];
      _deltaParameters( 3 ) = specificScalarParameters[
                                               B_DeltaKappa2 ];
      _deltaParameters( 4 ) = 0.02;
      _deltaParameters( 5 ) = 0.02;
      _deltaParameters( 6 ) = 0.02;
      _deltaParameters( 7 ) = 0.02;
      _deltaParameters( 8 ) = 0.02;
      _deltaParameters( 9 ) = 0.02;
      _deltaParameters( 10 ) = specificScalarParameters[
                                               B_DeltaParallelDiffusivity ];
      _deltaParameters( 11 ) = specificScalarParameters[
                                               B_DeltaIsotropicDiffusivity ];
      _deltaParameters( 12 ) = specificScalarParameters[
                                               B_DeltaStationaryFraction ];


      // collecting fixed parameter(s)
      _fixed.resize( 9U );
      _fixed[ 0 ] = ( specificScalarParameters[
                      B_FixedIsotropicFraction ] > 0.0 ? true : false );
      _fixed[ 1 ] = ( specificScalarParameters[
                      B_FixedIntracellularFraction ] > 0.0 ? true : false );
      _fixed[ 2 ] = ( specificScalarParameters[
                      B_FixedKappa1 ] > 0.0 ? true : false );
      _fixed[ 3 ] = ( specificScalarParameters[
                      B_FixedKappa2 ] > 0.0 ? true : false );
      _fixed[ 4 ] = ( specificScalarParameters[
                      B_FixedPrincipalOrientation ] > 0.0 ? true : false );
      _fixed[ 5 ] = _fixed[ 4 ];
      _fixed[ 6 ] = _fixed[ 4 ];

      _fixed[ 7 ] = ( specificScalarParameters[
                      B_FixedSecondaryOrientation ] > 0.0 ? true : false );
      _fixed[ 8 ] = _fixed[ 7 ];
      _fixed[ 9 ] = _fixed[ 7 ];
      _fixed[ 10 ] = ( specificScalarParameters[
                      B_FixedParallelDiffusivity ] > 0.0 ? true : false );
      _fixed[ 11 ] = ( specificScalarParameters[
                      B_FixedIsotropicDiffusivity ] > 0.0 ? true : false );
      _fixed[ 12 ] = ( specificScalarParameters[
                      B_FixedStationaryFraction ] > 0.0 ? true : false );


      // collecting noise standard deviation
      _noiseStandardDeviation = specificScalarParameters[
                                                     B_NoiseStandardDeviation ];

      // collecting QuickSearch parameters
      _applyQuickSearchOptimizer = ( specificScalarParameters[
                           B_ApplyQuickSearchOptimizer ] > 0.0 ? true : false );
      _quicksearchStepCount = ( int32_t )( specificScalarParameters[
                                               B_QuickSearchStepCount ] + 0.5 );

      // collecting NLP parameters
      _applyQuickSearchOptimizer = ( specificScalarParameters[
                                   B_ApplyNlpOptimizer ] > 0.0 ? true : false );
      _nlpMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                           B_NlpMaximumIterationCount ] + 0.5 );
      _nlpMaximumSizeOfTheSimplex = specificScalarParameters[
                                                 B_NlpMaximumSizeOfTheSimplex ];

      // collecting MCMC parameters
      _applyMcmcOptimizer = ( specificScalarParameters[
                                  B_ApplyMcmcOptimizer ] > 0.0 ? true : false );
      _mcmcBurninCount = ( int32_t )( specificScalarParameters[
                                          B_McmcBurninCount ] + 0.5 );
      _mcmcSampleCount = ( int32_t )( specificScalarParameters[
                                          B_McmcSampleCount ] + 0.5 );
      _mcmcIntervalCount = ( int32_t )( specificScalarParameters[
                                          B_McmcIntervalCount ] + 0.5 );
      _mcmcMaximumIterationCount = ( int32_t )( specificScalarParameters[
                                          B_McmcMaximumIterationCount ] + 0.5 );

    }

  }
  GKG_CATCH( "void gkg::NoddiOdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters )" );

}


void 
gkg::NoddiOdfCartesianField::prepareMCMCFunction( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "preparing MCMC function" << std::flush;

    }

    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {

      _noddiMCMCFunction.reset( new gkg::NoddiMCMCWatsonFunction(
                                      this->_dw,
                                      this->_outputOrientationSet,
                                      this->_noiseStandardDeviation ) );

    }
    else if ( _sphereDistributionFunction ==
               gkg::NoddiOdfCartesianField::Bingham )
    {

      _noddiMCMCFunction.reset( new gkg::NoddiMCMCBinghamFunction(
                                  this->_dw,
                                  this->_outputOrientationSet,
                                  this->_noiseStandardDeviation ) );

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 23 ) << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::NoddiOdfCartesianField::prepareMCMCFunction( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::NoddiOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {
   
    gkg::Vector initialParameters( _initialParameters );

    gkg::RCPointer< gkg::NoddiMCMCParameterSet > noddiMCMCParameterSet; 
    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {

      noddiMCMCParameterSet.reset( new gkg::NoddiMCMCWatsonParameterSet(
                                      this->_lowerParameters,
                                      this->_upperParameters,
                                      this->_fixed,
                                      this->_deltaParameters ) );

      // setting the principal orientation to the first eigenvector of the 
      // diffusion tensor model
      const Vector3d< float >& principalOrientation =
                  _tensorCartesianField->getItem( originSite )->getEigenVector(
                                                         gkg::Tensor::Maximum );
      initialParameters( 3 ) = principalOrientation.x;
      initialParameters( 4 ) = principalOrientation.y;
      initialParameters( 5 ) = principalOrientation.z;

    }
    else if ( _sphereDistributionFunction ==
              gkg::NoddiOdfCartesianField::Bingham )
    {

      noddiMCMCParameterSet.reset( new gkg::NoddiMCMCBinghamParameterSet(
                                  this->_lowerParameters,
                                  this->_upperParameters,
                                  this->_fixed,
                                  this->_deltaParameters ) );

      // setting the principal orientation to the first eigenvector of the 
      // diffusion tensor model and the secondary orientation to the second
      // eigenvector of the diffusion tensor model
      const Vector3d< float >& principalOrientation =
                  _tensorCartesianField->getItem( originSite )->getEigenVector(
                                                         gkg::Tensor::Maximum );
      initialParameters( 4 ) = principalOrientation.x;
      initialParameters( 5 ) = principalOrientation.y;
      initialParameters( 6 ) = principalOrientation.z;

      const Vector3d< float >& secondaryOrientation =
                    _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                          gkg::Tensor::Middle );
      initialParameters( 7 ) = secondaryOrientation.x;
      initialParameters( 8 ) = secondaryOrientation.y;
      initialParameters( 9 ) = secondaryOrientation.z;

    }

    gkg::NoddiOdf noddiOdf( &_outputOrientationSet,
                            *_t2, *_dw,
                            originSite,
                            _noddiMCMCFunction,
                            noddiMCMCParameterSet,
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
                              noddiOdf );

    }
    else
    {

      return noddiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::NoddiOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::NoddiOdfCartesianField::newItem( const gkg::Vector3d< float >& originSite,
                                      const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::Vector initialParameters( _initialParameters );
    gkg::RCPointer< gkg::NoddiMCMCParameterSet > noddiMCMCParameterSet; 
    if ( _sphereDistributionFunction ==
         gkg::NoddiOdfCartesianField::Watson )
    {

      noddiMCMCParameterSet.reset( new gkg::NoddiMCMCWatsonParameterSet(
                                      this->_lowerParameters,
                                      this->_upperParameters,
                                      this->_fixed,
                                      this->_deltaParameters ) );

      // setting the principal orientation to the first eigenvector of the 
      // diffusion tensor model

      // BUG if using iBox: should use originSite coordinate to get direction
      // instead of using lutSite hereafter:

      const Vector3d< float >& principalOrientation =
                    _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                         gkg::Tensor::Maximum );
      initialParameters( 3 ) = principalOrientation.x;
      initialParameters( 4 ) = principalOrientation.y;
      initialParameters( 5 ) = principalOrientation.z;

    }
    else if ( _sphereDistributionFunction ==
              gkg::NoddiOdfCartesianField::Bingham )
    {

      noddiMCMCParameterSet.reset( new gkg::NoddiMCMCBinghamParameterSet(
                                  this->_lowerParameters,
                                  this->_upperParameters,
                                  this->_fixed,
                                  this->_deltaParameters ) );

      // setting the principal orientation to the first eigenvector of the 
      // diffusion tensor model and the secondary orientation to the second
      // eigenvector of the diffusion tensor model

      // BUG if using iBox: should use originSite coordinate to get direction
      // instead of using lutSite hereafter:

      const Vector3d< float >& principalOrientation =
                    _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                         gkg::Tensor::Maximum );
      initialParameters( 4 ) = principalOrientation.x;
      initialParameters( 5 ) = principalOrientation.y;
      initialParameters( 6 ) = principalOrientation.z;


      // BUG if using iBox: should use originSite coordinate to get direction
      // instead of using lutSite hereafter:

      const Vector3d< float >& secondaryOrientation =
                    _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                          gkg::Tensor::Middle );
      initialParameters( 7 ) = secondaryOrientation.x;
      initialParameters( 8 ) = secondaryOrientation.y;
      initialParameters( 9 ) = secondaryOrientation.z;

    }

    gkg::NoddiOdf noddiOdf( &_outputOrientationSet,
                            *_t2, *_dw,
                            originSite,
                            _noddiMCMCFunction,
                            noddiMCMCParameterSet,
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
                              noddiOdf );

    }
    else
    {

      return noddiOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::NoddiOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
