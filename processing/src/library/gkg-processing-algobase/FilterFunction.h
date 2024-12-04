#ifndef _gkg_processing_algobase_FilterFunction_h_
#define _gkg_processing_algobase_FilterFunction_h_


namespace gkg
{


//
// class FilterFunction< IN,
//                       OUT >
//

template < class IN, class OUT >
class FilterFunction
{

  public:

    virtual ~FilterFunction();

    virtual void filter( const IN& in,
                         OUT& out ) const = 0;

};


}


#endif


