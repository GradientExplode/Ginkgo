#include <gkg-processing-container/Curve3d.h>
#include <vector>



int main()
{

  std::vector< gkg::Vector3d< float > > testVector;
  testVector.push_back( gkg::Vector3d< float >(0.0f,0.0f,0.0f) );
  testVector.push_back( gkg::Vector3d< float >(0.0f,0.0f,0.0f) );
  testVector.push_back( gkg::Vector3d< float >(0.0f,0.0f,0.0f) );

  gkg::Curve3d< float > curve3d( testVector );

  std::cout << "curve3d.getPointCount() = " 
            << curve3d.getPointCount() << std::endl;


  return EXIT_SUCCESS;

}
