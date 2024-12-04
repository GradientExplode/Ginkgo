#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelLevenbergMarquardtFunction_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelLevenbergMarquardtFunction_h_


#include <gkg-processing-numericalanalysis/LevenbergMarquardtFunction.h>


namespace gkg
{


class TwoPoolModelLevenbergMarquardtFunction : 
                                          public gkg::LevenbergMarquardtFunction
{

  public:

    TwoPoolModelLevenbergMarquardtFunction( const gkg::Vector& parameters );
    ~TwoPoolModelLevenbergMarquardtFunction();

    double getValueWithParametersAt( const gkg::Vector& x,
                                     const gkg::Vector& p ) const;
    gkg::Vector getDerivativeWithParametersAt( const gkg::Vector& x,
                                               const gkg::Vector& p ) const;

};


}


#endif


