#include <gkg-simulation-dendrite-population/DendritePopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomPointOverSphereDistribution.h>
#include <gkg-processing-numericalanalysis/WatsonDispersedOrientationDistribution.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-simulation-dendrite-population/Dendrite.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::DendritePopulation::DendritePopulation( int32_t id,
                                             const gkg::VirtualTissue* parent,
                                             const gkg::Dictionary& parameters,
                                             bool verbose,
                                             std::ofstream* osLog )
                        : gkg::Population( id, parent )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting sphere atom oversampling ratio
    ////////////////////////////////////////////////////////////////////////////

    _atomOversamplingRatio = gkg::getFloatValue( parameters,
                                                 "atom_oversampling_ratio",
                                                 false,
                                                 10.0f );
    this->_branchAtomOversamplingRatio = _atomOversamplingRatio;


    ////////////////////////////////////////////////////////////////////////////
    // collecting dendrite parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary dendriteParameters = gkg::getDictionaryValue( parameters,
                                                                  "dendrite" );


    ////////////////////////////////////////////////////////////////////////////
    // building dendrite diameter distribution
    ////////////////////////////////////////////////////////////////////////////

    _dendriteDiameterDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                         gkg::getDictionaryValue( dendriteParameters,
                                                  "diameter_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // building dendrite length distribution
    ////////////////////////////////////////////////////////////////////////////

    _dendriteLengthDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                         gkg::getDictionaryValue( dendriteParameters,
                                                  "length_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting global angular dispersion in degrees
    ////////////////////////////////////////////////////////////////////////////

    _globalAngularDispersionInDegrees = gkg::getFloatValue(
                                       dendriteParameters,
                                       "global_angular_dispersion_in_degrees" );


    ////////////////////////////////////////////////////////////////////////////
    // do we generate tortuosity ?
    ////////////////////////////////////////////////////////////////////////////

    _hasTortuosity = ( gkg::getIntegerValue( dendriteParameters,
                                             "has_tortuosity",
                                             false,
                                             1 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting tortuosity information
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasTortuosity )
    {

      gkg::Dictionary tortuosityParameters = gkg::getDictionaryValue(
                                                               dendriteParameters,
                                                               "tortuosity" );
    
      _tortuosityMagnitude = gkg::getFloatValue( tortuosityParameters,
                                                 "magnitude" );
      _tortuosityAngularDispersionInDegrees = gkg::getFloatValue(
                                              tortuosityParameters,
                                              "angular_dispersion_in_degrees" );
      _tortuosityWaveLength = gkg::getFloatValue( tortuosityParameters,
                                                 "wave_length" );

    }
    else
    {

      _tortuosityMagnitude = 0.0f;
      _tortuosityAngularDispersionInDegrees = 0.0f;
      _tortuosityWaveLength = 0.0f;

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting mean orientation
    ////////////////////////////////////////////////////////////////////////////


    _meanOrientation = gkg::getVector3dOfFloatValue( dendriteParameters,
                                                     "mean_orientation" );


    ////////////////////////////////////////////////////////////////////////////
    // collecting volume fraction
    ////////////////////////////////////////////////////////////////////////////

    _volumeFraction = gkg::getFloatValue( parameters,
                                          "volume_fraction" );


    ////////////////////////////////////////////////////////////////////////////
    // computing the kappa factor of the Watson's distribution corresponding
    // to the current global angular dispersion
    ////////////////////////////////////////////////////////////////////////////

    gkg::NumericalAnalysisImplementationFactory* 
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    randomGenerator.setSeedWithTime();

    double kappa = -1.0;
    if ( _globalAngularDispersionInDegrees <= 60.0f )
    {

      kappa = factory->getWatsonKappaParameterFromAngularDispersion(
                                    ( double )_globalAngularDispersionInDegrees,
                                    true,    // in degrees
                                    1000,    // orientation count
                                    1.0 );   // epsilon
    
    }


    ////////////////////////////////////////////////////////////////////////////
    // computing the volume of the sphere containing the field of view
    ////////////////////////////////////////////////////////////////////////////

    const gkg::BoundingBox< float >& fieldOfView = _parent->getFieldOfView();
    // The 2 variables below are unused
    //float maximumFieldOfViewSpan =
    //                            std::max( fieldOfView.getSpanX(),
    //                                      std::max( fieldOfView.getSpanY(),
    //                                               fieldOfView.getSpanZ() ) );
    //float radiusOfSphereContainingFieldOfView = maximumFieldOfViewSpan * 
    //                                            ( float )std::sqrt( 3.0f ) /
    //                                            2.0f;
    float fieldOfViewVolume = fieldOfView.getSpanX() *
                              fieldOfView.getSpanY() *
                              fieldOfView.getSpanZ();
    float fieldOfViewLowerX = fieldOfView.getLowerX();
    float fieldOfViewLowerY = fieldOfView.getLowerY();
    float fieldOfViewLowerZ = fieldOfView.getLowerZ();
    float fieldOfViewUpperX = fieldOfView.getUpperX();
    float fieldOfViewUpperY = fieldOfView.getUpperY();
    float fieldOfViewUpperZ = fieldOfView.getUpperZ();


    ////////////////////////////////////////////////////////////////////////////
    // creating straight dispersed cylinders till we reach the target dendrite
    // volume fraction
    ////////////////////////////////////////////////////////////////////////////

    float dendriteDiameter = 0.0f;
    float dendriteLength = 0.0f;
    gkg::RandomPointOverSphereDistribution< float > 
      randomPointOverSphereDistribution( 1.0f, 1e3 );
    gkg::RCPointer< gkg::WatsonDispersedOrientationDistribution< float > >
      watsonDispersedOrientationDistribution;
    if ( kappa >= 0.0 )
    {

      watsonDispersedOrientationDistribution.reset(
        new gkg::WatsonDispersedOrientationDistribution< float >(
                                                           kappa,
                                                           _meanOrientation ) );

    }
    gkg::Vector3d< float > startingAtomCenter;
    gkg::Vector3d< float > endingAtomCenter;
    gkg::Vector3d< float > dendriteOrientation;

    std::list< gkg::RCPointer< gkg::Dendrite > > dendrites;

    int32_t dendriteId = 0;
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

      // computing a random dendrite diameter
      dendriteDiameter = _dendriteDiameterDistribution->getRandomValue();
      
      // computing a random dendrite length
      dendriteLength = _dendriteLengthDistribution->getRandomValue();
      
      // computing the starting point inside the FOV
      startingAtomCenter.x = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerX,
                                                            fieldOfViewUpperX );
      startingAtomCenter.y = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerY,
                                                            fieldOfViewUpperY );
      startingAtomCenter.z = ( float )factory->getUniformRandomNumber(
                                                            randomGenerator,
                                                            fieldOfViewLowerZ,
                                                            fieldOfViewUpperZ );
      // computing the dendrite orientation
      if ( kappa >= 0.0 )
      {

        // computing the dispersed dendrite orientation according to Watson's 
        // distribution
        watsonDispersedOrientationDistribution->getRandomOrientation( 
                                                          dendriteOrientation );

      }
      else
      {

        randomPointOverSphereDistribution.getRandomPoint( dendriteOrientation );

      }

      // computing the end point
      endingAtomCenter.x = startingAtomCenter.x + dendriteLength *
                           dendriteOrientation.x;
      endingAtomCenter.y = startingAtomCenter.y + dendriteLength *
                           dendriteOrientation.y;
      endingAtomCenter.z = startingAtomCenter.z + dendriteLength *
                           dendriteOrientation.z;

      // creating the dendrite
      gkg::RCPointer< gkg::Dendrite >
        dendrite( new gkg::Dendrite( dendriteId,
                                     this,
                                     startingAtomCenter,
                                     endingAtomCenter,
                                     _atomOversamplingRatio,
                                     dendriteDiameter / 2.0f,
                                     _hasTortuosity,
                                     _tortuosityMagnitude,
                                     _tortuosityAngularDispersionInDegrees,
                                     _tortuosityWaveLength ) );

      // only dendrites having at least 2 sphere atoms are added to the list 
      // of dendrites
      if ( dendrite->getAtomCount() > 1 )
      {

        dendrites.push_back( dendrite );
        totalVolume += dendrite->getVolume();

        ++ dendriteId;

      }

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

    // The variable below is unused
    //int32_t dendriteCount = dendriteId;


    ////////////////////////////////////////////////////////////////////////////
    // copying back dendrites to protected member(s)
    ////////////////////////////////////////////////////////////////////////////

    _dendrites = std::vector< gkg::RCPointer< gkg::Dendrite > >(
                                                              dendrites.begin(),
                                                              dendrites.end() );
    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( dendrites.begin(),
                                                         dendrites.end() );
    dendrites.clear();

  }
  GKG_CATCH( "gkg::DendritePopulation::DendritePopulation( "
             "int32_t id, "
             "const Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::DendritePopulation::DendritePopulation(
                        const gkg::DendritePopulation& other )
                        : gkg::Population( other ),
                          _atomOversamplingRatio(
                            other._atomOversamplingRatio ),
                          _dendriteDiameterDistribution(
                            other._dendriteDiameterDistribution ),
                          _dendriteLengthDistribution(
                            other._dendriteLengthDistribution ),
                          _globalAngularDispersionInDegrees(
                            other._globalAngularDispersionInDegrees ),
                          _hasTortuosity(
                            other._hasTortuosity ),
                          _tortuosityMagnitude(
                            other._tortuosityMagnitude ),
                          _tortuosityAngularDispersionInDegrees(
                            other._tortuosityAngularDispersionInDegrees ),
                          _tortuosityWaveLength(
                            other._tortuosityWaveLength ),
                          _meanOrientation(
                            other._meanOrientation ),
                          _volumeFraction(
                            other._volumeFraction ),
                          _dendrites(
                            other._dendrites )
{
}


gkg::DendritePopulation&gkg::DendritePopulation::operator=(
                                          const gkg::DendritePopulation& other )
{

  try
  {

    this->gkg::Population::operator=( other );
    _atomOversamplingRatio = other._atomOversamplingRatio;
    _dendriteDiameterDistribution = other._dendriteDiameterDistribution;
    _dendriteLengthDistribution = other._dendriteLengthDistribution;
    _globalAngularDispersionInDegrees = other._globalAngularDispersionInDegrees;
    _hasTortuosity = other._hasTortuosity;
    _tortuosityMagnitude = other._tortuosityMagnitude;
    _tortuosityAngularDispersionInDegrees =
                                    other._tortuosityAngularDispersionInDegrees;
    _tortuosityWaveLength = other._tortuosityWaveLength;
    _meanOrientation = other._meanOrientation;
    _volumeFraction = other._volumeFraction;
    _dendrites = other._dendrites;

    return *this;

  }
  GKG_CATCH( "gkg::DendritePopulation&gkg::DendritePopulation::operator=( "
             "const gkg::DendritePopulation& other )" );

}


gkg::DendritePopulation::~DendritePopulation()
{
}


std::string gkg::DendritePopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string gkg::DendritePopulation::getTypeName() const" );

}


std::string gkg::DendritePopulation::getStaticName()
{

  try
  {

    return "dendrite-population";

  }
  GKG_CATCH( "std::string gkg::DendritePopulation::getStaticName()" );

}


//
// adding the DendritePopulation creator to the population factory
//

RegisterPopulationCreator( gkg, DendritePopulation );


