#include <ExcitationModules/SingleTxExcitation2dModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::SingleTxExcitation2dModule::SingleTxExcitation2dModule( 
           const std::vector< double >& doubleParameters,
           const std::vector< std::string >& stringParameters,
           const std::map< std::string,
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
                                : gkg::ExcitationModule( false ),

                                  _defaultFlipAngle( doubleParameters[ 0 ] ),
                                  // RF-signal axis
                                  _rfDuration(
                                      ( long )( doubleParameters[ 1 ] + 0.5 ) ),
                                  _rfSampleCount(
                                   ( int32_t )( doubleParameters[ 2 ] + 0.5 ) ),
                                  _rfBandwidthTimeProduct( 
                                                        doubleParameters[ 3 ] ),
                                  _rfInitialPhase(
                                      ( long )( doubleParameters[ 4 ] + 0.5 ) ),
                                  _rfWaveformEnveloppe( "rfWaveformEnveloppe" ),
                                  _rfSpoilIncrement( 0.0 ),

                                  // Slice gradient axis (Z)
                                  _sliceSelection( "sliceSelection" ),
                                  _sliceSelectionRefocus(
                                                      "sliceSelectionRefocus" ),

                                  // Phase gradient axis (Y)

                                  // Read gradient axis (X)

                                  // ADC signal data axis

                                  // Numeric Crystal Oscillator axis
                                  _oscillatorSynchronization(
                                                  "oscillatorSynchronization" ),
                                  _rfOffsets( "rfOffsets" ),
                                  _rfResetSynthetizer( "rfResetSynthetizer" )
{
}


gkg::SingleTxExcitation2dModule::~SingleTxExcitation2dModule()
{
}


void gkg::SingleTxExcitation2dModule::initialize( SeqLim& seqLim )
{

  //////////////////////////////////////////////////////////////////////////////
  // calling initialize methode of the mother class
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::ExcitationModule::initialize( seqLim );
  
  //////////////////////////////////////////////////////////////////////////////
  // Setting flip angle
  //////////////////////////////////////////////////////////////////////////////

  seqLim.setFlipAngle( 10.0, // min in degrees
                       90.0, // max
                       1.0,  // increment
                       _defaultFlipAngle );

}


NLSStatus gkg::SingleTxExcitation2dModule::prepare(
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::SingleTxExcitation2dModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );
  mrProt.PAT().setlAccelFactPE( ( int32_t ) 2 );
  
  //////////////////////////////////////////////////////////////////////////////
  // creating an excitation sinus radiofrequency
  //////////////////////////////////////////////////////////////////////////////

  _rfWaveformEnveloppe.setTypeExcitation();
  _rfWaveformEnveloppe.setDuration( _rfDuration );

  _rfWaveformEnveloppe.setFlipAngle( mrProt.flipAngle() );
  _rfWaveformEnveloppe.setInitialPhase( _rfInitialPhase );

  _rfWaveformEnveloppe.setThickness(
                                    mrProt.sliceSeries().aFront().thickness() );
  _rfWaveformEnveloppe.setSamples( _rfSampleCount );

  _rfWaveformEnveloppe.setBandwidthTimeProduct( _rfBandwidthTimeProduct );

  if( !_rfWaveformEnveloppe.prepSinc( mrProt, seqExpo ) )
  {

    return _rfWaveformEnveloppe.getNLSStatus();

  }

  //////////////////////////////////////////////////////////////////////////////
  // creating a slice selection gradient
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelection.setAmplitude( _rfWaveformEnveloppe.getGSAmplitude() );
  _sliceSelection.setRampTimes(fSDSRoundUpGRT(
                   std::max< long >( ( long )( _minimumRiseTimePerMeter *
                                               _sliceSelection.getAmplitude() ),
                                     SysProperties::getCoilCtrlLead() ) ) );
  _sliceSelection.setDuration( fSDSRoundUpGRT(
                                            _rfWaveformEnveloppe.getDuration() +
                                            _sliceSelection.getRampUpTime() ) );


  if ( !_sliceSelection.prep() ) 
  {

    return _sliceSelection.getNLSStatus();

  }

  if ( !_sliceSelection.check() )
  {

    return _sliceSelection.getNLSStatus();

  }


  //////////////////////////////////////////////////////////////////////////////
  // creating a slice refocusing gradient
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelectionRefocus.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _sliceSelectionRefocus.setMinRiseTime( _minimumRiseTimePerMeter );

  // computing the half surface to prepare the refocusing gradient
  double halfSelectionMomentum = _sliceSelection.getMomentum(
                                         _sliceSelection.getDuration() -
                                         _rfWaveformEnveloppe.getDuration() / 2,
                                         _sliceSelection.getTotalTime() );

  if ( !_sliceSelectionRefocus.prepSymmetricTOTShortestTime(
                                                      -halfSelectionMomentum ) )
  {

    return _sliceSelectionRefocus.getNLSStatus();

  }


  //////////////////////////////////////////////////////////////////////////////
  // creating an oscillator for synchronisation
  //////////////////////////////////////////////////////////////////////////////

  _oscillatorSynchronization.setCode( SYNCCODE_OSC0 );
  // setting the pulse duration to 10us
  _oscillatorSynchronization.setDuration( 10 );


  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside the module
  //////////////////////////////////////////////////////////////////////////////

  long sliceSelectionDuration = _sliceSelection.getDuration() + 
                                _sliceSelection.getRampDownTime();
  long sliceSelectionRefocusDuration = _sliceSelectionRefocus.getDuration() + 
                                       _sliceSelectionRefocus.getRampDownTime();

  long rfStartTime = _sliceSelection.getDuration() -
                     _rfWaveformEnveloppe.getDuration();
  _rfWaveformEnveloppe.setStartTime( rfStartTime );
  _rfResetSynthetizer.setStartTime( _sliceSelection.getDuration() );

  _sliceSelection.setStartTime( 0 );

  _sliceSelectionRefocus.setStartTime( sliceSelectionDuration );

  _anchorTime = rfStartTime + _rfWaveformEnveloppe.getDuration() / 2;
  _durationBeforeAnchorTime  = rfStartTime +
                               _rfWaveformEnveloppe.getDuration() / 2;
  _durationAfterAnchorTime = _rfWaveformEnveloppe.getDuration() / 2 +
                             _sliceSelection.getRampDownTime() + 
                             sliceSelectionRefocusDuration;

  return status;

}


NLS_STATUS gkg::SingleTxExcitation2dModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;

  _rfOffsets.prepSet( slices[ sliceIndex ], 
                      _rfWaveformEnveloppe, 
                      _startLine );
  _rfResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                               _rfWaveformEnveloppe, 
                               _startLine ); 

  return status;

}


NLS_STATUS gkg::SingleTxExcitation2dModule::setKernelTiming( MrProt& mrProt )
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
  //        Sync ); 

  fRTEI( this->getStartTime() +
         _sliceSelection.getStartTime(), 
         0, 
         0, 
         0, 
         0, 
         0, 
         &_sliceSelection, 
         &_oscillatorSynchronization );

  fRTEI( this->getStartTime() +
         _rfWaveformEnveloppe.getStartTime(), 
         &_rfOffsets,
         &_rfWaveformEnveloppe, 
         0, 
         0, 
         0, 
         0, 
         0 );

  fRTEI( this->getStartTime() +
         _rfResetSynthetizer.getStartTime(), 
         &_rfResetSynthetizer,
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );

  fRTEI( this->getStartTime() +
         _sliceSelectionRefocus.getStartTime(), 
         0, 
         0, 
         0, 
         0, 
         0, 
         &_sliceSelectionRefocus, 
         0 );

  return status;

}


const sRF_PULSE_SINC& 
gkg::SingleTxExcitation2dModule::getRfWaveformEnveloppe() const
{

  return _rfWaveformEnveloppe;

}


const sGRAD_PULSE& 
gkg::SingleTxExcitation2dModule::getSliceSelectionGradientPulse() const
{

  return _sliceSelection;

}


std::string gkg::SingleTxExcitation2dModule::getStaticName()
{

  return "SingleTxExcitation2dModule";

}


//
// adding the SingleTxExcitation2dModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, SingleTxExcitation2dModule );

