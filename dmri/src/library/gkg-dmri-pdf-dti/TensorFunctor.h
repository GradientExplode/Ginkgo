#ifndef _gkg_dmri_pdf_dti_TensorFunctor_h_
#define _gkg_dmri_pdf_dti_TensorFunctor_h_


#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class TensorFunctor
{

  public:

    virtual ~TensorFunctor();

    virtual std::string getName() const = 0;

    virtual void process( C& output,
                          TensorCartesianField& tensorCartesianField,
                          const std::vector< double >& scalarParameters,
                          const std::vector< std::string >& stringParameters,
                          bool verbose ) const;

  protected:

    TensorFunctor();

    // RGB scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]
    // lower FA threshold must be stored in scalarParameters[ 2 ]
    // upper FA threshold must be stored in scalarParameters[ 3 ]

    virtual void processItem(
                I& item,
                const Tensor& tensor,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
