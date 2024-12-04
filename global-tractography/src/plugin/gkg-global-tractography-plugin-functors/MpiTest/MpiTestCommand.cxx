#include <gkg-global-tractography-plugin-functors/MpiTest/MpiTestCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-core-exception/Exception.h>

#include <mpi/mpi.h>
#include <iostream>


gkg::MpiTestCommand::MpiTestCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MpiTestCommand::MpiTestCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MpiTestCommand::MpiTestCommand( const gkg::Dictionary& parameters )
                      : gkg::Command( parameters )
{

  try
  {
    
    execute();

  }
  GKG_CATCH( "gkg::MpiTestCommand::MpiTestCommand( "
             "const gkg::Dictionary& parameters )" );

}

gkg::MpiTestCommand::MpiTestCommand()
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::MpiTestCommand::MpiTestCommand()" );

}


gkg::MpiTestCommand::~MpiTestCommand()
{
}


std::string gkg::MpiTestCommand::getStaticName()
{

  try
  {

    return "MpiTest";

  }
  GKG_CATCH( "std::string gkg::MpiTestCommand::getStaticName()" );

}


void gkg::MpiTestCommand::parse()
{

  try
  {

    execute();

  }
  GKG_CATCH( "void gkg::MpiTestCommand::parse()" );

}


void gkg::MpiTestCommand::execute()
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
  GKG_CATCH( "void gkg::MpiTestCommand::execute()" );

}


RegisterCommandCreator( MpiTestCommand,
                        DECLARE_NO_PARAMETER_HELP );
