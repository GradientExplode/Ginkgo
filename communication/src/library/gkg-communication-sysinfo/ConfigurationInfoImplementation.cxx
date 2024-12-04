#include <gkg-communication-sysinfo/ConfigurationInfoImplementation.h>


gkg::ConfigurationInfoImplementation::ConfigurationInfoImplementation(
                                     gkg::ConfigurationInfo* configurationInfo )
                                     : _target( configurationInfo ),
                                       _hasAnonymousName( false )
{
}


gkg::ConfigurationInfoImplementation::~ConfigurationInfoImplementation()
{
}


gkg::ConfigurationInfo* gkg::ConfigurationInfoImplementation::getTarget() const
{

  return _target;

}


void gkg::ConfigurationInfoImplementation::setAnonymousName( bool value )
{

  _hasAnonymousName = value;

}


bool gkg::ConfigurationInfoImplementation::hasAnonymousName() const
{

  return _hasAnonymousName;

}
