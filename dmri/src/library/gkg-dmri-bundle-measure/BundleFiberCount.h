#ifndef _gkg_dmri_bundle_measure_BundleFiberCount_h_
#define _gkg_dmri_bundle_measure_BundleFiberCount_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>


namespace gkg
{


class BundleFiberCount : public BundleMeasure< int32_t >
{

  public:

    typedef BundleMeasure< int32_t >::Bundle Bundle;

    BundleFiberCount();
    virtual ~BundleFiberCount();

    int32_t get( const Bundle& bundle ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

};


}


#endif
