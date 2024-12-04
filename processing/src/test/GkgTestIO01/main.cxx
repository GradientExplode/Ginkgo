#include <gkg-core-io/StdVectorItemIOFactory_i.h>
#include <gkg-core-io/StdListItemIOFactory_i.h>
#include <gkg-core-exception/Exception.h>
#include <complex>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    //
    // testing vector I/O
    //

    // writing data
    std::ofstream os;

    try
    {

      os.open( "vector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"vector.txt\"" ) );

    }

    std::vector< std::complex< float > > v1( 10 );
    for ( size_t i = 0; i < 10; i++ )
    {

      v1[ i ] = std::complex< float >( ( float )i, ( float )i / 2.0f );

    }
    gkg::ItemWriter< std::vector< std::complex< float > > >& vectorW =
    gkg::StdVectorItemIOFactory<
               std::complex< float > >::getInstance().getWriter( true, false );
    vectorW.write( os, v1 );

    os.close();

    // reading data
    std::ifstream is;

    try
    {

      is.open( "vector.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"vector.txt\"" ) );

    }

    std::vector< std::complex< float > > v2;
    gkg::ItemReader< std::vector< std::complex< float > > >& vectorR =
    gkg::StdVectorItemIOFactory<
               std::complex< float > >::getInstance().getReader( true, false );
    vectorR.read( is, v2 );

    is.close();

    for ( size_t i = 0; i < v2.size(); i++ )
    {

      std::cout << v2[ i ] << " ";

    }
    std::cout << std::endl;

    //
    // testing list I/O
    //
    try
    {

      os.open( "list.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"list.txt\"" ) );

    }

    std::list< std::complex< float > > l1;
    for ( size_t i = 0; i < 10; i++ )
    {

      l1.push_back( std::complex< float >( ( float )i, ( float )i / 2.0f ) );

    }
    gkg::ItemWriter< std::list< std::complex< float > > >& listW =
    gkg::StdListItemIOFactory<
               std::complex< float > >::getInstance().getWriter( true, false );
    listW.write( os, l1 );

    os.close();

    // reading data
    try
    {

      is.open( "list.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"list.txt\"" ) );

    }

    std::list< std::complex< float > > l2;
    gkg::ItemReader< std::list< std::complex< float > > >& listR =
    gkg::StdListItemIOFactory<
               std::complex< float > >::getInstance().getReader( true, false );
    listR.read( is, l2 );

    is.close();

    std::list< std::complex< float > >::const_iterator l = l2.begin(),
                                                       le = l2.end();
    while ( l != le )
    {

      std::cout << *l << " ";
      ++ l;

    }
    std::cout << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
