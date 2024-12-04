#ifndef _gkg_dartel_bundle_DartelBundleTransformation_h_
#define _gkg_dartel_bundle_DartelBundleTransformation_h_


#include <gkg-dartel-core/DartelTransformation.h>
#include <gkg-dmri-container/BundleMap.h>


namespace gkg
{


template < class L, class S >
class DartelBundleTransformation : public DartelTransformation< S, float >
{

  public:

    DartelBundleTransformation( int32_t k );
    virtual ~DartelBundleTransformation();

    void wrap( const BundleMap< L >& inBundleMap,
               const Volume< S >& inVolumeProxy,
               const Volume< float >& flowField, 
               BundleMap< L >& outBundleMap );
    void unwrap( const BundleMap< L >& inBundleMap,
                 const Volume< S >& inVolumeProxy,
                 const Volume< float >& flowField, 
                 const Volume< S >& templateProxy,
                 BundleMap< L >& outBundleMap );

};


}


#endif
