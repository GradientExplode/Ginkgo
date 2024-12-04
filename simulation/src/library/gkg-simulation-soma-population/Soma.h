#ifndef _gkg_simulation_soma_population_Soma_h_
#define _gkg_simulation_soma_population_Soma_h_


#include <gkg-simulation-virtual-tissue/Cell.h>
#include <gkg-core-pattern/RCPointer.h>
#include <vector>


namespace gkg
{


class SomaPopulation;


class Soma : public Cell
{

  public:

    Soma( int32_t id,
          const SomaPopulation* parent,
          float diameter,
          const Vector3d< float >& center );
    Soma( const Soma& other );
    virtual ~Soma();

    Soma& operator=( const Soma& other );

    float getDiameter() const;
    Vector3d< float > getCenter() const;
    float getVolume() const;

    void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;
    void addDetails();


  protected:

    float _diameter;

};



}


#endif
