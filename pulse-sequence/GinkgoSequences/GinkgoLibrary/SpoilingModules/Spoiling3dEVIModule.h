#ifndef _gkg_pulse_sequence_GinkgoSpinEcho3d_Spoiling3dEVIModule_h_
#define _gkg_pulse_sequence_GinkgoSpinEcho3d_Spoiling3dEVIModule_h_


#include <SpoilingModules/SpoilingModule.h>
#include <ExcitationModules/SingleTxExcitation3dModule.h>
#include <PreEncodingModules/EPI3dPreEncodingModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class Spoiling3dEVIModule : public SpoilingModule,
                         public Creator3Arg< Spoiling3dEVIModule,
                                             SequenceModule,
                                             const std::vector< double >&,
                                             const std::vector< std::string >&,
                                             const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~Spoiling3dEVIModule();

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

    long getTotalPartitionRewindingDuration() const;
    long getTimeAfterAdc() const;
    
    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< Spoiling3dEVIModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                  std::string,
                                  RCPointer< SequenceModule > >& >;

    Spoiling3dEVIModule(
                   const std::vector< double >& /* doubleParameters */,
                   const std::vector< std::string >& /* stringParameters */,
                   const std::map< std::string, RCPointer< SequenceModule > >&
                                                         otherSequenceModules );

    // RF-signal axis
    RCPointer< SingleTxExcitation3dModule > _singleTxExcitation3dModule;

    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelectionSpoil;
    sGRAD_PULSE_3D _partitionRewinding;
    //~ sGRAD_PULSE_PE _partitionRewinding;

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseRewinding;
    sGRAD_PULSE_PE _grappaPhaseRewinding;

    // Read gradient axis (X)
    sGRAD_PULSE_RO _readoutPresetSpoil;
    RCPointer< EPI3dPreEncodingModule > _EPI3dPreEncodingModule;

    // ADC signal data axis

    // Numeric Crystal Oscillator axis

};


}


#endif

