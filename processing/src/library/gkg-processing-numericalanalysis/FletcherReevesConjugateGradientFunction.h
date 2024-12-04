#ifndef _gkg_processing_numericalanalysis_FletcherReevesConjugateGradientFunction_h_
#define _gkg_processing_numericalanalysis_FletcherReevesConjugateGradientFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class FletcherReevesConjugateGradientFunction
{

  public:

    virtual ~FletcherReevesConjugateGradientFunction();

    int32_t getParameterCount() const;

    virtual double getValueAt( const Vector& parameters ) const = 0;
    virtual Vector getDerivativeAt( const Vector& parameters ) const = 0;
    virtual double getValueAndDerivativeAt( const Vector& parameters,
                                            Vector& derivative ) const = 0;

  protected:

    FletcherReevesConjugateGradientFunction( int32_t parameterCount );

    int32_t _parameterCount;

};


}


#endif
