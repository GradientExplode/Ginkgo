#ifndef _gkg_dmri_qspace_sampling_SmsVb15SingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SmsVb15SingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>


namespace gkg
{


class SmsVb15SingleShellQSpaceSampling : public SingleShellQSpaceSampling
{

  public:

    SmsVb15SingleShellQSpaceSampling(
                           float bValue,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~SmsVb15SingleShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

