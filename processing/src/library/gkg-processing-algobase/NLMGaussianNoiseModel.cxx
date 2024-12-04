#include <gkg-processing-algobase/NLMGaussianNoiseModel.h>


gkg::NLMGaussianNoiseModel::NLMGaussianNoiseModel( const float& sigma )
                          : gkg::NLMNoiseModel( sigma )
{
}


float gkg::NLMGaussianNoiseModel::getValue( const float& weight,
                                            const float& value ) const
{

  return weight * value;

}


float gkg::NLMGaussianNoiseModel::getFiltered( const float &value ) const
{

  if ( value > 0.0f )
  {

    return value;

  }

  return -1.0f;

}
