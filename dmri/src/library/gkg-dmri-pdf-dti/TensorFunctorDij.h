#ifndef _gkg_dmri_pdf_dti_TensorFunctorDij_h_
#define _gkg_dmri_pdf_dti_TensorFunctorDij_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>
#include <vector>


namespace gkg
{


class TensorFunctorDij : public TensorFunctor< Volume< float >,
                                            std::vector< float > >,
                         public Singleton< TensorFunctorDij >
{

  public:

    ~TensorFunctorDij();

    std::string getName() const;


  protected:

    friend class Singleton< TensorFunctorDij >;

    TensorFunctorDij();

    void processItem(
                 std::vector< float >& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif