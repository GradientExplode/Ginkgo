#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>



class MyObject : public gkg::TypedObject< gkg::Dictionary >
{

  public:

    MyObject() {}
    virtual ~MyObject() {}


};


int main( int /* argc */, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    MyObject myObject;

    myObject.addAttribute( "dimension", ( int32_t )256 );
    myObject.addAttribute( "resolution", ( double )3.0 );

    gkg::GenericObjectList times( 5 );
    for ( int32_t i = 0; i < 5; i++ )
    times[ i ] = ( int16_t )( 2 * i );

    myObject.addAttribute( "times", times );

    std::set< std::string > attributes = myObject.getAttributes();

    std::set< std::string >::const_iterator a = attributes.begin(),
                                          ae = attributes.end();
    std::cout << "attributes : ";
    while ( a != ae )
    {

      std::cout << *a << " ";
      ++ a;

    }
    std::cout << std::endl;

    try
    {

      int32_t value;
      myObject.getAttribute( "dimension", value );
      std::cout << "dimension = " << value << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "failed to read \"dimension\" " ) +
                                e.what() );

    }

    try
    {

      double value;
      myObject.getAttribute( "resolution", value );
      std::cout << "resolution = " << value << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "failed to read \"resolution\" ") +
                                e.what() );

    }

    try
    {

      gkg::GenericObjectList timesBis;
      myObject.getAttribute( "times", timesBis );
      std::cout << "resolution = ";
      for ( size_t i = 0; i < timesBis.size(); i++ )
      {

         std::cout << timesBis[ i ]->getValue< int16_t >() << " ";

      }
      std::cout << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "failed to read \"times\" ") +
                                e.what() );

    }

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
