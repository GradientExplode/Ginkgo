#ifndef _gkg_dmri_qspace_sampling_GehcNewSingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GehcNewSingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>


namespace gkg
{


class GehcNewSingleShellQSpaceSampling : public SingleShellQSpaceSampling
{

  public:

    GehcNewSingleShellQSpaceSampling(
                           float bValue,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GehcNewSingleShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

