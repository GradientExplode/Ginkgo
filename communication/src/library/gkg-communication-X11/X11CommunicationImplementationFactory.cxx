#include <gkg-communication-X11/X11CommunicationImplementationFactory.h>
#include <gkg-communication-X11/X11ConfigurationInfoImplementation.h>
#include <gkg-communication-X11/X11DirectoryImplementation.h>
#include <gkg-communication-X11/X11FileImplementation.h>
#include <gkg-communication-X11/X11MutexImplementation.h>
#include <gkg-communication-X11/X11PluginLoaderImplementation.h>
#include <gkg-communication-X11/X11SemaphoreImplementation.h>
#include <gkg-communication-X11/X11SystemInfoImplementation.h>
#include <gkg-communication-X11/X11ThreadConditionImplementation.h>
#include <gkg-communication-X11/X11ThreadImplementation.h>


//
// access to factory
//

gkg::CommunicationImplementationFactory& 
gkg::getCommunicationImplementationFactory()
{

  return gkg::X11CommunicationImplementationFactory::getInstance();

}


//
// X11CommunicationImplementationFactory methods
//


gkg::X11CommunicationImplementationFactory::
                                         X11CommunicationImplementationFactory()
                                          : CommunicationImplementationFactory()
{
}


gkg::X11CommunicationImplementationFactory::
                                        ~X11CommunicationImplementationFactory()
{
}


gkg::ConfigurationInfoImplementation* 
gkg::X11CommunicationImplementationFactory::
                                          createConfigurationInfoImplementation(
                               gkg::ConfigurationInfo* configurationInfo ) const
{

  return new gkg::X11ConfigurationInfoImplementation( configurationInfo );

}


gkg::SystemInfoImplementation* 
gkg::X11CommunicationImplementationFactory::createSystemInfoImplementation(
                                             gkg::SystemInfo* systemInfo ) const
{

  return new gkg::X11SystemInfoImplementation( systemInfo );

}


gkg::MutexImplementation*
gkg::X11CommunicationImplementationFactory::createMutexImplementation(
                                                       gkg::Mutex* mutex ) const
{

  return new gkg::X11MutexImplementation( mutex );

}


gkg::SemaphoreImplementation* 
gkg::X11CommunicationImplementationFactory::createSemaphoreImplementation(
                                                      gkg::Semaphore* semaphore,
                                                      int32_t count ) const
{

  return new gkg::X11SemaphoreImplementation( semaphore, count );

}


gkg::ThreadConditionImplementation* 
gkg::X11CommunicationImplementationFactory::createThreadConditionImplementation(
                                   gkg::ThreadCondition* threadCondition ) const
{

  return new gkg::X11ThreadConditionImplementation( threadCondition );

}


gkg::ThreadImplementation* 
gkg::X11CommunicationImplementationFactory::createThreadImplementation(
                                                     gkg::Thread* thread ) const
{

  return new gkg::X11ThreadImplementation( thread );

}


gkg::PluginLoaderImplementation* 
gkg::X11CommunicationImplementationFactory::createPluginLoaderImplementation(
                                         gkg::PluginLoader* pluginLoader ) const
{

  return new gkg::X11PluginLoaderImplementation( pluginLoader );

}


gkg::DirectoryImplementation* 
gkg::X11CommunicationImplementationFactory::createDirectoryImplementation(
                                                 gkg::Directory* directory,
                                                 const std::string& name ) const
{

  return new gkg::X11DirectoryImplementation( directory, name );

}


gkg::FileImplementation* 
gkg::X11CommunicationImplementationFactory::createFileImplementation(
                                                 gkg::File* file,
                                                 const std::string& name ) const
{

  return new gkg::X11FileImplementation( file, name );

}


