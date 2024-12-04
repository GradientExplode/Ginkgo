#ifndef _gkg_dmri_bundle_measure_BundleDensityMap_h_
#define _gkg_dmri_bundle_measure_BundleDensityMap_h_


#include <gkg-dmri-bundle-measure/BundleMeasure.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-coordinates/Vector3d.h>


namespace gkg
{


class BundleDensityMap : public BundleMeasure< Volume< float > >
{

  public:

    typedef BundleMeasure< Volume< float > >::Bundle Bundle;

    BundleDensityMap( const Vector3d< int32_t >& size,
                      const Vector3d< double >& resolution );
    virtual ~BundleDensityMap();

    Volume< float > get( const Bundle& bundle ) const;

  protected:

    Vector3d< int32_t > _size;
    Vector3d< double > _resolution;

};


}


#endif
