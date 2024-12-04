#ifndef _gkg_processing_numericalanalysis_SemiConstrainedGaussianMixtureFunction_h_
#define _gkg_processing_numericalanalysis_SemiConstrainedGaussianMixtureFunction_h_


#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>


namespace gkg
{


class SemiConstrainedGaussianMixtureFunction : 
                                          public gkg::LevenbergMarquardtFunction
{

public:

  SemiConstrainedGaussianMixtureFunction( const gkg::Vector& parameters );

  double getValueWithParametersAt( const gkg::Vector& x, 
                                   const gkg::Vector& p ) const;
  gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x, 
                                             const gkg::Vector& p ) const;

};


}


#endif
