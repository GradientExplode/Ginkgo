#ifndef _gkg_dmri_tractography_StoppingStrategy_h_
#define _gkg_dmri_tractography_StoppingStrategy_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-dmri-container/OdfContinuousField.h>


namespace gkg
{



class StoppingStrategy
{

  public:

    virtual ~StoppingStrategy();

    virtual bool stop( const Vector3d< float >& pointOdfs,
                       OrientationDistributionFunction*& odf,
                       float& gfa,
                       float& fiberLength,
                       std::list< Vector3d< float > >& trajectory,
                       bool& keepFiber ) = 0;

  protected:

    StoppingStrategy(
            OdfContinuousField& odfContinuousField,
            const BoundingBox< int32_t >& computingMaskBoundingBox,
            const Volume< int16_t >& computingMask,
            const Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
            float step,
            float maximumFiberLength );

    OdfContinuousField& _odfContinuousField;
    const BoundingBox< int32_t >& _computingMaskBoundingBox;
    const Volume< int16_t >& _computingMask;
    const Transform3d< float >& _transform3dFromOdfsRealToMaskVoxel;
    float _step;
    float _maximumFiberLength;

};


}


#endif
