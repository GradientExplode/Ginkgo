#ifndef _gkg_dmri_bundle_distance_BundleDistanceFunctor_h_
#define _gkg_dmri_bundle_distance_BundleDistanceFunctor_h_


#include <gkg-processing-container/Curve3dMap.h>


namespace gkg
{


class BundleDistanceFunctor
{

  public:

    virtual ~BundleDistanceFunctor();

    virtual float getDistance( const Curve3dMap< float >& bundle1,
                               const Curve3dMap< float >& bundle2 ) const = 0;

  protected:

    BundleDistanceFunctor();

};


}


#endif

