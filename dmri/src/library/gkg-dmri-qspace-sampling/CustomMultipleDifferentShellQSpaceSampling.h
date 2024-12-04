#ifndef _gkg_dmri_qspace_sampling_CustomMultipleDifferentShellQSpaceSampling_h_
#define _gkg_dmri_qspace_sampling_CustomMultipleDifferentShellQSpaceSampling_h_


#include <gkg-dmri-qspace-sampling/MultipleDifferentShellQSpaceSampling.h>
#include <gkg-dmri-qspace-sampling/CustomSingleShellQSpaceSampling.h>
#include <gkg-core-pattern/RCPointer.h>
#include <map>
#include <list>
#include <vector>


namespace gkg
{


class CustomMultipleDifferentShellQSpaceSampling :
                                            public MultipleDifferentShellQSpaceSampling
{

  public:

    CustomMultipleDifferentShellQSpaceSampling(
                           const std::string& fileNameBValues,
                           const std::string& fileNameOrientations,
                           float deltaBValue,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    CustomMultipleDifferentShellQSpaceSampling(
                           const std::vector< float >& bValues,
                           const std::vector< Vector3d< float > >& orientations,
                           float deltaBValue,
                           const HomogeneousTransform3d< float >& transform3d,
                           const Dictionary& gradientCharacteristics );
    virtual ~CustomMultipleDifferentShellQSpaceSampling();

    void addInformationToHeader( HeaderedObject& headeredObject ) const;

    std::vector< RCPointer< CustomSingleShellQSpaceSampling > >
      getShells( std::map< float, std::list< int32_t > >&
                                                       bValueAndIndices ) const;

    float getDeltaBValue() const;

  protected:

    void setShells( const std::vector< float >& bValues,
                    const std::vector< Vector3d< float > >& orientations,
                    float deltaBValue );

    float _deltaBValue;

};


}


#endif

