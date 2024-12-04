#include <gkg-dmri-microstructure-axcaliber/AxCaliberMCMCFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>

#define DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT 20
#define CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_P 0.005
#define CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_Q 0.005
#define INITIAL_STEP_SIZE 0.001

#define GAMMA 2.675221900e8

gkg::AxCaliberMCMCFunction::AxCaliberMCMCFunction( 
                                gkg::RCPointer< gkg::Volume< float > > dw,
                                const gkg::OrientationSet& outputOrientationSet,
                                double noiseStandardDeviation,
                                int32_t maximumCallaghanModelRootCount,
                                int32_t maximumCallaghanModelFunctionCount )
                           : gkg::MicrostructureMCMCFunction(
                                                       dw,
                                                       outputOrientationSet,
                                                       noiseStandardDeviation ),
                             _maximumCallaghanModelRootCount(
                                               maximumCallaghanModelRootCount ),
                             _maximumCallaghanModelFunctionCount(
                                            maximumCallaghanModelFunctionCount )
{

  try
  {

    // checking the sequence type is PGSE //////////////////////////////////
    if ( this->_gradientCharacteristics.find( "type" )
         != this->_gradientCharacteristics.end() )
    {

      gkg::Dictionary::const_iterator 
        c = this->_gradientCharacteristics.find( "type" );
      if ( c->second->getString() !=
           "PGSE" )
      {

        throw std::runtime_error(
                           "only PGSE scheme supported for AxCaliber model" );

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'type'" );

    }

    // collecting gradient magnitude(s) //////////////////////////////////////
    gkg::GenericObjectList golGradientMagnitudes;
    if ( this->_gradientCharacteristics.find( "gradient-magnitudes" )
         != this->_gradientCharacteristics.end() )
    {

      gkg::Dictionary::const_iterator 
        c = this->_gradientCharacteristics.find( "gradient-magnitudes" );

      golGradientMagnitudes = c->second->getValue< gkg::GenericObjectList >();
      int32_t rankCount = ( int32_t )golGradientMagnitudes.size();
      this->_gradientMagnitudes.resize( rankCount );
      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        double value = golGradientMagnitudes[ r ]->getScalar();
        this->_gradientMagnitudes[ r ] = value;

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'gradient-magnitudes'" );

    }

    // collecting little delta(s) ////////////////////////////////////////////
    gkg::GenericObjectList golLittleDeltas;
    if ( this->_gradientCharacteristics.find( "little-delta" )
         != this->_gradientCharacteristics.end() )
    {

      gkg::Dictionary::const_iterator 
        c = this->_gradientCharacteristics.find( "little-delta" );

      golLittleDeltas = c->second->getValue< gkg::GenericObjectList >();

      int32_t rankCount = ( int32_t )golLittleDeltas.size();
      this->_littleDeltas.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        this->_littleDeltas[ r ] = golLittleDeltas[ r ]->getScalar();

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'little-delta'" );

    }

    // collecting big delta(s) ///////////////////////////////////////////////
    gkg::GenericObjectList golBigDeltas;
    if ( this->_gradientCharacteristics.find( "big-delta" )
         != this->_gradientCharacteristics.end() )
    {

      gkg::Dictionary::const_iterator 
        c = this->_gradientCharacteristics.find( "big-delta" );

      golBigDeltas = c->second->getValue< gkg::GenericObjectList >();

      int32_t rankCount = ( int32_t )golBigDeltas.size();
      this->_bigDeltas.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        this->_bigDeltas[ r ] =  golBigDeltas[ r ]->getScalar();

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'big-delta'" );

    }


    // collecting rise time(s) ///////////////////////////////////////////////
    gkg::GenericObjectList golRiseTimes;
    if ( this->_gradientCharacteristics.find( "rise-time" )
         != this->_gradientCharacteristics.end() )
    {

      gkg::Dictionary::const_iterator 
        c = this->_gradientCharacteristics.find( "rise-time" );

      golRiseTimes = c->second->getValue< gkg::GenericObjectList >();

      int32_t rankCount = ( int32_t )golRiseTimes.size();
      this->_riseTimes.resize( rankCount );

      int32_t r = 0;
      for ( r = 0; r < rankCount; r++ )
      {

        this->_riseTimes[ r ] =  golRiseTimes[ r ]->getScalar();

      }

    }
    else
    {

      throw std::runtime_error( "missing attribute 'rise-time'" );

    }

//       // computing b-values ////////////////////////////////////////////////////
//       int32_t rankCount = ( int32_t )( _gradientMagnitudes.size() );
//       _bValues.resize( rankCount );
// 
//       int32_t r = 0;
//       for ( r = 0; r < rankCount; r++ )
//       {
// 
//         _bValues[ r ] = GAMMA * GAMMA *
//                         _gradientMagnitudes[ r ] * _gradientMagnitudes[ r ] *
//                         ( ( _littleDeltas[ r ] - _riseTimes[ r ] ) *
//                           ( _littleDeltas[ r ] - _riseTimes[ r ] ) *
//                           ( _bigDeltas[ r ] - ( 1.0 / 3.0 ) * 
//                             ( _littleDeltas[ r ] - _riseTimes[ r ] ) ) -
//                           ( 1.0 / 6.0 ) *  
//                           ( _littleDeltas[ r ] - _riseTimes[ r ] ) *
//                           _riseTimes[ r ] * _riseTimes[ r ] +
//                           ( 1.0 / 30.0 ) *
//                           _riseTimes[ r ] * _riseTimes[ r ] * _riseTimes[ r ] );
// 
//       }

 
  }
  GKG_CATCH( "gkg::AxCaliberMCMCFunction::AxCaliberMCMCFunction( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet& outputOrientationSet, "
             "double noiseStandardDeviation, "
             "int32_t maximumCallaghanModelRootCount, "
             "int32_t maximumCallaghanModelFunctionCount )" );

}


gkg::AxCaliberMCMCFunction::~AxCaliberMCMCFunction()
{
}


void gkg::AxCaliberMCMCFunction::getValuesAt( const gkg::Vector& parameters,
                                              gkg::Vector& values ) const
{

  try
  {

    // pointing to parameters using references
    const double& hinderedDiffusivity = parameters( 0 );
    const double& intracellularFraction = parameters( 1 );
    const double& intracellularPerpendicularDiffusivity = parameters( 2 );
    const double& intracellularParallelDiffusivity = parameters( 3 );
    const double& diameterMean = parameters( 4 );
    const double& diameterStdDev = parameters( 5 );


    // precomputing weights from target Gamma distribution
    std::vector< float > weights( DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT );
    std::vector< float > diameters( DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT );

    double shape = ( diameterMean * diameterMean ) / 
                   ( diameterStdDev * diameterStdDev );
    double scale = ( diameterStdDev * diameterStdDev ) / diameterMean;

    double lowerDiameter = _factory->getGammaCdfInverseP(
                                   CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_P,
                                   shape,
                                   scale );
    double upperDiameter = _factory->getGammaCdfInverseP(
                                   CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_P,
                                   shape,
                                   scale );

    double deltaDiameter = ( upperDiameter - lowerDiameter ) /
                           DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT;
    int32_t d = 0;
    for ( d = 0; d < DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT; d++ )
    {

      diameters[ d ] = ( float )lowerDiameter + ( ( float )d + 0.5f ) * 
                       ( float )deltaDiameter;
      weights[ d ] = ( float )_factory->getGammaCdfInverseP(
                                                    lowerDiameter + ( d + 1 ) *
                                                    deltaDiameter,
                                                    shape,
                                                    scale ) -
                     ( float )_factory->getGammaCdfInverseP(
                                                    lowerDiameter + d *
                                                    deltaDiameter,
                                                    shape,
                                                    scale );

    }


    // looping over measurements
    int32_t i = 0;
    int32_t r = 0;
    int32_t f = 0;

    float intracellularContribution = 0.0;
    float hinderedContribution = 0.0;

    for ( i = 0; i < this->_inputOrientationCount; i++ )
    {

      // computing CSF contribution due to partial volume effect
      hinderedContribution = std::exp( -this->_bValues[ i ] *
                                       hinderedDiffusivity );

      // computing restricted intracellular contribution
      intracellularContribution = 0.0f;

      // looping over axon diameters
      for ( d = 0; d < DIAMETER_GAMMA_DISTRIBUTION_BIN_COUNT; d++ )
      {

        const float& currentDiameter = diameters[ d ];
        const float& currentWeight = weights[ d ];

        float normalizedWeight = currentWeight /
                                 ( M_PI * currentDiameter * currentDiameter );

        float besselZerothOrderContribution = 0.0f;

        double q = ( GAMMA / 2 * M_PI ) * _littleDeltas[ i ] *
                   _gradientMagnitudes[ i ];
        double twoPiTimesQTimesDiameter = 2.0 * M_PI * q * currentDiameter;
        double squareOfTwoPiTimesQTimesDiameter = twoPiTimesQTimesDiameter *
                                                  twoPiTimesQTimesDiameter;

        // looping over Callaghan's roots
        for ( r = 0; r < _maximumCallaghanModelRootCount; r++ )
        {

          double betaOr = _factory->getZeroBesselDerivative( 0, r );
          double squareOfBetaOr = betaOr * betaOr;
          besselZerothOrderContribution += ( float )( 
              4.0 * std::exp( -betaOr *
                              intracellularPerpendicularDiffusivity *
                              _bigDeltas[ i ] / 
                              ( currentDiameter * currentDiameter ) ) *
              ( ( twoPiTimesQTimesDiameter *
                  _factory->getBesselDerivative( 0, twoPiTimesQTimesDiameter,
                                                 INITIAL_STEP_SIZE ) ) /
                ( squareOfTwoPiTimesQTimesDiameter -
                  squareOfBetaOr ) ) );

        }


        float besselNthOrderContribution = 0.0f;
        // looping over Callaghan's roots
        for ( r = 0; r < _maximumCallaghanModelRootCount; r++ )
        {


          // looping over Callaghan's functions
          for ( f = 0; f < _maximumCallaghanModelFunctionCount; f++ )
          {

            double betafr = _factory->getZeroBesselDerivative( f, r );
            double squareOfBetafr = betafr * betafr;

            besselNthOrderContribution = ( float )( 
              8.0 * std::exp( -betafr *
                              intracellularPerpendicularDiffusivity *
                              _bigDeltas[ i ] / 
                              ( currentDiameter * currentDiameter ) ) *
              ( squareOfBetafr / ( squareOfBetafr - f * f ) ) *
              ( ( twoPiTimesQTimesDiameter *
                  _factory->getBesselDerivative( f, twoPiTimesQTimesDiameter,
                                                 INITIAL_STEP_SIZE ) ) /
                ( squareOfTwoPiTimesQTimesDiameter -
                  squareOfBetafr ) ) );

          }

        }

        intracellularContribution += normalizedWeight * 
                                     ( besselZerothOrderContribution +
                                       besselNthOrderContribution );

      }

      // computing mixture of compartments
      values( i ) = ( 1.0 - intracellularFraction ) * hinderedContribution +
                    intracellularFraction * intracellularContribution;

    }

  }
  GKG_CATCH( "void gkg::AxCaliberMCMCFunction::getValuesAt( "
             "const gkg::Vector& parameters, "
             "gkg::Vector& values ) const" );

}


void gkg::AxCaliberMCMCFunction::getModelAt(
         const gkg::Vector& /*parameters*/,
         const std::vector< float >& /*multipleShellBValues*/,
         const std::vector< std::vector< float > >& /*gradientCharacteristics*/,
         int32_t /*inputOrientationCount*/,
         std::vector< std::vector< float > >& /*values*/,
         std::vector< float >& /*coordinates*/ ) const
{

  try
  {

    // to be implemented like in NODDI model
    throw std::runtime_error( "not implemented yet" );

  }
  GKG_CATCH( "void gkg::AxCaliberMCMCFunction::getModelAt( "
             "const gkg::Vector& parameters, "
             "const std::vector< float >& bValues, "
             "int32_t inputOrientationCount, "
             "std::vector< std::vector< float > >& values, "
             "std::vector< float >& coordinates ) const" );

}


gkg::OrientationDistributionFunction::BasisType 
gkg::AxCaliberMCMCFunction::getType() const
{

  try
  {

    return gkg::OrientationDistributionFunction::AxCaliber;

  }
  GKG_CATCH( "gkg::OrientationDistributionFunction::BasisType "
             "gkg::NoddiMCMCWatsonFunction::getType() const" );


}


int32_t gkg::AxCaliberMCMCFunction::getParameterCount() const
{

  try
  {

    return 6;

  }
  GKG_CATCH( "int32_t "
             "gkg::AxCaliberMCMCFunction::getParameterCount() const" );

}


int32_t gkg::AxCaliberMCMCFunction::getMaximumCallaghanModelRootCount() const
{

  try
  {

    return _maximumCallaghanModelRootCount;

  }
  GKG_CATCH( "int32_t gkg::AxCaliberMCMCFunction::"
             "getMaximumCallaghanModelRootCount() const" );

}


int32_t 
gkg::AxCaliberMCMCFunction::getMaximumCallaghanModelFunctionCount() const
{

  try
  {

    return _maximumCallaghanModelFunctionCount;

  }
  GKG_CATCH( "int32_t gkg::AxCaliberMCMCFunction::"
             "getMaximumCallaghanModelFunctionCount() const" );

}


#undef CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_P
#undef CUMULATIVE_GAMMA_TAIL_DISTRIBUTION_Q
#undef INITIAL_STEP_SIZE

#undef GAMMA


