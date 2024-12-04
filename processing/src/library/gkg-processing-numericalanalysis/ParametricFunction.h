#ifndef _gkg_processing_numericalanalysis_ParametricFunction_h_
#define _gkg_processing_numericalanalysis_ParametricFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class ParametricFunction
{

  public:

    virtual ~ParametricFunction();

    const Vector& getParameters() const;

    virtual void getValueAt( const Vector& x, Vector& y ) const = 0;
    virtual void getValuesAt( Matrix& x, Matrix& y ) const = 0;

  protected:

    ParametricFunction( const Vector& parameters );

    const Vector& _parameters;

};


}

#endif

