#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuService.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuCommandId.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuRegisterCallback.h>
#include <gkg-processing-plugin-functors/DicomProcessingUnit/DpuUnregisterCallback.h>


gkg::DpuService::DpuService( int32_t port )
               : gkg::SocketService( port ),
		 _reader( NULL )
{
}


gkg::DpuService::~DpuService()
{

  delete _reader;

}


void gkg::DpuService::createReader( int32_t fd )
{

  _reader = new gkg::RpcCommandReader( fd );
  
  if ( _reader )
  {
  
    gkg::DpuRegisterCallback* registerCbk =
      new gkg::DpuRegisterCallback( gkg::DPU_REGISTER_CLIENT, NULL );
    gkg::DpuUnregisterCallback* unregisterCbk =
      new gkg::DpuUnregisterCallback( gkg::DPU_UNREGISTER_CLIENT, NULL );
      
    _reader->add( registerCbk );
    _reader->add( unregisterCbk );      
  
  }

}
