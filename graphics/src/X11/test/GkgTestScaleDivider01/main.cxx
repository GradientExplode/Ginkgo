// scale divider(s) (usually used for grid management)

#include <gkg-graphics-X11-base/LinearScaleDivider.h>
#include <gkg-graphics-X11-base/LogScaleDivider.h>
#include <iostream>
#include <cstdlib>


int main( int /*argc*/, char* /*argv*/[] )
{

  float lowerBound, upperBound;
  int32_t majorCount, minorCount;
  int32_t type;
  bool end = false;
  char choice = 0x0;

  while ( !end )
  {

    std::cout << "Lower bound ? ";
    std::cin >> lowerBound;
    std::cout << "Upper bound ? ";
    std::cin >> upperBound;
    std::cout << "Major count ? ";
    std::cin >> majorCount;
    std::cout << "Minor count ? ";
    std::cin >> minorCount;
    std::cout << "Linear(0) or Log(1) ? ";
    std::cin >> type;

    gkg::ScaleDivider< float >* sd = 0;

    switch ( type )
    {

      case 0:

        sd = new gkg::LinearScaleDivider< float >( lowerBound, upperBound,
                                                   majorCount, minorCount );
        break;

      case 1:

        sd = new gkg::LogScaleDivider< float >( lowerBound, upperBound,
                                                majorCount, minorCount );
        break;

    }

    int32_t m;
    std::cout << "Major marks :" << std::endl;
    for ( m = 0; m < sd->getMajorCount(); m++ )
    {

      std::cout << sd->getMajorMark( m ) << " ";

    }
    std::cout << std::endl << std::endl;

    std::cout << "Minor marks :" << std::endl;
    for ( m = 0; m < sd->getMinorCount(); m++ )
    {

      std::cout << sd->getMinorMark( m ) << " ";

    }
    std::cout << std::endl << std::endl;

    std::cout << "Continue (y/n) ?";
    std::cin >> choice;
    end = ( choice == 'n' );

  } 
  
  return EXIT_SUCCESS;

}
