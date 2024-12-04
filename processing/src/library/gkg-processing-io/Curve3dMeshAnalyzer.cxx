#include <gkg-processing-io/Curve3dMeshAnalyzer_i.h>
#include <gkg-core-exception/Exception.h>


template class gkg::Curve3dMeshAnalyzer< float >;
template class gkg::Curve3dMeshAnalyzer< double >;


//
// registrating standard GIS analyzer
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::Curve3dMeshDiskFormat< float >::getInstance().getName(),
         &gkg::Curve3dMeshAnalyzer< float >::getInstance() );
    gkg::DiskFormatAnalyzer::getInstance().registerAnalyzer(
         gkg::Curve3dMeshDiskFormat< double >::getInstance().getName(),
         &gkg::Curve3dMeshAnalyzer< double >::getInstance() );

    return true;

  }
  GKG_CATCH( "Curve3d Mesh analyzer registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
