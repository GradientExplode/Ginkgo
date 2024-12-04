#ifndef _gkg_dmri_odf_functor_SphericalHarmonicsOdfPeakFinder_h_
#define _gkg_dmri_odf_functor_SphericalHarmonicsOdfPeakFinder_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <vector>


namespace gkg
{


class SphericalHarmonicsOdfPeakFinder
{

  public:

    struct Peak
    {

      Peak( float thePhi = 0.0f, float theTheta = 0.0f, float theHeight = 0.0f )
          : phi( thePhi ),
            theta( theTheta ),
            height( theHeight )
      {
      }
      Peak( const Peak& other )
          : phi( other.phi ),
            theta( other.theta ),
            height( other.height )
      {
      }
      Peak& operator=( const Peak& other )
      {

        phi = other.phi;
        theta = other.theta;
        height = other.height;
        return *this;

      }
      float phi;
      float theta;
      float height;

    };

    SphericalHarmonicsOdfPeakFinder( int32_t peakCount,
                                     float colatitudeDerivativeThreshold,
                                     float resolution,
                                     float clusteringThreshold,
                                     float angularThreshold,
                                     float minimumIncrement,
                                     float maximumIncrement );
    virtual ~SphericalHarmonicsOdfPeakFinder();

    void getPeaks( const OrientationDistributionFunction& odf,
                   std::vector< Vector3d< float > >& peakDirections,
                   std::vector< float >& peakMagnitudes ) const;

  protected:

    void getTanThetaPolynomialCoefficients(
                                     const std::vector< float >& shCoefficients,
                                     float phi,
                                     Vector& polynomCoefficients ) const;
    void findExtremaAmongPolynomialRoots(
                                     const std::vector< float >& shCoefficients,
                                     float phi,
                                     const CVector& polynomialRoots,
                                     std::list< Peak >& odfPeaks ) const;
    void computeHessian( const std::vector< float >& shCoefficients,
                         float phi,
                         float theta,
                         float& psidTheta2,
                         float& psidThetadPhi,
                         float& psidPhi2, 
                         float& psidTheta ) const;
    void getPeakCartesianCoordinates(
                            const std::vector< float >& shCoefficients,
                            float phi,
                            float theta,
                            Vector3d< float >& odfPeakCartesianCoordinates,
                            float& radial ) const;
    void tunePhiIncrement( const std::vector< float >& shCoefficients,
                           float phi,
                           const CVector& polynomialRoots,
                           CVector& storedPolynomialRoots,
                           float& storedPhi,
                           float& phiIncrement ) const;
    float getDistanceBeetweenOdfPeaks(
                                     const std::vector< float >& shCoefficients,
                                     float oldPhi,
                                     float oldTheta,
                                     float currentPhi,
                                     float currentTheta ) const;
    float getAngleBeetweenOdfPeaks( const std::vector< float >& shCoefficients,
                                    float oldPhi,
                                    float oldTheta,
                                    float currentPhi,
                                    float currentTheta ) const;
    void computeClusters( const std::vector< float >& shCoefficients,
                          const std::list< Peak >& odfPeaks,
                          std::list< Peak >& odfPeakClusters ) const;
    void pickBiggestPeaks( const std::vector< float >& shCoefficients,
                           const std::list< Peak >& odfPeakClusters,
                           std::vector< Vector3d< float > >& peakDirections,
                           std::vector< float >& peakMagnitudes ) const;

    
    int32_t _peakCount;
    float _colatitudeDerivativeThreshold;
    float _resolution;
    float _clusteringThreshold;
    float _angularThreshold;
    float _minimumIncrement;
    float _maximumIncrement;

    NumericalAnalysisImplementationFactory* _factory;

};



}



#endif
