#include <gkg-communication-sysinfo/SystemInfoImplementation.h>


gkg::SystemInfoImplementation::SystemInfoImplementation(
                                                   gkg::SystemInfo* systemInfo )
                              : _target( systemInfo )
{
}


gkg::SystemInfoImplementation::~SystemInfoImplementation()
{
}


gkg::SystemInfo* gkg::SystemInfoImplementation::getTarget() const
{

  return _target;

}
