#ifndef _gkg_pulse_sequence_GinkgoLibrary_SimpleRefocusingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SimpleRefocusingModule_h_


#include <RefocusingModules/RefocusingModule.h>
#include <ExcitationModules/ExcitationModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>

namespace gkg
{

class SimpleRefocusingModule :
                  public RefocusingModule,
                  public Creator3Arg< SimpleRefocusingModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~SimpleRefocusingModule();

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
    
    const sRF_PULSE_SINC& getRfRefocusingWaveformEnveloppe() const;
    const sGRAD_PULSE& getSliceSelectionRefocusingGradientPulse() const;
    
    void setRfRefocusingSpoilPhase( double rfRefocusingSpoilPhase );

    static std::string getStaticName();

  protected:
    
    SimpleRefocusingModule(
                  const std::vector< double >& /* doubleParameters */,
                  const std::vector< std::string >& /* stringParameters */,
                  const std::map< std::string, 
                  RCPointer< SequenceModule > >& otherSequenceModules );

    friend struct Creator3Arg< SimpleRefocusingModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

                                                   
    // RF-signal axis
    sRF_PULSE_SINC _rfRefocusingWaveformEnveloppe;
    double _rfRefocusingSpoilIncrement;
    double _rfRefocusingSpoilPhase;
    double _rfSpoilIncrement;
    double _rfSpoilPhase;


    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelectionCrusher;
    sGRAD_PULSE _sliceSelectionRefocusing;

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseCrusher;

    // Read gradient axis (X)
    sGRAD_PULSE _readoutCrusher;
    
    // ADC signal data axis

    // Numeric Crystal Oscillator axis
    sFREQ_PHASE _rfRefocusingOffsets;
    sFREQ_PHASE _rfRefocusingResetSynthetizer;

};


}


#endif

