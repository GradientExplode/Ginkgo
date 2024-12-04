#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorFactory_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorFactory_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator.h>
#include <string>
#include <map>
#include <list>
#include <vector>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMapOperatorFactory : public Singleton<
                                        BundleMapOperatorFactory< L, Compare > >
{

  public:

    typedef BundleMapOperator< L, Compare >*
              ( *BundleMapOperatorCreator )( const std::vector< std::string >&,
                                             const std::vector< double >&,
                                             bool );

    ~BundleMapOperatorFactory();

    bool registerBundleMapOperator(
                            const std::string& name,
                            BundleMapOperatorCreator bundleMapOperatorCreator,
                            const std::string& stringParameterHelp,
                            const std::string& scalarParameterHelp );

    BundleMapOperator< L, Compare >*
      create( const std::string& name,
              const std::vector< std::string >& stringParameters,
              const std::vector< double >& scalarParameters,
              bool verbose ) const;

    bool hasBundleMapOperator( const std::string& name ) const;
    std::set< std::string > getNames() const;
    std::string getNameListHelp() const;
    std::string getStringParameterHelp( const std::string& name ) const;
    std::string getScalarParameterHelp( const std::string& name ) const;

  protected:

    friend class Singleton< BundleMapOperatorFactory< L, Compare > >;

    BundleMapOperatorFactory();

    std::map< std::string,
              BundleMapOperatorCreator > _bundleMapOperatorCreators;
    std::map< std::string, std::string > _stringParameterHelps;
    std::map< std::string, std::string > _scalarParameterHelps;

};


}


#endif
