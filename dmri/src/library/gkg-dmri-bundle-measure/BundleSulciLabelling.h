#ifndef _gkg_dmri_bundle_measure_BundleSulciLabelling_h_
#define _gkg_dmri_bundle_measure_BundleSulciLabelling_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <fstream>


namespace gkg
{


struct SulciPart
{

  public:

    SulciPart();
    SulciPart( int16_t theLabel,
               const BoundingBox< float >& theBoundingBox,
               const Vector3d< float >& theGravityCenter,
               const Vector3d< float >& theDirection );
    SulciPart( const SulciPart& other );
    virtual ~SulciPart();

    SulciPart& operator=( const SulciPart& other );

    int16_t label;
    BoundingBox< float > boundingBox;
    Vector3d< float > gravityCenter;
    Vector3d< float > direction;

};


struct SulciInformation
{

  public:

    SulciInformation();
    SulciInformation( const std::string& theName,
                      int16_t theLabel,
                      const std::list< SulciPart >& theSulciParts );
    SulciInformation( const SulciInformation& other );
    virtual ~SulciInformation();

    SulciInformation& operator=( const SulciInformation& other );

    std::string name;
    int16_t label;
    std::list< SulciPart > sulciParts;

};


class BundleSulciLabelling : public BundleMeasure< std::vector< float > >
{

  public:
    
    typedef BundleMeasure< std::vector< int32_t > >::Bundle Bundle;
  
    BundleSulciLabelling(
           RCPointer< MeshMap< int32_t, float, 3U > > pialMesh,
           RCPointer< std::vector< SulciInformation > > sulciInformations,
           RCPointer< Volume< int16_t > > sulciMask,
           RCPointer< Transform3d< float > > transform3dFromPialMeshToSulciMask,
           RCPointer< Transform3d< float > > transform3dFromBundleToSulciMask,
           float resamplingStep,
           float maximumDisplacement,
           float displacementStep,
           int32_t neighborhoodSize,
           double extremityTukeyApodizationAlphaWeight,
           double orthogonalityTukeyApodizationAlphaWeight,
           double extremityTukeyApodizationPower,
           double orthogonalityTukeyApodizationPower );
    virtual ~BundleSulciLabelling();
    
    std::vector< float > get( const Bundle& bundle) const;


    // stringParameters[ 0 ] : input mesh filename
    // stringParameters[ 1 ] : input sulci graph filename
    // stringParameters[ 2 ] : input pial mesh to sulci mask map 3D transform
    //                         filename
    // stringParameters[ 3 ] : input bundle map to sulci mask 3D transform
    //                         filename
    // stringParameters[ 4 ] : directory where to store intermediate results
    // stringParameters[ 5 ] : unwanted sulci name list filename 

    // scalarParameters[ 0 ] : resampling step 
    // scalarParameters[ 1 ] : maximum displacement
    // scalarParameters[ 2 ] : displacement step
    // scalarParameters[ 3 ] : neighborhood size
    // scalarParameters[ 4 ] : extremity Tukey apodization alpha weight
    // scalarParameters[ 5 ] : orthogonality Tukey apodization alpha weight
    // scalarParameters[ 6 ] : extremity Tukey apodization power
    // scalarParameters[ 7 ] : orthogonality Tukey apodization power
    // scalarParameters[ 8 ] : verbose

    template < class L, class Compare >
    static void spreadSheetFunctor(
             const std::vector< std::string >& stringParameters,
             const std::vector< double >& scalarParameters,
             RCPointer< BundleMap< L, Compare > > bundleMap,
             std::map< L,
                       std::map< std::string, Matrix >,
                       Compare >& measures );

    static void readSulciGraphInformation(
                const std::string& graphArgFileName,
                const std::string& graphDataDirectoryName,
                const std::string& intermediateDirectoryName,
                const std::string& unwantedSulciFileName,
                std::vector< SulciInformation >& sulciInformations,
                Volume< int16_t >& sulciMask,
                std::vector< std::string >& keptSulciNames,
                std::set< std::string >& unwantedSulciNames,
                bool verbose );

    static void rayToBoundingBoxIntersection(
                                        const BoundingBox< float >& boundingBox,
                                        const Vector3d< float >& gravityCenter,
                                        const Vector3d< float >& direction,
                                        Vector3d< float >& lowerExtremity,
                                        Vector3d< float >& upperExtremity );

    static bool seekKey( std::ifstream& is, const std::string& key );

    static float getTukeyWindowFunction( int32_t index,
                                         float alpha,
                                         float power,
                                         int32_t count );
                                         

  protected:

    RCPointer< MeshMap< int32_t, float, 3U  > > _pialMesh;
    RCPointer< std::vector< SulciInformation > > _sulciInformations;
    RCPointer< Volume< int16_t > > _sulciMask; 
    RCPointer< Transform3d< float > > _transform3dFromPialMeshToSulciMask;
    RCPointer< Transform3d< float > > _transform3dFromBundleToSulciMask;
    float _resamplingStep;
    float _maximumDisplacement;
    float _displacementStep; 
    int32_t _neighborhoodSize;
    double _extremityTukeyApodizationAlphaWeight;
    double _orthogonalityTukeyApodizationAlphaWeight;
    double _extremityTukeyApodizationPower;
    double _orthogonalityTukeyApodizationPower;

    int16_t _sulciMaximumLabel;
    Vector3d< int32_t > _sulciMaskSize;
    Vector3d< double > _sulciMaskResolution;
    BoundingBox< int32_t > _boundingBoxSulciMask;

    
    std::vector< Vector3d< float > > _vertices;
    std::vector< Vector3d< float > > _normals;
    BoundingBox< float > _boundingBoxPialMesh;

    RCPointer< Volume< std::list< int32_t >* > > _meshVertexLut;
    
};


}


#endif
