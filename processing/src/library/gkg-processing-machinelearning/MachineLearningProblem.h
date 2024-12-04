#ifndef _gkg_processing_machinelearning_MachineLearningProblem_h_
#define _gkg_processing_machinelearning_MachineLearningProblem_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-cppext/StdInt.h>

#include <vector>


namespace gkg
{


class MachineLearningProblemImplementation;
class MachineLearningData;
class Vector;


class MachineLearningProblem
{

  public:

    MachineLearningProblem( 
                           RCPointer< MachineLearningData > machineLearningData,
                           Vector& labels );
    MachineLearningProblem( 
                           RCPointer< MachineLearningData > machineLearningData,
                           const std::vector< double >& labels );
    MachineLearningProblem( 
                           RCPointer< MachineLearningData > machineLearningData,
                           Vector& labels,
                           const std::vector< uint8_t >& selectedLabels );
    virtual ~MachineLearningProblem();

    int32_t getMaximumIndex() const;

    MachineLearningProblemImplementation* getImplementation() const;

  protected:

    RCPointer< MachineLearningData > _machineLearningData;

    MachineLearningProblemImplementation* _machineLearningProblemImplementation;

};


}


#endif
