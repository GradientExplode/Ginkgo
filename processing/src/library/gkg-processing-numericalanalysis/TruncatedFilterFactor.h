#ifndef _gkg_processing_numericalanalysis_TruncatedFilterFactor_h_
#define _gkg_processing_numericalanalysis_TruncatedFilterFactor_h_


#include <gkg-processing-numericalanalysis/FilterFactor.h>


namespace gkg
{


class TruncatedFilterFactor : public FilterFactor
{

  public:

    TruncatedFilterFactor();
    virtual ~TruncatedFilterFactor();

    virtual void setThreshold( double threshold );
    virtual const double& getThreshold() const;

    virtual double getFactor( double singularValue ) const;

  protected:

    double _threshold;

};


}


#endif

