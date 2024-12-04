#ifndef _gkg_core_pattern_Singleton_h_
#define _gkg_core_pattern_Singleton_h_


namespace gkg
{


template < class T > 
class Singleton
{

  public:

    static T& getInstance() 
    {
    
      static T instance;
      return instance;

    }

};


}


#define ForceSingletonInstanciation( T, NAME ) \
static T& forced_##NAME##_instanciation __attribute__((unused)) = \
                                                               T::getInstance();


#define ForceSingletonInstanciation2( T1,T2, NAME ) \
static T1,T2& forced_##NAME##_instanciation __attribute__((unused)) = \
                                                           T1,T2::getInstance();


#define ForceSingletonInstanciation3( T1,T2,T3, NAME ) \
static T1,T2,T3& forced_##NAME##_instanciation __attribute__((unused)) = \
                                                        T1,T2,T3::getInstance();


#endif
