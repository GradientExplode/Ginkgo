#ifndef _gkg_core_pattern_SingletonAccessor_h_
#define _gkg_core_pattern_SingletonAccessor_h_


#include <gkg-core-pattern/Singleton.h>


namespace gkg
{


template < class T, class B > 
class SingletonAccessor : public Singleton< T >
{

  public:

    static B& getAccessor() 
    {

      return Singleton< T >::getInstance();

    }

};


}


#endif
