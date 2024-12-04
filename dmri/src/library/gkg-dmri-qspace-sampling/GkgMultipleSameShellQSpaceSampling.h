#ifndef _gkg_dmri_qspace_sampling_GkgMultipleSameShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GkgMultipleSameShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>


namespace gkg
{


class GkgMultipleSameShellQSpaceSampling :
                                        public MultipleSameShellQSpaceSampling
{

  public:

    GkgMultipleSameShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GkgMultipleSameShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

