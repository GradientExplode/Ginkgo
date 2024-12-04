#ifndef _gkg_communication_algorithm_Binarizer_h_
#define _gkg_communication_algorithm_Binarizer_h_


#include <gkg-communication-algorithm/Algorithm.h>
#include <gkg-communication-algorithm/TypedAlgorithmParameter.h>
#include <vector>


namespace gkg
{


template < class T >
class Binarizer : public Algorithm
{

  public:

    Binarizer();
    virtual ~Binarizer();

    bool check();
    void run();

  protected:

    TypedAlgorithmParameter< std::vector< T > >* _inputVector;
    TypedAlgorithmParameter< std::vector< T > >* _outputVector;
    TypedAlgorithmParameter< T >* _threshold;
    TypedAlgorithmParameter< T >* _background;
    TypedAlgorithmParameter< T >* _foreground;

};




}


#endif
