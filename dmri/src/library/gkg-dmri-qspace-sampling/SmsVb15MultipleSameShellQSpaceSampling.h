#ifndef _gkg_dmri_qspace_sampling_SmsVb15MultipleSameShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SmsVb15MultipleSameShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleSameShellQSpaceSampling.h>


namespace gkg
{


class SmsVb15MultipleSameShellQSpaceSampling :
                                        public MultipleSameShellQSpaceSampling
{

  public:

    SmsVb15MultipleSameShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           int32_t orientationCount,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~SmsVb15MultipleSameShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

