#include <gkg-dmri-microstructure-axon-mapping/CTOGSEIntracellularContributionCache.h>
#include <gkg-dmri-microstructure-axon-mapping/CanonicalWatsonSphericalHarmonicsCache.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-pattern/StdVectorCompare.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


#define GAMMA 2.675221900e8


gkg::CTOGSEIntracellularContributionCache::
                                          CTOGSEIntracellularContributionCache()
                                          : _factory(
     gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory() ),
                                           _gradientCharacteristics( 0 ),
                                           _maximumGPDCylinderSumIndex( 0 ),
                                           _defaultKappa( 0.0 ),
                                           _lowerKappa( 0.0 ),
                                           _upperKappa( 0.0 ),
                                           _kappaBinCount( 0 ),
                                           _fixedKappa( false ),
                                           _dotProductBinCount( 0 ),
                                           _defaultParallelDiffusivity( 0.0 ),
                                           _lowerParallelDiffusivity( 0.0 ),
                                           _upperParallelDiffusivity( 0.0 ),
                                           _parallelDiffusivityBinCount( 0 ),
                                           _fixedParallelDiffusivity( false ),
                                           _defaultAxonDiameter( 0.0 ),
                                           _lowerAxonDiameter( 0.0 ),
                                           _upperAxonDiameter( 0.0 ),
                                           _axonDiameterBinCount( 0 ),
                                           _fixedAxonDiameter( false )
{
}


gkg::CTOGSEIntracellularContributionCache::
                                         ~CTOGSEIntracellularContributionCache()
{
}


void gkg::CTOGSEIntracellularContributionCache::update(
                                 const gkg::Dictionary& gradientCharacteristics,
                                 int32_t maximumGPDCylinderSumIndex,
                                 double defaultKappa,
                                 double lowerKappa,
                                 double upperKappa,
                                 int32_t kappaBinCount,
                                 bool fixedKappa,
                                 int32_t dotProductBinCount,
                                 double defaultParallelDiffusivity,
                                 double lowerParallelDiffusivity,
                                 double upperParallelDiffusivity,
                                 int32_t parallelDiffusivityBinCount,
                                 bool fixedParallelDiffusivity,
                                 double defaultAxonDiameter,
                                 double lowerAxonDiameter,
                                 double upperAxonDiameter,
                                 int32_t axonDiameterBinCount,
                                 bool fixedAxonDiameter )
{

  try
  {



    if ( ( &gradientCharacteristics != _gradientCharacteristics ) ||

         ( maximumGPDCylinderSumIndex != _maximumGPDCylinderSumIndex ) ||

         ( defaultKappa != _defaultKappa ) ||
         ( lowerKappa != _lowerKappa ) ||
         ( upperKappa != _upperKappa ) ||
         ( kappaBinCount != _kappaBinCount ) ||
         ( fixedKappa != _fixedKappa ) ||

         ( dotProductBinCount != _dotProductBinCount ) ||

         ( defaultParallelDiffusivity != _defaultParallelDiffusivity ) ||
         ( lowerParallelDiffusivity != _lowerParallelDiffusivity ) ||
         ( upperParallelDiffusivity != _upperParallelDiffusivity ) ||
         ( parallelDiffusivityBinCount != _parallelDiffusivityBinCount ) ||
         ( fixedParallelDiffusivity != _fixedParallelDiffusivity ) ||

         ( defaultAxonDiameter != _defaultAxonDiameter ) ||
         ( lowerAxonDiameter != _lowerAxonDiameter ) ||
         ( upperAxonDiameter != _upperAxonDiameter ) ||
         ( axonDiameterBinCount != _axonDiameterBinCount ) ||
         ( fixedAxonDiameter != _fixedAxonDiameter ) )
    {

      // locking mutex in order to be thread safe
      _mutex.lock();


      // updating protected field(s)
      _gradientCharacteristics = &gradientCharacteristics;
      _maximumGPDCylinderSumIndex = maximumGPDCylinderSumIndex;
      _defaultKappa = defaultKappa;
      _lowerKappa = lowerKappa;
      _upperKappa = upperKappa;
      _kappaBinCount = kappaBinCount;
      _fixedKappa = fixedKappa;
      _dotProductBinCount = dotProductBinCount;
      _defaultParallelDiffusivity = defaultParallelDiffusivity;
      _lowerParallelDiffusivity = lowerParallelDiffusivity;
      _upperParallelDiffusivity = upperParallelDiffusivity;
      _parallelDiffusivityBinCount = parallelDiffusivityBinCount;
      _fixedParallelDiffusivity = fixedParallelDiffusivity;
      _defaultAxonDiameter = defaultAxonDiameter;
      _lowerAxonDiameter = lowerAxonDiameter;
      _upperAxonDiameter = upperAxonDiameter;
      _axonDiameterBinCount = axonDiameterBinCount;
      _fixedAxonDiameter = fixedAxonDiameter;

      if ( _fixedKappa )
      {

        _kappaBinCount = 1;

      }

      if ( _fixedParallelDiffusivity )
      {

        _parallelDiffusivityBinCount = 1;

      }

      if ( _fixedAxonDiameter )
      {

        _axonDiameterBinCount = 1;

      }


      //////////////////////////////////////////////////////////////////////////
      // decoding the diffusion encoding scheme assuming it is CTOGSE
      //////////////////////////////////////////////////////////////////////////

      // checking the sequence type is CTOGSE //////////////////////////////////
      if ( this->_gradientCharacteristics->find( "type" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "type" );
        if ( c->second->getString() !=
             "Cosine-Trapezoid-OGSE" )
        {

          throw std::runtime_error(
                                  "only Cosine-Trapezoid-OGSE scheme supported "
                                  "for Axon Mapping model" );

        }

      }
      else
      {

        throw std::runtime_error( "missing attribute 'type'" );

      }


      // collecting gradient magnitude(s) //////////////////////////////////////
      gkg::GenericObjectList golGradientMagnitudes;
      if ( this->_gradientCharacteristics->find( "gradient-magnitudes" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "gradient-magnitudes" );

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
      if ( this->_gradientCharacteristics->find( "little-delta" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "little-delta" );

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
      if ( this->_gradientCharacteristics->find( "big-delta" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "big-delta" );

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
      if ( this->_gradientCharacteristics->find( "rise-time" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "rise-time" );

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

      // collecting frequency(s) ///////////////////////////////////////////////
      gkg::GenericObjectList golPeriods;
      if ( this->_gradientCharacteristics->find( "period" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "period" );

        golPeriods = c->second->getValue< gkg::GenericObjectList >();

        int32_t rankCount = ( int32_t )golPeriods.size();
        this->_frequencies.resize( rankCount );
        int32_t r = 0;
        for ( r = 0; r < rankCount; r++ )
        {

          this->_frequencies[ r ] =  1.0 / golPeriods[ r ]->getScalar();

        }

      }
      else
      {

        throw std::runtime_error( "missing attribute period'" );

      }

      // collecting phases ///////////////////////////////////////////////
      gkg::GenericObjectList golPhases;
      if ( this->_gradientCharacteristics->find( "phase" )
           != this->_gradientCharacteristics->end() )
      {

        gkg::Dictionary::const_iterator 
          c = this->_gradientCharacteristics->find( "phase" );

        golPhases = c->second->getValue< gkg::GenericObjectList >();

        int32_t rankCount = ( int32_t )golPhases.size();
        this->_phases.resize( rankCount );

        int32_t r = 0;
        for ( r = 0; r < rankCount; r++ )
        {

          this->_phases[ r ] =  golPhases[ r ]->getScalar();

        }

      }
      else
      {

        throw std::runtime_error( "missing attribute phase'" );

      }

      // computing b-values ////////////////////////////////////////////////////
      int32_t rankCount = ( int32_t )( _gradientMagnitudes.size() );
      _bValues.resize( rankCount );

      int32_t lobeCount = 0;
      double effectiveBigDelta = 0.0;
      int32_t r = 0;

      for ( r = 0; r < rankCount; r++ )
      {

        lobeCount = ( int32_t )( 2.0 * _littleDeltas[ r ] * 
                                             _frequencies[ r ] + 1.0 );
        effectiveBigDelta = 1.0 / ( 6.0 * _frequencies[ r ] ) +
                            ( 2.0 * _riseTimes[ r ] ) / 3.0 -
                            ( _riseTimes[ r ] * _riseTimes[ r ] ) /
                            ( 6.0 * ( 1.0 / ( 4.0 * _frequencies[ r ] ) -
                                      0.5 * _riseTimes[ r ] ) ) +
                            ( 2.0 * lobeCount + 1.0 ) *
                            std::pow( _riseTimes[ r ], 3 ) /
                            ( 120.0 * lobeCount *
                              ( 1.0 / ( 4.0 * _frequencies[ r ] ) -
                                0.5 * _riseTimes[ r ] ) );


        _bValues[ r ] = 4.0 * lobeCount * 
                        GAMMA *  _gradientMagnitudes[ r ] * 
                        GAMMA *  _gradientMagnitudes[ r ] *
                        std::pow( ( 0.25 / ( _frequencies[ r ] ) -
                                   0.5 * _riseTimes[ r ] ),
                                  2 ) *
                        effectiveBigDelta; 

      }


      // building a LUT of sequence configuration(s) ///////////////////////////

      int32_t index = 0;
      std::map< std::vector< double >, std::pair< int32_t, double >,            
                gkg::StdVectorCompare< double > > sequenceConfigurationLut;

      for ( r = 0; r < rankCount; r++ )
      {

        std::vector< double > sequenceConfiguration( 6, 0.0 );
        sequenceConfiguration[ 0 ] = _gradientMagnitudes[ r ];
        sequenceConfiguration[ 1 ] = _littleDeltas[ r ];
        sequenceConfiguration[ 2 ] = _bigDeltas[ r ];
        sequenceConfiguration[ 3 ] = _riseTimes[ r ];
        sequenceConfiguration[ 4 ] = _frequencies[ r ];
        sequenceConfiguration[ 5 ] = _phases[ r ];


        if ( sequenceConfigurationLut.find( sequenceConfiguration ) ==
             sequenceConfigurationLut.end() )
        {

          sequenceConfigurationLut[ sequenceConfiguration ] = 
                           std::pair< int32_t, double >( index, _bValues[ r ] );
          ++ index;

        }

      }

      int32_t sequenceConfigurationCount = 
                                     ( int32_t )sequenceConfigurationLut.size();


      _sequenceConfigurationIndices.resize( rankCount );
      for ( r = 0; r < rankCount; r++ )
      {

        std::vector< double > sequenceConfiguration( 6, 0.0 );
        sequenceConfiguration[ 0 ] = _gradientMagnitudes[ r ];
        sequenceConfiguration[ 1 ] = _littleDeltas[ r ];
        sequenceConfiguration[ 2 ] = _bigDeltas[ r ];
        sequenceConfiguration[ 3 ] = _riseTimes[ r ];
        sequenceConfiguration[ 4 ] = _frequencies[ r ];
        sequenceConfiguration[ 5 ] = _phases[ r ];

        _sequenceConfigurationIndices[ r ] = 
                        sequenceConfigurationLut[ sequenceConfiguration ].first;
        
      }

      _sequenceConfigurations.resize( sequenceConfigurationCount );
      std::map< std::vector< double >, std::pair< int32_t, double >,
                gkg::StdVectorCompare< double > >::const_iterator
        s = sequenceConfigurationLut.begin(),
        se = sequenceConfigurationLut.end();
      while ( s != se )
      {

        std::vector< double >& sequenceConfiguration = 
                                     _sequenceConfigurations[ s->second.first ];
        sequenceConfiguration.resize( 7 );
        sequenceConfiguration[ 0 ] = s->second.second;
        sequenceConfiguration[ 1 ] = ( s->first )[ 0 ];
        sequenceConfiguration[ 2 ] = ( s->first )[ 1 ];
        sequenceConfiguration[ 3 ] = ( s->first )[ 2 ];
        sequenceConfiguration[ 4 ] = ( s->first )[ 3 ];
        sequenceConfiguration[ 5 ] = ( s->first )[ 4 ];
        sequenceConfiguration[ 6 ] = ( s->first )[ 5 ];

        ++ s;

      }


      //////////////////////////////////////////////////////////////////////////
      // building a LUT of intracellular contribution of a cylinder
      //////////////////////////////////////////////////////////////////////////

      double bValue = 0.0;
      double diffusionGradientMagnitude = 0.0;
      double littleDelta = 0.0;
      double bigDelta = 0.0;
      double riseTime = 0.0;
      double frequency = 0.0;
      double phase = 0.0;
      double kappa = 0.0;
      double dotProduct = 0.0;
      double parallelDiffusivity = 0.0;
      double axonDiameter = 0.0;

      _intracellularContributionLut.resize( sequenceConfigurationCount );

      int32_t sequenceConfigurationIndex = 0;
      for ( sequenceConfigurationIndex = 0;
            sequenceConfigurationIndex < sequenceConfigurationCount;
            sequenceConfigurationIndex++ )
      {

        const std::vector< double >& sequenceConfiguration =
                          _sequenceConfigurations[ sequenceConfigurationIndex ];
        bValue = sequenceConfiguration[ 0 ];
        diffusionGradientMagnitude = sequenceConfiguration[ 1 ];
        littleDelta = sequenceConfiguration[ 2 ];
        bigDelta = sequenceConfiguration[ 3 ];
        riseTime = sequenceConfiguration[ 4 ];
        frequency = sequenceConfiguration[ 5 ];
        phase = sequenceConfiguration[ 6 ];

        _intracellularContributionLut
          [ sequenceConfigurationIndex ].resize( _kappaBinCount );

        int32_t kappaIndex = 0;
        for ( kappaIndex = 0; kappaIndex < _kappaBinCount;
              kappaIndex++ )
        {

          if ( fixedKappa )
          {

            kappa = _defaultKappa;

          }
          else
          {

            kappa = _lowerKappa + 
                    ( double )kappaIndex *
                    ( _upperKappa -
                      _lowerKappa  ) /
                    ( double )_kappaBinCount;

          }

          _intracellularContributionLut
            [ sequenceConfigurationIndex ]
            [ kappaIndex ].resize( _dotProductBinCount );
         
          int32_t dotProductIndex = 0;
          for ( dotProductIndex = 0; dotProductIndex < _dotProductBinCount;
                dotProductIndex++ )
          {

            dotProduct = -1.0 + 
                         ( double )dotProductIndex * 2.0 /
                         ( double )_dotProductBinCount;

            _intracellularContributionLut
              [ sequenceConfigurationIndex ]
              [ kappaIndex ]
              [ dotProductIndex ].resize( _parallelDiffusivityBinCount );
 
            int32_t parallelDiffusivityIndex = 0;
            for ( parallelDiffusivityIndex = 0;
                  parallelDiffusivityIndex < _parallelDiffusivityBinCount;
                  parallelDiffusivityIndex++ )
            {

              if ( _fixedParallelDiffusivity )
              {

                parallelDiffusivity = _defaultParallelDiffusivity;

              }
              else
              {

                parallelDiffusivity = _lowerParallelDiffusivity + 
                                      ( double )parallelDiffusivityIndex *
                                      ( _upperParallelDiffusivity -
                                        _lowerParallelDiffusivity  ) /
                                      ( double )_parallelDiffusivityBinCount;

              }

              _intracellularContributionLut
              [ sequenceConfigurationIndex ]
              [ kappaIndex ]
              [ dotProductIndex ]
              [ parallelDiffusivityIndex ].resize( _axonDiameterBinCount );

              int32_t axonDiameterIndex = 0;
              for ( axonDiameterIndex = 0;
                    axonDiameterIndex < _axonDiameterBinCount;
                    axonDiameterIndex++ )
              {

                if ( _fixedAxonDiameter )
                {

                  axonDiameter = _defaultAxonDiameter;

                }
                else
                {

                  axonDiameter = _lowerAxonDiameter +
                                 ( double )axonDiameterIndex *
                                 ( _upperAxonDiameter -
                                   _lowerAxonDiameter  ) /
                                 ( double )_axonDiameterBinCount;      

                }

                _intracellularContributionLut[ sequenceConfigurationIndex ]
                                             [ kappaIndex ]
                                             [ dotProductIndex ]
                                             [ parallelDiffusivityIndex ]
                                             [ axonDiameterIndex ] =
                                         this->computeIntracellularContribution(
                                                     bValue,
                                                     diffusionGradientMagnitude,
                                                     littleDelta,
                                                     bigDelta,
                                                     riseTime,
                                                     frequency,
                                                     phase,
                                                     kappa,
                                                     dotProduct,
                                                     parallelDiffusivity,
                                                     axonDiameter );

              }

            }

          }

        }

      }

      // unlocking mutex
      _mutex.unlock();

    }

  }
  GKG_CATCH( "void gkg::CTOGSEIntracellularContributionCache::update( "
             "const gkg::Dictionary& gradientCharacteristics, "
             "double defaultKappa, "
             "double lowerKappa, "
             "double upperKappa, "
             "int32_t kappaBinCount, "
             "bool fixedKappa, "
             "int32_t dotProductBinCount, "
             "double defaultParallelDiffusivity, "
             "double lowerParallelDiffusivity, "
             "double upperParallelDiffusivity, "
             "int32_t parallelDiffusivityBinCount, "
             "bool fixedParallelDiffusivity, "
             "double defaultAxonDiameter, "
             "double lowerAxonDiameter, "
             "double upperAxonDiameter, "
             "int32_t axonDiameterBinCount, "
             "bool fixedAxonDiameter )" );

}


const std::vector< double >& 
gkg::CTOGSEIntracellularContributionCache::getGradientMagnitudes() const
{

  try
  {

    return _gradientMagnitudes;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getGradientMagnitudes() "
             "const" );

}


const std::vector< double >& 
gkg::CTOGSEIntracellularContributionCache::getLittleDeltas() const
{

  try
  {

    return _littleDeltas;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getLittleDeltas() "
             "const" );

}


const std::vector< double >& 
gkg::CTOGSEIntracellularContributionCache::getBigDeltas() const
{

  try
  {

    return _bigDeltas;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getBigDeltas() "
             "const" );

}


const std::vector< double >& 
gkg::CTOGSEIntracellularContributionCache::getRiseTimes() const
{

  try
  {

    return _riseTimes;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getRiseTimes() "
             "const" );

}


const std::vector< double >&
gkg::CTOGSEIntracellularContributionCache::getBValues() const
{

  try
  {

    return _bValues;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getBValues() "
             "const" );

}



const std::vector< double >&
gkg::CTOGSEIntracellularContributionCache::getFrequencies() const
{

  try
  {

    return _frequencies;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getFrequencies() "
             "const" );

}



const std::vector< double >&
gkg::CTOGSEIntracellularContributionCache::getPhases() const
{

  try
  {

    return _phases;

  }
  GKG_CATCH( "const std::vector< double >& "
             "gkg::CTOGSEIntracellularContributionCache::getPhases() "
             "const" );

}



const std::vector< int32_t >& 
gkg::CTOGSEIntracellularContributionCache::getSequenceConfigurationIndices()
const
{

  try
  {

    return _sequenceConfigurationIndices;

  }
  GKG_CATCH( "const std::vector< int32_t >& "
             "gkg::CTOGSEIntracellularContributionCache::"
             "getSequenceConfigurationIndices() "
             "const" );

}


int32_t 
gkg::CTOGSEIntracellularContributionCache::getSequenceConfigurationCount() const
{

  try
  {

    return ( int32_t )_sequenceConfigurations.size();

  }
  GKG_CATCH( "nt32_t "
             "gkg::CTOGSEIntracellularContributionCache::"
             "getSequenceConfigurationCount() "
             "const" );

}


const std::vector< std::vector< double > >& 
gkg::CTOGSEIntracellularContributionCache::getSequenceConfigurations() const
{

  try
  {

    return _sequenceConfigurations;

  }
  GKG_CATCH( "const std::vector< std::vector< double > >& "
             "gkg::CTOGSEIntracellularContributionCache::"
             "getSequenceConfigurations() "
             "const" );

}


double
gkg::CTOGSEIntracellularContributionCache::getIntracellularContribution(
                                        int32_t rank,
                                        double kappa,
                                        double dotProduct,
                                        double parallelDiffusivity,
                                        double axonDiameter ) const
{

  try
  {


    int32_t sequenceConfigurationIndex = _sequenceConfigurationIndices[ rank ];


    int32_t kappaIndex = 0;
    if ( !_fixedKappa )
    {

      kappaIndex = ( int32_t )(
                   ( double )( _kappaBinCount - 1 ) *
                   ( kappa - _lowerKappa ) /
                   ( _upperKappa - _lowerKappa ) + 0.5 );

    }

    int32_t dotProductIndex = ( int32_t )(
              ( double )( _dotProductBinCount - 1 ) *
              ( dotProduct + 1.0 ) / 2.0 + 0.5 );

    int32_t parallelDiffusivityIndex = 0;
    if ( !_fixedParallelDiffusivity )
    {

      parallelDiffusivityIndex = ( int32_t )(
              ( double )( _parallelDiffusivityBinCount - 1 ) *
              ( parallelDiffusivity - _lowerParallelDiffusivity ) /
              ( _upperParallelDiffusivity - _lowerParallelDiffusivity ) + 0.5 );

    }

    int32_t axonDiameterIndex = 0;
    if ( !_fixedAxonDiameter )
    {

      axonDiameterIndex = ( int32_t )(
                          ( double )( _axonDiameterBinCount - 1 ) *
                          ( axonDiameter - _lowerAxonDiameter ) /
                          ( _upperAxonDiameter - _lowerAxonDiameter ) + 0.5 );

    }

    return _intracellularContributionLut[ sequenceConfigurationIndex ]
                                        [ kappaIndex ]
                                        [ dotProductIndex ]
                                        [ parallelDiffusivityIndex ]
                                        [ axonDiameterIndex ];

  }
  GKG_CATCH( "float "
             "gkg::CTOGSEIntracellularContributionCache::"
             "getIntracellularContribution( "
             "int32_t rank, "
             "double kappa, "
             "double dotProduct, "
             "double parallelDiffusivity, "
             "double axonDiameter ) const" );

}

double
gkg::CTOGSEIntracellularContributionCache::getIntracellularContribution(
                                        int32_t bIndex,
                                        int32_t /* rank */,
                                        double kappa,
                                        double dotProduct,
                                        double parallelDiffusivity,
                                        double axonDiameter ) const
{

  try
  {


    int32_t sequenceConfigurationIndex = bIndex;

    int32_t kappaIndex = 0;
    if ( !_fixedKappa )
    {

      kappaIndex = ( int32_t )(
                   ( double )( _kappaBinCount - 1 ) *
                   ( kappa - _lowerKappa ) /
                   ( _upperKappa - _lowerKappa ) + 0.5 );

    }

    int32_t dotProductIndex = ( int32_t )(
              ( double )( _dotProductBinCount - 1 ) *
              ( dotProduct + 1.0 ) / 2.0 + 0.5 );

    int32_t parallelDiffusivityIndex = 0;
    if ( !_fixedParallelDiffusivity )
    {

      parallelDiffusivityIndex = ( int32_t )(
              ( double )( _parallelDiffusivityBinCount - 1 ) *
              ( parallelDiffusivity - _lowerParallelDiffusivity ) /
              ( _upperParallelDiffusivity - _lowerParallelDiffusivity ) + 0.5 );

    }

    int32_t axonDiameterIndex = 0;
    if ( !_fixedAxonDiameter )
    {

      axonDiameterIndex = ( int32_t )(
                          ( double )( _axonDiameterBinCount - 1 ) *
                          ( axonDiameter - _lowerAxonDiameter ) /
                          ( _upperAxonDiameter - _lowerAxonDiameter ) + 0.5 );

    }

    return _intracellularContributionLut[ sequenceConfigurationIndex ]
                                        [ kappaIndex ]
                                        [ dotProductIndex ]
                                        [ parallelDiffusivityIndex ]
                                        [ axonDiameterIndex ];

  }
  GKG_CATCH( "double "
             "gkg::IntracellularContributionCache::"
             "getIntracellularContribution( "
             "int32_t rank, "
             "double kappa, "
             "double dotProduct, "
             "double parallelDiffusivity, "
             "double axonDiameter ) const" );

}




double 
gkg::CTOGSEIntracellularContributionCache::computeIntracellularContribution( 
                                              double bValue,
                                              double diffusionGradientMagnitude,
                                              double littleDelta,
                                              double bigDelta,
                                              double riseTime,
                                              double frequency,
                                              double phase,
                                              double kappa,
                                              double dotProduct,
                                              double parallelDiffusivity,
                                              double axonDiameter ) const
{

  try
  {

    int32_t maximumSphericalHarmonicsOrder = 
                     gkg::CanonicalWatsonSphericalHarmonicsCache::getInstance().
                                            getMaximumSphericalHarmonicsOrder();
    const std::vector< double >& canonicalWatsonFunctionDecomposition =
    gkg::CanonicalWatsonSphericalHarmonicsCache::getInstance().
                                                      getDecomposition( kappa );

    double parallelL = bValue * parallelDiffusivity;
    double perpendicularL =
                   - ( diffusionGradientMagnitude * diffusionGradientMagnitude *
                     this->computePerpendicularAttenuationInCylinder(
                                                            littleDelta,
                                                            bigDelta,
                                                            riseTime,
                                                            frequency,
                                                            phase,
                                                            parallelDiffusivity,
                                                            axonDiameter ) );

    double intracellularContribution = 0.0;
    int32_t l = 0;
    for ( l = 0; l <= maximumSphericalHarmonicsOrder; l += 2 )
    {

      intracellularContribution += 
        canonicalWatsonFunctionDecomposition[ l ] * 
        std::sqrt( ( 2.0 * l + 1.0 ) / ( 4.0 * M_PI ) ) *
        this->_factory->getLegendrePolynomial(
                                 l,
                                 dotProduct ) *
        this->computeC2n( l / 2, parallelL - perpendicularL );

    }

    intracellularContribution *= 0.5 * std::exp( -perpendicularL );

    return std::max( intracellularContribution, 1e-4 );

  }
  GKG_CATCH( "double gkg::CTOGSEIntracellularContributionCache::"
             "computeIntracellularContribution( "
             "double bValue, "
             "double diffusionGradientMagnitude, "
             "double littleDelta, "
             "double bigDelta, "
             "double riseTime, "
             "double frequency, "
             "double phase, "
             "double kappa, "
             "double dotProduct, "
             "double parallelDiffusivity, "
             "double axonDiameter ) const" );

}


double 
gkg::CTOGSEIntracellularContributionCache::
                                      computePerpendicularAttenuationInCylinder(
                                                     double littleDelta,
                                                     double bigDelta,
                                                     double /* riseTime */,
                                                     double frequency,  
                                                     double /* phase */,
                                                     double parallelDiffusivity,
                                                     double axonDiameter ) const
{

  try
  {

    double sum = 0.0;
    double axonRadius = axonDiameter / 2.0;
    double squareOfAxonRadius = axonRadius * axonRadius;
    double squareOfParallelDiffusivity = parallelDiffusivity * 
                                         parallelDiffusivity;
    int32_t m = 0;
    for ( m = 1; m <= _maximumGPDCylinderSumIndex; m++ )
    {


      // collecting the mth zero 'alpha_m' of the derivative of J1 by solving
      //     alpha_m x J1( alpha_m * axon_diameter )' = 0
      double besselRoot = 
                         this->_factory->getZeroBesselDerivativeFunctionJ1( m );

      double squareOfBesselRoot = besselRoot * besselRoot;

      double B_n = 2.0 * ( squareOfAxonRadius / squareOfBesselRoot ) /
                   ( squareOfBesselRoot  - 1.0 );

      double lambda_n = squareOfBesselRoot / squareOfAxonRadius;
      double squareOfLambdaN = lambda_n * lambda_n;

      // for trapezoid with integer number of periods ( cf Ianus )
      //double squareOfRiseTime = riseTime * riseTime;

      double N = 2.0 * littleDelta * frequency;
      double littleDelta_n_tilde = parallelDiffusivity * 
                                   lambda_n * 
                                   littleDelta;
      double bigDelta_n_tilde = parallelDiffusivity * 
                                lambda_n * 
                                bigDelta;      
      double nu_n_tilde = parallelDiffusivity * 
                          lambda_n / ( 2.0 * frequency ); 

      //double t_n_tilde = parallelDiffusivity * 
      //                   lambda_n * 
      //                   riseTime;    
      //double E_1n = std::exp( -( double )( nu_n_tilde ) );

/*
      double gamma_n = ( 1.0 - N * ( std::exp( - nu_n_tilde ) + 1.0 ) +
                       std::exp( - nu_n_tilde ) - 
                       ( std::exp( -bigDelta_n_tilde +
                         0.5 * std::exp( 
                           ( littleDelta_n_tilde - bigDelta_n_tilde ) ) +
                         0.5 * std::exp( 
                           ( -littleDelta_n_tilde - bigDelta_n_tilde ) ) ) ) ) +
                       littleDelta_n_tilde +
                       N * ( std::exp( -nu_n_tilde ) - 1.0 );
*/

      double prefactor = ( 1.0 - std::exp( -nu_n_tilde ) ) /
                         ( 1.0 + std::exp( -nu_n_tilde ) );

      prefactor *= prefactor; 

      double gamma_n = prefactor * 
                       ( 1.0 - N * ( std::exp( - nu_n_tilde ) + 1.0 ) ) +
                       std::exp( - nu_n_tilde ) - 
                       ( std::exp( -bigDelta_n_tilde ) +
                         0.5 * std::exp( 
                           ( littleDelta_n_tilde - bigDelta_n_tilde ) ) +
                         0.5 * std::exp( 
                           ( -littleDelta_n_tilde - bigDelta_n_tilde ) ) ) +
                       littleDelta_n_tilde +
                       N * ( std::exp( -nu_n_tilde ) - 1.0 );

      sum += B_n * gamma_n / squareOfLambdaN;

    }

    return -2.0 * GAMMA * GAMMA * sum / squareOfParallelDiffusivity;


  }
  GKG_CATCH( "double gkg::CTOGSEIntracellularContributionCache::"
             "computePerpendicularAttenuationInCylinder( "
             "double littleDelta, "
             "double bigDelta, "
             "double riseTime, "
             "double frequency, "
             "double phase, "
             "double parallelDiffusivity, "
             "double axonDiameter ) const" );

}


double
gkg::CTOGSEIntracellularContributionCache::
                                         computeC2n( int32_t n, double x ) const
{

  try
  {

    double a = ( double )n + 0.5;
    double b = 2.0 * ( double )n + 1.5;
    return std::pow( -x, n ) *
           ( this->_factory->getGammaFunction( a ) /
             this->_factory->getGammaFunction( b ) ) *
           this->_factory->getHypergeometricFunction1F1( a, b, -x );

  }
  GKG_CATCH( "double gkg::CTOGSEIntracellularContributionCache::computeC2n( "
             "int32_t n, float x ) const" );

}

#undef GAMMA
