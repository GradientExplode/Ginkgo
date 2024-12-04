#ifndef _gkg_processing_denoising_deblurring_FastIterativeShrinkageThresholdingAlgorithm_h_
#define _gkg_processing_denoising_deblurring_FastIterativeShrinkageThresholdingAlgorithm_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/TestFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Vector.h>


namespace gkg
{



template < class T, class M, class K >
class FastIterativeShrinkageThresholdingAlgorithm
{

  public:

    FastIterativeShrinkageThresholdingAlgorithm();
    virtual ~FastIterativeShrinkageThresholdingAlgorithm();

    void solve( const Volume< T >& input,
                const Volume< M >& mask,
                const Volume< K >& kernel,
                const TestFunction< M >& testFunction,
                const T& background,
                int32_t maximumIterationCount,
                double tolerance,
                double shrinkageStep,
                int32_t skrinkageMaximumIterationCount,
                double shrinkageTolerance,
                Volume< T >& output ) const;

  protected:

    void applyIterativeShrinkageOperator(
                                         const Matrix& AtA,
                                         const Vector& Atb,
                                         double lipschitzConstant,
                                         const Vector& currentY,
                                         Vector& currentX,
                                         double shrinkageStep,
                                         int32_t shrinkageMaximumIterationCount,
                                         double shrinkageTolerance ) const;
    double sgn( double u ) const;

    NumericalAnalysisImplementationFactory* _factory;

};


}


#endif

