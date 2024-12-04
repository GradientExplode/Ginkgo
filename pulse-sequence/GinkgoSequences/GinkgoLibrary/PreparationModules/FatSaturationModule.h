#ifndef _gkg_pulse_sequence_GinkgoLibrary_FatSaturationModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_FatSaturationModule_h_


#include <Core/SequenceModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>
#include <fstream>

namespace gkg
{


class FatSaturationModule : public SequenceModule,
                  public Creator3Arg< FatSaturationModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~FatSaturationModule();

    void initialize( SeqLim& seqLim );

    NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );

    NLS_STATUS modifyKernel( MrProt& mrProt,
                             SeqLim& seqLim,
                             MrProtocolData::SeqExpo& seqExpo,
                             const sSLICE_POS* slices,
                             long sliceIndex,
                             long partition,
                             long lineIndex );

    NLS_STATUS setKernelTiming( MrProt& mrProt );

    const sRF_PULSE_SINC& getRfFatSaturationWaveformEnveloppe() const;
    const long& getFatSaturationIterations() const;
    void setFatSaturationFlipAngle( double flipAngle );
    void setFatSaturationFrequencyOffset( double frequencyOffset );
    void setFatSaturationNumberOfIterations( long iterations );

    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< FatSaturationModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

    FatSaturationModule( 
                    const std::vector< double >& /* doubleParameters */,
                    const std::vector< std::string >& /* stringParameters */,
                    const std::map< std::string, 
                    RCPointer< SequenceModule > >& /* otherSequenceModules */ );
  
    long _iterations;
    double _unitDuration;
    long _rfDuration;
    double _frequencyOffset;
    double _fatSaturationFlipAngle;
    
    // RF-signal axis
    //~ sRF_PULSE_EXT _rfFatSaturationWaveformEnveloppe;
    sRF_PULSE_SINC _rfFatSaturationWaveformEnveloppe;
    double _rfSpoilIncrement;
    double _rfSpoilPhase;

    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelectionFatSaturationSpoil1;
    sGRAD_PULSE _sliceSelectionFatSaturationSpoil2;
    sGRAD_PULSE _sliceSelectionFatSaturationCrusher;

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseFatSaturationSpoil1;
    sGRAD_PULSE_PE _phaseFatSaturationSpoil2;
    sGRAD_PULSE_PE _phaseFatSaturationCrusher;

    // Read gradient axis (X)
    sGRAD_PULSE _readoutFatSaturationSpoil1;
    sGRAD_PULSE _readoutFatSaturationSpoil2;

    // ADC signal data axis

    // Numeric Crystal Oscillator axis
    sFREQ_PHASE _rfFatSaturationOffsets;
    sFREQ_PHASE _rfFatSaturationResetSynthetizer;

};


}


#endif

