#ifndef _gkg_processing_morphology_ConnectedComponents_h_
#define _gkg_processing_morphology_ConnectedComponents_h_


#include <gkg-processing-morphology/Neighborhood3d.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <map>


namespace gkg
{


template < class T > class Volume;
template < class T > class TestFunction;


template < class IN, class OUT >
class ConnectedComponents
{

  public:

    ConnectedComponents( const TestFunction< IN >& testFunction,
                         const OUT& background,
                         size_t minimumComponentSize,
                         size_t maximumComponentCount,
                         bool binary,
                         Neighborhood3d::Type neighborhoodType,
                         int32_t stride = 1 );
    virtual ~ConnectedComponents();

    std::map< OUT, size_t > getComponents( const Volume< IN >& inputVolume,
                                           Volume< OUT >& connectedComponents,
                                           int32_t rank = 0,
                                           bool verbose = false ) const;
  protected:

    void reallocate( const Volume< IN >& in, Volume< OUT >& out ) const;

    const TestFunction< IN >& _testFunction;
    OUT _background;
    size_t _minimumComponentSize;
    size_t _maximumComponentCount;
    int32_t _stride;
    bool _binary;
    std::list< Vector3d< int32_t > > _neighborOffsets;

};


}


#endif
