#include <gkg-processing-algobase/NLMNoiseModel.h>


gkg::NLMNoiseModel::NLMNoiseModel( const float& sigma )
                  : _sigma( sigma )
{
}


gkg::NLMNoiseModel::~NLMNoiseModel()
{
}


float gkg::NLMNoiseModel::getSigma() const
{

  return _sigma;

}
