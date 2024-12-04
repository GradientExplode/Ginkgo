#ifndef _gkg_dmri_pdf_dti_TensorFunctorDiffusionWeightedT2_h_
#define _gkg_dmri_pdf_dti_TensorFunctorDiffusionWeightedT2_h_


#include <gkg-dmri-pdf-dti/TensorFunctor.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


class TensorFunctorDiffusionWeightedT2 : public TensorFunctor< Volume< float >, float >,
                                         public Singleton<
                                              TensorFunctorDiffusionWeightedT2 >
{

  public:

    ~TensorFunctorDiffusionWeightedT2();

    std::string getName() const;

    void process( Volume< float >& output,
                  TensorCartesianField& tensorCartesianField,
                  const std::vector< double >& scalarParameters,
                  const std::vector< std::string >& stringParameters,
                  bool verbose ) const;

  protected:

    friend class Singleton< TensorFunctorDiffusionWeightedT2 >;

    TensorFunctorDiffusionWeightedT2();

    void processItem(
                 float& item,
                 const Tensor& tensor,
                 const std::vector< double >& scalarParameters,
                 const std::vector< std::string >& stringParameters ) const;

};


}


#endif
