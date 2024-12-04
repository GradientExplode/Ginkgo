#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModel_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModel_h_


#include <gkg-dmri-pdf/ProbabilityDensityFunction.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


template < class T > class Volume;
class HeaderedObject;
template < class T > class Vector3d;
class Vector;
class Matrix;


//
// this->_coefficients[  0 ] -> Fast Dxx
// this->_coefficients[  1 ] -> Fast Dxy
// this->_coefficients[  2 ] -> Fast Dxz
// this->_coefficients[  3 ] -> Fast Dyy
// this->_coefficients[  4 ] -> Fast Dyz
// this->_coefficients[  5 ] -> Fast Dzz
//
// this->_coefficients[  6 ] -> Slow Dxx
// this->_coefficients[  7 ] -> Slow Dxy
// this->_coefficients[  8 ] -> Slow Dxz
// this->_coefficients[  9 ] -> Slow Dyy
// this->_coefficients[ 10 ] -> Slow Dyz
// this->_coefficients[ 11 ] -> Slow Dzz
//
// this->_coefficients[ 12 ] -> Fast Volume Fraction
//

class TwoPoolModel : public ProbabilityDensityFunction
{

  public:

    enum Pool
    {

      Fast,
      Slow

    };

    enum CoefficientIndex
    {

      FastDxx = 0,
      FastDxy,
      FastDxz,
      FastDyy,
      FastDyz,
      FastDzz,
      SlowDxx,
      SlowDxy,
      SlowDxz,
      SlowDyy,
      SlowDyz,
      SlowDzz,
      FastVolumeFraction

    };

    enum EigenSystemIndex
    {

      FastMaximum = 0,
      FastMiddle,
      FastMinimum,
      SlowMaximum,
      SlowMiddle,
      SlowMinimum

    };

    enum FitMethod
    {

      LevenbergMarquardt,
      NelderMeadSimplex

    };

    // default identity tensor
    TwoPoolModel( const OrientationSet* orientationSet = 0 );

    // for standard estimation on the acquired grid
    TwoPoolModel( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Vector3d< int32_t >& site,
                  FitMethod fitMethod );

    // for partial voluming resampled data
    TwoPoolModel( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Vector3d< float >& site,
                  FitMethod fitMethod );

    // for standard estimation on the acquired grid with q-space sample info
    TwoPoolModel( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Matrix& qSpaceSamples,
                  const Vector3d< int32_t >& site,
                  FitMethod fitMethod );

    // for partial voluming resampled data with q-space sample info
    TwoPoolModel( const OrientationSet* orientationSet,
                  const Volume< float >& t2,
                  const Volume< float >& dw,
                  const Matrix& qSpaceSamples,
                  const Vector3d< float >& site,
                  FitMethod fitMethod );

    // from saved values
    TwoPoolModel( const OrientationSet* orientationSet,
                  const Vector& coefficients,
                  float diffusionTime = 1.0f );

    // copy constructor
    TwoPoolModel( const TwoPoolModel& other );
    
    virtual ~TwoPoolModel();

    TwoPoolModel& operator=( const TwoPoolModel& other );

    void setCoefficients( const std::vector< float >& coefficients,
                          float diffusionTime );
    void setCoefficients( const Vector& coefficients,
                          float diffusionTime );
    void setOrientationSet( const OrientationSet* orientationSet );

    std::vector< float > getOrientationProbabilities( Pool poolIndex ) const;
    float getDisplacementProbability(
                                  const Vector3d< float >& displacement ) const;

    const Vector& getCoefficients() const;
    float getCoefficient( CoefficientIndex coefficientIndex ) const;
    float getInverseCoefficient( CoefficientIndex coefficientIndex ) const;
    float getFastVolumeFraction() const;
    float getSlowVolumeFraction() const;

    const Vector& getEigenValues() const;
    float getEigenValue( EigenSystemIndex eigenSystemIndex ) const;
    gkg::Vector3d< float >
      getEigenVector( EigenSystemIndex eigenSystemIndex ) const;

    float getDeterminant( Pool poolIndex ) const;
    float getApparentDiffusionCoefficient( Pool poolIndex ) const;
    float getNorm2( Pool poolIndex ) const;
    float getNorm( Pool poolIndex ) const;
    float getFractionalAnisotropy( Pool poolIndex ) const;
    float getLambdaParallel( Pool poolIndex ) const;
    float getLambdaTransverse( Pool poolIndex ) const;
    float getDiffusionCoefficient(
                           Pool poolIndex,
                           const Vector3d< float >& unitaryOrientation ) const;
    float getInverseDiffusionCoefficient(
                                 Pool poolIndex,
                                 const Vector3d< float >& displacement ) const;

    float getAttenuation( const Vector3d< float >& orientation,
                          float bValue ) const;

    float getDiffusionTime() const;

  protected:

    void getDiffusionTime( const HeaderedObject& headeredObject );
    void getQSpaceSamples( const gkg::HeaderedObject& headeredObject,
                           Matrix& qSpaceSamples );
    void getBiexponentialSolution( const Matrix& qSpaceSamples,
                                   const Vector& normalizedSignal,
                                   FitMethod fitMethod );
    void doEigenSystemAndInverseMatrix();
    void doScaling();

    const OrientationSet* _orientationSet;

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

RegisterBaseType( gkg::TwoPoolModel,
                  gkg_TwoPoolModel );


#endif
