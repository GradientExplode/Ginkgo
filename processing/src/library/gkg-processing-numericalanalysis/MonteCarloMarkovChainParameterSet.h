#ifndef _gkg_processing_numericalanalysis_MonteCarloMarkovChainParameterSet_h_
#define _gkg_processing_numericalanalysis_MonteCarloMarkovChainParameterSet_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


namespace gkg
{


class MonteCarloMarkovChainParameterSet
{

  public:

    virtual ~MonteCarloMarkovChainParameterSet();

    int32_t getParameterCount() const;

    const Vector& getCurrentParameters() const;
    const Vector& getNewParameters() const;

    const Vector& getLowerBounds() const;
    const Vector& getUpperBounds() const;

    const std::vector< bool >& getFixed() const;

    const Vector& getSteps() const;

    // setting values of current parameters
    void setCurrentParameters( const Vector& others );

    // return true if the new parameters are well located within their lower and
    // upper bounds, and return false if not
    virtual bool updateParameters() = 0;

    // copy new parameters to current parameters
    virtual void acceptNewParameters();

  protected:

    // minimumParameters: lower bound values for all the parameters
    // maximumParameters: upper bound values for all the parameters
    // fixedParameters: set to true if fixed or false if to be modified
    // parameterScales: Preal = scale * Pmcmc
    // parameterSteps: magnitude of the modification for each parameter

    MonteCarloMarkovChainParameterSet( int32_t parameterCount,
                                       const Vector& lowerBounds,
                                       const Vector& upperBounds,
                                       const std::vector< bool >& fixed,
                                       const Vector& steps );

    int32_t _parameterCount;

    Vector _currentParameters;
    Vector _newParameters;

    Vector _lowerBounds;
    Vector _upperBounds;
    std::vector< bool > _fixed;
    Vector _steps;


};



}


#endif
