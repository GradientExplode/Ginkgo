#ifndef _gkg_dmri_qspace_sampling_GkgMultipleDifferentShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GkgMultipleDifferentShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleDifferentShellQSpaceSampling.h>


namespace gkg
{


class GkgMultipleDifferentShellQSpaceSampling :
                                    public MultipleDifferentShellQSpaceSampling
{

  public:

    GkgMultipleDifferentShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           const std::vector< int32_t >& orientationCounts,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GkgMultipleDifferentShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

