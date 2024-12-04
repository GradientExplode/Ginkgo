#ifndef _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel2_h_
#define _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel2_h_


#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class DiffusionGaussianModel2 : public DiffusionGaussianModel
{

  public:

    DiffusionGaussianModel2( double volumeFraction,
                             double fastRatio,
                             const Matrix& fastDirectTensor,
                             const Matrix& slowDirectTensor,
                             double bValue,
                             double bigDelta,
                             double littleDelta,
                             double radius );
    ~DiffusionGaussianModel2();

    float getSignalContribution( const Vector& u ) const;
    float getOdfContribution( const Vector& u ) const;

  protected:

    double _volumeFraction;
    double _fastRatio;

    Matrix _fastDirectTensor;
    Matrix _slowDirectTensor;

    Matrix _fastInverseTensor;
    Matrix _slowInverseTensor;

    double _bValue;
    double _bigDelta;
    double _littleDelta;
    double _radius;

    double _fastDirectDeterminant;
    double _slowDirectDeterminant;

    double _q;

};


}


#endif
