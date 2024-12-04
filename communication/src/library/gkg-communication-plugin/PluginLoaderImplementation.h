#ifndef _gkg_communication_plugin_PluginLoaderImplementation_h_
#define _gkg_communication_plugin_PluginLoaderImplementation_h_


#include <string>
#include <list>


namespace gkg
{


class PluginLoader;
class File;


class PluginLoaderImplementation
{

  public:

    virtual ~PluginLoaderImplementation();

    PluginLoader* getTarget() const;

    void load( bool verbose, bool loadFunctors ) const;
    virtual void loadPlugin( const std::string& pluginName,
                             bool verbose ) const = 0;
    void addPluginConfigFileName(
                                const std::string& pluginConfigFileName );

  protected:

    PluginLoaderImplementation( PluginLoader* pluginLoader );

    void createEmptyPluginConfigFile( const File& name );
    void loadPluginConfigFile( const std::string& pluginConfigFileName,
                               bool verbose ) const;

    PluginLoader* _target;
    std::list< std::string > _pluginConfigFileNames;
    std::list< std::string > _functorConfigFileNames;

};


}


#endif

