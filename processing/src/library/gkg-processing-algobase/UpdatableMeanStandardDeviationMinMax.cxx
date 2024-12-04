#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UpdatableMeanStandardDeviationMinMax::
                                          UpdatableMeanStandardDeviationMinMax()
                                       : _mean( 0.0 ),
                                         _standardDeviation( 0.0 ),
                                         _minimum( 1e38 ),
                                         _maximum( -1e38 ),
                                         _sum( 0.0 ),
                                         _sumOfSquare( 0.0 ),
                                         _count( 0 )
{
}

gkg::UpdatableMeanStandardDeviationMinMax::
                                           UpdatableMeanStandardDeviationMinMax(
                        const gkg::UpdatableMeanStandardDeviationMinMax& other )
                                       : _mean( other._mean ),
                                         _standardDeviation(
                                                     other._standardDeviation ),
                                         _minimum( other._minimum ),
                                         _maximum( other._maximum ),
                                         _sum( other._sum ),
                                         _sumOfSquare( other._sumOfSquare ),
                                         _count( other._count )
{
}


gkg::UpdatableMeanStandardDeviationMinMax::
                                         ~UpdatableMeanStandardDeviationMinMax()
{
}


gkg::UpdatableMeanStandardDeviationMinMax& 
gkg::UpdatableMeanStandardDeviationMinMax::operator=(
                        const gkg::UpdatableMeanStandardDeviationMinMax& other )
{

  try
  {

    _mean = other._mean;
    _standardDeviation = other._standardDeviation;
    _minimum = other._minimum;
    _maximum = other._maximum;
    _sum = other._sum;
    _sumOfSquare = other._sumOfSquare;
    _count = other._count;

    return *this;

  }
  GKG_CATCH( "gkg::UpdatableMeanStandardDeviationMinMax& "
             "gkg::UpdatableMeanStandardDeviationMinMax::operator=( "
             "const gkg::UpdatableMeanStandardDeviationMinMax& other )" );

}


void gkg::UpdatableMeanStandardDeviationMinMax::add( double value )
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

    if ( value < _minimum )
    {

      _minimum = value;

    }

    if ( value > _maximum )
    {

      _maximum = value;

    }

  }
  GKG_CATCH(
        "void gkg::UpdatableMeanStandardDeviationMinMax::add( double value )" );

}


const double& gkg::UpdatableMeanStandardDeviationMinMax::getMean() const
{

  return _mean;

}


const double& 
gkg::UpdatableMeanStandardDeviationMinMax::getStandardDeviation() const
{

  return _standardDeviation;

}


const double& gkg::UpdatableMeanStandardDeviationMinMax::getMinimum() const
{

  return _minimum;

}


const double& gkg::UpdatableMeanStandardDeviationMinMax::getMaximum() const
{

  return _maximum;

}


const double& gkg::UpdatableMeanStandardDeviationMinMax::getSum() const
{

  return _sum;

}


const double& gkg::UpdatableMeanStandardDeviationMinMax::getSumOfSquare() const
{

  return _sumOfSquare;

}


const int32_t& gkg::UpdatableMeanStandardDeviationMinMax::getCount() const
{

  return _count;

}

