#ifndef _gkg_dmri_qspace_sampling_GehcNewMultipleSameShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GehcNewMultipleSameShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>


namespace gkg
{


class GehcNewMultipleSameShellQSpaceSampling :
                                        public MultipleSameShellQSpaceSampling
{

  public:

    GehcNewMultipleSameShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GehcNewMultipleSameShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

