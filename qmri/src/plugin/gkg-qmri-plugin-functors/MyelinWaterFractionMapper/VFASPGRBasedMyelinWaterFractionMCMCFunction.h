#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_VFASPGRBasedMyelinWaterFractionMCMCFunction_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_VFASPGRBasedMyelinWaterFractionMCMCFunction_h_


#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMCMCFunction.h>


namespace gkg
{


class VFASPGRBasedMyelinWaterFractionMCMCFunction :
                                          public MyelinWaterFractionMCMCFunction
{

  public:

    VFASPGRBasedMyelinWaterFractionMCMCFunction(
                         const std::vector< Vector >& echoTimes,
                         const Vector& flipAngles,
                         const Vector& repetitionTimes,
                         const std::vector< double >& t2NoiseStandardDeviations,
                         double t1NoiseStandardDeviation );
    virtual ~VFASPGRBasedMyelinWaterFractionMCMCFunction();

    void setB1Scaling( float b1SCaling );

    // parameters(  0 ) -> myelinWaterFraction
    // parameters(  1 ) -> myelinWaterT1
    // parameters(  2 ) -> myelinWaterT2
    // parameters(  3 ) -> grayWhiteT1
    // parameters(  4 ) -> grayWhiteT2
    // parameters(  5 ) -> csfFraction
    // parameters(  6 ) -> csfT1
    // parameters(  7 ) -> csfT2
    // parameters(  8 ) -> protonDensity
    // parameters(  9 ) -> protonDensityScaling
    // parameters( 10 ) -> t2WeightedScaling of second volume
    // parameters( 10 + ... ) -> t2WeightedScaling of ... volume
    // parameters( 10 + t2WeightedCount - 2 ) -> t2WeightedScaling of N+1 volume
    void getValuesAt( const Vector& parameters,
                      Vector& values ) const;

  protected:

    std::vector< Vector > _echoTimes;
    Vector _flipAngles;
    Vector _repetitionTimes;
    float _b1Scaling;

};


}


#endif

