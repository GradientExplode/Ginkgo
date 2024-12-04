#ifndef _gkg_communication_plugin_Plugin_h_
#define _gkg_communication_plugin_Plugin_h_


#include <string>


namespace gkg
{


class Plugin
{

  public:

    virtual ~Plugin();

    virtual std::string getName() const = 0;

  protected:

    Plugin();

    virtual void initialize();

};


}


#endif
