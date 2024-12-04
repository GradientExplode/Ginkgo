#ifndef _gkg_processing_graph_DijkstraAlgorithm_h_
#define _gkg_processing_graph_DijkstraAlgorithm_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <vector>
#include <list>


namespace gkg
{


class DijkstraAlgorithm
{

  public:

    DijkstraAlgorithm(
              const std::vector< std::list< std::pair< int32_t, float > > >&
                                                                  adjacencyList,
              const std::list< int32_t >& sourceVertices );
    virtual ~DijkstraAlgorithm();

    const std::vector< float >& getDistances() const;

  protected:

    std::vector< float > _distances;

};


}


#endif
