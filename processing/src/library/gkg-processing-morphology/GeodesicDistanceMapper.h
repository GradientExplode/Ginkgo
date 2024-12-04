#ifndef _gkg_processing_morphology_GeodesicDistanceMapper_h_
#define _gkg_processing_morphology_GeodesicDistanceMapper_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <vector>


namespace gkg
{


template < class IN >
class GeodesicDistanceMapper
{

  public:

    GeodesicDistanceMapper( const Volume< IN >& mask,
                            bool verbose = false );
    virtual ~GeodesicDistanceMapper();

    template < class OUT >
    void getDistance( Volume< OUT >& distanceMap,
                       Vector3d< int32_t > sourceVertex,
                       bool verbose = false ) const;

  protected:

    void setEdge( int32_t vertex1, int32_t vertex2, float distance );

    std::list< int32_t > 
      getVertexLabels( const Vector3d< int32_t >& site ) const;

    const Volume< IN >& _mask;
    Vector3d< int32_t > _maskSize;
    Vector3d< double > _maskResolution;

    std::vector< std::list< std::pair< int32_t, float > > > _adjacencyGraph;
    Volume< int32_t > _labelledVertexVolume;

};


}


#endif
