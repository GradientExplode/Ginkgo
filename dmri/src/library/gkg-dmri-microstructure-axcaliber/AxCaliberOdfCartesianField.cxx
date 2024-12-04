#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianField.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdf.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCFunction.h>
#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCParameterSet.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::AxCaliberOdfCartesianField::AxCaliberOdfCartesianField(
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

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );


    // setting ODF type and functor
    _items.getHeader().addAttribute(
                                  "odf_type",
                                  gkg::TypeOf< gkg::AxCaliberOdf >::getName() );

    this->_functor = new gkg::AxCaliberOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::AxCaliberOdfCartesianField::"
             "AxCaliberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::AxCaliberOdfCartesianField::AxCaliberOdfCartesianField(
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

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                                  "odf_type",
                                  gkg::TypeOf< gkg::AxCaliberOdf >::getName() );

    this->_functor = new gkg::AxCaliberOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::AxCaliberOdfCartesianField::"
             "AxCaliberOdfCartesianField( "
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


gkg::AxCaliberOdfCartesianField::AxCaliberOdfCartesianField(
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

    // initializing from specific scalar parameters
    initialize( specificScalarParameters, specificStringParameters );

    // setting ODF type and functor
    _items.getHeader().addAttribute(
                                  "odf_type",
                                  gkg::TypeOf< gkg::AxCaliberOdf >::getName() );

    this->_functor = new gkg::AxCaliberOdfCartesianFieldFunctor( *this );

    // preparing MCMC function
    prepareMCMCFunction(  verbose );

    // computing ODFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::AxCaliberOdfCartesianField::"
             "AxCaliberOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::AxCaliberOdfCartesianField::~AxCaliberOdfCartesianField()
{
}



const gkg::Vector& gkg::AxCaliberOdfCartesianField::getInitialParameters() const
{

  try
  {

    return _initialParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxCaliberOdfCartesianField::getInitialParameters() const" );

}


const gkg::Vector& gkg::AxCaliberOdfCartesianField::getLowerParameters() const
{

  try
  {

    return _lowerParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxCaliberOdfCartesianField::getLowerParameters() const" );

}


const gkg::Vector& gkg::AxCaliberOdfCartesianField::getUpperParameters() const
{

  try
  {

    return _upperParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxCaliberOdfCartesianField::getUpperParameters() const" );

}


const gkg::Vector& gkg::AxCaliberOdfCartesianField::getDeltaParameters() const
{

  try
  {

    return _deltaParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxCaliberOdfCartesianField::getDeltaParameters() const" );

}


const std::vector< bool >& gkg::AxCaliberOdfCartesianField::getFixed() const
{

  try
  {

    return _fixed;

  }
  GKG_CATCH( "const std::vector< bool >& "
             "gkg::AxCaliberOdfCartesianField::getFixed() const" );

}


int32_t
gkg::AxCaliberOdfCartesianField::getMaximumCallaghanModelRootCount() const
{

  try
  {

    return _maximumCallaghanModelRootCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::"
             "getMaximumCallaghanModelRootCount() "
             "const" );

}


int32_t
gkg::AxCaliberOdfCartesianField::getMaximumCallaghanModelFunctionCount() const
{

  try
  {

    return _maximumCallaghanModelFunctionCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::"
             "getMaximumCallaghanModelFunctionCount() "
             "const" );

}


double gkg::AxCaliberOdfCartesianField::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( "double "
             "gkg::AxCaliberOdfCartesianField::getNoiseStandardDeviation() "
             "const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getQuickSearchStepCount() const
{

  try
  {

    return _quicksearchStepCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getQuickSearchStepCount() "
             "const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getNLPMaximumIterationCount() const
{

  try
  {

    return _nlpMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getNLPMaximumIterationCount() "
             "const" );

}


double gkg::AxCaliberOdfCartesianField::getNLPMaximumSizeOfTheSimplex() const
{

  try
  {

    return _nlpMaximumSizeOfTheSimplex;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getNLPMaximumSizeOfTheSimplex() "
             "const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getMCMCBurninCount() const
{

  try
  {

    return _mcmcBurninCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getMCMCBurninCount() const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getMCMCSampleCount() const
{

  try
  {

    return _mcmcSampleCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getMCMCSampleCount() const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getMCMCIntervalCount() const
{

  try
  {

    return _mcmcIntervalCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::getMCMCIntervalCount() const" );

}


int32_t gkg::AxCaliberOdfCartesianField::getMCMCMaximumIterationCount() const
{

  try
  {

    return _mcmcMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberOdfCartesianField::"
             "getMCMCMaximumIterationCount() const" );

}


void gkg::AxCaliberOdfCartesianField::checkOrInitializeDefaultParameters(
                      std::vector< double >& specificScalarParameters,
                      std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( !specificStringParameters.empty() )
    {

      throw std::runtime_error(
                           "no string parameter required for AxCaliber model" );

    }

    if ( specificScalarParameters.empty() )
    {

        specificScalarParameters.resize( 43U );

        ////////////////////////////// initial /////////////////////////////////
        // InitialHinderedDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::InitialHinderedDiffusivity ] = 3e-9;
        // InitialIntracellularFraction
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::InitialIntracellularFraction ] = 0.5;
        // InitialIntracellularPerpendicularDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        InitialIntracellularPerpendicularDiffusivity ] = 1.7e-9;
        // InitialIntracellularParallelDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        InitialIntracellularParallelDiffusivity ] = 1.2e-9;
        // InitialAlpha
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::InitialDiameterMean ] = 3.0e-6;
        // InitialBeta
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::InitialDiameterStdDev ] = 0.3e-6;


        /////////////////////////////// lower //////////////////////////////////
        // LowerHinderedDiffusivity
        specificScalarParameters[
          gkg::AxCaliberOdfCartesianField::LowerHinderedDiffusivity ] = 0.0;
        // LowerIntracellularFraction
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::LowerIntracellularFraction ] = 0.0;
        // LowerIntracellularPerpendicularDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        LowerIntracellularPerpendicularDiffusivity ] = 0.0;
        // LowerIntracellularParallelDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        LowerIntracellularParallelDiffusivity ] = 0.0;
        // LowerAlpha
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::LowerDiameterMean ] = 0.0;
        // LowerBeta
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::LowerDiameterStdDev ] = 0.0;


        /////////////////////////////// upper //////////////////////////////////
        // UpperHinderedDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::UpperHinderedDiffusivity ] = 6e-9;
        // UpperIntracellularFraction
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::UpperIntracellularFraction ] = 0.5;
        // UpperIntracellularPerpendicularDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        UpperIntracellularPerpendicularDiffusivity ] = 3e-9;
        // UpperIntracellularParallelDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        UpperIntracellularParallelDiffusivity ] = 3e-9;
        // UpperAlpha
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::UpperDiameterMean ] = 10.0e-6;
        // UpperBeta
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::UpperDiameterStdDev ] = 1.0e-6;


        /////////////////////////////// delta //////////////////////////////////
        // DeltaHinderedDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                                          DeltaHinderedDiffusivity ] = 0.25e-9;
        // DeltaIntracellularFraction
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::DeltaIntracellularFraction ] = 0.05;
        // DeltaIntracellularPerpendicularDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        DeltaIntracellularPerpendicularDiffusivity ] = 0.25e-9;
        // DeltaIntracellularParallelDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        DeltaIntracellularParallelDiffusivity ] = 0.25e-9;
        // DeltaAlpha
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::DeltaDiameterMean ] = 1.0e-6;
        // DeltaBeta
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::DeltaDiameterStdDev ] = 0.1e-6;


        /////////////////////////////// fixed //////////////////////////////////
        // FixedHinderedDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::FixedHinderedDiffusivity ] = 1.0;
        // FixedIntracellularFraction
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::FixedIntracellularFraction ] = 0.0;
        // FixedIntracellularPerpendicularDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        FixedIntracellularPerpendicularDiffusivity ] = 1.0;
        // FixedIntracellularParallelDiffusivity
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                        FixedIntracellularParallelDiffusivity ] = 1.0;
        // FixedAlpha
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::FixedDiameterMean ] = 0.0;
        // FixedBeta
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::FixedDiameterStdDev ] = 0.0;


        ////////////////////// Callaghan roots and functions  //////////////////
        // MaximumCallaghanModelRootCount
        specificScalarParameters[
          gkg::AxCaliberOdfCartesianField::
                                          MaximumCallaghanModelRootCount ] = 20;
        // MaximumCallaghanModelFunctionCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::
                                      MaximumCallaghanModelFunctionCount ] = 50;


        ////////////////////////////// noise std dev  //////////////////////////
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::NoiseStandardDeviation ] = 50;


        ///////////////////////// QuickSearch optimizer  ///////////////////////
        // ApplyQuickSearchOptimizer
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::ApplyQuickSearchOptimizer ] = 1;
        // QuickSearchStepCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::QuickSearchStepCount ] = 10;


        ///////////////////////////// NLP optimizer  ///////////////////////////
        // ApplyNlpOptimizer
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::ApplyNlpOptimizer ] = 1;
        // NlpMaximumIterationCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::NlpMaximumIterationCount ] = 1000;
        // NlpMaximumSizeOfTheSimplex
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::NlpMaximumSizeOfTheSimplex ] = 0.001;


        //////////////////////////// MCMC optimizer  ///////////////////////////
        // ApplyMcmcOptimizer
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::ApplyMcmcOptimizer ] = 1;
        // McmcBurninCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::McmcBurninCount ] = 10000;
        // McmcSampleCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::McmcSampleCount ] = 100;
        // McmcIntervalCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::McmcIntervalCount ] = 1000;
        // McmcMaximumIterationCount
        specificScalarParameters[ 
          gkg::AxCaliberOdfCartesianField::McmcMaximumIterationCount ] = 50000;

    }
    else
    {

      // check for scalar vector size //////////////////////////////////////////
      if ( specificScalarParameters.size() != 43U )
      {

        throw std::runtime_error( "invalid scalar parameter count" );

      }

      // check(s) for hindered diffusivity /////////////////////////////////////
      // InitialHinderedDiffusivity
      if ( specificScalarParameters[ 
         gkg::AxCaliberOdfCartesianField::InitialHinderedDiffusivity ] < 0.0 )
      {

        throw std::runtime_error( "hindered diffusivity  must be positive" );

      }
      
      // LowerHinderedDiffusivity < UpperHinderedDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerHinderedDiffusivity ] > 
           specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperHinderedDiffusivity ])
      {

        throw std::runtime_error(
                          "lower hindered diffusivity is greater than "
                          "upper hindered diffusivity" );

      }

      // InitialHinderedDiffusivity
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialHinderedDiffusivity ] < 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerHinderedDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialHinderedDiffusivity ] > 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::UpperHinderedDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "initial hindered diffusivity must be between "
                           "lower and upper bounds" );

      }

      // DeltaHinderedDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::DeltaHinderedDiffusivity ] >= 
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::UpperHinderedDiffusivity ] -
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerHinderedDiffusivity ] ) )
      {

        throw std::runtime_error( 
                           "delta hindered diffusivity must be lower than "
                           "upper bound minus lower bound" );

      }


      // check(s) for intracellular fraction ///////////////////////////////////
      // LowerIntracellularFraction
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerIntracellularFraction ]
                                                                      < 0.0 )
      {

        throw std::runtime_error(
                            "lower intracellular fraction must be positive" );

      }
      // UpperIntracellularFraction
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperIntracellularFraction ] 
                                                                      > 1.0 )
      {

        throw std::runtime_error(
               "upper intracellular fraction must be lower or equal to 1.0" );

      }
      // LowerIntracellularFraction < UpperIntracellularFraction
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerIntracellularFraction ] > 
           specificScalarParameters[
             gkg::AxCaliberOdfCartesianField::UpperIntracellularFraction ] )
      {

        throw std::runtime_error( "lower intracellular fraction is greater "
                                  "than upper intracellular fraction" );

      }
      // InitialIntracellularFraction
      if ( ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::InitialIntracellularFraction ] < 
             specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerIntracellularFraction ] ) ||
           ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::InitialIntracellularFraction ] > 
             specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperIntracellularFraction ] ) )
      {

        throw std::runtime_error( 
             "initial intracellular fraction must be between lower and upper "
             "bounds" );

      }
      // DeltaIntracellularFraction
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::DeltaIntracellularFraction ] >= 
           ( specificScalarParameters[ 
              gkg::AxCaliberOdfCartesianField::UpperIntracellularFraction ] -
             specificScalarParameters[ 
              gkg::AxCaliberOdfCartesianField::LowerIntracellularFraction ] ) )
      {

        throw std::runtime_error( 
          "delta intracellular fraction must be lower than upper bound minus "
          "lower bound" );
        
      }


      // check(s) for intracellular perpendicular diffusivity //////////////////
      // LowerIntracellularPerpendicularDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                            LowerIntracellularPerpendicularDiffusivity ] < 0.0 )
      {

        throw std::runtime_error(
                   "intracellular perpendicular diffusivity must be positive" );

      }
      
      // LowerIntracellularPerpendicularDiffusivity < 
      //                              UpperIntracellularPerpendicularDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                  LowerIntracellularPerpendicularDiffusivity ] >
           specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                  UpperIntracellularPerpendicularDiffusivity ] )
      {

        throw std::runtime_error(
                "lower intracellular perpendicular diffusivity is greater than "
                "upper intracellular perpendicular diffusivity" );

      }

      // InitialIntracellularPerpendicularDiffusivity
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               InitialIntracellularPerpendicularDiffusivity ] < 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               LowerIntracellularPerpendicularDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               InitialIntracellularPerpendicularDiffusivity ] > 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               UpperIntracellularPerpendicularDiffusivity ] ) )
      {

        throw std::runtime_error( 
              "initial intracellular perpendicular diffusivity must be between "
              "lower and upper bounds" );

      }

      // DeltaIntracellularPerpendicularDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                DeltaIntracellularPerpendicularDiffusivity ] >= 
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                UpperIntracellularPerpendicularDiffusivity ] -
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                LowerIntracellularPerpendicularDiffusivity ] ) )
      {

        throw std::runtime_error( 
             "delta intracellular perpendicular diffusivity must be lower than "
             "upper bound minus lower bound" );

      }



      // check(s) for intracellular parallel diffusivity ///////////////////////
      // LowerIntracellularParallelDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                 LowerIntracellularParallelDiffusivity ] < 0.0 )
      {

        throw std::runtime_error(
          "intracellular parallel diffusivity must be positive" );

      }
      
      // LowerIntracellularParallelDiffusivity < 
      // UpperIntracellularParallelDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                  LowerIntracellularParallelDiffusivity ] >
           specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                  UpperIntracellularParallelDiffusivity ] )
      {

        throw std::runtime_error(
                "lower intracellular parallel diffusivity is greater than "
                "upper intracellular parallel diffusivity" );

      }

      // InitialIntracellularParallelDiffusivity
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               InitialIntracellularParallelDiffusivity ] < 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               LowerIntracellularParallelDiffusivity ] ) ||
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               InitialIntracellularParallelDiffusivity ] > 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                               UpperIntracellularParallelDiffusivity ] ) )
      {

        throw std::runtime_error( 
              "initial intracellular parallel diffusivity must be between "
              "lower and upper bounds" );

      }

      // DeltaIntracellularParallelDiffusivity
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::
                                DeltaIntracellularParallelDiffusivity ] >= 
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                UpperIntracellularParallelDiffusivity ] -
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                LowerIntracellularParallelDiffusivity ] ) )
      {

        throw std::runtime_error( 
             "delta intracellular parallel diffusivity must be lower than "
             "upper bound minus lower bound" );

      }

    
      // check(s) for diameter mean  ///////////////////////////////////////////
      // LowerDiameterMean
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerDiameterMean ] < 0.0 )
      {

        throw std::runtime_error( "lower diameter mean must be positive" );

      }

      // UpperDiameterMean
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperDiameterMean ] > 20.0e-6 )
      {

        throw std::runtime_error(
                       "upper diameter mean must be lower or equal to 20.0um" );

      }

      // LowerDiameterMean < UpperDiameterMean
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerDiameterMean ] > 
           specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperDiameterMean ] )
      {

        throw std::runtime_error( "lower diameter mean is greater "
                                  "than upper diameter mean" );

      }
      // InitialDiameterMean
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialDiameterMean ] < 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerDiameterMean ] ) ||
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialDiameterMean ] > 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::UpperDiameterMean ] ) )
      {

        throw std::runtime_error( 
               "initial diameter mean must be between lower and upper bounds" );

      }
      // DeltaDiameterMean
      if ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::DeltaDiameterMean ] >= 
           ( specificScalarParameters[
               gkg::AxCaliberOdfCartesianField::UpperDiameterMean ] -
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerDiameterMean ] ) )
      {

        throw std::runtime_error( 
          "delta diameter mean must be lower than upper bound minus "
          "lower bound" );
      }


      // check(s) for diameter stddev  /////////////////////////////////////////
      // LowerDiameterStdDev
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerDiameterStdDev ] < 0.0 )
      {

        throw std::runtime_error( "lower diameter stddev must be positive" );

      }

      // UpperDiameterStdDev
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperDiameterStdDev ] > 10.0e-6 )
      {

        throw std::runtime_error(
                     "upper diameter stddev must be lower or equal to 10.0um" );

      }

      // LowerDiameterStdDev < UpperDiameterStdDev
      if ( specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::LowerDiameterStdDev ] > 
           specificScalarParameters[ 
             gkg::AxCaliberOdfCartesianField::UpperDiameterStdDev ] )
      {

        throw std::runtime_error( "lower diameter stddev is greater "
                                  "than upper diameter stddev" );

      }
      // InitialDiameterStdDev
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialDiameterStdDev ] < 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerDiameterStdDev ] ) ||
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::InitialDiameterStdDev ] > 
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::UpperDiameterStdDev ] ) )
      {

        throw std::runtime_error( 
             "initial diameter stddev must be between lower and upper bounds" );

      }
      // DeltaDiameterStdDev
      if ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::DeltaDiameterStdDev ] >= 
           ( specificScalarParameters[
               gkg::AxCaliberOdfCartesianField::UpperDiameterStdDev ] -
             specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::LowerDiameterStdDev ] ) )
      {

        throw std::runtime_error( 
          "delta diameter stddev must be lower than upper bound minus "
          "lower bound" );
      }



      // check(s) for fixed //////////////////////////////////////////////////
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedHinderedDiffusivity ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedHinderedDiffusivity ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                           "fixed isotropic diffusivity must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedIntracellularFraction ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedIntracellularFraction ] 
             != 1.0 ) )
      {

        throw std::runtime_error(
                          "fixed intracellular fraction must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                    FixedIntracellularPerpendicularDiffusivity ]
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                    FixedIntracellularPerpendicularDiffusivity ]
             != 1.0 ) )
      {

        throw std::runtime_error( 
         "fixed intracellular perpendicular diffusivity must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                         FixedIntracellularParallelDiffusivity ]
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::
                                         FixedIntracellularParallelDiffusivity ]
             != 1.0 ) )
      {

        throw std::runtime_error(
                            "fixed parallel diffusivity must be either 0 or 1");

      }
      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedDiameterMean ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedDiameterMean ] 
             != 1.0 ) )
      {

        throw std::runtime_error( "fixed diameter mean must be either 0 or 1");

      }

      if ( ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedDiameterStdDev ] 
             != 0.0 ) &&
           ( specificScalarParameters[ 
               gkg::AxCaliberOdfCartesianField::FixedDiameterStdDev ] 
             != 1.0 ) )
      {

        throw std::runtime_error( 
                                 "fixed diameter stddev must be either 0 or 1");

      }


      // check for Callaghan's model root count ////////////////////////////////
      if ( specificScalarParameters[
             gkg::AxCaliberOdfCartesianField::MaximumCallaghanModelRootCount ]
                                                                        <= 0.0 )
      {

        throw std::runtime_error(
             "maximum Callaghan's model root count must be strictly positive" );

      }


      // check for Callaghan's model function count ////////////////////////////
      if ( specificScalarParameters[
             gkg::AxCaliberOdfCartesianField::
                                            MaximumCallaghanModelFunctionCount ]
                                                                        <= 0.0 )
      {

        throw std::runtime_error(
                             "maximum Callaghan's model function count must be "
                             "strictly positive" );

      }


      // check for noise standard deviation //////////////////////////////////
      if ( specificScalarParameters[
             gkg::AxCaliberOdfCartesianField::NoiseStandardDeviation ] <= 0.0 )
      {

        throw std::runtime_error(
                       "noise standard deviation must be strictly positive" );

      }

      // check(s) for QuickSeach parameter ///////////////////////////////////
      if ( specificScalarParameters[
              gkg::AxCaliberOdfCartesianField::QuickSearchStepCount ] <= 0.0 )
      {

        throw std::runtime_error(
                         "QuickSearch step count must be strictly positive" );

      }

      // check(s) for NLP parameter //////////////////////////////////////////
      if ( specificScalarParameters[
            gkg::AxCaliberOdfCartesianField::NlpMaximumIterationCount ] <= 0.0 )
      {

        throw std::runtime_error(
                    "NLP maximum iteration count must be strictly positive" );

      }
      if ( specificScalarParameters[
          gkg::AxCaliberOdfCartesianField::NlpMaximumSizeOfTheSimplex ] <= 0.0 )
      {

        throw std::runtime_error(
                "NLP maximum size of the simplex must be strictly positive" );

      }

      // check(s) for MCMCM parameter ////////////////////////////////////////
      if ( specificScalarParameters[
              gkg::AxCaliberOdfCartesianField::McmcBurninCount ] <= 0.0 )
      {

        throw std::runtime_error(
                              "MCMC burnin count must be strictly positive" );

      }

      if ( specificScalarParameters[
              gkg::AxCaliberOdfCartesianField::McmcSampleCount ] <= 0.0 )
      {

        throw std::runtime_error(
                              "MCMC sample count must be strictly positive" );

      }

      if ( specificScalarParameters[
              gkg::AxCaliberOdfCartesianField::McmcIntervalCount ] <= 0.0 )
      {

        throw std::runtime_error(
                            "MCMC interval count must be strictly positive" );

      }

      if ( specificScalarParameters[
           gkg::AxCaliberOdfCartesianField::McmcMaximumIterationCount ] <= 0.0 )
      {

        throw std::runtime_error(
                   "MCMC maximum iteration count must be strictly positive" );

      }

    }

  }
  GKG_CATCH( "void gkg::AxCaliberOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::AxCaliberOdfCartesianField::initialize( 
              const std::vector< double >& specificScalarParameters,
              const std::vector< std::string >& /* specificStringParameters */ )
{

  try
  {

    // collecting initial parameter(s)
    _initialParameters.reallocate( 6 );
    _initialParameters( 0 ) = specificScalarParameters[
                                 InitialHinderedDiffusivity ];
    _initialParameters( 1 ) = specificScalarParameters[
                                 InitialIntracellularFraction ];
    _initialParameters( 2 ) = specificScalarParameters[
                                 InitialIntracellularPerpendicularDiffusivity ];
    _initialParameters( 3 ) = specificScalarParameters[
                                 InitialIntracellularParallelDiffusivity ];
    _initialParameters( 4 ) = specificScalarParameters[
                                 InitialDiameterMean ];
    _initialParameters( 5 ) = specificScalarParameters[
                                 InitialDiameterStdDev ];


    // collecting lower parameter(s)
    _lowerParameters.reallocate( 6 );
    _lowerParameters( 0 ) = specificScalarParameters[
                                 LowerHinderedDiffusivity ];
    _lowerParameters( 1 ) = specificScalarParameters[
                                 LowerIntracellularFraction ];
    _lowerParameters( 2 ) = specificScalarParameters[
                                 LowerIntracellularPerpendicularDiffusivity ];
    _lowerParameters( 3 ) = specificScalarParameters[
                                 LowerIntracellularParallelDiffusivity ];
    _lowerParameters( 4 ) = specificScalarParameters[
                                 LowerDiameterMean ];
    _lowerParameters( 5 ) = specificScalarParameters[
                                 LowerDiameterStdDev ];


    // collecting upper parameter(s)
    _upperParameters.reallocate( 6 );
    _upperParameters( 0 ) = specificScalarParameters[
                                 UpperHinderedDiffusivity ];
    _upperParameters( 1 ) = specificScalarParameters[
                                 UpperIntracellularFraction ];
    _upperParameters( 2 ) = specificScalarParameters[
                                 UpperIntracellularPerpendicularDiffusivity ];
    _upperParameters( 3 ) = specificScalarParameters[
                                 UpperIntracellularParallelDiffusivity ];
    _upperParameters( 4 ) = specificScalarParameters[
                                 UpperDiameterMean ];
    _upperParameters( 5 ) = specificScalarParameters[
                                 UpperDiameterStdDev ];


    // collecting delta parameter(s)
    _deltaParameters.reallocate( 6 );
    _deltaParameters( 0 ) = specificScalarParameters[
                                 DeltaHinderedDiffusivity ];
    _deltaParameters( 1 ) = specificScalarParameters[
                                 DeltaIntracellularFraction ];
    _deltaParameters( 2 ) = specificScalarParameters[
                                 DeltaIntracellularPerpendicularDiffusivity ];
    _deltaParameters( 3 ) = specificScalarParameters[
                                 DeltaIntracellularParallelDiffusivity ];
    _deltaParameters( 4 ) = specificScalarParameters[
                                 DeltaDiameterMean ];
    _deltaParameters( 5 ) = specificScalarParameters[
                                 DeltaDiameterStdDev ];


    // collecting fixed parameter(s)
    _fixed.resize( 6U );
    _fixed[ 0 ] = ( specificScalarParameters[
                    FixedHinderedDiffusivity ] > 0.0 ?
                    true : false );
    _fixed[ 1 ] = ( specificScalarParameters[
                    FixedIntracellularFraction ] > 0.0 ?
                    true : false );
    _fixed[ 2 ] = ( specificScalarParameters[
                    FixedIntracellularPerpendicularDiffusivity ] > 0.0 ?
                    true : false );
    _fixed[ 3 ] = ( specificScalarParameters[
                    FixedIntracellularParallelDiffusivity ] > 0.0 ?
                    true : false );
    _fixed[ 4 ] = ( specificScalarParameters[
                    FixedDiameterMean ] > 0.0 ?
                    true : false );
    _fixed[ 5 ] = ( specificScalarParameters[
                    FixedDiameterStdDev ] > 0.0 ?
                    true : false );
    

    // collecting maximum Callaghan's model root and function count(s)
    _maximumCallaghanModelRootCount = ( int32_t )( specificScalarParameters[
                                   MaximumCallaghanModelRootCount ] + 0.5 );
    _maximumCallaghanModelFunctionCount = ( int32_t )( specificScalarParameters[
                                   MaximumCallaghanModelFunctionCount ] + 0.5 );

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
  GKG_CATCH( "void gkg::AxCaliberOdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& "
             "/*specificStringParameters*/ )" );

}


void 
gkg::AxCaliberOdfCartesianField::prepareMCMCFunction( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "preparing MCMC function" << std::flush;

    }

    _axCaliberMCMCFunction.reset( new gkg::AxCaliberMCMCFunction(
                                  this->_dw,
                                  this->_outputOrientationSet,
                                  this->_noiseStandardDeviation,
                                  this->_maximumCallaghanModelRootCount,
                                  this->_maximumCallaghanModelFunctionCount ) );

    if ( verbose )
    {

      std::cout << gkg::Eraser( 23 ) << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::AxCaliberOdfCartesianField::prepareMCMCFunction( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::AxCaliberOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {
   
    gkg::Vector initialParameters( _initialParameters );

    gkg::RCPointer< gkg::AxCaliberMCMCParameterSet >
      axCaliberMCMCParameterSet( new gkg::AxCaliberMCMCParameterSet(
                                      this->_lowerParameters,
                                      this->_upperParameters,
                                      this->_fixed,
                                      this->_deltaParameters ) );

    gkg::AxCaliberOdf axCaliberOdf( &_outputOrientationSet,
                                    *_t2, *_dw,
                                    originSite,
                                    _axCaliberMCMCFunction,
                                    axCaliberMCMCParameterSet,
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
                              axCaliberOdf );

    }
    else
    {

      return axCaliberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AxCaliberOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::AxCaliberOdfCartesianField::newItem( const gkg::Vector3d< float >& originSite,
                                      const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    gkg::Vector initialParameters( _initialParameters );
    gkg::RCPointer< gkg::AxCaliberMCMCParameterSet >
      axCaliberMCMCParameterSet( new gkg::AxCaliberMCMCParameterSet(
                                      this->_lowerParameters,
                                      this->_upperParameters,
                                      this->_fixed,
                                      this->_deltaParameters ) );

    gkg::AxCaliberOdf axCaliberOdf( &_outputOrientationSet,
                                    *_t2, *_dw,
                                    originSite,
                                    _axCaliberMCMCFunction,
                                    axCaliberMCMCParameterSet,
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
                              axCaliberOdf );

    }
    else
    {

      return axCaliberOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AxCaliberOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
