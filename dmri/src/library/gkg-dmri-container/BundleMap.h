#ifndef _gkg_dmri_container_BundleMap_h_
#define _gkg_dmri_container_BundleMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-container/Curve3dMap.h>
#include <gkg-processing-container/Volume.h>
#include <list>
#include <set>
#include <vector>
#include <map>
#include <iostream>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class BundleMap : public HeaderedObject
{

  public:

    typedef Curve3dMap< float > Bundle;
    typedef LightCurve3d< float > Fiber;

    typedef typename std::map< L, Bundle, Compare >::iterator
                                                         iterator;
    typedef typename std::map< L, Bundle, Compare >::const_iterator
                                                         const_iterator;
    typedef typename std::map< L, Bundle, Compare >::reverse_iterator
                                                         reverse_iterator;
    typedef typename std::map< L, Bundle, Compare >::const_reverse_iterator
                                                         const_reverse_iterator;

    BundleMap();
    BundleMap( const std::list< L >& labels,
               const std::list< Bundle >& bundles );
    BundleMap( const std::vector< L >& labels,
               const std::vector< Bundle >& bundles );
    BundleMap( const BundleMap< L, Compare >& other ); 
    virtual ~BundleMap();

    BundleMap< L, Compare >& operator=( const BundleMap< L, Compare >& other );

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

    Bundle& addBundle( const L& label, const Bundle& bundle );
    void addBundles( const std::list< L >& labels,
                     const std::list< Bundle >& bundles );
    void addBundles( const std::vector< L >& labels,
                     const std::vector< Bundle >& bundles );
    bool hasBundle( const L& label ) const;

    void addCurve3ds( const L& label,
                      const std::list< LightCurve3d< float > >& lightCurve3ds );
    void addCurve3ds(
                    const L& label,
                    const std::vector< LightCurve3d< float > >& lightCurve3ds );

    void addCurve3ds( const L& label,
                      const std::list< Curve3d< float > >& curve3ds );
    void addCurve3ds( const L& label,
                      const std::vector< Curve3d< float > >& curve3ds );

    void removeBundle( const L& label );
    void clear();


    const std::set< L, Compare >& getLabels() const;

    const Bundle& getBundle( const L& label ) const;
    int32_t getBundleCount() const;

    void setEquidistant( int32_t equidistantPointCount );

    BoundingBox< float > getBoundingBox() const;

    void getDensityMask(
                    Volume< float >& densityMask,
                    float resamplingStep,
                    const Vector3d< double >& outputResolution,
                    bool normalizedDensity,
                    bool verbose ) const;

    void getDensityAndSectionMasks(
                    Volume< float >& densityMask,
                    Volume< int16_t >& sectionMask,
                    std::vector< int32_t >& sectionCounts,
                    const std::vector< LightCurve3d< float > >& centroids,
                    const std::map< std::string, int32_t >& labelToIndexLut,
                    float sectionWidth,
                    float resamplingStep,
                    const Vector3d< double >& outputResolution,
                    bool normalizedDensity,
                    bool verbose ) const;

    int32_t getTotalPointCount() const;
    int32_t getTotalSegmentCount() const;

  protected:

    void updateHeader();

    std::map< L, Bundle, Compare > _bundles;
    std::set< L, Compare > _labels;

};


}


namespace std
{


template < class L, class Compare >
ostream& 
operator<<( ostream& os, const gkg::BundleMap< L, Compare >& thing );


}


#endif

