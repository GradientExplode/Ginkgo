#include <PreparationModules/TrapezoidDiffusionGradientModule.h>
#include <Core/SequenceModuleFactory.h>


gkg::TrapezoidDiffusionGradientModule::TrapezoidDiffusionGradientModule( 
           const std::vector< double >& doubleParameters,
           const std::vector< std::string >& stringParameters,
           const std::map< std::string,
           gkg::RCPointer< gkg::SequenceModule > >& /* otherSequenceModules */ )
                        : gkg::DiffusionGradientModule(),           
                          //~ _diffusionIndex( 0 ),
                          _diffusionStartTimesDifference( 20000 ),
                          _diffusionPlateauDuration( 4000 ),
                          _sliceSelectionDiffusionAmplitude( 15.0 ),
                          _phaseDiffusionAmplitude( 15.0 ),
                          _readoutDiffusionAmplitude( 15.0 ),
                        
                          // RF-signal axis
                 
                          // Slice gradient axis (Z)
                          _sliceSelectionDiffusion( "sliceSelectionDiffusion" ),
                             
                          // Phase gradient axis (Y)
                          _phaseDiffusion( "phaseDiffusion" ),
                             
                          // Read gradient axis (X)
                          _readoutDiffusion( "readoutDiffusion" )
                             
                          // ADC signal data axis
                             
                          // Numeric Crystal Oscillator axis
{
  
  //////////////////////////////////////////////////////////////////////////////
  // retrieving parameters from file
  //////////////////////////////////////////////////////////////////////////////                                   
  
  INIAccess diffusionParametersFile;
  char errorString[ MAX_INIACCESS_ERROR ];
  float *diffusionParameters;
  diffusionParameters = new float[3 * 6];
  int bigDelta;
  int smallDelta;
  
  char fileName [300];
  #ifdef WIN32
    const char fname[ 26 ] = "\\diffusion_parameters.ini";
  #else
    const char fname[ 26 ] = "/diffusion_parameters.ini";
  #endif
  char *envCustomerSeq = getenv("CustomerSeq");
  strcat(strcpy(fileName , envCustomerSeq), fname );
  diffusionParametersFile.fOpen( fileName, errorString);
  diffusionParametersFile.fGetInt( "Timings",
                                   "bigDelta",
                                   &bigDelta,
                                   errorString );
  diffusionParametersFile.fGetInt( "Timings",
                                   "smallDelta",
                                   &smallDelta,
                                   errorString );
   
  
  diffusionParametersFile.fGetFloatArray( "GradientAmplitudes",
                                          "amplitudes",
                                          0,
                                          3,
                                          6,
                                          diffusionParameters,
                                          errorString );
  diffusionParametersFile.fClose();
  
  //////////////////////////////////////////////////////////////////////////////
  // setting the diffusion parameters
  //////////////////////////////////////////////////////////////////////////////
  
  //~ _diffusionStartTimesDifference = long( _parameters[ 0 ] ); // big delta
  //~ _diffusionPlateauDuration = long(  _parameters[ 1 ] ); // small delta
  //~ _sliceSelectionDiffusionAmplitude = _parameters[ 2 ]; // Gz
  //~ _phaseDiffusionAmplitude = _parameters[ 3 ]; // Gy
  //~ _readoutDiffusionAmplitude = _parameters[ 4 ]; // Gx
  _diffusionStartTimesDifference = bigDelta; // big delta 13440
  _diffusionPlateauDuration = smallDelta; // small delta 3440
  _sliceSelectionDiffusionAmplitude = diffusionParameters[ 2 ]; // Gz 21.917
  _phaseDiffusionAmplitude = diffusionParameters[ 1 ]; // Gy 0
  _readoutDiffusionAmplitude = diffusionParameters[ 0 ]; // Gx 21.917
  //~ _diffusionStartTimesDifference = 13440; // big delta 13440
  //~ _diffusionPlateauDuration = 3440; // small delta 3440
  //~ _sliceSelectionDiffusionAmplitude = 21.917; // Gz 21.917
  //~ _phaseDiffusionAmplitude = 0; // Gy 0
  //~ _readoutDiffusionAmplitude = 21.917; // Gx 21.917
  
  
}

gkg::TrapezoidDiffusionGradientModule::~TrapezoidDiffusionGradientModule()
{
}


void gkg::TrapezoidDiffusionGradientModule::initialize( SeqLim& seqLim )
{ 
}


NLSStatus gkg::TrapezoidDiffusionGradientModule::prepare( MrProt& mrProt,
                                        SeqLim& seqLim,
                                        MrProtocolData::SeqExpo& seqExpo )
{

  static const char *ptModule = { 
                             "gkg::TrapezoidDiffusionGradientModule::prepare" };
  
  NLS_STATUS status = SEQU__NORMAL;
  
  this->gkg::DiffusionGradientModule::prepare( mrProt, 
                                               seqLim, 
                                               seqExpo );
                                              
  //////////////////////////////////////////////////////////////////////////////
  // harmonizing duration
  //////////////////////////////////////////////////////////////////////////////
  
  double diffusionGradientMaxAmplitude = std::max< double >( 
                                         std::max< double >( 
                                         _sliceSelectionDiffusionAmplitude,
                                         _phaseDiffusionAmplitude ),
                                         _readoutDiffusionAmplitude );
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing slice selection diffusion gradient  
  //////////////////////////////////////////////////////////////////////////////
  
  _sliceSelectionDiffusion.setAmplitude( _sliceSelectionDiffusionAmplitude );;
  _sliceSelectionDiffusion.setRampTimes( fSDSRoundUpGRT( 
        std::max< long >( ( long )( _minimumRiseTimePerMeter *
                                    diffusionGradientMaxAmplitude ),
                                    SysProperties::getCoilCtrlLead() ) ) );
  
  _sliceSelectionDiffusion.setDuration( fSDSRoundUpGRT(
                                   _diffusionPlateauDuration +
                                   _sliceSelectionDiffusion.getRampUpTime() ) );
                                   
   if ( !_sliceSelectionDiffusion.prep() )  
  {
  
    return _sliceSelectionDiffusion.getNLSStatus();
    
  }
  
  if ( !_sliceSelectionDiffusion.check() ) 
  {
  
    return _sliceSelectionDiffusion.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing phase diffusion gradient
  //////////////////////////////////////////////////////////////////////////////

  _phaseDiffusion.setAmplitude( _phaseDiffusionAmplitude );
  _phaseDiffusion.setRampTimes( fSDSRoundUpGRT( 
                          std::max< long >( ( long )( _minimumRiseTimePerMeter *
                          diffusionGradientMaxAmplitude ),
                          SysProperties::getCoilCtrlLead() ) ) );              

  _phaseDiffusion.setDuration( fSDSRoundUpGRT(
                               _diffusionPlateauDuration +
                               _phaseDiffusion.getRampUpTime() ) );
                               
  if ( !_phaseDiffusion.prep() )  
  {
  
    return _phaseDiffusion.getNLSStatus();
    
  }
  
  if ( !_phaseDiffusion.check() ) 
  {
  
    return _phaseDiffusion.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing readout diffusion gradient
  //////////////////////////////////////////////////////////////////////////////

  _readoutDiffusion.setAmplitude( _readoutDiffusionAmplitude );
  _readoutDiffusion.setRampTimes( fSDSRoundUpGRT( 
        std::max< long >( ( long )( _minimumRiseTimePerMeter *
                                    diffusionGradientMaxAmplitude ),
                                    SysProperties::getCoilCtrlLead() ) ) );
  
  _readoutDiffusion.setDuration( fSDSRoundUpGRT(
                                 _diffusionPlateauDuration +
                                 _readoutDiffusion.getRampUpTime() ) );
  
  if ( !_readoutDiffusion.prep() )  
  {
  
    return _readoutDiffusion.getNLSStatus();
    
  }
  
  if ( !_readoutDiffusion.check() ) 
  {
  
    return _readoutDiffusion.getNLSStatus();
    
  }
  
  //////////////////////////////////////////////////////////////////////////////
  // preparing timing inside the module // TEMPORARY !!! redefine rampTimes
  //////////////////////////////////////////////////////////////////////////////
  
  long maxRampDuration = std::max( std::max( 
                                   _phaseDiffusion.getRampDownTime(),
                                   _sliceSelectionDiffusion.getRampDownTime() ),
                                   _readoutDiffusion.getRampDownTime() );  
                                      
  _separationTime = _diffusionStartTimesDifference - 
                    _diffusionPlateauDuration -
                    maxRampDuration;
  
  _phaseDiffusion.setStartTime( 0 ); // for the first diffusion gradients
  _readoutDiffusion.setStartTime( _diffusionStartTimesDifference +
                                  maxRampDuration ); // for the second diffusion 
                                                     // gradients
  
  _anchorTime = 2 * maxRampDuration +
                _diffusionPlateauDuration +
                ( _separationTime / 2 );
  _durationBeforeAnchorTime = _anchorTime;
  _durationAfterAnchorTime = 2 * maxRampDuration +
                             _diffusionPlateauDuration +
                             ( _separationTime / 2 );
  
  return status;


}


NLS_STATUS gkg::TrapezoidDiffusionGradientModule::modifyKernel( MrProt& mrProt,
                                               SeqLim& seqLim,
                                               MrProtocolData::SeqExpo& seqExpo,
                                               const sSLICE_POS* slices,
                                               long sliceIndex,
                                               long partition,
                                               long lineIndex )
{


  NLS_STATUS status  = SEQU__NORMAL;
 
  _sliceSelectionDiffusion.setAmplitude( _sliceSelectionDiffusionAmplitude );
  _phaseDiffusion.setAmplitude( _phaseDiffusionAmplitude );
  _readoutDiffusion.setAmplitude( _readoutDiffusionAmplitude );
  
  return status;


}


NLS_STATUS gkg::TrapezoidDiffusionGradientModule::setKernelTiming( MrProt& mrProt )
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
  
  fRTEI( this->getStartTime() +
         _phaseDiffusion.getStartTime(),
         0,
         0,
         0,
         &_phaseDiffusion,
         &_readoutDiffusion,
         &_sliceSelectionDiffusion,
         0 );

  fRTEI( this->getStartTime() +
         _readoutDiffusion.getStartTime(),
         0,
         0,
         0,
         &_phaseDiffusion,
         &_readoutDiffusion,
         &_sliceSelectionDiffusion,
         0 );
  
  return status;


}

const long& gkg::TrapezoidDiffusionGradientModule::getSeparationTime() const
{
  
  return _separationTime;
  
}

long gkg::TrapezoidDiffusionGradientModule::getFirstGradientEndTime()
{
  
  return _anchorTime - 
         _durationBeforeAnchorTime + 
         _phaseDiffusion.getStartTime() +
         _phaseDiffusion.getDuration() +
         _phaseDiffusion.getRampDownTime();
  
}

std::string gkg::TrapezoidDiffusionGradientModule::getStaticName()
{

  return "TrapezoidDiffusionGradientModule";

}

//~ void gkg::TrapezoidDiffusionGradientModule::setDiffusionIndex( 
                                                       //~ long diffusionIndex )
//~ {
  
  //~ _diffusionIndex = diffusionIndex;
  
//~ }

void gkg::TrapezoidDiffusionGradientModule::setParameters( 
                                        long diffusionStartTimesDifference,
                                        long diffusionPlateauDuration,
                                        double sliceSelectionDiffusionAmplitude,
                                        double phaseDiffusionAmplitude,
                                        double readoutDiffusionAmplitude )
{
  
  _diffusionStartTimesDifference = diffusionStartTimesDifference; // big delta
  _diffusionPlateauDuration = diffusionPlateauDuration; // small delta
  _sliceSelectionDiffusionAmplitude = sliceSelectionDiffusionAmplitude; // Gz
  _phaseDiffusionAmplitude = phaseDiffusionAmplitude; // Gy
  _readoutDiffusionAmplitude = readoutDiffusionAmplitude; // Gx
  
}

//
// adding the TrapezoidDiffusionGradientModule creator to the sequence module 
// factory
//

RegisterSequenceModuleCreator( gkg, 
                               TrapezoidDiffusionGradientModule );
