#ifndef _gkg_dmri_microstructure_axon_mapping_CanonicalWatsonSphericalHarmonicsCache_h_
#define _gkg_dmri_microstructure_axon_mapping_CanonicalWatsonSphericalHarmonicsCache_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <gkg-communication-thread/Mutex.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <vector>


#define CANONICAL_WATSON_SPHERICAL_HARMONICS_BIN_COUNT 1000


namespace gkg
{


class NumericalAnalysisImplementationFactory;


class CanonicalWatsonSphericalHarmonicsCache :
                      public Singleton< CanonicalWatsonSphericalHarmonicsCache >
{

  public:

    ~CanonicalWatsonSphericalHarmonicsCache();

    void update( const std::vector< gkg::Vector3d< float > >& orientations,
                 int32_t maximumSphericalHarmonicsOrder,
                 double lowerKappa,
                 double upperKappa );

    int32_t getMaximumSphericalHarmonicsOrder() const;
    const std::vector< double >& getDecomposition( double kappa ) const;


  protected:

    friend class Singleton< CanonicalWatsonSphericalHarmonicsCache >;

    CanonicalWatsonSphericalHarmonicsCache();

    NumericalAnalysisImplementationFactory* _factory;
    const std::vector< Vector3d< float > >* _orientations;
    int32_t _orientationCount;
    int32_t _maximumSphericalHarmonicsOrder;
    double _lowerKappa;
    double _upperKappa;

    std::vector< std::vector< double > > _canonicalDecompositions;

    Mutex _mutex;

};



}



#endif
