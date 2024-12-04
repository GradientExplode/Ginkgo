#include <gkg-fmri-stimulus/Stimulus.h>


gkg::Stimulus::Stimulus( const std::string& name )
              : _name( name )
{
}


gkg::Stimulus::~Stimulus()
{
}


const std::string& gkg::Stimulus::getName() const
{

  return _name;

}
