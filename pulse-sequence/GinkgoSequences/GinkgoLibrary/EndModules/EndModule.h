#ifndef _gkg_pulse_sequence_GinkgoLibrary_EndModule_h_
#define _gkg_pulse_sequence_GinkgoLibrary_EndModule_h_


#include <Core/SequenceModule.h>
#include <Core/RCPointer.h>
#include <Core/Creator.h>
#include <vector>
#include <map>

namespace gkg
{


class EndModule : public SequenceModule,
                  public Creator3Arg< EndModule,
                                      SequenceModule,
                                      const std::vector< double >&,
                                      const std::vector< std::string >&,
                                      const std::map<
                                                std::string,
                                                RCPointer< SequenceModule > >& >
{

  public:

    virtual ~EndModule();

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

  protected:

    friend struct Creator3Arg< EndModule,
                               SequenceModule,
                               const std::vector< double >&,
                               const std::vector< std::string >&,
                               const std::map<
                                        std::string,
                                        RCPointer< SequenceModule > >& >;

    EndModule( const std::vector< double >& doubleParameters,
               const std::vector< std::string >& stringParameters,
               const std::map< std::string, 
               RCPointer< SequenceModule > >& /* otherSequenceModules */ );
                                                   
    long _defaultTR;

};


}


#endif

