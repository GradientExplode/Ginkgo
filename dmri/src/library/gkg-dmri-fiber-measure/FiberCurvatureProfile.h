#ifndef _gkg_dmri_fiber_measure_FiberCurvatureProfile_h_
#define _gkg_dmri_fiber_measure_FiberCurvatureProfile_h_


#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>
#include <vector>


namespace gkg
{


class FiberCurvatureProfile : public FiberMeasure< std::vector< float > >
{

  public:

    typedef FiberMeasure< std::vector< float > >::Fiber Fiber;

    FiberCurvatureProfile();
    virtual ~FiberCurvatureProfile();

    std::vector< float > get( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< int32_t, std::map< std::string, Matrix > >,
                       Compare >& measures );

};


}


#endif
