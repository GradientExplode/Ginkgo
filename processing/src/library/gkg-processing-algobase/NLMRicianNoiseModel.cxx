#include <gkg-processing-algobase/NLMRicianNoiseModel.h>

#include <cmath>


gkg::NLMRicianNoiseModel::NLMRicianNoiseModel( const float& sigma )
                        : gkg::NLMNoiseModel( sigma ),
                          _riceConstant( 2.0f * sigma * sigma )
{
}


float gkg::NLMRicianNoiseModel::getValue( const float& weight,
                                          const float& value ) const
{

  return weight * value * value;

}


float gkg::NLMRicianNoiseModel::getFiltered( const float &value ) const
{

  float squareFiltered = value - _riceConstant;

  if ( squareFiltered > 0.0f )
  {

    return std::sqrt( squareFiltered );

  }

  return -1.0f;

}
