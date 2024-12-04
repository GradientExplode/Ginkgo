#ifndef _gkg_dmri_qspace_sampling_GehcSingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_GehcSingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>


namespace gkg
{


class GehcSingleShellQSpaceSampling : public SingleShellQSpaceSampling
{

  public:

    GehcSingleShellQSpaceSampling(
                           float bValue,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~GehcSingleShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

