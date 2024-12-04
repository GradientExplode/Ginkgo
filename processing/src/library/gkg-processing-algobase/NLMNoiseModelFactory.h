#ifndef _gkg_processing_algobase_NLMNoiseModelFactory_h_
#define _gkg_processing_algobase_NLMNoiseModelFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


class NLMNoiseModel;
 

class NLMNoiseModelFactory : public Singleton< NLMNoiseModelFactory >
{

  public:
  
    ~NLMNoiseModelFactory();
    
    NLMNoiseModel* createNLMNoiseModelFunction( 
                                              const std::string& name,
                                              float sigma = 1.0f,
					      float channelCount = 1.0f ) const;
					   
  protected:
  
    friend class Singleton< NLMNoiseModelFactory >;
    
    NLMNoiseModelFactory();

};


}


#endif
