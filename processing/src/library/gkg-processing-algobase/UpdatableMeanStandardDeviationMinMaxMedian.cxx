#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMaxMedian.h>
#include <gkg-core-exception/Exception.h>

#include <cmath>
#include <vector>
#include <algorithm>


gkg::UpdatableMeanStandardDeviationMinMaxMedian::
                                    UpdatableMeanStandardDeviationMinMaxMedian()
                                               : _mean( 0.0 ),
                                                 _standardDeviation( 0.0 ),
                                                 _minimum( 1e38 ),
                                                 _maximum( -1e38 ),
                                                 _sum( 0.0 ),
                                                 _sumOfSquare( 0.0 ),
                                                 _count( 0 )
{
}

gkg::UpdatableMeanStandardDeviationMinMaxMedian::
                                     UpdatableMeanStandardDeviationMinMaxMedian(
                  const gkg::UpdatableMeanStandardDeviationMinMaxMedian& other )
                                       : _mean( other._mean ),
                                         _standardDeviation(
                                                     other._standardDeviation ),
                                         _minimum( other._minimum ),
                                         _maximum( other._maximum ),
                                         _values( other._values ),
                                         _sum( other._sum ),
                                         _sumOfSquare( other._sumOfSquare ),
                                         _count( other._count )
{
}


gkg::UpdatableMeanStandardDeviationMinMaxMedian::
                                   ~UpdatableMeanStandardDeviationMinMaxMedian()
{
}


gkg::UpdatableMeanStandardDeviationMinMaxMedian& 
gkg::UpdatableMeanStandardDeviationMinMaxMedian::operator=(
                  const gkg::UpdatableMeanStandardDeviationMinMaxMedian& other )
{

  try
  {

    _mean = other._mean;
    _standardDeviation = other._standardDeviation;
    _minimum = other._minimum;
    _maximum = other._maximum;
    _values = other._values;
    _sum = other._sum;
    _sumOfSquare = other._sumOfSquare;
    _count = other._count;

    return *this;

  }
  GKG_CATCH( "gkg::UpdatableMeanStandardDeviationMinMaxMedian& "
             "gkg::UpdatableMeanStandardDeviationMinMaxMedian::operator=( "
             "const gkg::UpdatableMeanStandardDeviationMinMaxMedian& other )" );

}


void gkg::UpdatableMeanStandardDeviationMinMaxMedian::add( double value )
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

    // storing value for future median computation
    _values.push_back( value );

  }
  GKG_CATCH( "void gkg::UpdatableMeanStandardDeviationMinMaxMedian::"
             "add( double value )" );

}


const double& gkg::UpdatableMeanStandardDeviationMinMaxMedian::getMean() const
{

  return _mean;

}


const double& 
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getStandardDeviation() const
{

  return _standardDeviation;

}


const double&
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getMinimum() const
{

  return _minimum;

}


const double&
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getMaximum() const
{

  return _maximum;

}


const double&
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getMedian() const
{

  try
  {

    std::vector< double > medianValues( _values.begin(), _values.end() );
    std::sort ( medianValues.begin(), medianValues.end() );

    return medianValues[ medianValues.size() / 2U ];

  }
  GKG_CATCH( "const double& "
             "gkg::UpdatableMeanStandardDeviationMinMaxMedian::"
             "getMedian() const" );

}


const double&
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getSum() const
{

  return _sum;

}


const double&
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getSumOfSquare() const
{

  return _sumOfSquare;

}


const int32_t& gkg::UpdatableMeanStandardDeviationMinMaxMedian::getCount() const
{

  return _count;

}


const std::list< double >& 
gkg::UpdatableMeanStandardDeviationMinMaxMedian::getValues() const
{

  try
  {

    return _values;

  }
  GKG_CATCH( "const std::list< double >& "
             "gkg::UpdatableMeanStandardDeviationMinMaxMedian::"
             "getValues() const" );

}


