#ifndef _gkg_pulse_sequence_GinkgoGradientEcho2d_GinkgoGradientEcho2d_h
#define _gkg_pulse_sequence_GinkgoGradientEcho2d_GinkgoGradientEcho2d_h 1

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


class GinkgoGradientEcho2dUI;


class __IMP_EXP GinkgoGradientEcho2d : public StdSeqIF
{


  public:

    GinkgoGradientEcho2d();
    virtual ~GinkgoGradientEcho2d();

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
                                 
    const GinkgoGradientEcho2dUI* getUI( void ) const;

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

    GinkgoGradientEcho2dUI* _userInterface;
    
    virtual NLS_STATUS createUI( SeqLim& seqLim );
    template< class TYPE > void UnusedArguments( TYPE Argument ) 
                        const { if( false ) { TYPE Dummy; Dummy = Argument; } };

  private:
  
    GinkgoGradientEcho2d ( const GinkgoGradientEcho2d& right );
    GinkgoGradientEcho2d &operator = ( const GinkgoGradientEcho2d& right );


};


};


#endif



