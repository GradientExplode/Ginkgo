#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyService_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_GlobalTractographyService_h_


#include <gkg-communication-socket/SocketService.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-dmri-global-tractography/SpinGlassConnection.h>
#include <gkg-processing-container/BoundingBox.h>
#include <list>


namespace gkg
{


class GlobalTractographyViewer;
class GlobalTractographyServiceThread;
class RpcCommandReader;
class SpinGlassAndConnectionMap;
class GlobalTractographyOptimizationInformation;


class GlobalTractographyService : public SocketService
{

  public:

    GlobalTractographyService( 
                           GlobalTractographyViewer* globalTractographyViewer );
    virtual ~GlobalTractographyService();

    // Posting an event
    void postEvent( int32_t eventId );

    // Creation of a spin glass
    void createSpinGlass( uint64_t spinGlassIdentity,
                          const Vector3d< float >& spinGlassPosition,
                          const Vector3d< float >& spinGlassOrientation );

    // Deletion of a spin glass
    void deleteSpinGlass( uint64_t spinGlassIdentity );

    // Move of a spin glass
    void moveSpinGlass( uint64_t spinGlassIdentity,
                        const Vector3d< float >& newSpinGlassPosition,
                        const Vector3d< float >& newSpinGlassOrientation );

    // Creation of a spin glass connection
    void createSpinGlassConnection(
                             uint64_t spinGlassConnectionIdentity,
                             uint64_t spinGlass1Identity,
                             SpinGlass::ExtremityType spinGlassExtremityType1,
                             uint64_t spinGlass2Identity,
                             SpinGlass::ExtremityType spinGlassExtremityType2 );

    // Deletion of a spin glass connection
    void deleteSpinGlassConnection( uint64_t spinGlassConnectionIdentity );

    // Accessor to the spin glass and connection map
    const RCPointer< SpinGlassAndConnectionMap >&
                                           getSpinGlassAndConnectionMap() const;

    // Accessor to the global tractography optimization information
    const RCPointer< GlobalTractographyOptimizationInformation >&
                           getGlobalTractographyOptimizationInformation() const;

  protected:

    void createReader( int32_t fd );

    GlobalTractographyViewer* _globalTractographyViewer;

    RCPointer< SpinGlassAndConnectionMap > _spinGlassAndConnectionMap;
    RCPointer< GlobalTractographyOptimizationInformation >
                                     _globalTractographyOptimizationInformation;

    GlobalTractographyServiceThread* _globalTractographyServiceThread;
    RCPointer< RpcCommandReader > _rpcCommandReader;

};


}


#endif

