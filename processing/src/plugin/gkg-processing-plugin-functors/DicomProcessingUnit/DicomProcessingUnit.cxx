#include <gkg-processing-plugin-functors/DicomProcessingUnit/DicomProcessingUnit.h>
#include <gkg-communication-rpc/RpcCommandWriter.h>
#include <gkg-processing-rpc/RpcTransferVolumeCommand_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>


gkg::DicomProcessingUnit::~DicomProcessingUnit()
{

  std::map< gkg::HostPort, 
            gkg::RpcCommandWriter*, 
	    gkg::HostPortCompare >::iterator
    it = _writers.begin(),
    ie = _writers.end();
    
  while ( it != ie )
  {
  
    delete it->second;
    ++it;  

  }

}


void gkg::DicomProcessingUnit::addWriter( const std::string& host, 
                                          int32_t port )
{

  try
  {
  
    gkg::RpcCommandWriter* writer = new gkg::RpcCommandWriter( host, port );
    gkg::HostPort hostPort( host, port );
    _writers.insert( std::make_pair( hostPort, writer ) );
    
  }
  GKG_CATCH( "void gkg::DicomProcessingUnit::addWriter( "
             "const std::string& host, int32_t port )" );

}


void gkg::DicomProcessingUnit::removeWriter( const std::string& host,
                                             int32_t port )
{

  try
  {
  
    gkg::HostPort hostPort( host, port );
    std::map< gkg::HostPort, 
              gkg::RpcCommandWriter*, 
	      gkg::HostPortCompare >::iterator
      it = _writers.find( hostPort );
    
    if ( it != _writers.end() )
    {
  
      delete it->second;
      _writers.erase( it );
  
    }

  }
  GKG_CATCH( "void gkg::DicomProcessingUnit::removeWriter( "
             "const std::string& host, int32_t port )" );

}


void gkg::DicomProcessingUnit::send( const std::string& folderName )
{

  try
  {
  
    gkg::RpcTransferVolumeCommand< int16_t > volInt16Cmd( 
                                               gkg::DPU_TRANSFER_VOLUME_INT16 );

    gkg::Reader::getInstance().read( folderName, 
                                     volInt16Cmd.getVolume(),
				     "dicom" );

    std::map< gkg::HostPort, 
              gkg::RpcCommandWriter*, 
	      gkg::HostPortCompare >::iterator
      it = _writers.begin(),
      ie = _writers.end();

    while ( it != ie )
    {
    
      it->second->send( volInt16Cmd );
      it->second->flush();
      ++it;
    
    }

  }
  GKG_CATCH( "void gkg::DicomProcessingUnit::send( "
             "const std::string& folderName )" );

}


void gkg::DicomProcessingUnit::send( gkg::DpuCommandId commandId )
{

  try
  {
  
    gkg::RpcCommand cmd( commandId );

    std::map< gkg::HostPort, 
              gkg::RpcCommandWriter*, 
	      gkg::HostPortCompare >::iterator
      it = _writers.begin(),
      ie = _writers.end();

    while ( it != ie )
    {
    
      it->second->send( cmd );
      it->second->flush();
      ++it;
    
    }

  }
  GKG_CATCH( "void gkg::DicomProcessingUnit::send( "
             "gkg::DpuCommandId commandId )" );

}
