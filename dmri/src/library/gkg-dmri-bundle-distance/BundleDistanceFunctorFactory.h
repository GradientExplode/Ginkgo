#ifndef _gkg_dmri_bundle_distance_BundleDistanceFunctorFactory_h_
#define _gkg_dmri_bundle_distance_BundleDistanceFunctorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-bundle-distance/BundleDistanceFunctor.h>
#include <map>
#include <vector>


namespace gkg
{


class BundleDistanceFunctorFactory : public Singleton<
                                                  BundleDistanceFunctorFactory >
{

  public:

    typedef BundleDistanceFunctor*
              ( *BundleDistanceFunctorCreator )( const std::vector< double >&  );

    ~BundleDistanceFunctorFactory();

    bool registerBundleDistanceFunctor(
                      const std::string& name,
                      BundleDistanceFunctorCreator fiberDistanceFunctorCreator,
                      const std::string& scalarParameterHelp );

    BundleDistanceFunctor*
      create( const std::string& name,
              const std::vector< double >& scalarParameters ) const;

    bool hasBundleDistanceFunctor( const std::string& name ) const;
    std::set< std::string > getNames() const;
    std::string getNameListHelp() const;
    std::string getScalarParameterHelp( const std::string& name ) const;

  protected:

    friend class Singleton< BundleDistanceFunctorFactory >;

    BundleDistanceFunctorFactory();

    std::map< std::string,
              BundleDistanceFunctorCreator > _bundleDistanceFunctorCreators;
    std::map< std::string, std::string > _scalarParameterHelps;

};


}


#endif

