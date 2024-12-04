#ifndef _gkg_processing_machinelearning_MachineLearningCrossValidation_h_
#define _gkg_processing_machinelearning_MachineLearningCrossValidation_h_


namespace gkg
{


struct MachineLearningCrossValidation
{

  MachineLearningCrossValidation( double theMeanSquareError = 0.0,
                                  double theSquaredCorrelationCoefficient = 0.0,
                                  double theAccuracy = 0.0 );
  MachineLearningCrossValidation( const MachineLearningCrossValidation& other );

  double meanSquareError;
  double squaredCorrelationCoefficient;
  double accuracy;

};


}


#endif
