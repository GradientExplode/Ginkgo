#ifndef _gkg_processing_numericalanalysis_LevenbergMarquardtFunction_h_
#define _gkg_processing_numericalanalysis_LevenbergMarquardtFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class LevenbergMarquardtFunction
{

  public:

    virtual ~LevenbergMarquardtFunction();

    const Vector& getParameters() const;
    Vector& getParameters();

    virtual double getValueWithParametersAt( const Vector& x,
                                             const Vector& p ) const = 0;
    virtual Vector getDerivativeWithParametersAt( const Vector& x,
                                                  const Vector& p ) const = 0;

    double getValueAt( const Vector& x ) const;
    Vector getDerivativeAt( const Vector& x ) const;

  protected:

    LevenbergMarquardtFunction( const Vector& parameters );

    Vector _parameters;

};


}


#endif
