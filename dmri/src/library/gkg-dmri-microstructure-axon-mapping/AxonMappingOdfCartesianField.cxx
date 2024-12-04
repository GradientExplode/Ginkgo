#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdfCartesianField.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEDefines.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEDefines.h>
#include <gkg-dmri-microstructure-axon-mapping/OptimizationDefines.h>
#include <gkg-dmri-microstructure-axon-mapping/AxonMappingOdf.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingOdfCartesianFieldFunctor.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingMCMCFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingMCMCFunction.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEAxonMappingMCMCParameterSet.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEAxonMappingMCMCParameterSet.h>
#include <gkg-dmri-microstructure-axon-mapping/CanonicalWatsonSphericalHarmonicsCache.h>
#include <gkg-dmri-microstructure-axon-mapping/PGSEIntracellularContributionCache.h>
#include <gkg-dmri-microstructure-axon-mapping/CTOGSEIntracellularContributionCache.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


////////////////////////////////////////////////////////////////////////////////
// Defines for parameter checking
////////////////////////////////////////////////////////////////////////////////

#define AXON_MAPPING_INFINITY 1e38

#define checkNonNegativeOrNullParameter( specificScalarParameters, TAG, \
                                         OFFSET ) \
if ( specificScalarParameters[ OFFSET + TAG ] <= 0.0 ) \
 { throw std::runtime_error( #TAG" must be strictly positive" ); }


#define checkParameter( specificScalarParameters, \
                        INITIAL_TAG, \
                        LOWER_TAG, \
                        UPPER_TAG, \
                        DELTA_TAG, \
                        FIXED_TAG, \
                        LOWER_VALUE, \
                        UPPER_VALUE ) \
if ( ( specificScalarParameters[ LOWER_TAG ] < LOWER_VALUE ) ||\
      ( specificScalarParameters[ LOWER_TAG ] > UPPER_VALUE ) ) \
 { throw std::runtime_error( \
     #LOWER_TAG" must be between "#LOWER_VALUE" and "#UPPER_VALUE ); } \
\
if ( ( specificScalarParameters[ UPPER_TAG ] < LOWER_VALUE ) ||\
      ( specificScalarParameters[ UPPER_TAG ] > UPPER_VALUE ) ) \
 { throw std::runtime_error( \
     #UPPER_TAG" must be between "#LOWER_VALUE" and "#UPPER_VALUE ); } \
\
 if ( specificScalarParameters[ UPPER_TAG ] < \
      specificScalarParameters[ LOWER_TAG ] ) \
 { throw std::runtime_error( \
     #UPPER_TAG" must be greater than "#LOWER_TAG ); } \
\
 if ( ( specificScalarParameters[ INITIAL_TAG ] < \
        specificScalarParameters[ LOWER_TAG ] ) ||\
      ( specificScalarParameters[ INITIAL_TAG ] > \
        specificScalarParameters[ UPPER_TAG ] ) ) \
 { throw std::runtime_error( \
     #INITIAL_TAG" must be between "#LOWER_TAG" and "#UPPER_TAG ); } \
\
 if ( specificScalarParameters[ DELTA_TAG ] >= \
      ( specificScalarParameters[ UPPER_TAG ] - \
        specificScalarParameters[ LOWER_TAG ] ) \
     ) \
 { throw std::runtime_error( \
     #DELTA_TAG" must be lower than ("#UPPER_TAG"-"#LOWER_TAG")" ); } \
\
 if ( ( specificScalarParameters[ FIXED_TAG ] != 0.0 ) && \
      ( specificScalarParameters[ FIXED_TAG ] != 1.0 ) ) \
 { throw std::runtime_error( #FIXED_TAG" must be either 0 or 1"); }



////////////////////////////////////////////////////////////////////////////////
// class AxonMappingOdfCartesianField
////////////////////////////////////////////////////////////////////////////////

gkg::AxonMappingOdfCartesianField::AxonMappingOdfCartesianField(
                     gkg::RCPointer< gkg::Volume< float > > t2,
                     gkg::RCPointer< gkg::Volume< float > > dw,
                     gkg::RCPointer< gkg::Volume< int16_t > > mask,
                     const gkg::OrientationSet* outputOrientationSet,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     bool verbose )
                                  : gkg::OdfCartesianField(
                                                          t2, dw, mask,
                                                          outputOrientationSet )
{

  try
  {

    // getting the axon mapping model
    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      _axonMappingModel = gkg::AxonMappingOdfCartesianField::PGSE;

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      _axonMappingModel =
                         gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE;

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
    _items.getHeader().addAttribute(
                                "odf_type",
                                gkg::TypeOf< gkg::AxonMappingOdf >::getName() );

    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      this->_functor = new gkg::PGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      this->_functor = new gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnGrid( gkg::BoundingBox< int32_t >( 0, _t2->getSizeX() - 1,
                                                    0, _t2->getSizeY() - 1,
                                                    0, _t2->getSizeZ() - 1 ),
                       verbose );

  }
  GKG_CATCH( "gkg::AxonMappingOdfCartesianField::"
             "AxonMappingOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::AxonMappingOdfCartesianField::AxonMappingOdfCartesianField(
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

    // getting the axon mapping model
    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      _axonMappingModel = gkg::AxonMappingOdfCartesianField::PGSE;

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      _axonMappingModel =
                         gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE;

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
                                gkg::TypeOf< gkg::AxonMappingOdf >::getName() );

    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      this->_functor = new gkg::PGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      this->_functor = new gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }

    // preparing MCMC function
    prepareMCMCFunction( verbose );

    // computing ODFs
    calculatingOnSubGrid( boundingBox, superResolution, verbose );

  }
  GKG_CATCH( "gkg::AxonMappingOdfCartesianField::"
             "AxonMappingOdfCartesianField( "
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


gkg::AxonMappingOdfCartesianField::AxonMappingOdfCartesianField(
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

    // getting the axon mapping model
    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      _axonMappingModel = gkg::AxonMappingOdfCartesianField::PGSE;

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      _axonMappingModel =
                         gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE;

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
                                gkg::TypeOf< gkg::AxonMappingOdf >::getName() );

    if ( specificStringParameters[ 0 ] == "pgse" )
    {

      this->_functor = new gkg::PGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }
    else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
    {

      this->_functor = new gkg::CTOGSEAxonMappingOdfCartesianFieldFunctor(
                                                                        *this );

    }

    // preparing MCMC function
    prepareMCMCFunction(  verbose );

    // computing ODFs
    calculatingOnGrid( boundingBox, verbose );

  }
  GKG_CATCH( "gkg::AxonMappingOdfCartesianField::"
             "AxonMappingOdfCartesianField( "
             "gkg::RCPointer< gkg::Volume< float > > t2, "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "gkg::RCPointer< gkg::Volume< int16_t > > mask, "
             "const gkg::BoundingBox< int32_t >& boundingBox, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "bool verbose )" );

}


gkg::AxonMappingOdfCartesianField::~AxonMappingOdfCartesianField()
{
}



gkg::AxonMappingOdfCartesianField::AxonMappingModel 
gkg::AxonMappingOdfCartesianField::getAxonMappingModel() const
{

  try
  {

    return _axonMappingModel;

  }
  GKG_CATCH( "gkg::AxonMappingOdfCartesianField::AxonMappingModel "
             "gkg::AxonMappingOdfCartesianField::getAxonMappingModel() "
             "const" );

}


const gkg::Vector& 
gkg::AxonMappingOdfCartesianField::getInitialParameters() const
{

  try
  {

    return _initialParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxonMappingOdfCartesianField::getInitialParameters() const" );

}


const gkg::Vector& 
gkg::AxonMappingOdfCartesianField::getLowerParameters() const
{

  try
  {

    return _lowerParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxonMappingOdfCartesianField::getLowerParameters() const" );

}


const gkg::Vector& 
gkg::AxonMappingOdfCartesianField::getUpperParameters() const
{

  try
  {

    return _upperParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxonMappingOdfCartesianField::getUpperParameters() const" );

}


const gkg::Vector& 
gkg::AxonMappingOdfCartesianField::getDeltaParameters() const
{

  try
  {

    return _deltaParameters;

  }
  GKG_CATCH( "const gkg::Vector& "
             "gkg::AxonMappingOdfCartesianField::getDeltaParameters() const" );

}


const std::vector< bool >& 
gkg::AxonMappingOdfCartesianField::getFixed() const
{

  try
  {

    return _fixed;

  }
  GKG_CATCH( "const std::vector< bool >& "
             "gkg::AxonMappingOdfCartesianField::getFixed() const" );

}


double gkg::AxonMappingOdfCartesianField::getNoiseStandardDeviation() const
{

  try
  {

    return _noiseStandardDeviation;

  }
  GKG_CATCH( "double "
             "gkg::AxonMappingOdfCartesianField::getNoiseStandardDeviation() "
             "const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getQuickSearchStepCount() const
{

  try
  {

    return _quicksearchStepCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::getQuickSearchStepCount() "
             "const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getNLPMaximumIterationCount() const
{

  try
  {

    return _nlpMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::getNLPMaximumIterationCount() "
             "const" );

}


double gkg::AxonMappingOdfCartesianField::getNLPMaximumSizeOfTheSimplex() const
{

  try
  {

    return _nlpMaximumSizeOfTheSimplex;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::"
             "getNLPMaximumSizeOfTheSimplex() "
             "const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getMCMCBurninCount() const
{

  try
  {

    return _mcmcBurninCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::getMCMCBurninCount() const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getMCMCSampleCount() const
{

  try
  {

    return _mcmcSampleCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::getMCMCSampleCount() const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getMCMCIntervalCount() const
{

  try
  {

    return _mcmcIntervalCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::"
             "getMCMCIntervalCount() const" );

}


int32_t gkg::AxonMappingOdfCartesianField::getMCMCMaximumIterationCount() const
{

  try
  {

    return _mcmcMaximumIterationCount;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxonMappingOdfCartesianField::"
             "getMCMCMaximumIterationCount() const" );

}


void gkg::AxonMappingOdfCartesianField::checkOrInitializeDefaultParameters(
                         std::vector< double >& specificScalarParameters,
                         std::vector< std::string >& specificStringParameters )
{

  try
  {

    if ( specificStringParameters.empty() )
    {

      specificStringParameters.resize( 1 );
      specificStringParameters[ 0 ] = "pgse";

    }
    else
    {

      if ( specificStringParameters.size() != 1U )
      {

        throw std::runtime_error( "only one string parameter required" );

      }

      if ( ( specificStringParameters[ 0 ] != "pgse" ) &&
           ( specificStringParameters[ 0 ] != "cosine-trapezoid-ogse" ) )
      {

        throw std::runtime_error( 
         "axon mapping model type must be one of pgse/cosine-trapezoid-ogse" );

      }

    }

    if ( specificScalarParameters.empty() )
    {

      if ( specificStringParameters[ 0 ] == "pgse" )
      {

        double pgseScalarParameters[] =
        {

          0.0, 3.0e-9, 0.0e-9, 0.0, 0.5,   0.0, 1.7e-9,  2.0e-6,
          0.0, 0.0e-9, 0.0e-9, 0.0, 0.0,   0.0, 0.0e-9,  0.0e-6,
          1.0, 6.0e-9, 3.0e-9, 1.0, 1.0, 200.0, 3.0e-9, 10.0e-6,
          0.1, 0.5e-9, 0.5e-9, 0.1, 0.1,   2.0, 0.5e-9,  1.0e-6,
          0.0,    0.0,    0.0, 0.0, 0.0,   0.0,    0.0,     0.0, 1.0,
          0.01,
          1.0, 10.0,
          1.0, 1000.0, 0.01,
          1.0, 5000.0, 500.0, 100.0, 100000.0

        };
        specificScalarParameters.assign( pgseScalarParameters,
                                         pgseScalarParameters +
                                         PGSE_End +
                                         OptimizationParameters_End );

      }
      else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
      {

        double ctogseScalarParameters[] =
        {

          0.0, 3.0e-9, 0.0e-9, 0.0, 0.5,   0.0, 1.7e-9,  2.0e-6,
          0.0, 0.0e-9, 0.0e-9, 0.0, 0.0,   0.0, 0.0e-9,  0.0e-6,
          1.0, 6.0e-9, 3.0e-9, 1.0, 1.0, 200.0, 3.0e-9, 10.0e-6,
          0.1, 0.5e-9, 0.5e-9, 0.1, 0.1,   2.0, 0.5e-9,  1.0e-6,
          0.0,    0.0,    0.0, 0.0, 0.0,   0.0,    0.0,     0.0, 1.0,
          0.01,
          1.0, 10.0,
          1.0, 1000.0, 0.01,
          1.0, 5000.0, 500.0, 100.0, 100000.0

        };
        specificScalarParameters.assign( ctogseScalarParameters,
                                         ctogseScalarParameters +
                                         CTOGSE_End +
                                         OptimizationParameters_End );

      }

    }
    else
    {

      int32_t offsetToOptimizationParameters = 0;

      if ( specificStringParameters[ 0 ] == "pgse" )
      {

        // check for scalar vector size ////////////////////////////////////////
        if ( specificScalarParameters.size() != 
                                    ( PGSE_End + OptimizationParameters_End ) )
        {

          throw std::runtime_error( "invalid scalar parameter count" );

        }

        checkParameter(
          specificScalarParameters,
          PGSE_InitialIsotropicFraction,
          PGSE_LowerIsotropicFraction,
          PGSE_UpperIsotropicFraction,
          PGSE_DeltaIsotropicFraction,
          PGSE_FixedIsotropicFraction,
          0.0, 1.0 );

        checkParameter(
          specificScalarParameters,
          PGSE_InitialIsotropicDiffusivity,
          PGSE_LowerIsotropicDiffusivity,
          PGSE_UpperIsotropicDiffusivity,
          PGSE_DeltaIsotropicDiffusivity,
          PGSE_FixedIsotropicDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          PGSE_InitialExtraPerpendicularDiffusivity,
          PGSE_LowerExtraPerpendicularDiffusivity,
          PGSE_UpperExtraPerpendicularDiffusivity,
          PGSE_DeltaExtraPerpendicularDiffusivity,
          PGSE_FixedExtraPerpendicularDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

         checkParameter(
          specificScalarParameters,
          PGSE_InitialStationaryFraction,
          PGSE_LowerStationaryFraction,
          PGSE_UpperStationaryFraction,
          PGSE_DeltaStationaryFraction,
          PGSE_FixedStationaryFraction,
          0.0, 1.0 );

         checkParameter(
          specificScalarParameters,
          PGSE_InitialIntracellularFraction,
          PGSE_LowerIntracellularFraction,
          PGSE_UpperIntracellularFraction,
          PGSE_DeltaIntracellularFraction,
          PGSE_FixedIntracellularFraction,
          0.0, 1.0 );

         checkParameter(
          specificScalarParameters,
          PGSE_InitialKappa,
          PGSE_LowerKappa,
          PGSE_UpperKappa,
          PGSE_DeltaKappa,
          PGSE_FixedKappa,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          PGSE_InitialParallelDiffusivity,
          PGSE_LowerParallelDiffusivity,
          PGSE_UpperParallelDiffusivity,
          PGSE_DeltaParallelDiffusivity,
          PGSE_FixedParallelDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          PGSE_InitialAxonDiameter,
          PGSE_LowerAxonDiameter,
          PGSE_UpperAxonDiameter,
          PGSE_DeltaAxonDiameter,
          PGSE_FixedAxonDiameter,
          0.0, AXON_MAPPING_INFINITY );


        // check for combined isotropic & intracellular ////////////////////////
        if ( ( specificScalarParameters[ PGSE_InitialIsotropicFraction ] +
               specificScalarParameters[ PGSE_InitialIntracellularFraction ] +
               specificScalarParameters[ PGSE_InitialStationaryFraction ] ) 
             > 1.0 )
        {

          throw std::runtime_error( 
              "the sum of isotropic, intracellular and stationary fractions is "
              "greater than 1.0" );

        }

        offsetToOptimizationParameters = PGSE_End;
        
      }
      else if ( specificStringParameters[ 0 ] == "cosine-trapezoid-ogse" )
      {

        // check for scalar vector size ////////////////////////////////////////
        if ( specificScalarParameters.size() != 
                                    ( CTOGSE_End + OptimizationParameters_End ) )
        {

          throw std::runtime_error( "invalid scalar parameter count" );

        }

        checkParameter(
          specificScalarParameters,
          CTOGSE_InitialIsotropicFraction,
          CTOGSE_LowerIsotropicFraction,
          CTOGSE_UpperIsotropicFraction,
          CTOGSE_DeltaIsotropicFraction,
          CTOGSE_FixedIsotropicFraction,
          0.0, 1.0 );

        checkParameter(
          specificScalarParameters,
          CTOGSE_InitialIsotropicDiffusivity,
          CTOGSE_LowerIsotropicDiffusivity,
          CTOGSE_UpperIsotropicDiffusivity,
          CTOGSE_DeltaIsotropicDiffusivity,
          CTOGSE_FixedIsotropicDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          CTOGSE_InitialExtraPerpendicularDiffusivity,
          CTOGSE_LowerExtraPerpendicularDiffusivity,
          CTOGSE_UpperExtraPerpendicularDiffusivity,
          CTOGSE_DeltaExtraPerpendicularDiffusivity,
          CTOGSE_FixedExtraPerpendicularDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

         checkParameter(
          specificScalarParameters,
          CTOGSE_InitialStationaryFraction,
          CTOGSE_LowerStationaryFraction,
          CTOGSE_UpperStationaryFraction,
          CTOGSE_DeltaStationaryFraction,
          CTOGSE_FixedStationaryFraction,
          0.0, 1.0 );

         checkParameter(
          specificScalarParameters,
          CTOGSE_InitialIntracellularFraction,
          CTOGSE_LowerIntracellularFraction,
          CTOGSE_UpperIntracellularFraction,
          CTOGSE_DeltaIntracellularFraction,
          CTOGSE_FixedIntracellularFraction,
          0.0, 1.0 );

         checkParameter(
          specificScalarParameters,
          CTOGSE_InitialKappa,
          CTOGSE_LowerKappa,
          CTOGSE_UpperKappa,
          CTOGSE_DeltaKappa,
          CTOGSE_FixedKappa,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          CTOGSE_InitialParallelDiffusivity,
          CTOGSE_LowerParallelDiffusivity,
          CTOGSE_UpperParallelDiffusivity,
          CTOGSE_DeltaParallelDiffusivity,
          CTOGSE_FixedParallelDiffusivity,
          0.0, AXON_MAPPING_INFINITY );

        checkParameter(
          specificScalarParameters,
          CTOGSE_InitialAxonDiameter,
          CTOGSE_LowerAxonDiameter,
          CTOGSE_UpperAxonDiameter,
          CTOGSE_DeltaAxonDiameter,
          CTOGSE_FixedAxonDiameter,
          0.0, AXON_MAPPING_INFINITY );


        // check for combined isotropic & intracellular ////////////////////////
        if ( ( specificScalarParameters[ CTOGSE_InitialIsotropicFraction ] +
               specificScalarParameters[ CTOGSE_InitialIntracellularFraction ] +
               specificScalarParameters[ CTOGSE_InitialStationaryFraction ] ) 
             > 1.0 )
        {

          throw std::runtime_error( 
              "the sum of isotropic, intracellular and stationary fractions is "
              "greater than 1.0" );

        }

        offsetToOptimizationParameters = CTOGSE_End;
        
      }


      // check for noise standard deviation //////////////////////////////////
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        NoiseStandardDeviation,
        offsetToOptimizationParameters );

      // check(s) for QuickSeach parameter ///////////////////////////////////
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        QuickSearchStepCount,
        offsetToOptimizationParameters );

      // check(s) for NLP parameter //////////////////////////////////////////
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        NlpMaximumIterationCount,
        offsetToOptimizationParameters );
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        NlpMaximumSizeOfTheSimplex,
        offsetToOptimizationParameters );

      // check(s) for MCMCM parameter ////////////////////////////////////////
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        McmcBurninCount,
        offsetToOptimizationParameters );
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        McmcSampleCount,
        offsetToOptimizationParameters );
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        McmcIntervalCount,
        offsetToOptimizationParameters );
      checkNonNegativeOrNullParameter(
        specificScalarParameters,
        McmcMaximumIterationCount,
        offsetToOptimizationParameters );


    }

  }
  GKG_CATCH( "void gkg::AxonMappingOdfCartesianField::"
             "checkOrInitializeDefaultParameters( "
             "std::vector< double >& specificScalarParameters, "
             "std::vector< std::string >& specificStringParameters )" );

}


void gkg::AxonMappingOdfCartesianField::initialize( 
              const std::vector< double >& specificScalarParameters,
              const std::vector< std::string >& /* specificStringParameters */ )
{

  try
  {

    // collecting parameter(s) to compute Axon Mapping model
    int32_t offsetToOptimizationParameters = 0;
    if ( _axonMappingModel ==
         gkg::AxonMappingOdfCartesianField::PGSE )
    {

      // collecting initial parameter(s)
      _initialParameters.reallocate( 11 );
      _initialParameters( 0 ) =
        specificScalarParameters[ PGSE_InitialIsotropicFraction ];
      _initialParameters( 1 ) =
        specificScalarParameters[ PGSE_InitialIsotropicDiffusivity ];
      _initialParameters( 2 ) =
        specificScalarParameters[ PGSE_InitialExtraPerpendicularDiffusivity ];
      _initialParameters( 3 ) =
        specificScalarParameters[ PGSE_InitialStationaryFraction ];
      _initialParameters( 4 ) =
        specificScalarParameters[ PGSE_InitialIntracellularFraction ];
      _initialParameters( 5 ) =
        specificScalarParameters[ PGSE_InitialKappa ];
      _initialParameters( 6 ) =
        specificScalarParameters[ PGSE_InitialParallelDiffusivity ];
      _initialParameters( 7 ) =
        specificScalarParameters[ PGSE_InitialAxonDiameter ];
      _initialParameters( 8 ) = 0.0; // will be set to firt eigenvector of DTI
      _initialParameters( 9 ) = 0.0;
      _initialParameters( 10 ) = 0.0;

      // collecting lower parameter(s)
      _lowerParameters.reallocate( 11 );
      _lowerParameters( 0 ) =
        specificScalarParameters[ PGSE_LowerIsotropicFraction ];
      _lowerParameters( 1 ) =
        specificScalarParameters[ PGSE_LowerIsotropicDiffusivity ];
      _lowerParameters( 2 ) =
        specificScalarParameters[ PGSE_LowerExtraPerpendicularDiffusivity ];
      _lowerParameters( 3 ) =
        specificScalarParameters[ PGSE_LowerStationaryFraction ];
      _lowerParameters( 4 ) =
        specificScalarParameters[ PGSE_LowerIntracellularFraction ];
      _lowerParameters( 5 ) =
        specificScalarParameters[ PGSE_LowerKappa ];
      _lowerParameters( 6 ) =
        specificScalarParameters[ PGSE_LowerParallelDiffusivity ];
      _lowerParameters( 7 ) =
        specificScalarParameters[ PGSE_LowerAxonDiameter ];
      _lowerParameters( 8 ) = -1.0;
      _lowerParameters( 9 ) = -1.0;
      _lowerParameters( 10 ) = -1.0;

      // collecting upper parameter(s)
      _upperParameters.reallocate( 11 );
      _upperParameters( 0 ) =
        specificScalarParameters[ PGSE_UpperIsotropicFraction ];
      _upperParameters( 1 ) =
        specificScalarParameters[ PGSE_UpperIsotropicDiffusivity ];
      _upperParameters( 2 ) =
        specificScalarParameters[ PGSE_UpperExtraPerpendicularDiffusivity ];
      _upperParameters( 3 ) =
        specificScalarParameters[ PGSE_UpperStationaryFraction ];
      _upperParameters( 4 ) =
        specificScalarParameters[ PGSE_UpperIntracellularFraction ];
      _upperParameters( 5 ) =
        specificScalarParameters[ PGSE_UpperKappa ];
      _upperParameters( 6 ) =
        specificScalarParameters[ PGSE_UpperParallelDiffusivity ];
      _upperParameters( 7 ) =
        specificScalarParameters[ PGSE_UpperAxonDiameter ];
      _upperParameters( 8 ) = +1.0;
      _upperParameters( 9 ) = +1.0;
      _upperParameters( 10 ) = +1.0;

      // collecting delta parameter(s)
      _deltaParameters.reallocate( 11 );
      _deltaParameters( 0 ) =
        specificScalarParameters[ PGSE_DeltaIsotropicFraction ];
      _deltaParameters( 1 ) =
        specificScalarParameters[ PGSE_DeltaIsotropicDiffusivity ];
      _deltaParameters( 2 ) =
        specificScalarParameters[ PGSE_DeltaExtraPerpendicularDiffusivity ];
      _deltaParameters( 3 ) =
        specificScalarParameters[ PGSE_DeltaStationaryFraction ];
      _deltaParameters( 4 ) =
        specificScalarParameters[ PGSE_DeltaIntracellularFraction ];
      _deltaParameters( 5 ) =
        specificScalarParameters[ PGSE_DeltaKappa ];
      _deltaParameters( 6 ) =
        specificScalarParameters[ PGSE_DeltaParallelDiffusivity ];
      _deltaParameters( 7 ) =
        specificScalarParameters[ PGSE_DeltaAxonDiameter ];
      _deltaParameters( 8 ) = 0.02;
      _deltaParameters( 9 ) = 0.02;
      _deltaParameters( 10 ) = 0.02;

      // collecting fixed parameter(s)
      _fixed.resize( 11U );
      _fixed[ 0 ] =
        ( specificScalarParameters[ PGSE_FixedIsotropicFraction ] > 0.0 ?
          true : false );
      _fixed[ 1 ] =
        ( specificScalarParameters[ PGSE_FixedIsotropicDiffusivity ] > 0.0 ?
          true : false );
      _fixed[ 2 ] =
        ( specificScalarParameters[ PGSE_FixedExtraPerpendicularDiffusivity ] 
          > 0.0 ? true : false );
      _fixed[ 3 ] =
        ( specificScalarParameters[ PGSE_FixedStationaryFraction ] > 0.0 ?
          true : false );
      _fixed[ 4 ] =
        ( specificScalarParameters[ PGSE_FixedIntracellularFraction ] > 0.0 ?
          true : false );
      _fixed[ 5 ] =
        ( specificScalarParameters[ PGSE_FixedKappa ] > 0.0 ?
          true : false );
      _fixed[ 6 ] =
        ( specificScalarParameters[ PGSE_FixedParallelDiffusivity ] > 0.0 ?
          true : false );
      _fixed[ 7 ] =
        ( specificScalarParameters[ PGSE_FixedAxonDiameter ] > 0.0 ?
          true : false );
      _fixed[ 8 ] = 
        ( specificScalarParameters[ PGSE_FixedPrincipalOrientation ] > 0.0 ?
          true : false );
      _fixed[ 9 ] = _fixed[ 8 ];
      _fixed[ 10 ] = _fixed[ 8 ];

      offsetToOptimizationParameters = PGSE_End;

    }
    else if ( _axonMappingModel ==
              gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE )
    {

      // collecting initial parameter(s)
      _initialParameters.reallocate( 11 );
      _initialParameters( 0 ) =
        specificScalarParameters[ CTOGSE_InitialIsotropicFraction ];
      _initialParameters( 1 ) =
        specificScalarParameters[ CTOGSE_InitialIsotropicDiffusivity ];
      _initialParameters( 2 ) =
        specificScalarParameters[ CTOGSE_InitialExtraPerpendicularDiffusivity ];
      _initialParameters( 3 ) =
        specificScalarParameters[ CTOGSE_InitialStationaryFraction ];
      _initialParameters( 4 ) =
        specificScalarParameters[ CTOGSE_InitialIntracellularFraction ];
      _initialParameters( 5 ) =
        specificScalarParameters[ CTOGSE_InitialKappa ];
      _initialParameters( 6 ) =
        specificScalarParameters[ CTOGSE_InitialParallelDiffusivity ];
      _initialParameters( 7 ) =
        specificScalarParameters[ CTOGSE_InitialAxonDiameter ];
      _initialParameters( 8 ) = 0.0; // will be set to firt eigenvector of DTI
      _initialParameters( 9 ) = 0.0;
      _initialParameters( 10 ) = 0.0;

      // collecting lower parameter(s)
      _lowerParameters.reallocate( 11 );
      _lowerParameters( 0 ) =
        specificScalarParameters[ CTOGSE_LowerIsotropicFraction ];
      _lowerParameters( 1 ) =
        specificScalarParameters[ CTOGSE_LowerIsotropicDiffusivity ];
      _lowerParameters( 2 ) =
        specificScalarParameters[ CTOGSE_LowerExtraPerpendicularDiffusivity ];
      _lowerParameters( 3 ) =
        specificScalarParameters[ CTOGSE_LowerStationaryFraction ];
      _lowerParameters( 4 ) =
        specificScalarParameters[ CTOGSE_LowerIntracellularFraction ];
      _lowerParameters( 5 ) =
        specificScalarParameters[ CTOGSE_LowerKappa ];
      _lowerParameters( 6 ) =
        specificScalarParameters[ CTOGSE_LowerParallelDiffusivity ];
      _lowerParameters( 7 ) =
        specificScalarParameters[ CTOGSE_LowerAxonDiameter ];
      _lowerParameters( 8 ) = -1.0;
      _lowerParameters( 9 ) = -1.0;
      _lowerParameters( 10 ) = -1.0;

      // collecting upper parameter(s)
      _upperParameters.reallocate( 11 );
      _upperParameters( 0 ) =
        specificScalarParameters[ CTOGSE_UpperIsotropicFraction ];
      _upperParameters( 1 ) =
        specificScalarParameters[ CTOGSE_UpperIsotropicDiffusivity ];
      _upperParameters( 2 ) =
        specificScalarParameters[ CTOGSE_UpperExtraPerpendicularDiffusivity ];
      _upperParameters( 3 ) =
        specificScalarParameters[ CTOGSE_UpperStationaryFraction ];
      _upperParameters( 4 ) =
        specificScalarParameters[ CTOGSE_UpperIntracellularFraction ];
      _upperParameters( 5 ) =
        specificScalarParameters[ CTOGSE_UpperKappa ];
      _upperParameters( 6 ) =
        specificScalarParameters[ CTOGSE_UpperParallelDiffusivity ];
      _upperParameters( 7 ) =
        specificScalarParameters[ CTOGSE_UpperAxonDiameter ];
      _upperParameters( 8 ) = +1.0;
      _upperParameters( 9 ) = +1.0;
      _upperParameters( 10 ) = +1.0;

      // collecting delta parameter(s)
      _deltaParameters.reallocate( 11 );
      _deltaParameters( 0 ) =
        specificScalarParameters[ CTOGSE_DeltaIsotropicFraction ];
      _deltaParameters( 1 ) =
        specificScalarParameters[ CTOGSE_DeltaIsotropicDiffusivity ];
      _deltaParameters( 2 ) =
        specificScalarParameters[ CTOGSE_DeltaExtraPerpendicularDiffusivity ];
      _deltaParameters( 3 ) =
        specificScalarParameters[ CTOGSE_DeltaStationaryFraction ];
      _deltaParameters( 4 ) =
        specificScalarParameters[ CTOGSE_DeltaIntracellularFraction ];
      _deltaParameters( 5 ) =
        specificScalarParameters[ CTOGSE_DeltaKappa ];
      _deltaParameters( 6 ) =
        specificScalarParameters[ CTOGSE_DeltaParallelDiffusivity ];
      _deltaParameters( 7 ) =
        specificScalarParameters[ CTOGSE_DeltaAxonDiameter ];
      _deltaParameters( 8 ) = 0.02;
      _deltaParameters( 9 ) = 0.02;
      _deltaParameters( 10 ) = 0.02;

      // collecting fixed parameter(s)
      _fixed.resize( 11U );
      _fixed[ 0 ] =
        ( specificScalarParameters[ CTOGSE_FixedIsotropicFraction ] > 0.0 ?
          true : false );
      _fixed[ 1 ] =
        ( specificScalarParameters[ CTOGSE_FixedIsotropicDiffusivity ] > 0.0 ?
          true : false );
      _fixed[ 2 ] =
        ( specificScalarParameters[ CTOGSE_FixedExtraPerpendicularDiffusivity ] 
          > 0.0 ? true : false );
      _fixed[ 3 ] =
        ( specificScalarParameters[ CTOGSE_FixedStationaryFraction ] > 0.0 ?
          true : false );
      _fixed[ 4 ] =
        ( specificScalarParameters[ CTOGSE_FixedIntracellularFraction ] > 0.0 ?
          true : false );
      _fixed[ 5 ] =
        ( specificScalarParameters[ CTOGSE_FixedKappa ] > 0.0 ?
          true : false );
      _fixed[ 6 ] =
        ( specificScalarParameters[ CTOGSE_FixedParallelDiffusivity ] > 0.0 ?
          true : false );
      _fixed[ 7 ] =
        ( specificScalarParameters[ CTOGSE_FixedAxonDiameter ] > 0.0 ?
          true : false );
      _fixed[ 8 ] = 
        ( specificScalarParameters[ CTOGSE_FixedPrincipalOrientation ] > 0.0 ?
          true : false );
      _fixed[ 9 ] = _fixed[ 8 ];
      _fixed[ 10 ] = _fixed[ 8 ];

      offsetToOptimizationParameters = CTOGSE_End;

    }

    // collecting noise standard deviation
    _noiseStandardDeviation =
      specificScalarParameters[
        offsetToOptimizationParameters + NoiseStandardDeviation ];

    // collecting QuickSearch parameters
    _applyQuickSearchOptimizer =
      ( specificScalarParameters[
          offsetToOptimizationParameters + ApplyQuickSearchOptimizer ] > 0.0 ?
        true : false );
    _quicksearchStepCount =
      ( int32_t )( specificScalarParameters[
         offsetToOptimizationParameters + QuickSearchStepCount ] + 0.5 );

    // collecting NLP parameters
    _applyNlpOptimizer =
      ( specificScalarParameters[
          offsetToOptimizationParameters + ApplyNlpOptimizer ] > 0.0 ?
          true : false );
    _nlpMaximumIterationCount =
      ( int32_t )( specificScalarParameters[
          offsetToOptimizationParameters + NlpMaximumIterationCount ] + 0.5 );
    _nlpMaximumSizeOfTheSimplex =
      specificScalarParameters[
        offsetToOptimizationParameters + NlpMaximumSizeOfTheSimplex ];

    // collecting MCMC parameters
    _applyMcmcOptimizer =
      ( specificScalarParameters[
          offsetToOptimizationParameters + ApplyMcmcOptimizer ] > 0.0 ?
          true : false );
    _mcmcBurninCount =
      ( int32_t )( specificScalarParameters[
          offsetToOptimizationParameters + McmcBurninCount ] + 0.5 );
    _mcmcSampleCount =
      ( int32_t )( specificScalarParameters[
          offsetToOptimizationParameters + McmcSampleCount ] + 0.5 );
    _mcmcIntervalCount =
      ( int32_t )( specificScalarParameters[
          offsetToOptimizationParameters + McmcIntervalCount ] + 0.5 );
    _mcmcMaximumIterationCount =
      ( int32_t )( specificScalarParameters[
         offsetToOptimizationParameters + McmcMaximumIterationCount ] + 0.5 );


  }
  GKG_CATCH( "void gkg::AxonMappingOdfCartesianField::initialize( "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters )" );

}


void 
gkg::AxonMappingOdfCartesianField::prepareMCMCFunction( bool verbose )
{

  try
  {

    if ( verbose )
    {

      std::cout << "preparing Monte-Carlo-Markov-Chain function" << std::flush;

    }

    if ( _axonMappingModel ==
         gkg::AxonMappingOdfCartesianField::PGSE )
    {

      _axonMappingMCMCFunction.reset( new gkg::PGSEAxonMappingMCMCFunction(
                                      this->_dw,
                                      this->_outputOrientationSet,
                                      this->_noiseStandardDeviation ) );

      // updating cache for canonical Watson's function spherical harmonics 
      // decomposition
      if ( verbose )
      {

        std::cout << " preparing canonical Watson cache" << std::flush;

      }
      gkg::CanonicalWatsonSphericalHarmonicsCache::getInstance().update(
                                  this->_outputOrientationSet.getOrientations(),
                                  12,
                                  this->_lowerParameters( PGSE_Kappa ),
                                  this->_upperParameters( PGSE_Kappa ) );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 33 ) << std::flush;

      }

      // updating cache for canonical Watson' function spherical harmonics 
      // decomposition
      if ( verbose )
      {

        std::cout << " preparing intracellular cache" << std::flush;

      }
      gkg::PGSEIntracellularContributionCache::getInstance().update(
                         _axonMappingMCMCFunction->getGradientCharacteristics(),
                         // Neuman maximum sum index ///////////////////////////
                         20,
                         // kappa //////////////////////////////////////////////
                         this->_initialParameters( PGSE_Kappa ),
                         this->_lowerParameters( PGSE_Kappa ),
                         this->_upperParameters( PGSE_Kappa ),
                         50,
                         this->_fixed[ PGSE_Kappa ],
                         // dot product bin count //////////////////////////////
                         100,
                         // parallel diffusivity ///////////////////////////////
                         this->_initialParameters( PGSE_ParallelDiffusivity ),
                         this->_lowerParameters( PGSE_ParallelDiffusivity ),
                         this->_upperParameters( PGSE_ParallelDiffusivity ),
                         100,
                         this->_fixed[ PGSE_ParallelDiffusivity ],
                         // axon diameter //////////////////////////////////////
                         this->_initialParameters( PGSE_AxonDiameter ),
                         this->_lowerParameters( PGSE_AxonDiameter ),
                         this->_upperParameters( PGSE_AxonDiameter ),
                         50,
                         this->_fixed[ PGSE_AxonDiameter ] );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 30 ) << std::flush;

      }

    }
    else if ( _axonMappingModel ==
              gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE )
    {

      _axonMappingMCMCFunction.reset( new gkg::CTOGSEAxonMappingMCMCFunction(
                                      this->_dw,
                                      this->_outputOrientationSet,
                                      this->_noiseStandardDeviation ) );

      // updating cache for canonical Watson's function spherical harmonics 
      // decomposition
      if ( verbose )
      {

        std::cout << " preparing canonical Watson cache" << std::flush;

      }
      gkg::CanonicalWatsonSphericalHarmonicsCache::getInstance().update(
                                  this->_outputOrientationSet.getOrientations(),
                                  12,
                                  this->_lowerParameters( CTOGSE_Kappa ),
                                  this->_upperParameters( CTOGSE_Kappa ) );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 33 ) << std::flush;

      }

      // updating cache for canonical Watson' function spherical harmonics 
      // decomposition
      if ( verbose )
      {

        std::cout << " preparing intracellular cache" << std::flush;

      }
      gkg::CTOGSEIntracellularContributionCache::getInstance().update(
                         _axonMappingMCMCFunction->getGradientCharacteristics(),
                         // GPD maximum sum index //////////////////////////////
                         20,
                         // kappa //////////////////////////////////////////////
                         this->_initialParameters( CTOGSE_Kappa ),
                         this->_lowerParameters( CTOGSE_Kappa ),
                         this->_upperParameters( CTOGSE_Kappa ),
                         50,
                         this->_fixed[ CTOGSE_Kappa ],
                         // dot product bin count //////////////////////////////
                         100,
                         // parallel diffusivity ///////////////////////////////
                         this->_initialParameters( CTOGSE_ParallelDiffusivity ),
                         this->_lowerParameters( CTOGSE_ParallelDiffusivity ),
                         this->_upperParameters( CTOGSE_ParallelDiffusivity ),
                         100,
                         this->_fixed[ CTOGSE_ParallelDiffusivity ],
                         // axon diameter //////////////////////////////////////
                         this->_initialParameters( CTOGSE_AxonDiameter ),
                         this->_lowerParameters( CTOGSE_AxonDiameter ),
                         this->_upperParameters( CTOGSE_AxonDiameter ),
                         50,
                         this->_fixed[ CTOGSE_AxonDiameter ] );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 30 ) << std::flush;

      }

    }

    if ( verbose )
    {

      std::cout << gkg::Eraser( 43 ) << std::flush;

    }

  }
  GKG_CATCH( "void "
             "gkg::AxonMappingOdfCartesianField::prepareMCMCFunction( "
             "bool verbose )" );

}


gkg::OrientationDistributionFunction 
gkg::AxonMappingOdfCartesianField::newItem(
                                     const gkg::Vector3d< int32_t >& originSite,
                                     const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    // allocating the axon mapping MCMC parameter set   
    gkg::Vector initialParameters( _initialParameters );
    gkg::RCPointer< gkg::AxonMappingMCMCParameterSet >
      axonMappingMCMCParameterSet; 

    if ( _axonMappingModel ==
         gkg::AxonMappingOdfCartesianField::PGSE )
    {

      axonMappingMCMCParameterSet.reset( 
                                    new gkg::PGSEAxonMappingMCMCParameterSet(
                                                     this->_lowerParameters,
                                                     this->_upperParameters,
                                                     this->_fixed,
                                                     this->_deltaParameters ) );

    }
    else if ( _axonMappingModel ==
              gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE )
    {

      axonMappingMCMCParameterSet.reset( 
                                    new gkg::CTOGSEAxonMappingMCMCParameterSet(
                                                     this->_lowerParameters,
                                                     this->_upperParameters,
                                                     this->_fixed,
                                                     this->_deltaParameters ) );

    }

    // setting the principal orientation to the first eigenvector of the 
    // diffusion tensor model
    const Vector3d< float >& principalOrientation =
                  _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                       gkg::Tensor::Maximum );
    initialParameters( 8 ) = principalOrientation.x;
    initialParameters( 9 ) = principalOrientation.y;
    initialParameters( 10 ) = principalOrientation.z;

    // creating the axon mapping ODF item itself
    gkg::AxonMappingOdf axonMappingOdf( &_outputOrientationSet,
                                        *_t2, *_dw,
                                        originSite,
                                        _axonMappingMCMCFunction,
                                        axonMappingMCMCParameterSet,
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
                              axonMappingOdf );

    }
    else
    {

      return axonMappingOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AxonMappingOdfCartesianField::newItem( "
             "const gkg::Vector3d< int32_t >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}


gkg::OrientationDistributionFunction 
gkg::AxonMappingOdfCartesianField::newItem(
                                       const gkg::Vector3d< float >& originSite,
                                       const gkg::Vector3d< int32_t >& lutSite )
{

  try
  {

    // allocating the axon mapping MCMC parameter set   
    gkg::Vector initialParameters( _initialParameters );
    gkg::RCPointer< gkg::AxonMappingMCMCParameterSet >
      axonMappingMCMCParameterSet; 

    if ( _axonMappingModel ==
         gkg::AxonMappingOdfCartesianField::PGSE )
    {

      axonMappingMCMCParameterSet.reset( 
                                    new gkg::PGSEAxonMappingMCMCParameterSet(
                                                     this->_lowerParameters,
                                                     this->_upperParameters,
                                                     this->_fixed,
                                                     this->_deltaParameters ) );

    }
    else if ( _axonMappingModel ==
              gkg::AxonMappingOdfCartesianField::CosineTrapezoidOGSE )
    {

      axonMappingMCMCParameterSet.reset( 
                                    new gkg::CTOGSEAxonMappingMCMCParameterSet(
                                                     this->_lowerParameters,
                                                     this->_upperParameters,
                                                     this->_fixed,
                                                     this->_deltaParameters ) );

    }

    // setting the principal orientation to the first eigenvector of the 
    // diffusion tensor model
    const Vector3d< float >& principalOrientation =
                  _tensorCartesianField->getItem( lutSite )->getEigenVector(
                                                       gkg::Tensor::Maximum );
    initialParameters( 8 ) = principalOrientation.x;
    initialParameters( 9 ) = principalOrientation.y;
    initialParameters( 10 ) = principalOrientation.z;

    // creating the axon mapping ODF item itself
    gkg::AxonMappingOdf axonMappingOdf( &_outputOrientationSet,
                                        *_t2, *_dw,
                                        originSite,
                                        _axonMappingMCMCFunction,
                                        axonMappingMCMCParameterSet,
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
                              axonMappingOdf );

    }
    else
    {

      return axonMappingOdf;

    }

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction "
             "gkg::AxonMappingOdfCartesianField::newItem( "
             "const gkg::Vector3d< float >& originSite, "
             "const gkg::Vector3d< int32_t >& lutSite )" );

}
