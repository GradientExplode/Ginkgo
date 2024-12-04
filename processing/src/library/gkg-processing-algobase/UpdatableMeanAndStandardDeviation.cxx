#include <gkg-processing-algobase/UpdatableMeanAndStandardDeviation.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UpdatableMeanAndStandardDeviation::UpdatableMeanAndStandardDeviation()
                                       : _mean( 0.0 ),
                                         _standardDeviation( 0.0 ),
                                         _sum( 0.0 ),
                                         _sumOfSquare( 0.0 ),
                                         _count( 0 )
{
}

gkg::UpdatableMeanAndStandardDeviation::UpdatableMeanAndStandardDeviation(
                           const gkg::UpdatableMeanAndStandardDeviation& other )
                                       : _mean( other._mean ),
                                         _standardDeviation(
                                                     other._standardDeviation ),
                                         _sum( other._sum ),
                                         _sumOfSquare( other._sumOfSquare ),
                                         _count( other._count )
{
}


gkg::UpdatableMeanAndStandardDeviation::~UpdatableMeanAndStandardDeviation()
{
}


gkg::UpdatableMeanAndStandardDeviation& 
gkg::UpdatableMeanAndStandardDeviation::operator=(
                           const gkg::UpdatableMeanAndStandardDeviation& other )
{

  try
  {

    _mean = other._mean;
    _standardDeviation = other._standardDeviation;
    _sum = other._sum;
    _sumOfSquare = other._sumOfSquare;
    _count = other._count;

    return *this;

  }
  GKG_CATCH( "gkg::UpdatableMeanAndStandardDeviation& "
             "gkg::UpdatableMeanAndStandardDeviation::operator=( "
             "const gkg::UpdatableMeanAndStandardDeviation& other )" );

}


void gkg::UpdatableMeanAndStandardDeviation::add( double value )
{

  try
  { 
  
    _sum += value;
    _sumOfSquare += value * value;
    ++ _count;
    _mean = _sum / ( double )_count;
    if( _count > 1 )
    {
      _standardDeviation = std::sqrt( ( _sumOfSquare - _sum * _sum / 
                                      ( double )_count ) /
                                      ( ( double )_count - 1.0 ) );
    }
    else
    {

      _standardDeviation = 0.0;

    }

  }
  GKG_CATCH(
           "void gkg::UpdatableMeanAndStandardDeviation::add( double value )" );

}


const double& gkg::UpdatableMeanAndStandardDeviation::getMean() const
{

  return _mean;

}


const double& 
gkg::UpdatableMeanAndStandardDeviation::getStandardDeviation() const
{

  return _standardDeviation;

}


const double& gkg::UpdatableMeanAndStandardDeviation::getSum() const
{

  return _sum;

}


const double& gkg::UpdatableMeanAndStandardDeviation::getSumOfSquare() const
{

  return _sumOfSquare;

}


const int32_t& gkg::UpdatableMeanAndStandardDeviation::getCount() const
{

  return _count;

}

