#ifndef _gkg_dmri_bundle_measure_BundleFromVolumeStatisticsAlongCentroid_h_
#define _gkg_dmri_bundle_measure_BundleFromVolumeStatisticsAlongCentroid_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-resampling/QuickResampler.h>
#include <gkg-processing-algobase/MinimumFilter.h>
#include <gkg-processing-algobase/MaximumFilter.h>
#include <gkg-processing-algobase/AverageFilter.h>
#include <gkg-processing-algobase/StdDevFilter.h>
#include <gkg-processing-statistics/StatisticalParameterMap.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>
#include <vector>
#include <map>


namespace gkg
{


template < class T > class Transform3d;


template < class T >
class BundleFromVolumeStatisticsAlongCentroid :
                  public BundleMeasure< std::vector< StatisticalParameterMap > >
{

  public:

    typedef typename BundleMeasure< std::vector< T > >::Bundle Bundle;

    BundleFromVolumeStatisticsAlongCentroid(
                  RCPointer< std::vector< LightCurve3d< float > > > centroids,
                  RCPointer< std::map< std::string, int32_t > > labelToIndexLut,
                  RCPointer< Volume< int16_t > > sectionVolume,
                  RCPointer< std::vector< int32_t > > sectionCounts,
                  RCPointer< Volume< T > > scalarVolume,
                  RCPointer< Transform3d< float > > 
                                           transform3dFromScalarVolumeToBundles,
                  int32_t rank,
                  RCPointer< Volume< uint8_t > > maskVolume,
                  RCPointer< Transform3d< float > > 
                                             transform3dFromMaskVolumeToBundles,
                  const std::string& fileNameIndividualBundle,
                  const T& background );
    virtual ~BundleFromVolumeStatisticsAlongCentroid();

    void setCentroidLabel( const std::string& label );
    std::vector< StatisticalParameterMap > get( const Bundle& bundle ) const;

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

  protected:

    RCPointer< std::vector< LightCurve3d< float > > > _centroids;
    RCPointer< std::map< std::string, int32_t > > _labelToIndexLut;
    RCPointer< Volume< int16_t > > _sectionVolume;
    RCPointer< std::vector< int32_t > > _sectionCounts;
    gkg::Vector3d< double > _sectionVolumeResolution;
    RCPointer< Volume< T > > _scalarVolume;
    gkg::Vector3d< double > _scalarVolumeResolution;
    int32_t _rank;
    RCPointer< Transform3d< float > > _transform3dFromScalarVolumeToBundles;
    RCPointer< Volume< uint8_t > > _maskVolume;
    gkg::Vector3d< double > _maskVolumeResolution;
    RCPointer< Transform3d< float > > _transform3dFromMaskVolumeToBundles;
    float _sectionWidth;
    float _resamplingStep;
    std::string _fileNameIndividualBundle;
    T _background;
    std::string _centroidLabel;

    QuickResampler< T >* _resampler;
    MinimumFilter< std::vector< float >, double > _minimumFilter;
    MaximumFilter< std::vector< float >, double > _maximumFilter;
    AverageFilter< std::vector< float >, double > _averageFilter;
    StdDevFilter< std::vector< float >, double > _stdDevFilter;

};


}


#endif
