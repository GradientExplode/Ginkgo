#ifndef _gkg_processing_numericalanalysis_NonLinearProgrammingFunction_h_
#define _gkg_processing_numericalanalysis_NonLinearProgrammingFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class NonLinearProgrammingFunction
{

  public:

    virtual ~NonLinearProgrammingFunction();

    int32_t getParameterCount() const;

    bool hasDerivative() const;

    virtual double getValueAt( const Vector& parameters ) const = 0;
    virtual void getDerivativeAt( const Vector& parameters,
                                  Vector& derivative ) const;
    virtual double getValueAndDerivativeAt( const Vector& parameters,
                                            Vector& derivative ) const;

  protected:

    NonLinearProgrammingFunction( int32_t parameterCount,
                                  bool hasDerivative );

    int32_t _parameterCount;
    bool _hasDerivative;

};


}


#endif
