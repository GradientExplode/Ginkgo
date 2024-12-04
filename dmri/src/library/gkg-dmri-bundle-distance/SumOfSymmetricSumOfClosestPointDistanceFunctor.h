#ifndef _gkg_dmri_bundle_distance_SumOfSymmetricSumOfClosestPointDistanceFunctor_h_
#define _gkg_dmri_bundle_distance_SumOfSymmetricSumOfClosestPointDistanceFunctor_h_


#include <gkg-dmri-bundle-distance/BundleDistanceFunctor.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class SumOfSymmetricSumOfClosestPointDistanceFunctor : 
             public BundleDistanceFunctor,
             public Creator1Arg< SumOfSymmetricSumOfClosestPointDistanceFunctor,
                                 BundleDistanceFunctor,
                                 const std::vector< double >& >
{

  public:

    virtual ~SumOfSymmetricSumOfClosestPointDistanceFunctor();

    float getDistance( const Curve3dMap< float >& bundle1,
                       const Curve3dMap< float >& bundle2 ) const;

    static std::string getStaticName();
    static std::string getScalarParameterHelp();

  protected:


    friend struct Creator1Arg< SumOfSymmetricSumOfClosestPointDistanceFunctor,
                               BundleDistanceFunctor,
                               const std::vector< double >& >;

    SumOfSymmetricSumOfClosestPointDistanceFunctor(
                                const std::vector< double >& scalarParameters );

};


}


#endif

