#include <gkg-processing-raytracing/ModelToWorldTransform3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


class ModelToWorldTransform3dObserver : public gkg::Observer
{

  public:

    ModelToWorldTransform3dObserver() : gkg::Observer() {}
    ~ModelToWorldTransform3dObserver() {}

    void update( const gkg::Observable*, void* )
   { std::cout << "ModelToWorldTransform3d has changed" << std::endl; }

};


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::ModelToWorldTransform3d< float >
      modelToWorldTransform3d;

    ModelToWorldTransform3dObserver modelToWorldTransform3dObserver;
    modelToWorldTransform3d.addObserver( &modelToWorldTransform3dObserver );

    modelToWorldTransform3d.setParameters( 3.0, 4.0, 5.0,
                                           0.0, 0.0, 90.0,
                                           1.0, 2.0, -3.0,
                                           true );

    gkg::Vector3d< float > from( 1.0, 0.0, 0.0 ), to;
    
    modelToWorldTransform3d.getDirect( from, to );
    std::cout << "from = " << from << "   ---->   "
              << "to = " << to << std::endl;

    modelToWorldTransform3d.getInverse( to, from );
    std::cout << "to = " << to << "   ---->   "
              << "from = " << from << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
