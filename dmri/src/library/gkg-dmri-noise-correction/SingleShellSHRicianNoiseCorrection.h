#ifndef _gkg_users_vbrion_SingleShellSHRicianNoiseCorrection_h_
#define _gkg_users_vbrion_SingleShellSHRicianNoiseCorrection_h_


#include <gkg-processing-numericalanalysis/FletcherReevesConjugateGradientFunction.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


//
// class SHRician
//

class SingleShellSHRicianNoiseFunction :
                              public FletcherReevesConjugateGradientFunction,
                              public NelderMeadSimplexFunction
{

  public:

    SingleShellSHRicianNoiseFunction( const Matrix& B,
                           const Vector& signal,
                           float noiseStdDev );
    ~SingleShellSHRicianNoiseFunction();

    double getValueAt( const Vector& parameters ) const;
    Vector getDerivativeAt( const Vector& parameters ) const;
    double getValueAndDerivativeAt( const Vector& parameters,
                                    Vector& derivative ) const;

 private:

    static double alpha( double z );

    const Matrix& _B;
    const Vector& _signal;
    float _noiseStdDev;

};


//
// class SingleShellSHRicianNoiseCorrection
//

class SingleShellSHRicianNoiseCorrection
{

  public:

    SingleShellSHRicianNoiseCorrection(
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             float noiseStdDev,
                             int32_t maximumIterationCount,
                             float stepSize,
                             float maximumTestGradient,
                             float maximumTolerance );
    SingleShellSHRicianNoiseCorrection(
                             const gkg::Volume< float >& t2,
                             const gkg::Volume< float >& dw,
                             const gkg::Vector3d< int32_t >& site,
                             int32_t maximumSphericalHarmonicOrder,
                             float regularizationLCurveFactor,
                             float noiseStdDev,
                             const std::vector< float >& deltaParameters,
                             int32_t maximumIterationCount,
                             float maximumTestSize );
    virtual ~SingleShellSHRicianNoiseCorrection();

    const Vector& getDwSphericalHarmonicsCoefficients() const;
    const Vector& getDenoisedDw() const;
    bool isCorrected() const;

  protected:

    void getReconstructionMatrix( const HeaderedObject& headeredObject,
                                  int32_t maximumSphericalHarmonicOrder,
                                  float regularizationLCurveFactor,
                                  Matrix& B,
                                  Matrix& Bplus );

    Vector _dwSphericalHarmonicsCoefficients;
    Vector _denoisedDw;
    bool _corrected;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for SingleShellSHRicianNoiseCorrection
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::SingleShellSHRicianNoiseCorrection,
                  gkg_SingleShellSHRicianNoiseCorrection );


#endif
