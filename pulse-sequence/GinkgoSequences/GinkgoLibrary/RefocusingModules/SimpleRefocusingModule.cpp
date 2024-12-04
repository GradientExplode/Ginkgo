#include <RefocusingModules/SimpleRefocusingModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::SimpleRefocusingModule::SimpleRefocusingModule(
                 const std::vector< double >& /* doubleParameters */,
                 const std::vector< std::string >& /* stringParameters */,
                 const std::map< std::string,
                 gkg::RCPointer< gkg::SequenceModule > >& otherSequenceModules )
                               : gkg::RefocusingModule(),
             // RF-signal axis
            _rfRefocusingWaveformEnveloppe( "rfRefocusingWaveformEnveloppe" ),
            _rfRefocusingSpoilIncrement( 0.0 ),
            _rfRefocusingSpoilPhase( 0.0 ),
          
            // Slice gradient axis (Z)
            _sliceSelectionCrusher( "sliceSelectionCrusher" ),
            _sliceSelectionRefocusing( "sliceSelectionRefocusing" ),
               
            // Phase gradient axis (Y)
            _phaseCrusher( "phaseCrusher" ),
               
            // Read gradient axis (X)
            _readoutCrusher( "readoutCrusher" ),
               
            // ADC signal data axis
               
            // Numeric Crystal Oscillator axis
            _rfRefocusingOffsets( "rfRefocusingOffsets" ),
            _rfRefocusingResetSynthetizer( "rfRefocusingResetSynthetizer" )
{
}


gkg::SimpleRefocusingModule::~SimpleRefocusingModule()
{
}


void gkg::SimpleRefocusingModule::initialize( SeqLim& seqLim )
{

  // nothing to do
  UTRACE( Notice, 0, "+-+-+ Initialize refoc" );

}


NLSStatus gkg::SimpleRefocusingModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::SimpleRefocusingModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );

  //////////////////////////////////////////////////////////////////////////////
  // creating a refocusing sinus radiofrequency
  //////////////////////////////////////////////////////////////////////////////

  _rfRefocusingWaveformEnveloppe.setTypeRefocussing();
  _rfRefocusingWaveformEnveloppe.setDuration( 5120 );
  _rfRefocusingWaveformEnveloppe.setFlipAngle( 180.0 );
  _rfRefocusingWaveformEnveloppe.setInitialPhase( 0 );
  _rfRefocusingWaveformEnveloppe.setThickness(
                                    mrProt.sliceSeries().aFront().thickness() );
  _rfRefocusingWaveformEnveloppe.setSamples( 128 );
  _rfRefocusingWaveformEnveloppe.setBandwidthTimeProduct( 2.70 );
  _rfRefocusingWaveformEnveloppe.setRequiredGSPolarity( -1.0 );

  if( ! _rfRefocusingWaveformEnveloppe.prepSinc( mrProt, seqExpo ) ) 
  {

    return _rfRefocusingWaveformEnveloppe.getNLSStatus();

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a slice selection refocussing gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _sliceSelectionRefocusing.setAmplitude( 
                              _rfRefocusingWaveformEnveloppe.getGSAmplitude() );
  _sliceSelectionRefocusing.setRampTimes(fSDSRoundUpGRT( 
  std::max< long >(( long )( _minimumRiseTimePerMeter *
                             _sliceSelectionRefocusing.getAmplitude() ),
                                         SysProperties::getCoilCtrlLead() ) ) );
  _sliceSelectionRefocusing.setDuration( fSDSRoundUpGRT(
                                 _rfRefocusingWaveformEnveloppe.getDuration() +
                                 _sliceSelectionRefocusing.getRampUpTime() ) );


  if ( !_sliceSelectionRefocusing.prep() )  
  {
  
    return _sliceSelectionRefocusing.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // Computing moments for crushers
  //////////////////////////////////////////////////////////////////////////////
  
  double sliceSelectionCrusherMoment, readoutCrusherMoment, phaseCrusherMoment;
  double voxelResolution = ( mrProt.sliceSeries().front().readoutFOV() /
                             mrProt.kSpace().baseResolution() ) * 0.001; // m
  double lapsNumber = 4.0;
  double gamma = 42.57; // MHz/T
  // double crusherMomentum = ( 1000 * lapsNumber ) / 
                           // ( gamma * voxelResolution );  
  double crusherMomentum = ( 100 * lapsNumber ) / ( gamma * voxelResolution );  
  sliceSelectionCrusherMoment = crusherMomentum;
  readoutCrusherMoment = crusherMomentum;
  phaseCrusherMoment = crusherMomentum;
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a slice selection crusher gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _sliceSelectionCrusher.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _sliceSelectionCrusher.setMinRiseTime ( _minimumRiseTimePerMeter );

  if ( !_sliceSelectionCrusher.prepSymmetricTOTShortestTime(
                                                 sliceSelectionCrusherMoment ) )
  {
    
    return _sliceSelectionCrusher.getNLSStatus();
      
  }

  //////////////////////////////////////////////////////////////////////////////
  // creating a phase crusher gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _phaseCrusher.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phaseCrusher.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  if ( !_phaseCrusher.prepSymmetricTOTShortestTime( phaseCrusherMoment ) )
  {
    
    return _phaseCrusher.getNLSStatus();
      
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a readout crusher gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _readoutCrusher.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _readoutCrusher.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  
  if ( !_readoutCrusher.prepSymmetricTOTShortestTime( readoutCrusherMoment ) )
  {
    
    return _readoutCrusher.getNLSStatus();
      
  }
                             
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside the module
  //////////////////////////////////////////////////////////////////////////////
  
  long crusherTotalDuration = std::max( std::max( 
                                     _phaseCrusher.getDuration() +
                                     _phaseCrusher.getRampDownTime(),
                                     _readoutCrusher.getDuration() +
                                     _readoutCrusher.getRampDownTime() ), 
                                     _sliceSelectionCrusher.getDuration() +
                                     _sliceSelectionCrusher.getRampDownTime() );
  
  long firstCrusherStartTime = 0;
  _phaseCrusher.setStartTime( firstCrusherStartTime );
  
  long sliceSelectionRefocusingStartTime = fSDSRoundUpGRT(
                                  firstCrusherStartTime + 
                                  crusherTotalDuration );
  _sliceSelectionRefocusing.setStartTime( sliceSelectionRefocusingStartTime );

  long rfStartTime =  fSDSRoundUpGRT( sliceSelectionRefocusingStartTime +
                      ( ( _sliceSelectionRefocusing.getDuration() +
                      _sliceSelectionRefocusing.getRampDownTime() ) / 2 ) -
                      ( _rfRefocusingWaveformEnveloppe.getDuration() / 2 ) );
                      
  _rfRefocusingWaveformEnveloppe.setStartTime( rfStartTime );
  long rfRefocusingResetSynthetizerStartTime = fSDSRoundUpGRT(
                                 rfStartTime +
                                 _rfRefocusingWaveformEnveloppe.getDuration() );
  _rfRefocusingResetSynthetizer.setStartTime( 
                                        rfRefocusingResetSynthetizerStartTime );
  
  
  long secondCrusherStartTime = fSDSRoundUpGRT( 
                                sliceSelectionRefocusingStartTime + 
                                _sliceSelectionRefocusing.getDuration() +
                                _sliceSelectionRefocusing.getRampDownTime() );
  _sliceSelectionCrusher.setStartTime( secondCrusherStartTime );
  
  _anchorTime = sliceSelectionRefocusingStartTime + 
                ( _sliceSelectionRefocusing.getDuration() +
                  _sliceSelectionRefocusing.getRampDownTime() ) / 2;
  _durationBeforeAnchorTime = sliceSelectionRefocusingStartTime + 
                              ( _sliceSelectionRefocusing.getDuration() +
                              _sliceSelectionRefocusing.getRampDownTime() ) / 2;
  _durationAfterAnchorTime = 
                             secondCrusherStartTime +
                             crusherTotalDuration -
                             _durationBeforeAnchorTime;

  return status;

}


NLS_STATUS gkg::SimpleRefocusingModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{
    
  _rfRefocusingOffsets.prepSet( slices[ sliceIndex ],
                                _rfRefocusingWaveformEnveloppe );
  _rfRefocusingResetSynthetizer.prepNeg( slices[ sliceIndex ],
                                         _rfRefocusingWaveformEnveloppe );

  return SEQU__NORMAL;

}


NLS_STATUS gkg::SimpleRefocusingModule::setKernelTiming( MrProt& mrProt )
{


  NLS_STATUS status  = SEQU__NORMAL;

  // Sequence timing: 
  // fRTEI( StartTime(usec),  - 1000 to be aligned with the simulation axis
  //        NCO Event, 
  //        SRF Event, 
  //        ADC Event, 
  //        phase gradient Event, 
  //        read gradient Event, 
  //        slice gradient Event, 
  //        Sync ); 
  
  fRTEI( this->getStartTime() + 
         _rfRefocusingWaveformEnveloppe.getStartTime(),
         &_rfRefocusingOffsets,
         &_rfRefocusingWaveformEnveloppe,
         0,
         0,
         0,
         0,
         0 );
         
  fRTEI( this->getStartTime() + 
         _rfRefocusingResetSynthetizer.getStartTime(),
         &_rfRefocusingResetSynthetizer,
         0,
         0,
         0,
         0,
         0,
         0 );
         
  fRTEI( this->getStartTime(),
         0,
         0,
         0,
         &_phaseCrusher,
         &_readoutCrusher,
         &_sliceSelectionCrusher,
         0 );

  fRTEI( this->getStartTime() + 
         _sliceSelectionCrusher.getStartTime(),
         0,
         0,
         0,
         &_phaseCrusher,
         &_readoutCrusher,
         &_sliceSelectionCrusher,
         0 );
  
  fRTEI( this->getStartTime() + 
         _sliceSelectionRefocusing.getStartTime(),
         0,
         0,
         0,
         0,
         0,
         &_sliceSelectionRefocusing,
         0 );

  return status;

}

void gkg::SimpleRefocusingModule::setRfRefocusingSpoilPhase( 
                                                 double rfRefocusingSpoilPhase )
{
  
  _rfRefocusingSpoilPhase = rfRefocusingSpoilPhase;
  
}

const sRF_PULSE_SINC& 
gkg::SimpleRefocusingModule::getRfRefocusingWaveformEnveloppe() const
{
  
  return _rfRefocusingWaveformEnveloppe;
  
}

const sGRAD_PULSE& 
gkg::SimpleRefocusingModule::getSliceSelectionRefocusingGradientPulse() const
{
  
  return _sliceSelectionRefocusing;
  
}

std::string gkg::SimpleRefocusingModule::getStaticName()
{

  return "SimpleRefocusingModule";

}


//
// adding the SimpleRefocusingModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, SimpleRefocusingModule );
