#include <SpoilingModules/Spoiling3dModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::Spoiling3dModule::Spoiling3dModule( 
              const std::vector< double >& /* doubleParameters */,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string,
                              gkg::RCPointer< gkg::SequenceModule > >&
                                                          otherSequenceModules )
                      : gkg::SpoilingModule(),
                        // RF-signal axis
                        _singleTxExcitation3dModule( otherSequenceModules.find(
                                              stringParameters[ 0 ] )->second ),
                        
                        // Slice gradient axis (Z)
                        _sliceSelectionSpoil( "sliceSelectionSpoil" ),
                        _partitionRewinding( "partitionRewinding" ),

                        // Phase gradient axis (Y)
                        _phaseRewinding( "phaseRewinding" ),

                        // Read gradient axis (X)
                        _singleLineReading3dModule( otherSequenceModules.find(
                                               stringParameters[ 1 ] )->second )

                        // ADC signal data axis

                        // Numeric Crystal Oscillator axis
{
}


gkg::Spoiling3dModule::~Spoiling3dModule()
{
}


void gkg::Spoiling3dModule::initialize( SeqLim& seqLim )
{

  // nothing to do here

}


NLSStatus gkg::Spoiling3dModule::prepare( MrProt& mrProt,
                                          SeqLim& seqLim,
                                          MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::Spoiling3dModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );

  //////////////////////////////////////////////////////////////////////////////
  // creating a phase rewinding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseRewinding = _singleLineReading3dModule->getPhaseEncodingGradientPulse();
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a phase rewinding gradient
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelectionSpoil = 
                  _singleTxExcitation3dModule->getSliceSelectionGradientPulse();
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a partition rewinding gradient
  //////////////////////////////////////////////////////////////////////////////
  _partitionRewinding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _partitionRewinding.setMinRiseTime( _minimumRiseTimePerMeter );
  _partitionRewinding.setAxis( SEQ::AXIS_SLICE );
  if ( !_partitionRewinding.prep3DShortestTime( 
                                        mrProt,
                                        SEQ::DIR_ASCENDING,
                                        0,
                                        0 - mrProt.kSpace().echoPartition() ) )
  {
  
    return _partitionRewinding.getNLSStatus();
  
  }
  _partitionRewinding.setAmplitude( - _partitionRewinding.getAmplitude() );
  
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside this module
  //////////////////////////////////////////////////////////////////////////////

  _phaseRewinding.setStartTime( 0 );
  _partitionRewinding.setStartTime( 0 );
  _sliceSelectionSpoil.setStartTime( _partitionRewinding.getDuration() +
                                     _partitionRewinding.getRampDownTime() );
 
  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;

  _durationAfterAnchorTime = std::max( 
                             _phaseRewinding.getDuration() +
                             _phaseRewinding.getRampDownTime(),
                             _partitionRewinding.getDuration() +
                             _partitionRewinding.getRampDownTime() +
                             _sliceSelectionSpoil.getDuration() +
                             _sliceSelectionSpoil.getRampDownTime() );

  return status;

}


NLS_STATUS gkg::Spoiling3dModule::modifyKernel(
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo,
                                              const sSLICE_POS* slices,
                                              long sliceIndex,
                                              long partitionIndex,
                                              long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;

  if( !_phaseRewinding.prepPE( mrProt, _centerLineIndex - lineIndex ) )
  {

    return _phaseRewinding.getNLSStatus();

  }

  return status;

}


NLS_STATUS gkg::Spoiling3dModule::setKernelTiming( MrProt& mrProt )
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

  fRTEI( this->getStartTime() +
         _partitionRewinding.getStartTime(),
         0, 
         0, 
         0,
         &_phaseRewinding, 
         0, 
         &_partitionRewinding, 
         0 );

  return status;

}

long gkg::Spoiling3dModule::getTotalPartitionRewindingDuration() const
{
  
  return _partitionRewinding.getTotalTime();

}

long gkg::Spoiling3dModule::getTimeAfterAdc() const
{
  
  long timeAfterADC =  std::max( _phaseRewinding.getTotalTime(),
                                 _partitionRewinding.getTotalTime());

  timeAfterADC = std::max( timeAfterADC , 
                           static_cast<long>(SysProperties::getCoilCtrlHold()));
  
  return timeAfterADC;

}
 
std::string gkg::Spoiling3dModule::getStaticName()
{

  return "Spoiling3dModule";

}

//
// adding the Spoiling3dModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, Spoiling3dModule );
