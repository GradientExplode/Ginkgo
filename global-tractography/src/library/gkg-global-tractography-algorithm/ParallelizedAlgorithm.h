#ifndef _gkg_dmri_tractography_ParallelizedAlgorithm_h_
#define _gkg_dmri_tractography_ParallelizedAlgorithm_h_


#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class ParallelizedAlgorithm
{

  public:

    virtual ~ParallelizedAlgorithm();

    virtual void prepare();
    virtual bool compute() const = 0;

  protected:

    ParallelizedAlgorithm();

};


}


#endif
