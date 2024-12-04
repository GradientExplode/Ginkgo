#ifndef _gkg_dmri_bundle_measure_BundleFromVolumeStatistics_h_
#define _gkg_dmri_bundle_measure_BundleFromVolumeStatistics_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>
#include <vector>
#include <map>


namespace gkg
{


template < class T > class Transform3d;


template < class T >
class BundleFromVolumeStatistics :
                                 public BundleMeasure< StatisticalParameterMap >
{

  public:

    typedef typename BundleMeasure< std::vector< T > >::Bundle Bundle;

    BundleFromVolumeStatistics(
                  RCPointer< Volume< T > > scalarVolume,
                  RCPointer< Transform3d< float > > 
                                           transform3dFromScalarVolumeToBundles,
                  int32_t rank,
                  RCPointer< Volume< uint8_t > > maskVolume,
                  RCPointer< Transform3d< float > > 
                                             transform3dFromMaskVolumeToBundles,
                  float resamplingStep,
                  const T& background );
    virtual ~BundleFromVolumeStatistics();

    StatisticalParameterMap get( const Bundle& bundle ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

  protected:

    RCPointer< Volume< T > > _scalarVolume;
    gkg::Vector3d< double > _scalarVolumeResolution;
    int32_t _rank;
    RCPointer< Transform3d< float > > _transform3dFromScalarVolumeToBundles;
    RCPointer< Volume< uint8_t > > _maskVolume;
    gkg::Vector3d< double > _maskVolumeResolution;
    RCPointer< Transform3d< float > > _transform3dFromMaskVolumeToBundles;
    float _resamplingStep;
    T _background;

    QuickResampler< T >* _resampler;

};


}


#endif
