#ifndef _gkg_dmri_fiber_distance_FiberDistanceFunctorFactory_h_
#define _gkg_dmri_fiber_distance_FiberDistanceFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-fiber-distance/FiberDistanceFunctor.h>
#include <map>
#include <vector>


namespace gkg
{


class FiberDistanceFunctorFactory : public Singleton<
                                                   FiberDistanceFunctorFactory >
{

  public:

    typedef FiberDistanceFunctor*
              ( *FiberDistanceFunctorCreator )( const std::vector< double >&  );

    ~FiberDistanceFunctorFactory();

    bool registerFiberDistanceFunctor(
                      const std::string& name,
                      FiberDistanceFunctorCreator fiberDistanceFunctorCreator,
                      const std::string& scalarParameterHelp );

    FiberDistanceFunctor*
      create( const std::string& name,
              const std::vector< double >& scalarParameters ) const;

    bool hasFiberDistanceFunctor( const std::string& name ) const;
    std::set< std::string > getNames() const;
    std::string getNameListHelp() const;
    std::string getScalarParameterHelp( const std::string& name ) const;

  protected:

    friend class Singleton< FiberDistanceFunctorFactory >;

    FiberDistanceFunctorFactory();

    std::map< std::string,
              FiberDistanceFunctorCreator > _fiberDistanceFunctorCreators;
    std::map< std::string, std::string > _scalarParameterHelps;

};


}


#endif

