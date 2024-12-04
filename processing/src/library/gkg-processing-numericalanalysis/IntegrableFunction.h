#ifndef _gkg_processing_numericalanalysis_IntegrableFunction_h_
#define _gkg_processing_numericalanalysis_IntegrableFunction_h_


namespace gkg
{


class IntegrableFunction
{

  public:

    virtual ~IntegrableFunction();

    virtual double getValue( double x ) const = 0;

  protected:

    IntegrableFunction();

};


}


#endif
