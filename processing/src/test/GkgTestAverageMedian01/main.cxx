#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <iostream>


int main()
{

  std::vector< int32_t > in1( 10 );
  in1[ 0 ] = -5;
  in1[ 1 ] = 10;
  in1[ 2 ] = 3;
  in1[ 3 ] = -16;
  in1[ 4 ] = 7;
  in1[ 5 ] = 2;
  in1[ 6 ] = 4;
  in1[ 7 ] = 9;
  in1[ 8 ] = 18;
  in1[ 9 ] = -1;

  gkg::AverageFilter< std::vector< int32_t >, double > averageFilter1;
  double average1 = 0.0;
  averageFilter1.filter( in1, average1 );

  gkg::MedianFilter< std::vector< int32_t >, float > medianFilter1;
  float median1 = 0.0;
  medianFilter1.filter( in1, median1 );

  std::cout << "average1 : " << average1 << std::endl;
  std::cout << "median1  : " << median1 << std::endl;

  return EXIT_SUCCESS;

}
