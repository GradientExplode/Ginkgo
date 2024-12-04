#include "GinkgoSpinEcho2d.h"
#include "GinkgoSpinEcho2d_UI.h"

#include <Core/SiemensInclude.h> 
#include <ExcitationModules/SingleTxExcitation2dModule.h>
#include <RefocusingModules/SimpleRefocusingModule.h>
#include <ReadingModules/SingleLineReading2dModule.h>
#include <SpoilingModules/Spoiling2dModule.h>


#define OnErrorReturn( S ) if( !MrSucceeded( S ) ) return( S )

#ifdef SEQUENCE_CLASS_GinkgoSpinEcho2d

SEQIF_DEFINE( gkg::GinkgoSpinEcho2d )

#endif


gkg::GinkgoSpinEcho2d::GinkgoSpinEcho2d()
                    : _userInterface( NULL ),
                      _lineCountPerSecond( 0 ),
                      _rfSpoilIncrement( 0 ),
                      _rfSpoilPhase( 0 )
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
                                          
  std::vector< double > refocusingDoubeParameters;
  std::vector< std::string > refocusingStringParameters;
  _sequenceModules[ "02-RefocusingModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SimpleRefocusingModule",
                                          refocusingDoubeParameters,
                                          refocusingStringParameters,
                                          _sequenceModules );                                          

  std::vector< double > singleLineReading2dDoubleParameters( 3U );
  singleLineReading2dDoubleParameters[ 0 ] = 1000; // Default TE
  singleLineReading2dDoubleParameters[ 1 ] = 390; // Default BW per pixel
  singleLineReading2dDoubleParameters[ 2 ] = 256; // Default base resolution
  std::vector< std::string > singleLineReading2dStringParameters;
  _sequenceModules[ "03-SingleLineReading2dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SingleLineReading2dModule",
                                          singleLineReading2dDoubleParameters,
                                          singleLineReading2dStringParameters,
                                          _sequenceModules );

  std::vector< double > spoiling2dDoubleParameters;
  std::vector< std::string > spoiling2dStringParameters( 2U );
  spoiling2dStringParameters[ 0 ] = "01-SingleTxExcitation2dModule"; 
                                                                   // Excitation
  spoiling2dStringParameters[ 1 ] = "03-SingleLineReading2dModule"; // Reading
  _sequenceModules[ "04-Spoiling2dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "Spoiling2dModule",
                                          spoiling2dDoubleParameters,
                                          spoiling2dStringParameters,
                                          _sequenceModules );

  std::vector< double > endDoubleParameters( 1U );
  endDoubleParameters[ 0 ] = 2000000; // Default TR
  std::vector< std::string > endStringParameters;
  _sequenceModules[ "05-EndModule" ] = 
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

  status = _userInterface->registerUI( seqLim );
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

  long actualLineCount; 
  if( ( int32_t ) ( partialFourierFactor * _lineCount ) - _lineCount / 2 <= 10 
      && mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    

    actualLineCount = ( ( int32_t ) ( _lineCount * grappaFactor ) + 
                       grappaOffset ) / 2 +
                       ( int32_t ) ( partialFourierFactor * _lineCount ) - 
                       _lineCount / 2;
                       
  }
  else
  {
    
    
    actualLineCount = ( int32_t ) ( partialFourierFactor * _lineCount ) *
                       grappaFactor + grappaOffset;
    
    
  }

  double measureTimeUsec = ( double ) actualLineCount * 
                           mrProt.tr()[0] * _sliceCount;
                                            
  seqExpo.setMeasureTimeUsec( measureTimeUsec );
  seqExpo.setTotalMeasureTimeUsec( measureTimeUsec );
  seqExpo.setMeasured3dPartitions( 1 );
  seqExpo.setMeasuredPELines( _lineCount );
  seqExpo.setSlicePerConcat( 0 );
  
  seqExpo.setRelevantReadoutsForMeasTime( _sliceCount * _lineCount );

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

  seqExpo.setRFInfo( _lineCount * _sliceCount *
                    ( static_cast< const gkg::SingleTxExcitation2dModule* >(
                      _sequenceModules.find( "01-SingleTxExcitation2dModule" )->
                        second.getPointer() )->
                                        getRfWaveformEnveloppe().getRFInfo() + 
                      static_cast< const gkg::SimpleRefocusingModule* >(
                      _sequenceModules.find( "02-RefocusingModule" )->
                        second.getPointer() )->
                             getRfRefocusingWaveformEnveloppe().getRFInfo() ) );

          
  //////////////////////////////////////////////////////////////////////////////
  // checking minimum required TE and defining effective TE
  //////////////////////////////////////////////////////////////////////////////

  int32_t minimumRequiredTE = 2 * std::max(
       _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                   getDurationAfterAnchorTime(),
       _sequenceModules[ "03-SingleLineReading2dModule" ]->
                                               getDurationBeforeAnchorTime() ) +
       _sequenceModules[ "02-RefocusingModule" ]-> getTotalDuration(); 

  if ( minimumRequiredTE > mrProt.te()[0] ) 
  {

    return SBB_NEGATIV_TEFILL;

  }    


  //////////////////////////////////////////////////////////////////////////////
  // computing minimum required TR
  //////////////////////////////////////////////////////////////////////////////

  int32_t minimumRequiredTR =
                      _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                            getTotalDuration() +
                      mrProt.te()[0] +
                      _sequenceModules[ "03-SingleLineReading2dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "04-Spoiling2dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "05-EndModule" ]->getTotalDuration();

  if ( minimumRequiredTR > mrProt.tr()[0] ) 
  {

    return SBB_NEGATIV_TRFILL;

  }


  //////////////////////////////////////////////////////////////////////////////
  // preparing reconstruction
  //////////////////////////////////////////////////////////////////////////////

  fSUSetSequenceString( "2dse",
                        mrProt,
                        seqExpo );

  // real time reconstruction
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

  int line = 0;
  // Max gradient lines
  int linesToCheck[ 4 ] = { 0,
                            1,
                            seqExpo.getMeasuredPELines() - 2,
                            seqExpo.getMeasuredPELines() - 1 };

  while ( line < 4 )
  {
  
    OnErrorReturn( runKernel( mrProt,
                              seqLim,
                              seqExpo,
                              KERNEL_CHECK,
                              0,
                              0,
                              linesToCheck[ line ] ) );
    ++ line;

  }

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
                                         
  gkg::SingleLineReading2dModule*
    singleLineReading2dModule = static_cast< gkg::SingleLineReading2dModule* >(
              _sequenceModules[ "03-SingleLineReading2dModule" ].getPointer() );

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
  
  int32_t currentKernelCalls = 0;
  double measureTimeUsec = 0;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    measureTimeUsec = ( double ) ( _lineCount / 2 + 10 ) * 
                      mrProt.tr()[0] * _sliceCount ;
    
  }
  else
  {
    
    measureTimeUsec = ( double ) _lineCount * mrProt.tr()[0] * _sliceCount ;
    
  }
  long kernelRequestsPerMeasurement = _sliceCount; 
  long kernelCallsPerRelevantSignal =  
             std::max<long>( 1, 
                             ( long )( kernelRequestsPerMeasurement * 1000000. /
                             measureTimeUsec ) );
  
  
  long lineIndex = 0;
  long sliceIndex = 0;
  long repetitionIndex = 0;
  
  for( repetitionIndex = 0; 
       repetitionIndex <= mrProt.repetitions();
       repetitionIndex++ )
  {
    
    singleLineReading2dModule->setRepetitionIndex( repetitionIndex );
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
          
                 
      for ( lineIndex = _startLineIndex;
            lineIndex < _lineCount;
            lineIndex++ )
          
      {
        
        if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
        {
          
          if( ( lineIndex <= _centerLineIndex + 10 &&
                lineIndex >= _centerLineIndex - 10 ) ||
              ( ( repetitionIndex%2 == 0 && lineIndex%2 == 0 ) ||
                ( repetitionIndex%2 != 0 && lineIndex%2 != 0 ) ) )
          {
            
            
            ++ currentKernelCalls;
                  
            singleLineReading2dModule->setAdcReadoutRelevantForMeasureTime( 
                                                currentKernelCalls,
                                                kernelCallsPerRelevantSignal );
                  
            singleLineReading2dModule->setAdcReadoutMDH( 
                                      mrProt,
                                      lineIndex,
                                      _slices[ sliceIndex ].getSliceIndex() );
            
                  
            OnErrorReturn( runKernel( mrProt,
                                      seqLim,
                                      seqExpo,
                                      KERNEL_IMAGE,
                                      _slices[ sliceIndex ].getSliceIndex(),
                                      0,
                                      lineIndex ) );
            
          }
                                    
        }
        else
        {
          
          ++ currentKernelCalls;
              
          singleLineReading2dModule->setAdcReadoutRelevantForMeasureTime( 
                                                currentKernelCalls,
                                                kernelCallsPerRelevantSignal );
                
          singleLineReading2dModule->setAdcReadoutMDH( 
                                      mrProt,
                                      lineIndex,
                                      _slices[ sliceIndex ].getSliceIndex() );
          
                
          OnErrorReturn( runKernel( mrProt,
                                    seqLim,
                                    seqExpo,
                                    KERNEL_IMAGE,
                                    _slices[ sliceIndex ].getSliceIndex(),
                                    0,
                                    lineIndex ) );
                                    
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
                                           long partition, 
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
                                       partition,
                                       lineIndex );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

    }

    ++ sm;

  }


  //////////////////////////////////////////////////////////////////////////////
  // STARTING SEQUENCE TIMING
  //////////////////////////////////////////////////////////////////////////////
  
  fRTEBInit( _slices[ sliceIndex ].getROT_MATRIX() );


  //////////////////////////////////////////////////////////////////////////////
  // setting anchor time for all module(s)
  //////////////////////////////////////////////////////////////////////////////
  
  _sequenceModules[ "01-SingleTxExcitation2dModule" ]->setAnchorTime( 
         _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                getDurationBeforeAnchorTime() );
                                                
  _sequenceModules[ "02-RefocusingModule" ]->setAnchorTime( 
         mrProt.te()[ 0 ] / 2 +
         _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                getDurationBeforeAnchorTime() );
 
  _sequenceModules[ "03-SingleLineReading2dModule" ]->setAnchorTime( 
         mrProt.te()[ 0 ] +
         _sequenceModules[ "01-SingleTxExcitation2dModule" ]->
                                                 getDurationBeforeAnchorTime());

  _sequenceModules[ "04-Spoiling2dModule" ]->setAnchorTime( 
         _sequenceModules[ "03-SingleLineReading2dModule" ]->getEndTime() );

   _sequenceModules[ "05-EndModule" ]->setAnchorTime( mrProt.tr()[ 0 ] );


  //////////////////////////////////////////////////////////////////////////////
  // setting kernel timing of all module(s)
  //////////////////////////////////////////////////////////////////////////////

  sm = _sequenceModules.begin();
  while ( sm != sme )
  {

    status = sm->second->setKernelTiming( mrProt );
    if ( status != SEQU__NORMAL )
    {
    
      return status;

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

  OnErrorReturn( fRTEBFinish() );

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
