#ifndef _gkg_communication_algorithm_VectorWriter_h_
#define _gkg_communication_algorithm_VectorWriter_h_


#include <gkg-communication-algorithm/Algorithm.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter.h>
#include <vector>
#include <string>


namespace gkg
{


template < class T >
class VectorWriter : public Algorithm
{

  public:

    VectorWriter();
    virtual ~VectorWriter();

    bool check();
    void run();

  protected:

    TypedAlgorithmParameter< std::string >* _fileName;
    TypedAlgorithmParameter< std::vector< T > >* _vector;

};




}


#endif
