#ifndef _gkg_dmri_tractography_GfaAlgorithm_h_
#define _gkg_dmri_tractography_GfaAlgorithm_h_


#include <gkg-dmri-plugin-functors/MpiTest/ParallelizedAlgorithm.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class GfaAlgorithm :
           public ParallelizedAlgorithm,
           public Creator1Arg< GfaAlgorithm,
                               ParallelizedAlgorithm,
                               const std::string& >
{

  public:

    GfaAlgorithm( const std::string& );
    virtual ~GfaAlgorithm();

    static std::string getStaticName();

  protected:

    friend class Creator1Arg< GfaAlgorithm,
                              ParallelizedAlgorithm,
                              const std::string& >;

    bool compute() const;

};


}


#endif
