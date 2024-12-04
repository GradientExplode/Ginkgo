#include <gkg-global-tractography-plugin-functors/MpiLaunch/MpiLaunchCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-core-exception/Exception.h>

#include <mpi/mpi.h>
#include <iostream>


gkg::MpiLaunchCommand::MpiLaunchCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin,
                             bool removeFirst )
                 : gkg::Command( argc, argv,
                                 loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MpiLaunchCommand::MpiLaunchCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MpiLaunchCommand::MpiLaunchCommand( const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {
    
    execute();

  }
  GKG_CATCH( "gkg::MpiLaunchCommand::MpiLaunchCommand( "
             "const gkg::Dictionary& parameters )" );

}

gkg::MpiLaunchCommand::MpiLaunchCommand()
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::MpiLaunchCommand::MpiLaunchCommand()" );

}


gkg::MpiLaunchCommand::~MpiLaunchCommand()
{
}


std::string gkg::MpiLaunchCommand::getStaticName()
{

  try
  {

    return "MpiLaunch";

  }
  GKG_CATCH( "std::string gkg::MpiLaunchCommand::getStaticName()" );

}


void gkg::MpiLaunchCommand::parse()
{

  try
  {

    execute();

  }
  GKG_CATCH( "void gkg::MpiLaunchCommand::parse()" );

}


void gkg::MpiLaunchCommand::execute()
{

  try
  {

    int32_t currentMpiProcessId;
    int32_t mpiProcessesCount;

    // initialize the mpi environnement
    MPI_Init( &_argc, &_argv );

    // get the total nb of procceses in the MPI_COMM_WORLD
    MPI_Comm_size( MPI_COMM_WORLD, &mpiProcessesCount );
    // get the ID of the current process
    MPI_Comm_rank( MPI_COMM_WORLD, &currentMpiProcessId );

    std::cout << "I am process " << currentMpiProcessId
              << "/" << mpiProcessesCount << std::endl;

    // block until all process in the MPI_COMM_WORLD have reached the barrier
    MPI_Barrier( MPI_COMM_WORLD );

    // terminates the MPI execution environment
    MPI_Finalize();

  }
  GKG_CATCH( "void gkg::MpiLaunchCommand::execute()" );

}


RegisterCommandCreator( MpiLaunchCommand,
                        DECLARE_NO_PARAMETER_HELP );
