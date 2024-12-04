#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelNelderMeadSimplexFunction_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelNelderMeadSimplexFunction_h_


#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>


namespace gkg
{


class Vector;
class Matrix;

class TwoPoolModelNelderMeadSimplexFunction : public NelderMeadSimplexFunction
{

  public:

    TwoPoolModelNelderMeadSimplexFunction( const Matrix& qSpaceSamples,
                                           const Vector& measuredAttenuations );
    virtual ~TwoPoolModelNelderMeadSimplexFunction();

    double getValueAt( const Vector& parameters ) const;

  protected:

    const Matrix& _qSpaceSamples;
    const Vector& _measuredAttenuations;

};




}



#endif
