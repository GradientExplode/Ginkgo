#include <gkg-communication-sysinfo/KeyboardHit.h>
#include <gkg-core-exception/Exception.h>


void gkg::keyboardHit()
{

  try
  {

    char c;

    std::cout << "click <e> to continue..." << std::flush;
    do
    {

      std::cin >> c;

    }
    while ( c != 'e' );

  }
  GKG_CATCH( "void gkg::keyboardHit()" );

}
