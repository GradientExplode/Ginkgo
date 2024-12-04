#include "Core/CompositeSequenceModule.h"


gkg::CompositeSequenceModule::CompositeSequenceModule()
                             : gkg::SequenceModule()
{
}


gkg::CompositeSequenceModule::~CompositeSequenceModule()
{
}


void gkg::CompositeSequenceModule::addModule(
                   const gkg::RCPointer< gkg::SequenceModule >& sequenceModule )
{

  if ( !sequenceModule.isEmpty() )
  {

    _sequenceModules.push_back( sequenceModule );

    // a completer


  }
  else
  {

    UTRACE( Error, 0,
            "Tried to add an empty sequence module to a composite module." );  

  }


}


void gkg::CompositeSequenceModule::initialize( SeqLim& seqLim )
{

  std::list< gkg::RCPointer< gkg::SequenceModuule > >::iterator
    s = _sequenceModules.begin(),
    se = _sequenceModules.end();
  while ( s != se )
  {

    s->intialize( seqLim );
    ++ s;

  }

}


NLS_STATUS gkg::CompositeSequenceModule::prepare(
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  NLSStatus status = SEQU__NORMAL;

  std::list< gkg::RCPointer< gkg::SequenceModuule > >::iterator
    s = _sequenceModules.begin(),
    se = _sequenceModules.end();
  while ( s != se )
  {

    status = s->prepare( mrProt, seqLim, seqExpo );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }

    ++ s;

  }

  return status;

}


NLS_STATUS gkg::CompositeSequenceModule::modifyKernel(
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLSStatus status = SEQU__NORMAL;

  std::list< gkg::RCPointer< gkg::SequenceModuule > >::iterator
    s = _sequenceModules.begin(),
    se = _sequenceModules.end();
  while ( s != se )
  {

    status = s->modifyKernel( mrProt,
                              seqLim,
                              seqExpo,
                              slices,
                              sliceIndex,
                              partitionIndex,
                              lineIndex );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }

    ++ s;

  }

  return status;

}


NLS_STATUS gkg::CompositeSequenceModule::setKernelTiming( MrProt& mrProt )
{

  NLSStatus status = SEQU__NORMAL;

  std::list< gkg::RCPointer< gkg::SequenceModuule > >::iterator
    s = _sequenceModules.begin(),
    se = _sequenceModules.end();
  while ( s != se )
  {

    status = s->setKernelTiming( mrProt );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }

    ++ s;

  }

  return status;

}



