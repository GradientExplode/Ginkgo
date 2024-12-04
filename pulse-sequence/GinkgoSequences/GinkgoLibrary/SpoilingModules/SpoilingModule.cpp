#include <SpoilingModules/SpoilingModule.h>


gkg::SpoilingModule::SpoilingModule()
                    : gkg::SequenceModule(),
                      _grappaFlag( false )
{
}


gkg::SpoilingModule::~SpoilingModule()
{
}

void 
gkg::SpoilingModule::setGrappaFlag( bool grappaFlag )
{

  _grappaFlag = grappaFlag;

}


bool gkg::SpoilingModule::getGrappaFlag()
{
  
  return _grappaFlag;

}
