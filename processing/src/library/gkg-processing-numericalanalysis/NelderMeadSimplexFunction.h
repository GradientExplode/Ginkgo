#ifndef _gkg_processing_numericalanalysis_NelderMeadSimplexFunction_h_
#define _gkg_processing_numericalanalysis_NelderMeadSimplexFunction_h_


#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{


class NelderMeadSimplexFunction
{

  public:

    virtual ~NelderMeadSimplexFunction();

    int32_t getParameterCount() const;

    virtual double getValueAt( const Vector& parameters ) const = 0;

  protected:

    NelderMeadSimplexFunction( int32_t parameterCount );

    int32_t _parameterCount;

};


}


#endif
