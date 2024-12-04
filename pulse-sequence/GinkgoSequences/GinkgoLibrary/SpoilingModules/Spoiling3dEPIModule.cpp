#include <SpoilingModules/Spoiling3dEPIModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::Spoiling3dEPIModule::Spoiling3dEPIModule( 
              const std::vector< double >& /* doubleParameters */,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string,
                              gkg::RCPointer< gkg::SequenceModule > >&
                                                          otherSequenceModules )
                      : gkg::SpoilingModule(),
              
                        // RF-signal axis
                        _singleTxExcitation3dModule( otherSequenceModules.find(
                                              stringParameters[ 0 ] )->second ),
                        
                        // Slice gradient axis (Z)
                        _sliceSelectionSpoil( "sliceSelectionSpoil" ),
                        _partitionRewinding( "partitionRewinding" ),

                        // Phase gradient axis (Y)
                        _phaseRewinding( "phaseRewinding" ),

                        // Read gradient axis (X)
                        _readoutPresetSpoil( "readoutPresetSpoil" ),
                        _EPI3dPreEncodingModule( otherSequenceModules.find(
                                               stringParameters[ 1 ] )->second )

                        // ADC signal data axis

                        // Numeric Crystal Oscillator axis
{

}


gkg::Spoiling3dEPIModule::~Spoiling3dEPIModule()
{
}


void gkg::Spoiling3dEPIModule::initialize( SeqLim& seqLim )
{

  // nothing to do here

}


NLSStatus gkg::Spoiling3dEPIModule::prepare( MrProt& mrProt,
                                          SeqLim& seqLim,
                                          MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::Spoiling3dEPIModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );
  
  //~ //////////////////////////////////////////////////////////////////////////
  //~ // creating a phase rewinding gradient
  //~ //////////////////////////////////////////////////////////////////////////
  
  //~ // null moment version
  //~ // _phaseRewinding = _EPI3dPreEncodingModule->
                                          //~ // getPhasePresetGradientPulse();
  
  //~ // killer version
  //~ double voxelResolution = ( mrProt.sliceSeries().front().readoutFOV() /
                             //~ mrProt.kSpace().baseResolution() ) * 0.001;//m
  //~ double lapsNumber = 4.0;
  //~ double gamma = 42.57; // MHz/T
  //~ double phaseKillerMoment = ( 1000 * lapsNumber ) / 
                             //~ ( gamma * voxelResolution );
  
  //~ _phaseRewinding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  //~ _phaseRewinding.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  //~ if ( !_phaseRewinding.prepSymmetricTOTShortestTime( phaseKillerMoment ) )
  //~ {
    
    //~ return _phaseRewinding.getNLSStatus();
      
  //~ }
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a phase rewinding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseRewinding = _EPI3dPreEncodingModule->getPhasePresetGradientPulse();
  _grappaPhaseRewinding = _EPI3dPreEncodingModule->
                                            getGrappaPhasePresetGradientPulse();
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a readout spoiler
  //////////////////////////////////////////////////////////////////////////////
  
  _readoutPresetSpoil = _EPI3dPreEncodingModule->
                                                getReadoutPresetGradientPulse();
  _readoutPresetSpoil.setAmplitude( - _readoutPresetSpoil.getAmplitude() );

  
  //////////////////////////////////////////////////////////////////////////////
  // creating a partition rewinding gradient
  //////////////////////////////////////////////////////////////////////////////
  
  // null moment version
  _partitionRewinding = _EPI3dPreEncodingModule->
                                      getPartitionPreEncodingGradientPulse();
  _partitionRewinding.setAmplitude( - _partitionRewinding.getAmplitude() );
  
  // killer version
  //~ double partitionKillerMoment = ( 1000 * lapsNumber ) / 
                                 //~ ( gamma * voxelResolution );
  //~ _partitionRewinding.setMaxMagnitude( _absoluteMaximumGradientAmplitude );
  //~ _partitionRewinding.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  //~ if( !_partitionRewinding.prepSymmetricTOTShortestTime( 
                                                  //~ partitionKillerMoment ) )
  //~ {
    
    //~ return _phaseRewinding.getNLSStatus();
      
  //~ }
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside this module
  //////////////////////////////////////////////////////////////////////////////

  _phaseRewinding.setStartTime( 0 );
  _partitionRewinding.setStartTime( 0 );
  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;

  _durationAfterAnchorTime = std::max( std::max( 
                                       _partitionRewinding.getDuration() +
                                       _partitionRewinding.getRampDownTime(),
                                       _readoutPresetSpoil.getDuration() +
                                       _readoutPresetSpoil.getRampDownTime() ),
                                       _phaseRewinding.getDuration() +
                                       _phaseRewinding.getRampDownTime() );

  return status;

}


NLS_STATUS gkg::Spoiling3dEPIModule::modifyKernel(
                                              MrProt& mrProt,
                                              SeqLim& seqLim,
                                              MrProtocolData::SeqExpo& seqExpo,
                                              const sSLICE_POS* slices,
                                              long sliceIndex,
                                              long partitionIndex,
                                              long lineIndex )
{

  NLS_STATUS status  = SEQU__NORMAL;
  
  // null moment version
  // if( !_partitionRewinding.prepPE( mrProt,
                                  // partitionIndex - _centerPartitionIndex ) )
  // {

    // return _partitionRewinding.getNLSStatus();

  // }
  // _partitionRewinding.setAmplitude( - _partitionRewinding.getAmplitude() );
 
  return status;

}


NLS_STATUS gkg::Spoiling3dEPIModule::setKernelTiming( MrProt& mrProt )
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
           _readoutPresetSpoil.getStartTime(),
           0, 
           0, 
           0,
           &_grappaPhaseRewinding, 
           &_readoutPresetSpoil, //0, 
           &_partitionRewinding, 
           0 );
           
           
  }
  else
  {
    
    fRTEI( this->getStartTime() +
           _readoutPresetSpoil.getStartTime(),
           0, 
           0, 
           0,
           &_phaseRewinding, 
           &_readoutPresetSpoil, //0, 
           &_partitionRewinding, 
           0 );
           
           
  }
  
  return status;

}

long gkg::Spoiling3dEPIModule::getTotalPartitionRewindingDuration() const
{
  
  return _partitionRewinding.getTotalTime();

}

long gkg::Spoiling3dEPIModule::getTimeAfterAdc() const
{
  
  long timeAfterADC =  std::max( _phaseRewinding.getTotalTime(),
                                 _partitionRewinding.getTotalTime());

  timeAfterADC = std::max( timeAfterADC , 
                           static_cast<long>(SysProperties::getCoilCtrlHold()));
  
  return timeAfterADC;

}
 
std::string gkg::Spoiling3dEPIModule::getStaticName()
{

  return "Spoiling3dEPIModule";

}

//
// adding the Spoiling3dEPIModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, Spoiling3dEPIModule );
