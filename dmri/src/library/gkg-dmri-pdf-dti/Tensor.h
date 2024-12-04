#ifndef _gkg_dmri_pdf_dti_Tensor_h_
#define _gkg_dmri_pdf_dti_Tensor_h_


#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Vector;
class Matrix;


class Tensor : public ProbabilityDensityFunction
{

  public:

    enum CoefficientIndex
    {

      Dxx = 0,
      Dxy,
      Dxz,
      Dyy,
      Dyz,
      Dzz

    };

    enum EigenSystemIndex
    {

      Maximum = 0,
      Middle,
      Minimum

    };

    enum FitMethod
    {

      LinearSquare,
      RobustPositiveDefinite

    };


    // default identity tensor
    Tensor();
    Tensor( float dxx,
            float dxy,
            float dxz,
            float dyy,
            float dyz,
            float dzz,
            float diffusionTime = 1.0f );
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling);
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution );

    // for standard estimation on the acquired grid
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< int32_t >& site,
            FitMethod fitMethod );
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< int32_t >& site,
            FitMethod fitMethod );

    // for partial voluming resampled data
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< float >& site,
            FitMethod fitMethod );
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Vector3d< float >& site,
            FitMethod fitMethod );

    // for standard estimation on the acquired grid with invertion system
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Matrix& B,
            const Matrix& U,
            const Vector& S,
            const Matrix& V,
            const Vector3d< int32_t >& site,
            FitMethod fitMethod );
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Matrix& B,
            const Matrix& U,
            const Vector& S,
            const Matrix& V,
            const Vector3d< int32_t >& site,
            FitMethod fitMethod );

    // for partial voluming resampled data with invertion system
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Matrix& B,
            const Matrix& U,
            const Vector& S,
            const Matrix& V,
            const Vector3d< float >& site,
            FitMethod fitMethod );
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Volume< float >& t2,
            const Volume< float >& dw,
            const Matrix& B,
            const Matrix& U,
            const Vector& S,
            const Matrix& V,
            const Vector3d< float >& site,
            FitMethod fitMethod );

    // from saved values
    Tensor( const std::map< float, OrientationSet >* outputMultiShellSampling,
            const Vector& coefficients, float diffusionTime = 1.0f );
    Tensor( const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution,
            const Vector& coefficients, float diffusionTime = 1.0f );

    // copy constructor
    Tensor( const Tensor& other );
    
    virtual ~Tensor();

    Tensor& operator=( const Tensor& other );

    float getDisplacementProbability(
                                  const Vector3d< float >& displacement ) const;

    const Vector& getCoefficients() const;
    float getCoefficient( CoefficientIndex coefficientIndex ) const;
    float getInverseCoefficient( CoefficientIndex coefficientIndex ) const;

    const Vector& getEigenValues() const;
    float getEigenValue( EigenSystemIndex eigenSystemIndex ) const;
    gkg::Vector3d< float >
      getEigenVector( EigenSystemIndex eigenSystemIndex ) const;

    float getDeterminant() const;
    float getApparentDiffusionCoefficient() const;
    float getNorm2() const;
    float getNorm() const;
    float getFractionalAnisotropy() const;
    float getLambdaParallel() const;
    float getLambdaTransverse() const;
    float getDiffusionCoefficient(
                           const Vector3d< float >& unitaryOrientation ) const;
    float getInverseDiffusionCoefficient(
                                 const Vector3d< float >& displacement ) const;
    Vector getRotatedDiffusionWeightedSignal(
                                          const OrientationSet& orientationSet,
                                          const Matrix& rotationMatrix,
                                          double bValue,
                                          float S0 ) const;
    Rotation3d< float > getRotation3d() const;

    float getDiffusionTime() const;

  protected:

    void getDiffusionTime( const HeaderedObject& headeredObject );
    void getOrientationSVDDecomposition( const HeaderedObject& headeredObject,
                                         Matrix& U,
                                         Vector& S,
                                         Matrix& V );
    void getBMatrix( const HeaderedObject& headeredObject,
                     Matrix& B );

    void doEigenSystemAndInverseMatrix();
    void doScaling();

    void getRobustPositiveDefiniteTensor( const Matrix& B,
                                          const Vector& timeSeries );
    double computeJ( const Matrix& D,
                     const Matrix& B,
                     const Vector& weights,
                     const Vector& timeSeries,
                     Matrix& F,
                     double& ED ) const;
    double computeDeltaTau( const Matrix& D, const Matrix& F ) const;
    void computeHplusHminus( double deltaTau,
                             const Matrix& F,
                             const Matrix& D,
                             Matrix& Hplus,
                             Matrix& Hminus ) const;
    void computeNewD( const Matrix& Hplus,
                      const Matrix& Hminus,
                      Matrix& D ) const;
    bool testConvergence( const Matrix& D, const Matrix& oldD ) const;

    Vector _coefficients;
    Vector _inverseCoefficients;
    Vector _eigenValues;
    Matrix _eigenVectors;
    float _diffusionTime;

};


}


////////////////////////////////////////////////////////////////////////////////
// creating type name for Tensor
////////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::Tensor,
                  gkg_Tensor );


#endif
