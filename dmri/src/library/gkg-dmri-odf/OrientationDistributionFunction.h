#ifndef _gkg_dmri_odf_OrientationDistributionFunction_h_
#define _gkg_dmri_odf_OrientationDistributionFunction_h_


#include <gkg-dmri-diffusion-feature/DiffusionFeature.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


class OrientationSet;
template < class T > class  Transform3d;
template < class R, class S, uint32_t D, class Compare > class MeshMap;


class OrientationDistributionFunction : public DiffusionFeature
{

  public:

    enum BasisType
    {

      Standard,
      UnnormalizedStandard,
      SphericalHarmonics,
      MultiTissueSphericalHarmonics,
      NoddiWatson,
      NoddiBingham,
      PGSEAxonMapping,
      CTOGSEAxonMapping,
      Shore,
      Ivim,
      AxCaliber

    };

    OrientationDistributionFunction(BasisType basisType = Standard,
                                    const OrientationSet* orientationSet = 0 );
    OrientationDistributionFunction(const OrientationDistributionFunction&
                                    other );
    virtual ~OrientationDistributionFunction();

    OrientationDistributionFunction& operator=(
                                 const OrientationDistributionFunction& other );

    BasisType getBasisType() const;
    std::string getBasisTypeName() const;
    void setBasisType( BasisType basisType );
    void setBasisType( const std::string& basisTypeName );
    void setStandardBasisType();
    void setUnnormalizedStandardBasisType();

    void updateCache( const OrientationSet& orientationSet,
                      float apertureAngle ) const;

    virtual void setOrientationSet( const OrientationSet* orientationSet );
    
    virtual std::vector< float > getOrientationProbabilities() const;
    virtual void getOrientationProbabilities( 
                                    std::vector< float >& probabilities ) const;
    virtual void
      getOrientationProbabilitiesWithSpecificNormalization(
                                 std::vector< float >& orientationProbabilities,
                                 float minimumProbability,
                                 float maximumProbability ) const;

    virtual std::vector< float > getUnnormalizedOrientationProbabilities() const;
    virtual std::vector< float > getUnnormalizedOrientationProbabilities(
                                const Vector3d< float >& coneUnitaryOrientation,
                                float apertureAngle,
                                float* minimumProbability = 0,
                                float* maximumProbability = 0 ) const;
    virtual std::vector< float > getUnnormalizedOrientationProbabilities(
                                int32_t coneUnitaryOrientationIndex,
                                float apertureAngle,
                                float* minimumProbability = 0,
                                float* maximumProbability = 0 ) const;
    virtual std::vector< float > getUnnormalizedOrientationProbabilitiesFast(
                                int32_t coneUnitaryOrientationIndex,
                                float& minimumProbability,
                                float& maximumProbability ) const;
    virtual float getOrientationProbability( int32_t index ) const;
    virtual float getOrientationProbability( 
                            const Vector3d< float >& unitaryOrientation ) const;
    virtual float getOrientationProbabilityWithSpecificNormalization(
                                              int32_t index,
                                              float minimumProbability,
                                              float maximumProbability  ) const;
    virtual float getOrientationProbabilityWithSpecificNormalization( 
                                    const Vector3d< float >& unitaryOrientation,
                                    float minimumProbability,
                                    float maximumProbability  ) const;
    virtual float getOrientationProbabilityWithSpecificNormalization(
                                 const Vector3d< float >& unitaryOrientation,
                                 float globalMinimumProbability,
                                 float globalMaximumProbability,
                                 std::vector< float >& orientationProbabilities,
                                 float& localMinimumProbability,
                                 float& localMaximumProbability ) const;
    virtual float getOrientationProbabilityWithSpecificNormalization(
                           const std::vector< float >& orientationProbabilities,
                           float localMinimumProbability,
                           float localMaximumProbability,
                           const Vector3d< float >& unitaryOrientation,
                           float globalMinimumProbability,
                           float globalMaximumProbability ) const;


    virtual const Vector3d< float >& getPrincipalOrientation() const;
    virtual int32_t getPrincipalOrientationIndex() const;
    virtual int32_t getPrincipalOrientationIndexFast() const;

    virtual int32_t getGibbsSampledOrientationIndexFast(
                                         const RandomGenerator& randomGenerator,
                                         float temperature = 1.0f ) const;
    virtual const Vector3d< float >& getGibbsSampledOrientation(
                                         const RandomGenerator& randomGenerator,
                                         float temperature = 1.0f ) const;

    virtual void getGibbsSampledOrientationIndicesFast(
                              const RandomGenerator& randomGenerator,
                              std::vector< int32_t >& sampledOrientationIndices,
                              float temperature = 1.0f ) const;
    virtual void getGibbsSampledOrientations(
                          const RandomGenerator& randomGenerator,
                          std::vector< Vector3d< float > >& sampledOrientations,
                          float temperature = 1.0f ) const;

    virtual const Vector3d< float >& getPrincipalOrientation( 
                                const Vector3d< float >& coneUnitaryOrientation,
                                float apertureAngle ) const;
    virtual int32_t getPrincipalOrientationIndex( 
                                            int32_t coneUnitaryOrientationIndex,
                                            float apertureAngle ) const;
    virtual int32_t getPrincipalOrientationIndexFast( 
                                    int32_t coneUnitaryOrientationIndex ) const;

    virtual float getMinimumOrientationProbability() const;
    virtual float getMaximumOrientationProbability() const;
    virtual void getMinMaxOrientationProbability( float& min,
                                                  float& max ) const;
    virtual void getMinMaxAndOrientationProbabilities(
                                            std::vector< float >& probabilities,
                                            float& min, float& max) const;
    virtual float getGeneralizedFractionalAnisotropy() const;
    virtual float getStandardDeviation() const;
    virtual float getNormalizedEntropy() const;
    virtual float getNematicOrder() const;
    
    template < class R, class Compare >
    void addSymmetricalMesh( const Transform3d< float >& transform,
                             const R& rank,
                             MeshMap< R, float, 3U, Compare >& meshMap ) const;


  protected:

    
    BasisType _basisType;
    const OrientationSet* _orientationSet;

  private:

    int32_t getUnnormalizedOrientationProbabilitiesMaxIndex(
                                    int32_t coneUnitaryOrientationIndex ) const;
    int32_t getUnnormalizedOrientationProbabilitiesMaxIndexFast(
                                    int32_t coneUnitaryOrientationIndex ) const;
  
};


}


///////////////////////////////////////////////////////////////////////////////
// creating type name for OrientationDistributionFunction
///////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::OrientationDistributionFunction,
                  gkg_OrientationDistributionFunction );


#endif
