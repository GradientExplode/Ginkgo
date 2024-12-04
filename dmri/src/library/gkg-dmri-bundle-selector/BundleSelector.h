#ifndef _gkg_dmri_bundle_selector_BundleSelector_h_
#define _gkg_dmri_bundle_selector_BundleSelector_h_


#include <gkg-processing-container/Curve3dMap.h>


namespace gkg
{


class BundleSelector
{

  public:

    typedef Curve3dMap< float > Bundle;

    virtual ~BundleSelector();

    virtual bool select( const Bundle& bundle ) const= 0;

  protected:

    BundleSelector();

};


}


#endif
