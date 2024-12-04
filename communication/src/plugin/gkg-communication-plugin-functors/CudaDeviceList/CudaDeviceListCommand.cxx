#include <gkg-communication-plugin-functors/CudaDeviceList/CudaDeviceListCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-cuda/CudaDevices.h>
#include <gkg-core-exception/Exception.h>


gkg::CudaDeviceListCommand::CudaDeviceListCommand( int32_t argc, 
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::CudaDeviceListCommand::CudaDeviceListCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::CudaDeviceListCommand::CudaDeviceListCommand()
                          : gkg::Command()
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::CudaDeviceListCommand::CudaDeviceListCommand()" );

}


gkg::CudaDeviceListCommand::CudaDeviceListCommand(
                                             const gkg::Dictionary& parameters )
                          : gkg::Command( parameters )
{

  try
  {

    execute();

  }
  GKG_CATCH( "gkg::CudaDeviceListCommand::CudaDeviceListCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::CudaDeviceListCommand::~CudaDeviceListCommand()
{
}


std::string gkg::CudaDeviceListCommand::getStaticName()
{

  try
  {

    return "CudaDeviceList";

  }
  GKG_CATCH( "std::string gkg::CudaDeviceListCommand::getStaticName()" );

}


void gkg::CudaDeviceListCommand::parse()
{

  try
  {

    gkg::Application application( _argc, _argv, 
                                  "List CUDA devices",
                                  _loadPlugin );

    application.initialize();

    execute();

  }
  GKG_CATCH_FUNCTOR( "void gkg::CudaDeviceListCommand::parse()" );

}


void gkg::CudaDeviceListCommand::execute()
{

  try
  {

    size_t deviceCount = gkg::CudaDevices::getInstance().size();

    if ( !deviceCount )
    {

      std::cerr << "No CUDA compatible devices detected" << std::endl; 

    }
    else
    {

      gkg::CudaDevices::getInstance().print();

      if ( deviceCount > 1 )
      {

        gkg::CudaProperties bestProperties =
           gkg::CudaDevices::getInstance().getProperties(
                            gkg::CudaDevices::getInstance().getBestDeviceId() );

        std::cout << "Best device = " << bestProperties._name << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::CudaDeviceListCommand::execute()" );

}


RegisterCommandCreator( CudaDeviceListCommand,
                        DECLARE_NO_PARAMETER_HELP );
