#include <gkg-simulation-neuron-population/NeuronPopulation.h>
#include <gkg-simulation-virtual-tissue/PopulationFactory.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-distribution/DistributionFactory.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>
#include <iomanip>



std::map< std::string,
              std::map< std::string,
                        std::vector< gkg::VirtualTissue > > >
gkg::NeuronPopulation::_neuronModels;

std::map< std::string,
              std::map< std::string,
                        std::vector< float > > >
gkg::NeuronPopulation::_neuronModelDiameters;

std::map< std::string,
              std::map< std::string,
                        std::vector< gkg::Vector3d< float > > > >
gkg::NeuronPopulation::_neuronModelCenters;

std::map< std::string,
              std::map< std::string,
                        std::vector< float > > >
gkg::NeuronPopulation::_neuronModelVolumes;



gkg::NeuronPopulation::NeuronPopulation( int32_t id,
                                         const gkg::VirtualTissue* parent,
                                         const gkg::Dictionary& parameters,
                                         bool verbose,
                                         std::ofstream* osLog )
                      : gkg::Population( id, parent )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // collecting neuron database
    ////////////////////////////////////////////////////////////////////////////

    _neuronDatabaseName = gkg::getStringValue( parameters, "database" );


    ////////////////////////////////////////////////////////////////////////////
    // collecting neuron type
    ////////////////////////////////////////////////////////////////////////////

    _neuronTypeName = gkg::getStringValue( parameters, "type" );


    ////////////////////////////////////////////////////////////////////////////
    // loading the neuron model database if not yet loaded
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string,
              std::map< std::string,
                        std::vector< gkg::VirtualTissue > > >::const_iterator
      nm = _neuronModels.find( _neuronDatabaseName );
    if ( nm == _neuronModels.end() )
    {

      loadNeuronModels( _neuronDatabaseName, _neuronTypeName );

    }
    else
    {

      std::map< std::string,
                std::vector< gkg::VirtualTissue > >::const_iterator
        nt = nm->second.find( _neuronTypeName );
      if ( nt == nm->second.end() )
      {

        loadNeuronModels( _neuronDatabaseName, _neuronTypeName );

      }

    }
    const std::vector< gkg::VirtualTissue >& 
      neuronModels = _neuronModels[ _neuronDatabaseName ][ _neuronTypeName ];
    const std::vector< float >& 
      neuronModelDiameters = _neuronModelDiameters[ _neuronDatabaseName ]
                                                   [ _neuronTypeName ];
    const std::vector< gkg::Vector3d< float > >& 
      neuronModelCenters = _neuronModelCenters[ _neuronDatabaseName ]
                                              [ _neuronTypeName ];
    const std::vector< float >& 
      neuronModelVolumes = _neuronModelVolumes[ _neuronDatabaseName ]
                                              [ _neuronTypeName ];


    ////////////////////////////////////////////////////////////////////////////
    // collecting neuron parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary neuronParameters = gkg::getDictionaryValue( parameters,
                                                                "neuron" );


    ////////////////////////////////////////////////////////////////////////////
    // collecting soma parameters
    ////////////////////////////////////////////////////////////////////////////

    gkg::Dictionary somaParameters = gkg::getDictionaryValue( neuronParameters,
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

    std::list< gkg::RCPointer< gkg::Neuron > > neurons;

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

      // randomly selecting a neuron model
      uint32_t randomNeuronModelIndex = factory->getUniformRandomUInt32(
                                     randomGenerator,
                                     ( uint32_t )( neuronModels.size() - 1U ) );

      gkg::Vector3d< float >
        randomRotationParameters( 
            factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ),
            factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ),
            factory->getUniformRandomNumber( randomGenerator, 0.0, 2 * M_PI ) );

      // creating a new soma
      gkg::RCPointer< gkg::Neuron >
        neuron( new gkg::Neuron( id,
                                 this,
                                 neuronModels[ randomNeuronModelIndex ],
                                 neuronModelDiameters[ randomNeuronModelIndex ],
                                 neuronModelCenters[ randomNeuronModelIndex ],
                                 neuronModelVolumes[ randomNeuronModelIndex ],
                                 randomRotationParameters,
                                 diameter,
                                 center ) );

      neurons.push_back( neuron );
      totalVolume += neuron->getVolume();

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

    _neurons = std::vector< gkg::RCPointer< gkg::Neuron > >( neurons.begin(),
                                                             neurons.end() );
    _cells = std::vector< gkg::RCPointer< gkg::Cell > >( neurons.begin(),
                                                         neurons.end() );
    neurons.clear();


  }
  GKG_CATCH( "gkg::NeuronPopulation::NeuronPopulation( "
             "int32_t id, "
             "const gkg::VirtualTissue* parent, "
             "const gkg::Dictionary& parameters, "
             "bool verbose, "
             "std::ofstream* osLog )" );

}


gkg::NeuronPopulation::NeuronPopulation( const gkg::NeuronPopulation& other )
                      : gkg::Population( other ),
                        _neuronDatabaseName(
                          other._neuronDatabaseName ),
                        _neuronTypeName(
                          other._neuronTypeName ),
                        _somaDiameterDistribution(
                          other._somaDiameterDistribution ),
                        _volumeFraction(
                          other._volumeFraction ),
                        _neurons( other._neurons )
{
}


gkg::NeuronPopulation& gkg::NeuronPopulation::operator=(
                                            const gkg::NeuronPopulation& other )
{

  try
  {

    this->gkg::Population::operator=( other );
    _neuronDatabaseName = other._neuronDatabaseName;
    _neuronTypeName = other._neuronTypeName;
    _somaDiameterDistribution = other._somaDiameterDistribution;
    _volumeFraction = other._volumeFraction;
    _neurons = other._neurons;

    return *this;

  }
  GKG_CATCH( "gkg::NeuronPopulation& gkg::NeuronPopulation::operator=( "
             "const gkg::NeuronPopulation& other )" );

}


gkg::NeuronPopulation::~NeuronPopulation()
{
}


std::string gkg::NeuronPopulation::getTypeName() const
{

  try
  {

    return getStaticName();

  }
  GKG_CATCH( "std::string gkg::NeuronPopulation::getTypeName() const" );

}


std::string gkg::NeuronPopulation::getStaticName()
{

  try
  {

    return "neuron-population";

  }
  GKG_CATCH( "std::string gkg::NeuronPopulation::getStaticName()" );

}


void gkg::NeuronPopulation::loadNeuronModels(
                                          const std::string& neuronDatabaseName,
                                          const std::string& neuronTypeName )
{

  try
  {

    // collecting all neuron model path(s) for current neuron type
    std::set< std::string > neuronModelPaths = 
      gkg::ConfigurationInfo::getInstance().getMedusaNeuronModelPaths(
                                                             neuronDatabaseName,
                                                             neuronTypeName );
    uint32_t neuronModelCountForCurrentType =
                                            ( uint32_t )neuronModelPaths.size();

    _neuronModels[ neuronDatabaseName ][ neuronTypeName ].resize( 
                                               neuronModelCountForCurrentType ); 
    _neuronModelDiameters[ neuronDatabaseName ][ neuronTypeName ].resize( 
                                               neuronModelCountForCurrentType ); 
    _neuronModelCenters[ neuronDatabaseName ][ neuronTypeName ].resize( 
                                               neuronModelCountForCurrentType ); 
    _neuronModelVolumes[ neuronDatabaseName ][ neuronTypeName ].resize( 
                                               neuronModelCountForCurrentType ); 

    std::vector< gkg::VirtualTissue >&
      neuronModels = _neuronModels[ neuronDatabaseName ]
                                  [ neuronTypeName ];
    std::vector< float >&
      neuronModelDiameters = _neuronModelDiameters[ neuronDatabaseName ]
                                                  [ neuronTypeName ];
    std::vector< gkg::Vector3d< float > >&
      neuronModelCenters = _neuronModelCenters[ neuronDatabaseName ]
                                              [ neuronTypeName ];
    std::vector< float >&
      neuronModelVolumes = _neuronModelVolumes[ neuronDatabaseName ]
                                              [ neuronTypeName ];

    std::set< std::string >::const_iterator
      p = neuronModelPaths.begin();
    uint32_t neuronModelIndex = 0;
    for ( neuronModelIndex = 0;
          neuronModelIndex < neuronModelCountForCurrentType;
          neuronModelIndex++ )
    {

      // reading the neuron model
      gkg::VirtualTissue& neuronModel = neuronModels[ neuronModelIndex ];
      gkg::Reader::getInstance().read( *p, neuronModel );

      // extracting its diameter
      neuronModelDiameters[ neuronModelIndex ] =
        gkg::getFloatValue( neuronModel.getHeader().getTypedValue(),
                            "neuron_model_diameter" );

      // extracting its center
      neuronModelCenters[ neuronModelIndex ] =
        gkg::getVector3dOfFloatValue( neuronModel.getHeader().getTypedValue(),
                                      "neuron_model_center" );

      // extracting its volume
      neuronModelVolumes[ neuronModelIndex ] =
        gkg::getFloatValue( neuronModel.getHeader().getTypedValue(),
                            "neuron_model_volume" );

      ++ p;

    }

  }
  GKG_CATCH( "void gkg::NeuronPopulation::loadNeuronModels( "
             "const std::string& neuronDatabaseName, "
             "const std::string& neuronTypeName )" );

}


//
// adding the NeuronPopulation creator to the population factory
//

RegisterPopulationCreator( gkg, NeuronPopulation );


