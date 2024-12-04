#ifndef _gkg_dmri_qspace_sampling_SphericalQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SphericalQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>


namespace gkg
{


class SphericalQSpaceSampling : public QSpaceSampling
{

  public:

    virtual ~SphericalQSpaceSampling();

    bool isCartesian() const;
    bool isSpherical() const;

    virtual bool isSingleShell() const = 0;
    virtual bool isMultipleShell() const = 0;

  protected:

    SphericalQSpaceSampling(
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );


};


}


#endif
