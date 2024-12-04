#ifndef _gkg_processing_numericalanalysis_DifferentiableFunction_h_
#define _gkg_processing_numericalanalysis_DifferentiableFunction_h_


namespace gkg
{


class DifferentiableFunction
{

  public:

    virtual ~DifferentiableFunction();

    virtual double getValue( double x ) const = 0;

  protected:

    DifferentiableFunction();

};


}


#endif
