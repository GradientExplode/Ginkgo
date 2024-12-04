#include <PreparationModules/DiffusionGradientModule.h>


gkg::DiffusionGradientModule::DiffusionGradientModule()
                            : gkg::SequenceModule()
{
}


gkg::DiffusionGradientModule::~DiffusionGradientModule()
{
}


void gkg::DiffusionGradientModule::initialize( SeqLim& seqLim )
{
}

NLSStatus gkg::DiffusionGradientModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::DiffusionGradientModule::prepare" };
  
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

NLS_STATUS gkg::DiffusionGradientModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partition,
                                               long lineIndex )
{

  return SEQU__NORMAL;

}


NLS_STATUS gkg::DiffusionGradientModule::setKernelTiming( MrProt& mrProt )
{      

  return SEQU__NORMAL;

}


void gkg::DiffusionGradientModule::setParameters( 
                                              std::vector< double > parameters )
{

  _parameters = parameters;

}

void gkg::DiffusionGradientModule::setRefocusingDuration( 
                                                       long refocusingDuration )
{

  _refocusingDuration = refocusingDuration;

}
