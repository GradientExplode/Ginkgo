#include <gkg-processing-machinelearning/MachineLearningProblem.h>
#include <gkg-processing-machinelearning/MachineLearningProblemImplementation.h>
#include <gkg-processing-machinelearning/MachineLearningData.h>
#include <gkg-processing-machinelearning/MachineLearningSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-exception/Exception.h>

#include <algorithm>


gkg::MachineLearningProblem::MachineLearningProblem(
                 gkg::RCPointer< gkg::MachineLearningData > machineLearningData,
                 gkg::Vector& labels )
                           : _machineLearningData( machineLearningData )
{

  try
  {

    _machineLearningProblemImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                                     createMachineLearningProblemImplementation( 
                                                          this,
                                                          *_machineLearningData,
                                                          labels );

  }
  GKG_CATCH( "gkg::MachineLearningProblem::MachineLearningProblem( "
             "gkg::RCPointer< gkg::MachineLearningData > machineLearningData, "
             "gkg::Vector& labels )" );

}


gkg::MachineLearningProblem::MachineLearningProblem(
                 gkg::RCPointer< gkg::MachineLearningData > machineLearningData,
                 const std::vector< double >& inLabels )
                           : _machineLearningData( machineLearningData )
{

  try
  {

    gkg::Vector labels( inLabels.size() );
    std::vector< double >::const_iterator 
      l = inLabels.begin(),
      le = inLabels.end();
    int32_t k = 0;

    while ( l != le )
    {

      labels( k++ ) = *l;
      ++l;

    }

    _machineLearningProblemImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                                     createMachineLearningProblemImplementation( 
                                                          this,
                                                          *_machineLearningData,
                                                          labels );

  }
  GKG_CATCH( "gkg::MachineLearningProblem::MachineLearningProblem( "
             "gkg::RCPointer< gkg::MachineLearningData > machineLearningData, "
             "gkg::Vector& labels, "
             "const std::vector< uint8_t >& selectedLabels )" );

}


gkg::MachineLearningProblem::MachineLearningProblem(
                 gkg::RCPointer< gkg::MachineLearningData > machineLearningData,
                 gkg::Vector& inLabels,
                 const std::vector< uint8_t >& selectedLabels )
                           : _machineLearningData( machineLearningData )
{

  try
  {

    int32_t selectionCount = (int32_t)std::count_if(
                            selectedLabels.begin(),
                            selectedLabels.end(),
                            std::bind2nd( std::not_equal_to< uint8_t >(), 0 ) );
    gkg::Vector labels( selectionCount );
    std::vector< uint8_t >::const_iterator s = selectedLabels.begin();
    int32_t i, k, labelCount = inLabels.getSize();

    for ( i = 0, k = 0; i < labelCount; i++ )
    {

      if ( *s )
      {

        labels( k ) = inLabels( i );
        ++k;

      }

      ++s;

    }

    _machineLearningProblemImplementation =
      gkg::MachineLearningSelector::getInstance().getImplementationFactory()->
                                     createMachineLearningProblemImplementation( 
                                                          this,
                                                          *_machineLearningData,
                                                          labels );

  }
  GKG_CATCH( "gkg::MachineLearningProblem::MachineLearningProblem( "
             "gkg::RCPointer< gkg::MachineLearningData > machineLearningData, "
             "gkg::Vector& labels, "
             "const std::vector< uint8_t >& selectedLabels )" );

}


gkg::MachineLearningProblem::~MachineLearningProblem()
{

  delete _machineLearningProblemImplementation;

}


int32_t gkg::MachineLearningProblem::getMaximumIndex() const
{

  try
  {

    return _machineLearningData->getMaximumIndex();

  }
  GKG_CATCH( "int32_t gkg::MachineLearningProblem::getMaximumIndex() const" );

}


gkg::MachineLearningProblemImplementation*
gkg::MachineLearningProblem::getImplementation() const
{

  return _machineLearningProblemImplementation;

}
