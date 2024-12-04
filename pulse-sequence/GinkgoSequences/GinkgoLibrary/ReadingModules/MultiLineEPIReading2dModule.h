#ifndef _gkg_pulse_sequence_GinkgoLibrary_MultiLineEPIReading2dModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_MultiLineEPIReading2dModule_h_


#include <ReadingModules/ReadingModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class MultiLineEPIReading2dModule :
                  public ReadingModule,
                  public Creator3Arg< MultiLineEPIReading2dModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~MultiLineEPIReading2dModule();

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

    const sGRAD_PULSE_PE& getPhaseEncodingGradientPulse( MrProt& mrProt ) const;
    void setRepetitionIndex( long repetitionIndex );
    const sGRAD_PULSE_PE& getGrappaPhaseEncodingGradientPulse() const;
    const sGRAD_PULSE_RO& getReadoutUpGradientPulse() const;
    const sREADOUT& getAdcReadout() const;
    long getAdcShift();
    void setAdcSpoilPhase( double adcSpoilPhase );
    void setAdcReadoutMDH( MrProt& mrProt,
                           long lineIndex,
                           long sliceIndex,
                           long partitionIndex );
    void setAdcReadoutRelevantForMeasureTime( long currentKernelCalls,
                                              long lineCountPerSecond );

    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< MultiLineEPIReading2dModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

    MultiLineEPIReading2dModule(
                  const std::vector< double >& /* doubleParameters */,
                  const std::vector< std::string >& /* stringParameters */,
                  const std::map< std::string, 
                  RCPointer< SequenceModule > >& otherSequenceModules );
    long _lineIndex;
    long _sliceIndex;
    long _adcShift;
    long _repetitionIndex;
    
    // RF-signal axis
    double _adcSpoilPhase;

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseEncoding;
    sGRAD_PULSE_PE _grappaPhaseEncoding;

    // Read gradient axis (X)
    sGRAD_PULSE_RO _readoutUp;
    sGRAD_PULSE_RO _readoutDown;

    // ADC signal data axis
    sREADOUT _adcReadout;
    sFREQ_PHASE _adcOffsets;
    sFREQ_PHASE _adcResetSynthetizer;

    // Numeric Crystal Oscillator axis

};


}


#endif
