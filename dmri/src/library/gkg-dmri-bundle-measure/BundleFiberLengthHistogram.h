#ifndef _gkg_dmri_bundle_measure_BundleFiberLengthHistogram_h_
#define _gkg_dmri_bundle_measure_BundleFiberLengthHistogram_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-dmri-fiber-measure/FiberLength.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>
#include <list>


namespace gkg
{


class BundleFiberLengthHistogram : public BundleMeasure< std::list< float > >
{

  public:

    typedef BundleMeasure< std::list< float > >::Bundle Bundle;

    BundleFiberLengthHistogram();
    virtual ~BundleFiberLengthHistogram();

    std::list< float > get( const Bundle& bundle ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

  protected:

    FiberLength _fiberLength;

};


}


#endif
