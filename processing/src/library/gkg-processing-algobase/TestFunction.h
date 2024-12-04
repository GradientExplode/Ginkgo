#ifndef _gkg_processing_algobase_TestFunction_h_
#define _gkg_processing_algobase_TestFunction_h_


namespace gkg
{


template < class T >
class TestFunction
{

  public:

    virtual ~TestFunction();

    virtual bool getState( const T& value ) const = 0;

  protected:

    TestFunction();

};


}


#endif
