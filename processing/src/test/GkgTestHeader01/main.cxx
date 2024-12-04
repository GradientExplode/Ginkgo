#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/BaseObjectWriter.h>
#include <gkg-core-io/BaseObjectReader.h>
#include <gkg-core-object/BaseObject_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::Volume< int32_t > volume( 3, 4, 5, 6 );

    std::vector< std::string > oneVector( 4 );
    oneVector[ 0 ] = "january";
    oneVector[ 1 ] = "february";
    oneVector[ 2 ] = "march";
    oneVector[ 3 ] = "april";
    volume.getHeader().addAttribute( "vector", oneVector );

    std::vector< int32_t > twoVector( 3 );
    twoVector[ 0 ] = 2;
    twoVector[ 1 ] = 3;
    twoVector[ 2 ] = 4;
    volume.getHeader().addAttribute( "vector2", twoVector );

    // writing header
    std::ofstream os( "volume.header" );
    os << "# adding a comment for testing gkg::UncommentCounterInputFileStream"
       << std::endl;
    gkg::BaseObjectWriter baseObjectWriter;
    baseObjectWriter.write( os, volume.getHeader() );
    os.close();

    // reading header
    gkg::UncommentCounterInputFileStream is( "volume.header" );

    gkg::TypedObject< gkg::Dictionary > header;


    gkg::BaseObjectReader baseObjectReader;
    baseObjectReader.read( is, header );

    std::string objectType;
    header.getAttribute( "object_type", objectType );
    std::cout << "object_type = " << objectType << std::endl;

    std::string itemType;
    header.getAttribute( "item_type", itemType );
    std::cout << "item_type = " << itemType << std::endl;

    double sizeX;
    header.getAttribute( "sizeX", sizeX );
    std::cout << "sizeX = " << sizeX << std::endl;

    double sizeY;
    header.getAttribute( "sizeY", sizeY );
    std::cout << "sizeY = " << sizeY << std::endl;

    double sizeZ;
    header.getAttribute( "sizeZ", sizeZ );
    std::cout << "sizeZ = " << sizeZ << std::endl;

    double sizeT;
    header.getAttribute( "sizeT", sizeT );
    std::cout << "sizeT = " << sizeT << std::endl;

    std::vector< std::string > anotherVector;
    header.getAttribute( "vector", anotherVector );
    for ( size_t s = 0; s < anotherVector.size(); s++ )
    {

      std::cout << "vector[ " << s << " ] = "
                << anotherVector[ s ]
                << std::endl;

    }

    std::vector< double > stillAnotherVector;
    header.getAttribute( "vector2", stillAnotherVector );
    for ( size_t s = 0; s < stillAnotherVector.size(); s++ )
    {

      std::cout << "vector2[ " << s << " ] = "
                << stillAnotherVector[ s ]
                << std::endl;

    }

    is.close();

  }
  GKG_CATCH_COMMAND( result );

  return result;;

}
