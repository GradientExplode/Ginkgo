#ifndef _gkg_dmri_qspace_sampling_MultipleSameShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_MultipleSameShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>


namespace gkg
{


class MultipleSameShellQSpaceSampling : public MultipleShellQSpaceSampling
{

  public:

    virtual ~MultipleSameShellQSpaceSampling();

    // be careful: the orientation set contains raw orientation without
    // application of the 3D tranform !
    const OrientationSet& getOrientationSet( int32_t /*shellIndex*/ ) const;

  protected:

    MultipleSameShellQSpaceSampling(
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

};


}


#endif

