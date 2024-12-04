#ifndef _gkg_processing_algobase_NLMRicianNoiseModel_h_
#define _gkg_processing_algobase_NLMRicianNoiseModel_h_


#include <gkg-processing-algobase/NLMNoiseModel.h>


namespace gkg
{


class NLMRicianNoiseModel : public NLMNoiseModel
{

  public:

    NLMRicianNoiseModel( const float& sigma );

    float getValue( const float& weight,
                    const float& value ) const;
    float getFiltered( const float &value ) const;

  private:

    float _riceConstant;

};


}


#endif
