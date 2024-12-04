#ifndef _gkg_anatomist_plugin_objects_AOdfFieldObject_h_
#define _gkg_anatomist_plugin_objects_AOdfFieldObject_h_


#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <anatomist/config/version.h>
#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <anatomist/color/paletteList.h>
#include <anatomist/color/objectPalette.h>
#include <anatomist/color/palette.h>
#include <anatomist/application/Anatomist.h>
#include <anatomist/config/version.h>
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


class AFiberReader;


class AFiberObject : public QObject, public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    AFiberObject( const std::string& fileNameBundleMap );
    virtual ~AFiberObject();

    bool Is2DObject();
    bool Is3DObject();

    Tree* optionTree() const;

    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;

    bool boundingBox( std::vector<float> & bmin,
                      std::vector<float> & bmax ) const;

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;
    
    static int32_t registerClass();

    static int32_t _classType;
    static Tree* _optionTree;

    static int32_t getClassType();

#if ( ANATOMIST_VERSION_MAJOR >= 5 ) || \
    ( ( ANATOMIST_VERSION_MAJOR == 4 ) && \
      ( ANATOMIST_VERSION_MINOR >= 6 ) && \
      ( ANATOMIST_VERSION_TINY >= 0 ) )
    static std::list< anatomist::AObject* > load( 
#else
    static anatomist::AObject* load( 
#endif
                             const std::string& fileName,
                             anatomist::ObjectReader::PostRegisterList& regList,
                             carto::Object options );

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

    void loadReader();
    void deleteReader();
    void clearReader();
    void draw() const;

    void updateShaderCode();
    void setShaderAttributes();

    void updateDataValues();

    std::string _fileNameBundleMap;
    std::string _dataType;
    bool _isOneValuePerBundleShaderType;
    std::string _fileNameData;
    std::string _fileNameVolume;
    bool _isDisplayed;
    anatomist::GLList* _glList;

    // shader attributes
    QGLShaderProgram* _qGLShaderProgram;
    QGLShader* _qGLShader;

    int32_t _uniformIsShaderedObject;
    int32_t _uniformPointCount;

    int32_t _uniformLocationCoordinates;
    int32_t _uniformLocationDataValues;

    BoundingBox< float > _boundingBox;

    AFiberReader* _aFiberReader;

    std::vector< QVector3D* > _coordinates;
    std::vector< int32_t > _fiberCount;
    int32_t _samplingValue;
    std::vector< QVector3D* > _dataValues;
    std::vector< std::pair< std::string, int32_t > > _bundleNames;

    double _minimumValue;
    double _maximumValue;
    double _minimumRange;
    double _maximumRange;

};


}


#endif
