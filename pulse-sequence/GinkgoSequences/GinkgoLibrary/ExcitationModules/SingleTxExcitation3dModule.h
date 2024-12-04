#ifndef _gkg_pulse_sequence_GinkgoLibrary_SingleTxExcitation3dModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SingleTxExcitation3dModule_h_


#include <ExcitationModules/ExcitationModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>


namespace gkg
{


class SingleTxExcitation3dModule : 
                         public ExcitationModule,
                         public Creator3Arg< SingleTxExcitation3dModule,
                                             SequenceModule,
                                             const std::vector< double >&,
                                             const std::vector< std::string >&,
                                             const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~SingleTxExcitation3dModule();

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

    const sRF_PULSE_SINC& getRfWaveformEnveloppe() const;
    const sGRAD_PULSE& getSliceSelectionGradientPulse() const;
    const sGRAD_PULSE_3D& getPartitionEncodingGradientPulse() const;

    static std::string getStaticName();

  protected:

    friend struct Creator3Arg< SingleTxExcitation3dModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                  std::string,
                                  RCPointer< SequenceModule > >& >;

    //
    // doubleParameters[ 0 ] = long rfDuration = 2560
    // doubleParameters[ 1 ] = int32_t rfSampleCount = 128
    // doubleParameters[ 2 ] = double rfBandwidthTimeProduct = 2.70
    // doubleParameters[ 3 ] = long rfInitialPhase = 0
    //
    SingleTxExcitation3dModule(
                   const std::vector< double >& doubleParameters,
                   const std::vector< std::string >& stringParameters,
                   const std::map< std::string, 
                   RCPointer< SequenceModule > >& /* otherSequenceModules */ );

     double _defaultFlipAngle;
    // RF-signal axis
    long _rfDuration;
    int32_t _rfSampleCount;
    double _rfBandwidthTimeProduct;
    double _rfInitialPhase;
    sRF_PULSE_SINC _rfWaveformEnveloppe;
    double _rfSpoilIncrement;

    // Slice gradient axis (Z)
    sGRAD_PULSE _sliceSelection;
    sGRAD_PULSE_3D _partitionEncoding;

    // Phase gradient axis (Y)

    // Read gradient axis (X)

    // ADC signal data axis

    // Numeric Crystal Oscillator axis
    sSYNC_OSC _oscillatorSynchronization;  
    sFREQ_PHASE _rfOffsets;
    sFREQ_PHASE _rfResetSynthetizer;

};


}


#endif
