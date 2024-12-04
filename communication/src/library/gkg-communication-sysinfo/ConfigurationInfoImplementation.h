#ifndef _gkg_communication_sysinfo_ConfigurationInfoImplementation_h_
#define _gkg_communication_sysinfo_ConfigurationInfoImplementation_h_


#include <string>


namespace gkg
{


class ConfigurationInfo;


class ConfigurationInfoImplementation
{

  public:

    virtual ~ConfigurationInfoImplementation();

    ConfigurationInfo* getTarget() const;

    virtual std::string getHomePath() const = 0;
    virtual std::string getSharePath() const = 0;
    virtual std::string getTmpPath() const = 0;
    virtual std::string getEnvPath() const = 0;
    virtual std::string getMemoryMappingPath() const = 0;

    virtual void setAnonymousName( bool value );
    virtual bool hasAnonymousName() const;

  protected:

    ConfigurationInfoImplementation( ConfigurationInfo* configurationInfo );

    ConfigurationInfo* _target;
    bool _hasAnonymousName;

};


}


#endif

