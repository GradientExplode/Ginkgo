#include <gkg-processing-numericalanalysis/TikhonovFilterFactor.h>


gkg::TikhonovFilterFactor::TikhonovFilterFactor()
                          : gkg::FilterFactor(),
                            _lambda( 0.0 )
{
}


gkg::TikhonovFilterFactor::~TikhonovFilterFactor()
{
}


void gkg::TikhonovFilterFactor::setLambda( double lambda )
{

  _lambda = lambda;

}


const double& gkg::TikhonovFilterFactor::getLambda() const
{

  return _lambda;

}


double gkg::TikhonovFilterFactor::getFactor( double singularValue ) const
{

  double sv2 = singularValue * singularValue;
  return ( sv2 / ( sv2 + _lambda * _lambda ) );

}
