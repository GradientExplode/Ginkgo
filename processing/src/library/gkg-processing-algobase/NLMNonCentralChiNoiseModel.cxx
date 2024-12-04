#include <gkg-processing-algobase/NLMNonCentralChiNoiseModel.h>

#include <cmath>


gkg::NLMNonCentralChiNoiseModel::NLMNonCentralChiNoiseModel( 
                                                     const float& sigma,
                                                     const float& channelCount )
                               : gkg::NLMNoiseModel( sigma ),
                                 _ncChiConstant( 2.0f * sigma * sigma *
                                                 channelCount )
{
}


float gkg::NLMNonCentralChiNoiseModel::getValue( const float& weight,
                                                 const float& value ) const
{

  return weight * value * value;

}


float gkg::NLMNonCentralChiNoiseModel::getFiltered( const float &value ) const
{

  float squareFiltered = value - _ncChiConstant;

  if ( squareFiltered > 0.0f )
  {

    return std::sqrt( squareFiltered );

  }

  return -1.0f;

}
