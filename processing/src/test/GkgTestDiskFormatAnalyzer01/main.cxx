#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/DiskFormatAnalyzer.h>
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

    // writing data
    gkg::Writer::getInstance().write( "volume", volumeOut, false );

    // analyzing data
    gkg::AnalyzedObject object;
    std::string format;
    gkg::DiskFormatAnalyzer::getInstance().analyze( "volume", format, object );

    std::cout << "format = " << format << std::endl;
    std::cout << "object_type = " << object.getObjectType() << std::endl;
    std::cout << "item_type = " << object.getItemType() << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
