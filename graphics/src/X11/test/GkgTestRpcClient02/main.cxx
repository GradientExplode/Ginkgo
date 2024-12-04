// RPC inter-process communication channel : sender
// This example sends data to the plotter GkgTestRpcServer02;
// Workflow:
// - open a terminal, and launch GkgTestRpcServer02
// - open a new terminal, and launch GkgTestRpcClient02, and click on <Enter>
//   to see new data plotted.


#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "Rpc.h"


int main( int /*argc*/, char* /*argv*/[] )
{


  Writer* writer = new Writer( 8001 );

  char msg[81];
  float x = 0.0;
  float y = 92.25;
  float delta = -0.25;
  std::cerr << "Hit return to send the data." << std::endl;
  for ( int32_t k = 0; k < 20; k++ )
  {

    x += 0.25;
    int32_t mod = k % 5;
    if ( mod == 0 )
    {

      delta = -delta;

    }
    y += delta;
    sprintf( msg, "FOO %5.3f %5.3f", x, y );
    std::cerr << msg;
    writer->send( msg );
    writer->flush();
    getchar();

  }
  writer->send( ( char* )"exit" );
  writer->flush();

  delete writer;

  return EXIT_SUCCESS;

}
