#include <gkg-dmri-plugin-functors/DwiNodePaving/DwiNodePavingCommand.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-mask/MaskPartitioner.h>
#include <gkg-processing-morphology/Neighborhood3d.h>

#include <iostream>
#include <list>

#include <gkg-processing-numericalanalysis/RandomGenerator.h>



gkg::DwiNodePavingCommand::DwiNodePavingCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin,
                                   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, 
                                          removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiNodePavingCommand::DwiNodePavingCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiNodePavingCommand::DwiNodePavingCommand(
                                        const std::string& fileNameMask,
                                        const std::string& fileNameOutput,
                                        std::string neighborhoodStrategy,
                                        float neighborhoodDistance,
                                        int32_t targetPartCount,
                                        float loadImbalanceTolerance,
                                        int32_t runCount,
                                        bool totalCommunicationMinimization,
                                        bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( fileNameMask,
             fileNameOutput,
             neighborhoodStrategy,
             neighborhoodDistance,
             targetPartCount,
             loadImbalanceTolerance,
             runCount,
             totalCommunicationMinimization,
             verbose );

  }
  GKG_CATCH( "gkg::DwiNodePavingCommand::DwiNodePavingCommand( "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "std::string neighborhoodStrategy, "
             "float neighborhoodDistance, "
             "int32_t targetPartCount, "
             "float loadImbalanceTolerance, "
             "int32_t runCount, "
             "bool totalCommunicationMinimization, "
             "bool verbose )" );

}


gkg::DwiNodePavingCommand::DwiNodePavingCommand(
                                            const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_STRING_PARAMETER( parameters, std::string, neighborhoodStrategy );
    DECLARE_FLOATING_PARAMETER( parameters, float, neighborhoodDistance );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, targetPartCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, loadImbalanceTolerance );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, runCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool,
                               totalCommunicationMinimization );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameMask,
             fileNameOutput,
             neighborhoodStrategy,
             neighborhoodDistance,
             targetPartCount,
             loadImbalanceTolerance,
             runCount,
             totalCommunicationMinimization,
             verbose );

  }
  GKG_CATCH( "gkg::DwiNodePavingCommand::DwiNodePavingCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiNodePavingCommand::~DwiNodePavingCommand()
{
}


std::string gkg::DwiNodePavingCommand::getStaticName()
{

  try
  {

    return "DwiNodePaving";

  }
  GKG_CATCH( "std::string gkg::DwiNodePavingCommand::getStaticName()" );

}


void gkg::DwiNodePavingCommand::parse()
{

  try
  {

    std::string fileNameMask;
    std::string fileNameOutput;
    std::string neighborhoodStrategy = "spin_distance";
    float neighborhoodDistance = 0.0;
    int32_t targetPartCount;
    float loadImbalanceTolerance = 1.001;
    int32_t runCount = 100;
    bool totalCommunicationMinimization = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI nodes paving for "
                                                "parallelized Global "
                                                "Tractography",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input mask file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output paved volume file name",
                                 fileNameOutput );
    application.addSingleOption( "-ns",
                                 "Neighborhood mapping strategy among 'sphere',"
                                 " 'cube' or 'spin_distance"
                                 " (default=spin_distance)",
                                 neighborhoodStrategy,
                                 true );
    application.addSingleOption( "-d",
                                 "Neighborhood distance in mm to build the "
                                 "neighborhood, if 0 is given then the "
                                 "mask resolution in x is chosen"
                                 " (default=0.0)",
                                 neighborhoodDistance,
                                 true );
    application.addSingleOption( "-p",
                                 "Target part count in in the mask",
                                 targetPartCount );
    application.addSingleOption( "-t",
                                 "Allowed load imbalance tolerance"
                                 " (default=1.001)",
                                 loadImbalanceTolerance,
                                 true );
    application.addSingleOption( "-N",
                                 "Number of runs of METIS"
                                 " (default=100)",
                                 runCount,
                                 true );
    application.addSingleOption( "-comMin",
                                 "Cost to minimize :\n"
                                 " - true : minimize the total communication "
                                 "volume, it may need more time to compute the "
                                 "partition,\n"
                                 " - false : minimize the edge cuts number\n"
                                 "(default=false)",
                                 totalCommunicationMinimization,
                                 true );
    application.addSingleOption( "-v",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameMask,
             fileNameOutput,
             neighborhoodStrategy,
             neighborhoodDistance,
             targetPartCount,
             loadImbalanceTolerance,
             runCount,
             totalCommunicationMinimization,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiNodePavingCommand::parse()" );

}


void gkg::DwiNodePavingCommand::execute( const std::string& fileNameMask,
                                         const std::string& fileNameOutput,
                                         std::string neighborhoodStrategy,
                                         float neighborhoodDistance,
                                         int32_t targetPartCount,
                                         float loadImbalanceTolerance,
                                         int32_t runCount,
                                         bool totalCommunicationMinimization,
                                         bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    gkg::Neighborhood3d::Type neighborhoodType;

    if ( neighborhoodStrategy == "spin_distance")
    {

      neighborhoodType =
                gkg::Neighborhood3d::Neighborhood3d_DistanceFromVoxelBoundaries;
    }
    else if ( neighborhoodStrategy == "sphere" )
    {

      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_Sphere;

    }
    else if ( neighborhoodStrategy == "cube" )
    {

      neighborhoodType = gkg::Neighborhood3d::Neighborhood3d_Cube;

    }
    else
    {

      throw std::runtime_error( "wrong neighborhoodStrategy parameter");

    }


    if ( neighborhoodDistance < 0 )
    {

      throw std::runtime_error( "wrong neighborhoodDistance argument (<0)");

    }

    if ( runCount <= 0 )
    {

      throw std::runtime_error( "run count should be > 0" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading computing mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }

    gkg::Volume< int16_t > mask;
    gkg::Reader::getInstance().read( fileNameMask, mask );

    gkg::Vector3d< double > maskResolution;
    mask.getResolution( maskResolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building neighborhood
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "bulding neighborhood : " << std::flush;

    }

    std::list< Vector3d< int32_t > > neighborhoodOffsets =
                gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                      neighborhoodType,
                                      1,
                                      neighborhoodDistance / maskResolution.x );

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "  neighborhood pattern size "
                << neighborhoodOffsets.size()
                << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // metis graph construction
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "METIS graph construction : " << std::flush;

    }

    gkg::MaskPartitioner< int16_t, int16_t > maskPartitioner(
                                                    neighborhoodOffsets,
                                                    mask );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // metis partitioning
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "METIS partitioning : " << std::flush;

    }

    gkg::Volume< int16_t > pavedVolume;

    int32_t startingRandomSeed = 0;
    gkg::MaskPartitioner< int16_t, int16_t >::PartitionStatistics
      partitionStatistics = maskPartitioner.computePartition(
                                                pavedVolume,
                                                loadImbalanceTolerance,
                                                targetPartCount,
                                                runCount,
                                                totalCommunicationMinimization,
                                                startingRandomSeed );


    if ( verbose )
    {

      std::cout << "done" << std::endl;
      std::cout << "  ratio " << partitionStatistics.imbalance << std::endl;
      std::cout << "  cost  " << partitionStatistics.cost << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing output paved volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing output volume : " << std::flush;
    }

    gkg::Writer::getInstance().write( fileNameOutput, pavedVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiNodePavingCommand::execute( "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "std::string neighborhoodStrategy, "
             "float neighborhoodDistance, "
             "const int32_t& partsToBePartitioned, "
             "const float& loadImbalanceTolerance, "
             "int32_t runCount, "
             "bool totalCommunicationMinimization, "
             "bool verbose )" );

}



RegisterCommandCreator(
    DwiNodePavingCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_STRING_PARAMETER_HELP( neighborhoodStrategy ) +
    DECLARE_FLOATING_PARAMETER_HELP( neighborhoodDistance ) +
    DECLARE_INTEGER_PARAMETER_HELP( targetPartCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( loadImbalanceTolerance ) +
    DECLARE_INTEGER_PARAMETER_HELP( runCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( totalCommunicationMinimization ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

