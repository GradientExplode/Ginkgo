#include <gkg-processing-io/Curve3dMapMeshAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>


template class gkg::Curve3dMapMeshAnalyzer< float >;
template class gkg::Curve3dMapMeshAnalyzer< double >;


//
// registrating analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::Curve3dMeshDiskFormat< float >::getInstance().getName(),
         &gkg::Curve3dMapMeshAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::Curve3dMeshDiskFormat< double >::getInstance().getName(),
         &gkg::Curve3dMapMeshAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Curve3d Map Mesh analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
