#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuRegisterCallback.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DicomProcessingUnit.h>


gkg::DpuRegisterCallback::DpuRegisterCallback( int32_t id, void* parameter )
                        : gkg::RpcCallback( id, parameter )
{
}


void gkg::DpuRegisterCallback::receive( gkg::SocketStream& stream )
{

  std::string host;
  int32_t port;
  
  stream >> host;
  stream >> port;
  
  gkg::DicomProcessingUnit::getInstance().addWriter( host, port );

}
