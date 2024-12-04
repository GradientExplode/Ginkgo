#include <PreEncodingModules/PreEncodingModule.h>


gkg::PreEncodingModule::PreEncodingModule()
                   : gkg::SequenceModule(),
                     _grappaFlag( false )
{
}


gkg::PreEncodingModule::~PreEncodingModule()
{
}

void gkg::PreEncodingModule::initialize( SeqLim& seqLim )
{
}

void 
gkg::PreEncodingModule::setGrappaFlag( bool grappaFlag )
{

  _grappaFlag = grappaFlag;

}


bool gkg::PreEncodingModule::getGrappaFlag()
{
  
  return _grappaFlag;

}
