#ifndef _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctor_h_
#define _gkg_dmri_pdf_two_pool_model_TwoPoolModelFunctor_h_


#include <gkg-dmri-pdf-two-pool-model/TwoPoolModelCartesianField.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <string>
#include <vector>


namespace gkg
{


template < class C, class I >
class TwoPoolModelFunctor
{

  public:

    virtual ~TwoPoolModelFunctor();

    virtual std::string getName() const = 0;

    virtual void process(
                         C& output,
                         TwoPoolModelCartesianField& twoPoolModelCartesianField,
                         const std::vector< double >& scalarParameters,
                         const std::vector< std::string >& stringParameters,
                         bool verbose ) const;

  protected:

    TwoPoolModelFunctor();

    // RGB scale must be stored in scalarParameters[ 0 ]
    // mesh scale must be stored in scalarParameters[ 1 ]
    // lower FA threshold must be stored in scalarParameters[ 2 ]
    // upper FA threshold must be stored in scalarParameters[ 3 ]

    virtual void processItem(
                I& item,
                const TwoPoolModel& twoPoolModel,
                const std::vector< double >& scalarParameters,
                const std::vector< std::string >& stringParameters ) const;

};


}


#endif
