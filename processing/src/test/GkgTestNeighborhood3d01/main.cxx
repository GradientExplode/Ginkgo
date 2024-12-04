#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::cout << "testing Neigborhood3d with stride=1 : " << std::endl;

    std::cout << std::endl << "6 connex" << std::endl;
    std::list< gkg::Vector3d< int32_t > > neighbors = 
      gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                        gkg::Neighborhood3d::Neighborhood3d_6 );
    std::list< gkg::Vector3d< int32_t > >::iterator it, ie;

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }
    
    std::cout << std::endl << "18 connex" << std::endl;
    neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_18 );

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }
    
    std::cout << std::endl << "26 connex" << std::endl;
    neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                       gkg::Neighborhood3d::Neighborhood3d_26 );

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }

    std::cout << std::endl;
    std::cout << "testing Neigborhood3d with stride=2 : " << std::endl;

    std::cout << std::endl << "6 connex" << std::endl;
    neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                     gkg::Neighborhood3d::Neighborhood3d_6, 2 );

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }
    
    std::cout << std::endl << "18 connex" << std::endl;
    neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                    gkg::Neighborhood3d::Neighborhood3d_18, 2 );

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }
    
    std::cout << std::endl << "26 connex" << std::endl;
    neighbors = gkg::Neighborhood3d::getInstance().getNeighborOffsets(
                                    gkg::Neighborhood3d::Neighborhood3d_26, 2 );

    it = neighbors.begin();
    ie = neighbors.end();

    while ( it != ie )
    {

      std::cout << "(" << it->x << "," 
                       << it->y << "," 
                       << it->z << ")" << std::endl;

      ++it;

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
