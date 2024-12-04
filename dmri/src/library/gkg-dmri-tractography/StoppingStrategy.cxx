#include <gkg-dmri-tractography/StoppingStrategy.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::StoppingStrategy::StoppingStrategy(
        gkg::OdfContinuousField& odfContinuousField,
        const gkg::BoundingBox< int32_t >& computingMaskBoundingBox,
        const gkg::Volume< int16_t >& computingMask,
        const gkg::Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
        float step,
        float maximumFiberLength )
                      : _odfContinuousField( odfContinuousField ),
                        _computingMaskBoundingBox( computingMaskBoundingBox ),
                        _computingMask( computingMask ),
                        _transform3dFromOdfsRealToMaskVoxel(
                                           transform3dFromOdfsRealToMaskVoxel ),
                        _step( step ),
                        _maximumFiberLength( maximumFiberLength )
{
}


gkg::StoppingStrategy::~StoppingStrategy()
{
}
