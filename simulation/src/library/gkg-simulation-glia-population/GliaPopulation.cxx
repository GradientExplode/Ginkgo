#include <gkg-simulation-glia-population/GliaPopulation.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::GliaPopulation::GliaPopulation( int32_t id,
                                     const gkg::VirtualTissue* parent,
                                     const gkg::Dictionary& parameters,
                                     bool verbose,
                                     std::ofstream* osLog )
                    : gkg::Population( id, parent )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting soma parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary somaParameters = gkg::getDictionaryValue( parameters,
                                                              "soma" );


    ////////////////////////////////////////////////////////////////////////////
    // building soma diameter distribution
    ////////////////////////////////////////////////////////////////////////////

    _somaDiameterDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                         gkg::getDictionaryValue( somaParameters,
                                                  "diameter_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting process parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary processParameters = gkg::getDictionaryValue( parameters,
                                                                 "process" );


    ////////////////////////////////////////////////////////////////////////////
    // collecting process atom oversampling ratio
    ////////////////////////////////////////////////////////////////////////////

    _processAtomOversamplingRatio = gkg::getFloatValue(
                                                      processParameters,
                                                      "atom_oversampling_ratio",
                                                      false,
                                                      1.0f );
    this->_branchAtomOversamplingRatio = _processAtomOversamplingRatio;


    ////////////////////////////////////////////////////////////////////////////
    // collecting process minimum segment point count
    ////////////////////////////////////////////////////////////////////////////

    _processMinimumSegmentPointCount = gkg::getIntegerValue(
                                             processParameters,
                                             "minimum_segment_point_count",
                                             false,
                                             20 );


    ////////////////////////////////////////////////////////////////////////////
    // building process diameter distribution
    ////////////////////////////////////////////////////////////////////////////

    _processDiameterDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                         gkg::getDictionaryValue( processParameters,
                                                  "diameter_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // building process node count distribution
    ////////////////////////////////////////////////////////////////////////////

    _processNodeCountDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                       gkg::getDictionaryValue( processParameters,
                                                "node_count_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting process balancing factor (equal to 1.0 when not branching)
    ////////////////////////////////////////////////////////////////////////////

    _processBalancingFactor = gkg::getFloatValue( processParameters,
                                                  "balancing_factor",
                                                  false,
                                                  1.0f );


    ////////////////////////////////////////////////////////////////////////////
    // do we generate process tortuosity ?
    ////////////////////////////////////////////////////////////////////////////

    _processHasTortuosity = ( gkg::getIntegerValue( processParameters,
                                                    "has_tortuosity",
                                                    false,
                                                    1 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting process tortuosity information
    ////////////////////////////////////////////////////////////////////////////

    if ( _processHasTortuosity )
    {

      gkg::Dictionary processTortuosityParameters = gkg::getDictionaryValue(
                                                              processParameters,
                                                              "tortuosity" );
    
      _processTortuosityMagnitude = gkg::getFloatValue( 
                                                    processTortuosityParameters,
                                                    "magnitude" );
      _processTortuosityAngularDispersionInDegrees = gkg::getFloatValue(
                                              processTortuosityParameters,
                                              "angular_dispersion_in_degrees" );
      _processTortuosityWaveLength = gkg::getFloatValue(
                                                    processTortuosityParameters,
                                                    "wave_length" );

    }
    else
    {

      _processTortuosityMagnitude = 0.0f;
      _processTortuosityAngularDispersionInDegrees = 0.0f;
      _processTortuosityWaveLength = 0.0f;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting volume fraction
    ////////////////////////////////////////////////////////////////////////////

    _volumeFraction = gkg::getFloatValue( parameters,
                                          "volume_fraction" );


    ////////////////////////////////////////////////////////////////////////////
    // pointing to the num. analysis factory and creating a random generator
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* 
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    randomGenerator.setSeedWithTime();


    ////////////////////////////////////////////////////////////////////////////
    // computing the field of view volume
    ////////////////////////////////////////////////////////////////////////////

    const gkg::BoundingBox< float >& fieldOfView = _parent->getFieldOfView();
    float fieldOfViewLowerX = fieldOfView.getLowerX();
    float fieldOfViewUpperX = fieldOfView.getUpperX();
    float fieldOfViewLowerY = fieldOfView.getLowerY();
    float fieldOfViewUpperY = fieldOfView.getUpperY();
    float fieldOfViewLowerZ = fieldOfView.getLowerZ();
    float fieldOfViewUpperZ = fieldOfView.getUpperZ();
    float fieldOfViewVolume = ( fieldOfViewUpperX - fieldOfViewLowerX ) *
                              ( fieldOfViewUpperY - fieldOfViewLowerY ) *
                              ( fieldOfViewUpperZ - fieldOfViewLowerZ );


    ////////////////////////////////////////////////////////////////////////////
    // creating somas till we reach the target soma volume fraction
    ////////////////////////////////////////////////////////////////////////////

    float somaDiameter = 0.0f;
    gkg::Vector3d< float > somaCenter;

    std::list< gkg::Atom* > somaAtoms;

    float totalVolume = 0.0f;
    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "    totalVolume / fieldOfViewVolume : "
               << std::fixed
               << std::setprecision( 4 )
               << totalVolume / fieldOfViewVolume
               << " (target="
               << std::fixed
               << std::setprecision( 4 )
               << _volumeFraction
               << ")"
               << std::flush;

      }
      else
      {

        std::cout << "    totalVolume / fieldOfViewVolume : "
                  << std::fixed
                  << std::setprecision( 4 )
                  << totalVolume / fieldOfViewVolume
                  << " (target="
                  << std::fixed
                  << std::setprecision( 4 )
                  << _volumeFraction
                  << ")"
                  << std::flush;

      }

    }

    do
    {

      // computing a soma diameter 
      somaDiameter = _somaDiameterDistribution->getRandomValue();


      // randomly selecting the center of the soma inside the voxel
      somaCenter.x = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerX,
                                                            fieldOfViewUpperX );
      somaCenter.y = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerY,
                                                            fieldOfViewUpperY );
      somaCenter.z = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerZ,
                                                            fieldOfViewUpperZ );

      // adding a new soma
      somaAtoms.push_back( new gkg::SphereAtom( somaCenter,
                                                somaDiameter / 2.0f ) );
      totalVolume += somaAtoms.back()->getVolume();

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << gkg::Eraser( 22 )
                 << std::flush
                 << std::fixed
                 << std::setprecision( 4 )
                 << totalVolume / fieldOfViewVolume
                 << " (target="
                 << std::fixed
                 << std::setprecision( 4 )
                 << _volumeFraction
                 << ")"
                 << std::flush;

        }
        else
        {

          std::cout << gkg::Eraser( 22 )
                    << std::flush
                    << std::fixed
                    << std::setprecision( 4 )
                    << totalVolume / fieldOfViewVolume
                    << " (target="
                    << std::fixed
                    << std::setprecision( 4 )
                    << _volumeFraction
                    << ")"
                    << std::flush;

        }

      }

    }
    while( ( totalVolume / fieldOfViewVolume ) < _volumeFraction );

    if ( verbose )
    {

      std::cout << std::endl;

    }

    _somaAtoms = std::vector< gkg::Atom* >( somaAtoms.begin(),
                                            somaAtoms.end() );
    somaAtoms.clear();

  }
  GKG_CATCH( "gkg::GliaPopulation::GliaPopulation( int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::GliaPopulation::GliaPopulation( const gkg::GliaPopulation& other )
                    : gkg::Population( other ),
                      _somaDiameterDistribution(
                        other._somaDiameterDistribution ),
                      _processAtomOversamplingRatio(
                        other._processAtomOversamplingRatio ),
                      _processMinimumSegmentPointCount(
                        other._processMinimumSegmentPointCount ),
                      _processDiameterDistribution(
                        other._processDiameterDistribution ),
                      _processNodeCountDistribution(
                        other._processNodeCountDistribution ),
                      _processBalancingFactor( 
                        other._processBalancingFactor ),
                      _processHasTortuosity(
                        other._processHasTortuosity ),
                      _processTortuosityMagnitude(
                        other._processTortuosityMagnitude ),
                      _processTortuosityAngularDispersionInDegrees(
                        other._processTortuosityAngularDispersionInDegrees ),
                      _processTortuosityWaveLength(
                        other._processTortuosityWaveLength ),
                      _volumeFraction(
                        other._volumeFraction ),
                      _somaAtoms( other._somaAtoms )
{
}


gkg::GliaPopulation& gkg::GliaPopulation::operator=(
                                         const gkg::GliaPopulation& other )
{

  try
  {

    this->gkg::Population::operator=( other );

    _somaDiameterDistribution = other._somaDiameterDistribution;
    _processAtomOversamplingRatio =  other._processAtomOversamplingRatio;
    _processMinimumSegmentPointCount = other._processMinimumSegmentPointCount;
    _processDiameterDistribution = other._processDiameterDistribution;
    _processNodeCountDistribution = other._processNodeCountDistribution;
    _processBalancingFactor = other._processBalancingFactor;
    _processHasTortuosity = other._processHasTortuosity;
    _processTortuosityMagnitude = other._processTortuosityMagnitude;
    _processTortuosityAngularDispersionInDegrees = 
                             other._processTortuosityAngularDispersionInDegrees;
    _processTortuosityWaveLength = other._processTortuosityWaveLength;
    _volumeFraction = other._volumeFraction;
    _somaAtoms = other._somaAtoms;

    return *this;

  }
  GKG_CATCH( "gkg::GliaPopulation&gkg::GliaPopulation::operator=( "
             "const gkg::GliaPopulation& other )" );

}


gkg::GliaPopulation::~GliaPopulation()
{
}
