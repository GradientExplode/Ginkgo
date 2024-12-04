#ifndef _gkg_processing_regiongrowing_RegionGrowingOptimizer_h_
#define _gkg_processing_regiongrowing_RegionGrowingOptimizer_h_


#include <gkg-processing-regiongrowing/RegionGrowingAlgorithm.h>
#include <set>


namespace gkg
{


template < class L >
class RegionGrowingOptimizer
{

  public:

    RegionGrowingOptimizer( RCPointer< RegionGrowingAlgorithm< L > >
                                                       regionGrowingAlgorithm,
                            bool verbose );
    virtual ~RegionGrowingOptimizer();

    void iterate();

  protected:

    virtual L chooseNewLabel( const Vector3d< int32_t >& site,
                              const L& currentLabel,
                              const std::set< L >& newPutativeLabels ) = 0;

    RCPointer< RegionGrowingAlgorithm< L > > _regionGrowingAlgorithm;
    bool _verbose;

};


}


#endif
