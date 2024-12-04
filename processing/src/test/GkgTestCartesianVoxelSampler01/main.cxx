#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-coordinates/CartesianVoxelSampler.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{


  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // case of 27 point(s) with (1.0, 1.0, 1.0) voxel resolution
    
    std::cout << std::endl
              << "case of 27 point(s) with (1.0, 1.0, 1.0) voxel resolution"
              << std::endl
              << "========================================================="
              << std::endl;
    gkg::CartesianVoxelSampler< float >
      voxelSampler27_1_1_1( gkg::Vector3d< double >( 1.0, 1.0, 1.0 ), 27 );

    int32_t offsetIndex = 0;
    gkg::Vector3d< float > origin( 0.0, 0.0, 0.0 );
    gkg::Vector3d< float > destination( 0.0, 0.0, 0.0 );
    for ( offsetIndex = 0; offsetIndex < voxelSampler27_1_1_1.getPointCount();
          offsetIndex++ )
    {

      voxelSampler27_1_1_1.getPoint( origin, 
                                     offsetIndex,
                                     destination );
      std::cout << destination << std::endl;

    }

    // case of 27 point(s) with (2.0, 1.0, 3.0) voxel resolution
    std::cout << std::endl
              << "case of 27 point(s) with (2.0, 1.0, 3.0) voxel resolution"
              << std::endl
              << "========================================================="
              << std::endl;
    gkg::CartesianVoxelSampler< float >
      voxelSampler27_2_1_3( gkg::Vector3d< double >( 2.0, 1.0, 3.0 ), 27 );


    for ( offsetIndex = 0; offsetIndex < voxelSampler27_2_1_3.getPointCount();
          offsetIndex++ )
    {

      voxelSampler27_2_1_3.getPoint( origin, 
                                     offsetIndex,
                                     destination );
      std::cout << destination << std::endl;

    }


    // case of 53 point(s) with (1.0, 1.0, 1.0) voxel resolution
    std::cout << std::endl
              << "case of 53 point(s) with (1.0, 1.0, 1.0) voxel resolution"
              << std::endl
              << "========================================================="
              << std::endl;
    gkg::CartesianVoxelSampler< float >
      voxelSampler53_1_1_1( gkg::Vector3d< double >( 1.0, 1.0, 1.0 ), 53 );

    for ( offsetIndex = 0; offsetIndex < voxelSampler53_1_1_1.getPointCount();
          offsetIndex++ )
    {

      voxelSampler53_1_1_1.getPoint( origin, 
                                     offsetIndex,
                                     destination );
      std::cout << destination << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;


}
