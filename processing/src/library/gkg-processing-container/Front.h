#ifndef _gkg_processing_container_Front_h_
#define _gkg_processing_container_Front_h_

#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <deque>
#include <set>


namespace gkg
{


template< class T >
class Front
{

  public:

    Front( RCPointer< Volume< T > > labelVolume,
           const T& label,
           int32_t stride );
    Front( const Front& other );
    virtual ~Front();

    Front< T >& operator=( const Front< T >& other );
    int32_t getSiteCount() const;
    const Vector3d< int32_t >& getSite( int32_t index ) const;
    const std::deque< Vector3d< int32_t > >& getSites() const;
    const std::set< Vector3d< int32_t >, Vector3dCompare< int32_t > >& 
            getUniqueSites() const;
    bool hasSite( const Vector3d< int32_t >& site ) const;

    void addSite( const Vector3d< int32_t >& site );
    void removeSite( const Vector3d< int32_t >& site );

    void getRandomSite( Vector3d< int32_t >& site ) const;

  protected:

    std::deque< Vector3d< int32_t > > _sites;
    std::set< Vector3d< int32_t >, Vector3dCompare< int32_t > > _uniqueSites;
    RCPointer< Volume< T > > _labelVolume;
    T _label;
    RandomGenerator _randomGenerator;
    BoundingBox< int32_t > _boundingBox;
    std::list< Vector3d< int32_t > > _neighbors;

};


}


#endif
