#include "GinkgoSpinEcho3d.h"
#include "GinkgoSpinEcho3d_UI.h"

#include <Core/SiemensInclude.h> 
#include <SpoilingModules/Spoiling3dEPIModule.h>
#include <PreparationModules/FatSaturationModule.h>
#include <ExcitationModules/SingleTxExcitation3dModule.h>
#include <ReadingModules/MultiLineEPIReading3dModule.h>
#include <PreEncodingModules/EPI3dPreEncodingModule.h>
#include <RefocusingModules/SimpleRefocusingModule.h>
#include <CorrectionModules/EPIPhaseCorrectionModule.h>

#define OnErrorReturn( S ) if( !MrSucceeded( S ) ) return( S )

#ifdef SEQUENCE_CLASS_GinkgoSpinEcho3d

SEQIF_DEFINE( gkg::GinkgoSpinEcho3d )

#endif


gkg::GinkgoSpinEcho3d::GinkgoSpinEcho3d()
                    : _userInterface( NULL ),
                      _lineCountPerSecond( 0 ),
                      _rfSpoilPhase( 0.0 ),
                      _totalDuration( 0 ),
                      _tool( *this ),
                      _grappaFlag( false )
{

  std::vector< double > fatSaturationDoubleParameters;
  std::vector< std::string > fatSaturationStringParameters;
  _sequenceModules[ "00-FatSaturationModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create( 
                                                  "FatSaturationModule",
                                                  fatSaturationDoubleParameters,
                                                  fatSaturationStringParameters,
                                                  _sequenceModules );
  
  std::vector< double > singleTxExcitation3dDoubleParameters( 5U );
  singleTxExcitation3dDoubleParameters[ 0 ] = 90.00; // default flip angle 
  singleTxExcitation3dDoubleParameters[ 1 ] = 2560; // RF duration
  singleTxExcitation3dDoubleParameters[ 2 ] = 128; // RF sample count
  singleTxExcitation3dDoubleParameters[ 3 ] = 2.70; // RF bandwidth time product
  singleTxExcitation3dDoubleParameters[ 4 ] = 0; // RF intital phase 
  std::vector< std::string > singleTxExcitation3dStringParameters;
  _sequenceModules[ "01-SingleTxExcitation3dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SingleTxExcitation3dModule",
                                          singleTxExcitation3dDoubleParameters,
                                          singleTxExcitation3dStringParameters,
                                          _sequenceModules );
                                          
  std::vector< double > EPIPhaseCorrectionModuleDoubleParameters;
  std::vector< std::string > EPIPhaseCorrectionModuleStringParameters;
  _sequenceModules[ "02-EPIPhaseCorrectionModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                       "EPIPhaseCorrectionModule",
                                       EPIPhaseCorrectionModuleDoubleParameters,
                                       EPIPhaseCorrectionModuleStringParameters,
                                       _sequenceModules );   
  
  std::vector< double > refocusingDoubleParameters;
  std::vector< std::string > refocusingStringParameters;
  _sequenceModules[ "03-RefocusingModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SimpleRefocusingModule",
                                          refocusingDoubleParameters,
                                          refocusingStringParameters,
                                          _sequenceModules ); 

  std::vector< double > EPIPresetModuleDoubleParameters;
  std::vector< std::string > EPIPresetModuleStringParameters;
  _sequenceModules[ "04-EPI3dPreEncodingModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "EPI3dPreEncodingModule",
                                          EPIPresetModuleDoubleParameters,
                                          EPIPresetModuleStringParameters,
                                          _sequenceModules );
  
  std::vector< double > multiLineReading3dEPIDoubleParameters( 3U );
  multiLineReading3dEPIDoubleParameters[ 0 ] = 308000; // Default TE
  multiLineReading3dEPIDoubleParameters[ 1 ] = 1450; // Default BW per pixel
  multiLineReading3dEPIDoubleParameters[ 2 ] = 64; // Default base resolution
  std::vector< std::string > multiLineReading3dEPIStringParameters;
  _sequenceModules[ "05-MultiLineEPIReading3dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "MultiLineEPIReading3dModule",
                                          multiLineReading3dEPIDoubleParameters,
                                          multiLineReading3dEPIStringParameters,
                                          _sequenceModules );

  std::vector< double > spoiling3dDoubleParameters;
  std::vector< std::string > spoiling3dStringParameters( 2U );
  spoiling3dStringParameters[ 0 ] = "01-SingleTxExcitation3dModule"; 
                                                                   // Excitation
  spoiling3dStringParameters[ 1 ] = "04-EPI3dPreEncodingModule"; 
                                                               // Reading preset
  _sequenceModules[ "06-Spoiling3dEPIModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "Spoiling3dEPIModule",
                                          spoiling3dDoubleParameters,
                                          spoiling3dStringParameters,
                                          _sequenceModules );

  std::vector< double > endDoubleParameters( 1U );
  endDoubleParameters[ 0 ] = 3000000; // Default TR
  std::vector< std::string > endStringParameters;
  _sequenceModules[ "07-EndModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "EndModule",
                                          endDoubleParameters,
                                          endStringParameters,
                                          _sequenceModules );

}


gkg::GinkgoSpinEcho3d::~GinkgoSpinEcho3d()
{

  if ( _userInterface )
  {
      
    delete _userInterface;
    
  }

}


NLSStatus gkg::GinkgoSpinEcho3d::initialize( SeqLim& seqLim )
{

  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::initialize" };
  
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
  // no intention to rephase momentum 1st order for phase encoding (spin echo)
  //////////////////////////////////////////////////////////////////////////////
  
  if( mIsUnittestActive() )
  { 
    SeqUT.DisableTestCase( lGpFirstMomentNotRephasedErr,
                           RTEB_ORIGIN_fSEQRunKernel,
                           "No intention to rephase momentum 1st order ");
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // setting shimming procedure
  //////////////////////////////////////////////////////////////////////////////
  
  seqLim.setAdjShim( SEQ::ADJSHIM_TUNEUP, 
                     SEQ::ADJSHIM_STANDARD ); 
  
  //////////////////////////////////////////////////////////////////////////////
  // checking user interface initialization
  //////////////////////////////////////////////////////////////////////////////
  
  status = createUI( seqLim );
  if ( ( status & MRRESULT_SEV ) == MRRESULT_SEV ) 
  {
      
    MRTRACE( "Instantiation of GinkgoSpinEcho3dUI class failed!" );
    return status;
    
  }
    
#ifdef WIN32

  if ( !_userInterface ) 
  {
      
    MRTRACE( "GinkgoSpinEcho3dUI object null pointer (creation failed)!" );
    return SEQU_ERROR;
    
  }

#endif

  //~ status = _userInterface->registerUI( seqLim );
  status = _userInterface->registerUI( seqLim, _tool );

#ifdef WIN32

  if( MrSeverity( status ) != MRRESULT_SUCCESS ) 
  {

    MRTRACE( "Registering GinkgoSpinEcho3dUI object failed! Errorcode: %i",
             status );
    return status;
    
  }

  //~ seqLim.setDefaultEVAProt( _T( "%SiemensEvaDefProt%\\Inline\\Inline.evp" ) );

#endif
  
  return status;
  
    
}


NLSStatus gkg::GinkgoSpinEcho3d::prepare( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo )
{
  
    
  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  if( !_tool.prepare( mrProt, seqLim ) ) 
  {
    
    return SEQU_ERROR;
  
  }
  
  gkg::FatSaturationModule*
    fatSaturation2dModule = static_cast< gkg::FatSaturationModule* >(
              _sequenceModules[ "00-FatSaturationModule" ].getPointer() );
  
  fatSaturation2dModule->setFatSaturationFlipAngle( 
                                        _tool.getDoubleValue( mrProt, 0 ) );
  fatSaturation2dModule->setFatSaturationFrequencyOffset( 
                                        _tool.getDoubleValue( mrProt, 1 ) );
  fatSaturation2dModule->setFatSaturationNumberOfIterations( 
                                          _tool.getLongValue( mrProt, 2 ) );
  
  //////////////////////////////////////////////////////////////////////////////
  // geometry management
  //////////////////////////////////////////////////////////////////////////////
  
  _lineCount = mrProt.kSpace().phaseEncodingLines();
  _sliceCount = mrProt.sliceSeries().getlSize(); 
  _partitionCount = mrProt.kSpace().partitions();
  _centerLineIndex = mrProt.kSpace().echoLine();
  _centerPartitionIndex = mrProt.kSpace().echoPartition();
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
  if( mrProt.PAT().getucPATMode() == SEQ::PAT_MODE_GRAPPA )
  {
    
    grappaFactor = 1.0 / 2.0;
    grappaOffset = 10;
    
  }
  _startLineIndex = _lineCount - 
                    ( int32_t ) ( partialFourierFactor * _lineCount ); 
  _startLine = _startLineIndex - _centerLineIndex;
  _startPartitionIndex = _partitionCount - 
                         ( int32_t ) ( partialFourierFactor * _partitionCount ); 
  _startPartition = _startPartitionIndex - _centerPartitionIndex;  
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing line management
  //////////////////////////////////////////////////////////////////////////////

  double measureTimeUsec = 0.0;
  
  measureTimeUsec = ( double )mrProt.tr()[0] * _partitionCount ;

  long kernelRequestsPerMeasurement = _partitionCount;  
  long kernelCallsPerRelevantSignal =  
             std::max<long>( 1, 
                             ( long )( kernelRequestsPerMeasurement * 1000000. /
                             measureTimeUsec ) );
  seqExpo.setRelevantReadoutsForMeasTime( kernelRequestsPerMeasurement / 
                                          kernelCallsPerRelevantSignal );
                                            
  seqExpo.setMeasureTimeUsec( measureTimeUsec );
  seqExpo.setTotalMeasureTimeUsec( measureTimeUsec );
  seqExpo.setMeasured3dPartitions( _partitionCount );
  seqExpo.setMeasuredPELines( _lineCount );
  seqExpo.setSlicePerConcat( 1 );

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
  // (spin echo) 
                                      
  seqExpo.setRFInfo( _partitionCount *
                     ( static_cast< const gkg::SingleTxExcitation3dModule* >(
                      _sequenceModules.find( "01-SingleTxExcitation3dModule" )->
                        second.getPointer() )->
                                          getRfWaveformEnveloppe().getRFInfo() +
                      static_cast< const gkg::SimpleRefocusingModule* >(
                      _sequenceModules.find( "03-RefocusingModule" )->
                        second.getPointer() )->
                         getRfRefocusingWaveformEnveloppe().getRFInfo() /*) )*/+
                      static_cast< const gkg::FatSaturationModule* >(
                     _sequenceModules.find( "00-FatSaturationModule" )->
                       second.getPointer() )->
                         getRfFatSaturationWaveformEnveloppe().getRFInfo() *
                     static_cast< const gkg::FatSaturationModule* >(
                     _sequenceModules.find( "00-FatSaturationModule" )->
                       second.getPointer() )->
                          getFatSaturationIterations() ) );

          
  //////////////////////////////////////////////////////////////////////////////
  // checking minimum required TE and defining effective TE
  //////////////////////////////////////////////////////////////////////////////

  long adcShift =  
    static_cast< gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->getAdcShift();
  double readoutDuration = 
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  long readoutTotalDuration = 
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  
  long phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse().getRampDownTime();
  long readoutModuleTotalDuration = 
       _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->getTotalDuration();
                     
  long centerLineReadoutCenterTime = 
                          ( - _startLine - 1 ) * readoutModuleTotalDuration +
                          phaseEncodeDuration + ( adcShift / 2 ) +
                          fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getRampDownTime();
    centerLineReadoutCenterTime = 
                     _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                                                          getTotalDuration() +
                     ( - _startLine / 2 ) * readoutModuleTotalDuration +
                     phaseEncodeDuration + ( adcShift / 2 ) + 
                     fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  }
  
  //~ long readoutTotalDuration = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getRampDownTime() +
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getDuration();
                     
  //~ long readoutRampTime = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getRampDownTime();
  
  //~ long adcShift =  
    //~ static_cast< gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->getAdcShift();
  
  //~ long phaseEncodeDuration =  
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
       //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       //~ second.getPointer() )->getPhaseEncodingGradientPulse().getDuration() +
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
       //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       //~ second.getPointer() )->getPhaseEncodingGradientPulse().getRampDownTime();
  
  //~ long readoutModuleTotalDuration = 
       //~ _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->getTotalDuration();                   
  
 //~ long centerLineReadoutCenterTime = 
                     //~ ( - _startLine - 1 ) * readoutModuleTotalDuration +
                     //~ phaseEncodeDuration + ( adcShift / 2 ) +
                     //~ fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  //~ if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  //~ {
    
    //~ centerLineReadoutCenterTime = 
                     //~ _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                                                          //~ getTotalDuration() +
                     //~ ( - _startLine / 2 ) * readoutModuleTotalDuration +
                     //~ phaseEncodeDuration + ( adcShift / 2 ) + 
                     //~ fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  //~ }
  
  long minimumRequiredTE =  
       2 * std::max(
       _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                  getDurationAfterAnchorTime() +
       _sequenceModules[ "02-EPIPhaseCorrectionModule" ]->getTotalDuration() +
       _sequenceModules[ "03-RefocusingModule" ]->getDurationBeforeAnchorTime(),
       _sequenceModules[ "03-RefocusingModule" ]->getDurationAfterAnchorTime() +
       _sequenceModules[ "04-EPI3dPreEncodingModule" ]->getTotalDuration() +
       centerLineReadoutCenterTime );
  
  if ( minimumRequiredTE > mrProt.te()[0] ) 
  {

    return SBB_NEGATIV_TEFILL;

  }    


  //////////////////////////////////////////////////////////////////////////////
  // computing minimum required TR
  //////////////////////////////////////////////////////////////////////////////

  long EPIReadingTotalDuration =  
                     _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                     getTotalDuration() +
                    ( _lineCount * 
                     _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->
                       getTotalDuration() ) +
                     _sequenceModules[ "06-Spoiling3dEPIModule" ]->
                     getTotalDuration();
  
  long minimumRequiredTR =
                      _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                            getTotalDuration() +
                      mrProt.te()[0] +
                      EPIReadingTotalDuration -
                      centerLineReadoutCenterTime +
                      _sequenceModules[ "06-Spoiling3dEPIModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "07-EndModule" ]->getTotalDuration();
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    minimumRequiredTR += ( _partitionCount - _centerPartitionIndex - 1 ) * 
                         _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                                                            getTotalDuration();
    
  }

  if ( minimumRequiredTR > mrProt.tr()[0] ) 
  {

    return SBB_NEGATIV_TRFILL;

  }
  

  //////////////////////////////////////////////////////////////////////////////
  // preparing reconstruction
  //////////////////////////////////////////////////////////////////////////////

  fSUSetSequenceString( "3dEPI",
                        mrProt,
                        seqExpo );
                              
  seqExpo.setOnlineFFT( SEQ::ONLINE_FFT_NONE );
      
  seqExpo.setICEProgramFilename( "%SiemensIceProgs%\\IceProgram3D" );
  seqExpo.setICEProgramParam( ICE_PROGRAM_PARA_SHOW_OFFLINE, 
                              SEQ::SO_SHOW_YES );

  return status;

}


NLSStatus gkg::GinkgoSpinEcho3d::check( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo, 
                                        SEQCheckMode* /* checkMode */ )
{

  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::check" };

  NLS_STATUS status = SEQU__NORMAL;

  // TO DO

  return status;

}


NLSStatus gkg::GinkgoSpinEcho3d::run( MrProt& mrProt, 
                                      SeqLim& seqLim, 
                                      MrProtocolData::SeqExpo& seqExpo )
{


  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::run" };
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
                                         
  gkg::EPIPhaseCorrectionModule*
   phaseCorrection3dModuleEPI = static_cast< gkg::EPIPhaseCorrectionModule* >(
            _sequenceModules[ "02-EPIPhaseCorrectionModule" ].getPointer() );
  gkg::EPI3dPreEncodingModule*
   epi3dReadingPresetModule = static_cast< gkg::EPI3dPreEncodingModule* >(
            _sequenceModules[ "04-EPI3dPreEncodingModule" ].getPointer() );
  gkg::MultiLineEPIReading3dModule*
   multiLineReading3dModuleEPI = 
             static_cast< gkg::MultiLineEPIReading3dModule* >(
            _sequenceModules[ "05-MultiLineEPIReading3dModule" ].getPointer() );
  gkg::Spoiling3dEPIModule*
   spoiling3dModule = static_cast< gkg::Spoiling3dEPIModule* >(
            _sequenceModules[ "06-Spoiling3dEPIModule" ].getPointer() );
  
  long lineIndex = 0;
  long currentKernelCalls = 0;
  long partitionIndex = 0; 
  bool grappaEvenLines = true;

  long measureTimeUsec = ( long )_lineCount * mrProt.tr()[0] * _partitionCount;
  long kernelRequestsPerMeasurement = _partitionCount;   
  long kernelCallsPerRelevantSignal =  
             std::max<long>( 1, 
                             ( long )( kernelRequestsPerMeasurement * 1000000. /
                             measureTimeUsec ) );
  
   long repetitionIndex = 0;
   
  //////////////////////////////////////////////////////////////////////////////
  // STARTING SEQUENCE TIMING
  //////////////////////////////////////////////////////////////////////////////
  
  for( repetitionIndex = 0; 
       repetitionIndex <= mrProt.repetitions();
       repetitionIndex++ )
  {
    
    multiLineReading3dModuleEPI->setRepetitionIndex( repetitionIndex );
    //~ for( diffusionIndex = 0; 
         //~ diffusionIndex < diffusionCount; 
         //~ diffusionIndex++ )
    //~ { 
      
      //~ trapezoidDiffusionGradientModule -> setDiffusionIndex( diffusionIndex );
      
      for( partitionIndex = 0; 
           partitionIndex < _partitionCount; 
           partitionIndex++ )
      { 
           
        if( IS_UNIT_TEST_ACTIVE( seqLim ) )
        {
              
          mSEQTest( mrProt,
                    seqLim,
                    seqExpo,
                    RTEB_ClockInitTR,
                    0,
                    0,
                    _slices[ 0 ].getSliceIndex(),
                    partitionIndex,
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
            
            phaseCorrection3dModuleEPI->setGrappaFlag( true );
            epi3dReadingPresetModule->setGrappaFlag( true );
            multiLineReading3dModuleEPI->setGrappaFlag( true );
            spoiling3dModule->setGrappaFlag( true );
            _grappaFlag = true;
            
            fRTEBInit( _slices[ 0 ].getROT_MATRIX() ); 
            
            ++ currentKernelCalls;
                
            multiLineReading3dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                  currentKernelCalls,
                                                  kernelCallsPerRelevantSignal );
                  
            multiLineReading3dModuleEPI->setAdcReadoutMDH( 
                                        mrProt,
                                        lineIndex,
                                        _slices[ 0 ].getSliceIndex(),
                                        partitionIndex );
            
                  
            OnErrorReturn( runKernel( mrProt,
                                      seqLim,
                                      seqExpo,
                                      KERNEL_IMAGE,
                                      _slices[ 0 ].getSliceIndex(),
                                      partitionIndex,
                                      lineIndex ) );
            
            status = fRTEBFinish(); 
              
                  
          }
          
          phaseCorrection3dModuleEPI->setGrappaFlag( false );
          epi3dReadingPresetModule->setGrappaFlag( false );
          multiLineReading3dModuleEPI->setGrappaFlag( false );
          spoiling3dModule->setGrappaFlag( false );
          _grappaFlag = false;
          
          _totalDuration = 0;
          for ( lineIndex = _startLineIndex;
                lineIndex < _lineCount;
                lineIndex++ )
              
          {
            
            if( lineIndex%2 == 0 )
            {
            
              fRTEBInit( _slices[ 0 ].getROT_MATRIX() ); 
                
              ++ currentKernelCalls;
                  
              multiLineReading3dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                    currentKernelCalls,
                                                    kernelCallsPerRelevantSignal );
                    
              multiLineReading3dModuleEPI->setAdcReadoutMDH( 
                                          mrProt,
                                          lineIndex,
                                          _slices[ 0 ].getSliceIndex(),
                                          partitionIndex );
              
                    
              OnErrorReturn( runKernel( mrProt,
                                        seqLim,
                                        seqExpo,
                                        KERNEL_IMAGE,
                                        _slices[ 0 ].getSliceIndex(),
                                        partitionIndex,
                                        lineIndex ) );
              
              status = fRTEBFinish(); 
              
            }
                  
          }
          
        }
        else
        {
        
          phaseCorrection3dModuleEPI->setGrappaFlag( false );
          epi3dReadingPresetModule->setGrappaFlag( false );
          multiLineReading3dModuleEPI->setGrappaFlag( false );
          spoiling3dModule->setGrappaFlag( false );
          _grappaFlag = false;
          
          _totalDuration = 0;
          for ( lineIndex = _startLineIndex;
                lineIndex < _lineCount;
                lineIndex++ )
              
          {
            
            fRTEBInit( _slices[ 0 ].getROT_MATRIX() ); 
              
            ++ currentKernelCalls;
                
            multiLineReading3dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                  currentKernelCalls,
                                                  kernelCallsPerRelevantSignal );
                  
            multiLineReading3dModuleEPI->setAdcReadoutMDH( 
                                        mrProt,
                                        lineIndex,
                                        _slices[ 0 ].getSliceIndex(),
                                        partitionIndex );
            
                  
            OnErrorReturn( runKernel( mrProt,
                                      seqLim,
                                      seqExpo,
                                      KERNEL_IMAGE,
                                      _slices[ 0 ].getSliceIndex(),
                                      partitionIndex,
                                      lineIndex ) );
            
            status = fRTEBFinish(); 
              
                  
          }
        
        }
        
      }
      
    //~ }
  
  }
  
  //~ for( partitionIndex = _startPartitionIndex; 
       //~ partitionIndex < _partitionCount; 
       //~ partitionIndex++ ) 
  //~ {
            
      //~ ++ currentKernelCalls;
      //~ _totalDuration = 0;
      //~ if( IS_UNIT_TEST_ACTIVE( seqLim ) )
      //~ {
            
        //~ mSEQTest( mrProt,
                  //~ seqLim,
                  //~ seqExpo,
                  //~ RTEB_ClockInitTR,
                  //~ 0,
                  //~ 0,
                  //~ 0,
                  //~ 0,
                  //~ 0 );
            
      //~ }
              
      //~ for ( lineIndex = _startLineIndex;
            //~ lineIndex < _lineCount;
            //~ lineIndex++ )
      //~ {
        
        //~ fRTEBInit( _slices[ 0 ].getROT_MATRIX() );
              
        //~ if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
        //~ {
          
          //~ if( _centerLineIndex%2 == 0 )
          //~ {
            
            //~ grappaEvenLines = lineIndex%2 != 0;
            
          //~ }
          //~ else
          //~ {
            
            //~ grappaEvenLines = lineIndex%2 == 0;
            
          //~ }
          
          //~ if( lineIndex == _centerLineIndex ||
              //~ lineIndex == _centerLineIndex - 1 ||
              //~ lineIndex == _centerLineIndex + 1 )
          //~ {
            
            //~ if( lineIndex == _centerLineIndex )
            //~ {
              
              //~ multiLineReading3dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                 //~ currentKernelCalls,
                                                 //~ kernelCallsPerRelevantSignal );
            
            //~ }
            
            //~ multiLineReading3dModuleEPI->setAdcReadoutMDHFirstLastScan( 
                                                               //~ mrProt,
                                                               //~ lineIndex,
                                                               //~ 0,
                                                               //~ partitionIndex );
                                                
            //~ OnErrorReturn( runKernel( mrProt,
                                      //~ seqLim,
                                      //~ seqExpo,
                                      //~ KERNEL_IMAGE,
                                      //~ 0,
                                      //~ partitionIndex,
                                      //~ lineIndex ) );
                                      
          //~ }
          //~ else if( grappaEvenLines )
          //~ {
            
            //~ multiLineReading3dModuleEPI->setAdcReadoutMDHFirstLastScan( 
                                                //~ mrProt,
                                                //~ lineIndex,
                                                //~ 0,
                                                //~ partitionIndex );
                                                
            //~ OnErrorReturn( runKernel( mrProt,
                                      //~ seqLim,
                                      //~ seqExpo,
                                      //~ KERNEL_IMAGE,
                                      //~ 0,
                                      //~ partitionIndex,
                                      //~ lineIndex ) );
            
          //~ }
        
        //~ }
        //~ else
        //~ {
          
          //~ if( lineIndex == _centerLineIndex )
          //~ {
            
            //~ multiLineReading3dModuleEPI->setAdcReadoutRelevantForMeasureTime( 
                                                 //~ currentKernelCalls,
                                                 //~ kernelCallsPerRelevantSignal );
          
          //~ }
                
          //~ multiLineReading3dModuleEPI->setAdcReadoutMDHFirstLastScan( 
                                                               //~ mrProt,
                                                               //~ lineIndex,
                                                               //~ 0,
                                                               //~ partitionIndex );
            
          //~ OnErrorReturn( runKernel( mrProt,
                                    //~ seqLim,
                                    //~ seqExpo,
                                    //~ KERNEL_IMAGE,
                                    //~ 0,
                                    //~ partitionIndex,
                                    //~ lineIndex ) );
        
        //~ }
        
        //~ status = fRTEBFinish();    
      
      //~ }
          
          
    //~ }
  

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


NLS_STATUS gkg::GinkgoSpinEcho3d::runKernel( MrProt& mrProt, 
                                           SeqLim& seqLim,  
                                           MrProtocolData::SeqExpo& seqExpo,
                                           long kernelMode,  
                                           long /*sliceIndex*/, 
                                           long partitionIndex, 
                                           long lineIndex )
{

  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::runKernel" };

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
                                       0,
                                       partitionIndex,
                                       lineIndex );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }

    ++ sm;

  }
  
  //////////////////////////////////////////////////////////////////////////////
  // duration
  //////////////////////////////////////////////////////////////////////////////
  
  //~ long readoutTotalDuration = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getRampDownTime() +
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getDuration();
  
  //~ long readoutRampTime = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getRampDownTime();
  
  //~ long partitionInducedShift = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getPartitionEncodingGradientPulse().getDuration();
  
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
    static_cast< gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->getAdcShift();
  double readoutDuration = 
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  long readoutTotalDuration = 
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getRampDownTime() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     second.getPointer() )->
                     getReadoutUpGradientPulse().getDuration();
  
  long phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getPhaseEncodingGradientPulse().getRampDownTime();
  long readoutModuleTotalDuration = 
       _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->getTotalDuration();
  
  //~ long readoutDuration = 
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getRampDownTime() +
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->
                     //~ getReadoutUpGradientPulse().getDuration();

  //~ long adcShift =  
    //~ static_cast< gkg::MultiLineEPIReading3dModule* >(
                     //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
                     //~ second.getPointer() )->getAdcShift();
  
  //~ long phaseEncodeDuration =  
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
       //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       //~ second.getPointer() )->getPhaseEncodingGradientPulse().getDuration() +
    //~ static_cast< const gkg::MultiLineEPIReading3dModule* >(
       //~ _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       //~ second.getPointer() )->getPhaseEncodingGradientPulse().getRampDownTime();
  
  //~ long readoutModuleTotalDuration = 
       //~ _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->getTotalDuration();                   
  
  //~ long centerLineReadoutCenterTime = 
                     //~ ( - _startLine - 1 ) * readoutModuleTotalDuration +
                     //~ phaseEncodeDuration + ( adcShift / 2 ) +
                     //~ fSDSRoundUpGRT( readoutTotalDuration / 2 );
                     
  double centerLineReadoutCenterTime = 
                          ( - _startLine - 1 ) * readoutModuleTotalDuration +
                          phaseEncodeDuration + ( adcShift / 2 ) +
                          fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    phaseEncodeDuration =  
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getDuration() +
    static_cast< const gkg::MultiLineEPIReading3dModule* >(
       _sequenceModules.find( "05-MultiLineEPIReading3dModule" )->
       second.getPointer() )->
       getGrappaPhaseEncodingGradientPulse().getRampDownTime();
    centerLineReadoutCenterTime = 
                     _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                                                          getTotalDuration() +
                     ( - _startLine / 2 ) * readoutModuleTotalDuration +
                     phaseEncodeDuration + ( adcShift / 2 ) + 
                     fSDSRoundUpGRT( readoutTotalDuration / 2 );
  
  }
  
  if( lineIndex == _startLineIndex ||
      ( _grappaFlag == true && lineIndex == _lineCount / 4 - 1 ) )
  {
  
  _sequenceModules[ "00-FatSaturationModule" ]->setAnchorTime( 
         _sequenceModules[ "00-FatSaturationModule" ]->
                                             getDurationBeforeAnchorTime() );
  
  _sequenceModules[ "01-SingleTxExcitation3dModule" ]->setAnchorTime( 
         _sequenceModules[ "00-FatSaturationModule" ]->getEndTime() +
         _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                getDurationBeforeAnchorTime() );
  
  _sequenceModules[ "02-EPIPhaseCorrectionModule" ]->setAnchorTime( 
            _sequenceModules[ "01-SingleTxExcitation3dModule" ]->getEndTime() );  
  
  _sequenceModules[ "03-RefocusingModule" ]->setAnchorTime( 
         _sequenceModules[ "00-FatSaturationModule" ]->getEndTime() +
         mrProt.te()[ 0 ] / 2 +
         _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                getDurationBeforeAnchorTime() );
                                                  
    _sequenceModules[ "04-EPI3dPreEncodingModule" ]->setAnchorTime( 
                 _sequenceModules[ "00-FatSaturationModule" ]->getEndTime() +
                 mrProt.te()[ 0 ] +
                 _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                 getDurationBeforeAnchorTime() -
                 centerLineReadoutCenterTime -
                 _sequenceModules[ "04-EPI3dPreEncodingModule" ]->
                                                           getTotalDuration() );
  
    _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->setAnchorTime( 
              _sequenceModules[ "04-EPI3dPreEncodingModule" ]->getEndTime() );

  
    _totalDuration += _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->
                                                                  getEndTime() +
                      _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                  getDurationBeforeAnchorTime();
  
  }
  else if( lineIndex == _lineCount - adjust ||
           ( _grappaFlag == true && 
             lineIndex == _lineCount - _lineCount / 4 - 1 ) )
  {
    
    _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->setAnchorTime( 0 );
  
    _sequenceModules[ "06-Spoiling3dEPIModule" ]->setAnchorTime( 
           _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->getEndTime() );
    
    _sequenceModules[ "07-EndModule" ]->setAnchorTime( 
           mrProt.tr()[ 0 ] +
           _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                 getDurationBeforeAnchorTime() - 
           _totalDuration );

  }
  else
  {
  
    _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->setAnchorTime( 0 );

    _totalDuration += _sequenceModules[ "05-MultiLineEPIReading3dModule" ]->
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
        
        if( sm != _sequenceModules.find( "07-EndModule" ) &&
            sm != _sequenceModules.find( "06-Spoiling3dEPIModule" ) )
        {
          
          status = sm->second->setKernelTiming( mrProt );
        
        }
        if ( status != SEQU__NORMAL )
        {
        
          return status;
        
        }
      }
      else if( lineIndex == _lineCount - adjust  ||
               ( _grappaFlag == true && 
                 lineIndex == _lineCount - _lineCount / 4 - 1 ) )
      {
        
        if( sm == _sequenceModules.find( "05-MultiLineEPIReading3dModule" ) ||
            sm == _sequenceModules.find( "06-Spoiling3dEPIModule" ) || 
            sm == _sequenceModules.find( "07-EndModule" ) )
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
        
        if( sm == _sequenceModules.find( "05-MultiLineEPIReading3dModule" ) )
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

        if( sm == _sequenceModules.find( "05-MultiLineEPIReading3dModule" ) )
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
                0,
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
                0,
                0,
                0 );
                
    }
    
    mSEQTest( mrProt,
              seqLim,
              seqExpo,
              RTEB_ClockCheck, 
              10,
              lineIndex,
              0,
              0,
              0 );
  
  }

  return status;


}


NLS_STATUS gkg::GinkgoSpinEcho3d::createUI( SeqLim& )
{


#ifdef WIN32

  static const char *ptModule = { "gkg::GinkgoSpinEcho3d::createUI" };

  if ( _userInterface )  
  {

    delete _userInterface;

  }

  try 
  {

    _userInterface = new gkg::GinkgoSpinEcho3dUI();

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


const gkg::GinkgoSpinEcho3dUI* 
                                  gkg::GinkgoSpinEcho3d::getUI( void ) const
{
  
  return _userInterface;

}
