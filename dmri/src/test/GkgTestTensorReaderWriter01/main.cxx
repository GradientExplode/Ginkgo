#include <gkg-communication-plugin/PluginLoader.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-core-io/ItemWriter.h>
#include <gkg-core-io/ItemReader.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-core-exception/Exception.h>
#include <complex>
#include <iostream>
#include <fstream>


int main( int, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    gkg::PluginLoader::getInstance().load();
    gkg::NumericalAnalysisSelector::getInstance().select( "gsl" );

    // writing data
    std::ofstream os;

    try
    {

      os.open( "tensor.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"tensor.txt\"" ) );

    }

    gkg::Tensor tensor1;
    std::cout << tensor1.getCoefficients() << std::endl;


    gkg::ItemWriter< gkg::Tensor >& itemW =
    gkg::DefaultItemIOFactory< gkg::Tensor >::getInstance().getWriter( true,
                                                                       false );
    itemW.write( os, tensor1 );

    os.close();

    // reading data
    std::ifstream is;

    try
    {

      is.open( "tensor.txt" );

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( std::string( "cannot open \"tensor.txt\"" ) );

    }

    gkg::Tensor tensor2;

    gkg::ItemReader< gkg::Tensor >& itemR =
    gkg::DefaultItemIOFactory< gkg::Tensor >::getInstance().getReader( true,
                                                                       false );
    itemR.read( is, tensor2 );

    is.close();

    std::cout << tensor2.getCoefficients() << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}
