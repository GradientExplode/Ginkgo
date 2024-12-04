#ifndef _gkg_dmri_qspace_sampling_SmsVb15MultipleDifferentShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SmsVb15MultipleDifferentShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleDifferentShellQSpaceSampling.h>


namespace gkg
{


class SmsVb15MultipleDifferentShellQSpaceSampling :
                                    public MultipleDifferentShellQSpaceSampling
{

  public:

    SmsVb15MultipleDifferentShellQSpaceSampling(
                           const std::vector< float >& shellBValues,
                           const std::vector< int32_t >& orientationCounts,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~SmsVb15MultipleDifferentShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

