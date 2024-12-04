#include <gkg-processing-numericalanalysis/TruncatedFilterFactor.h>


gkg::TruncatedFilterFactor::TruncatedFilterFactor()
                           : gkg::FilterFactor()
{
}


gkg::TruncatedFilterFactor::~TruncatedFilterFactor()
{
}


void gkg::TruncatedFilterFactor::setThreshold( double threshold )
{

  _threshold = threshold;

}


const double& gkg::TruncatedFilterFactor::getThreshold() const
{

  return _threshold;

}


double gkg::TruncatedFilterFactor::getFactor( double singularValue ) const
{

  return ( singularValue >= _threshold ? 1.0 : 0.0 );

}
