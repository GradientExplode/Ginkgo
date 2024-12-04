#ifndef _gkg_pulse_sequence_GinkgoLibrary_SequenceModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_SequenceModule_h_


#include <Core/SiemensInclude.h>
#include <string>


namespace gkg
{


class SequenceModule
{

  public:

    virtual ~SequenceModule();

    virtual void initialize( SeqLim& seqLim ) = 0;

    virtual NLSStatus prepare( MrProt& mrProt,
                       SeqLim& seqLim,
                       MrProtocolData::SeqExpo& seqExpo );

    virtual NLS_STATUS modifyKernel( MrProt& mrProt,
                                     SeqLim& seqLim,
                                     MrProtocolData::SeqExpo& seqExpo,
                                     const sSLICE_POS* slices,
                                     long sliceIndex,
                                     long partitionIndex,
                                     long lineIndex ) = 0;

    virtual NLS_STATUS setKernelTiming( MrProt& mrProt ) = 0;
    
    void setAnchorTime( long anchorTime );
    
    long getAnchorTime() const; 
    long getStartTime() const; 
    long getEndTime() const; 
    long getDurationBeforeAnchorTime() const;
    long getDurationAfterAnchorTime() const;
    long getTotalDuration() const;

  protected:

    SequenceModule();
    
    // timing management
    double _minimumRiseTimePerMeter;
    double _absoluteMaximumGradientAmplitude;
    double _nominalMaximumGradientAmplitude;
    long _anchorTime;
    long _durationBeforeAnchorTime;
    long _durationAfterAnchorTime;
    long _totalDuration;
    
    // geometry management
    long _lineCount;
    long _sliceCount;
    long _partitionCount;
    long _centerLineIndex;
    long _centerPartitionIndex;
    long _startLineIndex; 
    long _startLine;
    long _startPartitionIndex;
    long _startPartition;

};


}


#endif
