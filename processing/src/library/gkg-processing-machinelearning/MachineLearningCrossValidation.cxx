#include <gkg-processing-machinelearning/MachineLearningCrossValidation.h>


gkg::MachineLearningCrossValidation::MachineLearningCrossValidation(
                                        double theMeanSquareError,
                                        double theSquaredCorrelationCoefficient,
                                        double theAccuracy )
                                   : meanSquareError( theMeanSquareError ),
                                     squaredCorrelationCoefficient( 
                                             theSquaredCorrelationCoefficient ),
                                     accuracy( theAccuracy )
{
}


gkg::MachineLearningCrossValidation::MachineLearningCrossValidation(
                                   const MachineLearningCrossValidation& other )
                                   : meanSquareError( other.meanSquareError ),
                                     squaredCorrelationCoefficient( 
                                          other.squaredCorrelationCoefficient ),
                                     accuracy( other.accuracy )
{
}
