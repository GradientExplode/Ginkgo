#ifndef _gkg_dmri_qspace_sampling_MultipleDifferentShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_MultipleDifferentShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>


namespace gkg
{


class MultipleDifferentShellQSpaceSampling : public MultipleShellQSpaceSampling
{

  public:

    virtual ~MultipleDifferentShellQSpaceSampling();

    // be careful: the orientation set contains raw orientation without
    // application of the 3D tranform !
    const OrientationSet& getOrientationSet( int32_t shellIndex ) const;

  protected:

    MultipleDifferentShellQSpaceSampling(
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

};


}


#endif

