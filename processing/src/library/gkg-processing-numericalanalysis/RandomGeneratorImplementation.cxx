#include <gkg-processing-numericalanalysis/RandomGeneratorImplementation.h>


gkg::RandomGeneratorImplementation::RandomGeneratorImplementation(
                                          gkg::RandomGenerator* randomGenerator,
                                          gkg::RandomGenerator::Type /*type*/ )
                                   : _target( randomGenerator )
{
}


gkg::RandomGeneratorImplementation::~RandomGeneratorImplementation()
{
}


gkg::RandomGenerator* gkg::RandomGeneratorImplementation::getTarget() const
{

  return _target;

}
