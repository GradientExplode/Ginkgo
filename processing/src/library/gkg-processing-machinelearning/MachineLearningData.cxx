#include <gkg-processing-machinelearning/MachineLearningData_i.h>
#include <gkg-processing-numericalanalysis/SparseMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>


gkg::MachineLearningData::MachineLearningData(
                                            gkg::SparseMatrix& data,
                                            gkg::MachineLearningScaler& scaler )
{

  try
  {

    scaler.scale( data );
    _machineLearningDataImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                          createMachineLearningDataImplementation( this, data );

  }
  GKG_CATCH( "gkg::MachineLearningData::MachineLearningData( "
             "gkg::SparseMatrix& data, gkg::MachineLearningScaler& scaler )" );

}


gkg::MachineLearningData::MachineLearningData(
                                            gkg::Matrix& data,
                                            gkg::MachineLearningScaler& scaler )
{

  try
  {

    scaler.scale( data );
    _machineLearningDataImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                          createMachineLearningDataImplementation( this, data );

  }
  GKG_CATCH( "gkg::MachineLearningData::MachineLearningData( "
             "gkg::Matrix& data, gkg::MachineLearningScaler& scaler )" );

}


gkg::MachineLearningData::~MachineLearningData()
{

  delete _machineLearningDataImplementation;

}


int32_t gkg::MachineLearningData::getMeasureCount() const
{

  try
  {

    return _machineLearningDataImplementation->getMeasureCount();

  }
  GKG_CATCH( "int32_t gkg::MachineLearningData::getMeasureCount() const" );

}


int32_t gkg::MachineLearningData::getMaximumIndex() const
{

  try
  {

    return _machineLearningDataImplementation->getMaximumIndex();

  }
  GKG_CATCH( "int32_t gkg::MachineLearningData::getMaximumIndex() const" );

}


gkg::MachineLearningDataImplementation*
gkg::MachineLearningData::getImplementation() const
{

  return _machineLearningDataImplementation;

}
