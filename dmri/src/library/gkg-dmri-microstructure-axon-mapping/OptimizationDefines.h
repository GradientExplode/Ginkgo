#ifndef _gkg_dmri_microstructure_axon_mapping_OptimizationDefines_h_
#define _gkg_dmri_microstructure_axon_mapping_OptimizationDefines_h_


////////////////////////////////////////////////////////////////////////////////
// Defines for optimizer parameters
////////////////////////////////////////////////////////////////////////////////

#define NoiseStandardDeviation                       0

#define ApplyQuickSearchOptimizer                    1
#define QuickSearchStepCount                         2

#define ApplyNlpOptimizer                            3
#define NlpMaximumIterationCount                     4
#define NlpMaximumSizeOfTheSimplex                   5

#define ApplyMcmcOptimizer                           6
#define McmcBurninCount                              7
#define McmcSampleCount                              8
#define McmcIntervalCount                            9
#define McmcMaximumIterationCount                   10

#define OptimizationParameters_End                  11


#endif

