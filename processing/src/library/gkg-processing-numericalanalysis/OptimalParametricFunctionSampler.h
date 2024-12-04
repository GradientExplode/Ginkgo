#ifndef _gkg_processing_numericalanalysis_OptimalParametricFunctionSampler_h_
#define _gkg_processing_numericalanalysis_OptimalParametricFunctionSampler_h_


#include <gkg-processing-numericalanalysis/ParametricFunction.h>


namespace gkg
{



class OptimalParametricFunctionSampler
{

  public:

    virtual ~OptimalParametricFunctionSampler();
 
    virtual void get( Matrix& x ) const = 0;

  protected:

    OptimalParametricFunctionSampler( const ParametricFunction& function );

    const ParametricFunction& _function;

};


}


#endif
