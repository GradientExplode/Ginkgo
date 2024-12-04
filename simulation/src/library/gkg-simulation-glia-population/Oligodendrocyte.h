#ifndef _gkg_simulation_glia_population_Oligodendrocyte_h_
#define _gkg_simulation_glia_population_Oligodendrocyte_h_


#include <gkg-simulation-glia-population/GlialCell.h>
#include <gkg-processing-container/BoundingBox.h>


namespace gkg
{


class OligodendrocytePopulation;


class Oligodendrocyte : public GlialCell
{

  public:

    Oligodendrocyte( int32_t id,
                     const OligodendrocytePopulation* parent,
                     Atom* somaAtom,
                     float myelinatedFiberSearchRadius,
                     int32_t processNodeCount,
                     float processAtomOversamplingRatio,
                     int32_t processMinimumSegmentPointCount,
                     float processRadius,
                     bool processHasTortuosity,
                     float processTortuosityMagnitude,
                     float processTortuosityAngularDispersionInDegrees,
                     float processTortuosityWaveLength );
    Oligodendrocyte( const Oligodendrocyte& other );
    virtual ~Oligodendrocyte();

    Oligodendrocyte& operator=( const Oligodendrocyte& other );

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

    void addInteractionWithOtherPopulations();

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
