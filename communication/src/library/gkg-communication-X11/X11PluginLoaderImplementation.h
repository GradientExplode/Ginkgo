#ifndef _gkg_communication_X11_X11PluginLoaderImplementation_h_
#define _gkg_communication_X11_X11PluginLoaderImplementation_h_


#include <gkg-communication-plugin/PluginLoaderImplementation.h>

#include <list>


namespace gkg
{


class X11PluginLoaderImplementation : public PluginLoaderImplementation
{

  public:

    X11PluginLoaderImplementation( PluginLoader* pluginLoader );
    ~X11PluginLoaderImplementation();

    void loadPlugin( const std::string& pluginName, bool verbose ) const;

    static X11PluginLoaderImplementation*
             getImplementation( PluginLoader* pluginLoader );

  private:

    mutable std::list< void* > _handles;

};


}


#endif

