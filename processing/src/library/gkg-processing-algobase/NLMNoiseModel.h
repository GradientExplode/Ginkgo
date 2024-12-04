#ifndef _gkg_processing_algobase_NLMNoiseModel_h_
#define _gkg_processing_algobase_NLMNoiseModel_h_


namespace gkg
{


class NLMNoiseModel
{

  public:

    virtual ~NLMNoiseModel();

    float getSigma() const;

    virtual float getValue( const float& weight,
                            const float& value ) const = 0;
    virtual float getFiltered( const float &value ) const = 0;

  protected:

    NLMNoiseModel( const float& sigma );

    float _sigma;

};


}


#endif
