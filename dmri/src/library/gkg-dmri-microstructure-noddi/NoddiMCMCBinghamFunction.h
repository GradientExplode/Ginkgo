#ifndef _gkg_dmri_microstructure_noddi_NoddiMCMCBinghamFunction_h_
#define _gkg_dmri_microstructure_noddi_NoddiMCMCBinghamFunction_h_


#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class NoddiMCMCBinghamFunction : public NoddiMCMCFunction
{

  public:

    NoddiMCMCBinghamFunction( RCPointer< Volume< float > > dw,
                              const OrientationSet& outputOrientationSet,
                              double noiseSandardDeviation );
    virtual ~NoddiMCMCBinghamFunction();

    void getValuesAt( const Vector& parameters, Vector& values ) const;
    void getModelAt(
             const Vector& parameters,
             const std::vector< float >& bValues,
             const std::vector< std::vector< float > >& gradientCharacteristics,
             int32_t inputOrientationCount,
             std::vector< std::vector< float > >& values,
             std::vector< float >& coordinates ) const;

    OrientationDistributionFunction::BasisType getType() const;
    int32_t getParameterCount() const;

  protected:

    void getApparentExtracellularTensor(
                                 const Vector3d< float >& principalOrientation,
                                 const Vector3d< float >& secondaryOrientation,
                                 double parallelDiffusivityPrime,
                                 double secondaryDiffusivityPrime,
                                 double tertiaryDiffusivityPrime,
                                 Matrix& apparentExtracellularTensor ) const;

};


}


#endif
