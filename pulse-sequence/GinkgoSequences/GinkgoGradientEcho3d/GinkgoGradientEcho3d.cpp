#include "GinkgoGradientEcho3d.h"
#include "GinkgoGradientEcho3d_UI.h"

#include <Core/SiemensInclude.h> 
#include <SpoilingModules/Spoiling3dModule.h>
#include <ExcitationModules/SingleTxExcitation3dModule.h>
#include <ReadingModules/SingleLineReading3dModule.h>


#define OnErrorReturn( S ) if( !MrSucceeded( S ) ) return( S )

#ifdef SEQUENCE_CLASS_GinkgoGradientEcho3d

SEQIF_DEFINE( gkg::GinkgoGradientEcho3d )

#endif


gkg::GinkgoGradientEcho3d::GinkgoGradientEcho3d()
                    : _userInterface( NULL ),
                      _lineCountPerSecond( 0 )
{


  std::vector< double > singleTxExcitation3dDoubleParameters( 5U );
  singleTxExcitation3dDoubleParameters[ 0 ] = 15.00; // default flip angle 
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

  std::vector< double > singleLineReading3dDoubleParameters( 3U );
  singleLineReading3dDoubleParameters[ 0 ] = 1000; // Default TE
  singleLineReading3dDoubleParameters[ 1 ] = 390; // Default BW per pixel
  singleLineReading3dDoubleParameters[ 2 ] = 256; // Default base resolution
  std::vector< std::string > singleLineReading3dStringParameters;
  _sequenceModules[ "02-SingleLineReading3dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "SingleLineReading3dModule",
                                          singleLineReading3dDoubleParameters,
                                          singleLineReading3dStringParameters,
                                          _sequenceModules );

  std::vector< double > spoiling3dDoubleParameters;
  std::vector< std::string > spoiling3dStringParameters( 2U );
  spoiling3dStringParameters[ 0 ] = "01-SingleTxExcitation3dModule"; 
                                                                   // Excitation
  spoiling3dStringParameters[ 1 ] = "02-SingleLineReading3dModule"; // Reading
  _sequenceModules[ "03-Spoiling3dModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "Spoiling3dModule",
                                          spoiling3dDoubleParameters,
                                          spoiling3dStringParameters,
                                          _sequenceModules );

  std::vector< double > endDoubleParameters( 1U );
  endDoubleParameters[ 0 ] = 200000; // Default TR
  std::vector< std::string > endStringParameters;
  _sequenceModules[ "04-EndModule" ] = 
    gkg::SequenceModuleFactory::getInstance().create(
                                          "EndModule",
                                          endDoubleParameters,
                                          endStringParameters,
                                          _sequenceModules );

}


gkg::GinkgoGradientEcho3d::~GinkgoGradientEcho3d()
{

  if ( _userInterface )
  {
      
    delete _userInterface;
    
  }

}


NLSStatus gkg::GinkgoGradientEcho3d::initialize( SeqLim& seqLim )
{

  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::initialize" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  seqLim.setSequenceOwner( "GINKGO TEAM" );

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
      
    MRTRACE( "Instantiation of GinkgoGradientEcho3dUI class failed!" );
    return status;
    
  }
    
#ifdef WIN32

  if ( !_userInterface ) 
  {
      
    MRTRACE( "GinkgoGradientEcho3dUI object null pointer (creation failed)!" );
    return SEQU_ERROR;
    
  }

  status = _userInterface->registerUI( seqLim );
  if( MrSeverity( status ) != MRRESULT_SUCCESS ) 
  {

    MRTRACE( "Registering GinkgoGradientEcho3dUI object failed! Errorcode: %i",
             status );
    return status;
    
  }

  seqLim.setDefaultEVAProt( _T( "%SiemensEvaDefProt%\\Inline\\Inline.evp" ) );

#endif
  
  return status;
  
    
}


NLSStatus gkg::GinkgoGradientEcho3d::prepare( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo )
{
  
    
  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
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
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
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

  if( ( int32_t ) ( partialFourierFactor * _lineCount ) - _lineCount / 2 <= 10 
      && mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    

    _actualLineCount = ( ( int32_t ) ( _lineCount * grappaFactor ) + 
                       grappaOffset ) / 2 +
                       ( int32_t ) ( partialFourierFactor * _lineCount ) - 
                       _lineCount / 2;
                       
  }
  else
  {
    
    
    _actualLineCount = ( int32_t ) ( partialFourierFactor * _lineCount ) *
                       grappaFactor + grappaOffset;
    
    
  }

  double measureTimeUsec = 0.0;
  
  measureTimeUsec = ( double ) _actualLineCount * 
                    mrProt.tr()[0] * _partitionCount ;

  long kernelRequestsPerMeasurement = _partitionCount * 
                                      _actualLineCount;   
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
                                         
  seqExpo.setRFInfo( _lineCount * 
                     _partitionCount *
                     static_cast< const gkg::SingleTxExcitation3dModule* >(
                      _sequenceModules.find( "01-SingleTxExcitation3dModule" )->
                        second.getPointer() )->
                                         getRfWaveformEnveloppe().getRFInfo() );

          
  //////////////////////////////////////////////////////////////////////////////
  // checking minimum required TE and defining effective TE
  //////////////////////////////////////////////////////////////////////////////

  if( static_cast< const gkg::Spoiling3dModule* >(
                  _sequenceModules.find( "03-Spoiling3dModule" )->
                  second.getPointer() )-> getTotalPartitionRewindingDuration() >
      static_cast< const gkg::SingleLineReading3dModule* >(
                  _sequenceModules.find( "02-SingleLineReading3dModule" )->
                  second.getPointer() )-> getTotalPhaseEncodingDuration() )
  {
    
    return SEQU_ERROR;
    
  }
  
  long minDurationBetweenReadoutAndRFPulse = 
        static_cast<long>(SysProperties::getMinDurationBetweenReadoutAndRFPulse( 
        ( ( double ) static_cast< const gkg::SingleLineReading3dModule* >(
           _sequenceModules.find( "02-SingleLineReading3dModule" )->
           second.getPointer() )-> getAdcReadout().getDwellTime() ) / 
           seqLim.getReadoutOSFactor() / 1000. ) );

  int32_t minimumRequiredTE = 
       _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                  getDurationAfterAnchorTime() +
       _sequenceModules[ "02-SingleLineReading3dModule" ]->
                                               getDurationBeforeAnchorTime(); 

  if ( minimumRequiredTE > mrProt.te()[0] ) 
  {

    return SBB_NEGATIV_TEFILL;

  }    


  //////////////////////////////////////////////////////////////////////////////
  // computing minimum required TR
  //////////////////////////////////////////////////////////////////////////////

  int32_t minimumRequiredTR =
                      _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "02-SingleLineReading3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "03-Spoiling3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "04-EndModule" ]->getTotalDuration();

  if ( minimumRequiredTR > mrProt.tr()[0] ) 
  {

    return SBB_NEGATIV_TRFILL;

  }
  
  long availableTime = fSDSRoundDownGRT( 
             mrProt.te()[0] - 
             ( static_cast< const gkg::SingleTxExcitation3dModule* >(
               _sequenceModules.find( "01-SingleTxExcitation3dModule" )->
               second.getPointer() )-> 
                                      getRfWaveformEnveloppe().getDuration()/2 +
               static_cast< const gkg::SingleTxExcitation3dModule* >(
               _sequenceModules.find( "01-SingleTxExcitation3dModule" )->
               second.getPointer() )-> 
                            getSliceSelectionGradientPulse().getRampDownTime() +
               static_cast< const gkg::SingleLineReading3dModule* >(
               _sequenceModules.find( "02-SingleLineReading3dModule" )->
               second.getPointer() )-> getAdcReadout().getDuration()/2 +
               static_cast< const gkg::SingleLineReading3dModule* >(
               _sequenceModules.find( "02-SingleLineReading3dModule" )->
               second.getPointer() )-> getReadout().getRampDownTime() )); 

  if ( availableTime <= 0 )
  {
      return SBB_NEGATIV_TEFILL ;
  }


  //////////////////////////////////////////////////////////////////////////////
  // preparing reconstruction
  //////////////////////////////////////////////////////////////////////////////

  fSUSetSequenceString( "Ginkgo",
                        mrProt,
                        seqExpo );
                              
  seqExpo.setOnlineFFT( SEQ::ONLINE_FFT_PHASE );
      
  seqExpo.setICEProgramFilename( "%SiemensIceProgs%\\IceProgramOnlinePeFt3D" );
  seqExpo.setICEProgramParam( ICE_PROGRAM_PARA_SHOW_OFFLINE, 
                              SEQ::SO_SHOW_YES );

  return status;

}


NLSStatus gkg::GinkgoGradientEcho3d::check( MrProt& mrProt, 
                                      SeqLim& seqLim, 
                                      MrProtocolData::SeqExpo& seqExpo, 
                                      SEQCheckMode* /* checkMode */ )
{

  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::check" };

  NLS_STATUS status = SEQU__NORMAL;

  int lineIndex = 0;
  int partitionIndex = 0;
  // Max gradient lines
  int linesToCheck[ 4 ] = { 0,
                            1,
                            seqExpo.getMeasuredPELines() - 2,
                            seqExpo.getMeasuredPELines() - 1 };
  int partitionsToCheck[ 4 ] = { 0,
                                 1,
                                 seqExpo.getMeasured3dPartitions() - 2,
                                 seqExpo.getMeasured3dPartitions() - 1 };

  while ( ( lineIndex < 4 ) && ( ( ( status ) & NLS_SEV ) == NLS_SUCCESS ) )
  {
    
    partitionIndex = 0;
  
    while( ( partitionIndex < 4 ) && ( ( ( status ) & NLS_SEV ) == NLS_SUCCESS ) )
    {
      
      status = runKernel( mrProt, 
                          seqLim, 
                          seqExpo, 
                          KERNEL_CHECK, 
                          0, 
                          partitionsToCheck[ partitionIndex ], 
                          linesToCheck[ lineIndex ]);
      ++ partitionIndex;
      
    }

    ++ lineIndex;

  }

  return status;

}


NLSStatus gkg::GinkgoGradientEcho3d::run( MrProt& mrProt, 
                                        SeqLim& seqLim, 
                                        MrProtocolData::SeqExpo& seqExpo )
{


  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::run" };
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
                                         
  gkg::SingleLineReading3dModule*
    singleLineReading3dModule = static_cast< gkg::SingleLineReading3dModule* >(
              _sequenceModules[ "02-SingleLineReading3dModule" ].getPointer() );
  
  //~ singleLineReading3dModule->setAdcReadoutMDHParameters( mrProt );
  
  int32_t lineIndex = 0;
  int32_t currentKernelCalls = 0;
  int32_t partitionIndex = 0; 
  int32_t repetitionIndex = 0; 
  long grappaLineIndex = _startLineIndex;

  long measureTimeUsec = ( double )_actualLineCount * 
                                   mrProt.tr()[0] * _partitionCount ;
  long kernelRequestsPerMeasurement = /*_sliceCount * */
                                      _partitionCount * 
                                      _actualLineCount;   
  long kernelCallsPerRelevantSignal =  
             std::max<long>( 1, 
                             ( long )( kernelRequestsPerMeasurement * 1000000. /
                             measureTimeUsec ) );
  
  int32_t minimumRequiredTR =
                      _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "02-SingleLineReading3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "03-Spoiling3dModule" ]->
                                                            getTotalDuration() +
                      _sequenceModules[ "04-EndModule" ]->getTotalDuration();
  
  
  for( repetitionIndex = 0; 
       repetitionIndex <= mrProt.repetitions();
       repetitionIndex++ )
  {
    
    singleLineReading3dModule->setRepetitionIndex( repetitionIndex );
    for( partitionIndex = _startPartitionIndex; 
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
                    0,
                    0,
                    0 );
              
        }
                
        grappaLineIndex = _startLineIndex; 
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
                    
              singleLineReading3dModule->setAdcReadoutRelevantForMeasureTime( 
                                                       currentKernelCalls,
                                                       kernelCallsPerRelevantSignal );
                    
              //~ singleLineReading3dModule->setAdcReadoutMDHFirstLastScan( 
                                                //~ mrProt,
                                                //~ lineIndex,
                                                //~ 0,
                                                //~ partitionIndex );
              singleLineReading3dModule->setAdcReadoutMDH( mrProt,
                                                           lineIndex,
                                                           partitionIndex );
                
              OnErrorReturn( runKernel( mrProt,
                                        seqLim,
                                        seqExpo,
                                        KERNEL_IMAGE,
                                        0,
                                        partitionIndex,
                                        lineIndex ) );
              
              ++ grappaLineIndex;     
              
            }
            
          }
          else
          {
          
            ++ currentKernelCalls;
                  
            singleLineReading3dModule->setAdcReadoutRelevantForMeasureTime( 
                                                      currentKernelCalls,
                                                      kernelCallsPerRelevantSignal );
                  
            //~ singleLineReading3dModule->setAdcReadoutMDHFirstLastScan( 
                                              //~ mrProt,
                                              //~ lineIndex,
                                              //~ 0,
                                              //~ partitionIndex );
            singleLineReading3dModule->setAdcReadoutMDH( mrProt,
                                                         lineIndex,
                                                         partitionIndex );
              
            OnErrorReturn( runKernel( mrProt,
                                      seqLim,
                                      seqExpo,
                                      KERNEL_IMAGE,
                                      0,
                                      partitionIndex,
                                      lineIndex ) );
            
            ++ grappaLineIndex;     
            
          }
        
        //~ for ( lineIndex = _startLineIndex;
              //~ lineIndex < _lineCount;
              //~ lineIndex++ )
        //~ {
          
          //~ ++ currentKernelCalls;
                
          //~ singleLineReading3dModule->setAdcReadoutRelevantForMeasureTime( 
                                                   //~ currentKernelCalls,
                                                   //~ kernelCallsPerRelevantSignal );
                
          //~ // singleLineReading3dModule->setAdcReadoutMDHFirstLastScan( 
                                            //~ // mrProt,
                                            //~ // lineIndex,
                                            //~ // 0,
                                            //~ // partitionIndex );
          //~ singleLineReading3dModule->setAdcReadoutMDH( mrProt,
                                                       //~ lineIndex,
                                                       //~ partitionIndex );  
          
          //~ OnErrorReturn( runKernel( mrProt,
                                    //~ seqLim,
                                    //~ seqExpo,
                                    //~ KERNEL_IMAGE,
                                    //~ 0,
                                    //~ partitionIndex,
                                    //~ lineIndex ) );
          
             
        //~ }
        
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


NLS_STATUS gkg::GinkgoGradientEcho3d::runKernel( MrProt& mrProt, 
                                           SeqLim& seqLim,  
                                           MrProtocolData::SeqExpo& seqExpo,
                                           long kernelMode,  
                                           long /*sliceIndex*/, 
                                           long partitionIndex, 
                                           long lineIndex )
{

  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::runKernel" };

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
  // STARTING SEQUENCE TIMING
  //////////////////////////////////////////////////////////////////////////////
  
  fRTEBInit( _slices[ 0 /*sliceIndex*/ ].getROT_MATRIX() );


  //////////////////////////////////////////////////////////////////////////////
  // setting anchor time for all module(s)
  //////////////////////////////////////////////////////////////////////////////
  
  _sequenceModules[ "01-SingleTxExcitation3dModule" ]->setAnchorTime( 
         _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                getDurationBeforeAnchorTime() );
 
  _sequenceModules[ "02-SingleLineReading3dModule" ]->setAnchorTime( 
         mrProt.te()[ 0 ] +
         _sequenceModules[ "01-SingleTxExcitation3dModule" ]->
                                                getDurationBeforeAnchorTime() );

  _sequenceModules[ "03-Spoiling3dModule" ]->setAnchorTime( 
             _sequenceModules[ "02-SingleLineReading3dModule" ]->getEndTime() );

   _sequenceModules[ "04-EndModule" ]->setAnchorTime( mrProt.tr()[ 0 ] );


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

  OnErrorReturn( fRTEBFinish() );

  return status;


}


NLS_STATUS gkg::GinkgoGradientEcho3d::createUI( SeqLim& )
{


#ifdef WIN32

  static const char *ptModule = { "gkg::GinkgoGradientEcho3d::createUI" };

  if ( _userInterface )  
  {

    delete _userInterface;

  }

  try 
  {

    _userInterface = new gkg::GinkgoGradientEcho3dUI();

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


const gkg::GinkgoGradientEcho3dUI* 
                                  gkg::GinkgoGradientEcho3d::getUI( void ) const
{
  
  return _userInterface;

}
