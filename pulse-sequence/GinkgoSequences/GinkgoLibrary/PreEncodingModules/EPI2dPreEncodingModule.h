#ifndef _gkg_pulse_sequence_GinkgoLibrary_EPI2dPreEncodingModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_EPI2dPreEncodingModule_h_


//~ #include <ReadingModules/ReadingModule.h>
#include <Core/SiemensInclude.h>
#include <Core/SequenceModule.h>
#include <PreEncodingModules/PreEncodingModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class EPI2dPreEncodingModule :
                  public PreEncodingModule,
                  public Creator3Arg< EPI2dPreEncodingModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~EPI2dPreEncodingModule();

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
    
    const sGRAD_PULSE_RO& getReadoutPresetGradientPulse() const;
    const sGRAD_PULSE_PE& getPhasePresetGradientPulse() const;
    const sGRAD_PULSE_PE& getGrappaPhasePresetGradientPulse() const;
    
    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< EPI2dPreEncodingModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

    EPI2dPreEncodingModule(
                  const std::vector< double >& /* doubleParameters */,
                  const std::vector< std::string >& /* stringParameters */,
                  const std::map< std::string, RCPointer< SequenceModule > >&
                                                         otherSequenceModules );
    // RF-signal axis

    // Slice gradient axis (Z)

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phasePreEncoding;
    sGRAD_PULSE_PE _grappaPhasePreEncoding;

    // Read gradient axis (X)
    sGRAD_PULSE_RO _readoutPreEncoding;

    // ADC signal data axis
    sREADOUT _adcReadout;
    
    // Numeric Crystal Oscillator axis

};


}


#endif
