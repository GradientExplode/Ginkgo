#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::Volume< int32_t > volumeOut( 3, 4 );
    gkg::Volume< int32_t > volumeIn;

    volumeOut.getHeader().addAttribute( "resolutionX", ( double )1.5 );
    volumeOut.getHeader().addAttribute( "resolutionY", ( double )2.5 );
    volumeOut.getHeader().addAttribute( "resolutionZ", ( double )3.5 );
    volumeOut.getHeader().addAttribute( "resolutionT", ( double )4.5 );

    int32_t x, y;
    for ( y = 0; y < volumeOut.getSizeY(); y++ )
    {

      for ( x = 0; x < volumeOut.getSizeX(); x++ )
      {

        volumeOut( x, y ) = x + y;

      }

    }

    gkg::TypedDiskFormat< gkg::Volume< int32_t > >*
      format = gkg::DiskFormatFactory< gkg::Volume< int32_t > >::getInstance().
                getDiskFormat( "gis" );
    if ( format )
    {

      // writing GIS data
      format->write( "volume", volumeOut, false );

      // reading GIS data
      format->read( "volume.ima", volumeIn );

      std::string objectType, itemType;
      int32_t sizeX, sizeY, sizeZ, sizeT;
      double resolutionX, resolutionY, resolutionZ, resolutionT;
      std::string byteOrderName;
      std::string ioModeName;

      volumeIn.getHeader().getAttribute( "object_type", objectType );
      volumeIn.getHeader().getAttribute( "item_type", itemType );
      volumeIn.getHeader().getAttribute( "sizeX", sizeX );
      volumeIn.getHeader().getAttribute( "sizeY", sizeY );
      volumeIn.getHeader().getAttribute( "sizeZ", sizeZ );
      volumeIn.getHeader().getAttribute( "sizeT", sizeT );
      volumeIn.getHeader().getAttribute( "resolutionX", resolutionX );
      volumeIn.getHeader().getAttribute( "resolutionY", resolutionY );
      volumeIn.getHeader().getAttribute( "resolutionZ", resolutionZ );
      volumeIn.getHeader().getAttribute( "resolutionT", resolutionT );
      volumeIn.getHeader().getAttribute( "byte_order", byteOrderName );
      volumeIn.getHeader().getAttribute( "io_mode", ioModeName );

      std::cout << "object_type = " << objectType << std::endl;
      std::cout << "item_type = " << itemType << std::endl;
      std::cout << "sizeX = " << sizeX << std::endl;
      std::cout << "sizeY = " << sizeY << std::endl;
      std::cout << "sizeZ = " << sizeZ << std::endl;
      std::cout << "sizeT = " << sizeT << std::endl;
      std::cout << "resolutionX = " << resolutionX << std::endl;
      std::cout << "resolutionY = " << resolutionY << std::endl;
      std::cout << "resolutionZ = " << resolutionZ << std::endl;
      std::cout << "resolutionT = " << resolutionT << std::endl;
      std::cout << "byte_order = " << byteOrderName << std::endl;
      std::cout << "io_mode = " << ioModeName << std::endl;

      for ( y = 0; y < volumeIn.getSizeY(); y++ )
      {

        for ( x = 0; x < volumeIn.getSizeX(); x++ )
        {

          std::cout << volumeIn( x, y ) << "\t";

        }
        std::cout << std::endl;

      }

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
