#ifndef _gkg_pulse_sequence_GinkgoSpinEcho3d_GinkgoSpinEcho3d_h
#define _gkg_pulse_sequence_GinkgoSpinEcho3d_GinkgoSpinEcho3d_h 1

#include <Core/SiemensInclude.h>
#include <Core/SequenceModuleFactory.h>


#ifdef BUILD_SEQU

#define __OWNER

#endif

// The following include is necessary for the DLL generation (using __IMP_EXP)
#include "MrCommon/MrGlobalDefinitions/ImpExpCtrl.h"


class MrProt;
class SeqLim;
class SeqExpo;
class Sequence;


namespace gkg
{


class GinkgoSpinEcho3dUI;


class __IMP_EXP GinkgoSpinEcho3d : public StdSeqIF
{


  public:

    GinkgoSpinEcho3d();
    virtual ~GinkgoSpinEcho3d();

    virtual NLSStatus initialize( SeqLim& seqLim );
    
    virtual NLSStatus prepare( MrProt& mrProt,
                               SeqLim& seqLim,
                               MrProtocolData::SeqExpo& seqExpo );
                               
    virtual NLSStatus check( MrProt& mrProt,
                             SeqLim& seqLim,
                             MrProtocolData::SeqExpo& seqExpo,
                             SEQCheckMode* seqCheckMode );
                             
    virtual NLSStatus run( MrProt& mrProt,
                           SeqLim& seqLim,
                           MrProtocolData::SeqExpo& seqExpo );
                           
    virtual NLS_STATUS runKernel( MrProt& mrProt,
                                 SeqLim& seqLim,
                                 MrProtocolData::SeqExpo& seqExpo,
                                 long kernelMode,
                                 long sliceIndex,
                                 long partitionIndex,
                                 long lineIndex );
                                 
    const GinkgoSpinEcho3dUI* getUI( void ) const;

  protected:

    sSLICE_POS _slices[ K_NO_SLI_MAX ];
    long _lineCountPerSecond;
    
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
    
    std::map< std::string, RCPointer< SequenceModule > > _sequenceModules;

    GinkgoSpinEcho3dUI* _userInterface;
    
    virtual NLS_STATUS createUI( SeqLim& seqLim );
    template< class TYPE > void UnusedArguments( TYPE Argument ) 
                        const { if( false ) { TYPE Dummy; Dummy = Argument; } };
                        
    WPT_NAMESPACE::WIPParameterTool _tool;

  private:
  
    double _rfSpoilPhase;
    long _totalDuration;
    bool _grappaFlag;
  
    GinkgoSpinEcho3d ( const GinkgoSpinEcho3d& right );
    GinkgoSpinEcho3d &operator = ( const GinkgoSpinEcho3d& right );


};


};


#endif
