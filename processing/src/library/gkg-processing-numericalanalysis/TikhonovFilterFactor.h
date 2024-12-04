#ifndef _gkg_processing_numericalanalysis_TikhonovFilterFactor_h_
#define _gkg_processing_numericalanalysis_TikhonovFilterFactor_h_


#include <gkg-processing-numericalanalysis/FilterFactor.h>


namespace gkg
{


class TikhonovFilterFactor : public FilterFactor
{

  public:

    TikhonovFilterFactor();
    virtual ~TikhonovFilterFactor();

    virtual void setLambda( double lambda );
    virtual const double& getLambda() const;

    virtual double getFactor( double singularValue ) const;

  protected:

    double _lambda;

};


}


#endif

