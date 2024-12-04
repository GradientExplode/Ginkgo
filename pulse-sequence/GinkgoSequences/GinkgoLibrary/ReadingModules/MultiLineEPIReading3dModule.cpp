#include <ReadingModules/MultiLineEPIReading3dModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::MultiLineEPIReading3dModule::MultiLineEPIReading3dModule( 
              const std::vector< double >& doubleParameters,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string,
              gkg::RCPointer< gkg::SequenceModule > >& otherSequenceModules )
                               : gkg::ReadingModule(
                                    gkg::ReadingModule::CartesianEchoVolumar3d,
                                    otherSequenceModules.find(
                                    "01-SingleTxExcitation3dModule" )->second ),
                                 // RF-signal axis

                                 // Slice gradient axis (Z)

                                 // Phase gradient axis (Y)
                                 _phaseEncoding( "phaseEncoding" ),
                                 _grappaPhaseEncoding( "grappaPhaseEncoding" ),

                                 // Read gradient axis (X)
                                 _readoutUp( "readoutUp" ),
                                 _readoutDown( "readoutDown" ),

                                 // ADC signal data axis
                                 _adcReadout( "adcReadout" ),
                                 _adcOffsets( "adcOffsets" ),
                                 _adcResetSynthetizer( "adcResetSynthetizer" )                        
{
  
  _defaultTE = doubleParameters[ 0 ];
  _defaultBWPerPixel = doubleParameters[ 1 ];
  _defaultBaseResolution = doubleParameters[ 2 ];
  
}


gkg::MultiLineEPIReading3dModule::~MultiLineEPIReading3dModule()
{
}


void gkg::MultiLineEPIReading3dModule::initialize( SeqLim& seqLim )
{
  
  //////////////////////////////////////////////////////////////////////////////
  // calling initialize methode of the mother class
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::ReadingModule::initialize( seqLim );
  
  //////////////////////////////////////////////////////////////////////////////
  // initializing specific parameters
  //////////////////////////////////////////////////////////////////////////////

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
  
  seqLim.setSliceThickness( 1.000, 
                            10.000, 
                            0.500, 
                            10.000 );
  seqLim.setSliceDistanceFactor( 0.000, 
                                 8.000, 
                                 0.010, 
                                 0.200 ); 

  seqLim.setMultiSliceMode( SEQ::MSM_SEQUENTIAL );
  seqLim.setSliceSeriesMode( SEQ::ASCENDING );
  seqLim.enableSliceShift();
  seqLim.enableMSMA();
  seqLim.enableOffcenter();
  seqLim.setAllowedSliceOrientation( SEQ::DOUBLE_OBLIQUE );
  
  seqLim.setImagesPerSlab( 8,
                           512,
                           2,
                           16 );
  seqLim.setPartition( 8,
                       256,
                       2,
                       16 );
  
  seqLim.setSlabThickness( 10.000,
                           160.000 );
  seqLim.set3DPartThickness( 0.100,
                             200,
                             0.01,
                             1.0 );
  
  seqLim.setSliceOversampling( 0.0000, 1.0000, 0.0100, 0.0000 );
  seqLim.setMinSliceResolution( 0.5 );
  
  
  seqLim.setPhases( 1,
                    K_NO_SLI_MAX,
                    1,
                    1 );
  
}

 

NLSStatus gkg::MultiLineEPIReading3dModule::prepare( 
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::MultiLineEPIReading3dModule::prepare" };
  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::ReadingModule::prepare( mrProt,
                                     seqLim,
                                     seqExpo );
                                  
  //////////////////////////////////////////////////////////////////////////////
  // creating an ADC gate
  //////////////////////////////////////////////////////////////////////////////
  
  _adcReadout.setColumns( mrProt.kSpace().getlBaseResolution() );
  _adcReadout.setDwellTime( long( ( mrProt.rxSpec().effDwellTime(
                                   seqLim.getReadoutOSFactor() )[0] ) + 0.5 ) );

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
                                          - 1 ) ) // !!! -1?
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
  // creating a grappa phase encoding gradient
  //////////////////////////////////////////////////////////////////////////////
  
  _grappaPhaseEncoding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  _grappaPhaseEncoding.setMinRiseTime( _minimumRiseTimePerMeter );
  if( !_grappaPhaseEncoding.prepPEShortestTime( mrProt,
                                                SEQ::DIR_ASCENDING,
                                                0.0,
                                                - 2 ) ) // !!! -2?
  {

    return _grappaPhaseEncoding.getNLSStatus();

  }

  if( !_grappaPhaseEncoding.check() )
  {

    return _grappaPhaseEncoding.getNLSStatus();

  }

  //////////////////////////////////////////////////////////////////////////////
  // preparing timing
  //////////////////////////////////////////////////////////////////////////////
  
  _adcShift = 0;
  long minDistanceBetweenRO = fSDSRoundUpGRT( 
                                       _adcReadout.getMinDistanceBetweenReadout( 
                                       _adcReadout, true, mrProt ) );
  if( mrProt.PAT().getucPATMode() == SEQ::PAT_MODE_GRAPPA )
  {
    
    if( _readoutUp.getRampDownTime() +
        _grappaPhaseEncoding.getDuration() +
        _grappaPhaseEncoding.getRampDownTime() < 
        minDistanceBetweenRO )
    {
      
      _adcShift = minDistanceBetweenRO -
                 _readoutUp.getRampDownTime() +
                 _grappaPhaseEncoding.getDuration() +
                 _grappaPhaseEncoding.getRampDownTime();
                 
    }

  }                           
  else
  {
    
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
                               
  }
   
  _phaseEncoding.setStartTime( 0 ); 
  _grappaPhaseEncoding.setStartTime( 0 ); 
  
  long readoutStartTime = 0;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
    
    readoutStartTime = _grappaPhaseEncoding.getDuration() +
                       _grappaPhaseEncoding.getRampDownTime() +
                       _adcShift / 2;
  
  }
  else
  {
    
    readoutStartTime = _phaseEncoding.getDuration() +
                       _phaseEncoding.getRampDownTime() +
                       _adcShift / 2;
    
  }
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

  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {

    _durationAfterAnchorTime = std::max( _readoutUp.getDuration() +
                                         _readoutUp.getRampDownTime(),
                                         _readoutDown.getDuration() +
                                         _readoutDown.getRampDownTime() ) +
                               _grappaPhaseEncoding.getDuration() +
                               _grappaPhaseEncoding.getRampDownTime() +
                               _adcShift; 
  
  }                           
  else
  {
  
    _durationAfterAnchorTime = std::max( _readoutUp.getDuration() +
                                         _readoutUp.getRampDownTime(),
                                         _readoutDown.getDuration() +
                                         _readoutDown.getRampDownTime() ) +
                               _phaseEncoding.getDuration() +
                               _phaseEncoding.getRampDownTime() +
                               _adcShift; 
                               
  }
 
  return status;

}


NLS_STATUS gkg::MultiLineEPIReading3dModule::modifyKernel( 
                                               MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partitionIndex,
                                               long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;
  
  _lineIndex = lineIndex;
  _partitionIndex = partitionIndex;

  
  if( mrProt.PAT().getucPATMode() == SEQ::PAT_MODE_GRAPPA )
  {
    
    if( _lineIndex%4 == 0 )
    {
        
        _adcOffsets.prepSet( slices[ sliceIndex ], 
                             _adcReadout, 
                             _readoutDown,
                             lineIndex - _centerLineIndex,
                             partitionIndex - _centerPartitionIndex );
        _adcResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                      _adcReadout, 
                                      _readoutDown,
                                      lineIndex - _centerLineIndex,
                                      partitionIndex - _centerPartitionIndex );
    
    }
    else
    {
        
        _adcOffsets.prepSet( slices[ sliceIndex ], 
                             _adcReadout, 
                             _readoutUp,
                             lineIndex - _centerLineIndex,
                             partitionIndex - _centerPartitionIndex );
        _adcResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                      _adcReadout, 
                                      _readoutUp,
                                      lineIndex - _centerLineIndex,
                                      partitionIndex - _centerPartitionIndex );
    
    }
    
  }
  else
  {
    
    if( _lineIndex%2 == 0 )
    {
        
        _adcOffsets.prepSet( slices[ sliceIndex ], 
                             _adcReadout, 
                             _readoutDown,
                             lineIndex - _centerLineIndex,
                             partitionIndex - _centerPartitionIndex );
        _adcResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                      _adcReadout, 
                                      _readoutDown,
                                      lineIndex - _centerLineIndex,
                                      partitionIndex - _centerPartitionIndex );
    
    }
    else
    {
        
        _adcOffsets.prepSet( slices[ sliceIndex ], 
                             _adcReadout, 
                             _readoutUp,
                             lineIndex - _centerLineIndex,
                             partitionIndex - _centerPartitionIndex );
        _adcResetSynthetizer.prepNeg( slices[ sliceIndex ], 
                                      _adcReadout, 
                                      _readoutUp,
                                      lineIndex - _centerLineIndex,
                                      partitionIndex - _centerPartitionIndex );
    
    }
    
  }
  
  return status;

}


NLS_STATUS gkg::MultiLineEPIReading3dModule::setKernelTiming( MrProt& mrProt )
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
  
  int32_t adjust = 1;
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
  
      adjust = 2;
  
  }
  else
  {
  
    adjust = 1;
  
  }
  
  long minDistanceBetweenRO = fSDSRoundUpGRT( 
                                       _adcReadout.getMinDistanceBetweenReadout( 
                                       _adcReadout, true, mrProt ) );
    
  long adcReadoutStartTime = _adcReadout.getStartTime();
  long adcReadoutEndTime = adcReadoutStartTime + 
                           ( long )_adcReadout.getRoundedDuration();
                           
  bool patialFourierDownLine = true;
  bool patialFourierUpLine = true;
  
  if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
  {
      
    if( _grappaFlag == true )
    {
      
      if( _lineIndex%2 == 0 )
      {
        
        fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                _readoutDown.getStartTime() ),
                0,
                0,
                0,
                0, 
                &_readoutDown,
                0,
                0 );
      
      }
      else
      {
        
        fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                _readoutUp.getStartTime() ),
                0,
                0,
                0,
                0, 
                &_readoutUp,
                0,
                0 );
        
      }
      
      if( ( _lineIndex < _centerLineIndex + 10 &&
            _lineIndex >= _centerLineIndex - 10 ) )
      {
        
        if( _lineIndex != _startLineIndex )
        {
            
          fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                  _phaseEncoding.getStartTime() ), 
                  0, 
                  0,
                  0, 
                  &_phaseEncoding, 
                  0, 
                  0,
                  0 );
                  
        }
        
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
                
      }
              
    }
    else
    {
      
      if( _lineIndex != _startLineIndex )
      {
          
        fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                _grappaPhaseEncoding.getStartTime() ), 
                0, 
                0,
                0, 
                &_grappaPhaseEncoding, 
                0, 
                0,
                0 );
                
      }
      
      if( _lineIndex%4 == 0 )
      {
        
        fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                _readoutDown.getStartTime() ),
                0,
                0,
                0,
                0, 
                &_readoutDown,
                0,
                0 );
      
      }
      else
      {
        
        fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                _readoutUp.getStartTime() ),
                0,
                0,
                0,
                0, 
                &_readoutUp,
                0,
                0 );
        
      }
      
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
              
    }
    
  }
  else
  {
                           
    if( _lineIndex != _startLineIndex )
    {
        
      fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              _phaseEncoding.getStartTime() ), 
              0, 
              0,
              0, 
              &_phaseEncoding, 
              0, 
              0,
              0 );
              
    }
    
      
    if( _lineIndex%2 == 0 )
    {
      
      fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              _readoutDown.getStartTime() ),
              0,
              0,
              0,
              0, 
              &_readoutDown,
              0,
              0 );
    
    }
    else
    {
      
      fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              _readoutUp.getStartTime() ),
              0,
              0,
              0,
              0, 
              &_readoutUp,
              0,
              0 );
      
    }
    
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
    
  }
  
  fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
         0, 
         0, 
         0, 
         0, 
         0, 
         0, 
         0 );
                           
  //~ if( _lineIndex == _startLineIndex )
  //~ {
    
    //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
           //~ _readoutDown.getStartTime() ),
           //~ 0,
           //~ 0,
           //~ 0,
           //~ 0, 
           //~ &_readoutDown,
           //~ 0,
           //~ 0 );
           
    //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
           //~ adcReadoutStartTime ),
           //~ &_adcOffsets, 
           //~ 0, 
           //~ &_adcReadout, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
    
    //~ fRTEI( this->getStartTime() +
           //~ adcReadoutEndTime,
           //~ &_adcResetSynthetizer, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
           
    //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
    
  //~ }
  //~ else
  //~ {

    //~ if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
    //~ {
      
      //~ if( _lineIndex == _centerLineIndex ||
          //~ _lineIndex == _centerLineIndex - 1 ||
          //~ _lineIndex == _centerLineIndex + 1 )
      //~ {
        
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              //~ _phaseEncoding.getStartTime() ), 
              //~ 0, 
              //~ 0,
              //~ 0, 
              //~ &_phaseEncoding, 
              //~ 0, 
              //~ 0,
              //~ 0 );
        
      //~ }
      //~ else
      //~ {
        
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                //~ _grappaPhaseEncoding.getStartTime() ), 
                //~ 0, 
                //~ 0,
                //~ 0, 
                //~ &_grappaPhaseEncoding, 
                //~ 0, 
                //~ 0,
                //~ 0 );
              
      //~ }
      
    //~ }
    //~ else
    //~ {
      
      //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
              //~ _phaseEncoding.getStartTime() ), 
              //~ 0, 
              //~ 0,
              //~ 0, 
              //~ &_phaseEncoding, 
              //~ 0, 
              //~ 0,
              //~ 0 );
              
    //~ }
    
    
    //~ if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
    //~ {
      
      //~ if( _lineIndex == _centerLineIndex ||
          //~ _lineIndex == _centerLineIndex - 1 ||
          //~ _lineIndex == _centerLineIndex + 1 )
      //~ {
        
        //~ if( _lineIndex%2 == 0 )
        //~ {
          
          //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                  //~ _readoutDown.getStartTime() ),
                  //~ 0,
                  //~ 0,
                  //~ 0,
                  //~ 0, 
                  //~ &_readoutDown,
                  //~ 0,
                  //~ 0 );
        
        //~ }
        //~ else
        //~ {
          
          //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                  //~ _readoutUp.getStartTime() ),
                  //~ 0,
                  //~ 0,
                  //~ 0,
                  //~ 0, 
                  //~ &_readoutUp,
                  //~ 0,
                  //~ 0 );
         
       //~ }
        
      //~ }
      
      //~ else if( patialFourierUpLine )
      //~ {
        
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                //~ _readoutUp.getStartTime() ),
                //~ 0,
                //~ 0,
                //~ 0,
                //~ 0, 
                //~ &_readoutUp,
                //~ 0,
                //~ 0 );
        
         //~ if( _partitionIndex != mrProt.kSpace().partitions() - 1 && 
             //~ _lineIndex == mrProt.kSpace().phaseEncodingLines() - 1 )
         //~ {
           
           //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0 );
               
          //~ }
      
      //~ }
      //~ else
      //~ {
        
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                //~ _readoutDown.getStartTime() ),
                //~ 0,
                //~ 0,
                //~ 0,
                //~ 0, 
                //~ &_readoutDown,
                //~ 0,
                //~ 0 );
                
        //~ if( _partitionIndex != _partitionCount - 1 && 
            //~ _lineIndex == _lineCount - 1 )
        //~ {
              
          //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
              //~ 0, 
              //~ 0, 
              //~ 0, 
              //~ 0, 
              //~ 0, 
              //~ 0, 
              //~ 0 );
              
        //~ }
      
      //~ }
      
    //~ }
    //~ else
    //~ {
      
      //~ if( _lineIndex%2 == 0 )
      //~ {
        
        //~ _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                //~ _readoutDown.getStartTime() ),
                //~ 0,
                //~ 0,
                //~ 0,
                //~ 0, 
                //~ &_readoutDown,
                //~ 0,
                //~ 0 );
      
      //~ }
      //~ else
      //~ {
        
        //~ _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_REFLECT );
        //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
                //~ _readoutUp.getStartTime() ),
                //~ 0,
                //~ 0,
                //~ 0,
                //~ 0, 
                //~ &_readoutUp,
                //~ 0,
                //~ 0 );
        
         //~ if( _partitionIndex != _partitionCount - 1 && 
             //~ _lineIndex == _lineCount - 1 )
         //~ {
               
           //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0, 
               //~ 0 );
               
          //~ }
        
        
      //~ }
      
    //~ }
    
  
     //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
           //~ adcReadoutStartTime ),
           //~ &_adcOffsets, 
           //~ 0, 
           //~ &_adcReadout, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
    
    //~ fRTEI( fSDSRoundUpGRT( this->getStartTime() +
           //~ adcReadoutEndTime ),
           //~ &_adcResetSynthetizer, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
           
    //~ fRTEI( fSDSRoundUpGRT( this->getEndTime() ),
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0, 
           //~ 0 );
  
  //~ }

  return status;

}


const sGRAD_PULSE_PE& 
gkg::MultiLineEPIReading3dModule::getPhaseEncodingGradientPulse() const
{

  return _phaseEncoding;

}

const sGRAD_PULSE_PE& 
gkg::MultiLineEPIReading3dModule::getGrappaPhaseEncodingGradientPulse() const
{

  return _grappaPhaseEncoding;

}

//~ const sGRAD_PULSE_PE& 
//~ gkg::MultiLineEPIReading3dModule::getPhaseReturnGradientPulse() const
//~ {

  //~ return _phaseReturn;

//~ }

//~ const sGRAD_PULSE_3D& 
//~ gkg::MultiLineEPIReading3dModule::getPartitionEncodingGradientPulse() const
//~ {

  //~ return _partitionEncoding;

//~ }

const sGRAD_PULSE_RO& 
gkg::MultiLineEPIReading3dModule::getReadoutUpGradientPulse() const
{

  return _readoutUp;

}

const sREADOUT& gkg::MultiLineEPIReading3dModule::getAdcReadout() const
{

  return _adcReadout;

}

void gkg::MultiLineEPIReading3dModule::setAdcSpoilPhase( double adcSpoilPhase )
{
  
  _adcSpoilPhase = adcSpoilPhase;
  
}

void 
gkg::MultiLineEPIReading3dModule::setRepetitionIndex( long repetitionIndex )
{

  _repetitionIndex = repetitionIndex;

}


void gkg::MultiLineEPIReading3dModule::setAdcReadoutRelevantForMeasureTime( 
                                                       long currentKernelCalls,
                                                       long lineCountPerSecond )
{

    _adcReadout.setRelevantForMeasTime();
  
}

void gkg::MultiLineEPIReading3dModule::setAdcReadoutMDH( MrProt& mrProt,
                                                         long lineIndex, 
                                                         long sliceIndex,
                                                         long partitionIndex )
{

   _adcReadout.getMDH().setPhaseFT( false );
   _adcReadout.getMDH().addToEvalInfoMask( MDH_OFFLINE );
   _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_PHASCOR );
   _adcReadout.getMDH().setCrep( _repetitionIndex ); 
   _adcReadout.getMDH().setClin( lineIndex );
   _adcReadout.getMDH().setCslc( sliceIndex ); 
   _adcReadout.getMDH().setCpar( partitionIndex ); 
   _adcReadout.getMDH().setCseg( 0 );
   _adcReadout.getMDH().setCacq( 0 );
   _adcReadout.getMDH().setKSpaceCentreLineNo( _centerLineIndex );
   _adcReadout.getMDH().setKSpaceCentrePartitionNo( _centerPartitionIndex );
   _adcReadout.getMDH().setKSpaceCentreColumn( ( unsigned short )(
                                   mrProt.kSpace().getlBaseResolution() / 2 ) );
  
    long adjust = 1;
    
    if( mrProt.PAT().getucPATMode()==SEQ::PAT_MODE_GRAPPA )
    {
    
      adjust = 2;
      if( _grappaFlag == true )
      {
        
        if( ( lineIndex < _centerLineIndex + 10 &&
              lineIndex >= _centerLineIndex - 10 ) )
        {
          
          _adcReadout.getMDH().setPATRefScan( true );
          
        }
        else
        {
          
          _adcReadout.getMDH().setPATRefScan( false );
          
          
        }
        
        if ( lineIndex%2 == 0 )
        {
          
          _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_REFLECT );
          _adcReadout.getMDH().setCseg( 0 );
          
        }
        else
        {
          
          _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
          _adcReadout.getMDH().setCseg( 1 );
          
        } 
        
      }
      else
      {
        
        _adcReadout.getMDH().setPATRefScan( false );
        if ( lineIndex%4 == 0 )
        {
          
          _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_REFLECT );
          _adcReadout.getMDH().setCseg( 0 );
          
        }
        else
        {
          
          _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
          _adcReadout.getMDH().setCseg( 1 );
          
        } 
        
        
      }
      
    }
    else
    {
    
      if ( lineIndex%2 == 0 )
      {
        
        _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_REFLECT );
        _adcReadout.getMDH().setCseg( 0 );
        
      }
      else
      {
        
        _adcReadout.getMDH().addToEvalInfoMask( MDH_REFLECT );
        _adcReadout.getMDH().setCseg( 1 );
        
      } 
      
    }
    
    if ( lineIndex == _startLineIndex ||
         ( _grappaFlag == true && lineIndex == _centerLineIndex - 10 ) )
    {
    
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( false );
      _adcReadout.getMDH().setLastScanInConcat( false );
      _adcReadout.getMDH().setLastScanInMeas( false );
    
    }
    else if ( lineIndex == _lineCount - adjust ||
              ( _grappaFlag == true && 
                lineIndex == _centerLineIndex + 9 ) )
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
      _adcReadout.getMDH().setPhaseFT( false );
      _adcReadout.getMDH().deleteFromEvalInfoMask( MDH_D3FFT );
    
    }
    
    if( _lineIndex == _startLineIndex && 
        _partitionIndex == _startPartitionIndex )
    {
      
      _adcReadout.getMDH().setFirstScanInSlice( true );
      
    }
    else if( _lineIndex == _lineCount - 1 && 
        _partitionIndex == _partitionCount -1 )
    {
      
      _adcReadout.getMDH().setFirstScanInSlice( false );
      _adcReadout.getMDH().setLastScanInSlice( true );
      _adcReadout.getMDH().setLastScanInConcat( true );
      _adcReadout.getMDH().setLastScanInMeas( true );
      
    }
    
  //~ }
    
}

long gkg::MultiLineEPIReading3dModule::getAdcShift()
{
  
  return _adcShift;
  
}

std::string gkg::MultiLineEPIReading3dModule::getStaticName()
{

  return "MultiLineEPIReading3dModule";

}

std::string gkg::MultiLineEPIReading3dModule::getName()
{

  return "MultiLineEPIReading3dModule";

}


//
// adding the MultiLineEPIReading3dModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, MultiLineEPIReading3dModule );
