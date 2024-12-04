#ifndef _gkg_dmri_qspace_sampling_CartesianQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_CartesianQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>


namespace gkg
{


class CartesianQSpaceSampling : public QSpaceSampling
{

  public:

    CartesianQSpaceSampling(
                           float maximumBValue,
                           int32_t qSpacePointCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~CartesianQSpaceSampling();

    bool isCartesian() const;
    bool isSpherical() const;

    float getMaximumBValue() const;
    int32_t getQSpacePointCount() const;
    // returns the common diffusion time to all the q-space samples
    float getDiffusionTime() const;

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

  protected:

    float _maximumBValue;
    int32_t _qSpacePointCount;

};


}


#endif
