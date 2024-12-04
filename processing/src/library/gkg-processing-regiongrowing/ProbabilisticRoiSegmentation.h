#ifndef _gkg_processing_regiongrowing_ProbabilisticRoiSegmentation_h_
#define _gkg_processing_regiongrowing_ProbabilisticRoiSegmentation_h_

#include <gkg-processing-regiongrowing/RegionGrowingAlgorithm.h>
#include <gkg-processing-container/RoiMeasure.h>
#include <gkg-processing-roi/AtlasInformation.h>
#include <gkg-processing-regiongrowing/RegionGrowingEnergy.h>
#include <gkg-processing-container/FrontEvolutionRoi.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <gkg-processing-transform/TalairachTransform3d_i.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <string>
#include <map>
#include <vector>
#include <set>


namespace gkg
{


template < class A, class C, class L >
class ProbabilisticRoiSegmentation : public RegionGrowingAlgorithm< L >
{

  public:

    ProbabilisticRoiSegmentation(
       const std::string& structureDictionaryName,
       const std::map< std::string, RCPointer< Volume< A > > >& atlasVolumes,
       const std::map< std::string, RCPointer< Volume< C > > >& contrastVolumes,
       const std::map< std::string,
                       std::map< L, RCPointer< PhiFunction > > >& 
                                                     dataAttachmentPhiFunctions,
       const std::map< std::string,
                       std::map< L, RCPointer< Transform3d< float > > > >& 
                                                      labelToAtlasTransform3ds,
       const std::map< std::string, std::map< std::string, double > >& weights,
       int32_t stride = 2,
       bool verbose = false );

    ProbabilisticRoiSegmentation(
       RCPointer< gkg::Volume< L > > labelVolume,
       const std::string& structureDictionaryName,
       const std::map< std::string, RCPointer< Volume< A > > >& atlasVolumes,
       const std::map< std::string, RCPointer< Volume< C > > >& contrastVolumes,
       const std::map< std::string,
                       std::map< L, RCPointer< PhiFunction > > >& 
                                                     dataAttachmentPhiFunctions,
       const std::map< std::string,
                       std::map< L, RCPointer< Transform3d< float > > > >& 
                                                      labelToAtlasTransform3ds,
       const std::map< std::string, std::map< std::string, double > >& weights,
       int32_t stride = 2,
       bool verbose = false );

    virtual ~ProbabilisticRoiSegmentation();

    const std::string& getStructureDictionaryName() const;

    RCPointer< Volume< A > >
      getAtlasVolume( const std::string& contrastName ) const;

    RCPointer< Volume< C > >
      getContrastVolume( const std::string& contrastName ) const;

    RCPointer< PhiFunction >
      getDataAttachmentPhiFunction( const std::string& contrastName,
                                    const L& label ) const;

    RCPointer< Transform3d< float > >
      getLabelToAtlasTransform3d( const std::string& contrastName,
                                  const L& label ) const;

    double getWeight( const std::string& contrastName,
                      const std::string& potentialName ) const;

    int32_t getStride() const;

    L getLabel( int32_t rank ) const;

    // methods for region growing algorithm
    void getRandomSite( Vector3d< int32_t >& site, L& label ) const;
    std::set< L > getNewPutativeLabels( const Vector3d< int32_t >& site,
                                        const L& label ) const;
    bool isBelongingToRoiMask( const L& label,
                               const Vector3d< int32_t >& site ) const;
    double getDeltaEnergy( const Vector3d< int32_t >& site,
                           const L& oldLabel, 
                           const L& newLabel );
    void applyModification( const Vector3d< int32_t >& site,
                            const L& oldLabel, 
                            const L& newLabel );

  private:

    void initializeLabelVolume();
    void initializeEnergy();
    void initializeFrontEvolutionRoi( const L& label, int32_t& rank );
    RCPointer< RoiMeasure< L > > initializeDataAttachmentMeasure(
                                               const std::string& contrastName,
                                               const L& label );
    RCPointer< RoiMeasure< L > > initializeVolumeMeasure( const L& label );
    RCPointer< RoiMeasure< L > > initializeSurfaceMeasure( const L& label );
    RCPointer< RoiMeasure< L > > initializeFrontPottsMeasure(
                const L& label,
                RCPointer< AtlasInformation< std::string > > atlasInformation );
    RCPointer< RoiMeasure< L > > initializeProbabilisticAtlasMeasure( 
                    const L& label,
                    RCPointer< Volume< A > > atlas,
                    const std::string& contrastName,
                    int32_t dataBaseCount );

    std::string _structureDictionaryName;
    std::map< std::string, RCPointer< Volume< A > > > _atlasVolumes;
    std::map< std::string, RCPointer< Volume< C > > > _contrastVolumes;
    std::map< std::string, std::map< L, RCPointer< PhiFunction > > >
                                                    _dataAttachmentPhiFunctions;
    std::map< std::string,
              std::map< L, RCPointer< Transform3d< float > > > >
                                                      _labelToAtlasTransform3ds;
    std::map< std::string, std::map< std::string, double > > _weights;
    int32_t _stride;
    RCPointer< IdentityPhiFunction > _identityPhiFunction;
    bool _verbose;

    RandomGenerator _randomGenerator;
    std::list< gkg::Vector3d< int32_t > > _neighbor26s;
    std::map< L, RCPointer< FrontEvolutionRoi< L > > > _frontEvolutionRois;
    std::map< int32_t, L > _rankToLabelLut;
    std::map< std::string,
              std::map< L, int32_t > > _contrastAndLabelToRankInAtlas;

};


}


#endif
