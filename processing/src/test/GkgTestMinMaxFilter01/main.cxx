#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-algobase/MinMaxFilter_i.h>
#include <gkg-processing-algobase/MinMaxIndexFilter_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::vector< float > values;

    values.push_back( 4.0f );
    values.push_back( -1.0f );
    values.push_back( 0.0f );
    values.push_back( 10.0f );
    values.push_back( -0.4f );
    values.push_back( 2.5f );
    values.push_back( -1.7f );
    values.push_back( 7.3f );
    values.push_back( 3.2f );
    values.push_back( 1.6f );

    std::vector< float >::iterator
      v = values.begin(),
      ve = values.end();

    std::cout << "Values : " << std::flush;
    while ( v != ve )
    {

      std::cout << *v << " ";
      ++ v;

    }
    std::cout << std::endl;

    std::pair< float, float > minMax;
    gkg::MinMaxFilter< std::vector< float >, float > minMaxFilter;
    std::pair< float, float > minMaxIndex;
    gkg::MinMaxIndexFilter< std::vector< float >, float > minMaxIndexFilter;

    minMaxFilter.filter( values, minMax );
    minMaxIndexFilter.filter( values, minMaxIndex );

    std::cout << "  Min = " << minMax.first
              << " (index = " << minMaxIndex.first << ")" << std::endl;
    std::cout << "  Max = " << minMax.second
              << " (index = " << minMaxIndex.second << ")" << std::endl;


  }
  GKG_CATCH_COMMAND( result );

  return result;

}

