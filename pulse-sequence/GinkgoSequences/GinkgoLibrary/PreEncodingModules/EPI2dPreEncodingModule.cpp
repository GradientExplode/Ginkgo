#include <PreEncodingModules/EPI2dPreEncodingModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::EPI2dPreEncodingModule::EPI2dPreEncodingModule( 
           const std::vector< double >& /* doubleParameters */,
           const std::vector< std::string >& /* stringParameters */,
           const std::map< std::string, 
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
           : gkg::PreEncodingModule(),
           // RF-signal axis
           
           // Slice gradient axis (Z)
           
           // Phase gradient axis (Y)
           _phasePreEncoding( "phasePreEncoding" ),
           
           // Read gradient axis (X)
           _readoutPreEncoding( "readoutPreEncoding" )
           
           // ADC signal data axis                       
{
}


gkg::EPI2dPreEncodingModule::~EPI2dPreEncodingModule()
{
}


void gkg::EPI2dPreEncodingModule::initialize( SeqLim& seqLim )
{
  
  seqLim.setDimension( SEQ::DIM_2 );
    
  seqLim.setPELines( 32,
                     1024,
                     1,
                     seqLim.getBaseResolution().getDef() );
              
  // Field of view settings in mm
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
  seqLim.setSlices( 1,
                    K_NO_SLI_MAX, 
                    1, 
                    5 );
  seqLim.setSliceThickness( 0.500, 
                            100.000, 
                            0.500, 
                            1.000 );
  seqLim.setSliceDistanceFactor( 0.000, 
                                  8.000, 
                                  0.010, 
                                  0.200 );
                                  
  seqLim.setSliceSeriesMode( SEQ::ASCENDING,
                             SEQ::INTERLEAVED,
                             SEQ::DESCENDING );
  seqLim.enableSliceShift();
  seqLim.enableMSMA();
  seqLim.enableOffcenter();
  seqLim.setAllowedSliceOrientation( SEQ::DOUBLE_OBLIQUE );
  
  seqLim.setPhases( 1,
                    K_NO_SLI_MAX,
                    1,
                    1 );
                              

}

 

NLSStatus gkg::EPI2dPreEncodingModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::EPI2dPreEncodingModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );

  //////////////////////////////////////////////////////////////////////////////
  // creating a phase pre-encoding gradient
  //////////////////////////////////////////////////////////////////////////////

  // _phasePreEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _phasePreEncoding.setMaxMagnitude( 10 );
  _phasePreEncoding.setMinRiseTime( _minimumRiseTimePerMeter );

  if( !_phasePreEncoding.prepPEShortestTime( mrProt,
                                             SEQ::DIR_ASCENDING,
                                             0.0,
                                             _lineCount / 2  ) )
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
  // creating an ADC gate
  //////////////////////////////////////////////////////////////////////////////

  _adcReadout.setColumns( mrProt.kSpace().getlBaseResolution() );
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
  // preparing timing
  //////////////////////////////////////////////////////////////////////////////

  long readoutPreEncodingStartTime = 0;
  long phasePreEncodingStartTime = 0;
  long readoutPreEncodingTotalDuration = _readoutPreEncoding.getDuration() +
                                         _readoutPreEncoding.getRampDownTime();
  long phasePreEncodingTotalDuration = _phasePreEncoding.getDuration() +
                                       _phasePreEncoding.getRampDownTime();
  
  _readoutPreEncoding.setStartTime( readoutPreEncodingStartTime );
  _phasePreEncoding.setStartTime( phasePreEncodingStartTime );

  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;
  _durationAfterAnchorTime = std::max( readoutPreEncodingTotalDuration,
                                       phasePreEncodingTotalDuration );

  return status;

}


NLS_STATUS gkg::EPI2dPreEncodingModule::modifyKernel( MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;

  return status;

}


NLS_STATUS gkg::EPI2dPreEncodingModule::setKernelTiming( MrProt& mrProt )
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

  if( _grappaFlag == true )
  {
    
    fRTEI( this->getStartTime() +
            _phasePreEncoding.getStartTime(), 
            0, 
            0,
            0, 
            &_grappaPhasePreEncoding, 
            0, //&_readoutPreEncoding, 
            0,
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
            0, //&_readoutPreEncoding, 
            0,
            0 );
            
  }
  
  return status;

}

const 
sGRAD_PULSE_RO& gkg::EPI2dPreEncodingModule::getReadoutPresetGradientPulse() const
{
  
  return _readoutPreEncoding;
  
}

const 
sGRAD_PULSE_PE& gkg::EPI2dPreEncodingModule::getPhasePresetGradientPulse() const
{
  
  return _phasePreEncoding;
  
}

const sGRAD_PULSE_PE& 
gkg::EPI2dPreEncodingModule::getGrappaPhasePresetGradientPulse() const
{
  
  return _grappaPhasePreEncoding;
  
}

std::string gkg::EPI2dPreEncodingModule::getStaticName()
{

  return "EPI2dPreEncodingModule";

}

//
// adding the EPI2dPreEncodingModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, EPI2dPreEncodingModule );
