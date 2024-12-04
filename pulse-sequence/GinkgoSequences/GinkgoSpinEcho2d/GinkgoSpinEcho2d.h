#ifndef _gkg_pulse_sequence_GinkgoSpinEcho2d_GinkgoSpinEcho2d_h
#define _gkg_pulse_sequence_GinkgoSpinEcho2d_GinkgoSpinEcho2d_h 1

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


class GinkgoSpinEcho2dUI;


class __IMP_EXP GinkgoSpinEcho2d : public StdSeqIF
{


  public:

    GinkgoSpinEcho2d();
    virtual ~GinkgoSpinEcho2d();

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
                                 long partition,
                                 long lineIndex );
                                 
    const GinkgoSpinEcho2dUI* getUI( void ) const;

  protected:

    sSLICE_POS _slices[ K_NO_SLI_MAX ];
    long _lineCountPerSecond;
    
    // geometry management
    long _lineCount;
    long _sliceCount;
    long _centerLineIndex;
    long _startLineIndex; 
    long _startLine;
    
    std::map< std::string, RCPointer< SequenceModule > > _sequenceModules;

    GinkgoSpinEcho2dUI* _userInterface;
    
    virtual NLS_STATUS createUI( SeqLim& seqLim );
    template< class TYPE > void UnusedArguments( TYPE Argument ) 
                        const { if( false ) { TYPE Dummy; Dummy = Argument; } };

  private:
  
    double _rfSpoilIncrement;
    double _rfSpoilPhase;
  
    GinkgoSpinEcho2d ( const GinkgoSpinEcho2d& right );
    GinkgoSpinEcho2d &operator = ( const GinkgoSpinEcho2d& right );


};


};


#endif



