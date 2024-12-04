#ifndef _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingMCMCFunction_h_
#define _gkg_dmri_microstructure_axon_mapping_CTOGSEAxonMappingMCMCFunction_h_


#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCFunction.h>

#include <vector>




namespace gkg
{


class CTOGSEAxonMappingMCMCFunction : public AxonMappingMCMCFunction
{

  public:

    CTOGSEAxonMappingMCMCFunction( RCPointer< Volume< float > > dw,
                                   const OrientationSet& outputOrientationSet,
                                   double noiseStandardDeviation );
    virtual ~CTOGSEAxonMappingMCMCFunction();

    void getValuesAt( const Vector& parameters, Vector& values ) const;
    void getModelAt( const Vector& parameters,
                     const std::vector< float >& multipleShellBValues,
                     const std::vector< std::vector< float > >& 
                                                        gradientCharacteristics,
                     int32_t inputOrientationCount,
                     std::vector< std::vector< float > >& values,
                     std::vector< float >& coordinates ) const;

    OrientationDistributionFunction::BasisType getType() const;
    int32_t getParameterCount() const;

  protected:

    void getApparentExtracellularTensor(
                                 const Vector3d< double >& principalOrientation,
                                 double parallelDiffusivityPrime,
                                 double perpendicularDiffusivityPrime,
                                 double* apparentExtracellularTensor ) const;

    double* _squareAndCrossInputOrientations;

};


}


#endif
