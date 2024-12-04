#include <gkg-processing-plugin-svm/LibsvmMachineLearningProblemImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-plugin-svm/LibsvmMachineLearningDataImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>


gkg::LibsvmMachineLearningProblemImplementation::
                                     LibsvmMachineLearningProblemImplementation( 
                            gkg::MachineLearningProblem* machineLearningProblem,
                            gkg::MachineLearningData& machineLearningData,
                            gkg::Vector& labels )
                                    : gkg::MachineLearningProblemImplementation(
                                                         machineLearningProblem,
                                                         machineLearningData,
                                                         labels )
{

  try
  {

    int32_t dataCount = machineLearningData.getMeasureCount();

    if ( dataCount != labels.getSize() )
    {

      throw std::runtime_error(
                         "measurement count and label count are not the same" );

    }

    _libsvmProblem.y = new double[ dataCount ];

    if ( !_libsvmProblem.y )
    {

      throw std::runtime_error( "not enough memory" );

    }

    _libsvmProblem.l = machineLearningData.getMeasureCount();
    _libsvmProblem.x = 
      static_cast< gkg::LibsvmMachineLearningDataImplementation* >(
                     machineLearningData.getImplementation() )->getLibsvmData();
    
    int32_t label = 0;
    for ( label = 0; label < _libsvmProblem.l; label++ )
    {

      _libsvmProblem.y[ label ] = labels( label );

    }

  }
  GKG_CATCH( "gkg::LibsvmMachineLearningProblemImplementation::"
             "LibsvmMachineLearningProblemImplementation(  "
             "gkg::MachineLearningProblem* machineLearningProblem, "
             "gkg::MachineLearningData& data, gkg::Vector& labels )" );

}


gkg::LibsvmMachineLearningProblemImplementation::
                                   ~LibsvmMachineLearningProblemImplementation()
{

  // we don't need to delete _libsvmProblem.x because it is a pointer to 
  // gkg::MachineLearningDataImplementation::svn_node that will be deleted
  // in the class gkg::MachineLearningDataImplementation destructor
  delete [] _libsvmProblem.y;

}


const svm_problem&
gkg::LibsvmMachineLearningProblemImplementation::getLibsvmProblem() const
{

  return _libsvmProblem;

}


gkg::LibsvmMachineLearningProblemImplementation*
gkg::LibsvmMachineLearningProblemImplementation::getImplementation(
                           gkg::MachineLearningProblem* machineLearningProblem )
{

  return static_cast< gkg::LibsvmMachineLearningProblemImplementation* >(
                                  machineLearningProblem->getImplementation() );

}
