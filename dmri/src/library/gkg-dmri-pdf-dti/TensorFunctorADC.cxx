#include <gkg-dmri-pdf-dti/TensorFunctorADC.h>
#include <gkg-dmri-pdf-dti/TensorFunctor_i.h>
#include <gkg-dmri-pdf-dti/TensorFunctorFactory_i.h>
#include <gkg-core-exception/Exception.h>


gkg::TensorFunctorADC::TensorFunctorADC()
                     : gkg::TensorFunctor< gkg::Volume< float >, float >()
{
}


gkg::TensorFunctorADC::~TensorFunctorADC()
{
}


std::string gkg::TensorFunctorADC::getName() const
{

  return "adc";

}


void gkg::TensorFunctorADC::processItem(
                 float& item,
                 const gkg::Tensor& tensor,
                 const std::vector< double >& /*scalarParameters*/,
                 const std::vector< std::string >& /*stringParameters*/ ) const
{

  try
  {

    item = tensor.getApparentDiffusionCoefficient();

  }
  GKG_CATCH( "void gkg::TensorFunctorADC::processItem( "
             "float& item, "
             "const gkg::Tensor& tensor, "
             "const std::vector< double >& /*scalarParameters*/, "
             "const std::vector< std::string >& /*stringParameters*/ ) "
             "const" );

}


////////////////////////////////////////////////////////////////////////////////
// Tensor functor registration
////////////////////////////////////////////////////////////////////////////////


static bool register_gkg_TensorFunctorADC __attribute__((unused)) =
gkg::TensorFunctorFactory< gkg::Volume< float >, float >::getInstance().
                                                          registerTensorFunctor(
                                         gkg::TensorFunctorADC::getInstance() );
