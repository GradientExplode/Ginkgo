#ifndef _gkg_anatomist_plugin_objects_AMedusaGeometryObject_h_
#define _gkg_anatomist_plugin_objects_AMedusaGeometryObject_h_


#include <anatomist/object/sliceable.h>
#include <anatomist/primitive/primitive.h>
#include <anatomist/object/oReader.h>
#include <graph/tree/tree.h>

#include <gkg-core-pattern/RCPointer.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-virtual-tissue/SphereAtom.h>
#include <gkg-simulation-virtual-tissue/EllipsoidAtom.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <vector>
#include <string>

#include <QObject>
#include <QtOpenGL>


namespace gkg
{


class AMedusaGeometryObject : public QObject,
                              public anatomist::SliceableObject
{

  public:

    friend class AnaGkgModule;

    AMedusaGeometryObject(
      const RCPointer< VirtualTissue >& virtualTissue,
      const RCPointer< std::vector< RGBComponent > >& populationColors );
    virtual ~AMedusaGeometryObject();

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

    int32_t getVertexCountPerSphereAtom() const;
    void setVertexCountPerSphereAtom( int32_t vertexCount );

    bool boundingBox( std::vector< float > & bmin,
                      std::vector< float > & bmax ) const;

    bool getFiberPopulationVisibility() const;
    void setFiberPopulationVisibility( bool value );

    bool getSomaPopulationVisibility() const;
    void setSomaPopulationVisibility( bool value );

    bool getNeuronPopulationVisibility() const;
    void setNeuronPopulationVisibility( bool value );

    bool getDendritePopulationVisibility() const;
    void setDendritePopulationVisibility( bool value );

    bool getAstrocytePopulationVisibility() const;
    void setAstrocytePopulationVisibility( bool value );

    bool getOligodendrocytePopulationVisibility() const;
    void setOligodendrocytePopulationVisibility( bool value );

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

    RCPointer< VirtualTissue > _virtualTissue;
    RCPointer< std::vector< RGBComponent > > _populationColors;
    BoundingBox< float > _boundingBox;

    int32_t _vertexCountPerSphereAtom;
    bool _fiberPopulationVisibility;
    bool _somaPopulationVisibility;
    bool _neuronPopulationVisibility;
    bool _dendritePopulationVisibility;
    bool _astrocytePopulationVisibility;
    bool _oligodendrocytePopulationVisibility;

};


}


#endif
