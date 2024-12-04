#ifndef _gkg_fmri_statistics_GLMStatisticalFunction_h_
#define _gkg_fmri_statistics_GLMStatisticalFunction_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>


namespace gkg
{


class GLMStatisticalFunction
{

  public:

    virtual ~GLMStatisticalFunction();

    virtual void process( const Vector& effects,
                          const Matrix& effectVariances,
                          Vector& statistics ) = 0;

  protected:

    GLMStatisticalFunction( int32_t effectiveScanCount,
                            int32_t contrastCount );

    int32_t _effectiveScanCount;
    int32_t _contrastCount;

    gkg::NumericalAnalysisImplementationFactory* _factory;

};


}


#endif
