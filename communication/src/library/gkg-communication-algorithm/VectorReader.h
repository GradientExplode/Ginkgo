#ifndef _gkg_communication_algorithm_VectorReader_h_
#define _gkg_communication_algorithm_VectorReader_h_


#include <gkg-communication-algorithm/Algorithm.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter.h>
#include <vector>
#include <string>


namespace gkg
{


template < class T >
class VectorReader : public Algorithm
{

  public:

    VectorReader();
    virtual ~VectorReader();

    bool check();
    void run();

  protected:

    TypedAlgorithmParameter< std::string >* _fileName;
    TypedAlgorithmParameter< std::vector< T > >* _vector;

};




}


#endif
