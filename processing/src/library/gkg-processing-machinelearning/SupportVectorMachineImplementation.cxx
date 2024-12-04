#include <gkg-processing-machinelearning/SupportVectorMachineImplementation.h>
#include <gkg-core-exception/Exception.h>


gkg::SupportVectorMachineImplementation::SupportVectorMachineImplementation( 
                                  SupportVectorMachine* supportVectorMachine,
                                  SupportVectorMachine::SVMType /* type */,
                                  SupportVectorMachine::KernelType /* kernel */,
                                  int32_t /* degree */,
                                  double /* gamma */,
                                  double /* coef0 */,
                                  double /* cost */,
                                  double /* nu */,
                                  double /* epsilonSVR */,
                                  double /* epsilonTermination */,
                                  int32_t /* cacheSizeMB */,
                                  int32_t /* shrinking */ )
                                        : _target( supportVectorMachine )
{
}


gkg::SupportVectorMachineImplementation::~SupportVectorMachineImplementation()
{
}


gkg::SupportVectorMachine* 
gkg::SupportVectorMachineImplementation::getTarget() const
{

  try
  {

    return _target;

  }
  GKG_CATCH( "gkg::SupportVectorMachine* "
             "gkg::SupportVectorMachineImplementation::getTarget() const" );

}
