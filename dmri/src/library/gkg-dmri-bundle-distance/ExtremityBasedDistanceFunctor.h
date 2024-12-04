#ifndef _gkg_dmri_bundle_distance_ExtremityBasedDistanceFunctor_h_
#define _gkg_dmri_bundle_distance_ExtremityBasedDistanceFunctor_h_


#include <gkg-dmri-bundle-distance/BundleDistanceFunctor.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class ExtremityBasedDistanceFunctor : 
             public BundleDistanceFunctor,
             public Creator1Arg< ExtremityBasedDistanceFunctor,
                                 BundleDistanceFunctor,
                                 const std::vector< double >& >
{

  public:

    virtual ~ExtremityBasedDistanceFunctor();

    float getDistance( const Curve3dMap< float >& bundle1,
                       const Curve3dMap< float >& bundle2 ) const;

    static std::string getStaticName();
    static std::string getScalarParameterHelp();

  protected:


    friend struct Creator1Arg< ExtremityBasedDistanceFunctor,
                               BundleDistanceFunctor,
                               const std::vector< double >& >;

    ExtremityBasedDistanceFunctor(
                                const std::vector< double >& scalarParameters );

};


}


#endif

