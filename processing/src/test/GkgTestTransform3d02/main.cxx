#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Referential.h>
#include <gkg-processing-transform/Transform3dManager.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );


    {

      std::cout << "rigid transform 3D (direct):" << std::endl;
      std::cout << "===========================" << std::endl;
      std::vector< float > parameters( 6 );
      
      parameters[ 0 ] = 10;
      parameters[ 1 ] = -25;
      parameters[ 2 ] = 18;
      parameters[ 3 ] = 5;
      parameters[ 4 ] = -27;
      parameters[ 5 ] = 59;
      gkg::RigidTransform3d< float > rigidTransform3d( parameters,
                                                       true, true ); 

      gkg::Vector3d< float > from( 1.0, 2.0, 3.0 ), to;
    
      rigidTransform3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rigidTransform3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rigid transform 3D (inverse):" << std::endl;
      std::cout << "============================" << std::endl;
      std::vector< float > parameters( 6 );
      
      parameters[ 0 ] = 10;
      parameters[ 1 ] = -25;
      parameters[ 2 ] = 18;
      parameters[ 3 ] = 5;
      parameters[ 4 ] = -27;
      parameters[ 5 ] = 59;
      gkg::RigidTransform3d< float > rigidTransform3d( parameters,
                                                       true, false ); 

      gkg::Vector3d< float > from( 4.08721, -25.7734, 62.415 ), to;
    
      rigidTransform3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rigidTransform3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    // in fact, 'gsl' library is the default; so the following line is not
    // mandatory: it is only for didactic purpose
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );


    {

      std::cout << "rigid transform 3D (Rodrigues):" << std::endl;
      std::cout << "==============================" << std::endl;
      std::vector< float > parameters( 6 );
      
      parameters[ 0 ] = 10;
      parameters[ 1 ] = 0;
      parameters[ 2 ] = 0;
      parameters[ 3 ] = 2;
      parameters[ 4 ] = 3;
      parameters[ 5 ] = 0;
      gkg::RigidTransform3d< float > rigidTransform3d( parameters,
                                                       true, true ); 

      float theta = 0.0;
      gkg::Matrix Omega;
    
      rigidTransform3d.getRotation3d().getRodriguesParameters( theta,
                                                               Omega,
                                                               true );

      std::cout << "theta = " << theta << " degrees" << std::endl;

      std::cout << "Omega = " << std::endl
                << Omega << std::endl;

      std::cout << "norm2 = " << rigidTransform3d.getNorm2() << std::endl;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
