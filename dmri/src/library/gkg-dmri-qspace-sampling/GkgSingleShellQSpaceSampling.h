#ifndef _gkg_dmri_qspace_sampling_GkgSingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GkgSingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>


namespace gkg
{


class GkgSingleShellQSpaceSampling : public SingleShellQSpaceSampling
{

  public:

    GkgSingleShellQSpaceSampling(
                           float bValue,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GkgSingleShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

