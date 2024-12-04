#ifndef _gkg_anatomist_plugin_objects_APointCloudObject_h_
#define _gkg_anatomist_plugin_objects_APointCloudObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>

#include <list>
#include <string>

#include <QObject>
#include <QtOpenGL>


namespace gkg
{


class APointCloudObject : public QObject,
                              public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    APointCloudObject(
      const RCPointer< std::list< Vector3d< float > > >& points,
      const RCPointer< std::list< Vector3d< float > > >& colors );
    virtual ~APointCloudObject();

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

    int32_t getSphereVertexCount() const;
    void setSphereVertexCount( int32_t vertexCount );

    float getSphereRadius() const;
    void setSphereRadius( float radius );

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

    RCPointer< std::list< Vector3d< float > > > _points;
    RCPointer< std::list< Vector3d< float > > > _colors;
    BoundingBox< float > _boundingBox;

    int32_t _sphereVertexCount;
    float _sphereRadius;

};


}


#endif
