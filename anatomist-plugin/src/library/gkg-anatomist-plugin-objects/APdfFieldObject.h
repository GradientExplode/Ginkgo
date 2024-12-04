#ifndef _gkg_anatomist_plugin_objects_APdfFieldObject_h_
#define _gkg_anatomist_plugin_objects_APdfFieldObject_h_


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


namespace gkg
{


class APdfFieldReader;


class APdfFieldObject : public QObject, public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    APdfFieldObject( const std::string& fileNameSiteMap,
                     const std::string& fileNameTextureMap );
    virtual ~APdfFieldObject();

    bool Is2DObject();
    bool Is3DObject();

    float getMeshScale() const;
    float getDisplacementMagnitude() const;
    int32_t getOutputOrientationCount() const;
    int32_t getResamplingNeighborCount() const;
    float getResamplingGaussianSigma() const;
    bool isPolarHarmonics() const;
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

    bool boundingBox( std::vector<float> & bmin,
                      std::vector<float> & bmax ) const;
    Point3df VoxelSize() const;

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    void setShadowOptions( bool hasHiddenShadows, bool hasApproximatedNormals );
    void setMeshScale( float meshScale );
    void setDisplacementMagnitude( float displacementMagnitude );
    void setResamplingNeighborCount( int32_t resamplingNeighborCount );
    void setResamplingGaussianSigma( float resamplingGaussianSigma );
    void setRenderingOptions( bool hideShadows,
                              float displacementMagnitude,
                              int32_t outputOrientationCount,
                              int32_t resamplingNeighborCount,
                              float resamplingGaussianSigma,
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
                                    anatomist::ObjectReader::PostRegisterList &,
                                    carto::Object options );

  protected:

    void loadReader();
    void deleteReader();
    void drawOrthogonalPlane( const Vector3d< int32_t >& begin,
                              const Vector3d< int32_t >& end ) const;
    void drawOrthogonalPlaneWithExactNormals( 
                              const Vector3d< int32_t >& begin,
                              const Vector3d< int32_t >& end ) const;
    void drawTransversalPlane(
                           const anatomist::SliceViewState* sliceVState ) const;
    void drawTransversalPlaneWithExactNormals(
                           const anatomist::SliceViewState* sliceVState ) const;
    void updateShaderCode();
    void setShaderAttributes();
    void getApproximatedNormals();

    std::string _fileNameSiteMap;
    std::string _fileNameTextureMap;
    int32_t _outputOrientationCount;
    int32_t _resamplingNeighborCount;
    float _resamplingGaussianSigma;
    float _meshScale;
    float _displacementMagnitude;
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

    APdfFieldReader* _aPdfFieldReader;

    std::vector< std::vector< std::vector< QVector4D  > > > _translations;
    std::vector< std::vector< std::vector< GLfloat*  > > > _transformations;
    std::vector < std::vector < std::vector < QVector3D* > > > _normals;

    QVector< GLfloat > _vertexCoordinates;
    QVector< GLuint > _vertexIndices;

    Vector3d< double > _resolution;

    bool _isPolarHarmonics;

};


}


#endif
