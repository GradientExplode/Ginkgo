#ifndef _gkg_anatomist_plugin_objects_AOdfFieldObject_h_
#define _gkg_anatomist_plugin_objects_AOdfFieldObject_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap.h>

#include <anatomist/config/version.h>
#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/color/paletteList.h>
#include <anatomist/color/objectPalette.h>
#include <anatomist/color/palette.h>
#include <anatomist/application/Anatomist.h>
#include <graph/tree/tree.h>

#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

#include <cartodata/volume/volume_d.h>
#include <aims/rgb/rgb.h>

#endif

#include <QObject>
#include <QtOpenGL>
#include <QGLShader>
#include <QGLShaderProgram>


namespace gkg
{



class ARealTimeFiberObject : public QObject, 
                             public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    ARealTimeFiberObject( BundleMap< int16_t >& bundleMap,
                          int32_t opacity = 30 );
    virtual ~ARealTimeFiberObject();

    bool Is2DObject();
    bool Is3DObject();

    Tree* optionTree() const;

    virtual int CanBeDestroyed();

    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;

    bool boundingBox( std::vector<float> & bmin,
                      std::vector<float> & bmax ) const;

    void updateBoundingBox();
    void update();
    void setOpacity( int32_t opacity );
    
    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    static int32_t registerClass();

    static int32_t _classType;
    static Tree* _optionTree;

    static int32_t getClassType();

    double getMinimumValue() const;
    double getMaximumValue() const;
    double getMinimumRange() const;
    double getMaximumRange() const;
    int32_t getSamplingValue() const;
    std::string getFileNameData() const;
    std::string getFileNameVolume() const;
    std::string getDataType() const;

    void setRenderingOptions( double _minimumValue,
                              double _maximumValue,
                              double _minimumRange,
                              double _maximumRange,
                              int32_t _samplingValue,
                              std::string& _dataType,
                              std::string& _fileNameData,
                              std::string& _fileNameVolume );

  protected:

#if ( ( ANATOMIST_VERSION_MAJOR == 5 ) && \
      ( ANATOMIST_VERSION_MINOR >= 1 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )

    carto::Volume< AimsRGBA > _palette;

#else

    AimsData<AimsRGBA>* _palette;

#endif

    void clear();
    void clearDataValues();
    void draw() const;

    void updateShaderCode();
    void setShaderAttributes();

    void getFiberCoordinates();
    void updateDataValues();

    BundleMap< int16_t >& _bundleMap;

    std::string _dataType;
    anatomist::GLList* _glList;

    // shader attributes
    QGLShaderProgram* _qGLShaderProgram;
    QGLShader* _qGLShader;

    int32_t _uniformIsShaderedObject;
    int32_t _uniformPointCount;

    int32_t _uniformLocationCoordinates;
    int32_t _uniformLocationDataValues;

    BoundingBox< float > _boundingBox;

    std::vector< QVector3D* > _coordinates;
    std::vector< int32_t > _fiberCount;
    int32_t _samplingValue;
    std::vector< QVector3D* > _dataValues;

    double _minimumValue;
    double _maximumValue;
    double _minimumRange;
    double _maximumRange;

};


}


#endif
