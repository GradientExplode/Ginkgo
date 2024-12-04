#ifndef _gkg_dmri_pdf_ProbabilityDensityFunction_h_
#define _gkg_dmri_pdf_ProbabilityDensityFunction_h_


#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-dmri-pdf/PdfMoment.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-dmri-qspace-sampling/MultipleShellQSpaceSampling.h>
#include <gkg-core-io/TypeOf.h>
#include <map>


namespace gkg
{


class ProbabilityDensityFunction : public DiffusionFeature
{

  public:

    enum BasisType
    {

      Standard,
      SecondOrderTensor,
      PolarHarmonics

    };

    ProbabilityDensityFunction( BasisType basisType = Standard );
    ProbabilityDensityFunction(
        BasisType basisType,
        const std::map< float, OrientationSet >* outputMultipleShellSampling );
    ProbabilityDensityFunction(
        BasisType basisType,
        const Vector3d< int32_t >* outputCartesianGridSize,
        const Vector3d< double >* outputCartesianGridResolution );
    ProbabilityDensityFunction( const ProbabilityDensityFunction& other );

    virtual ~ProbabilityDensityFunction();

    ProbabilityDensityFunction& operator=(
                                      const ProbabilityDensityFunction& other );

    BasisType getBasisType() const;
    std::string getBasisTypeName() const;
    void setBasisType( BasisType basisType );
    void setBasisType( const std::string& basisTypeName );
    void setStandardBasisType();

    void updatePolarHarmonicsCache() const;

    void updateDisplacementCache( const std::map< float, OrientationSet >&
                                            outputMultipleShellSampling ) const;
    void updateDisplacementCache(
           const gkg::Vector3d< int32_t >& outputCartesianGridSize,
           const gkg::Vector3d< double >& outputCartesianGridResolution ) const;

    virtual bool checkAndModifyOutputSampling(
               std::map< float, OrientationSet >* outputMultipleShellSampling );
    virtual void setOutputSampling(
         std::map< float, OrientationSet >* outputMultipleShellSampling,
         int32_t resamplingNeighborCount = 3,
         float resamplingGaussianSigma = 1.0f );
    virtual void setOutputSampling(
            const Vector3d< int32_t >* outputCartesianGridSize,
            const Vector3d< double >* outputCartesianGridResolution );


    virtual std::vector< float > getDisplacementProbabilities() const;
    virtual float getDisplacementProbability( int32_t index ) const;
    virtual float getDisplacementProbability( 
                                  const Vector3d< float >& displacement ) const;

    virtual float dot( const ProbabilityDensityFunction& other ) const;

    virtual float getPropagatorAnisotropy() const;
    virtual float getReturnToOriginProbability() const;
    virtual float getReturnToAxisProbability() const;
    virtual float getReturnToPlaneProbability() const;
    virtual float getNonGaussianity() const;
    virtual PdfMoment getRadialMoment(
                                   int32_t order,
                                   const OrientationSet& orientationSet ) const;
    virtual OrientationDistributionFunction getOrientationDistributionFunction(
                                   const OrientationSet& orientationSet ) const;

    virtual OrientationDistributionFunction 
                              getOrientationDistributionFunctionFromSingleShell(
                                                    const Matrix& Bplus ) const;

    virtual OrientationDistributionFunction 
                              getOrientationDistributionFunctionFromSingleShell(
                               const int32_t& maximumSphericalHarmonicOrder,
                               const double& regularizationLCurveFactor ) const;

    void getPolarHarmonicsCoefficients(
                       std::vector< float >& polarHarmonicsCoefficients ) const;
    void getPolarHarmonicsCoefficients(
                       Vector& polarHarmonicsCoefficients ) const;

  protected:

    BasisType _basisType;
    const std::map< float, OrientationSet >* _outputMultipleShellSampling;
    const Vector3d< int32_t >* _outputCartesianGridSize;
    const Vector3d< double >* _outputCartesianGridResolution;

};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for ProbabilityDensityFunction
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::ProbabilityDensityFunction,
                  gkg_ProbabilityDensityFunction );


#endif
