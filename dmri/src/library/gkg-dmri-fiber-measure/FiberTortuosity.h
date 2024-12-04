#ifndef _gkg_dmri_fiber_measure_FiberTortuosity_h_
#define _gkg_dmri_fiber_measure_FiberTortuosity_h_


#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>
#include <gkg-dmri-fiber-selector/FiberSelection.h>


namespace gkg
{


class FiberTortuosity : public FiberMeasure< float >
{

  public:

    typedef FiberMeasure< float >::Fiber Fiber;

    FiberTortuosity();
    virtual ~FiberTortuosity();

    float get( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< int32_t, std::map< std::string, Matrix > >,
                       Compare >& measures );

    template < class L, class Compare >
    static void selector(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L, std::set< int32_t >, Compare >& selection );

};


}


#endif
