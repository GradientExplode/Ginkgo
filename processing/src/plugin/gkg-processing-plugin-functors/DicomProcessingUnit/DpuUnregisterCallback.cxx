#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuUnregisterCallback.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DicomProcessingUnit.h>


gkg::DpuUnregisterCallback::DpuUnregisterCallback( int32_t id, void* parameter )
                           : gkg::RpcCallback( id, parameter )
{
}


void gkg::DpuUnregisterCallback::receive( gkg::SocketStream& stream )
{

  std::string host;
  int32_t port;
  
  stream >> host;
  stream >> port;
  
  gkg::DicomProcessingUnit::getInstance().removeWriter( host, port );

}
