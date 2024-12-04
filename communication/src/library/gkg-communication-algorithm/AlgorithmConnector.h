#ifndef _gkg_communication_algorithm_AlgorithmConnector_h_
#define _gkg_communication_algorithm_AlgorithmConnector_h_


#include <gkg-core-pattern/Singleton.h>
#include <string>
#include <map>


namespace gkg
{


class AlgorithmParameter;


//
// class AlgorithmConnector
//

class AlgorithmConnector
{

  public:

    virtual ~AlgorithmConnector();

    virtual std::string getType() const = 0;

    // connect( p1, p2 ) will plug destination p1 on to source p2
    virtual void connect( AlgorithmParameter* parameter1,
                          AlgorithmParameter* parameter2 ) = 0;

  protected:

    AlgorithmConnector();

};


//
// class TypedAlgorithmConnector
//

template < class T >
class TypedAlgorithmConnector : public AlgorithmConnector,
                                public Singleton< TypedAlgorithmConnector< T > >
{

  public:

    virtual ~TypedAlgorithmConnector();

    std::string getType() const;

    void connect( AlgorithmParameter* parameter1,
                  AlgorithmParameter* parameter2 );

  protected:

    friend class Singleton< TypedAlgorithmConnector< T > >;

    TypedAlgorithmConnector();

};


//
// class AlgorithmConnectorFactory
//

class AlgorithmConnectorFactory :
                                  public Singleton< AlgorithmConnectorFactory >
{

  public:

    virtual ~AlgorithmConnectorFactory();

    void registerAlgorithmConnector( AlgorithmConnector* algorithmConnector );

    AlgorithmConnector* getAlgorithmConnector( const std::string& type ) const;

  private:

    friend class Singleton< AlgorithmConnectorFactory >;

    AlgorithmConnectorFactory();

    std::map< std::string, AlgorithmConnector* > _algorithmConnectors;

};


}


#define RegisterAlgorithmConnector( Name, AlgorithmConnector )               \
static bool registerAlgorithmConnector_##Name()                              \
{                                                                            \
                                                                             \
  gkg::AlgorithmConnectorFactory::getInstance().registerAlgorithmConnector(  \
                                       &AlgorithmConnector::getInstance() ); \
                                                                             \
  return true;                                                               \
                                                                             \
}                                                                            \
                                                                             \
static bool registeredAlgorithmConnector_##Name __attribute((unused))=       \
  registerAlgorithmConnector_##Name()


#endif
