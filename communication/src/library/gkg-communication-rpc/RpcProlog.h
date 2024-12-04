#ifndef _gkg_communication_rpc_RpcProlog_h_
#define _gkg_communication_rpc_RpcProlog_h_


#include <string>


namespace gkg
{


class OStream;
class IStream;


// insert or extract a prolog at the beginning of a request.
class RpcProlog
{

  public:

    RpcProlog( const std::string& text );
    virtual ~RpcProlog();

    const std::string& getText() const;

  protected:

    friend IStream&
      operator>>( IStream& client, RpcProlog& prolog );
    friend OStream&
      operator<<( OStream& server, const RpcProlog& prolog );

    std::string _text;

};


IStream& operator>>( IStream& client, RpcProlog& prolog );
OStream& operator<<( OStream& server, const RpcProlog& prolog );


}


#endif
