#ifndef _gkg_communication_rpc_RpcEpilog_h_
#define _gkg_communication_rpc_RpcEpilog_h_


#include <string>


namespace gkg
{


class OStream;
class IStream;


// insert or extract an epilog at the end of a request.
class RpcEpilog
{

  public:

    RpcEpilog( const std::string& text );
    virtual ~RpcEpilog();

    const std::string& getText() const;

  protected:

    friend IStream&
      operator>>( IStream& client, RpcEpilog& epilog );
    friend OStream&
      operator<<( OStream& server, const RpcEpilog& epilog );

    std::string _text;

};



IStream& operator>>( IStream& client, RpcEpilog& epilog );
OStream& operator<<( OStream& server, const RpcEpilog& epilog );

}


#endif
