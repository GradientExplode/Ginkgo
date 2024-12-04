#ifndef _gkg_simulation_diffusion_process_MonteCarloDiffusionProcess_h_
#define _gkg_simulation_diffusion_process_MonteCarloDiffusionProcess_h_


#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-mri-sequence/MRISequencePool.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-simulation-kokkos/DiffusionProcessKernels.h>
#include <vector>
#include <fstream>


namespace gkg
{


class MonteCarloDiffusionProcess
{

  public:

    MonteCarloDiffusionProcess( RCPointer< VirtualTissue > virtualTissue,
                                const Dictionary& dictionary );
    virtual ~MonteCarloDiffusionProcess();

    float getTimeStepInUs() const;

    void simulate(
                  RCPointer< MRISequencePool > mriSequencePool,
                  const std::string& fileNameDiffusionProcessOutputParticles,
                  const std::string& fileNameDiffusionProcessOutputTrajectories,
                  std::vector< std::vector< float > >&
                                       perSequenceDiffusionWeightedAttenuations,
                  bool verbose,
                  std::ofstream* osLog = 0 ) const;

    void displayInformation( std::ofstream* osLog = 0 ) const;

    void displaySimpleMessage( const std::string& message,
                               bool verbose,
                               std::ofstream* osLog = 0,
                               bool applyEndl = false ) const;

  protected:

    void setParticleInitializationType(
                             const std::string& theParticleInitializationType );

    void setParticleMembraneInteractionType(
                        const std::string& theParticleMembraneInteractionType );



    RCPointer< VirtualTissue > _virtualTissue;
    DiffusionProcessParticleInitializer::Type _particleInitializationType; 
    DiffusionProcessMonteCarloAndMRISimulator::ParticleToMembraneInteractionType
                                             _particleToMembraneInteractionType; 
    int32_t _sessionCount;
    float _particleBulkDiffusivityInUm2PerUs;
    int32_t _particleCountPerSession;
    Vector3d< double > _gridResolutionInUm;
    float _timeStepInUs;
    float _totalDurationInUs;

    float _bulkStepLength;
    int32_t _stepCount;

    float _particleBackupPercentage;
    std::vector< int32_t > _randomParticleSelection;

};


}


#endif
