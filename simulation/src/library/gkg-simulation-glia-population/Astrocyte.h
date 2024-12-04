#ifndef _gkg_simulation_glia_population_Astrocyte_h_
#define _gkg_simulation_glia_population_Astrocyte_h_


#include <gkg-simulation-glia-population/GlialCell.h>
#include <gkg-processing-container/BoundingBox.h>


namespace gkg
{


class AstrocytePopulation;


class Astrocyte : public GlialCell
{

  public:

    Astrocyte( int32_t id,
               const AstrocytePopulation* parent,
               Atom* somaAtom,
               float totalRadius,
               int32_t processNodeCount,
               float processBalancingFactor,
               float processAtomOversamplingRatio,
               int32_t processMinimumSegmentPointCount,
               float processRadius,
               bool processHasTortuosity,
               float processTortuosityMagnitude,
               float processTortuosityAngularDispersionInDegrees,
               float processTortuosityWaveLength );
    Astrocyte( const Astrocyte& other );
    virtual ~Astrocyte();

    Astrocyte& operator=( const Astrocyte& other );

    float getProcessAtomOversamplingRatio() const;
    int32_t getProcessMinimumSegmentPointCount() const;
    float getProcessRadius() const;
    bool getProcessHasTortuosity() const;
    float getProcessTortuosityMagnitude() const;
    float getProcessTortuosityAngularDispersionInDegrees() const;
    float getProcessTortuosityWaveLength() const;

    void fillMeshAccumulators( 
                           std::vector< MeshAccumulator< int32_t, float, 3U > >&
                                                       meshAccumulators ) const;

  protected:

    float _processAtomOversamplingRatio;
    int32_t _processMinimumSegmentPointCount;
    float _processRadius;
    bool _processHasTortuosity;
    float _processTortuosityMagnitude;
    float _processTortuosityAngularDispersionInDegrees;
    float _processTortuosityWaveLength;

};



}


#endif
