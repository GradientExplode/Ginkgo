#ifndef _gkg_processing_container_SiteMapFactory_h_
#define _gkg_processing_container_SiteMapFactory_h_


#include <gkg-core-cppext/StdInt.h>
#include <gkg-core-pattern/Singleton.h>
#include <set>


namespace gkg
{


template < class T > class Volume;
template < class R, class S, class Compare > class SiteMap;
template < class T > class TestFunction;
template < class T > struct Vector3d;


//
// function toSiteHelper
//

template < class S >
void toSiteHelper( const Vector3d< int32_t >& point,
                   const Vector3d< double >& resolution,
                   Vector3d< S >& site );



//
// class SiteMapFactory
//

template < class L, class S, class Compare = std::less< L > >
class SiteMapFactory : public Singleton< SiteMapFactory< L, S, Compare > >
{

  public:

    ~SiteMapFactory();

    SiteMap< L, S, Compare >
      create( const Volume< L >& volume,
              const TestFunction< L >& inBackgroundFunction ) const;
    SiteMap< L, S, Compare >
      create( const Volume< L >& volume,
              const TestFunction< L >& inBackgroundFunction,
              const std::set< L, Compare >& selectedLabels ) const;
    SiteMap< L, S, Compare >
      create( const Volume< L >& volume,
              const TestFunction< L >& inBackgroundFunction,
              const L& selectedLabel ) const;

  private:

    friend class Singleton< SiteMapFactory< L, S, Compare > >;

    SiteMapFactory();

};


}


#endif
