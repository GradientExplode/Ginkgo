#ifndef _gkg_anatomist_plugin_objects_ARealTimeOdfFieldObject_h_
#define _gkg_anatomist_plugin_objects_ARealTimeOdfFieldObject_h_


#include <gkg-processing-container/CartesianField.h>
#include <gkg-processing-mesh/MeshDeformation.h>

#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <graph/tree/tree.h>
#include <QObject>
#include <QtOpenGL>
#include <QGLShader>
#include <QGLShaderProgram>


namespace gkg
{


class OrientationSet;


class ARealTimeOdfFieldObject : public QObject, 
                                public anatomist::SliceableObject
{

  public:

    ARealTimeOdfFieldObject( 
                           CartesianField< MeshDeformation< float > >& odfField,
                           OrientationSet& orientationSet,
                           float meshScale,
                           bool anatomistStandardMode = true );
    virtual ~ARealTimeOdfFieldObject();

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

    bool boundingBox( std::vector<float>& bmin,
                      std::vector<float>& bmax ) const;
    Point3df VoxelSize() const;

    bool render( anatomist::PrimList& primitiveList,
                 const anatomist::ViewState& viewState );
    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    static int32_t registerClass();

    static int32_t _classType;
    static Tree* _optionTree;

    static int32_t getClassType();

  protected:

    void getSphereVertexCoordinatesAndIndices();
    void drawOrthogonalPlane( const Vector3d< int32_t >& begin,
                              const Vector3d< int32_t >& end ) const;
    void drawTransversalPlane(
                           const anatomist::SliceViewState* sliceVState ) const;
    void updateShaderCode();
    void setShaderAttributes();
    void getApproximatedNormals();

    CartesianField< MeshDeformation< float > >& _odfCartesianField;
    OrientationSet& _orientationSet;
    int32_t _outputOrientationCount;
    float _meshScale;
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

    int32_t _uniformIsShaderedObject;
    int32_t _attributeLocationMeshScale;
    int32_t _attributeLocationCurrentVertexIndices;
    int32_t _attributeLocationSecondAndThirdVertexIndices;
    int32_t _attributeLocationSecondVertexCoordinates;
    int32_t _attributeLocationThirdVertexCoordinates;

    QVector< GLfloat > _vertexCoordinates;
    QVector< GLuint > _vertexIndices;

};


}


#endif
