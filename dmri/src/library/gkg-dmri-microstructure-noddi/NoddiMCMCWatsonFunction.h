#ifndef _gkg_dmri_microstructure_noddi_NoddiMCMCWastonFunction_h_
#define _gkg_dmri_microstructure_noddi_NoddiMCMCWastonFunction_h_


#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>

#include <vector>



#define NODDI_OPTIMIZED 1
//#define NODDI_NOT_OPTIMIZED 1

namespace gkg
{


class NoddiMCMCWatsonFunction : public NoddiMCMCFunction
{

  public:

    NoddiMCMCWatsonFunction( RCPointer< Volume< float > > dw,
                             const OrientationSet& outputOrientationSet,
                             double noiseStandardDeviation );
    virtual ~NoddiMCMCWatsonFunction();

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

#ifdef NODDI_NOT_OPTIMIZED
    void getApparentExtracellularTensor(
                                 const Vector3d< float >& principalOrientation,
                                 float parallelDiffusivityPrime,
                                 float perpendicularDiffusivityPrime,
                                 Matrix& apparentExtracellularTensor ) const;
#endif

#ifdef NODDI_OPTIMIZED
    void getApparentExtracellularTensor(
                                 const Vector3d< float >& principalOrientation,
                                 float parallelDiffusivityPrime,
                                 float perpendicularDiffusivityPrime,
                                 float* apparentExtracellularTensor ) const;
#endif

    float* _squareAndCrossInputOrientations;

};


}


#endif
