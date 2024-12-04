#ifndef _gkg_processing_morphology_MaurerDistanceMapper_h_
#define _gkg_processing_morphology_MaurerDistanceMapper_h_


#include <gkg-core-cppext/StdInt.h>

#include <vector>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN >
class MaurerDistanceMapper
{

  public:

    MaurerDistanceMapper( const TestFunction< IN >& testFunction );
    virtual ~MaurerDistanceMapper();

    template < class OUT >
    void getDistance( const Volume< IN >& volume,
                      Volume< OUT >& distance,
                      bool verbose = false ) const;

  protected:

    template < class OUT >
    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;

    const TestFunction< IN >& _testFunction;

};


}


#endif
