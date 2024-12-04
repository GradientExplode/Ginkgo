#ifndef _gkg_pulse_sequence_GinkgoLibrary_SingleLineReading3dModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SingleLineReading3dModule_h_


#include <ReadingModules/ReadingModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class SingleLineReading3dModule :
                  public ReadingModule,
                  public Creator3Arg< SingleLineReading3dModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~SingleLineReading3dModule();

    void initialize( SeqLim& seqLim );

    NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );

    NLS_STATUS modifyKernel( MrProt& mrProt,
                             SeqLim& seqLim,
                             MrProtocolData::SeqExpo& seqExpo,
                             const sSLICE_POS* slices,
                             long sliceIndex,
                             long partitionIndex,
                             long lineIndex );

    NLS_STATUS setKernelTiming( MrProt& mrProt );

    const sGRAD_PULSE_PE& getPhaseEncodingGradientPulse() const;
    void setAdcSpoilPhase( double adcSpoilPhase );
    
    void setRepetitionIndex( long repetitionIndex );
    void setAdcReadoutMDH( MrProt& mrProt,
                           int32_t lineIndex,
                           int32_t partitionIndex );

    void setAdcReadoutRelevantForMeasureTime( 
                                         int32_t currentKernelCalls,
                                         int32_t kernelCallsPerRelevantSignal );

    long getTotalPhaseEncodingDuration() const;
    const sGRAD_PULSE_RO& getReadout() const;
    const sREADOUT& getAdcReadout() const;
    
    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< SingleLineReading3dModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

    SingleLineReading3dModule(
                  const std::vector< double >& /* doubleParameters */,
                  const std::vector< std::string >& /* stringParameters */,
                  const std::map< std::string, RCPointer< SequenceModule > >&
                                                         otherSequenceModules );
    
    long _repetitionIndex;
    
    // RF-signal axis
    double _adcSpoilPhase;

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseEncoding;

    // Read gradient axis (X)
    sGRAD_PULSE_RO _readout;
    sGRAD_PULSE _readoutPreset;

    // ADC signal data axis
    sREADOUT _adcReadout;
    sFREQ_PHASE _adcOffsets;
    sFREQ_PHASE _adcResetSynthetizer;

    // Numeric Crystal Oscillator axis

};


}


#endif
