#ifndef _gkg_dmri_fiber_measure_FiberFromVolumeProfile_h_
#define _gkg_dmri_fiber_measure_FiberFromVolumeProfile_h_


#include <gkg-dmri-fiber-measure/FiberMeasure.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-dmri-container/FiberMeasureSpreadSheet.h>
#include <vector>


namespace gkg
{


template < class T >
class FiberFromVolumeProfile : public FiberMeasure< std::vector< T > >
{

  public:

    typedef typename FiberMeasure< std::vector< T > >::Fiber Fiber;

    FiberFromVolumeProfile( RCPointer< Volume< T > > volume,
                            const T& background = 0,
                            int32_t rank = 0 );
    virtual ~FiberFromVolumeProfile();

    std::vector< T > get( const Fiber& fiber ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< int32_t, std::map< std::string, Matrix > >,
                       Compare >& measures );

  protected:

    RCPointer< Volume< T > > _volume;
    T _background;
    int32_t _rank;
    Vector3d< double > _resolution;
    QuickResampler< T >* _resampler;

};


}


#endif
