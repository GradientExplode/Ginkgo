#ifndef _gkg_processing_container_SiteMap_h_
#define _gkg_processing_container_SiteMap_h_


#include <gkg-core-object/HeaderedObject.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/BoundingBox.h>
#include <list>
#include <set>
#include <vector>
#include <map>
#include <iostream>


namespace gkg
{



template < class R, class S, class Compare = std::less< R > >
class SiteMap : public HeaderedObject
{

  public:

    typedef typename std::map< R,
              std::list< Vector3d< S > >,
              Compare >::iterator iterator;
    typedef typename std::map< R,
              std::list< Vector3d< S > >,
              Compare >::const_iterator const_iterator;
    typedef typename std::map< R,
              std::list< Vector3d< S > >,
              Compare >::reverse_iterator reverse_iterator;
    typedef typename std::map< R,
              std::list< Vector3d< S > >,
              Compare >::const_reverse_iterator const_reverse_iterator;

    SiteMap();
    SiteMap( const SiteMap< R, S, Compare >& other );
    virtual ~SiteMap();

    SiteMap< R, S, Compare >&
      operator=( const SiteMap< R, S, Compare >& other );

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    iterator lower_bound( const R& rank );
    iterator upper_bound( const R& rank );

    const_iterator lower_bound( const R& rank ) const;
    const_iterator upper_bound( const R& rank ) const;

    std::pair< iterator, iterator > equal_range( const R& rank );
    std::pair< const_iterator,
               const_iterator > equal_range( const R& rank ) const;

    void addSite( const R& rank, const Vector3d< S >& site );
    void addSites( const R& rank,
                   const std::set< Vector3d< S >,
                                   Vector3dCompare< S > >& sites );
    void addSites( const R& rank,
                   const std::list< Vector3d< S > >& sites );
    void addSites( const R& rank,
                   const std::vector< Vector3d< S > >& sites );
    bool hasSite( const R& rank, const Vector3d< S >& site ) const;

    void removeSite( const R& rank, const Vector3d< S >& site );
    void removeSites( const R& rank );

    int32_t getRankCount() const;
    const std::set< R, Compare >& getRanks() const;
    int32_t getSiteCount( const R& rank ) const;
    const std::list< Vector3d< S > >& getSites( const R& rank ) const;

    iterator find( const R& rank );

    int32_t getIndex( const R& rank ) const;
    const R& getRank( int32_t index ) const;
    const std::map< R, int32_t, Compare >& getRankToIndexLut() const;
    const std::map< int32_t, R >& getIndexToRankLut() const;

    BoundingBox< S > getBoundingBox( const R& rank ) const;

    void clear();

  protected:

    void updateHeader();

    std::map< R,
              std::list< Vector3d< S > >,
              Compare > _sites;
    std::set< R, Compare > _ranks;
    std::map< R, int32_t, Compare > _rankToIndexLut;
    std::map< int32_t, R > _indexToRankLut;

};


}


namespace std
{


template < class R, class S, class Compare >
ostream& operator<<( ostream& os, const gkg::SiteMap< R, S, Compare >& thing );


}


#endif
