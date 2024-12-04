#include <gkg-processing-algobase/UpdatableWeightedMeanStandardDeviationMinMax.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>


gkg::UpdatableWeightedMeanStandardDeviationMinMax::
                                  UpdatableWeightedMeanStandardDeviationMinMax()
                                       : _mean( 0.0 ),
                                         _standardDeviation( 0.0 ),
                                         _minimum( 1e38 ),
                                         _maximum( -1e38 ),
                                         _valueSum( 0.0 ),
                                         _weightSum( 0.0 ),
                                         _valueSumOfSquare( 0.0 ),
                                         _weightSumOfSquare( 0.0 ),
                                         _count( 0 )
{
}

gkg::UpdatableWeightedMeanStandardDeviationMinMax::
                                   UpdatableWeightedMeanStandardDeviationMinMax(
                const gkg::UpdatableWeightedMeanStandardDeviationMinMax& other )
                                       : _mean( other._mean ),
                                         _standardDeviation(
                                                     other._standardDeviation ),
                                         _minimum( other._minimum ),
                                         _maximum( other._maximum ),
                                         _valueSum( other._valueSum ),
                                         _weightSum( other._weightSum ),
                                         _valueSumOfSquare( 
                                                      other._valueSumOfSquare ),
                                         _weightSumOfSquare( 
                                                     other._weightSumOfSquare ),
                                         _count( other._count )
{
}


gkg::UpdatableWeightedMeanStandardDeviationMinMax::
                                 ~UpdatableWeightedMeanStandardDeviationMinMax()
{
}


gkg::UpdatableWeightedMeanStandardDeviationMinMax& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::operator=(
                const gkg::UpdatableWeightedMeanStandardDeviationMinMax& other )
{

  try
  {

    _mean = other._mean;
    _standardDeviation = other._standardDeviation;
    _minimum = other._minimum;
    _maximum = other._maximum;
    _valueSum = other._valueSum;
    _weightSum = other._weightSum;
    _valueSumOfSquare = other._valueSumOfSquare;
    _weightSumOfSquare = other._weightSumOfSquare;
    _count = other._count;

    return *this;

  }
  GKG_CATCH( "gkg::UpdatableWeightedMeanStandardDeviationMinMax& "
             "gkg::UpdatableWeightedMeanStandardDeviationMinMax::operator=( "
             "const gkg::UpdatableWeightedMeanStandardDeviationMinMax& other )" );

}


void gkg::UpdatableWeightedMeanStandardDeviationMinMax::add( double value,
                                                             double weight )
{

  try
  { 
  
    _valueSum += value * weight;
    _weightSum += weight;

    _valueSumOfSquare += weight * value * value;
    _weightSumOfSquare += weight * weight;

    ++ _count;

    _mean = _valueSum / _weightSum;

    if( _count > 1 )
    {
      _standardDeviation = std::sqrt( ( _valueSumOfSquare * _weightSum -
                                        _valueSum * _valueSum ) / 
                                      ( _weightSum * _weightSum - 
                                        _weightSumOfSquare ) );
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
  GKG_CATCH( "void gkg::UpdatableWeightedMeanStandardDeviationMinMax::add( "
             "double value )" );

}


const double& gkg::UpdatableWeightedMeanStandardDeviationMinMax::getMean() const
{

  return _mean;

}


const double& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getStandardDeviation() const
{

  return _standardDeviation;

}


const double& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getMinimum() const
{

  return _minimum;

}


const double& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getMaximum() const
{

  return _maximum;

}


const double& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getValueSum() const
{

  return _valueSum;

}


const double& gkg::UpdatableWeightedMeanStandardDeviationMinMax::getValueSumOfSquare() const
{

  return _valueSumOfSquare;

}


const double& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getWeightSum() const
{

  return _weightSum;

}


const double& gkg::UpdatableWeightedMeanStandardDeviationMinMax::getWeightSumOfSquare() const
{

  return _weightSumOfSquare;

}


const int32_t& 
gkg::UpdatableWeightedMeanStandardDeviationMinMax::getCount() const
{

  return _count;

}

