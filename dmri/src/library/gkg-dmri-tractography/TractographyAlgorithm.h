#ifndef _gkg_dmri_tractography_TractographyAlgorithm_h_
#define _gkg_dmri_tractography_TractographyAlgorithm_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/VoxelSampler.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-tractography/SeedingStrategy.h>
#include <gkg-dmri-tractography/StoppingStrategy.h>
#include <gkg-dmri-tractography/StartingOrientationStrategy.h>
#include <vector>


namespace gkg
{


template < class R, class S, class Compare > class SiteMap;
template < class T > class Volume;
class OdfContinuousField;
class OdfCartesianField;
template < class T > class Transform3d;
template < class R, class Compare > class BundleMap;


template < class L, class Compare = std::less< L > >
class TractographyAlgorithm
{

  public:

    virtual ~TractographyAlgorithm();

    const Vector3d< int32_t >& getSize() const;
    const Vector3d< double >& getResolution() const;

    virtual void prepare( OdfCartesianField& odfCartesianField,
                          const Volume< int16_t >& computingMask,
                          const Transform3d< float >&
                                            transform3dFromRoisVoxelToOdfsReal,
                          const Transform3d< float >&
                                            transform3dFromRoisVoxelToMaskVoxel,
                          const Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                          const Transform3d< float >&
                                            transform3dFromOdfsRealToRoisVoxel,
                          bool verbose );
    virtual void track( 
                        const SiteMap< L, int32_t, Compare >& rois,
                        const SiteMap< L, int32_t, Compare >& subRois,
                        const RCPointer< SeedingStrategy >& seedingStrategy,
                        const RCPointer< StoppingStrategy >& stoppingStrategy,
                        const RCPointer< StartingOrientationStrategy >&
                                                    startingOrientationStrategy,
                        OdfContinuousField& odfContinuousField,
                        const Volume< int16_t >& computingMask,
                        const Transform3d< float >&
                                            transform3dFromRoisVoxelToOdfsReal,
                        const Transform3d< float >&
                                            transform3dFromRoisVoxelToMaskVoxel,
                        const Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                        const Transform3d< float >&
                                            transform3dFromOdfsRealToRoisVoxel,
                        BundleMap< L, Compare >& bundleMap,
                        bool verbose ) const = 0;

  protected:

    TractographyAlgorithm( const Vector3d< int32_t >& size,
                           const Vector3d< double >& resolution );

    Vector3d< int32_t > 
      getVoxelCoordinates( const Vector3d< float >& point ) const;
    Vector3d< float > 
      getRealCoordinates( const Vector3d< int32_t >& voxel ) const;

    Vector3d< int32_t > _size;
    Vector3d< double > _resolution;
    BoundingBox< float > _boundingBox;

};


}


#endif
