#ifndef _gkg_processing_numericalanalysis_FilterFactor_h_
#define _gkg_processing_numericalanalysis_FilterFactor_h_


namespace gkg
{


class FilterFactor
{

  public:

    virtual ~FilterFactor();

    virtual double getFactor( double singularValue ) const = 0;

  protected:

    FilterFactor();


};


}


#endif

