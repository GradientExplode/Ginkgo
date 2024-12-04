#include <gkg-simulation-glia-population/AstrocytePopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-simulation-glia-population/Astrocyte.h>
#include <gkg-core-exception/Exception.h>


gkg::AstrocytePopulation::AstrocytePopulation(
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
    // building total diameter distribution
    ////////////////////////////////////////////////////////////////////////////

    _totalDiameterDistribution.reset(
      gkg::DistributionFactory::getInstance().create( 
                  gkg::getDictionaryValue( parameters,
                                           "total_diameter_distribution" ) ) );


    ////////////////////////////////////////////////////////////////////////////
    // collecting the number of astrocytes to be created and reallocating
    // the container accordingly
    ////////////////////////////////////////////////////////////////////////////

    int32_t astrocyteCount = ( int32_t )this->_somaAtoms.size();
    _astrocytes.resize( astrocyteCount );


    ////////////////////////////////////////////////////////////////////////////
    // creating the individual astrocytes from their soma and process 
    // information
    ////////////////////////////////////////////////////////////////////////////

    int32_t id = 0;
    float totalRadius = 0.0f;
    int32_t processNodeCount = 0;
    float processRadius = 0.0f;
    for ( id = 0; id < astrocyteCount; id++ )
    {

      totalRadius = _totalDiameterDistribution->getRandomValue() / 2.0f;
      processNodeCount = ( int32_t )_processNodeCountDistribution->
                                                               getRandomValue();
      processRadius = _processDiameterDistribution->getRandomValue() / 2.0f;

      // creating a new astrocyte
      _astrocytes[ id ].reset( new gkg::Astrocyte(
                                   id,
                                   this,
                                   _somaAtoms[ id ],
                                   totalRadius,
                                   processNodeCount,
                                   _processBalancingFactor,
                                   _processAtomOversamplingRatio,
                                   _processMinimumSegmentPointCount,
                                   processRadius,
                                   _processHasTortuosity,
                                   _processTortuosityMagnitude,
                                   _processTortuosityAngularDispersionInDegrees,
                                   _processTortuosityWaveLength ) );

    }


    ////////////////////////////////////////////////////////////////////////////
    // copying back astrocytes to protected member(s)
    ////////////////////////////////////////////////////////////////////////////

    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( _astrocytes.begin(),
                                                         _astrocytes.end() );

  }
  GKG_CATCH( "gkg::AstrocytePopulation::AstrocytePopulation( "
             "int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::AstrocytePopulation::AstrocytePopulation(
                                         const gkg::AstrocytePopulation& other )
                         : gkg::GliaPopulation( other ),
                           _totalDiameterDistribution(
                                             other._totalDiameterDistribution ),
                           _astrocytes( other._astrocytes )
{
}


gkg::AstrocytePopulation& gkg::AstrocytePopulation::operator=(
                                         const gkg::AstrocytePopulation& other )
{

  try
  {

    this->gkg::GliaPopulation::operator=( other );

    _totalDiameterDistribution = other._totalDiameterDistribution;
    _astrocytes = other._astrocytes;

    return *this;

  }
  GKG_CATCH( "gkg::AstrocytePopulation& "
             "gkg::AstrocytePopulation::operator=( "
             "const gkg::AstrocytePopulation& other )" );

}


gkg::AstrocytePopulation::~AstrocytePopulation()
{
}


std::string gkg::AstrocytePopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string gkg::AstrocytePopulation::getTypeName() const" );

}


std::string gkg::AstrocytePopulation::getStaticName()
{

  try
  {

    return "astrocyte-population";

  }
  GKG_CATCH( "std::string gkg::AstrocytePopulation::getStaticName()" );

}


//
// adding the AstrocytePopulation creator to the population factory
//

RegisterPopulationCreator( gkg, AstrocytePopulation );


