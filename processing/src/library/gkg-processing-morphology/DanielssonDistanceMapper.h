#ifndef _gkg_processing_morphology_DanielssonDistanceMapper_h_
#define _gkg_processing_morphology_DanielssonDistanceMapper_h_


#include <gkg-core-cppext/StdInt.h>
#include <vector>


namespace gkg
{


template < class T > class Volume;
template < class T > class Vector3d;
template < class T > class TestFunction;

template < class IN >
class DanielssonDistanceMapper
{

  public:

    DanielssonDistanceMapper( const TestFunction< IN >& testFunction );
    virtual ~DanielssonDistanceMapper();

    void getComponents( const Volume< IN >& volume,
                        Volume< Vector3d< int32_t > >& components,
                        bool verbose = false ) const;
    template < class OUT >
    void getDistance( const Volume< IN >& volume,
                      Volume< OUT >& distance,
                      bool verbose = false ) const;
    template < class OUT >
    void getVoronoi( const Volume< IN >& volume,
                     const TestFunction< IN >& outsideTestFunction,
                     Volume< OUT >& voronoi,
                     const OUT& outsideValue,
                     bool verbose = false ) const;

  protected:

    void get3dComponents( const Volume< IN >& volume,
                          Volume< Vector3d< int32_t > >& components,
                          bool verbose ) const;
    void get2dComponents( const Volume< IN >& volume,
                          Volume< Vector3d< int32_t > >& components,
                          bool verbose ) const;
    template < class OUT >
    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;
    void updateLocalDistance( Volume< Vector3d< int32_t > >& components,
                              int32_t x, int32_t y, int32_t z,
                              const std::vector< int32_t >& offsets ) const;

    const TestFunction< IN >& _testFunction;

};


}


#endif
