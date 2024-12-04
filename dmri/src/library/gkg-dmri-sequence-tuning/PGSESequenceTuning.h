#ifndef _gkg_dmri_sequence_tuning_PGSESequenceTuning_h_
#define _gkg_dmri_sequence_tuning_PGSESequenceTuning_h_




namespace gkg
{



void getPGSEParameters( double maximumGradientAmplitude,      // in mT/m
                             double minimumSlewRate,          // in T/m/s
                             double gradientResolution,       // in us
                             double timeSeparation,           // in ms
                             double timeBeforeEcho,           // in ms
                             double bValue,                   // in s/mm2
                             double& littleDelta,             // in ms
                             double& bigDelta,                // in ms
                             double& rampWidth,               // in ms
                             double& diffusionTime,           // in ms
                             double& echoTime );              // in ms

}


#endif

