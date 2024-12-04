#include <Core/SequenceModule.h>


gkg::SequenceModule::SequenceModule()
                    : _minimumRiseTimePerMeter( 0.0 ),
                      _absoluteMaximumGradientAmplitude( 0.0 ),
                      _nominalMaximumGradientAmplitude( 0.0 ),
                      _anchorTime( 0 ),
                      _durationBeforeAnchorTime( 0 ),
                      _durationAfterAnchorTime( 0 ),
                      _totalDuration( 0 ),
                      _lineCount( 0 ),
                      _sliceCount( 0 ),
                      _partitionCount( 0 ),
                      _centerLineIndex( 0 ),
                      _centerPartitionIndex( 0 ),
                      _startLineIndex( 0 ),
                      _startLine( 0 ),
                      _startPartitionIndex( 0 ),
                      _startPartition( 0 )
{
}


gkg::SequenceModule::~SequenceModule()
{
}


NLSStatus gkg::SequenceModule::prepare(  MrProt& mrProt,
                                         SeqLim& /*seqLim*/,
                                         MrProtocolData::SeqExpo& /*seqExpo*/ )
{

  NLS_STATUS status = SEQU__NORMAL;
  
  //////////////////////////////////////////////////////////////////////////////
  // timing management
  //////////////////////////////////////////////////////////////////////////////
  
  double reallySmallestRiseTime = 5.; // [ us / ( mT / m ) ]
  _minimumRiseTimePerMeter = std::max< double >( reallySmallestRiseTime,
                                              SysProperties::getGradMinRiseTime(
                                                   mrProt.gradSpec().mode() ) );
  
  if( mrProt.gradSpec().isGSWDMode() )
  {
    
    _minimumRiseTimePerMeter = mrProt.gradSpec().GSWDMinRiseTime(); 
    
  }
  
  // Absolute
  double reallyBiggestAmplitude = 28.; // [ mT / m ]
  _absoluteMaximumGradientAmplitude = std::min< double >( 
                                            reallyBiggestAmplitude,
                                            0.8 * SysProperties::getGradMaxAmpl(
                                            mrProt.gradSpec().mode() ) );

  // Nominal
  _nominalMaximumGradientAmplitude = SysProperties::getGradMaxAmplNominal();
  
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

  return status;

}


void gkg::SequenceModule::setAnchorTime( long anchorTime )
{

  _anchorTime = anchorTime;

}

long gkg::SequenceModule::getAnchorTime() const
{

  return _anchorTime;

}


long gkg::SequenceModule::getStartTime() const
{

  return fSDSRoundUpGRT( _anchorTime - _durationBeforeAnchorTime );

}

 
long gkg::SequenceModule::getEndTime() const
{

  return _anchorTime + _durationAfterAnchorTime;

}


long gkg::SequenceModule::getDurationBeforeAnchorTime() const
{

  return _durationBeforeAnchorTime;

}


long gkg::SequenceModule::getDurationAfterAnchorTime() const
{

  return _durationAfterAnchorTime;

}


long gkg::SequenceModule::getTotalDuration() const
{

  return _durationBeforeAnchorTime + _durationAfterAnchorTime;

}
