#include <CorrectionModules/EPIPhaseCorrectionModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::EPIPhaseCorrectionModule::EPIPhaseCorrectionModule( 
              const std::vector< double >& /* doubleParameters */,
              const std::vector< std::string >& /* stringParameters */,
              const std::map< std::string,
                              gkg::RCPointer< gkg::SequenceModule > >&
                                                          otherSequenceModules )
                               : gkg::CorrectionModule(),
                                 _repetitionIndex( 0 ),
                                 // RF-signal axis

                                 // Slice gradient axis (Z)

                                 // Phase gradient axis (Y)
                                 _phaseEncoding( "phaseEncoding" ),
                                 _phaseDecoding( "phaseDecoding" ),
                                 _grappaPhaseEncoding( "grappaPhaseEncoding" ),

                                 // Read gradient axis (X)
                                 _readoutPreEncoding( "readoutPreEncoding" ),
                                 _readoutUp( "readoutUp" ),
                                 _readoutDown( "readoutDown" ),

                                 // ADC signal data axis
                                 _adcReadout( "adcReadout" ),
                                 _adcOffsets( "adcOffsets" ),
                                 _adcResetSynthetizer( "adcResetSynthetizer" )                        
{
}


gkg::EPIPhaseCorrectionModule::~EPIPhaseCorrectionModule()
{
}


void gkg::EPIPhaseCorrectionModule::initialize( SeqLim& seqLim )
{ 
}

 

NLSStatus gkg::EPIPhaseCorrectionModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::EPIPhaseCorrectionModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::CorrectionModule::prepare( mrProt,
                                        seqLim,
                                        seqExpo );
                                  
  //////////////////////////////////////////////////////////////////////////////
  // creating an ADC gate
  //////////////////////////////////////////////////////////////////////////////
  
  _adcReadout.setColumns( mrProt.kSpace().getlBaseResolution() );
  _adcReadout.setDwellTime( long( ( mrProt.rxSpec().effDwellTime(
                                   seqLim.getReadoutOSFactor() )[0] ) + 0.5 ) );


  //////////////////////////////////////////////////////////////////////////////
  // creating a readout pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  if( !_readoutPreEncoding.prepRO( mrProt, // BW = Gamma * G * FOV
                         static_cast< double >( _adcReadout.getDwellTime() ) ) ) 
  {

    return _readoutPreEncoding.getNLSStatus();

  }


  _readoutPreEncoding.setRampTimes( fSDSRoundUpGRT( _minimumRiseTimePerMeter *
                                         _readoutPreEncoding.getAmplitude() ) );
                                         
  _readoutPreEncoding.setDuration( fSDSRoundUpGRT( ( double )( 
                                           _readoutPreEncoding.getRampUpTime() +
                                           _adcReadout.getDuration() ) ) );

  if( !_readoutPreEncoding.prep() )
  {

    return _readoutPreEncoding.getNLSStatus();

  }
  
  double readoutMomentum = _readoutPreEncoding.getMomentum( 
                                        0, 
                                        _readoutPreEncoding.getDuration() + 
                                        _readoutPreEncoding.getRampDownTime() );
  
  _readoutPreEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _readoutPreEncoding.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  if ( !_readoutPreEncoding.prepSymmetricTOTShortestTime( 
                                                         readoutMomentum / 2 ) )
  {
    
    return _readoutPreEncoding.getNLSStatus();
      
  }

  if( !_readoutPreEncoding.check() )
  {

    return _readoutPreEncoding.getNLSStatus();

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a readout up gradient
  //////////////////////////////////////////////////////////////////////////////
  
  if( !_readoutUp.prepRO( mrProt, // BW = Gamma * G * FOV
                         static_cast< double >( _adcReadout.getDwellTime() ) ) ) 
  {

    return _readoutUp.getNLSStatus();

  }


  _readoutUp.setRampTimes( fSDSRoundUpGRT( _minimumRiseTimePerMeter *
                                         _readoutUp.getAmplitude() ) );
                                         
  _readoutUp.setDuration( fSDSRoundUpGRT( ( double )( 
                                                _readoutUp.getRampUpTime() +
                                                _adcReadout.getDuration() ) ) );

  if( !_readoutUp.prep() )
  {

    return _readoutUp.getNLSStatus();

  }

  if( !_readoutUp.check() )
  {

    return _readoutUp.getNLSStatus();

  }

  //////////////////////////////////////////////////////////////////////////////
  // creating a readout down gradient
  //////////////////////////////////////////////////////////////////////////////
  
  if( !_readoutDown.prepRO( mrProt, // BW = Gamma * G * FOV
                        static_cast< double >( _adcReadout.getDwellTime() ) ) ) 
  {

    return _readoutDown.getNLSStatus();

  }
  
  _readoutDown.setRampTimes( fSDSRoundUpGRT( _minimumRiseTimePerMeter *
                                         _readoutDown.getAmplitude() ) );
  
  _readoutDown.setDuration( fSDSRoundUpGRT( ( double )( 
                                                _readoutDown.getRampUpTime() +
                                                _adcReadout.getDuration() ) ) );

  if( !_readoutDown.prep() )
  {

    return _readoutDown.getNLSStatus();

  }
  _readoutDown.setAmplitude( - _readoutDown.getAmplitude() );

  if( !_readoutDown.check() )
  {

    return _readoutDown.getNLSStatus();

  }

  //////////////////////////////////////////////////////////////////////////////
  // creating a phase encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phaseEncoding.setMinRiseTime( _minimumRiseTimePerMeter );
  if( !_phaseEncoding.prepPEShortestTime( mrProt,
                                          SEQ::DIR_ASCENDING,
                                          0.0,
                                          _centerLineIndex  ) )
  {

    return _phaseEncoding.getNLSStatus();

  }

  if( !_phaseEncoding.check() )
  {

    return _phaseEncoding.getNLSStatus();

  }
  
  long phaseEncodingDuration = _phaseEncoding.getDuration() +
                               _phaseEncoding.getRampDownTime();
                               
  //////////////////////////////////////////////////////////////////////////////
  // creating a phase decoding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseDecoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phaseDecoding.setMinRiseTime( _minimumRiseTimePerMeter );
  if( !_phaseDecoding.prepPEShortestTime( mrProt,
                                          SEQ::DIR_ASCENDING,
                                          0.0,
                                          - _centerLineIndex  ) )
  {

    return _phaseDecoding.getNLSStatus();

  }

  if( !_phaseDecoding.check() )
  {

    return _phaseDecoding.getNLSStatus();

  }
  
  long phaseDecodingDuration = _phaseDecoding.getDuration() +
                               _phaseDecoding.getRampDownTime();

  //////////////////////////////////////////////////////////////////////////////
  // preparing timing
  //////////////////////////////////////////////////////////////////////////////
  
  _adcShift = 0;
  long minDistanceBetweenRO = fSDSRoundUpGRT( 
                                       _adcReadout.getMinDistanceBetweenReadout( 
                                       _adcReadout, true, mrProt ) );
    
  if( _readoutUp.getRampDownTime() +
      _phaseEncoding.getDuration() +
      _phaseEncoding.getRampDownTime() < 
      minDistanceBetweenRO )
  {
    
    _adcShift = minDistanceBetweenRO -
                _readoutUp.getRampDownTime() +
                _phaseEncoding.getDuration() +
                _phaseEncoding.getRampDownTime();
                
  }
                               
   
  _phaseEncoding.setStartTime( 0 ); 
  _readoutPreEncoding.setStartTime( 0 ); 
  _grappaPhaseEncoding.setStartTime( 0 ); 
  
  long readoutStartTime = 0;
    
  readoutStartTime = std::max( _readoutPreEncoding.getDuration() +
                                _readoutPreEncoding.getRampDownTime(),
                                _phaseEncoding.getDuration() +
                                _phaseEncoding.getRampDownTime() ) +
                      _adcShift / 2;
  
  _readoutUp.setStartTime( readoutStartTime );
  _readoutDown.setStartTime( readoutStartTime );

  long adcReadoutStartTime = readoutStartTime +
                             std::max( _readoutUp.getDuration() +
                                       _readoutUp.getRampDownTime(),
                                       _readoutDown.getDuration() +
                                       _readoutDown.getRampDownTime() ) / 2 -
                             ( _adcReadout.getDuration() / 2 );
  _adcReadout.setStartTime( adcReadoutStartTime );

  long adcReadoutEndTime = adcReadoutStartTime + 
                           ( long )_adcReadout.getRoundedDuration();

  _adcResetSynthetizer.setStartTime( adcReadoutEndTime );
  
  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;

  _durationAfterAnchorTime = std::max( _readoutPreEncoding.getDuration() +
                                        _readoutPreEncoding.getRampDownTime(),
                                        _phaseEncoding.getDuration() +
                                        _phaseEncoding.getRampDownTime() ) +
                              2 *( _readoutUp.getDuration() +
                                  _readoutUp.getRampDownTime() ) +
                              _readoutDown.getDuration() +
                              _readoutDown.getRampDownTime() +
                              _phaseDecoding.getDuration() +
                              _phaseDecoding.getRampDownTime() +
                              3 * _adcShift; 
                              
  
 
  return status;

}


NLS_STATUS gkg::EPIPhaseCorrectionModule::modifyKernel( MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;
  
  _slices = slices;
  _lineIndex = lineIndex;
  _sliceIndex = sliceIndex;
  
  _adcOffsets.prepSet( slices[ sliceIndex ], 
                        _adcReadout, 
                        _readoutDown,
                        0, 
                        0 );
  _adcResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                _adcReadout, 
                                _readoutDown,
                                0,
                                0 );
                                
  this->setAdcCorrectionMDH( mrProt, 0 );
  
  return status;

}


NLS_STATUS gkg::EPIPhaseCorrectionModule::setKernelTiming( MrProt& mrProt )
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
  
  long minDistanceBetweenRO = fSDSRoundUpGRT( 
                                       _adcReadout.getMinDistanceBetweenReadout( 
                                       _adcReadout, true, mrProt ) );
    
  long adcReadoutStartTime = _adcReadout.getStartTime();
  long adcReadoutEndTime = adcReadoutStartTime + 
                           ( long )_adcReadout.getRoundedDuration();
                           
  bool partialFourierDownLine = true;
  bool partialFourierUpLine = true;

  double firstShotDuration = _readoutUp.getDuration() +
                             _readoutUp.getRampDownTime() +
                             _adcShift; 
  double secondShotDuration = _readoutDown.getDuration() +
                              _readoutDown.getRampDownTime() +
                              _adcShift; 
         
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         _readoutPreEncoding.getStartTime() ),
         0,
         0,
         0,
         0, 
         &_readoutPreEncoding,
         0,
         0 );
  
  // First shot
  _adcOffsets.prepSet( _slices[ _sliceIndex ], 
                       _adcReadout, 
                       _readoutDown,
                       0, 
                       0 );
  _adcResetSynthetizer.prepNeg( _slices[ _sliceIndex ], 
                                _adcReadout, 
                                _readoutDown,
                                0,
                                0 );
  this->setAdcCorrectionMDH( mrProt, 0 );      
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         _readoutUp.getStartTime() ),
         0,
         0,
         0,
         0, 
         &_readoutDown,
         0,
         0 );
               
  fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
         0, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );

  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutStartTime ),
         &_adcOffsets, 
         0, 
         &_adcReadout, 
         0, 
         0, 
         0, 
         0 );
    
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutEndTime ),
         &_adcResetSynthetizer, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );
         
  // Second shot
  _adcOffsets.prepSet( _slices[ _sliceIndex ], 
                       _adcReadout, 
                       _readoutUp,
                       0, 
                       0 );
  _adcResetSynthetizer.prepNeg( _slices[ _sliceIndex ], 
                                _adcReadout, 
                                _readoutUp,
                                0,
                                0 );
  this->setAdcCorrectionMDH( mrProt, 1 );
      
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         _readoutDown.getStartTime() +
         firstShotDuration ),
         0,
         0,
         0,
         0, 
         &_readoutUp,
         0,
         0 );

  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutStartTime +
         firstShotDuration ),
         &_adcOffsets, 
         0, 
         &_adcReadout, 
         0, 
         0, 
         0, 
         0 );
    
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutEndTime +
         firstShotDuration ),
         &_adcResetSynthetizer, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );
         
  // Third shot
  _adcOffsets.prepSet( _slices[ _sliceIndex ], 
                       _adcReadout, 
                       _readoutDown,
                       0, 
                       0 );
  _adcResetSynthetizer.prepNeg( _slices[ _sliceIndex ], 
                                _adcReadout, 
                                _readoutDown,
                                0,
                                0 );
  this->setAdcCorrectionMDH( mrProt, 2 );
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         _readoutUp.getStartTime() +
         firstShotDuration +
         secondShotDuration ),
         0,
         0,
         0,
         0, 
         &_readoutDown,
         0,
         0 );

  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutStartTime +
         firstShotDuration +
         secondShotDuration ),
         &_adcOffsets, 
         0, 
         &_adcReadout, 
         0, 
         0, 
         0, 
         0 );
    
  fRTEI( fSDSRoundUpGRT( this->getStartTime() +
         adcReadoutEndTime +
         firstShotDuration +
         secondShotDuration ),
         &_adcResetSynthetizer, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );
          
  fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
         0, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );

  return status;

}


const sGRAD_PULSE_PE& 
gkg::EPIPhaseCorrectionModule::getPhaseEncodingGradientPulse() const
{

  return _phaseEncoding;

}

const sGRAD_PULSE_PE& 
gkg::EPIPhaseCorrectionModule::getGrappaPhaseEncodingGradientPulse() const
{

  return _grappaPhaseEncoding;

}

const sGRAD_PULSE_RO& 
gkg::EPIPhaseCorrectionModule::getReadoutUpGradientPulse() const
{

  return _readoutUp;

}

const sREADOUT& gkg::EPIPhaseCorrectionModule::getAdcReadout() const
{

  return _adcReadout;

}

void gkg::EPIPhaseCorrectionModule::setAdcSpoilPhase( double adcSpoilPhase )
{
  
  _adcSpoilPhase = adcSpoilPhase;
  
}

void 
gkg::EPIPhaseCorrectionModule::setRepetitionIndex( long repetitionIndex )
{

  _repetitionIndex = repetitionIndex;

}


void gkg::EPIPhaseCorrectionModule::setAdcReadoutRelevantForMeasureTime( 
                                                       long currentKernelCalls,
                                                       long lineCountPerSecond )
{

    _adcReadout.setRelevantForMeasTime();
  
}

void gkg::EPIPhaseCorrectionModule::setAdcCorrectionMDH( MrProt& mrProt,
                                                           long shotIndex )
{

   _adcReadout.getMDH().addToEvalInfoMask( MDH_OFFLINE );
  if( _grappaFlag == true )
  {
    
    _adcReadout.getMDH().setPATRefScan( true );
    
  }
  else
  {
    
    _adcReadout.getMDH().setPATRefScan( false );
    
  }
  
  _adcReadout.getMDH().setCrep( _repetitionIndex ); 
  _adcReadout.getMDH().setClin( _centerLineIndex );
  _adcReadout.getMDH().setCslc( _sliceIndex );
  _adcReadout.getMDH().setKSpaceCentreLineNo( _centerLineIndex );
  _adcReadout.getMDH().setKSpaceCentreColumn( ( unsigned short )(
                                   mrProt.kSpace().getlBaseResolution() / 2 ) );

  if ( shotIndex == 0 )
  {
    
    _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
    _adcReadout.getMDH().addToEvalInfoMask( MDH_PHASCOR );
    _adcReadout.getMDH().setFirstScanInSlice( false ); // true?
    _adcReadout.getMDH().setLastScanInSlice( false );
    _adcReadout.getMDH().setLastScanInConcat( false );
    _adcReadout.getMDH().setLastScanInMeas( false );
    //~ _adcReadout.getMDH().setPhaseFT( false );
    _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    _adcReadout.getMDH().setCseg( 1 );
    _adcReadout.getMDH().setCacq( 0 );
    
  }
  else if ( shotIndex == 1 )
  {
    
    _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_REFLECT );
    _adcReadout.getMDH().addToEvalInfoMask( MDH_PHASCOR );
    _adcReadout.getMDH().setFirstScanInSlice( false );
    _adcReadout.getMDH().setLastScanInSlice( false );
    _adcReadout.getMDH().setLastScanInConcat( false );
    _adcReadout.getMDH().setLastScanInMeas( false );
    //~ _adcReadout.getMDH().setPhaseFT( false );
    _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    _adcReadout.getMDH().setCseg( 0 );
    _adcReadout.getMDH().setCacq( 0 );
    
    
  } 
  else
  {
    
    _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
    _adcReadout.getMDH().addToEvalInfoMask( MDH_PHASCOR );
    _adcReadout.getMDH().setFirstScanInSlice( false );
    _adcReadout.getMDH().setLastScanInSlice( false );
    _adcReadout.getMDH().setLastScanInConcat( false );
    _adcReadout.getMDH().setLastScanInMeas( false );
    //~ _adcReadout.getMDH().setPhaseFT( false );
    _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    _adcReadout.getMDH().setCseg( 1 );
    _adcReadout.getMDH().setCacq( 1 );
    
    
  } 
    
}

long gkg::EPIPhaseCorrectionModule::getAdcShift()
{
  
  return _adcShift;
  
}

std::string gkg::EPIPhaseCorrectionModule::getStaticName()
{

  return "EPIPhaseCorrectionModule";

}


//
// adding the EPIPhaseCorrectionModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, EPIPhaseCorrectionModule );
