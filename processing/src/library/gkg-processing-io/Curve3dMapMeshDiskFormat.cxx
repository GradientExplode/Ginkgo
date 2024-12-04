#include <gkg-processing-io/Curve3dMapMeshDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


//
// disk format singleton instanciation
//

template class gkg::Curve3dMapMeshDiskFormat< float >;
template class gkg::Curve3dMapMeshDiskFormat< double >;


//
// registrating standard Curve3d Mesh disk format(s) for "Curve3d" object 
// factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Curve3dMap< float > 
                         >::getInstance().registerDiskFormat(
         gkg::Curve3dMapMeshDiskFormat< float >::getInstance().getName(),
         &gkg::Curve3dMapMeshDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Curve3dMap< double > 
                         >::getInstance().registerDiskFormat(
         gkg::Curve3dMapMeshDiskFormat< double >::getInstance().getName(),
         &gkg::Curve3dMapMeshDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Curve3d Map Mesh disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
