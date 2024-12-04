#include <gkg-communication-algorithm/VectorReader_i.h>
#include <gkg-communication-algorithm/Binarizer_i.h>
#include <gkg-communication-algorithm/VectorWriter_i.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


int main( int /*argc*/, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    //
    // reading algorithm
    //
    gkg::VectorReader< float > reader;
    reader.getParameter( "filename" )->set( std::string( "input.txt" ) );

    //
    // binarizing algorithm
    //
    gkg::Binarizer< float > binarizer;
    binarizer.getParameter( "threshold" )->set( 5.0f );
    binarizer.getParameter( "background" )->set( 0.0f );
    binarizer.getParameter( "foreground" )->set( 1.0f );

    binarizer.getParameter( "input_vector" )->connect(
                                              reader.getParameter( "vector" ) );

    //
    // writing algorithm
    //
    gkg::VectorWriter< float > writer;
    writer.getParameter( "filename" )->set( std::string( "output.txt" ) );

    writer.getParameter( "vector" )->connect(
                                    binarizer.getParameter( "output_vector" ) );

    //
    // checking
    //
    reader.check();
    binarizer.check();
    writer.check();

    //
    // running
    //
    reader.run();
    binarizer.run();
    writer.run();

    //
    // displaying result(s)
    //
    gkg::TypedAlgorithmParameter< std::vector< float > >* inputVector =
      gkg::TypedAlgorithmParameter< std::vector< float >
                                   >::getTypedAlgorithmParameter(
        reader.getParameter( "vector" ) );

    gkg::TypedAlgorithmParameter< std::vector< float > >* outputVector =
      gkg::TypedAlgorithmParameter< std::vector< float >
                                   >::getTypedAlgorithmParameter(
        writer.getParameter( "vector" ) );


    std::cout << "input_vector = ";
    std::vector< float >::const_iterator
      i = inputVector->getPointee()->begin(),
      ie = inputVector->getPointee()->end();
    while ( i != ie )
    {

      std::cout << *i << " ";
      ++ i;

    }
    std::cout << std::endl;

    std::cout << "output_vector = ";
    std::vector< float >::const_iterator
      o = outputVector->getPointee()->begin(),
      oe = outputVector->getPointee()->end();
    while ( o != oe )
    {

      std::cout << *o << " ";
      ++ o;

    }
    std::cout << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

