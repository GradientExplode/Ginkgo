#ifndef _gkg_communication_tcpip_Host_h_
#define _gkg_communication_tcpip_Host_h_


#include <string>


namespace gkg
{


class Host
{

  public:

    static const std::string& getName();

  private:

    static std::string _name;

};


}


#endif
