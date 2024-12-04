#ifndef _gkg_simulation_mri_sequence_MRISequence_h_
#define _gkg_simulation_mri_sequence_MRISequence_h_


#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-numericalanalysis/IntegrableFunction.h>
#include <gkg-core-object/Dictionary.h>
#include <vector>
#include <fstream>


//
// gyromagnetic ratio in Hz/T
//
#define GYROMAGNETIC_RATIO   42.576 * 1e6



namespace gkg
{


class MRISequence;
class NumericalAnalysisImplementationFactory;


class MRISequenceIntegrableFunction : public IntegrableFunction
{

  public:

    MRISequenceIntegrableFunction( const MRISequence* mriSequence );
    virtual ~MRISequenceIntegrableFunction();

    double getValue( double timeInUs ) const;

  protected:

    const MRISequence* _mriSequence;

};


class MRISequence
{

  public:

    virtual ~MRISequence();

    float getEchoTimeInUs() const;
    float getMaximumGradientSlewRateInTeslaPerMeterPerSecond() const;

    int32_t getTimeStepCount() const;

    int32_t getGradientMagnitudeSetCount() const;
    int32_t getGradientMagnitudeCount( int32_t setIndex ) const;
    float getGradientMagnitude( int32_t setIndex,
                                int32_t gradientMagnitudeIndex ) const;

    int32_t getGradientOrientationCount( int32_t setIndex ) const;
    const OrientationSet& getGradientOrientationSet( int32_t setIndex ) const;

    int32_t getTotalGradientMagnitudeCount() const;
    int32_t getTotalGradientOrientationCount(
                                  int32_t totalGradientOrientationIndex ) const;

    float getNoiseStandardDeviation() const;

    const Vector3d< float >& 
     getPhaseShift( int32_t timeStepIndex,
                    int32_t globalGradientMagnitudeIndex,
                    int32_t orientationIndex ) const;

    virtual void getNormalizedValueAt(
                                     float timeInUs, 
                                     float& normalizedGradientValue ) const = 0;

    virtual void getNormalizedIntegralValueAt(
                                 float timeInUs, 
                                 float& normalizedIntegralGradientValue ) const;

    void computePhaseShifts();

    float getBValueInSecondPerMm2( int32_t setIndex,
                                   int32_t gradientMagnitudeIndex,
                                   int32_t orientationIndex ) const;

    virtual void displayInformation( std::ofstream* osLog ) const;
    
    virtual void saveNormalizedProfile( const std::string& fileName ) const;

    virtual void saveNormalizedIntegralProfile(
                                            const std::string& fileName ) const;

    virtual void saveNormalizedDiffusionGradientProfile(
                                            const std::string& fileName,
                                            bool withBrukerHeader ) const;

    virtual void saveDiffusionInformation( const std::string& fileName ) const;

  protected:

    MRISequence( const gkg::Dictionary& parameters,
                 float timeStepInUs,
                 bool verbose,
                 std::ofstream* osLog );

    MRISequenceIntegrableFunction _integrableFunction;
    NumericalAnalysisImplementationFactory* _factory;

    float _echoTimeInUs;
    float _timeOffsetToDiffusionModuleInUs;
    float _maximumGradientSlewRateInTeslaPerMeterPerSecond;
    float _timeStepInUs;
    int32_t _timeStepCount;

    int32_t _setCount;

    std::vector< int32_t > _gradientMagnitudeCounts;
    std::vector< std::vector< float > > _gradientMagnitudeSets;
    float _maximumAbsoluteGradientMagnitudeInMilliTeslaPerMeter;

    std::vector< int32_t > _gradientOrientationCounts;
    std::vector< OrientationSet > _gradientOrientationSets;

    int32_t _totalGradientMagnitudeCount;    
    std::vector< int32_t > _totalGradientOrientationCounts;    

    std::vector<                      // -> timeStepIndex
      std::vector<                    // -> totalGradientMagnitudeIndex
        std::vector<                  // -> orientationIndex
          gkg::Vector3d< float > > > > _phaseShifts;
          
    float _rampTimeInUs;

    int32_t _diffusionGradientWaveformStartingIndex;
    int32_t _diffusionGradientWaveformEndingIndex;

    float _noiseStandardDeviation;

};


}


#endif


