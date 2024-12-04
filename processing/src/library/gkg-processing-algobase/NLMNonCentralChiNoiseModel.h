#ifndef _gkg_processing_algobase_NLMNonCentralChiNoiseModel_h_
#define _gkg_processing_algobase_NLMNonCentralChiNoiseModel_h_


#include <gkg-processing-algobase/NLMNoiseModel.h>


namespace gkg
{


class NLMNonCentralChiNoiseModel : public NLMNoiseModel
{

  public:

    NLMNonCentralChiNoiseModel( const float& sigma,
                                const float& channelCount );

    float getValue( const float& weight,
                    const float& value ) const;
    float getFiltered( const float &value ) const;

  private:

    float _ncChiConstant;

};


}


#endif
