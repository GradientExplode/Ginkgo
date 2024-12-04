#include <gkg-simulation-fiber-population/FiberPopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomPointOverSphereDistribution.h>
#include <gkg-processing-numericalanalysis/WatsonDispersedOrientationDistribution.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-simulation-fiber-population/Fiber.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::FiberPopulation::FiberPopulation( int32_t id,
                                       const gkg::VirtualTissue* parent,
                                       const gkg::Dictionary& parameters,
                                       bool verbose,
                                       std::ofstream* osLog )
                     : gkg::Population( id, parent )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // collecting atom oversampling ratio
    ////////////////////////////////////////////////////////////////////////////

    _atomOversamplingRatio = gkg::getFloatValue( parameters,
                                                 "atom_oversampling_ratio",
                                                 false,
                                                 10.0f );
    this->_branchAtomOversamplingRatio = _atomOversamplingRatio;


    ////////////////////////////////////////////////////////////////////////////
    // collecting axon parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary axonParameters = gkg::getDictionaryValue( parameters,
                                                              "axon" );


    ////////////////////////////////////////////////////////////////////////////
    // building axon diameter distribution
    ////////////////////////////////////////////////////////////////////////////

    _axonDiameterDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                         gkg::getDictionaryValue( axonParameters,
                                                  "diameter_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting global angular dispersion in degrees
    ////////////////////////////////////////////////////////////////////////////

    _globalAngularDispersionInDegrees = gkg::getFloatValue(
                                       axonParameters,
                                       "global_angular_dispersion_in_degrees" );


    ////////////////////////////////////////////////////////////////////////////
    // do we generate tortuosity ?
    ////////////////////////////////////////////////////////////////////////////

    _hasTortuosity = ( gkg::getIntegerValue( axonParameters,
                                             "has_tortuosity",
                                             false,
                                             1 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting tortuosity information
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasTortuosity )
    {

      gkg::Dictionary tortuosityParameters = gkg::getDictionaryValue(
                                                                axonParameters,
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
    // do we generate beading ?
    ////////////////////////////////////////////////////////////////////////////

    _hasBeading = ( gkg::getIntegerValue( axonParameters,
                                          "has_beading",
                                          false,
                                          1 ) ? true : false );


    ////////////////////////////////////////////////////////////////////////////
    // collecting beading information
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasBeading )
    {

      gkg::Dictionary beadingParameters = gkg::getDictionaryValue(
                                                                axonParameters,
                                                                "beading" );
    
      _interbeadingLengthDistribution.reset(
        gkg::DistributionFactory::getInstance().create( 
              gkg::getDictionaryValue( beadingParameters,
                                       "interbeading_length_distribution" ) ) );

      _beadingWidthDistribution.reset(
        gkg::DistributionFactory::getInstance().create( 
              gkg::getDictionaryValue( beadingParameters,
                                       "width_distribution" ) ) );

      _beadingMagnitudeRatioDistribution.reset(
        gkg::DistributionFactory::getInstance().create( 
              gkg::getDictionaryValue( beadingParameters,
                                       "magnitude_ratio_distribution" ) ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // collecting mean orientation
    ////////////////////////////////////////////////////////////////////////////


    _meanOrientation = gkg::getVector3dOfFloatValue( axonParameters,
                                                     "mean_orientation" );


    ////////////////////////////////////////////////////////////////////////////
    // do we generate myelin sheath ?
    ////////////////////////////////////////////////////////////////////////////

    _hasMyelinSheath = ( gkg::getIntegerValue( parameters,
                                               "has_myelin_sheath",
                                               false,
                                               1 ) ? true : false );

    if ( _hasMyelinSheath )
    {

      gkg::Dictionary myelinSheathParameters = gkg::getDictionaryValue(
                                                              parameters,
                                                              "myelin_sheath" );

      _gRatioDistribution.reset(
        gkg::DistributionFactory::getInstance().create(
                          gkg::getDictionaryValue( myelinSheathParameters,
                                                   "g_ratio_distribution" ) ) );

      _hasRanvierNodes = ( gkg::getIntegerValue( parameters,
                                                 "has_ranvier_nodes",
                                                 false,
                                                 1 ) ? true : false );

      if ( _hasRanvierNodes )
      {

        _internodalLengthToNodeWidthDistribution.reset(
          gkg::DistributionFactory::getInstance().create( 
            gkg::getDictionaryValue(
                           myelinSheathParameters,
                           "internodal_length_to_node_width_distribution" ) ) );

      }



    }
    else
    {

      _hasRanvierNodes = false;

    }


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

    double 
    kappa = factory->getPrecomputedWatsonKappaParameterFromAngularDispersion(
                                    ( double )_globalAngularDispersionInDegrees,
                                    true );


    ////////////////////////////////////////////////////////////////////////////
    // computing the volume of the sphere containing the field of view
    ////////////////////////////////////////////////////////////////////////////

    const gkg::BoundingBox< float >& fieldOfView = _parent->getFieldOfView();

    float maximumFieldOfViewSpan =
                                std::max( fieldOfView.getSpanX(),
                                          std::max( fieldOfView.getSpanY(),
                                                    fieldOfView.getSpanZ() ) );
    float radiusOfSphereContainingFieldOfView = maximumFieldOfViewSpan * 
                                                ( float )std::sqrt( 3.0f ) /
                                                2.0f;
    float fieldOfViewVolume = fieldOfView.getSpanX() *
                              fieldOfView.getSpanY() *
                              fieldOfView.getSpanZ();


    ////////////////////////////////////////////////////////////////////////////
    // creating straight dispersed cylinders till we reach the target fiber
    // volume fraction
    ////////////////////////////////////////////////////////////////////////////

    float outerMyelinSheathDiameter = 0.0f;
    float axonDiameter = 0.0f;
    float gRatio = 0.0f;
    gkg::RandomPointOverSphereDistribution< float > 
      randomPointOverSphereDistribution( radiusOfSphereContainingFieldOfView );
    gkg::WatsonDispersedOrientationDistribution< float > 
      watsonDispersedOrientationDistribution( kappa, _meanOrientation );
    gkg::Vector3d< float > startingAtomCenter;
    gkg::Vector3d< float > fiberOrientation;
    float internodalLength = 0.0f;
    float internodalLengthToNodeWidthRatio = 0.0f;
    float ranvierNodeWidth = 0.0f;
    float ranvierNodeOffset = 0.0f;


    // reset the seed of the random generator included of the dispersed 
    // Watson' distribution of orientation
    watsonDispersedOrientationDistribution.setRandomSeedWithTime();

    std::list< gkg::RCPointer< gkg::Fiber > > fibers;


    int32_t fiberId = 0;
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

      // computing a random outer myelin sheath diameter being equal to the 
      // axon membrane diameter divided by the g-ratio (g=d/D)
      axonDiameter = _axonDiameterDistribution->getRandomValue();

      if ( _hasMyelinSheath )
      {

        gRatio = _gRatioDistribution->getRandomValue();
        outerMyelinSheathDiameter = axonDiameter / gRatio;

      }
      else
      {

        outerMyelinSheathDiameter = axonDiameter;

      }


      // computing the starting point over the sphere containing the FOV
      randomPointOverSphereDistribution.getRandomPoint( startingAtomCenter );


      // computing the dispersed fiber orientation according to Watson's 
      // distribution on  if the angular dispersion is greater or equal to 0
      // else setting it to the mean fiber population orientation thus 
      // considering the angular dispersion to be nul (due to the failure
      // of the method 
      // gkg::NumericalAnalysisImplementationFactory::getRandomWatsonOrientation
      // for angles lower than 2 degrees
      if ( kappa >= 700 )
      {

        fiberOrientation = _meanOrientation;

      }
      else
      {

        watsonDispersedOrientationDistribution.getRandomOrientation( 
                                                             fiberOrientation );

      }

      // make sure that the orientation is pointed towards sphere center
      if ( startingAtomCenter.dot( fiberOrientation ) > 0.0f )
      {

        fiberOrientation *= -1.0f;

      }

      if ( _hasMyelinSheath )
      {

        // computing the internodal length
        internodalLength = 15.0f * gRatio * axonDiameter *
                         ( float )std::sqrt( std::log( 1.0 /
                                                       ( double )gRatio ) );
        internodalLengthToNodeWidthRatio =
                    _internodalLengthToNodeWidthDistribution->getRandomValue();
        ranvierNodeWidth = 5.0f * internodalLength /
                           internodalLengthToNodeWidthRatio;
        ranvierNodeOffset = ( float )factory->getUniformRandomNumber(
                                           randomGenerator,
                                           0.0,
                                           ( double )internodalLength / 2.0 );

      }


      // creating a new straight cylinder which starting and ending points
      // are on the field of view boundaries
      gkg::RCPointer< gkg::Fiber >
        fiber( new gkg::Fiber( fiberId,
                               this,
                               fieldOfView,
                               startingAtomCenter,
                               radiusOfSphereContainingFieldOfView,
                               _atomOversamplingRatio,
                               fiberOrientation,
                               axonDiameter,
                               outerMyelinSheathDiameter,
                               internodalLength,
                               ranvierNodeWidth,
                               ranvierNodeOffset,
                               _hasTortuosity,
                               _hasBeading,
                               _hasMyelinSheath,
                               _hasRanvierNodes ) );

      // only fibers having at least 2 atoms are added to the list of
      // fibers
      if ( fiber->getAtomCount() > 1 )
      {

        fibers.push_back( fiber );
        totalVolume += fiber->getVolume();
        ++ fiberId;

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

    int32_t fiberCount = fiberId;


    ////////////////////////////////////////////////////////////////////////////
    // copying back fibers to protected member(s)
    ////////////////////////////////////////////////////////////////////////////

    _fibers = std::vector< gkg::RCPointer< gkg::Fiber > >( fibers.begin(),
                                                           fibers.end() );
    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( fibers.begin(),
                                                         fibers.end() );
    fibers.clear();


    ////////////////////////////////////////////////////////////////////////////
    // adding tortuosity if requested
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasTortuosity )
    {

      float inducedTortuosity = 0.0f;

      while ( inducedTortuosity <
              _tortuosityAngularDispersionInDegrees * fiberCount )
      {

        // selecting a random fiber
        int32_t selectedFiberIndex = 0;
        if ( fiberCount > 1 )
        {

          selectedFiberIndex = ( int32_t )factory->getUniformRandomUInt32(
                                       randomGenerator,
                                       ( uint32_t )( fiberCount /*- 1*/ ) );
          if ( selectedFiberIndex == fiberCount )
          {

            -- selectedFiberIndex;

          }

        }
        const gkg::RCPointer< gkg::Fiber >& 
          selectedFiber = _fibers[ selectedFiberIndex ];

        inducedTortuosity += selectedFiber->addTortuosity(
                                                          fieldOfView,
                                                          _tortuosityMagnitude,
                                                          _tortuosityWaveLength,
                                                          randomGenerator,
                                                          factory );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding beading if requested
    ////////////////////////////////////////////////////////////////////////////

    if ( _hasBeading )
    {

      gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

      std::vector< gkg::RCPointer< gkg::Fiber > >::iterator
        f = _fibers.begin(),
        fe = _fibers.end();
      while ( f != fe )
      {

        ( *f )->addBeading( _interbeadingLengthDistribution,
                            _beadingWidthDistribution,
                            _beadingMagnitudeRatioDistribution,
                            randomGenerator,
                            factory );
        ++ f;

      }

    }

  }
  GKG_CATCH( "gkg::FiberPopulation::FiberPopulation( "
             "int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::FiberPopulation::FiberPopulation( const gkg::FiberPopulation& other )
                     : gkg::Population( other ),
                       _atomOversamplingRatio(
                         other._atomOversamplingRatio ),
                       _axonDiameterDistribution(
                         other._axonDiameterDistribution ),
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
                       _hasBeading(
                         other._hasBeading ),
                       _interbeadingLengthDistribution(
                         other._interbeadingLengthDistribution ),
                       _beadingWidthDistribution(
                         other._beadingWidthDistribution ),
                       _beadingMagnitudeRatioDistribution(
                         other._beadingMagnitudeRatioDistribution ),
                       _meanOrientation(
                         other._meanOrientation ),
                       _hasMyelinSheath(
                         other._hasMyelinSheath ),
                       _gRatioDistribution(
                         other._gRatioDistribution ),
                       _hasRanvierNodes(
                         other._hasRanvierNodes ),
                       _internodalLengthToNodeWidthDistribution(
                         other._internodalLengthToNodeWidthDistribution ),
                       _volumeFraction(
                         other._volumeFraction ),
                       _fibers(
                         other._fibers )
{
}


gkg::FiberPopulation::~FiberPopulation()
{
}


gkg::FiberPopulation&
gkg::FiberPopulation::operator=( const gkg::FiberPopulation& other )
{

  try
  {

    this->gkg::Population::operator=( other );
    _atomOversamplingRatio = other._atomOversamplingRatio;
    _axonDiameterDistribution = other._axonDiameterDistribution;
    _globalAngularDispersionInDegrees = other._globalAngularDispersionInDegrees;
    _hasTortuosity = other._hasTortuosity;
    _tortuosityMagnitude = other._tortuosityMagnitude;
    _tortuosityAngularDispersionInDegrees = 
                                    other._tortuosityAngularDispersionInDegrees;
    _tortuosityWaveLength = other._tortuosityWaveLength;
    _hasBeading = other._hasBeading;
    _interbeadingLengthDistribution = other._interbeadingLengthDistribution;
    _beadingWidthDistribution = other._beadingWidthDistribution;
    _beadingMagnitudeRatioDistribution = 
                                       other._beadingMagnitudeRatioDistribution;
    _meanOrientation = other._meanOrientation;
    _hasMyelinSheath = other._hasMyelinSheath;
    _gRatioDistribution = other._gRatioDistribution;
    _hasRanvierNodes = other._hasRanvierNodes;
    _internodalLengthToNodeWidthDistribution = 
                                 other._internodalLengthToNodeWidthDistribution;
    _volumeFraction = other._volumeFraction;
    _fibers = other._fibers;

    return *this;

  }
  GKG_CATCH( "gkg::FiberPopulation&gkg::FiberPopulation::operator=( "
             "const gkg::FiberPopulation& other )" );

}


std::string gkg::FiberPopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string gkg::FiberPopulation::getTypeName() const" );

}


template < class L >
void gkg::FiberPopulation::getBundleMap( gkg::BundleMap< L >& bundleMap,
                                         const L& label ) const
{

  try
  {

    std::list< gkg::LightCurve3d< float > > lightCurve3ds;

    std::vector< gkg::RCPointer< gkg::Fiber > >::const_iterator
        f = this->_fibers.begin(),
        fe = this->_fibers.end();
    while ( f != fe )
    {

      lightCurve3ds.push_back( ( *f )->getLightCurve3d() );

      ++ f;

    }

    bundleMap.addCurve3ds( label, lightCurve3ds );

  }
  GKG_CATCH( "gkg::FiberPopulation::FiberPopulation( "
             "int32_t, const Dictionary& parameters )" );

}



std::string gkg::FiberPopulation::getStaticName()
{

  try
  {

    return "fiber-population";

  }
  GKG_CATCH( "std::string gkg::FiberPopulation::getStaticName()" );

}


//
// forcing template member instantiation
//

template void gkg::FiberPopulation::getBundleMap(
                                        gkg::BundleMap< std::string >& bundleMap,
                                        const std::string& label ) const;
template void gkg::FiberPopulation::getBundleMap(
                                        gkg::BundleMap< int16_t >& bundleMap,
                                        const int16_t& label ) const;


//
// adding the FiberPopulation creator to the population factory
//

RegisterPopulationCreator( gkg, FiberPopulation );


