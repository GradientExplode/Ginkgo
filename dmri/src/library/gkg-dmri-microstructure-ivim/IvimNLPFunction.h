#ifndef _gkg_dmri_microstructure_ivim_IvimNLPFunction_h_
#define _gkg_dmri_microstructure_ivim_IvimNLPFunction_h_


#include <gkg-processing-numericalanalysis/NonLinearProgrammingFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class IvimNLPFunction : public NonLinearProgrammingFunction
{

  public:

    IvimNLPFunction(
                  const Vector& realMeasurements,
                  double noiseVariance,
                  const std::vector< float >& shellBValues,
                  int32_t maximumShellIndex,
                  double parenchymaDiffusivity );
    virtual ~IvimNLPFunction();

    void getValuesAt( const Vector& parameters, Vector& values ) const;
    double getValueAt( const Vector& parameters ) const;


  protected:

    double getLogLikelihood( const Vector& currentMeasurements ) const;

    Vector _realMeasurements;
    double _noiseVariance;
    std::vector< float > _shellBValues;
    int32_t _maximumShellIndex;
    double _parenchymaDiffusivity;

    double _twiceNoiseVariance;
    double _twoPi;
    int32_t _measurementCount;
    mutable Vector _currentMeasurements;

};


}


#endif
