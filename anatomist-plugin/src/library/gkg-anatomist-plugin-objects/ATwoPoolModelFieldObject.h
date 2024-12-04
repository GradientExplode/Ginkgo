#ifndef _gkg_anatomist_plugin_objects_ATwoPoolModelFieldObject_h_
#define _gkg_anatomist_plugin_objects_ATwoPoolModelFieldObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <anatomist/config/version.h>
#include <graph/tree/tree.h>
#include <QObject>
#include <QtOpenGL>
#include <QGLShader>
#include <QGLShaderProgram>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-pdf-two-pool-model/TwoPoolModel.h>


namespace gkg
{


class ATwoPoolModelFieldReader;


class ATwoPoolModelFieldObject : public QObject,
                                 public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    ATwoPoolModelFieldObject( const std::string& fileNameSiteMap,
                              const std::string& fileNameTextureMap );
    virtual ~ATwoPoolModelFieldObject();

    bool Is2DObject();
    bool Is3DObject();

    float getMeshScale() const;
    int32_t getOutputOrientationCount() const;
    bool hasApproximatedNormals() const;
    bool hasHiddenShadows() const;
    bool areRealNormalsAlreadyComputed() const;

    Tree* optionTree() const;

    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;
    float MinT() const;
    float MaxT() const;

    bool boundingBox( std::vector<float>& bmin,
                      std::vector<float>& bmax ) const;
    Point3df VoxelSize() const;

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    void setShadowOptions( bool hasHiddenShadows, bool hasApproximatedNormals );
    void setMeshScale( float meshScale );
    void setRenderingOptions( bool hideShadows,
                              int32_t outputOrientationCount,
                              bool hasApproximatedNormals );

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
                                    anatomist::ObjectReader::PostRegisterList&,
                                    carto::Object options );

  protected:

    void loadReader();
    void deleteReader();
    void drawOrthogonalPlane( const gkg::Vector3d< int32_t >& begin,
                              const gkg::Vector3d< int32_t >& end,
                              TwoPoolModel::Pool pool  ) const;
    void drawOrthogonalPlaneWithExactNormals( 
                                         const gkg::Vector3d< int32_t >& begin,
                                         const gkg::Vector3d< int32_t >& end,
                                         TwoPoolModel::Pool pool ) const;
    void drawTransversalPlane( const anatomist::SliceViewState* sliceVState,
                               TwoPoolModel::Pool pool ) const;
    void drawTransversalPlaneWithExactNormals(
                               const anatomist::SliceViewState* sliceVState,
                               TwoPoolModel::Pool pool ) const;
    void updateShaderCode();
    void setShaderAttributes();
    void getApproximatedNormals();

    std::string _fileNameSiteMap;
    std::string _fileNameTextureMap;
    int32_t _outputOrientationCount;
    float _meshScale;
    bool _hasApproximatedNormals;
    bool _hasHiddenShadows;
    bool _isDisplayed;
    anatomist::GLList* _glList;

    // shader attributes
    QGLShaderProgram* _qGLShaderProgram;
    QGLShader* _qGLShader;

    GLfloat* _currentVertexIndices;
    QVector2D* _secondAndThirdVertexIndices;

    QVector3D* _secondVertexCoordinates;
    QVector3D* _thirdVertexCoordinates;

    int32_t _uniformLocationTranslation;
    int32_t _uniformLocationTransformation;
    int32_t _uniformLocationNormal;

    int32_t _uniformIsShaderedObject;

    int32_t _attributeLocationMeshScale;

    int32_t _attributeLocationCurrentVertexIndices;
    int32_t _attributeLocationSecondAndThirdVertexIndices;

    int32_t _attributeLocationSecondVertexCoordinates;
    int32_t _attributeLocationThirdVertexCoordinates;

    BoundingBox< float > _boundingBox;

    bool _approximatedNormalsAlreadyComputed;
    bool _realNormalsAlreadyComputed;

    ATwoPoolModelFieldReader* _aTwoPoolModelFieldReader;

    std::vector< std::vector< std::vector< QVector4D > > > _translations;
    std::vector< std::vector< std::vector< GLfloat* > > > _fastTransformations;
    std::vector< std::vector< std::vector< GLfloat* > > > _slowTransformations;
    std::vector< std::vector< std::vector< QVector3D* > > > _fastNormals;
    std::vector< std::vector< std::vector< QVector3D* > > > _slowNormals;

    QVector< GLfloat > _vertexCoordinates;
    QVector< GLuint > _vertexIndices;

    Vector3d< double > _resolution;

};


}


#endif
