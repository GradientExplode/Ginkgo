#ifndef _gkg_processing_topology_TopologicalComponent_h_
#define _gkg_processing_topology_TopologicalComponent_h_


#include <gkg-processing-morphology/Neighborhood3d.h>


namespace gkg
{


class TopologicalComponent
{

  public:

    enum Adjacency
    {

      Adjacency3d_6,
      Adjacency3d_18,
      Adjacency3d_26

    };

    enum Connectivity
    {

      Connectivity3d_6,
      Connectivity3d_18,
      Connectivity3d_26

    };

    TopologicalComponent( Neighborhood3d::Type type,
                          Adjacency adjacency,
                          Connectivity connectivity );

    virtual ~TopologicalComponent();

    int32_t getNumberOfComponents( int32_t* neighborhood );
    int32_t getNumberOfComponentsForComplement( int32_t* neighborhood );

  private:

    int32_t getNumber( int32_t* neighborhood );

    int32_t _adjacency;
    int32_t _connectivity;

    int32_t _neighborCount[ 27 ];
    int32_t _relation[ 27 ][ 26 ];

};


}


#endif
