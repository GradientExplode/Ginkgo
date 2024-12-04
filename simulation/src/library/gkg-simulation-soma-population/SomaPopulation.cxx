#include <gkg-simulation-soma-population/SomaPopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-simulation-soma-population/Soma.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>


gkg::SomaPopulation::SomaPopulation( int32_t id,
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
    float fieldOfViewVolume = 
                         ( fieldOfView.getUpperX() - fieldOfView.getLowerX() ) *
                         ( fieldOfView.getUpperY() - fieldOfView.getLowerY() ) *
                         ( fieldOfView.getUpperZ() - fieldOfView.getLowerZ() );
    float fieldOfViewLowerX = fieldOfView.getLowerX();
    float fieldOfViewUpperX = fieldOfView.getUpperX();
    float fieldOfViewLowerY = fieldOfView.getLowerY();
    float fieldOfViewUpperY = fieldOfView.getUpperY();
    float fieldOfViewLowerZ = fieldOfView.getLowerZ();
    float fieldOfViewUpperZ = fieldOfView.getUpperZ();


    ////////////////////////////////////////////////////////////////////////////
    // creating somas till we reach the target soma volume fraction
    ////////////////////////////////////////////////////////////////////////////

    float diameter = 0.0f;
    gkg::Vector3d< float > center;

    std::list< gkg::RCPointer< gkg::Soma > > somas;

    int32_t id = 0;
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
      diameter = _somaDiameterDistribution->getRandomValue();


      // randomly selecting the center of the soma inside the voxel
      center.x = ( float )factory->getUniformRandomNumber(
                                          randomGenerator,
                                          fieldOfViewLowerX,
                                          fieldOfViewUpperX );
      center.y = ( float )factory->getUniformRandomNumber(
                                          randomGenerator,
                                          fieldOfViewLowerY,
                                          fieldOfViewUpperY );
      center.z = ( float )factory->getUniformRandomNumber(
                                          randomGenerator,
                                          fieldOfViewLowerZ,
                                          fieldOfViewUpperZ );

      // creating a new soma
      gkg::RCPointer< gkg::Soma >
        soma( new gkg::Soma( id,
                             this,
                             diameter,
                             center ) );

      somas.push_back( soma );
      totalVolume += soma->getVolume();

      ++ id;

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


    ////////////////////////////////////////////////////////////////////////////
    // copying back somas to protected member(s)
    ////////////////////////////////////////////////////////////////////////////

    _somas = std::vector< gkg::RCPointer< gkg::Soma > >( somas.begin(),
                                                         somas.end() );
    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( somas.begin(),
                                                         somas.end() );
    somas.clear();


  }
  GKG_CATCH( "gkg::SomaPopulation::SomaPopulation( "
             "int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::SomaPopulation::SomaPopulation( const gkg::SomaPopulation& other )
                     : gkg::Population( other ),
                       _somaDiameterDistribution(
                         other._somaDiameterDistribution ),
                       _volumeFraction(
                         other._volumeFraction ),
                       _somas( other._somas )
{
}


gkg::SomaPopulation& gkg::SomaPopulation::operator=(
                                             const gkg::SomaPopulation& other )
{

  try
  {

    this->gkg::Population::operator=( other );
    _somaDiameterDistribution = other._somaDiameterDistribution;
    _volumeFraction = other._volumeFraction;
    _somas = other._somas;

    return *this;

  }
  GKG_CATCH( "gkg::SomaPopulation& gkg::SomaPopulation::operator=( "
             "const gkg::SomaPopulation& other )" );

}


gkg::SomaPopulation::~SomaPopulation()
{
}


std::string gkg::SomaPopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string gkg::SomaPopulation::getTypeName() const" );

}


std::string gkg::SomaPopulation::getStaticName()
{

  try
  {

    return "soma-population";

  }
  GKG_CATCH( "std::string gkg::SomaPopulation::getStaticName()" );

}


//
// adding the SomaPopulation creator to the population factory
//

RegisterPopulationCreator( gkg, SomaPopulation );


