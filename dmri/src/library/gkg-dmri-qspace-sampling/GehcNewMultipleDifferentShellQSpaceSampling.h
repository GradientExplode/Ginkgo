#ifndef _gkg_dmri_qspace_sampling_GehcNewMultipleDifferentShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GehcNewMultipleDifferentShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleDifferentShellQSpaceSampling.h>


namespace gkg
{


class GehcNewMultipleDifferentShellQSpaceSampling :
                                    public MultipleDifferentShellQSpaceSampling
{

  public:

    GehcNewMultipleDifferentShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           const std::vector< int32_t >& orientationCounts,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GehcNewMultipleDifferentShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

