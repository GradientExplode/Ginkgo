#ifndef _gkg_anatomist_plugin_objects_APointCloudFieldObject_h_
#define _gkg_anatomist_plugin_objects_APointCloudFieldObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-core-pattern/RCPointer.h>

#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/SiteMap.h>
#include <gkg-processing-container/TextureMap.h>
#include <gkg-processing-container/MeshMap.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-processing-container/CartesianField.h>


#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>
#include <QGLShader>
#include <QGLShaderProgram>


namespace gkg
{


class APointCloudFieldObject : public QObject,
                               public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    APointCloudFieldObject(
      const RCPointer< CartesianField< std::vector< Vector3d< float > > > >&
                                                     pointCloudCartesianField );
    virtual ~APointCloudFieldObject();

    bool Is2DObject();
    bool Is3DObject();
    bool IsFusion2DAllowed();

    // tree menu specific to the object
    Tree* optionTree() const;

    // bounding box information
    float MinX2D() const;
    float MinY2D() const;
    float MinZ2D() const;
    float MaxX2D() const;
    float MaxY2D() const;
    float MaxZ2D() const;

    int32_t getVertexCountPerSphere() const;
    void setVertexCountPerSphere( int32_t vertexCount );

    float getMeshScale() const;
    void setMeshScale( float meshScale );

    float getPointRadius() const;
    void setPointRadius( float pointRadius );

    bool boundingBox( std::vector< float > & bmin,
                      std::vector< float > & bmax ) const;

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


    void computePointCloudTranslations();

    std::string getShaderSourceCode() const;
    void updateShader();

    RCPointer< CartesianField< std::vector< Vector3d< float > > > >
                                                      _pointCloudCartesianField;
    int32_t _vertexCountPerSphere;
    float _pointRadius;
    float _meshScale;
    bool _isAlreadyDisplayed;
    Vector3d< int32_t > _size;
    Vector3d< double > _resolution;
    double _halfMinimumResolution;
    Vector3d< int32_t > _offset;
    float _scaling;
    BoundingBox< float > _boundingBox;

    RCPointer< OrientationSet > _outputOrientationSet;
    RCPointer< MeshMap< int32_t, float, 3U > > _unitSphere;

    RCPointer< CartesianField< QVector4D > > _pointCloudTranslations;

    RCPointer< QGLShaderProgram > _shaderProgram;
    RCPointer< QGLShader > _shader;

    int32_t _attributeLocationRed;
    int32_t _attributeLocationGreen;
    int32_t _attributeLocationBlue;
    int32_t _attributeLocationOpacity;
    int32_t _attributeLocationPointRadius;
    int32_t _attributeLocationMeshScale;
    int32_t _uniformIsShaderedObject;
    int32_t _uniformLocationTranslation;
    int32_t _uniformLocationPoint;
    int32_t _uniformLocationNormals;
    int32_t _attributeLocationUnitSphereVertexIndex;

    std::vector< GLfloat > _unitSphereVertexIndices;

    std::vector< GLfloat > _unitSphereVertexCoordinates;
    std::vector< QVector3D > _unitSphereVertexNormals;
    std::vector< GLuint > _unitSpherePolygonIndices;

    anatomist::GLList* _sphereList;

};


}


#endif
