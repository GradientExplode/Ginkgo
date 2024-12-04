#ifndef _gkg_dmri_tractography_TractographyAlgorithmFactory_h_
#define _gkg_dmri_tractography_TractographyAlgorithmFactory_h_


#include <string>
#include <map>
#include <list>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-dmri-tractography/TractographyAlgorithm.h>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class TractographyAlgorithmFactory : public Singleton<
                                    TractographyAlgorithmFactory< L, Compare > >
{

  public:

    typedef TractographyAlgorithm< L, Compare >*
              ( *TractographyAlgorithmCreator )(
                                            const Vector3d< int32_t >&,
                                            const Vector3d< double >&,
                                            const std::vector< double >&,
                                            const std::vector< std::string >& );

    ~TractographyAlgorithmFactory();

    bool registerTractographyAlgorithm(
                    const std::string& name,
                    TractographyAlgorithmCreator tractographyAlgorithmCreator );

    TractographyAlgorithm< L, Compare >*
      create( const std::string& name,
              const Vector3d< int32_t >& size,
              const Vector3d< double >& resolution,
              const std::vector< double >& scalarParameters,
              const std::vector< std::string >& stringParameters ) const;

    bool hasTractographyAlgorithm( const std::string& name ) const;
    std::list< std::string > getNames() const;

  protected:

    friend class Singleton< TractographyAlgorithmFactory< L, Compare > >;

    TractographyAlgorithmFactory();

    std::map< std::string,
              TractographyAlgorithmCreator > _tractographyAlgorithmCreators;

};


}


#endif
