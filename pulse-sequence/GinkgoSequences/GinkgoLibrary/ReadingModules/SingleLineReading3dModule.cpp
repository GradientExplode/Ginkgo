#include <ReadingModules/SingleLineReading3dModule.h>
#include <Core/SequenceModuleFactory.h>

#define OnErrorReturn( S ) if( !MrSucceeded( S ) ) return( S )


gkg::SingleLineReading3dModule::SingleLineReading3dModule( 
              const std::vector< double >& doubleParameters,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string,
                              gkg::RCPointer< gkg::SequenceModule > >&
                                                          otherSequenceModules )
                               : gkg::ReadingModule(
                                 gkg::ReadingModule::CartesianSingleLine2d, //??
                                 otherSequenceModules.find(
                                 "01-SingleTxExcitation3dModule" )->second ),
                                 // RF-signal axis

                                 // Slice gradient axis (Z)

                                 // Phase gradient axis (Y)
                                 _phaseEncoding( "phaseEncoding" ),

                                 // Read gradient axis (X)
                                 _readoutPreset( "readoutPreset" ),
                                 _readout( "readout" ),

                                 // ADC signal data axis
                                 _adcReadout( "adcReadout" ),
                                 _adcOffsets( "adcOffsets" ),
                                 _adcResetSynthetizer( "adcResetSynthetizer" )                                            
{
  
  _defaultTE = doubleParameters[ 0 ];
  _defaultBWPerPixel = doubleParameters[ 1 ];
  _defaultBaseResolution = doubleParameters[ 2 ];
  
}


gkg::SingleLineReading3dModule::~SingleLineReading3dModule()
{
}


void gkg::SingleLineReading3dModule::initialize( SeqLim& seqLim )
{
  
  UTRACE( Notice, 0, "+-+-+ Initialize read" );
  //////////////////////////////////////////////////////////////////////////////
  // calling initialize methode of the mother class
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::ReadingModule::initialize( seqLim );
  
  //////////////////////////////////////////////////////////////////////////////
  // initializing specific parameters
  //////////////////////////////////////////////////////////////////////////////
  
  seqLim.setDimension( SEQ::DIM_3 );
    
  seqLim.setPELines( 32,
                     1024,
                     1,
                     seqLim.getBaseResolution().getDef() );
  
  // Field of view in mm
  double maximumFOV = SysProperties::getFoVMax();
  double defaultFOV = 256.0;
  double incrementFOV = 1.0;
  double minimumFOV = 20;
  
  minimumFOV = fSDSdRoundUpMinimum( minimumFOV, maximumFOV, incrementFOV );
  if( defaultFOV < minimumFOV )
  { 
    
    defaultFOV = minimumFOV;
    
  }
  if( defaultFOV > maximumFOV )
  {
    
    defaultFOV = maximumFOV;
    
  }

  seqLim.setReadoutFOV( minimumFOV, 
                         maximumFOV, 
                         incrementFOV, 
                         defaultFOV );
  seqLim.setPhaseFOV( minimumFOV, 
                       maximumFOV, 
                       incrementFOV, 
                       defaultFOV );
              
  seqLim.setConcatenations( 1,
                            1, 
                            1, 
                            1 );

  
   seqLim.setSlices( 1,
                     K_NO_SLI_MAX, 
                     1, 
                     1 );                  
  
  seqLim.setSliceThickness( 2.000, 
                            10.000, 
                            0.500, 
                            5.000 );
  seqLim.setSliceDistanceFactor( 0.000, 
                                  8.000, 
                                  0.010, 
                                  0.200 );
  seqLim.setMultiSliceMode( SEQ::MSM_SEQUENTIAL );
  seqLim.setSliceSeriesMode( SEQ::ASCENDING,
                             SEQ::INTERLEAVED,
                             SEQ::DESCENDING );
  seqLim.enableSliceShift();
  seqLim.enableMSMA();
  seqLim.enableOffcenter();
  seqLim.setAllowedSliceOrientation( SEQ::DOUBLE_OBLIQUE );
  
  seqLim.setPartition( 8,
                       256,
                       2,
                       32 );
  seqLim.setImagesPerSlab( 8,
                           512,
                           2,
                           32 );
  seqLim.setSlabThickness( 5.000,
                           160.000 );
  seqLim.set3DPartThickness( 0.100,
                             30.000,
                             0.01,
                             seqLim.getSlabThickness().getMax() /
                             seqLim.getPartition().getDef() );
  seqLim.setSliceOversampling( 0.0000, 1.0000, 0.0100, 0.0000 );
  seqLim.setMinSliceResolution( 0.5 );
  
  
  seqLim.setPhases( 1,
                    K_NO_SLI_MAX,
                    1,
                    1 );
                              

}

 

NLSStatus gkg::SingleLineReading3dModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::SingleLineReading3dModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;
  
  UTRACE( Notice, 0, "+-+-+ _defaultTE " << _defaultTE );
  UTRACE( Notice, 0, "+-+-+ _defaultBWPerPixel " << _defaultBWPerPixel );
  UTRACE( Notice, 0, "+-+-+ _defaultBaseResolution " << _defaultBaseResolution );
  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::ReadingModule::prepare( mrProt,
                                     seqLim,
                                     seqExpo );

  if( ( ( mrProt.kSpace().phasePartialFourierFactor() == SEQ::PF_HALF ) ||
        ( mrProt.kSpace().phasePartialFourierFactor() == SEQ::PF_5_8 ) ) &&
        ( mrProt.calcRealPartImages() ) )
  {
   
    return SEQU_ERROR; 
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating an ADC gate
  //////////////////////////////////////////////////////////////////////////////

  _adcReadout.setColumns( mrProt.kSpace().getlBaseResolution() );
  _adcReadout.setDwellTime( long( ( mrProt.rxSpec().effDwellTime(
                                   seqLim.getReadoutOSFactor() )[0] ) + 0.5 ) );
                              

  //////////////////////////////////////////////////////////////////////////////
  // creating a readout gradient
  //////////////////////////////////////////////////////////////////////////////
  
  //~ _readout.setRampTimes( fSDSRoundUpGRT( _minimumRiseTimePerMeter *
                                         //~ _readout.getAmplitude() ) );
  _readout.setRampTimes (600) ;
  _readout.setDuration( fSDSRoundUpGRT( _adcReadout.getDuration()+
                                        _readout.getRampUpTime() ) );
       
  if( !_readout.prepRO( mrProt, _adcReadout.getDwellTime() ) )
  {
    
        return _readout.getNLSStatus();
        
  }
   
  if (! _readout.check() )
  {
    
      return (_readout.getNLSStatus());
      
  }


  //////////////////////////////////////////////////////////////////////////////
  // creating a phase encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phaseEncoding.setMinRiseTime( _minimumRiseTimePerMeter );

  // testing with one extremum of the gradient
  if( !_phaseEncoding.prepPEShortestTime( mrProt,
                                          SEQ::DIR_ASCENDING,
                                          0.0,
                                          _lineCount - _centerLineIndex - 1 ) )
  {

    return _phaseEncoding.getNLSStatus();

  }

  if( !_phaseEncoding.check() )
  {

    return _phaseEncoding.getNLSStatus();

  }

  int32_t phaseEncodingTotalTime =
                        static_cast< int32_t >( _phaseEncoding.getTotalTime() );

  // testing with another extremum of the gradient
  if( !_phaseEncoding.prepPEShortestTime( mrProt,
                                          SEQ::DIR_ASCENDING,
                                          0.0,
                                          - _centerLineIndex ) )
  {

    return _phaseEncoding.getNLSStatus();

  }

  if( !_phaseEncoding.check() )
  {

    return _phaseEncoding.getNLSStatus();

  }

  if ( phaseEncodingTotalTime > _phaseEncoding.getTotalTime() )
  {

    if( !_phaseEncoding.prepPEShortestTime( 
                                           mrProt,
                                           SEQ::DIR_ASCENDING,
                                           0.0,
                                           _lineCount - _centerLineIndex - 1 ) )
    {

      return _phaseEncoding.getNLSStatus();

    }

  }


  //////////////////////////////////////////////////////////////////////////////
  // creating a readout preset gradient
  //////////////////////////////////////////////////////////////////////////////

  _readoutPreset.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _readoutPreset.setMinRiseTime( _minimumRiseTimePerMeter );

  if ( !_readoutPreset.prepSymmetricTOTShortestTime(
                                      -_readout.getMomentum( 0,
                                      _readout.getRampUpTime() +
                                      ( long )_adcReadout.getDuration() / 2) ) )
  {

    return _readoutPreset.getNLSStatus();

  }
  
  if( ! _readoutPreset.check() )
  {
    
    return _readoutPreset.getNLSStatus();
    
  }


  //////////////////////////////////////////////////////////////////////////////
  // preparing timing
  //////////////////////////////////////////////////////////////////////////////

  long phaseEncodingDuration = _phaseEncoding.getDuration() + 
                               _phaseEncoding.getRampDownTime();
  long readoutPresetDuration = _readoutPreset.getDuration() + 
                               _readoutPreset.getRampDownTime();
  long readoutStartTime = 0;
  if ( phaseEncodingDuration > readoutPresetDuration )
  {

    _phaseEncoding.setStartTime( 0 );
    _readoutPreset.setStartTime( phaseEncodingDuration -
                                 readoutPresetDuration );
    readoutStartTime = phaseEncodingDuration;

  }
  else
  {

    _phaseEncoding.setStartTime( readoutPresetDuration - 
                                 phaseEncodingDuration );
    _readoutPreset.setStartTime( 0 );
    readoutStartTime = readoutPresetDuration;

  }

  _readout.setStartTime( readoutStartTime );

  long adcReadoutStartTime = readoutStartTime +
                             _readout.getRampUpTime();
  _adcReadout.setStartTime( adcReadoutStartTime );

  long adcReadoutEndTime = adcReadoutStartTime + 
                           ( long )_adcReadout.getRoundedDuration();

  _adcResetSynthetizer.setStartTime( adcReadoutEndTime );

  _anchorTime = adcReadoutStartTime + ( long )_adcReadout.getDuration() / 2;
  _durationBeforeAnchorTime = _anchorTime;
  _durationAfterAnchorTime = readoutStartTime + 
                             _readout.getDuration() +
                             _readout.getRampDownTime() -  
                             _durationBeforeAnchorTime;

  return status;

}


NLS_STATUS gkg::SingleLineReading3dModule::modifyKernel( MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;

  if( !_phaseEncoding.prepPE( mrProt,
                              lineIndex - _centerLineIndex ) )
  {

    return _phaseEncoding.getNLSStatus();

  }

  _adcOffsets.prepSet( slices[ sliceIndex ],
                       _adcReadout,
                       _readout,
                       lineIndex - _centerLineIndex,
                       partitionIndex - _centerPartitionIndex );
  _adcResetSynthetizer.prepNeg( slices[ sliceIndex ],
                                _adcReadout,
                                _readout,
                                lineIndex - _centerLineIndex,
                                partitionIndex - _centerPartitionIndex );
                                
  _adcOffsets.increasePhase( _excitationModule->getRfSpoilPhase() );
  _adcResetSynthetizer.decreasePhase( _excitationModule->getRfSpoilPhase() );

  return status;

}


NLS_STATUS gkg::SingleLineReading3dModule::setKernelTiming( MrProt& mrProt )
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
         _phaseEncoding.getStartTime(), 
         0, 
         0,
         0, 
         &_phaseEncoding, 
         0, 
         0,
         0 );

  fRTEI( this->getStartTime() +
         _readoutPreset.getStartTime(), 
         0, 
         0,
         0, 
         0, 
         &_readoutPreset, 
         0,
         0 );

  fRTEI( this->getStartTime() +
         _readout.getStartTime(),
         0,
         0,
         0,
         0, 
         &_readout,
         0,
         0 );

  fRTEI( this->getStartTime() +
         _adcReadout.getStartTime(),
         &_adcOffsets, 
         0, 
         &_adcReadout, 
         0, 
         0, 
         0, 
         0 );

  fRTEI( this->getStartTime() +
         _adcResetSynthetizer.getStartTime(),
         &_adcResetSynthetizer, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );

  return status;

}


const sGRAD_PULSE_PE& 
gkg::SingleLineReading3dModule::getPhaseEncodingGradientPulse() const
{

  return _phaseEncoding;

}

void gkg::SingleLineReading3dModule::setAdcSpoilPhase( double adcSpoilPhase )
{
  
  _adcSpoilPhase = adcSpoilPhase;
  
}

void 
gkg::SingleLineReading3dModule::setRepetitionIndex( long repetitionIndex )
{

  _repetitionIndex = repetitionIndex;

}


void gkg::SingleLineReading3dModule::setAdcReadoutRelevantForMeasureTime( 
                                          int32_t currentKernelCalls,
                                          int32_t kernelCallsPerRelevantSignal )
{

    if ( ! ( currentKernelCalls % kernelCallsPerRelevantSignal ) )
    {
        _adcReadout.setRelevantForMeasTime() ;
    }
  
}

void gkg::SingleLineReading3dModule::setAdcReadoutMDH( MrProt& mrProt,
                                                       int32_t lineIndex, 
                                                       int32_t partitionIndex )
{

  _adcReadout.getMDH().addToEvalInfoMask( MDH_OFFLINE );
  //~ _adcReadout.getMDH().addToEvalInfoMask( MDH_ONLINE );
  /////
  //~ _adcReadout.getMDH().setPartitionFT( true );
  _adcReadout.getMDH().setCphs( ( unsigned short ) 0 );
  _adcReadout.getMDH().setCacq( ( unsigned short ) 0 ) ; 
  /////
  
  _adcReadout.getMDH().setCrep( ( unsigned short ) _repetitionIndex ); 
  _adcReadout.getMDH().setClin( ( unsigned short ) lineIndex );
  //~ _adcReadout.getMDH().setCslc( ( unsigned short ) 0 ); 
  _adcReadout.getMDH().setCpar( ( unsigned short ) partitionIndex ); 
  
  _adcReadout.getMDH().setCseg( ( unsigned short ) 0 );
  _adcReadout.getMDH().setKSpaceCentreLineNo( ( unsigned short ) 
                                                             _centerLineIndex );
  _adcReadout.getMDH().setKSpaceCentrePartitionNo( ( unsigned short ) 
                                                        _centerPartitionIndex );
  _adcReadout.getMDH().setKSpaceCentreColumn( ( unsigned short )(
                                   mrProt.kSpace().getlBaseResolution() / 2 ) );
  
  
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    if( lineIndex <= _centerLineIndex + 10 &&
        lineIndex >= _centerLineIndex - 10 )
    {
      
      _adcReadout.getMDH().setPATRefScan( true );
      
      if( ( _repetitionIndex%2 == 0 && lineIndex%2 == 0 ) ||
          ( _repetitionIndex%2 != 0 && lineIndex%2 != 0 ) )
      {
      
      
        _adcReadout.getMDH().setPATRefAndImaScan( true );
      
      
      }
      else
      {
        
        
        _adcReadout.getMDH().setPATRefAndImaScan( false );
        
        
      }
      
    }
    else
    {
      
      
      _adcReadout.getMDH().setPATRefScan( false );
      _adcReadout.getMDH().setPATRefAndImaScan( false );
      
      
    }
      
  }
  
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    int32_t adjust;
    if( _repetitionIndex%2 == 0 )
    {
    
       adjust = 2;
    
    }
    else
    {
    
      adjust = 1;
    
    }
  
    if ( lineIndex == _startLineIndex )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( true );
      _adcReadout.getMDH().setLastScanInSlice( false );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
      _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    
    }
    else if ( lineIndex == _lineCount - adjust )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( true );
      //~ _adcReadout.getMDH().setPhaseFT( true );
      _adcReadout.getMDH().addToEvalInfoMask( MDH_D3FFT );
      _adcReadout.getMDH().setLastScanInConcat( true );
      _adcReadout.getMDH().setLastScanInMeas( true );
    
    }
    else
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( false );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
      //~ _adcReadout.getMDH().setPhaseFT( false );
      _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    
    }
    
  }
  else
  {
  
    if ( lineIndex == _startLineIndex )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( true );
      _adcReadout.getMDH().setLastScanInSlice( false );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
      _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    
    }
    else if ( partitionIndex == _partitionCount - 1 && 
              lineIndex == _lineCount - 1 )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( true );
      //~ _adcReadout.getMDH().setPhaseFT( true );
      _adcReadout.getMDH().addToEvalInfoMask( MDH_D3FFT );
      _adcReadout.getMDH().setLastScanInConcat( true );
      _adcReadout.getMDH().setLastScanInMeas( true );
    
    }
    else if ( lineIndex == _lineCount - 1 )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( false );
      //~ _adcReadout.getMDH().setPhaseFT( true );
      _adcReadout.getMDH().addToEvalInfoMask( MDH_D3FFT );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
    
    }
    else
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( false );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
      //~ _adcReadout.getMDH().setPhaseFT( false );
      _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    
    }
    
  }

    
}

long gkg::SingleLineReading3dModule::getTotalPhaseEncodingDuration() const
{
  
  return _phaseEncoding.getTotalTime();
  
}

const sGRAD_PULSE_RO& 
gkg::SingleLineReading3dModule::getReadout() const
{

  return _readout;

}


const sREADOUT& 
gkg::SingleLineReading3dModule::getAdcReadout() const
{

  return _adcReadout;

}

std::string gkg::SingleLineReading3dModule::getStaticName()
{

  return "SingleLineReading3dModule";

}

//
// adding the SingleLineReading3dModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, SingleLineReading3dModule );
