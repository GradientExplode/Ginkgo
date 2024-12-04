#ifndef _gkg_processing_bias_BiasCorrection3d_i_h_
#define _gkg_processing_bias_BiasCorrection3d_i_h_


#include <gkg-processing-bias/BiasCorrection3d.h>
#include <gkg-processing-bias/BiasCorrection_i.h>
#include <gkg-processing-bias/EntropyBiasSimulatedAnnealingConfiguration3d_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>
#include <map>


//
// class BiasCorrection3d< T >
//

template < class T >
inline
gkg::BiasCorrection3d< T >::BiasCorrection3d(
                            const std::vector< double >& scalarParameters,
                            const std::vector< std::string >& stringParameters )
                           : gkg::BiasCorrection< T >()
{

  try
  {

    std::vector< double > theScalarParameters;
    std::vector< std::string > theStringParameters;

    //
    // sanity check(s)
    //
    if ( scalarParameters.empty() ||
         stringParameters.empty() )
    {

      theScalarParameters.resize( 16U );
      theScalarParameters[ 0 ] = 10;
      theScalarParameters[ 1 ] = 10;
      theScalarParameters[ 2 ] = 10;
      theScalarParameters[ 3 ] = 1.0;
      theScalarParameters[ 4 ] = 1.0;
      theScalarParameters[ 5 ] = 1.0;
      theScalarParameters[ 6 ] = 0;
      theScalarParameters[ 7 ] = 0;
      theScalarParameters[ 8 ] = 0;
      theScalarParameters[ 9 ] = 0;
      theScalarParameters[ 10 ] = 0;
      theScalarParameters[ 11 ] = 0;
      theScalarParameters[ 12 ] = 0;
      theScalarParameters[ 13 ] = 0;
      theScalarParameters[ 14 ] = 0;
      theScalarParameters[ 15 ] = 0;

    }
    else
    {

      theScalarParameters = scalarParameters;
      theStringParameters = stringParameters;

    }

    if ( !theStringParameters.empty() )
    {

      throw std::runtime_error( "need no string parameters" );

    }

    //
    // dealing with scalar parameters
    //
    _piecewiseBiasSize.x = ( int32_t )( theScalarParameters[ 0 ] + 0.5 );
    _piecewiseBiasSize.y = ( int32_t )( theScalarParameters[ 1 ] + 0.5 );
    _piecewiseBiasSize.z = ( int32_t )( theScalarParameters[ 2 ] + 0.5 );
    _entropyFactor = theScalarParameters[ 3 ];
    _dataAttachmentFactor = theScalarParameters[ 4 ];
    _regularizationFactor = theScalarParameters[ 5 ];
    _k = theScalarParameters[ 6 ];
    _mu = theScalarParameters[ 7 ];
    _initialTemperature  = theScalarParameters[ 8 ];
    _minimumTemperature = theScalarParameters[ 9 ];
    _trialCountPerStep = ( int32_t )( theScalarParameters[ 10 ] + 0.5 );
    _iterationCountPerTemperature = ( int32_t )( theScalarParameters[ 11 ] +
                                                 0.5 );
    _maximumStepSize = theScalarParameters[ 12 ];
    _verboseSimulatedAnnealing = ( int32_t )( theScalarParameters[ 13 ] +
                                              0.5 ) ?
                                 true:
                                 false;
    _entropyLevelCount = ( int32_t )( theScalarParameters[ 14 ] + 0.5 );
    _entropyIsSmoothingApplied = ( int32_t )( theScalarParameters[ 15 ] +
                                              0.5 ) ?
                                 true:
                                 false;

  }
  GKG_CATCH( "gkg::BiasCorrection3d< T >::BiasCorrection3d( "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters )" );

}


template < class T >
inline
gkg::BiasCorrection3d< T >::~BiasCorrection3d()
{
}


template < class T >
std::string gkg::BiasCorrection3d< T >::getName() const
{

  return getStaticName();

}


template < class T >
std::string gkg::BiasCorrection3d< T >::getStaticName()
{

  return "bias-correction-3d";

}


template < class T >
inline
void gkg::BiasCorrection3d< T >::correct( const gkg::Volume< T >& in,
                                          const gkg::Volume< int16_t >& mask,
                                          const gkg::Volume< float >& weight,
                                          gkg::Volume< T >& out,
                                          gkg::Volume< float >& bias,
                                          bool verbose ) const
{

  try
  {

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    // creating 3D bias Simulated Annealing configuration
    gkg::EntropyBiasSimulatedAnnealingConfiguration3d< T >
      entropyBiasSimulatedAnnealingConfiguration3d( in,
                                                    mask,
                                                    weight,
                                                    out,
                                                    bias,
                                                    _piecewiseBiasSize,
                                                    _entropyFactor,
                                                    _dataAttachmentFactor,
                                                    _regularizationFactor,
                                                    _entropyLevelCount,
                                                    _entropyIsSmoothingApplied );


    // looping over times
    int32_t t;
    int32_t sizeT = in.getSizeT();
    for ( t = 0; t < sizeT; t++ )
    {

      if ( verbose && !_verboseSimulatedAnnealing )
      {

        if ( t != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << " time[ " << std::setw( 4 ) << t + 1
                  << " / " << std::setw( 4 ) << sizeT
                  << " ]" << std::flush;

      }
      else
      {

        if ( t == 0 )
        {

          std::cout << std::endl;

        }
        std::cout << "--------------> time = " << t << std::endl;

      }

      entropyBiasSimulatedAnnealingConfiguration3d.setTime( t );

      gkg::SimulatedAnnealingConfiguration::BoltzmannParameters
        boltzmannParameters( _k,
                             _mu,
                             _initialTemperature,
                             _minimumTemperature );
      gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
      factory->getSimulatedAnnealingOptimumConfiguration(
                                   entropyBiasSimulatedAnnealingConfiguration3d,
                                   _trialCountPerStep,
                                   _iterationCountPerTemperature,
                                   _maximumStepSize,
                                   boltzmannParameters,
                                   randomGenerator,
                                   _verboseSimulatedAnnealing );

      if ( verbose && !_verboseSimulatedAnnealing )
      {

        std::cout << gkg::Eraser( 41 );

      }

    }

    // applying bias
    entropyBiasSimulatedAnnealingConfiguration3d.applyBiasCorrection();

  }
  GKG_CATCH( "template < class T > "
             "inline "
             "void gkg::BiasCorrection3d< T >::correct( "
             "const gkg::Volume< T >& in, "
             "const gkg::Volume< float >& weight, "
             "gkg::Volume< T >& out, "
             "gkg::Volume< float >& bias, "
             "bool verbose ) const" );

}


#endif


