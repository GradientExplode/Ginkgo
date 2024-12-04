#ifndef _gkg_processing_numericalanalysis_MonteCarloMarkovChainFunction_h_
#define _gkg_processing_numericalanalysis_MonteCarloMarkovChainFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


namespace gkg
{


class MonteCarloMarkovChainFunction
{

  public:

    virtual ~MonteCarloMarkovChainFunction();

    virtual void getValuesAt( const Vector& parameters,
                              Vector& values ) const = 0;
    virtual void getValuesAt( const std::vector< float >& parameters,
                              std::vector< float >& values ) const;
    virtual void getValuesAt( const std::vector< double >& parameters,
                              std::vector< double >& values ) const;

    virtual double getLikelihoodRatio(
                                      const Vector& realMeasurements,
                                      const Vector& currentMeasurements,
                                      const Vector& newMeasurements ) const = 0;

  protected:

    MonteCarloMarkovChainFunction();

};


}


#endif
