#include <gkg-dmri-pdf-dti/TensorFunctorEigenValues.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorEigenValues::TensorFunctorEigenValues()
                      : gkg::TensorFunctor< gkg::Volume< float >,
                                            std::vector< float > >()
{
}


gkg::TensorFunctorEigenValues::~TensorFunctorEigenValues()
{
}


std::string gkg::TensorFunctorEigenValues::getName() const
{

  return "eigenvalues";

}


void gkg::TensorFunctorEigenValues::processItem(
                 std::vector< float >& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    // allocating the 3 eigenvalue(s)
    item.resize( 3 );

    // collecting tensor coefficient(s)
    item[ 0 ] = tensor.getEigenValue( gkg::Tensor::Maximum );
    item[ 1 ] = tensor.getEigenValue( gkg::Tensor::Middle );
    item[ 2 ] = tensor.getEigenValue( gkg::Tensor::Minimum );

  }
  GKG_CATCH( "void gkg::TensorFunctorEigenValues::processItem( "
             "std::vector< float >& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorEigenValues __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >,
                        std::vector< float > >::getInstance().
                                                         registerTensorFunctor(
                        gkg::TensorFunctorEigenValues::getInstance() );
