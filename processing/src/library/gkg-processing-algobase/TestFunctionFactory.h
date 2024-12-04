#ifndef _gkg_processing_algobase_TestFunctionFactory_h_
#define _gkg_processing_algobase_TestFunctionFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>


namespace gkg
{


template < class T > class TestFunction;
 

template < class T >
class TestFunctionFactory : public Singleton< TestFunctionFactory< T > >
{

  public:
  
    ~TestFunctionFactory();
    
    TestFunction< T >* createTestFunction( const std::string& name,
                                           T threshold1,
					   T threshold2 = T( 0 ) ) const;
					   
  protected:
  
    friend class Singleton< TestFunctionFactory< T > >;
    
    TestFunctionFactory();

};


}


#endif
