#ifndef _gkg_communication_plugin_PluginManager_h_
#define _gkg_communication_plugin_PluginManager_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-pattern/Observable.h>
#include <string>
#include <map>


namespace gkg
{


class Plugin;


class PluginManager : public Singleton< PluginManager >,
                      public Observable
{

  public:

    ~PluginManager();

    Plugin* getPlugin( const std::string& name ) const;
    bool hasPlugin( const std::string& name ) const;

    void registerPlugin( Plugin* plugin );
    void unregisterPlugin( Plugin* plugin );

  protected:

    friend class Singleton< PluginManager >;

    PluginManager();

    std::map< std::string, Plugin* > _plugins;

};


}


#endif
