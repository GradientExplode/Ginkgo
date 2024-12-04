#ifndef _gkg_communication_X11_X11CommunicationImplementationFactory_h_
#define _gkg_communication_X11_X11CommunicationImplementationFactory_h_


#include <gkg-communication-core/CommunicationImplementationFactory.h>
#include <gkg-core-pattern/Singleton.h>



namespace gkg
{


class X11CommunicationImplementationFactory : 
                       public CommunicationImplementationFactory,
                       public Singleton< X11CommunicationImplementationFactory >
{

  public:

    ~X11CommunicationImplementationFactory();

    // configuration info
    ConfigurationInfoImplementation* 
      createConfigurationInfoImplementation(
        ConfigurationInfo* configurationInfo ) const;

    // system info
    SystemInfoImplementation* 
      createSystemInfoImplementation(
        SystemInfo* systemInfo ) const;

    // mutex
    MutexImplementation*
      createMutexImplementation(
        Mutex* mutex ) const;

    // semaphore
    SemaphoreImplementation* 
      createSemaphoreImplementation(
        Semaphore* semaphore,
        int32_t count ) const;

    // thread condition
    ThreadConditionImplementation* 
      createThreadConditionImplementation(
        ThreadCondition* threadCondition ) const;

    // thread
    ThreadImplementation* 
      createThreadImplementation(
        Thread* thread ) const;

    // plugin loader
    PluginLoaderImplementation* 
      createPluginLoaderImplementation(
        PluginLoader* pluginLoader ) const;

    // directory
    DirectoryImplementation* 
      createDirectoryImplementation(
        Directory* directory,
        const std::string& name ) const;

    // file
    FileImplementation* 
      createFileImplementation(
        File* file,
        const std::string& name ) const;

  protected:

    friend class Singleton< X11CommunicationImplementationFactory >;

    X11CommunicationImplementationFactory();

};


}


#endif
