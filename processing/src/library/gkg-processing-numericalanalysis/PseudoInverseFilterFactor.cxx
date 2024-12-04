#include <gkg-processing-numericalanalysis/PseudoInverseFilterFactor.h>


gkg::PseudoInverseFilterFactor::PseudoInverseFilterFactor()
                               : gkg::FilterFactor()
{
}


gkg::PseudoInverseFilterFactor::~PseudoInverseFilterFactor()
{
}


double 
gkg::PseudoInverseFilterFactor::getFactor( double /*singularValue*/ ) const
{

  return 1.0;

}
