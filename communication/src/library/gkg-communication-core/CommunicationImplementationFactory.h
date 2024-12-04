#ifndef _gkg_communication_core_CommunicationImplementationFactory_h_
#define _gkg_communication_core_CommunicationImplementationFactory_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class ConfigurationInfo;
class ConfigurationInfoImplementation;
class SystemInfo;
class SystemInfoImplementation;
class Mutex;
class MutexImplementation;
class Semaphore;
class SemaphoreImplementation;
class ThreadCondition;
class ThreadConditionImplementation;
class Thread;
class ThreadImplementation;
class PluginLoader;
class PluginLoaderImplementation;
class Directory;
class DirectoryImplementation;
class File;
class FileImplementation;


class CommunicationImplementationFactory
{

  public:

    virtual ~CommunicationImplementationFactory();

    // configuration info
    virtual ConfigurationInfoImplementation* 
              createConfigurationInfoImplementation(
                ConfigurationInfo* configurationInfo ) const = 0;

    // system info
    virtual SystemInfoImplementation* 
              createSystemInfoImplementation(
                SystemInfo* systemInfo ) const = 0;

    // mutex
    virtual MutexImplementation*
              createMutexImplementation(
                Mutex* mutex ) const = 0;

    // semaphore
    virtual SemaphoreImplementation* 
              createSemaphoreImplementation(
                Semaphore* semaphore,
                int32_t count ) const = 0;

    // thread condition
    virtual ThreadConditionImplementation* 
              createThreadConditionImplementation(
                ThreadCondition* threadCondition ) const = 0;

    // thread
    virtual ThreadImplementation* 
              createThreadImplementation(
                Thread* thread ) const = 0;

    // plugin loader
    virtual PluginLoaderImplementation* 
              createPluginLoaderImplementation(
                PluginLoader* pluginLoader ) const = 0;

    // directory
    virtual DirectoryImplementation* 
              createDirectoryImplementation(
                Directory* directory,
                const std::string& name ) const = 0;

    // file
    virtual FileImplementation* 
              createFileImplementation(
                File* file,
                const std::string& name ) const = 0;

  protected:

    CommunicationImplementationFactory();

};


// this function is used to get access to the concrete implementation
// factory and should be implemented in the concrete library
// (in libgkg-communication-X11.so for instance )
CommunicationImplementationFactory& getCommunicationImplementationFactory();


}


#endif
