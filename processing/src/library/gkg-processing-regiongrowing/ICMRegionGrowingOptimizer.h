#ifndef _gkg_processing_regiongrowing_ICMRegionGrowingOptimizer_h_
#define _gkg_processing_regiongrowing_ICMRegionGrowingOptimizer_h_


#include <gkg-processing-regiongrowing/RegionGrowingOptimizer.h>


namespace gkg
{


template < class L >
class ICMRegionGrowingOptimizer : public RegionGrowingOptimizer< L >
{

  public:

    ICMRegionGrowingOptimizer( RCPointer< RegionGrowingAlgorithm< L > >
                                                       regionGrowingAlgorithm,
                               bool verbose );
    virtual ~ICMRegionGrowingOptimizer();

  protected:

    L chooseNewLabel( const Vector3d< int32_t >& site,
                      const L& currentLabel,
                      const std::set< L >& newPutativeLabels );

};


}


#endif
