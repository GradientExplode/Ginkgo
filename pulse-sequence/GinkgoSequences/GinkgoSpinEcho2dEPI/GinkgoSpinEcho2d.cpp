#include "GinkgoSpinEcho2d.h"
#include "GinkgoSpinEcho2d_UI.h"

#include <Core/SiemensInclude.h> 
#include <ExcitationModules/SingleTxExcitation2dModule.h>
#include <RefocusingModules/SimpleRefocusingModule.h>
#include <CorrectionModules/EPIPhaseCorrectionModule.h>
#include <PreEncodingModules/EPI2dPreEncodingModule.h>
#include <ReadingModules/MultiLineEPIReading2dModule.h>
#include <SpoilingModules/Spoiling2dEPIModule.h>

#define OnErrorReturn( S ) if( !MrSucceeded( S ) ) return( S )

#ifdef SEQUENCE_CLASS_GinkgoSpinEcho2d

SEQIF_DEFINE( gkg::GinkgoSpinEcho2d )

#endif

gkg::GinkgoSpinEcho2d::GinkgoSpinEcho2d()
                    : _lineCountPerSecond( 0 ),
                      _rfSpoilIncrement( 0 ),
                      _rfSpoilPhase( 0 ),
                      _durationLine0( 0.0 ),
                      _durationLines( 0.0 ),
                      _totalDuration( 0.0 ),
                      _halfExcitationDuration( 0.0 ),
                      _presetEndTime( 0.0 ),
                      _userInterface( NULL ),
                      _grappaFlag( false )
{
  
  std::vector< double > singleTxExcitation2dDoubleParameters( 5U );
  singleTxExcitation2dDoubleParameters[ 0 ] = 90.00; // default flip angle 
  singleTxExcitation2dDoubleParameters[ 1 ] = 2560; // RF duration
  singleTxExcitation2dDoubleParameters[ 2 ] = 128; // RF sample count
  singleTxExcitation2dDoubleParameters[ 3 ] = 2.70; // RF bandwidth time product
  singleTxExcitation2dDoubleParameters[ 4 ] = 0; // RF intital phase 
  std::vector< std::string > singleTxExcitation2dStringParameters;
  _sequenceModules[ "01-SingleTxExcitation2dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SingleTxExcitation2dModule",
                                          singleTxExcitation2dDoubleParameters,
                                          singleTxExcitation2dStringParameters,
                                          _sequenceModules );
                                          
  // 02 for diffusion 
  
  std::vector< double > EPIPhaseCorrectionModuleDoubleParameters;
  std::vector< std::string > EPIPhaseCorrectionModuleStringParameters;
  _sequenceModules[ "03-EPIPhaseCorrectionModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                       "EPIPhaseCorrectionModule",
                                       EPIPhaseCorrectionModuleDoubleParameters,
                                       EPIPhaseCorrectionModuleStringParameters,
                                       _sequenceModules );                                        
  
  std::vector< double > refocusingDoubleParameters;
  std::vector< std::string > refocusingStringParameters;
  _sequenceModules[ "04-RefocusingModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SimpleRefocusingModule",
                                          refocusingDoubleParameters,
                                          refocusingStringParameters,
                                          _sequenceModules ); 
                                          
  std::vector< double > EPIPresetModuleDoubleParameters;
  std::vector< std::string > EPIPresetModuleStringParameters;
  _sequenceModules[ "05-EPI2dPreEncodingModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "EPI2dPreEncodingModule",
                                          EPIPresetModuleDoubleParameters,
                                          EPIPresetModuleStringParameters,
                                          _sequenceModules );
                                          
  std::vector< double > multiLineReading2dEPIDoubleParameters( 3U );
  multiLineReading2dEPIDoubleParameters[ 0 ] = 200000; // Default TE
  multiLineReading2dEPIDoubleParameters[ 1 ] = 1400; // Default BW per pixel
  multiLineReading2dEPIDoubleParameters[ 2 ] = 256; // Default base resolution
  std::vector< std::string > multiLineReading2dEPIStringParameters;
  _sequenceModules[ "06-MultiLineEPIReading2dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "MultiLineEPIReading2dModule",
                                          multiLineReading2dEPIDoubleParameters,
                                          multiLineReading2dEPIStringParameters,
                                          _sequenceModules );

  std::vector< double > spoiling2dDoubleParameters;
  std::vector< std::string > spoiling2dStringParameters( 2U );
  spoiling2dStringParameters[ 0 ] = "01-SingleTxExcitation2dModule"; 
                                                                   // Excitation
  spoiling2dStringParameters[ 1 ] = "05-EPI2dPreEncodingModule"; 
                                                               // Reading preset
  _sequenceModules[ "07-Spoiling2dEPIModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "Spoiling2dEPIModule",
                                          spoiling2dDoubleParameters,
                                          spoiling2dStringParameters,
                                          _sequenceModules );

  std::vector< double > endDoubleParameters( 1U );
  endDoubleParameters[ 0 ] = 2000000; // Default TR
  std::vector< std::string > endStringParameters;
  _sequenceModules[ "08-EndModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "EndModule",
                                          endDoubleParameters,
                                          endStringParameters,
                                          _sequenceModules );

}


gkg::GinkgoSpinEcho2d::~GinkgoSpinEcho2d()
{

  if ( _userInterface )
  {
      
    delete _userInterface;
    
  }

}


NLSStatus gkg::GinkgoSpinEcho2d::initialize( SeqLim& seqLim )
{
  
  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::initialize" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  seqLim.setSequenceOwner( "GINKGO TEAM" );

  //////////////////////////////////////////////////////////////////////////////
  // no intention to rephase momentum 1st order for phase encoding
  //////////////////////////////////////////////////////////////////////////////
  
  if( mIsUnittestActive() )
  { 
    SeqUT.DisableTestCase( lGpFirstMomentNotRephasedErr,
                           RTEB_ORIGIN_fSEQRunKernel,
                           "No intention to rephase momentum 1st order ");
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // enabling different gradient modes
  //////////////////////////////////////////////////////////////////////////////

  seqLim.setGradients(
        SEQ::GRAD_FAST,
        SEQ::GRAD_NORMAL,
        SEQ::GRAD_WHISPER
    );
  
  //////////////////////////////////////////////////////////////////////////////
  // initializing sequence modules
  //////////////////////////////////////////////////////////////////////////////

  std::map< std::string, gkg::RCPointer< gkg::SequenceModule > >::iterator
    sm = _sequenceModules.begin(),
    sme = _sequenceModules.end();
  while ( sm != sme )
  {
  
    sm->second->initialize( seqLim );
    ++ sm;

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // checking user interface initialization
  //////////////////////////////////////////////////////////////////////////////
  
  status = createUI( seqLim );
  if ( ( status & MRRESULT_SEV ) == MRRESULT_SEV ) 
  {
      
    MRTRACE( "Instantiation of GinkgoSpinEcho2dUI class failed!" );
    return status;
    
  }
    
#ifdef WIN32

  if ( !_userInterface ) 
  {
      
    MRTRACE( "GinkgoSpinEcho2dUI object null pointer (creation failed)!" );
    return SEQU_ERROR;
    
  }
  
#endif
  
  status = _userInterface->registerUI( seqLim );

#ifdef WIN32

  if( MrSeverity( status ) != MRRESULT_SUCCESS ) 
  {

    MRTRACE( "Registering GinkgoSpinEcho2dUI object failed! Errorcode: %i",
             status );
    return status;
    
  }

  seqLim.setDefaultEVAProt( _T( "%SiemensEvaDefProt%\\Inline\\Inline.evp" ) );

#endif
  
  return status;
  
    
}


NLSStatus gkg::GinkgoSpinEcho2d::prepare( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo )
{
  
    
  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // geometry management
  //////////////////////////////////////////////////////////////////////////////
  
  _lineCount = mrProt.kSpace().phaseEncodingLines();
  _sliceCount = mrProt.sliceSeries().getlSize(); 
  _centerLineIndex = mrProt.kSpace().echoLine();
  double partialFourierFactor = 1;
  if( mrProt.kSpace().phasePartialFourierFactor() == SEQ::PF_7_8 )
  {
    
    partialFourierFactor = ( 7.0 / 8.0 );
    
  }
  if( mrProt.kSpace().phasePartialFourierFactor() == SEQ::PF_6_8 )
  {
    
    partialFourierFactor = ( 6.0 / 8.0 );
    
  }
  if( mrProt.kSpace().phasePartialFourierFactor() == SEQ::PF_5_8 )
  {
    
    partialFourierFactor = ( 5.0 / 8.0 );
    
  }
  double grappaFactor = 1;
  long grappaOffset = 0;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    grappaFactor = 1.0 / 2.0;
    grappaOffset = 10;
    
  }
  _startLineIndex = _lineCount - 
                    ( int32_t ) ( partialFourierFactor * _lineCount ); 
  _startLine = _startLineIndex - _centerLineIndex;
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing line management
  //////////////////////////////////////////////////////////////////////////////

  long measureTimeUsec;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    measureTimeUsec = 2 * mrProt.tr()[0] * _sliceCount;
    
  }
  else
  {
    
    measureTimeUsec = mrProt.tr()[0] * _sliceCount;
    
  }
                                            
  seqExpo.setMeasureTimeUsec( measureTimeUsec );
  seqExpo.setTotalMeasureTimeUsec( measureTimeUsec );
  seqExpo.setMeasured3dPartitions( 1 );
  seqExpo.setMeasuredPELines( _lineCount );
  seqExpo.setSlicePerConcat( 0 );
 
  seqExpo.setRelevantReadoutsForMeasTime( _sliceCount );
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing all module(s)
  //////////////////////////////////////////////////////////////////////////////

  std::map< std::string, gkg::RCPointer< gkg::SequenceModule > >::iterator
    sm = _sequenceModules.begin(),
    sme = _sequenceModules.end();
  while ( sm != sme )
  {

    status = sm->second->prepare( mrProt, 
                                  seqLim,
                                  seqExpo );
                         
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }
    
    ++ sm;

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // setting gain and slices to acquire
  //////////////////////////////////////////////////////////////////////////////

  OnErrorReturn( fSSLSetRxGain( K_RX_GAIN_CODE_HIGH,
                                mrProt,
                                seqLim ) );
  OnErrorReturn( fSUPrepSlicePosArray( mrProt,
                                       seqLim,
                                       _slices ) );

  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    seqExpo.setRFInfo( 2 * _sliceCount *
                      ( static_cast< const gkg::SingleTxExcitation2dModule* >(
                      _sequenceModules.find( "01-SingleTxExcitation2dModule" )->
                          second.getPointer() )->
                                          getRfWaveformEnveloppe().getRFInfo() + 
                        static_cast< const gkg::SimpleRefocusingModule* >(
                        _sequenceModules.find( "04-RefocusingModule" )->
                          second.getPointer() )->
                             getRfRefocusingWaveformEnveloppe().getRFInfo() ) );
  
  }
  else
  {
    
    seqExpo.setRFInfo( _sliceCount *
                      ( static_cast< const gkg::SingleTxExcitation2dModule* >(
                      _sequenceModules.find( "01-SingleTxExcitation2dModule" )->
                          second.getPointer() )->
                                          getRfWaveformEnveloppe().getRFInfo() + 
                        static_cast< const gkg::SimpleRefocusingModule* >(
                        _sequenceModules.find( "04-RefocusingModule" )->
                          second.getPointer() )->
                             getRfRefocusingWaveformEnveloppe().getRFInfo() ) );
  
  }
          
  //////////////////////////////////////////////////////////////////////////////
  // checking minimum required TE and defining effective TE
  //////////////////////////////////////////////////////////////////////////////
  long adcShift =  
    static_cast< gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->getAdcShift();
  double readoutDuration = 
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  long readoutTotalDuration = 
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
                     
  long phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse( mrProt ).getDuration() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse( mrProt ).getRampDownTime();
  long readoutModuleTotalDuration = 
       _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->getTotalDuration();
  long centerLineReadoutCenterTime = 
                          ( - _startLine - 1 ) * readoutModuleTotalDuration +
                          phaseEncodeDuration + ( adcShift / 2 ) +
                          fSDSRoundUpGRT( readoutTotalDuration / 2 );

  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getRampDownTime();
    
    centerLineReadoutCenterTime = 
                     _sequenceModules[ "05-EPI2dPreEncodingModule" ]->
                                                          getTotalDuration() +
                     ( - _startLine / 2 ) * readoutModuleTotalDuration +
                     phaseEncodeDuration + ( adcShift / 2 ) + 
                     fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  }
                          
                                                                                                                   
  int32_t minimumRequiredTE =
       2 * std::max(
       _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                  getDurationAfterAnchorTime() +
       _sequenceModules[ "03-EPIPhaseCorrectionModule" ]->getTotalDuration() +
       _sequenceModules[ "04-RefocusingModule" ]->getDurationBeforeAnchorTime(),
       _sequenceModules[ "04-RefocusingModule" ]->getDurationAfterAnchorTime() +
       _sequenceModules[ "05-EPI2dPreEncodingModule" ]->getTotalDuration() +
       centerLineReadoutCenterTime );

  
  if ( minimumRequiredTE > mrProt.te()[ 0 ] ) 
  {

    return SBB_NEGATIV_TEFILL;

  }    


  //////////////////////////////////////////////////////////////////////////////
  // computing minimum required TR
  //////////////////////////////////////////////////////////////////////////////
  
  long lineCountAfterCenter =  _centerLineIndex;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    lineCountAfterCenter = _centerLineIndex / 2;
  
  }
  
  int32_t minimumRequiredTR =
                      _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                            getTotalDuration() +
                      mrProt.te()[0] +
                      lineCountAfterCenter *  
                          _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->
                                                            getTotalDuration() +
                      readoutDuration / 2 +
                      _sequenceModules[ "07-Spoiling2dEPIModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "08-EndModule" ]->getTotalDuration();

  
  if ( minimumRequiredTR > mrProt.tr()[0] ) 
  {

    return SBB_NEGATIV_TRFILL;

  }


  //////////////////////////////////////////////////////////////////////////////
  // preparing reconstruction
  //////////////////////////////////////////////////////////////////////////////

  fSUSetSequenceString( "2dseEPI",
                        mrProt,
                        seqExpo );
  
  seqExpo.setOnlineFFT( SEQ::ONLINE_FFT_PHASE );

  seqExpo.setICEProgramFilename( "%SiemensIceProgs%\\IceProgram2d" );
  seqExpo.setICEProgramParam( ICE_PROGRAM_PARA_SHOW_OFFLINE, 
                              SEQ::SO_SHOW_YES );

  return status;

}


NLSStatus gkg::GinkgoSpinEcho2d::check( MrProt& mrProt, 
                                      SeqLim& seqLim, 
                                      MrProtocolData::SeqExpo& seqExpo, 
                                      SEQCheckMode* /* checkMode */ )
{

  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::check" };

  NLS_STATUS status = SEQU__NORMAL;
  
  // TO DO 

  return status;

}


NLSStatus gkg::GinkgoSpinEcho2d::run( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo )
{


  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::run" };
  NLS_STATUS status = SEQU__NORMAL;

  if( IS_UNIT_TEST_ACTIVE( seqLim ) )
  {
    
    mSEQTest( mrProt,
              seqLim,
              seqExpo,
              RTEB_ORIGIN_fSEQRunStart,
              0,
              0,
              0,
              0,
              0 );
  
  }

  OnErrorReturn( fSBBMeasRepetDelaysRun( mrProt,
                                         seqLim,
                                         seqExpo,
                                         0 ) );
                                         
  gkg::EPIPhaseCorrectionModule*
   phaseCorrection2dModuleEPI = static_cast< gkg::EPIPhaseCorrectionModule* >(
            _sequenceModules[ "03-EPIPhaseCorrectionModule" ].getPointer() );
  gkg::EPI2dPreEncodingModule*
   epiReadingPreEncodingModule = static_cast< gkg::EPI2dPreEncodingModule* >(
            _sequenceModules[ "05-EPI2dPreEncodingModule" ].getPointer() );
  gkg::MultiLineEPIReading2dModule*
   multiLineReading2dModuleEPI = 
            static_cast< gkg::MultiLineEPIReading2dModule* >(
            _sequenceModules[ "06-MultiLineEPIReading2dModule" ].getPointer() );
  gkg::Spoiling2dEPIModule*
   spoiling2dModule = static_cast< gkg::Spoiling2dEPIModule* >(
            _sequenceModules[ "07-Spoiling2dEPIModule" ].getPointer() );

  if( IS_UNIT_TEST_ACTIVE( seqLim ) )
  {
  
    mSEQTest( mrProt,
              seqLim,
              seqExpo,
              RTEB_ClockInitTR,
              0,
              0,
              _slices[ 0 ].getSliceIndex(),
              0,
              0 );

  }
  
  int32_t lineIndex = 0;
  int32_t currentKernelCalls = 0;
  long measureTimeUsec;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    measureTimeUsec = 2 * mrProt.tr()[0] * _sliceCount;
    
  }
  else
  {
    
    measureTimeUsec = mrProt.tr()[0] * _sliceCount;
    
  }
  long kernelRequestsPerMeasurement;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    kernelRequestsPerMeasurement = 2 * _sliceCount;
    
  }
  else
  {
    
    kernelRequestsPerMeasurement = _sliceCount;
    
  }
  long kernelCallsPerRelevantSignal =  
             std::max<long>( 1, 
                             ( long )( kernelRequestsPerMeasurement * 1000000. /
                             measureTimeUsec ) );

  long sliceIndex;
  long diffusionIndex = 5; 
  _durationLine0 = 0.0;
  _durationLines = 0.0;
  _totalDuration = 0.0;
  
  
  long repetitionIndex = 0;
  
  //////////////////////////////////////////////////////////////////////////////
  // STARTING SEQUENCE TIMING
  //////////////////////////////////////////////////////////////////////////////
  
  for( repetitionIndex = 0; 
       repetitionIndex <= mrProt.repetitions();
       repetitionIndex++ )
  {
    
    multiLineReading2dModuleEPI->setRepetitionIndex( repetitionIndex );
      
    for( sliceIndex = 0; 
          sliceIndex < _sliceCount; 
          sliceIndex++ )
    { 
          
      if( IS_UNIT_TEST_ACTIVE( seqLim ) )
      {
            
        mSEQTest( mrProt,
                  seqLim,
                  seqExpo,
                  RTEB_ClockInitTR,
                  0,
                  0,
                  _slices[ sliceIndex ].getSliceIndex(),
                  0,
                  0 );
            
      }
          
      if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
      {
        
        _totalDuration = 0;
        long grappaStartLineIndex = ( _lineCount / 4 ) - 1; // TO DO with PF
        for ( lineIndex = grappaStartLineIndex;
              lineIndex < _lineCount - ( _lineCount / 4 );
              lineIndex++ )
            
        {
          
          phaseCorrection2dModuleEPI->setGrappaFlag( true );
          epiReadingPreEncodingModule->setGrappaFlag( true );
          multiLineReading2dModuleEPI->setGrappaFlag( true );
          spoiling2dModule->setGrappaFlag( true );
          _grappaFlag = true;
          
          fRTEBInit( _slices[ sliceIndex ].getROT_MATRIX() ); 
          
          ++ currentKernelCalls;
              
          multiLineReading2dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                currentKernelCalls,
                                                kernelCallsPerRelevantSignal );
                
          multiLineReading2dModuleEPI->setAdcReadoutMDH( 
                                      mrProt,
                                      lineIndex,
                                      _slices[ sliceIndex ].getSliceIndex(),
                                      diffusionIndex );
          
                
          OnErrorReturn( runKernel( mrProt,
                                    seqLim,
                                    seqExpo,
                                    KERNEL_IMAGE,
                                    _slices[ sliceIndex ].getSliceIndex(),
                                    0,
                                    lineIndex ) );
          
          status = fRTEBFinish(); 
            
                
        }
        
        phaseCorrection2dModuleEPI->setGrappaFlag( false );
        epiReadingPreEncodingModule->setGrappaFlag( false );
        multiLineReading2dModuleEPI->setGrappaFlag( false );
        spoiling2dModule->setGrappaFlag( false );
        _grappaFlag = false;
        
        _totalDuration = 0;
        for ( lineIndex = _startLineIndex;
              lineIndex < _lineCount;
              lineIndex++ )
            
        {
          
          if( lineIndex%2 == 0 )
          {
          
            fRTEBInit( _slices[ sliceIndex ].getROT_MATRIX() ); 
              
            ++ currentKernelCalls;
                
            multiLineReading2dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                 currentKernelCalls,
                                                 kernelCallsPerRelevantSignal );
                  
            multiLineReading2dModuleEPI->setAdcReadoutMDH( 
                                        mrProt,
                                        lineIndex,
                                        _slices[ sliceIndex ].getSliceIndex(),
                                        diffusionIndex );
            
                  
            OnErrorReturn( runKernel( mrProt,
                                      seqLim,
                                      seqExpo,
                                      KERNEL_IMAGE,
                                      _slices[ sliceIndex ].getSliceIndex(),
                                      0,
                                      lineIndex ) );
            
            status = fRTEBFinish(); 
            
          }
                
        }
        
      }
      else
      {
      
        phaseCorrection2dModuleEPI->setGrappaFlag( false );
        epiReadingPreEncodingModule->setGrappaFlag( false );
        multiLineReading2dModuleEPI->setGrappaFlag( false );
        spoiling2dModule->setGrappaFlag( false );
        _grappaFlag = false;
        
        _totalDuration = 0;
        for ( lineIndex = _startLineIndex;
              lineIndex < _lineCount;
              lineIndex++ )
            
        {
          
          fRTEBInit( _slices[ sliceIndex ].getROT_MATRIX() ); 
            
          ++ currentKernelCalls;
              
          multiLineReading2dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                currentKernelCalls,
                                                kernelCallsPerRelevantSignal );
                
          multiLineReading2dModuleEPI->setAdcReadoutMDH( 
                                      mrProt,
                                      lineIndex,
                                      _slices[ sliceIndex ].getSliceIndex(),
                                      diffusionIndex );
          
                
          OnErrorReturn( runKernel( mrProt,
                                    seqLim,
                                    seqExpo,
                                    KERNEL_IMAGE,
                                    _slices[ sliceIndex ].getSliceIndex(),
                                    0,
                                    lineIndex ) );
          
          status = fRTEBFinish(); 
            
                
        }
      
      }
      
    }
  
  }

  if( IS_UNIT_TEST_ACTIVE( seqLim ) )
  {
    
    mSEQTest( mrProt,
              seqLim,
              seqExpo,
              RTEB_ORIGIN_fSEQRunFinish,
              0,
              0,
              0,
              0,
              0 );
  
  }
    
  return status;


}


NLS_STATUS gkg::GinkgoSpinEcho2d::runKernel( MrProt& mrProt, 
                                             SeqLim& seqLim,  
                                             MrProtocolData::SeqExpo& seqExpo,
                                             long kernelMode,  
                                             long sliceIndex, 
                                             long partitionIndex, 
                                             long lineIndex )
{

  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::runKernel" };

  NLS_STATUS status  = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // modifying kernels of all module(s)
  //////////////////////////////////////////////////////////////////////////////  
  
  std::map< std::string, gkg::RCPointer< gkg::SequenceModule > >::iterator
    sm = _sequenceModules.begin(),
    sme = _sequenceModules.end();
  while ( sm != sme )
  {

    status = sm->second->modifyKernel( mrProt,
                                       seqLim,
                                       seqExpo,
                                       _slices,
                                       sliceIndex,
                                       partitionIndex,
                                       lineIndex );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }
    
    ++ sm;

  }

  //////////////////////////////////////////////////////////////////////////////
  // setting anchor time for all module(s)
  //////////////////////////////////////////////////////////////////////////////
  
  int32_t adjust = 1;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
  
      adjust = 2;
  
  }
  else
  {
  
    adjust = 1;
  
  }
  
  long adcShift =  
    static_cast< gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->getAdcShift();
  double readoutDuration = 
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  long readoutTotalDuration = 
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
                     _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  
  long phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse( mrProt ).getDuration() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse( mrProt ).getRampDownTime();
  long readoutModuleTotalDuration = 
       _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->getTotalDuration();

  double centerLineReadoutCenterTime = 
                          ( - _startLine - 1 ) * readoutModuleTotalDuration +
                          phaseEncodeDuration + ( adcShift / 2 ) +
                          fSDSRoundUpGRT( readoutTotalDuration / 2 );
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading2dModule* >(
       _sequenceModules.find( "06-MultiLineEPIReading2dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getRampDownTime();
    
    centerLineReadoutCenterTime = 
                     _sequenceModules[ "05-EPI2dPreEncodingModule" ]->
                                                          getTotalDuration() +
                     ( - _startLine / 2 ) * readoutModuleTotalDuration +
                     phaseEncodeDuration + ( adcShift / 2 ) + 
                     fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  }
  
  if( lineIndex == _startLineIndex ||
      ( _grappaFlag == true && lineIndex == _lineCount / 4 - 1 ) )
  {

    _sequenceModules[ "01-SingleTxExcitation2dModule" ]->setAnchorTime( 
          _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                getDurationBeforeAnchorTime() );
    
    _halfExcitationDuration = 
                           _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                getDurationBeforeAnchorTime();

    _sequenceModules[ "03-EPIPhaseCorrectionModule" ]->setAnchorTime( 
            _sequenceModules[ "01-SingleTxExcitation2dModule" ]->getEndTime() );                                              
    
    _sequenceModules[ "04-RefocusingModule" ]->setAnchorTime( 
          mrProt.te()[ 0 ] / 2 +
          _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                getDurationBeforeAnchorTime() );
                                                
    _sequenceModules[ "05-EPI2dPreEncodingModule" ]->setAnchorTime( 
                 mrProt.te()[ 0 ] +
                 _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                 getDurationBeforeAnchorTime() -
                 centerLineReadoutCenterTime -
                 _sequenceModules[ "05-EPI2dPreEncodingModule" ]->
                                                           getTotalDuration() );                                              

    _presetEndTime = _sequenceModules[ "05-EPI2dPreEncodingModule" ]->
                                                                   getEndTime();

    _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->setAnchorTime( 
              _sequenceModules[ "05-EPI2dPreEncodingModule" ]->getEndTime() );
  
    _totalDuration += _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->
                                                                  getEndTime() +
                      _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                  getDurationBeforeAnchorTime();                                         
   
  }
  else if( lineIndex == _lineCount - adjust ||
           ( _grappaFlag == true && 
             lineIndex == _lineCount - _lineCount / 4 - 1 ) )
  {
    
    _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->setAnchorTime( 0 );
  
    _sequenceModules[ "07-Spoiling2dEPIModule" ]->setAnchorTime( 
           _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->getEndTime() );
    
    _sequenceModules[ "08-EndModule" ]->setAnchorTime( 
           mrProt.tr()[ 0 ] +
           _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                 getDurationBeforeAnchorTime() - 
           _totalDuration );

  }
  else
  {
  
    _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->setAnchorTime( 0 );

    _totalDuration += _sequenceModules[ "06-MultiLineEPIReading2dModule" ]->
                                                             getTotalDuration();
    
  }
  
  
  //////////////////////////////////////////////////////////////////////////////
  // setting kernel timing of all module(s)
  //////////////////////////////////////////////////////////////////////////////

  sm = _sequenceModules.begin();
  while ( sm != sme )
  {
    
      if( lineIndex == _startLineIndex ||
          ( _grappaFlag == true && lineIndex == _lineCount / 4 - 1 ) )
      {
        
        if( sm != _sequenceModules.find( "08-EndModule" ) &&
            sm != _sequenceModules.find( "07-Spoiling2dEPIModule" ) )
        {
          
          status = sm->second->setKernelTiming( mrProt );
        
        }
        if ( status != SEQU__NORMAL )
        {
        
          return status;
        
        }
      }
      else if( lineIndex == _lineCount - adjust ||
               ( _grappaFlag == true && 
                 lineIndex == _lineCount - _lineCount / 4 - 1 ) )
      {
        
        if( sm == _sequenceModules.find( "06-MultiLineEPIReading2dModule" ) ||
            sm == _sequenceModules.find( "07-Spoiling2dEPIModule" ) || 
            sm == _sequenceModules.find( "08-EndModule" ) )
        {
      
          status = sm->second->setKernelTiming( mrProt );
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        else
        {
          
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        
      }
      else if( lineIndex == _startLineIndex + 1 ||
               ( _grappaFlag == true && lineIndex == _lineCount / 4 ) )
      {
        
        if( sm == _sequenceModules.find( "06-MultiLineEPIReading2dModule" ) )
        {
      
          status = sm->second->setKernelTiming( mrProt );
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        else
        {
          
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        
      }
      else
      {

        if( sm == _sequenceModules.find( "06-MultiLineEPIReading2dModule" ) )
        {
      
          status = sm->second->setKernelTiming( mrProt );
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        else
        {
          
          if ( status != SEQU__NORMAL )
          {
          
            return status;
          
          }
          
        }
        
      }

    ++ sm;

  }


  //////////////////////////////////////////////////////////////////////////////
  // running checks and tests
  //////////////////////////////////////////////////////////////////////////////

  if( IS_UNIT_TEST_ACTIVE( seqLim ) )
  {
    
    if ( kernelMode == KERNEL_CHECK )
    {
    
      mSEQTest( mrProt,
                seqLim,
                seqExpo,
                RTEB_ORIGIN_fSEQCheck,
                10,
                lineIndex,
                _slices[ sliceIndex ].getSliceIndex(),
                0, 
                0 );
                
    }
    
    else
    {
    
      mSEQTest( mrProt,
                seqLim,
                seqExpo,
                RTEB_ORIGIN_fSEQRunKernel,
                10,
                lineIndex,
                _slices[ sliceIndex ].getSliceIndex(),
                0,
                0 );
                
    }
    
    mSEQTest( mrProt,
              seqLim,
              seqExpo,
              RTEB_ClockCheck, 
              10,
              lineIndex,
              _slices[ sliceIndex ].getSliceIndex(),
              0,
              0 );
  
  }
  
  return status;


}


NLS_STATUS gkg::GinkgoSpinEcho2d::createUI( SeqLim& )
{


#ifdef WIN32

  static const char *ptModule = { "gkg::GinkgoSpinEcho2d::createUI" };

  if ( _userInterface )  
  {

    delete _userInterface;

  }
 
  try 
  {

    _userInterface = new gkg::GinkgoSpinEcho2dUI();

  }
  catch (...) 
  {

    delete _userInterface;

    TRACE_PUT1( TC_ALWAYS, 
                TF_SEQ, 
                "%s: Cannot instantiate UI class !", 
                ptModule );

    return SEQU_ERROR;
  
  }

#endif

  return SEQU_NORMAL;


}


const gkg::GinkgoSpinEcho2dUI* gkg::GinkgoSpinEcho2d::getUI( void ) const
{
  
  return _userInterface;

}
