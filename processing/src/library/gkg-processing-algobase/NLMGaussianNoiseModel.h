#ifndef _gkg_processing_algobase_NLMGaussianNoiseModel_h_
#define _gkg_processing_algobase_NLMGaussianNoiseModel_h_


#include <gkg-processing-algobase/NLMNoiseModel.h>


namespace gkg
{


class NLMGaussianNoiseModel : public NLMNoiseModel
{

  public:

    NLMGaussianNoiseModel( const float& sigma );

    float getValue( const float& weight,
                    const float& value ) const;
    float getFiltered( const float &value ) const;

};


}


#endif
