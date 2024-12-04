#ifndef _gkg_anatomist_plugin_objects_AOdfFieldObject_h_
#define _gkg_anatomist_plugin_objects_AOdfFieldObject_h_


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
#include <gkg-dmri-container/OdfCartesianField.h>

#include <gkg-communication-thread/LoopContext.h>

#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>
#include <QGLShader>
#include <QGLShaderProgram>


namespace gkg
{



class OdfNormalComputationContext : public LoopContext< int64_t >
{

  public:

    OdfNormalComputationContext(
             const std::vector< Vector3d< int32_t > >& sites,
             const CartesianField< std::vector< float > >& odfTransformations,
             const MeshMap< int32_t, float, 3U >& unitSphere,
             std::vector< std::vector< QVector3D > >& normalSets );
    virtual ~OdfNormalComputationContext();

    void doIt( int64_t startIndex, int64_t countIndex );

  protected:

    const std::vector< Vector3d< int32_t > >& _sites;
    const CartesianField< std::vector< float > >& _odfTransformations;
    const MeshMap< int32_t, float, 3U >& _unitSphere;
    std::vector< std::vector< QVector3D > >& _normalSets;

};



class AOdfFieldObject : public QObject,
                        public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    AOdfFieldObject(
      const RCPointer< OdfCartesianField >& odfCartesianField );
    virtual ~AOdfFieldObject();

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

    int32_t getVertexCountPerOdf() const;
    void setVertexCountPerOdf( int32_t vertexCount );

    float getMeshScale() const;
    void setMeshScale( float meshScale );

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


    void computeOdfTranslations();
    void computeOdfProbabilities();

    void computeOdfTransformations();
    void computeOdfNormals();

    std::string getShaderSourceCode() const;
    void updateShader();

    RCPointer< OdfCartesianField > _odfCartesianField;
    int32_t _vertexCountPerOdf;
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

    RCPointer< CartesianField< QVector4D > > _odfTranslations;
    RCPointer< CartesianField< std::vector< float > > > _odfProbabilities;
    RCPointer< CartesianField< std::vector< float > > > _odfTransformations;
    RCPointer< CartesianField< std::vector< QVector3D > > > _odfNormals;

    RCPointer< QGLShaderProgram > _shaderProgram;
    RCPointer< QGLShader > _shader;

    int32_t _attributeLocationOpacity;
    int32_t _attributeLocationMeshScale;
    int32_t _uniformIsShaderedObject;
    int32_t _uniformLocationTranslation;
    int32_t _uniformLocationNormals;
    int32_t _uniformLocationTransformations;
    int32_t _attributeLocationUnitSphereVertexIndex;

    std::vector< GLfloat > _unitSphereVertexIndices;

    std::vector< GLfloat > _unitSphereVertexCoordinates;
    std::vector< GLuint > _unitSpherePolygonIndices;

    anatomist::GLList* _sphereList;

};


}


#endif
