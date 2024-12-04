#ifndef _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel3_h_
#define _gkg_dmri_plugin_functors_DwiGaussianSimulator_DiffusionGaussianModel3_h_


#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel.h>
#include <gkg-processing-numericalanalysis/Matrix.h>


namespace gkg
{


class DiffusionGaussianModel3 : public DiffusionGaussianModel
{

  public:

    DiffusionGaussianModel3( double volumeFraction,
                             double fastRatio,
                             const Matrix& fastDirectTensor,
                             const Matrix& slowDirectTensor,
                             double bValue,
                             double snr );
    ~DiffusionGaussianModel3();

    float getSignalContribution( const Vector& u ) const;
    float getOdfContribution( const Vector& u ) const;

  protected:

    Matrix getNoiseContaminatedTensor( const Matrix& directTensor ) const;

    double _volumeFraction;
    double _fastRatio;

    Matrix _fastDirectTensor;
    Matrix _slowDirectTensor;

    Matrix _fastInverseTensor;
    Matrix _slowInverseTensor;

    double _bValue;
    double _snr;

    double _fastDirectDeterminant;
    double _slowDirectDeterminant;

};


}


#endif
