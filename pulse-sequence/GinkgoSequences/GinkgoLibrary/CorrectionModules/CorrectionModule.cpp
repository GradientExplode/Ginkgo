#include <CorrectionModules/CorrectionModule.h>


gkg::CorrectionModule::CorrectionModule()
                    : gkg::SequenceModule(),
                      _grappaFlag( false )
{
}


gkg::CorrectionModule::~CorrectionModule()
{
}


NLSStatus gkg::CorrectionModule::prepare( MrProt& mrProt,
                                          SeqLim& seqLim,
                                          MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::CorrectionModule::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );
    
  return status;


}

void 
gkg::CorrectionModule::setGrappaFlag( bool grappaFlag )
{

  _grappaFlag = grappaFlag;

}


bool gkg::CorrectionModule::getGrappaFlag()
{
  
  return _grappaFlag;

}
