#ifndef _gkg_processing_numericalanalysis_OptimalNonCentralFunctionSampler_h_
#define _gkg_processing_numericalanalysis_OptimalNonCentralFunctionSampler_h_


#include <gkg-processing-numericalanalysis/OptimalParametricFunctionSampler.h>


namespace gkg
{



class OptimalNonCentralChiFunctionSampler : 
                                         public OptimalParametricFunctionSampler
{

  public:
 
    OptimalNonCentralChiFunctionSampler( const ParametricFunction& function );

    void get( Matrix& x ) const;

};


}


#endif
