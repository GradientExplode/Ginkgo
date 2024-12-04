// threaded algorithm with detached, cancellable thread


#include "DetachedThread.h"
#include <gkg-core-cppext/CUniStd.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


void foo( DetachedThread& detachedThread )
{

  std::cerr << "beginning of foo()" << std::endl;
  detachedThread.launch();
  std::cerr << "end of foo()" << std::endl;

}


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    DetachedThread detachedThread;

    foo( detachedThread );
    sleep( 2 );
    foo( detachedThread );
    sleep( 10 );  

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
