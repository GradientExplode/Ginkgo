#ifndef _gkg_pulse_sequence_GinkgoLibrary_CompositeSequenceModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_CompositeSequenceModule_h_



#include "Core/SequenceModule.h"
#include "Core/RCPointer.h"
#include <list>



namespace gkg
{



class CompositeSequenceModule : public SequenceModule
{

  public:

    CompositeSequenceModule();
    virtual ~CompositeSequenceModule();

    void addModule( const RCPointer< SequenceModule >& sequenceModule );


    virtual void initialize( SeqLim& seqLim );

    virtual NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );

    virtual NLS_STATUS modifyKernel( MrProt& mrProt,
                                     SeqLim& seqLim,
                                     MrProtocolData::SeqExpo& seqExpo,
                                     const sSLICE_POS* slices,
                                     long sliceIndex,
                                     long partitionIndex,
                                     long lineIndex );

    virtual NLS_STATUS setKernelTiming( MrProt& mrProt );
    
    //void setAnchorTime( long anchorTime );
    //void setGrappaFlag( bool grappaFlag );
    
    //bool getGrappaFlag();
    //long getAnchorTime() const; 
    //long getStartTime() const; 
    //long getEndTime() const; 
    //long getDurationBeforeAnchorTime() const;
    //long getDurationAfterAnchorTime() const;
    //long getTotalDuration() const;


  protected:

    std::list< RCPointer< SequenceModule > > _sequenceModules;


};



}


#endif

