#ifndef _gkg_processing_container_RankSiteLut_h_
#define _gkg_processing_container_RankSiteLut_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <map>
#include <iostream>


namespace gkg
{


template < class R, class S, class Compare > class SiteMap;


//
// specific index type when texture map corresponds to a set of rank(s)/site(s)
//
template < class R, class S, class RankCompare = std::less< R > >
struct RankSite
{

  RankSite();
  RankSite( const R& rank, const Vector3d< S >& site );
  RankSite( const RankSite< R, S, RankCompare >& other );

  RankSite< R, S, RankCompare >&
    operator=( const RankSite< R, S, RankCompare >& other );

  R rank;
  Vector3d< S > site;

};


template < class R, class S, class RankCompare = std::less< R > >
struct RankSiteCompare /*: public std::binary_function<
                                                  RankSite< R, S, RankCompare >,
                                                  RankSite< R, S, RankCompare >,
                                                  bool >
                        deprecated in c++17 */
{

  typedef RankSite< R, S, RankCompare > first_argument_type;
  typedef RankSite< R, S, RankCompare > second_argument_type;
  typedef bool result_type;

  bool operator()( const RankSite< R, S, RankCompare >& rankSite1,
                   const RankSite< R, S, RankCompare >& rankSite2 ) const
  {

    RankCompare rankCompare;
    Vector3dCompare< S > siteCompare;
    return rankCompare( rankSite1.rank, rankSite2.rank ) ||
           ( ( rankSite1.rank == rankSite2.rank  ) &&
             siteCompare( rankSite1.site, rankSite2.site ) );

  }

};


//
// class RankSiteLut
//
template < class R, class S, class RankCompare = std::less< R > >
class RankSiteLut
{

  public:

    RankSiteLut();
    RankSiteLut( const SiteMap< R, S, RankCompare >& siteMap );
    RankSiteLut( const RankSiteLut< R, S, RankCompare >& other );
    virtual ~RankSiteLut();

    RankSiteLut< R, S, RankCompare >&
      operator=( const RankSiteLut< R, S, RankCompare >& other );

    int32_t getCount() const;

    int32_t getIndex( const R& rank, const Vector3d< S >& site ) const;
    int32_t getIndex( const RankSite< R, S, RankCompare >& rankSite ) const;

    const RankSite< R, S, RankCompare >& getRankSite( int32_t index ) const;

  protected:

    std::map< RankSite< R, S, RankCompare >,
              int32_t,
              RankSiteCompare< R, S, RankCompare > > _indices;
    std::map< int32_t, RankSite< R, S, RankCompare > > _rankSites;

};


}


namespace std
{


template < class R, class S, class RankCompare >
ostream& 
operator<<( ostream& os, const gkg::RankSite< R, S, RankCompare >& thing );


}


#endif
