#include <gkg-communication-rpc/RpcEpilog.h>
#include <gkg-communication-socket/OStream.h>
#include <gkg-communication-socket/IStream.h>
#include <iostream>
#include <cstring>


gkg::RpcEpilog::RpcEpilog( const std::string& text )
               : _text( text )
{

  if ( text.length() > 1024U )
  {

    std::cerr << "Epilog text too long!" << std::endl;

  }

}


gkg::RpcEpilog::~RpcEpilog()
{
}


const std::string& gkg::RpcEpilog::getText() const
{

  return _text;

}



gkg::IStream& gkg::operator>>( gkg::IStream& client, gkg::RpcEpilog& epilog )
{

  if ( client.good() )
  {

    int8_t gkgEndRequest[ 1024U ];
    client >> gkgEndRequest;
    std::string epilogStr = std::string( "GKG_COMMUNICATION_RPC_END_" ) +
                            epilog.getText() + "_REQUEST";
    if ( strcmp( ( char * )gkgEndRequest, epilogStr.c_str() ) )
    {

      client.clear( client.rdstate() | std::ios::failbit );

    }

  }
  return client;

}


gkg::OStream& gkg::operator<<( gkg::OStream& server,
                               const gkg::RpcEpilog& epilog )
{

  if ( server.good() )
  {

    std::string epilogStr = std::string( "GKG_COMMUNICATION_RPC_END_" ) +
                            epilog.getText() + "_REQUEST";

    server << epilogStr;

  }
  return server;

}
