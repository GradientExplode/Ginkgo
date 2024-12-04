#ifndef _gkg_dmri_qspace_sampling_CustomSingleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_CustomSingleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SingleShellQSpaceSampling.h>


namespace gkg
{


class CustomSingleShellQSpaceSampling : public SingleShellQSpaceSampling
{

  public:

    CustomSingleShellQSpaceSampling(
                           const std::string& fileNameBValues,
                           const std::string& fileNameOrientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    CustomSingleShellQSpaceSampling(
                           const std::vector< float >& bValues,
                           const std::vector< Vector3d< float > >& orientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~CustomSingleShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

};


}


#endif

