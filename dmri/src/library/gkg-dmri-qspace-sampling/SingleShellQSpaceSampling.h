#ifndef _gkg_dmri_qspace_sampling_SingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


class SingleShellQSpaceSampling : public SphericalQSpaceSampling
{

  public:

    virtual ~SingleShellQSpaceSampling();

    bool isSingleShell() const;
    bool isMultipleShell() const;


    // returns the average b-value from the vector of b-values stored in 
    // mother class QSpaceSampling since the encoding gradients of the MR image
    // may induce some small variations
    float getBValue() const;
    // returns the common diffusion time to all the q-space samples
    float getDiffusionTime() const;
    // be careful: the orientation set contains raw orientation without
    // application of the 3D tranform  and of the motion 3D rotations!
    const OrientationSet& getOrientationSet() const;

  protected:

    SingleShellQSpaceSampling(
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

    OrientationSet _orientationSet;

};


}


#endif

