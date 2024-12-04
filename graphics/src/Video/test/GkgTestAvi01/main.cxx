#include <gkg-graphics-video-avi/AviWriter_i.h>
#include <gkg-graphics-video-avi/AviFile.h>
#include <gkg-graphics-video-avi/AviMemory.h>
#include <gkg-core-exception/Exception.h>

#include <cstdlib>
#include <cstring>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {


    int32_t t, dx = 128, dy = 128, dt = 64, fps = 5;
    int32_t size = 3 * dx * dy;
    char color = 0;

    char* buffer = new char[ size ];

    if ( !buffer )
    {

      throw std::runtime_error( "could not allocate buffer" );

    }

    // first, test creation of Avi into memory
    gkg::AviWriter< gkg::AviMemory > avi( dx, dy, 24, dt, fps );

    for ( t = 0; t < dt; t++ )
    {

      color = ( char )( 255.0 * ( float )t / ( ( float )dt - 1.0f ) );
      memset( buffer, color, size );
      avi.addFrame( buffer, size );

    }

    avi.write( "aviMemory.avi" );
  
    // second, test creation of Avi using a temporary file
    gkg::AviWriter< gkg::AviFile > avi2( dx, dy, 24, dt, fps );

    for ( t = 0; t < dt; t++ )
    {

      color = ( char )( 255.0 * ( float )t / ( ( float )dt - 1.0f ) );
      memset( buffer, color, size );
      avi2.addFrame( buffer, size );

    }

    avi2.write( "aviFile.avi" );

    delete[] buffer;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
