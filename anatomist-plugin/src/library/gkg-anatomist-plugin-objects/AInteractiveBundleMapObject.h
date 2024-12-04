#ifndef _gkg_anatomist_plugin_objects_AInteractiveBundleMapObject_h_
#define _gkg_anatomist_plugin_objects_AInteractiveBundleMapObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>


namespace gkg
{


class ABundleMapReader;


class AInteractiveBundleMapObject : public QObject,
                                    public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

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

    int32_t getFiberSubSamplingFactor() const;
    void setFiberSubSamplingFactor( int32_t value );

    float getFiberLineWidth() const;
    void setFiberLineWidth( float value );

    int32_t getFiberColorEncoding() const;
    const std::string& getFileNameScalarFeatureVolume() const;
    const std::string& 
                      getFileNameScalarFeatureVolumeToBundleTransform3d() const;
    double getLowerBound() const;
    double getUpperBound() const;
    void setFiberColorEncoding(
              int32_t colorEncodingType,
              const std::string& fileNameScalarFeatureVolume,
              const std::string& fileNameScalarFeatureVolumeToBundleTransform3d,
              double lowerBound,
              double upperBound );

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;
    
    // static methods
    static std::list< anatomist::AObject* > load( 
                             const std::string& fileName,
                             anatomist::ObjectReader::PostRegisterList& regList,
                             carto::Object options );
    static int32_t registerClass();
    static int32_t getClassType();

    // static public attributes
    static int32_t _classType;
    static Tree* _optionTree;

  protected:

    void computeScalarFeatureVolumeBasedColors();

    RCPointer< BundleMap< std::string > > _bundleMap;
    RCPointer< std::vector< RGBComponent > > _bundleColors;
    BoundingBox< float > _boundingBox;

    int32_t _fiberSubSamplingFactor;
    float _fiberLineWidth;
    int32_t _fiberColorEncoding;
    std::string _fileNameScalarFeatureVolume;
    std::string _fileNameScalarFeatureVolumeToBundleTransform3d;
    double _lowerBound;
    double _upperBound;

    RCPointer< std::vector< float > > _scalarFeatureVolumeBasedColors;

    //std::vector< GLfloat > _pointsAndColors;
    //std::vector< GLuint > _segmentIndices;
    //GLuint _glBuffers[ 2 ];

};


}


#endif
