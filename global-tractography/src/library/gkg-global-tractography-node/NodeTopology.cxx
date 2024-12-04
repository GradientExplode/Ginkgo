#include <gkg-global-tractography-node/NodeTopology.h>
#include <gkg-core-exception/Exception.h>


gkg::NodeTopology::NodeTopology()
{

  try
  {

  }
  GKG_CATCH( "gkg::NodeTopology::NodeTopology()" );

}


gkg::NodeTopology::~NodeTopology()
{
}

bool gkg::NodeTopology::isOK()
{
  return true;
}
