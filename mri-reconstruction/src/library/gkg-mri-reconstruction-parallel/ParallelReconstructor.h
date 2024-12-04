#ifndef _gkg_mri_reconstruction_parallel_ParallelReconstructor_h_
#define _gkg_mri_reconstruction_parallel_ParallelReconstructor_h_


#include <gkg-mri-reconstruction-core/Reconstructor.h>
#include <gkg-mri-reconstruction-parallel/ReductionStrategy.h>


namespace gkg
{


template < class T >
class ParallelReconstructor : public Reconstructor< T >
{

  public:

    enum Axis
    {

      XAxis,
      YAxis,
      ZAxis

    };

    virtual ~ParallelReconstructor();

    void setReductionStrategy( Axis axis,
                               const ReductionStrategy< T >& strategy );

  protected:

    ParallelReconstructor();

    ReductionStrategy< T > _xReductionStrategy;
    ReductionStrategy< T > _yReductionStrategy;
    ReductionStrategy< T > _zReductionStrategy;

};


}


#endif
