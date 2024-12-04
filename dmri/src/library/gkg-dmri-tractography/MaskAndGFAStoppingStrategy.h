#ifndef _gkg_dmri_tractography_MaskAndGFAStoppingStrategy_h_
#define _gkg_dmri_tractography_MaskAndGFAStoppingStrategy_h_


#include <gkg-dmri-tractography/StoppingStrategy.h>



namespace gkg
{


class MaskAndGFAStoppingStrategy : public StoppingStrategy
{

  public:

    MaskAndGFAStoppingStrategy(
            OdfContinuousField& odfContinuousField,
            const BoundingBox< int32_t >& computingMaskBoundingBox,
            const Volume< int16_t >& computingMask,
            const Transform3d< float >& transform3dFromOdfsRealToMaskVoxel,
            float step,
            float maximumFiberLength,
            bool usingGFAThreshold = false,
            float gfaThreshold = 0.0f );
    virtual ~MaskAndGFAStoppingStrategy();

    bool stop( const Vector3d< float >& pointOdfs,
               OrientationDistributionFunction*& odf,
               float& gfa,
               float& fiberLength,
               std::list< Vector3d< float > >& trajectory,
               bool& keepFiber );

    bool usingGFAThreshold() const;
    float getGFAThreshold() const;

  protected:

    bool _usingGFAThreshold;
    float _gfaThreshold;


};


}



#endif

