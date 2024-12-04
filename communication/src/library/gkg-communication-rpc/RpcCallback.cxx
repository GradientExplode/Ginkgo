#include <gkg-communication-rpc/RpcCallback.h>


gkg::RpcCallback::RpcCallback( int32_t id, void* parameter )
                 : _parameter( parameter ),
		   _id( id )
{
}


gkg::RpcCallback::~RpcCallback()
{
}


int32_t gkg::RpcCallback::getId()
{

  return _id;

}
