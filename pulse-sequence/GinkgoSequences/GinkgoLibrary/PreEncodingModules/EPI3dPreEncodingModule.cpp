#include <PreEncodingModules/EPI3dPreEncodingModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::EPI3dPreEncodingModule::EPI3dPreEncodingModule( 
           const std::vector< double >& /* doubleParameters */,
           const std::vector< std::string >& /* stringParameters */,
           const std::map< std::string, 
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
           : gkg::PreEncodingModule(),
           // RF-signal axis
           
           // Slice gradient axis (Z)
           _partitionPreEncoding( "partitionPreEncoding" ),
           
           // Phase gradient axis (Y)
           _phasePreEncoding( "phasePreEncoding" ),
           
           // Read gradient axis (X)
           _readoutPreEncoding( "readoutPreEncoding" ),
           _inverseReadoutPreEncoding( "inverseReadoutPreEncoding" ),
           
           // ADC signal data axis 
           _adcReadout( "adcReadout" )                      
{
}


gkg::EPI3dPreEncodingModule::~EPI3dPreEncodingModule()
{
}


void gkg::EPI3dPreEncodingModule::initialize( SeqLim& seqLim )
{
  
  //////////////////////////////////////////////////////////////////////////////
  // initializing specific parameters
  //////////////////////////////////////////////////////////////////////////////
  
  seqLim.setDimension( SEQ::DIM_3 );
    
  seqLim.setBaseResolution( 32,
                            512,
                            SEQ::INC_16,
                            256 );
  
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
                            K_NO_SLI_MAX, 
                            1, 
                            1 );

  seqLim.enableSliceShift();
  seqLim.enableMSMA();
  seqLim.enableOffcenter();
  seqLim.setAllowedSliceOrientation( SEQ::DOUBLE_OBLIQUE );
  
  seqLim.setPhases( 1,
                    K_NO_SLI_MAX,
                    1,
                    1 );
                              

}

 

NLSStatus gkg::EPI3dPreEncodingModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::EPI3dPreEncodingModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a partition pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _partitionPreEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _partitionPreEncoding.setMinRiseTime( _minimumRiseTimePerMeter );
  if ( !_partitionPreEncoding.prep3DShortestTime( mrProt,
                                                  SEQ::DIR_ASCENDING,
                                                  0.0,
                                                  - _centerPartitionIndex ) )
  {
  
    return _partitionPreEncoding.getNLSStatus();
  
  }
  if( !_partitionPreEncoding.prep() )
  {

    return _partitionPreEncoding.getNLSStatus();

  }
 
  //////////////////////////////////////////////////////////////////////////////
  // creating a phase pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _phasePreEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phasePreEncoding.setMinRiseTime( _minimumRiseTimePerMeter );

  if( !_phasePreEncoding.prepPEShortestTime( mrProt,
                                             SEQ::DIR_ASCENDING,
                                             0.0,
                                             _startLine ) )
  {

    return _phasePreEncoding.getNLSStatus();

  }

  if( !_phasePreEncoding.check() )
  {

    return _phasePreEncoding.getNLSStatus();

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a grappa phase pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  // _grappaPhasePreEncoding.setMaxMagnitude( 
                                         // _absoluteMaximumGradientAmplitude );
  _grappaPhasePreEncoding.setMaxMagnitude( 10 );
  _grappaPhasePreEncoding.setMinRiseTime( _minimumRiseTimePerMeter );

  if( !_grappaPhasePreEncoding.prepPEShortestTime( mrProt,
                                                   SEQ::DIR_ASCENDING,
                                                   0.0,
                                                   _lineCount / 4  ) )
  {

    return _grappaPhasePreEncoding.getNLSStatus();

  }

  if( !_grappaPhasePreEncoding.check() )
  {

    return _grappaPhasePreEncoding.getNLSStatus();

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating an ADC gate (used only for calculation)
  //////////////////////////////////////////////////////////////////////////////

  _adcReadout.setColumns( long( mrProt.kSpace().getlBaseResolution() + 0.5 ) );
  _adcReadout.setDwellTime( long( ( mrProt.rxSpec().effDwellTime(
                                   seqLim.getReadoutOSFactor() )[0] ) + 0.5 ) );

  //////////////////////////////////////////////////////////////////////////////
  // creating a readout pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  if( !_readoutPreEncoding.prepRO( 
                         mrProt, // BW = Gamma * G * FOV
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
  
  if( !_readoutPreEncoding.prepSymmetricTOTShortestTime( readoutMomentum / 2 ) )
  {
    
    return _readoutPreEncoding.getNLSStatus();
      
  }

  if( !_readoutPreEncoding.check() )
  {

    return _readoutPreEncoding.getNLSStatus();

  }
  

  //////////////////////////////////////////////////////////////////////////////
  // creating an inverse readout pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////
  _inverseReadoutPreEncoding = _readoutPreEncoding;
  _inverseReadoutPreEncoding.setAmplitude( 
                                         - _readoutPreEncoding.getAmplitude() );
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing
  //////////////////////////////////////////////////////////////////////////////

  long readoutPreEncodingStartTime = 0;
  long phasePreEncodingStartTime = 0;
  long partitionPreEncodingStartTime = 0;
  long readoutPreEncodingTotalDuration = _readoutPreEncoding.getDuration() +
                                         _readoutPreEncoding.getRampDownTime();
  long phasePreEncodingTotalDuration = _phasePreEncoding.getDuration() +
                                       _phasePreEncoding.getRampDownTime();
  long partitionPreEncodingTotalDuration = _partitionPreEncoding.getDuration() +
                                       _partitionPreEncoding.getRampDownTime();
  
  _readoutPreEncoding.setStartTime( readoutPreEncodingStartTime );
  _phasePreEncoding.setStartTime( phasePreEncodingStartTime );
  _partitionPreEncoding.setStartTime( partitionPreEncodingStartTime );

  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;
  _durationAfterAnchorTime = std::max( std::max( 
                                            readoutPreEncodingTotalDuration,
                                            phasePreEncodingTotalDuration ),
                                            partitionPreEncodingTotalDuration );

  return status;

}


NLS_STATUS gkg::EPI3dPreEncodingModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;

  _partitionIndex = partitionIndex;
  _lineIndex = lineIndex; 
  
  if( !_partitionPreEncoding.prep3D( mrProt,
                                     partitionIndex - _centerPartitionIndex  ) )
  {
  
    return _partitionPreEncoding.getNLSStatus();
  
  }
  if( !_partitionPreEncoding.prep() )
  {

    return _partitionPreEncoding.getNLSStatus();

  }

  return status;

}


NLS_STATUS gkg::EPI3dPreEncodingModule::setKernelTiming( MrProt& mrProt )
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
  
  int32_t firstLine = _startLine;
  
  if( _grappaFlag == true )
  {
    
    if( _lineIndex == _startLineIndex )
    {
    
      fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              _grappaPhasePreEncoding.getStartTime() ), 
              0, 
              0,
              0, 
              &_grappaPhasePreEncoding, 
              &_readoutPreEncoding, 
              &_partitionPreEncoding,
              0 );
              
    }
    else
    {
    
      fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              _grappaPhasePreEncoding.getStartTime() ), 
              0, 
              0,
              0, 
              &_grappaPhasePreEncoding, 
              &_readoutPreEncoding, 
              0,
              0 );
              
    }
            
  }
  else
  {
    
    if( _lineIndex == _startLineIndex )
    {
    
      fRTEI( this->getStartTime() +
              _phasePreEncoding.getStartTime(), 
              0, 
              0,
              0, 
              &_phasePreEncoding, 
              &_readoutPreEncoding, 
              &_partitionPreEncoding,
              0 );
              
    }
    else
    {
    
      fRTEI( this->getStartTime() +
              _phasePreEncoding.getStartTime(), 
              0, 
              0,
              0, 
              &_phasePreEncoding, 
              &_readoutPreEncoding, 
              0,
              0 );
              
    }
            
  }
   
  //~ if( _lineIndex == _startLineIndex )
  //~ {
    
    //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
            //~ _phasePreEncoding.getStartTime() ), 
            //~ 0, 
            //~ 0,
            //~ 0, 
            //~ &_phasePreEncoding, 
            //~ &_readoutPreEncoding, 
            //~ &_partitionPreEncoding,
            //~ 0 );
            
    //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() +
            //~ _phasePreEncoding.getStartTime() ), 
            //~ 0, 
            //~ 0,
            //~ 0, 
            //~ 0, 
            //~ 0, 
            //~ 0,
            //~ 0 );
            
  //~ }
  
  //~ if( mrProt.PAT().getucPATMode() == SEQ::PAT_MODE_GRAPPA )
  //~ {
    
    //~ if( _lineIndex == _lineCount - 1 )
    //~ {
      
      //~ if( _centerLineIndex%2 == 0 )
      //~ {
      
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              //~ _phasePreEncoding.getStartTime() ), 
              //~ 0, 
              //~ 0,
              //~ 0, 
              //~ 0, 
              //~ &_readoutPreEncoding, 
              //~ 0,
              //~ 0 );
              
      //~ }
      //~ else
      //~ {
      
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              //~ _phasePreEncoding.getStartTime() ), 
              //~ 0, 
              //~ 0,
              //~ 0, 
              //~ 0, 
              //~ &_inverseReadoutPreEncoding, 
              //~ 0,
              //~ 0 );
              
      //~ }
      
      //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() +
            //~ _phasePreEncoding.getStartTime() ), 
            //~ 0, 
            //~ 0,
            //~ 0, 
            //~ 0, 
            //~ 0, 
            //~ 0,
            //~ 0 );
      
    //~ }
    
  //~ }

  return status;

}

const sGRAD_PULSE_RO& 
gkg::EPI3dPreEncodingModule::getReadoutPresetGradientPulse() const
{
  
  return _readoutPreEncoding;
  
}

const sGRAD_PULSE_PE& 
gkg::EPI3dPreEncodingModule::getPhasePresetGradientPulse() const
{
  
  return _phasePreEncoding;
  
}

const sGRAD_PULSE_3D&
gkg::EPI3dPreEncodingModule::getPartitionPreEncodingGradientPulse() const
{
  
  return _partitionPreEncoding;
  
}

const sGRAD_PULSE_PE& 
gkg::EPI3dPreEncodingModule::getGrappaPhasePresetGradientPulse() const
{
  
  return _grappaPhasePreEncoding;
  
}

std::string gkg::EPI3dPreEncodingModule::getStaticName()
{

  return "EPI3dPreEncodingModule";

}

std::string gkg::EPI3dPreEncodingModule::getName()
{

  return "EPI3dPreEncodingModule";

}


//
// adding the EPI3dPreEncodingModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, EPI3dPreEncodingModule );
