#include <gkg-processing-transform/Translation3d.h>
#include <gkg-processing-transform/RotationX3d.h>
#include <gkg-processing-transform/RotationY3d.h>
#include <gkg-processing-transform/RotationZ3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-processing-transform/Rotation3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/Referential.h>
#include <gkg-processing-transform/Transform3dManager.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    {

      std::cout << "translation 3D :" << std::endl;
      std::cout << "================" << std::endl;
      gkg::Translation3d< float > translation3d( 1.0, 2.0, 3.0, true );
      gkg::Vector3d< float > from( 3.0, 4.0, 5.0 ), to;

      translation3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      translation3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rotation X 3D :" << std::endl;
      std::cout << "===============" << std::endl;
      gkg::RotationX3d< float > rotationX3d( 45.0, true, true );
      gkg::Vector3d< float > from( 0.0, 1.0, 0.0 ), to;

      rotationX3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rotationX3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rotation Y 3D :" << std::endl;
      std::cout << "===============" << std::endl;
      gkg::RotationY3d< float > rotationY3d( 45.0, true, true );
      gkg::Vector3d< float > from( 1.0, 0.0, 0.0 ), to;

      rotationY3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rotationY3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rotation Z 3D :" << std::endl;
      std::cout << "===============" << std::endl;
      gkg::RotationZ3d< float > rotationZ3d( 45.0, true, true );
      gkg::Vector3d< float > from( 1.0, 0.0, 0.0 ), to;

      rotationZ3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rotationZ3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "scaling 3D :" << std::endl;
      std::cout << "=============" << std::endl;
      gkg::Scaling3d< float > scaling3d( 1.0, 2.0, 3.0, true );
      gkg::Vector3d< float > from( 1.0, 2.0, 3.0 ), to;

      scaling3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      scaling3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rotation 3D :" << std::endl;
      std::cout << "=============" << std::endl;
      gkg::Rotation3d< float > rotation3d( 0.0, 0.0, 45.0, true, true );
      gkg::Vector3d< float > from( 1.0, 0.0, 0.0 ), to;

      rotation3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rotation3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "rigid transform 3D :" << std::endl;
      std::cout << "====================" << std::endl;
      gkg::RigidTransform3d< float > rigidTransform3d( 0.0, 0.0, 90.0,
                                                       10.0, 0.0, 0.0,
                                                       true, true );
      gkg::Vector3d< float > from( 1.0, 0.0, 0.0 ), to;

      rigidTransform3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rigidTransform3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

      rigidTransform3d.setDirectParameters( 0.0, 0.0, 45.0,
                                            0.0, 10.0, 0.0,
                                            true, true, true );

      rigidTransform3d.getDirect( from, to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      rigidTransform3d.getInverse( to, from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

    }

    {

      std::cout << "transform 3D manager:" << std::endl;
      std::cout << "=====================" << std::endl;

      gkg::Referential* referential1 = new gkg::Referential( "referential1" );
      gkg::Referential* referential2 = new gkg::Referential( "referential2" );
      gkg::Referential* referential3 = new gkg::Referential( "referential3" );

      gkg::RCPointer< gkg::Translation3d< float > > translation1;
      translation1.reset( new gkg::Translation3d< float >( 10.0, 0.0, 0.0 ) );

      gkg::RCPointer< gkg::Translation3d< float > > translation2;
      translation2.reset( new gkg::Translation3d< float >( 0.0, -7.0, 0.0 ) );

      gkg::Transform3dManager< float >&
        manager = gkg::Transform3dManager< float >::getInstance();

      manager.registerTransform3d( referential1, referential2, translation1 );
      manager.registerTransform3d( referential2, referential3, translation2 );

      gkg::Vector3d< float > from( 0.0, 0.0, 0.0 ), to;
      manager.getTransform3d( referential1, referential3 )->getDirect( from,
                                                                       to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      manager.getTransform3d( referential1, referential3 )->getInverse( to,
                                                                        from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

      // checking that observer/observable mechanism does work
      translation1->setDirectTranslation( 15.0, 0.0, 0.0 );

      manager.getTransform3d( referential1, referential3 )->getDirect( from,
                                                                       to );
      std::cout << "from = " << from << "   ---->   "
                << "to = " << to << std::endl;

      manager.getTransform3d( referential1, referential3 )->getInverse( to,
                                                                        from );
      std::cout << "to = " << to << "   ---->   "
                << "from = " << from << std::endl;

      delete referential1;
      delete referential2;
      delete referential3;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
