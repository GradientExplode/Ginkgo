#ifndef _gkg_pulse_sequence_GinkgoLibrary_DiffusionGradientModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_DiffusionGradientModule_h_ 1

#include <Core/SiemensInclude.h>
#include <Core/SequenceModule.h>


namespace gkg
{


class DiffusionGradientModule : public SequenceModule
{


  public:

    DiffusionGradientModule();  

    virtual ~DiffusionGradientModule();

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
    
    static std::string getStaticName();
    
    void setParameters( std::vector< double > parameters );
    void setRefocusingDuration( long refocusingDuration );


  protected:
  
    std::vector< double > _parameters;
    long _refocusingDuration;

};


}


#endif
