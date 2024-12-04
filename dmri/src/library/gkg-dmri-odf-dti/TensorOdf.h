#ifndef _gkg_dmri_odf_dti_TensorOdf_h_
#define _gkg_dmri_odf_dti_TensorOdf_h_


#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/TypeOf.h>


namespace gkg
{


class Tensor;


class TensorOdf : public OrientationDistributionFunction
{

  public:

    TensorOdf( const OrientationSet* orientationSet,
               const Tensor& tensor );
    virtual ~TensorOdf();

    float getAnisotropy() const;

    const Tensor& getTensor() const;

  protected:

    const Tensor& _tensor;

};


}


////////////////////////////////////////////////////////////////////////////////
// creating type name for TensorOdf
////////////////////////////////////////////////////////////////////////////////

RegisterBaseType( gkg::TensorOdf, gkg_TensorOdf );


#endif
