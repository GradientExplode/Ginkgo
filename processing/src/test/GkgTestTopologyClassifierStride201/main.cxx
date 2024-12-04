#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-topology/TopologyClassifier_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    int32_t i, j, c_star, c_bar, imax = 1 << 26, simpleCount = 0;
    int32_t classes[ 9 ][ 7 ];
    gkg::Volume< int32_t > neighbor26( 5, 5, 5 );
    gkg::GreaterThanFunction< int32_t > testFunction( 0 );
    gkg::Vector3d< int32_t > site( 2, 2, 2 );

    for ( j = 0; j < 9; j++ )
    {

      for ( i = 0; i < 7; i++ )
      {

        classes[ j ][ i ] = 0;

      }

    }

    std::cout << "0 / 67108864" << std::endl;

    for ( i = 0; i < imax; i++ )
    {

      if ( i && !( i % 1000000 ) )
      {

	std::cout << i << " / 67108864" << std::endl;

      }

      neighbor26( 0, 0, 0 ) = i & 0x1;
      neighbor26( 0, 0, 2 ) = i & 0x2;
      neighbor26( 0, 0, 4 ) = i & 0x4;
      neighbor26( 0, 2, 0 ) = i & 0x8;
      neighbor26( 0, 2, 2 ) = i & 0x10;
      neighbor26( 0, 2, 4 ) = i & 0x20;
      neighbor26( 0, 4, 0 ) = i & 0x40;
      neighbor26( 0, 4, 2 ) = i & 0x80;
      neighbor26( 0, 4, 4 ) = i & 0x100;
      neighbor26( 2, 0, 0 ) = i & 0x200;
      neighbor26( 2, 0, 2 ) = i & 0x400;
      neighbor26( 2, 0, 4 ) = i & 0x800;
      neighbor26( 2, 2, 0 ) = i & 0x1000;
      neighbor26( 2, 2, 2 ) = 1;
      neighbor26( 2, 2, 4 ) = i & 0x2000;
      neighbor26( 2, 4, 0 ) = i & 0x4000;
      neighbor26( 2, 4, 2 ) = i & 0x8000;
      neighbor26( 2, 4, 4 ) = i & 0x10000;
      neighbor26( 4, 0, 0 ) = i & 0x20000;
      neighbor26( 4, 0, 2 ) = i & 0x40000;
      neighbor26( 4, 0, 4 ) = i & 0x80000;
      neighbor26( 4, 2, 0 ) = i & 0x100000;
      neighbor26( 4, 2, 2 ) = i & 0x200000;
      neighbor26( 4, 2, 4 ) = i & 0x400000;
      neighbor26( 4, 4, 0 ) = i & 0x800000;
      neighbor26( 4, 4, 2 ) = i & 0x1000000;
      neighbor26( 4, 4, 4 ) = i & 0x2000000;

      gkg::TopologyClassifier::getInstance().setStride( 2 );
      gkg::TopologyClassifier::getInstance().getCStarAndCBar( neighbor26, 
                                                              testFunction, 
                                                              site, 
                                                              c_star, 
                                                              c_bar );

      classes[ c_star ][ c_bar ]++;

      if ( gkg::TopologyClassifier::getInstance().isSimplePoint( neighbor26, 
                                                                 testFunction, 
                                                                 site ) )
      {

        simpleCount++;

      }

    }

    std::cout << "Results with stride = 2" << std::endl;
    std::cout << "Number of simple points : " << simpleCount << std::endl;
    std::cout << "Cstar  Cbar    Number" << std::endl;

    for ( j = 0; j < 9; j++ )
    {

      for ( i = 0; i < 7; i++ )
      {

        std::cout << "  " << j << "      " << i << "     ";
        std::cout << classes[ j ][ i ] << std::endl;

      }

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
