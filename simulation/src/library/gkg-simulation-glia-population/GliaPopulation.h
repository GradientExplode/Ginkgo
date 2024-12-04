#ifndef _gkg_simulation_glia_population_GliaPopulation_h_
#define _gkg_simulation_glia_population_GliaPopulation_h_


#include <gkg-simulation-virtual-tissue/Population.h>
#include <gkg-simulation-distribution/ParameterDistribution.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


class GliaPopulation : public Population
{

  public:

    virtual ~GliaPopulation();

    GliaPopulation( const GliaPopulation& other );

    GliaPopulation& operator=( const GliaPopulation& other );


  protected:

    GliaPopulation( int32_t id,
                    const VirtualTissue* parent,
                    const Dictionary& parameters,
                    bool verbose,
                    std::ofstream* osLog );

    RCPointer< ParameterDistribution > _somaDiameterDistribution;

    float _processAtomOversamplingRatio;
    int32_t _processMinimumSegmentPointCount;
    RCPointer< ParameterDistribution > _processDiameterDistribution;
    RCPointer< ParameterDistribution > _processNodeCountDistribution;
    float _processBalancingFactor;
    bool _processHasTortuosity;
    float _processTortuosityMagnitude;
    float _processTortuosityAngularDispersionInDegrees;
    float _processTortuosityWaveLength;

    float _volumeFraction;

    std::vector< Atom* > _somaAtoms;

};


}


#endif
