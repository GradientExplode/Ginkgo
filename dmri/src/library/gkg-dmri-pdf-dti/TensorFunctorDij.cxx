#include <gkg-dmri-pdf-dti/TensorFunctorDij.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorDij::TensorFunctorDij()
                      : gkg::TensorFunctor< gkg::Volume< float >,
                                            std::vector< float > >()
{
}


gkg::TensorFunctorDij::~TensorFunctorDij()
{
}


std::string gkg::TensorFunctorDij::getName() const
{

  return "dij";

}


void gkg::TensorFunctorDij::processItem(
                 std::vector< float >& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 6 coefficient(s)
    item.resize( 6 );

    // collecting tensor coefficient(s)
    item[ 0 ] = tensor.getCoefficient( gkg::Tensor::Dxx );
    item[ 1 ] = tensor.getCoefficient( gkg::Tensor::Dxy );
    item[ 2 ] = tensor.getCoefficient( gkg::Tensor::Dxz );
    item[ 3 ] = tensor.getCoefficient( gkg::Tensor::Dyy );
    item[ 4 ] = tensor.getCoefficient( gkg::Tensor::Dyz );
    item[ 5 ] = tensor.getCoefficient( gkg::Tensor::Dzz );

  }
  GKG_CATCH( "void gkg::TensorFunctorDij::processItem( "
             "std::vector< float >& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorDij __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >,
                        std::vector< float > >::getInstance().
                                                         registerTensorFunctor(
                        gkg::TensorFunctorDij::getInstance() );
