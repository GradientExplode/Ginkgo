#include <gkg-communication-sysinfo/FileImplementation.h>


gkg::FileImplementation::FileImplementation( gkg::File* file,
                                             const std::string& name )
                        : _target( file ),
                          _name( name )
{
}


gkg::FileImplementation::~FileImplementation()
{
}


gkg::File* gkg::FileImplementation::getTarget() const
{

  return _target;

}


std::string gkg::FileImplementation::getName() const
{

  return _name;

}
