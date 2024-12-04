#ifndef _gkg_dmri_microstructure_axcaliber_AxCaliberMCMCFunction_h_
#define _gkg_dmri_microstructure_axcaliber_AxCaliberMCMCFunction_h_


#include <gkg-dmri-microstructure-model/MicrostructureMCMCFunction.h>

#include <vector>


namespace gkg
{


class AxCaliberMCMCFunction : public MicrostructureMCMCFunction
{

  public:

    AxCaliberMCMCFunction( RCPointer< Volume< float > > dw,
                           const OrientationSet& outputOrientationSet,
                           double noiseStandardDeviation,
                           int32_t maximumCallaghanModelRootCount,
                           int32_t maximumCallaghanModelFunctionCount );
    virtual ~AxCaliberMCMCFunction();

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

    int32_t getMaximumCallaghanModelRootCount() const;
    int32_t getMaximumCallaghanModelFunctionCount() const;

  protected:

    int32_t _maximumCallaghanModelRootCount;
    int32_t _maximumCallaghanModelFunctionCount;

    std::vector< double > _gradientMagnitudes;
    std::vector< double > _littleDeltas;
    std::vector< double > _bigDeltas;
    std::vector< double > _riseTimes;

};


}


#endif
