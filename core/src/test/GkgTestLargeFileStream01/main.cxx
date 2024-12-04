#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-cppext/LargeFileStream.h>
#include <gkg-core-exception/Exception.h>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileName = "foo.bin";

    // creating a large file and writing to it
    gkg::largefile_ofstream os( fileName );
    os.truncate( 4000000000LL );
    uint8_t tmp = 0xff;
    os.seek( 4000000000LL - 1LL, gkg::IosBase::beg );
    os.write( ( char* )&tmp, 1U );
    os.close();

    std::cout << "value written at position "
              << 4000000000LL - 1LL
              << " : "
              << ( int32_t )tmp
              << std::endl;

    gkg::largefile_ifstream is( fileName );

    // reading from a large file
    uint8_t value = 0;
    is.seek( ( gkg::BasicLargeFileIFStream::off_type )4000000000LL - 1LL,
             gkg::IosBase::beg );
    is.read( ( char* )&value, sizeof( uint8_t ) );
    is.close();

    std::cout << "value read at position "
              << 4000000000LL - 1LL
              << " : "
              << ( int32_t )value
              << std::endl;


  }
  GKG_CATCH_COMMAND( result );

  return result;

}
