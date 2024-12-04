#ifndef _gkg_communication_X11_X11ConfigurationInfoImplementation_h_
#define _gkg_communication_X11_X11ConfigurationInfoImplementation_h_


#include <gkg-communication-sysinfo/ConfigurationInfoImplementation.h>


namespace gkg
{


class X11ConfigurationInfoImplementation :
                                          public ConfigurationInfoImplementation
{

  public:

    X11ConfigurationInfoImplementation( ConfigurationInfo* configurationInfo );
    ~X11ConfigurationInfoImplementation();

    std::string getHomePath() const;
    std::string getSharePath() const;
    std::string getTmpPath() const;
    std::string getEnvPath() const;
    std::string getMemoryMappingPath() const;

    static X11ConfigurationInfoImplementation*
                      getImplementation( ConfigurationInfo* configurationInfo );

};


}


#endif

