#ifndef _gkg_dmri_container_SpinGlassAndConnectionMap_h_
#define _gkg_dmri_container_SpinGlassAndConnectionMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>
#include <list>


namespace gkg
{


class AnatomicalPriors;


class SpinGlassAndConnectionMap : public HeaderedObject
{

  public:

    SpinGlassAndConnectionMap();
    SpinGlassAndConnectionMap( const SpinGlassAndConnectionMap& other );
    virtual ~SpinGlassAndConnectionMap();

    SpinGlassAndConnectionMap& operator=(
                                       const SpinGlassAndConnectionMap& other );

    void reset();

    void setBoundingBox( const Vector3d< int32_t >& lowerCorner,
                         const Vector3d< int32_t >& upperCorner,
                         const Vector3d< double >& roiResolution );
    void setRoiResolution( const Vector3d< double >& roiResolution );
    void setCurrentSpinGlass( uint64_t spinGlassIdentity );

    void addSpinGlass( uint64_t spinGlassIdentity,
                       const Vector3d< float >& spinGlassPosition,
                       const Vector3d< float >& spinGlassOrientation,
                       float spinGlassLength );
    void removeSpinGlass( uint64_t spinGlassIdentity );

    void moveSpinGlass( uint64_t spinGlassIdentity,
                        const Vector3d< float >& newSpinGlassPosition,
                        const Vector3d< float >& newSpinGlassOrientation );

    void addSpinGlassConnection(
                             uint64_t spinGlassConnectionIdentity,
                             uint64_t spinGlass1Identity,
                             SpinGlass::ExtremityType spinGlassExtremityType1,
                             uint64_t spinGlass2Identity,
                             SpinGlass::ExtremityType spinGlassExtremityType2 );
    void removeSpinGlassConnection( uint64_t spinGlassConnectionIdentity );

    // accessors for spin glasses, spin glass connections and bounding box
    const std::vector< RCPointer< SpinGlass > >& getSpinGlasses() const;
    const std::vector< RCPointer< SpinGlassConnection > >&
                                                getSpinGlassConnections() const;
    const BoundingBox< float >& getBoundingBox() const;
    const Vector3d< double >& getRoiResolution() const;
    const RCPointer< SpinGlass >& getCurrentSpinGlass() const;

    float getLowerX() const;
    float getLowerY() const;
    float getLowerZ() const;
    float getUpperX() const;
    float getUpperY() const;
    float getUpperZ() const;

    // collecting valid spin glasses and their connections
    int32_t getValidSpinGlassCount() const;
    void getValidSpinGlasses(
                       std::list< RCPointer< SpinGlass > >& spinGlasses ) const;

    int32_t getValidSpinGlassConnectionCount() const;
    void getValidSpinGlassConnections(
                       std::list< RCPointer< SpinGlassConnection > >&
                                                   spinGlassConnections ) const;

  protected:

    std::vector< RCPointer< SpinGlass > > _spinGlasses;
    std::vector< RCPointer< SpinGlassConnection > > _spinGlassConnections;

    BoundingBox< float > _boundingBox; // really useful?

    Vector3d< double > _roiResolution;
    uint64_t _spinGlassIdentity;

};


}



#endif

