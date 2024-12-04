#ifndef _gkg_communication_plugin_PluginLoader_h_
#define _gkg_communication_plugin_PluginLoader_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class PluginLoaderImplementation;


class PluginLoader : public Singleton< PluginLoader >
{

  public:

    virtual ~PluginLoader();

    virtual void load( bool verbose = true,
                       bool loadFunctors = false ) const;
    virtual void loadPlugin( const std::string& pluginName,
                             bool verbose ) const;
    virtual void addPluginConfigFileName(
                                const std::string& pluginConfigFileName );

    PluginLoaderImplementation* getImplementation() const;

  protected:

    friend class Singleton< PluginLoader >;

    PluginLoader();

    PluginLoaderImplementation* _pluginLoaderImplementation;

};



}


#endif
