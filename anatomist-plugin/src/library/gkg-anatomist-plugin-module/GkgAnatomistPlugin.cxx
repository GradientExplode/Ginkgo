#include <gkg-anatomist-plugin-module/GkgAnatomistPlugin.h>
#include <gkg-core-exception/Exception.h>
#include <mpi/mpi.h>


gkg::GkgAnatomistPlugin::GkgAnatomistPlugin()
                  : Module()
{
}


gkg::GkgAnatomistPlugin::~GkgAnatomistPlugin()
{

  // finalizing MPI
  MPI_Init( 0, 0 );
  int32_t finalized = 0;
  MPI_Finalized( &finalized );
  if ( !finalized )
  {

    MPI_Finalize();

  }

}


std::string gkg::GkgAnatomistPlugin::name() const
{

  return "gkg-anatomist-plugin";

}


std::string gkg::GkgAnatomistPlugin::description() const
{
  return "GKG plugin for anatomist";
}


void gkg::GkgAnatomistPlugin::objectsDeclaration()
{

}

void gkg::GkgAnatomistPlugin::controlsDeclaration()
{


}

void gkg::GkgAnatomistPlugin::actionsDeclaration()
{

}


// forcing instanciation
static bool initGkgAnatomistPlugin()
{

  try
  {

    gkg::GkgAnatomistPlugin* gkgAnatomistPlugin = new gkg::GkgAnatomistPlugin;
    gkgAnatomistPlugin->init();

    return true;

  }
  GKG_CATCH( "static bool initGkgAnatomistPlugin()" );

}


static bool theInitGkgAnatomistPlugin __attribute__((unused)) =
                                                       initGkgAnatomistPlugin();
