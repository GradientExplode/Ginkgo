#include <gkg-simulation-glia-population/OligodendrocytePopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-virtual-tissue/PopulationAtomLutContainer.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-simulation-glia-population/Oligodendrocyte.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-exception/Exception.h>


gkg::OligodendrocytePopulation::OligodendrocytePopulation(
                                             int32_t id,
                                             const gkg::VirtualTissue* parent,
                                             const gkg::Dictionary& parameters,
                                             bool verbose,
                                             std::ofstream* osLog )
                               : gkg::GliaPopulation( id, parent, parameters,
                                                      verbose, osLog )
{

  try
  {


    ////////////////////////////////////////////////////////////////////////////
    // building search radius distribution
    ////////////////////////////////////////////////////////////////////////////

    _myelinatedFiberSearchRadiusDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
        gkg::getDictionaryValue(
                            parameters,
                            "myelinated_fiber_search_radius_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting the number of oligodendrocytes to be created and reallocating
    // the container accordingly
    ////////////////////////////////////////////////////////////////////////////

    int32_t oligodendrocyteCount = ( int32_t )this->_somaAtoms.size();
    _oligodendrocytes.resize( oligodendrocyteCount );


    ////////////////////////////////////////////////////////////////////////////
    // creating the individual oligodendrocytes from their soma and process 
    // information
    ////////////////////////////////////////////////////////////////////////////

    int32_t id = 0;
    float myelinatedFiberSearchRadius = 0.0f;
    int32_t processNodeCount = 0;
    float processRadius = 0.0f;
    for ( id = 0; id < oligodendrocyteCount; id++ )
    {

      myelinatedFiberSearchRadius =
                     _myelinatedFiberSearchRadiusDistribution->getRandomValue();
      processNodeCount = ( int32_t )_processNodeCountDistribution->
                                                               getRandomValue();
      processRadius = _processDiameterDistribution->getRandomValue() / 2.0f;

      std::cout << "____________________________________" << std::endl;
      std::cout << "myelinatedFiberSearchRadius=" << myelinatedFiberSearchRadius
                << std::endl;
      std::cout << "processNodeCount=" << processNodeCount
                << std::endl;
      std::cout << "processRadius=" << processRadius
                << std::endl;
      std::cout << "_processAtomOversamplingRatio="
                << _processAtomOversamplingRatio
                << std::endl;
      std::cout << "_processMinimumSegmentPointCount="
                << _processMinimumSegmentPointCount
                << std::endl;
      std::cout << "_processHasTortuosity="
                << _processHasTortuosity
                << std::endl;
      std::cout << "_processTortuosityMagnitude="
                << _processTortuosityMagnitude
                << std::endl;
      std::cout << "_processTortuosityAngularDispersionInDegrees="
                << _processTortuosityAngularDispersionInDegrees
                << std::endl;
      std::cout << "_processTortuosityWaveLength="
                << _processTortuosityWaveLength
                << std::endl;

      // creating a new oligodendrocyte
      _oligodendrocytes[ id ].reset(
         new gkg::Oligodendrocyte( id,
                                   this,
                                   _somaAtoms[ id ],
                                   myelinatedFiberSearchRadius,
                                   processNodeCount,
                                   _processAtomOversamplingRatio,
                                   _processMinimumSegmentPointCount,
                                   processRadius,
                                   _processHasTortuosity,
                                   _processTortuosityMagnitude,
                                   _processTortuosityAngularDispersionInDegrees,
                                   _processTortuosityWaveLength ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // copying back oligodendrocytes to protected member(s)
    ////////////////////////////////////////////////////////////////////////////

    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( 
                                                      _oligodendrocytes.begin(),
                                                      _oligodendrocytes.end() );

  }
  GKG_CATCH( "gkg::OligodendrocytePopulation::OligodendrocytePopulation( "
             "int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::OligodendrocytePopulation::OligodendrocytePopulation(
                                   const gkg::OligodendrocytePopulation& other )
                               : gkg::GliaPopulation( other ),
                                 _myelinatedFiberSearchRadiusDistribution(
                                   other.
                                     _myelinatedFiberSearchRadiusDistribution ),
                                 _oligodendrocytes( other._oligodendrocytes )
{
}


gkg::OligodendrocytePopulation& gkg::OligodendrocytePopulation::operator=(
                                   const gkg::OligodendrocytePopulation& other )
{

  try
  {

    this->gkg::GliaPopulation::operator=( other );

    _myelinatedFiberSearchRadiusDistribution =
                                 other._myelinatedFiberSearchRadiusDistribution;
    _oligodendrocytes = other._oligodendrocytes;

    return *this;

  }
  GKG_CATCH( "gkg::OligodendrocytePopulation& "
             "gkg::OligodendrocytePopulation::operator=( "
             "const gkg::OligodendrocytePopulation& other )" );

}


gkg::OligodendrocytePopulation::~OligodendrocytePopulation()
{
}


std::string gkg::OligodendrocytePopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string "
             "gkg::OligodendrocytePopulation::getTypeName() const" );

}


void gkg::OligodendrocytePopulation::addInteractionWithOtherPopulations()
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting the fiber population atom center LUT
    ////////////////////////////////////////////////////////////////////////////

    std::set< std::string > populationTypes;
    populationTypes.insert( "fiber-population" );

    _fiberPopulationAtomCenterLut =
                  gkg::PopulationAtomLutContainer::getInstance().
                                  getPopulationAtomCenterLut( *_parent,
                                                              populationTypes );


    ////////////////////////////////////////////////////////////////////////////
    // adding interaction with fiber population(s) for each oligodendrocyte
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::RCPointer< gkg::Oligodendrocyte > >::iterator
      o = _oligodendrocytes.begin(),
      oe = _oligodendrocytes.end();
    while ( o != oe )
    {

      ( *o )->addInteractionWithOtherPopulations();
      ++ o;

    }

  }
  GKG_CATCH( "void "
             "gkg::OligodendrocytePopulation::"
             "addInteractionWithOtherPopulations()" );


}


const gkg::RCPointer< gkg::Volume< 
                      gkg::RCPointer< std::list< gkg::Vector3d< float > > > > >&
gkg::OligodendrocytePopulation::getFiberPopulationAtomCenterLut() const
{

  try
  {

    return _fiberPopulationAtomCenterLut;

  }
  GKG_CATCH( "const gkg::RCPointer< gkg::Volume< gkg::RCPointer< std::list< "
             "gkg::Vector3d< float > > > > >& "
             "gkg::OligodendrocytePopulation::"
             "getFiberPopulationAtomCenterLut() const" );

}


bool gkg::OligodendrocytePopulation::
                              getRandomSelectionOfClosestFiberAtomCenters(
                 const gkg::Vector3d< float >& somaCenter,
                 float searchRadius,
                 std::vector< gkg::Vector3d< float > >& fiberAtomCenters ) const
{

  try
  {

    // pointing to the field of view and grid resolution of the virtual tissue
    const gkg::BoundingBox< float >& fieldOfView = _parent->getFieldOfView();
    float gridResolution = _parent->getGridResolution();

    // computing the translation offset
    gkg::Vector3d< float > translation( -fieldOfView.getLowerX(),
                                        -fieldOfView.getLowerY(),
                                        -fieldOfView.getLowerZ() );

    // computnig the neighborhood size within which selecting fiber
    // atom(s)
    int32_t neighborhoodSize = ( int32_t )( searchRadius / gridResolution );

    if ( ( ( float )neighborhoodSize * gridResolution ) < searchRadius )
    {

      ++ neighborhoodSize;

    }

    // computing the translated soma center and the corresponding LUT voxel
    gkg::Vector3d< float > translatedSomaCenter = somaCenter + translation;
    gkg::Vector3d< int32_t > 
      somaCenterVoxel( ( int32_t )( translatedSomaCenter.x / gridResolution ),
                       ( int32_t )( translatedSomaCenter.y / gridResolution ),
                       ( int32_t )( translatedSomaCenter.z / gridResolution ) );

    int32_t lowerX = std::max( somaCenterVoxel.x - neighborhoodSize, 0 );
    int32_t upperX = std::min(
                          somaCenterVoxel.x + neighborhoodSize,
                          _fiberPopulationAtomCenterLut->getSizeX() - 1 );
    int32_t lowerY = std::max( somaCenterVoxel.y - neighborhoodSize, 0 );
    int32_t upperY = std::min(
                          somaCenterVoxel.y + neighborhoodSize,
                          _fiberPopulationAtomCenterLut->getSizeY() - 1 );
    int32_t lowerZ = std::max( somaCenterVoxel.z - neighborhoodSize, 0 );
    int32_t upperZ = std::min(
                          somaCenterVoxel.z + neighborhoodSize,
                          _fiberPopulationAtomCenterLut->getSizeZ() - 1 );

    // looping over neighbors and collecting all the fiber atom center()
    std::list< gkg::Vector3d< float > > allFiberAtomCenterList;
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    for ( z = lowerZ; z <= upperZ; z++ )
    {

      for ( y = lowerY; y <= upperY; y++ )
      {

        for ( x = lowerX; x <= upperX; x++ )
        {

          if ( !( *_fiberPopulationAtomCenterLut )( x, y, z ).isNull() )
          {

            allFiberAtomCenterList.insert(
                   allFiberAtomCenterList.end(),
                   ( *_fiberPopulationAtomCenterLut )( x, y, z )->begin(),
                   ( *_fiberPopulationAtomCenterLut )( x, y, z )->end() );

          }

        }

      }

    }
    std::vector< gkg::Vector3d< float > > allFiberAtomCenters(
                                           allFiberAtomCenterList.begin(),
                                           allFiberAtomCenterList.end() );

    // selecting a sub-set of all the fiber atom center(s)
    gkg::NumericalAnalysisImplementationFactory* factory =
     gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );

    int32_t allFiberAtomCenterCount = ( int32_t )allFiberAtomCenters.size();

    int32_t selectedFiberAtomCenterCount = ( int32_t )fiberAtomCenters.size();

    if ( allFiberAtomCenterCount >= selectedFiberAtomCenterCount )
    {

      int32_t s = 0;
      for ( s = 0; s < selectedFiberAtomCenterCount; s++ )
      {

        fiberAtomCenters[ s ] = allFiberAtomCenters[
                                    ( int32_t )factory->getUniformRandomUInt32(
                                            randomGenerator,
                                            allFiberAtomCenterCount ) ];

      }

      return true;

    }
    return false;
 
  }
  GKG_CATCH( "bool gkg::OligodendrocytePopulation::"
             "getRandomSelectionOfClosestFiberAtomCenters( "
             "const gkg::Vector3d< float >& somaCenter, "
             "std::vector< gkg::Vector3d< float > >& fiberAtomCenters ) "
             "const" );

}


std::string gkg::OligodendrocytePopulation::getStaticName()
{

  try
  {

    return "oligodendrocyte-population";

  }
  GKG_CATCH( "std::string gkg::OligodendrocytePopulation::getStaticName()" );

}


//
// adding the OligodendrocytePopulation creator to the population factory
//

RegisterPopulationCreator( gkg, OligodendrocytePopulation );


