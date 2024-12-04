#include <SpoilingModules/Spoiling2dEPIModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::Spoiling2dEPIModule::Spoiling2dEPIModule( 
              const std::vector< double >& /* doubleParameters */,
              const std::vector< std::string >& stringParameters,
              const std::map< std::string,
                              gkg::RCPointer< gkg::SequenceModule > >&
                                                          otherSequenceModules )
                      : gkg::SpoilingModule(),
              
                        // RF-signal axis
                        _singleTxExcitation2dModule( otherSequenceModules.find(
                                              stringParameters[ 0 ] )->second ),
                        
                        // Slice gradient axis (Z)
                        _sliceSelectionSpoil( "sliceSelectionSpoil" ),

                        // Phase gradient axis (Y)
                        _phaseRewinding( "phaseRewinding" ),

                        // Read gradient axis (X)
                        _readoutPresetSpoil( "readoutPresetSpoil" ),
                        _EPI2dPreEncodingModule( otherSequenceModules.find(
                                               stringParameters[ 1 ] )->second )

                        // ADC signal data axis

                        // Numeric Crystal Oscillator axis
{

}


gkg::Spoiling2dEPIModule::~Spoiling2dEPIModule()
{
}


void gkg::Spoiling2dEPIModule::initialize( SeqLim& seqLim )
{

  // nothing to do here

}


NLSStatus gkg::Spoiling2dEPIModule::prepare( MrProt& mrProt,
                                          SeqLim& seqLim,
                                          MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { "gkg::Spoiling2dEPIModule::prepare" };

  NLS_STATUS status = SEQU__NORMAL;

  //////////////////////////////////////////////////////////////////////////////
  // calling the method of the mother class to initialize maximum gradient
  // magnitude and minimum rise time
  //////////////////////////////////////////////////////////////////////////////

  this->gkg::SequenceModule::prepare( mrProt, 
                                      seqLim, 
                                      seqExpo );


  //////////////////////////////////////////////////////////////////////////////
  // creating a slice selection spoiling gradient
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelectionSpoil.setMaxMagnitude(
                                    _absoluteMaximumGradientAmplitude );
  _sliceSelectionSpoil.setMinRiseTime ( _minimumRiseTimePerMeter );
  
  if( !_sliceSelectionSpoil.prepSymmetricTOTShortestTime(
           _singleTxExcitation2dModule->getSliceSelectionGradientPulse().
             getMomentum( 0, _singleTxExcitation2dModule-> 
                                getSliceSelectionGradientPulse().getDuration() - 
                             _singleTxExcitation2dModule-> 
                                getRfWaveformEnveloppe().getDuration() / 2 ) ) )
  {

    return _sliceSelectionSpoil.getNLSStatus();

  }


  //////////////////////////////////////////////////////////////////////////////
  // creating a phase rewinding gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseRewinding = _EPI2dPreEncodingModule->getPhasePresetGradientPulse();
  _grappaPhaseRewinding = _EPI2dPreEncodingModule->
                                            getGrappaPhasePresetGradientPulse();
  
  //////////////////////////////////////////////////////////////////////////////
  // creating a readout spoiler
  //////////////////////////////////////////////////////////////////////////////
  
  _readoutPresetSpoil = _EPI2dPreEncodingModule->
                                                getReadoutPresetGradientPulse();
  _readoutPresetSpoil.setAmplitude( - _readoutPresetSpoil.getAmplitude() );


  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside this module
  //////////////////////////////////////////////////////////////////////////////

  _sliceSelectionSpoil.setStartTime( 0 );
  _readoutPresetSpoil.setStartTime( 0 );
  _phaseRewinding.setStartTime( 0 );
  _anchorTime = 0;
  _durationBeforeAnchorTime = 0;
  _durationAfterAnchorTime = std::max( std::max( 
                                       _sliceSelectionSpoil.getDuration() +
                                       _sliceSelectionSpoil.getRampDownTime(),
                                       _readoutPresetSpoil.getDuration() +
                                       _readoutPresetSpoil.getRampDownTime() ),
                                       _phaseRewinding.getDuration() +
                                       _phaseRewinding.getRampDownTime() );

  return status;

}


NLS_STATUS gkg::Spoiling2dEPIModule::modifyKernel(
                                              MrProt& mrProt,
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


NLS_STATUS gkg::Spoiling2dEPIModule::setKernelTiming( MrProt& mrProt )
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
           &_readoutPresetSpoil,
           &_sliceSelectionSpoil, 
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
           &_readoutPresetSpoil,
           &_sliceSelectionSpoil, 
           0 );
           
           
  }

  return status;

}


std::string gkg::Spoiling2dEPIModule::getStaticName()
{

  return "Spoiling2dEPIModule";

}


//
// adding the Spoiling2dEPIModule creator to the sequence module factory
//

RegisterSequenceModuleCreator( gkg, Spoiling2dEPIModule );
