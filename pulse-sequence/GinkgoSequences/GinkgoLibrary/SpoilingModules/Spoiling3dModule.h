#ifndef _gkg_pulse_sequence_GinkgoGradientEcho3d_Spoiling3dModule_h_
#define _gkg_pulse_sequence_GinkgoGradientEcho3d_Spoiling3dModule_h_


#include <SpoilingModules/SpoilingModule.h>
#include <ExcitationModules/SingleTxExcitation3dModule.h>
#include <ReadingModules/SingleLineReading3dModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class Spoiling3dModule : public SpoilingModule,
                         public Creator3Arg< Spoiling3dModule,
                                             SequenceModule,
                                             const std::vector< double >&,
                                             const std::vector< std::string >&,
                                             const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~Spoiling3dModule();

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

    friend struct Creator3Arg< Spoiling3dModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                  std::string,
                                  RCPointer< SequenceModule > >& >;

    Spoiling3dModule(
                   const std::vector< double >& /* doubleParameters */,
                   const std::vector< std::string >& /* stringParameters */,
                   const std::map< std::string, RCPointer< SequenceModule > >&
                                                         otherSequenceModules );

    // RF-signal axis
    RCPointer< SingleTxExcitation3dModule > _singleTxExcitation3dModule;

    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelectionSpoil;
    sGRAD_PULSE_3D _partitionRewinding;

    // Phase gradient axis (Y)
    sGRAD_PULSE_PE _phaseRewinding;

    // Read gradient axis (X)
    RCPointer< SingleLineReading3dModule > _singleLineReading3dModule;

    // ADC signal data axis

    // Numeric Crystal Oscillator axis

};


}


#endif

