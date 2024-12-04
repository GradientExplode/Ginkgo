#ifndef _gkg_dmri_pdf_dti_TensorFunctorEigenValues_h_
#define _gkg_dmri_pdf_dti_TensorFunctorEigenValues_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class TensorFunctorEigenValues : public TensorFunctor< Volume< float >,
                                            std::vector< float > >,
                                 public Singleton< TensorFunctorEigenValues >
{

  public:

    ~TensorFunctorEigenValues();

    std::string getName() const;


  protected:

    friend class Singleton< TensorFunctorEigenValues >;

    TensorFunctorEigenValues();

    void processItem(
                 std::vector< float >& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif
