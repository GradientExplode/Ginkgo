#ifndef _gkg_dmri_qspace_sampling_SingleDirectionQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_SingleDirectionQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/QSpaceSampling.h>


namespace gkg
{


class SingleDirectionQSpaceSampling : public QSpaceSampling
{

  public:

    SingleDirectionQSpaceSampling(
                           const std::string& fileNameBValues,
                           const std::string& fileNameOrientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    SingleDirectionQSpaceSampling(
                           const std::vector< float >& bValues,
                           const std::vector< Vector3d< float > >& orientations,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );

    virtual ~SingleDirectionQSpaceSampling();

    bool isCartesian() const;
    bool isSpherical() const;

    const Vector3d< float >& getMeanOrientation() const;

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

  protected:

    Vector3d< float > _meanOrientation;

};


}


#endif
