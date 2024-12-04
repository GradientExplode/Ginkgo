#ifndef _gkg_anatomist_plugin_objects_AInteractiveBundleMapObject_h_
#define _gkg_anatomist_plugin_objects_AInteractiveBundleMapObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-dmri-container/BundleMap.h>
#include <gkg-communication-thread/LoopContext.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>


namespace gkg
{

class ABundleMapReader;


//
// class AInteractiveBundleMapObject
//

class AInteractiveBundleMapObject : public QObject,
                                    public anatomist::SliceableObject
{

  public:


    friend class AnaGkgModule;

    struct FiberInformation
    {

      public:

        FiberInformation( const LightCurve3d< float >* thePointer = 0,
                          int32_t theBundleIndex = -1,
                          int32_t theFiberIndex = -1,
                          int32_t theLevelOfDetailIndex = -1 );
        FiberInformation( const FiberInformation& other );
        FiberInformation& operator=( const FiberInformation& other );

        const LightCurve3d< float >* pointer;
        int32_t bundleIndex;
        int32_t fiberIndex;
        int32_t levelOfDetailIndex;

    };


    struct FiberSegment
    {

      public:

        FiberSegment( int32_t theTotalFiberIndex = 0,
                      int32_t theStartingPointIndex = 0,
                      int32_t theEndingPointIndex = 0 );
        FiberSegment( const FiberSegment& other );
        FiberSegment& operator=( const FiberSegment& other );

        int32_t totalFiberIndex;
        int32_t startingPointIndex;
        int32_t endingPointIndex;

    };

    AInteractiveBundleMapObject(
                 const RCPointer< BundleMap< std::string > >& bundleMap,
                 const RCPointer< std::vector< RGBComponent > >& bundleColors );
    virtual ~AInteractiveBundleMapObject();

    bool Is2DObject();
    bool Is3DObject();

    // tree menu specific to the object
    Tree* optionTree() const;

    // bounding box information
    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;

    bool boundingBox( std::vector< float > & bmin,
                      std::vector< float > & bmax ) const;

    // core methods
    void fillFiberSegmentMap();
    void computeFiberDisplayPercentages();


////////////////////////////
    void fillPointsToKeepVector();
    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    // color encoding widget methods
    float getFiberLineWidth() const;
    void setFiberLineWidth( float value );
    int32_t getFiberColorEncoding() const;
    const std::string& getFileNameScalarFeatureVolume() const;
    const std::string& getFileNameScalarFeatureVolumeToBundleTransform3d()
                                                                          const;
    double getLowerBound() const;
    double getUpperBound() const;
    void setFiberColorEncoding(
              int32_t colorEncodingType,
              const std::string& fileNameScalarFeatureVolume,
              const std::string& fileNameScalarFeatureVolumeToBundleTransform3d,
              double lowerBound,
              double upperBound );

    // level of detail methods
    double getMinimumFiberDisplayPercentage() const;
    void setMinimumFiberDisplayPercentage( double value );

    double getMaximumFiberDisplayPercentage() const;
    void setMaximumFiberDisplayPercentage( double value );

    int32_t getLevelOfDetailCount() const;
    void setLevelOfDetailCount( int32_t value );

    int32_t getCurrentLevelOfDetailIndex() const;
    void setCurrentLevelOfDetailIndex( int32_t value );

    int32_t getFieldOfViewPartitionCount() const;
    void setFieldOfViewPartitionCount( int32_t value );
  

    // fiber sub-sampling methods
    int32_t getFiberSubSamplingFactor() const;
    void setFiberSubSamplingFactor( int32_t value );


    // fiber compression methods
    bool hasFiberCompression() const;
    void setFiberCompression( bool value );
    double getMaximumCompressionStep() const;
    void setMaximumCompressionStep( double value );
    double getMaximumCompressionError() const;
    void setMaximumCompressionError( double value );
    
    // static methods
    static std::list< anatomist::AObject* > load( 
                             const std::string& fileName,
                             anatomist::ObjectReader::PostRegisterList& regList,
                             carto::Object options );
    static int32_t registerClass();
    static int32_t getClassType();
    static void getSubFieldOfViewIndex( const Vector3d< float >& point,
                                        const int32_t& numberOfCubesPerAxis,
                                        const BoundingBox< float >& boundingBox,
                                        int32_t& subFieldOfViewIndex );

    // static public attributes
    static int32_t _classType;
    static Tree* _optionTree;

  protected:


//////////////////////////
    // level of detail methods to select fibers indices
    void getCubeCoordinates( const Vector3d< float >& point,
                             int32_t& indiceCube ) const;
    std::vector< std::pair< int32_t, int32_t > >
      getSelectedCubes( int32_t currentLevelOfDetail ) const;
    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
      getIndicesFromSelectedCubes( int32_t currentLevelOfDetail ) const;
    std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
      getCompressedIndices(
        std::vector< std::tuple< int32_t, int32_t, int32_t, int32_t > >
          indices ) const;
    void setIndicesZoomZero( int32_t levelOfDetail );
    void computeScalarFeatureVolumeBasedColors();
//////////////////////////


    // core attributes
    RCPointer< BundleMap< std::string > > _bundleMap;
    RCPointer< std::vector< RGBComponent > > _bundleColors;
    BoundingBox< float > _boundingBox;


    // color encoding attributes
    float _fiberLineWidth;
    int32_t _fiberColorEncoding;
    std::string _fileNameScalarFeatureVolume;
    std::string _fileNameScalarFeatureVolumeToBundleTransform3d;
    double _lowerBound;
    double _upperBound;

    // level of detail attributes
    double _minimumFiberDisplayPercentage;
    double _maximumFiberDisplayPercentage;
    int32_t _levelOfDetailCount;
    int32_t _currentLevelOfDetailIndex;

    int32_t _fieldOfViewPartitionCount;

    bool _enableZoomZero;

    // fiber sub-sampling
    int32_t _fiberSubSamplingFactor;


    // fiber compression
    bool _hasFiberCompression;
    double _maximumCompressionStep;
    double _maximumCompressionError;


    // structures to store all fibers linearly while keeping full information
    RCPointer< std::vector< FiberInformation > > _allFibers;
    RCPointer< std::vector< int32_t > > _bundleStartIndices;

    // level of detail vectors to store fiber indices
    std::vector< double > _fiberDisplayPercentages;

    // _fiberSegments[ levelOfDetailIndex ][ subFieldOfViewIndex ] providing
    // the detail of segment for each level of detail and each sub-field of 
    // view
    std::vector<
      std::vector<
        std::vector< FiberSegment > > >
      _fiberSegmentMap;

    // vector containing all fiber segments for the current level of detail
    std::vector< FiberSegment > _allFiberSegmentsForCurrentLevelOfDetail;

    // _keptFiberPoints[ bundleIndex ][ fiberIndex ]
    std::vector< std::vector< std::list< int32_t > > > _keptFiberPoints;





    RCPointer< std::vector< float > > _scalarFeatureVolumeBasedColors;





};


//
// classes AInteractiveBundleMapContext
//

class AInteractiveBundleMapPreallocateContext : public LoopContext< int32_t >
{

  public:

    AInteractiveBundleMapPreallocateContext(
        const BundleMap< std::string >::Bundle& bundle,
        const int32_t& bundleIndex,
        const int32_t& numberOfCubesPerAxis,
        const BoundingBox< float >& boundingBox,
        const std::vector< std::vector< int32_t > >& fiberToLevelOfDetail,
        std::vector< std::vector< int32_t > >& memoryNeededInVector );
    virtual ~AInteractiveBundleMapPreallocateContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const BundleMap< std::string >::Bundle& _bundle;
    const int32_t& _bundleIndex;
    const int32_t& _numberOfCubesPerAxis;
    const BoundingBox< float >& _boundingBox;
    const std::vector< std::vector< int32_t > >& _fiberToLevelOfDetail;
    std::vector< std::vector< int32_t > >& _memoryNeededInVector;
    
};


class AInteractiveBundleMapFillContext : public LoopContext< int32_t >
{

  public:

    AInteractiveBundleMapFillContext(
        const BundleMap< std::string >::Bundle& bundle,
        const int32_t& bundleIndex,
        const int32_t& numberOfCubesPerAxis,
        const BoundingBox< float >& boundingBox,
        const std::vector< std::vector< int32_t > >& fiberToLevelOfDetail,
        std::vector< std::vector< std::vector< std::tuple<
          int32_t, int32_t, int32_t, int32_t > > > >& indicesInCubesVector );
    virtual ~AInteractiveBundleMapFillContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const BundleMap< std::string >::Bundle& _bundle;
    const int32_t& _bundleIndex;
    const int32_t& _numberOfCubesPerAxis;
    const BoundingBox< float >& _boundingBox;
    const std::vector< std::vector< int32_t > >& _fiberToLevelOfDetail;
    std::vector< std::vector< std::vector< std::tuple<
      int32_t, int32_t, int32_t, int32_t > > > >& _indicesInCubesVector;
    
};


class AInteractiveBundleMapResamplerContext : public LoopContext< int32_t >
{

  public:

    AInteractiveBundleMapResamplerContext(
        const BundleMap< std::string >::Bundle& bundle,
        const int32_t& bundleIndex,
        const double& resamplingMaximumStep,
        const double& resamplingErrorThreshold,
        std::vector< std::vector< std::vector< int32_t > > >&
          pointsToKeepVector );
    virtual ~AInteractiveBundleMapResamplerContext();

    void doIt( int32_t startIndex, int32_t count );

  private:

    const BundleMap< std::string >::Bundle& _bundle;
    const int32_t& _bundleIndex;
    const double& _resamplingMaximumStep;
    const double& _resamplingErrorThreshold;
    std::vector< std::vector< std::vector< int32_t > > >& _pointsToKeepVector;
    
};


}


#endif
