#ifndef _gkg_processing_numericalanalysis_PseudoInverseFilterFactor_h_
#define _gkg_processing_numericalanalysis_PseudoInverseFilterFactor_h_


#include <gkg-processing-numericalanalysis/FilterFactor.h>


namespace gkg
{


class PseudoInverseFilterFactor : public FilterFactor
{

  public:

    PseudoInverseFilterFactor();
    virtual ~PseudoInverseFilterFactor();

    virtual double getFactor( double singularValue ) const;

};


}


#endif

