#ifndef _gkg_dmri_qspace_sampling_ArbitraryQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_ArbitraryQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>
#include <gkg-processing-coordinates/OrientationSet.h>


namespace gkg
{


class ArbitraryQSpaceSampling : public QSpaceSampling
{

  public:

    ArbitraryQSpaceSampling(
                           const std::string& fileNameBValues,
                           const std::string& fileNameOrientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    ArbitraryQSpaceSampling(
                           const std::vector< float >& bValues,
                           const std::vector< Vector3d< float > >& orientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

    virtual ~ArbitraryQSpaceSampling();

    bool isCartesian() const;
    bool isSpherical() const;

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

  protected:

    OrientationSet _orientationSet;

};


}


#endif
