#include <gkg-communication-rpc/RpcProlog.h>
#include <gkg-communication-socket/OStream.h>
#include <gkg-communication-socket/IStream.h>
#include <iostream>
#include <cstring>


gkg::RpcProlog::RpcProlog( const std::string& text )
               : _text( text )
{

  if ( text.length() > 1024U )
  {

    std::cerr << "Prolog text too long!" << std::endl;

  }

}


gkg::RpcProlog::~RpcProlog()
{
}


const std::string& gkg::RpcProlog::getText() const
{

  return _text;

}


gkg::IStream& gkg::operator>>( gkg::IStream& client, gkg::RpcProlog& prolog )
{

  if ( client.good() )
  {

    int8_t gkgBeginRequest[ 1024U ];
    client >> gkgBeginRequest;
    std::string prologStr = std::string( "GKG_COMMUNICATION_RPC_BEGIN_" ) +
                            prolog.getText() + "_REQUEST";
    if ( strcmp( ( char* )gkgBeginRequest, prologStr.c_str() ) )
    {

      client.clear( client.rdstate() | std::ios::failbit );

    }

  }
  return client;

}


gkg::OStream& gkg::operator<<( gkg::OStream& server, 
                               const gkg::RpcProlog& prolog )
{

  if ( server.good() )
  {

    std::string prologStr = std::string( "GKG_COMMUNICATION_RPC_BEGIN_" ) +
                            prolog.getText() + "_REQUEST";

    server << prologStr;

  }
  return server;

}
