#ifndef _gkg_pulse_sequence_GinkgoLibrary_TrapezoidDiffusionGradientModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_TrapezoidDiffusionGradientModule_h_ 1

#include <Core/SiemensInclude.h>
#include <Core/SequenceModule.h>
#include <PreparationModules/DiffusionGradientModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>

#include <fstream>

namespace gkg
{


class TrapezoidDiffusionGradientModule : 
                          public DiffusionGradientModule,
                          public Creator3Arg< TrapezoidDiffusionGradientModule,
                                              SequenceModule,
                                              const std::vector< double >&,
                                              const std::vector< std::string >&,
                                              const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{


  public:

    TrapezoidDiffusionGradientModule();  

    virtual ~TrapezoidDiffusionGradientModule();

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
    
    const long& getSeparationTime() const;
    long getFirstGradientEndTime();
    //~ void setDiffusionIndex( long diffusionIndex );
    
    void setParameters( long diffusionStartTimesDifference,
                        long diffusionPlateauDuration,
                        double sliceSelectionDiffusionAmplitude,
                        double phaseDiffusionAmplitude,
                        double readoutDiffusionAmplitude );
    
    static std::string getStaticName();
 

  protected:

    friend struct Creator3Arg< TrapezoidDiffusionGradientModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                  std::string,
                                  RCPointer< SequenceModule > >& >;

    TrapezoidDiffusionGradientModule(
                   const std::vector< double >& doubleParameters,
                   const std::vector< std::string >& stringParameters,
                   const std::map< std::string, 
                   RCPointer< SequenceModule > >& /* otherSequenceModules */ );
                                                   
    sGRAD_PULSE _sliceSelectionDiffusion;
    sGRAD_PULSE_PE _phaseDiffusion;
    sGRAD_PULSE _readoutDiffusion;
    //~ long _diffusionIndex;
    long _separationTime;
    long _diffusionPlateauDuration; // small delta
    long _diffusionStartTimesDifference; // big delta
    double _sliceSelectionDiffusionAmplitude; // Gz
    double _phaseDiffusionAmplitude; // Gy
    double _readoutDiffusionAmplitude; // Gx
    std::vector< std::vector< double > > _parameterSets;

};


}


#endif
