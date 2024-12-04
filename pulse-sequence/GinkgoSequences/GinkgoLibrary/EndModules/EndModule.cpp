#include <EndModules/EndModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::EndModule::EndModule(
           const std::vector< double >& doubleParameters,
           const std::vector< std::string >& stringParameters,
           const std::map< std::string, 
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
               : gkg::SequenceModule(),
                 _defaultTR( doubleParameters[ 0 ] )
{
}


gkg::EndModule::~EndModule()
{
}


void gkg::EndModule::initialize( SeqLim& seqLim )
{
  
  seqLim.setTR( 0, // index
                100, // min
                50000000, // max
                100, // increment
                _defaultTR );

}


NLSStatus gkg::EndModule::prepare( MrProt& mrProt,
                                   SeqLim& seqLim,
                                   MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::EndModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;

  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;
  _durationAfterAnchorTime = 0;

  return status;

}


NLS_STATUS gkg::EndModule::modifyKernel( MrProt& mrProt,
                                         SeqLim& seqLim,
                                         MrProtocolData::SeqExpo& seqExpo,
                                         const sSLICE_POS* slices,
                                         long sliceIndex,
                                         long partition,
                                         long lineIndex )
{

  return SEQU__NORMAL;

}


NLS_STATUS gkg::EndModule::setKernelTiming( MrProt& mrProt )
{


  NLS_STATUS status  = SEQU__NORMAL;

  // Sequence timing: 
  // fRTEI( StartTime(usec),
  //        NCO Event, 
  //        SRF Event, 
  //        ADC Event, 
  //        phase gradient Event, 
  //        read gradient Event, 
  //        slice gradient Event, 
  //        Event, 
  //        Sync );

  fRTEI( this->getStartTime(),
         0,
         0,
         0,
         0,
         0,
         0,
         0 );       

  return status;

}


std::string gkg::EndModule::getStaticName()
{

  return "EndModule";

}


//
// adding the EndModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, EndModule );
