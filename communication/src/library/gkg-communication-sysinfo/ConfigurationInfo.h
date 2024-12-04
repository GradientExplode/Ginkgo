#ifndef _gkg_communication_sysinfo_ConfigurationInfo_h_
#define _gkg_communication_sysinfo_ConfigurationInfo_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-cppext/StdInt.h>
#include <set>
#include <string>


namespace gkg
{


class ConfigurationInfoImplementation;
class File;

class ConfigurationInfo : public Singleton< ConfigurationInfo >
{

  public:

    ~ConfigurationInfo();

    // system information(s)
    std::string getHomePath() const;
    std::string getGkgVersion() const;
    std::string getGkgVersionAndPatch() const;
    std::string getSharePath() const;
    std::string getHiddenGkgPath() const;
    std::string getTmpPath() const;
    std::string getEnvPath() const;
    std::string getMemoryMappingPath() const;

    // gkg information(s)
    std::string getGkgPath() const;
    std::string getIconPath( const std::string& iconName ) const;
    std::string getUiPath( const std::string& applicationName,
                           const std::string& uiName ) const;
    std::string getGkgResourceConfigFileName() const;
    std::string getGkgPluginConfigFileName() const;
    std::string getGkgFunctorConfigFileName() const;
    std::string getHeaderExtensionName() const;
    std::string getFunctionalVolumeDimension() const;
    std::string getCSVSeparator() const;
    uint64_t getMemoryMappingPageSize() const;
    double getMemoryMappingSwitchOnFactor() const;
    bool isMemoryMappingVerbose() const;
    bool isMemoryMappingDisabled() const;
    int32_t getNumberOfCpuToUse() const;
    void setAnonymousName( bool value );
    bool hasAnonymousName() const;

    // gkg-private information(s)
    std::string getGkgPrivatePath() const;
    std::string getGkgPrivatePluginConfigFileName() const;
    std::string getGkgPrivateFunctorConfigFileName() const;

    // gkg-data information(s)
    std::string getGkgDataPath() const;
    std::string getAnatomicalTemplatesPath(
                                        const std::string& specy,
                                        const std::string& templateName ) const;
    std::string getWhiteMatterAtlasesPath( const std::string& specy,
                                           const std::string& atlasName ) const;
    std::string getMedusaResourcePath() const;
    std::set< std::string > getMedusaNeuronModelPaths(
                                    const std::string& neuronDatabaseName,
                                    const std::string& neuronTypeString ) const;

    // access to implementation class
    ConfigurationInfoImplementation* getImplementation() const;

  protected:

    friend class Singleton< ConfigurationInfo >;

    ConfigurationInfo();

    void createEmptyResourceConfigFile( const File& file ) const;

    ConfigurationInfoImplementation* _configurationInfoImplementation;

};


// this function is used to get the separator pattern for the paths list
// and should be implemented in the concrete library
// (in libgkg-communication-X11.so for instance)
std::string getEnvironmentPathSeparator();


}


#endif

