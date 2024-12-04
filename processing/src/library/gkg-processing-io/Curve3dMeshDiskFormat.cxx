#include <gkg-processing-io/Curve3dMeshDiskFormat_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>


//
// disk format singleton instanciation
//

template class gkg::Curve3dMeshDiskFormat< float >;
template class gkg::Curve3dMeshDiskFormat< double >;


//
// registrating standard Curve3d Mesh disk format(s) for "Curve3d" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::Curve3d< float > 
                         >::getInstance().registerDiskFormat(
         gkg::Curve3dMeshDiskFormat< float >::getInstance().getName(),
         &gkg::Curve3dMeshDiskFormat< float >::getInstance() );
    gkg::DiskFormatFactory< gkg::Curve3d< double > 
                         >::getInstance().registerDiskFormat(
         gkg::Curve3dMeshDiskFormat< double >::getInstance().getName(),
         &gkg::Curve3dMeshDiskFormat< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Curve3d Mesh disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
