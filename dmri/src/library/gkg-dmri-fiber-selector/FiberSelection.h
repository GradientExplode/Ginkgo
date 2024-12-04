#ifndef _gkg_dmri_fiber_selector_FiberSelection_h_
#define _gkg_dmri_fiber_selector_FiberSelection_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-container/BundleMap.h>
#include <vector>
#include <list>
#include <map>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class FiberSelection : public HeaderedObject
{

  public:

    typedef void ( *Selector )( const std::vector< std::string >&,
                                const std::vector< double >&,
                                RCPointer< BundleMap< L, Compare > >,
                                std::map< L,
                                          std::set< int32_t >,
                                          Compare >& );

    typedef typename BundleMap< L, Compare >::iterator iterator;
    typedef typename BundleMap< L, Compare >::const_iterator const_iterator;
    typedef typename BundleMap< L, Compare >::reverse_iterator reverse_iterator;
    typedef typename BundleMap< L, Compare >::const_reverse_iterator
                                                         const_reverse_iterator;

    FiberSelection();
    FiberSelection( RCPointer< BundleMap< L, Compare > > inputBundleMap );
    FiberSelection( const FiberSelection< L, Compare >& other );
    virtual ~FiberSelection();

    FiberSelection< L, Compare >& operator=(
                                    const FiberSelection< L, Compare >& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator lower_bound( const L& label );
    iterator upper_bound( const L& label );

    const_iterator lower_bound( const L& label ) const;
    const_iterator upper_bound( const L& label ) const;

    std::pair< iterator, iterator > equal_range( const L& label );
    std::pair< const_iterator, 
               const_iterator > equal_range( const L& label ) const;

    iterator find( const L& label );

    void setInputBundleMap( RCPointer< BundleMap< L, Compare > > bundleMap );

    void addSelector( const std::string& name,
                      const std::vector< std::string >& stringParameters,
                      const std::vector< double >& scalarParameters );

    BundleMap< L, Compare >& getSelectedBundleMap();

  protected:


    std::map< std::string, std::list< Selector > > _selectorLists;
    RCPointer< BundleMap< L, Compare > > _inputBundleMap;
    // _selectedBundleMap[ bundle_label ]
    BundleMap< L, Compare > _selectedBundleMap;

};


}


#endif
