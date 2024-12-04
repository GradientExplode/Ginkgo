#ifndef _gkg_dmri_qspace_sampling_MultipleShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_MultipleShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/SphericalQSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


class MultipleShellQSpaceSampling : public SphericalQSpaceSampling
{

  public:

    virtual ~MultipleShellQSpaceSampling();

    bool isSingleShell() const;
    bool isMultipleShell() const;

    int32_t getShellCount() const;
    float getBValue( int32_t shellIndex ) const;
    float getDiffusionTime( int32_t shellIndex ) const;
    // be careful: the orientation set contains raw orientation without
    // application of the 3D tranform !
    virtual const OrientationSet& 
      getOrientationSet( int32_t shellIndex ) const = 0;

  protected:

    MultipleShellQSpaceSampling(
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

    std::vector< float > _shellBValues;
    std::vector< float > _shellDiffusionTimes;
    std::vector< OrientationSet > _orientationSets;

};


}


#endif

