#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperator_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperator_h_


#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-container/BundleMap.h>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMapOperator
{

  public:

    virtual ~BundleMapOperator();

    virtual void startContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap ) = 0;
    virtual void addContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap ) = 0;
    virtual void endContribution(
       RCPointer< BundleMap< L, Compare > > inputBundleMap,
       RCPointer< BundleMap< L, Compare > > outputBundleMap ) = 0;

    virtual std::string getName() const = 0;

    virtual bool saveOutputBundleMap() const;

  protected:

    BundleMapOperator( bool verbose );

    bool _verbose;

};


}


#endif
