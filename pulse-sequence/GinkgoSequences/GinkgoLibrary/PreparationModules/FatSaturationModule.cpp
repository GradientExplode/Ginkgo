#include <PreparationModules/FatSaturationModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::FatSaturationModule::FatSaturationModule(
           const std::vector< double >& /* doubleParameters */,
           const std::vector< std::string >& /* stringParameters */,
           const std::map< std::string, 
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
                         : gkg::SequenceModule(),
    _rfFatSaturationWaveformEnveloppe( "rfFatSaturationWaveformEnveloppe" ),
    _sliceSelectionFatSaturationSpoil1( "sliceSelectionFatSaturationSpoil1" ),
    _sliceSelectionFatSaturationSpoil2( "sliceSelectionFatSaturationSpoil2" ),
    _phaseFatSaturationSpoil1( "phaseFatSaturationSpoil1" ),
    _phaseFatSaturationSpoil2( "phaseFatSaturationSpoil2" ),
    _readoutFatSaturationSpoil1( "readoutFatSaturationSpoil1" ),
    _readoutFatSaturationSpoil2( "readoutFatSaturationSpoil2" ),
    _rfFatSaturationOffsets( "rfFatSaturationOffsets" ),
    _rfFatSaturationResetSynthetizer( "rfFatSaturationResetSynthetizer" ),
    _sliceSelectionFatSaturationCrusher( "sliceSelectionFatSaturationCrusher" ),
    _phaseFatSaturationCrusher( "phaseFatSaturationCrusher" ),
    _iterations( 1 ),
    _unitDuration( 0. )
{
}


gkg::FatSaturationModule::~FatSaturationModule()
{
}


void gkg::FatSaturationModule::initialize( SeqLim& pSeqLim )
{
  
  //////////////////////////////////////////////////////////////////////////////
  // disabling unit test errors on fat saturation module
  //////////////////////////////////////////////////////////////////////////////
  
  if( mIsUnittestActive() )
  { 
    
    SeqUT.DisableTestCase( lFrequNotEquOmegaRFErr,
                           RTEB_ORIGIN_fSEQRunKernel,
                           "Using fat saturation pulse ");
    SeqUT.DisableTestCase( lRFAmplValErr,
                           RTEB_ORIGIN_fSEQRunKernel,
                           "Using fat saturation pulse ");
  }
  
  _frequencyOffset = 0; //-420;
  _fatSaturationFlipAngle = 0; //90;
  //~ double bandwidth = 450;
  //~ _rfDuration = 0; //10240;
  //~ long samples = 512;
  
}


NLSStatus gkg::FatSaturationModule::prepare( MrProt& mrProt,
                                      SeqLim& seqLim,
                                      MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::FatSaturationModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // reading parameters from file
  //////////////////////////////////////////////////////////////////////////////
  
  //~ std::string line; 
  //~ _frequencyOffset = -420;
  //~ _fatSaturationFlipAngle = 90;
  double bandwidth = 450;
  _rfDuration = 10240;
  long samples = 512;
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a fat saturation excitation sinus radiofrequency
  //////////////////////////////////////////////////////////////////////////////
  
  _rfFatSaturationWaveformEnveloppe.setTypeExcitation();
  _rfFatSaturationWaveformEnveloppe.setDuration( _rfDuration );

  _rfFatSaturationWaveformEnveloppe.setFlipAngle( _fatSaturationFlipAngle );
  _rfFatSaturationWaveformEnveloppe.setInitialPhase( 90 );  

  _rfFatSaturationWaveformEnveloppe.setThickness(
                                    mrProt.sliceSeries().aFront().thickness() );

  _rfFatSaturationWaveformEnveloppe.setSamples( samples ); 
  _rfFatSaturationWaveformEnveloppe.setBandwidthTimeProduct( bandwidth * 
                                                             _rfDuration *
                                                             1E-6 );

  if( !_rfFatSaturationWaveformEnveloppe.prepSinc( mrProt, seqExpo ) )
  {

    return _rfFatSaturationWaveformEnveloppe.getNLSStatus();

  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // creating slice selection spoiling gradients
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelectionFatSaturationSpoil1.setRampTimes( 500 );
  _sliceSelectionFatSaturationSpoil1.setDuration( 3000 );
  
  if( !_sliceSelectionFatSaturationSpoil1.prepAmplitude( -8.0 ) )
  {

    return _sliceSelectionFatSaturationSpoil1.getNLSStatus();

  }
  
  if( ! _sliceSelectionFatSaturationSpoil1.check() )
  {
    
    return _sliceSelectionFatSaturationSpoil1.getNLSStatus();
    
  }
  
  _sliceSelectionFatSaturationSpoil2.setRampTimes( 500 );
  _sliceSelectionFatSaturationSpoil2.setDuration( 3000 );
  
  if( !_sliceSelectionFatSaturationSpoil2.prepAmplitude( 8.0 ) )
  {

    return _sliceSelectionFatSaturationSpoil2.getNLSStatus();

  }
  
  if( ! _sliceSelectionFatSaturationSpoil2.check() )
  {
    
    return _sliceSelectionFatSaturationSpoil2.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a slice selection crusher gradient
  //////////////////////////////////////////////////////////////////////////////
  
  double voxelResolution = ( mrProt.sliceSeries().front().readoutFOV() /
                             mrProt.kSpace().baseResolution() ) * 0.001; // m
  double lapsNumber = 4.0;
  double gamma = 42.57; // MHz/T
  double crusherMomentum = ( 1000 * lapsNumber ) / ( gamma * voxelResolution );

  if ( !_sliceSelectionFatSaturationCrusher.prepSymmetricTOTShortestTime(
                                                 crusherMomentum ) )
  {
    
    return _sliceSelectionFatSaturationCrusher.getNLSStatus();
      
  }
  
   if ( !_sliceSelectionFatSaturationCrusher.check( 
                         _sliceSelectionFatSaturationCrusher.getAmplitude(), 
                         _sliceSelectionFatSaturationCrusher.getAmplitude () ) ) 
  {
  
    return _sliceSelectionFatSaturationCrusher.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a readout spoiling gradients
  //////////////////////////////////////////////////////////////////////////////

  _readoutFatSaturationSpoil1.setRampTimes( 500 );
  _readoutFatSaturationSpoil1.setDuration( 3000 );
  
  if( !_readoutFatSaturationSpoil1.prepAmplitude( -8.0 ) )
  {

    return _readoutFatSaturationSpoil1.getNLSStatus();

  }
  
  if( ! _readoutFatSaturationSpoil1.check() )
  {
    
    return _readoutFatSaturationSpoil1.getNLSStatus();
    
  }
  
  _readoutFatSaturationSpoil2.setRampTimes( 500 );
  _readoutFatSaturationSpoil2.setDuration( 3000 );
  
  if( !_readoutFatSaturationSpoil2.prepAmplitude( 8.0 ) )
  {

    return _readoutFatSaturationSpoil2.getNLSStatus();

  }
  
  if( ! _readoutFatSaturationSpoil2.check() )
  {
    
    return _readoutFatSaturationSpoil2.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating phase spoiling gradients
  //////////////////////////////////////////////////////////////////////////////

  _phaseFatSaturationSpoil1.setRampTimes( 500 );
  _phaseFatSaturationSpoil1.setDuration( 3000 );
  
  if( !_phaseFatSaturationSpoil1.prepAmplitude( 8.0 ) )
  {

    return _phaseFatSaturationSpoil1.getNLSStatus();

  }
  
  if( ! _phaseFatSaturationSpoil1.check() )
  {
    
    return _phaseFatSaturationSpoil1.getNLSStatus();
    
  }
  
  _phaseFatSaturationSpoil2.setRampTimes( 500 );
  _phaseFatSaturationSpoil2.setDuration( 3000 );
  
  if( !_phaseFatSaturationSpoil2.prepAmplitude( -8.0 ) )
  {

    return _phaseFatSaturationSpoil2.getNLSStatus();

  }
  
  if( ! _phaseFatSaturationSpoil2.check() )
  {
    
    return _phaseFatSaturationSpoil2.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a phase crusher gradient
  //////////////////////////////////////////////////////////////////////////////
  
  if ( !_phaseFatSaturationCrusher.prepSymmetricTOTShortestTime( 
                                                             crusherMomentum ) )
  {
    
    return _phaseFatSaturationCrusher.getNLSStatus();
      
  }
  
  if ( !_phaseFatSaturationCrusher.check( 
                                  _phaseFatSaturationCrusher.getAmplitude(), 
                                  _phaseFatSaturationCrusher.getAmplitude () ) ) 
  {
  
    return _phaseFatSaturationCrusher.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside the module
  //////////////////////////////////////////////////////////////////////////////
  
  long spoilerDuration = _sliceSelectionFatSaturationSpoil1.getDuration() +
                         _sliceSelectionFatSaturationSpoil1.getRampDownTime();
  long crusherDuration = _sliceSelectionFatSaturationCrusher.getDuration() +
                         _sliceSelectionFatSaturationCrusher.getRampDownTime();
                          
  _sliceSelectionFatSaturationSpoil1.setStartTime( 0 );
  
  _rfFatSaturationWaveformEnveloppe.setStartTime( 
                                 spoilerDuration );
                                 
  _rfFatSaturationResetSynthetizer.setStartTime(
                              _rfFatSaturationWaveformEnveloppe.getStartTime() + 
                              _rfFatSaturationWaveformEnveloppe.getDuration() );
                                     
  _phaseFatSaturationSpoil2.setStartTime(
                              _rfFatSaturationWaveformEnveloppe.getStartTime() + 
                              _rfFatSaturationWaveformEnveloppe.getDuration() );
                                   
  _sliceSelectionFatSaturationCrusher.setStartTime( 
                                      _phaseFatSaturationSpoil2.getStartTime() + 
                                      spoilerDuration );

  _unitDuration = 2 * spoilerDuration +
                  _rfFatSaturationWaveformEnveloppe.getDuration() +
                  crusherDuration;
  
  _anchorTime = 0.;
  _durationBeforeAnchorTime = 0.;
  _durationAfterAnchorTime = _iterations * _unitDuration;


  return status;

}


NLS_STATUS gkg::FatSaturationModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partition,
                                               long lineIndex )
{

  _rfFatSaturationOffsets.prepSet( slices[ sliceIndex ], 
                                   _rfFatSaturationWaveformEnveloppe );
  _rfFatSaturationResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                            _rfFatSaturationWaveformEnveloppe );
  
  //~ double chemicalWaterFatShiftInPpm = 3.5;
  //~ double gamma = 42.576;                                   
  //~ _frequencyOffset = SysProperties::getNominalB0() * gamma *  
                           //~ chemicalWaterFatShiftInPpm;
  
  double symmetricPhase = - ( _frequencyOffset * 
                              ( 360.0 / 1e6 ) *  
                              _rfDuration * 
                              0.5 ); 

  _rfFatSaturationOffsets.setFrequency( _frequencyOffset );
  _rfFatSaturationOffsets.setPhase( symmetricPhase );
  
  _rfFatSaturationResetSynthetizer.setFrequency( 0. );
  _rfFatSaturationResetSynthetizer.setPhase( symmetricPhase );
  
  return SEQU__NORMAL;

}


NLS_STATUS gkg::FatSaturationModule::setKernelTiming( MrProt& mrProt )
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
  
  long iteration;
  
  for( iteration = 0; iteration < _iterations; iteration ++ )
  {
  
    fRTEI( this->getStartTime() +
           iteration * _unitDuration +
           _sliceSelectionFatSaturationSpoil1.getStartTime(), 
           0,
           0, 
           0, 
           &_phaseFatSaturationSpoil1, 
           &_readoutFatSaturationSpoil1, 
           &_sliceSelectionFatSaturationSpoil1, 
           0 );
    
    fRTEI( this->getStartTime() +
           iteration * _unitDuration +
           _rfFatSaturationWaveformEnveloppe.getStartTime(), 
           &_rfFatSaturationOffsets,
           &_rfFatSaturationWaveformEnveloppe, 
           0, 
           0, 
           0, 
           0, 
           0 );
    
    fRTEI( this->getStartTime() +
           iteration * _unitDuration +
           _rfFatSaturationResetSynthetizer.getStartTime(), 
           &_rfFatSaturationResetSynthetizer,
           0, 
           0, 
           0, 
           0, 
           0, 
           0 );
           
    
    fRTEI( this->getStartTime() +
           iteration * _unitDuration +
           _phaseFatSaturationSpoil2.getStartTime(), 
           0,
           0, 
           0, 
           &_phaseFatSaturationSpoil2, 
           &_readoutFatSaturationSpoil2, 
           &_sliceSelectionFatSaturationSpoil2, 
           0 );
         
    fRTEI( this->getStartTime() +
           iteration * _unitDuration +
           _sliceSelectionFatSaturationCrusher.getStartTime(), 
           0,
           0, 
           0, 
           &_phaseFatSaturationCrusher, 
           0, 
           &_sliceSelectionFatSaturationCrusher, 
           0 );
  
  }   

  return status;

}

const sRF_PULSE_SINC& 
gkg::FatSaturationModule::getRfFatSaturationWaveformEnveloppe() const
{

  return _rfFatSaturationWaveformEnveloppe;

}

const long& 
gkg::FatSaturationModule::getFatSaturationIterations() const
{

  return _iterations;

}

void gkg::FatSaturationModule::setFatSaturationFlipAngle( double flipAngle )
{
  
  _fatSaturationFlipAngle = flipAngle;
  
}

void gkg::FatSaturationModule::setFatSaturationFrequencyOffset( 
                                                        double frequencyOffset )
{
  
  _frequencyOffset = frequencyOffset;
  
}

void gkg::FatSaturationModule::setFatSaturationNumberOfIterations( 
                                                               long iterations )
{
  
  _iterations = iterations;
  
}


std::string gkg::FatSaturationModule::getStaticName()
{

  return "FatSaturationModule";

}


//
// adding the FatSaturationModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, FatSaturationModule );
