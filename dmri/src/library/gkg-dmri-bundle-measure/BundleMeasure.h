#ifndef _gkg_dmri_bundle_measure_BundleMeasure_h_
#define _gkg_dmri_bundle_measure_BundleMeasure_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Curve3dMap.h>


namespace gkg
{


template < class M >
class BundleMeasure : public RCObject
{

  public:

    typedef Curve3dMap< float > Bundle;

    virtual ~BundleMeasure();

    virtual M get( const Bundle& bundle ) const = 0;

  protected:

    BundleMeasure();

};


}


#endif
