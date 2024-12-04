#ifndef _gkg_pulse_sequence_GinkgoGradientEcho2d_Spoiling2dEPIModule_h_
#define _gkg_pulse_sequence_GinkgoGradientEcho2d_Spoiling2dEPIModule_h_


#include <SpoilingModules/SpoilingModule.h>
#include <ExcitationModules/SingleTxExcitation2dModule.h>
#include <PreEncodingModules/EPI2dPreEncodingModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class Spoiling2dEPIModule : public SpoilingModule,
                         public Creator3Arg< Spoiling2dEPIModule,
                                             SequenceModule,
                                             const std::vector< double >&,
                                             const std::vector< std::string >&,
                                             const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~Spoiling2dEPIModule();

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

    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< Spoiling2dEPIModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                  std::string,
                                  RCPointer< SequenceModule > >& >;

    Spoiling2dEPIModule(
                   const std::vector< double >& /* doubleParameters */,
                   const std::vector< std::string >& /* stringParameters */,
                   const std::map< std::string, 
                   RCPointer< SequenceModule > >& otherSequenceModules );

    // RF-signal axis
    RCPointer< SingleTxExcitation2dModule > _singleTxExcitation2dModule;

    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelectionSpoil;

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseRewinding;
    sGRAD_PULSE_PE _grappaPhaseRewinding;

    // Read gradient axis (X)
    sGRAD_PULSE_RO _readoutPresetSpoil;
    RCPointer< EPI2dPreEncodingModule > _EPI2dPreEncodingModule;

    // ADC signal data axis

    // Numeric Crystal Oscillator axis

};


}


#endif

