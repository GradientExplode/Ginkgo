#include <gkg-fmri-statistics/GLMStatisticalFunction.h>
#include <gkg-core-exception/Exception.h>


gkg::GLMStatisticalFunction::GLMStatisticalFunction( int32_t effectiveScanCount,
                                                     int32_t contrastCount )
                            : _effectiveScanCount( effectiveScanCount ),
                              _contrastCount( contrastCount )
{

  try
  {

    // getting numerical analysis implementation factory
    _factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();

  }
  GKG_CATCH( "gkg::GLMStatisticalFunction::GLMStatisticalFunction( "
             "int32_t effectiveScanCount, "
             "int32_t contrastCount )" );

}


gkg::GLMStatisticalFunction::~GLMStatisticalFunction()
{
}
