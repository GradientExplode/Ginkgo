#include <ExcitationModules/ExcitationModule.h>


gkg::ExcitationModule::ExcitationModule( bool isParallelTx )
                      : gkg::SequenceModule(),
                        _isParallelTx( isParallelTx ),
                        _rfSpoilPhase( 0.0 )
{
}


gkg::ExcitationModule::~ExcitationModule()
{
}


void gkg::ExcitationModule::initialize( SeqLim& seqLim )
{

  seqLim.setAllowedFrequency( 8000000,  // min
                              500000000 ); // max
                       
  seqLim.setRequiredGradAmpl( 1.0 ); //  mT/m
  seqLim.setRequiredGradSlewRate( 10.0 ); // (mT/m)/ms
  
  seqLim.setRepetitions( 0,
                         511,
                         1,        
                         0);
  
  seqLim.setPATMode( SEQ::PAT_MODE_NONE, 
                     SEQ::PAT_MODE_GRAPPA );
    
                     
  seqLim.setGradients( SEQ::GRAD_FAST,
                       SEQ::GRAD_NORMAL,
                       SEQ::GRAD_WHISPER,
                       SEQ::GRAD_FAST_GSWD_RISETIME,
                       SEQ::GRAD_NORMAL_GSWD_RISETIME,
                       SEQ::GRAD_WHISPER_GSWD_RISETIME );

}


bool gkg::ExcitationModule::isParallelTx() const
{

  return _isParallelTx;

}


double gkg::ExcitationModule::getRfSpoilPhase() const
{

  return _rfSpoilPhase;

}
