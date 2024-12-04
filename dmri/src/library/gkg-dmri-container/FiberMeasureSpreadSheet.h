#ifndef _gkg_dmri_container_FiberMeasureSpreadSheet_h_
#define _gkg_dmri_container_FiberMeasureSpreadSheet_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-dmri-container/BundleMap.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <vector>
#include <list>
#include <map>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class FiberMeasureSpreadSheet : public HeaderedObject
{

  public:

    typedef void ( *Functor )( const std::vector< std::string >&,
                               const std::vector< double >&,
                               RCPointer< BundleMap< L, Compare > >,
                               std::map< L,
                                         std::map< int32_t,
                                                   std::map< std::string,
                                                             Matrix > >,
                                         Compare >& );

    typedef typename
      std::map< L,
                std::map< int32_t, std::map< std::string, Matrix > >,
                Compare >::iterator iterator;
    typedef typename
      std::map< L,
                std::map< int32_t, std::map< std::string, Matrix > >,
                Compare >::const_iterator const_iterator;
    typedef typename
      std::map< L,
                std::map< int32_t, std::map< std::string, Matrix > >,
                Compare >::reverse_iterator reverse_iterator;
    typedef typename
      std::map< L,
                std::map< int32_t, std::map< std::string, Matrix > >,
                Compare >::const_reverse_iterator const_reverse_iterator;

    FiberMeasureSpreadSheet();
    FiberMeasureSpreadSheet( RCPointer< BundleMap< L, Compare > > bundleMap );
    FiberMeasureSpreadSheet(
                           const FiberMeasureSpreadSheet< L, Compare >& other );
    virtual ~FiberMeasureSpreadSheet();

    FiberMeasureSpreadSheet< L, Compare >& operator=(
                           const FiberMeasureSpreadSheet< L, Compare >& other );

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

    void setBundleMap( RCPointer< BundleMap< L, Compare > > bundleMap );
    void addMeasure( const std::string& name,
                     const std::vector< std::string >& stringParameters,
                     const std::vector< double >& scalarParameters );
    void addMeasure( const L& bundleLabel,
                     int32_t fiberIndex,
                     const std::string& measureName,
                     const Matrix& measureMatrix );

    const Matrix& getMeasure( const L& bundleLabel,
                              int32_t fiberIndex,
                              const std::string& measureName ) const;

  protected:


    std::map< std::string, std::list< Functor > > _functorLists;
    RCPointer< BundleMap< L, Compare > > _bundleMap;
    // _measures[ bundle_label ][ fiber_index ][ measure_name ]
    std::map< L,
              std::map< int32_t, std::map< std::string, Matrix > >,
              Compare > _measures;

};


}


#endif
