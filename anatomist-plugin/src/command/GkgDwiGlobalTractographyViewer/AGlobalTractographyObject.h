#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AGlobalTractographyObject_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AGlobalTractographyObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/control/qObjTree.h>
#include <graph/tree/tree.h>

#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <QObject>
#include <QtOpenGL>


namespace gkg
{


class AGlobalTractographyObject : public QObject,
                                  public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    AGlobalTractographyObject( 
      const RCPointer< SpinGlassAndConnectionMap >& spinGlassAndConnectionMap );
    virtual ~AGlobalTractographyObject();

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

    bool boundingBox( Point3df& lowerCorner, Point3df& upperCorner ) const;

    bool glMakeBodyGLL( const anatomist::ViewState& viewState,
                        const anatomist::GLList& glList ) const;

    // static methods
    static int32_t registerClass();
    static int32_t getClassType();

    // static public attributes
    static int32_t _classType;
    static Tree* _optionTree;

  protected:

    RCPointer< SpinGlassAndConnectionMap > _spinGlassAndConnectionMap;

};


}


#endif
