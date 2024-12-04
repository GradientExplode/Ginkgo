#ifndef _gkg_communication_algorithm_TypedAlgorithmParameter_h_
#define _gkg_communication_algorithm_TypedAlgorithmParameter_h_


#include <gkg-communication-algorithm/AlgorithmParameter.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class T >
class TypedAlgorithmParameter : public AlgorithmParameter
{

  public:

    TypedAlgorithmParameter( const std::string& name,
                             bool isInput,
                             bool isOutput,
                             const std::string& description = "" );
    virtual ~TypedAlgorithmParameter();

    const RCPointer< T >& getPointee() const;

    void set( const T& value );

    void connect( TypedAlgorithmParameter< T >* other );

    static TypedAlgorithmParameter< T >*
      getTypedAlgorithmParameter( AlgorithmParameter* algorithmParameter );

  protected:

    RCPointer< T > _pointee;

};


}


#endif
