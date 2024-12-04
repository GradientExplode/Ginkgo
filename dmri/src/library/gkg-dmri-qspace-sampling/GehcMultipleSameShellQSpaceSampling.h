#ifndef _gkg_dmri_qspace_sampling_GehcMultipleSameShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GehcMultipleSameShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>


namespace gkg
{


class GehcMultipleSameShellQSpaceSampling :
                                        public MultipleSameShellQSpaceSampling
{

  public:

    GehcMultipleSameShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GehcMultipleSameShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

