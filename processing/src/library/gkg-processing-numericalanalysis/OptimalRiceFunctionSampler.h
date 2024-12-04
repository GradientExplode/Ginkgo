#ifndef _gkg_processing_numericalanalysis_OptimalRiceFunctionSampler_h_
#define _gkg_processing_numericalanalysis_OptimalRiceFunctionSampler_h_


#include <gkg-processing-numericalanalysis/OptimalParametricFunctionSampler.h>


namespace gkg
{



class OptimalRiceFunctionSampler : public OptimalParametricFunctionSampler
{

  public:
 
    OptimalRiceFunctionSampler( const ParametricFunction& function );

    void get( Matrix& x ) const;

};


}


#endif
