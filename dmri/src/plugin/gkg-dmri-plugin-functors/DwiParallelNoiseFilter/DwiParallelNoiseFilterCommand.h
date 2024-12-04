#ifndef _gkg_dmri_plugin_functors_DwiParallelNoiseFilter_DwiParallelNoiseFilterCommand_h_
#define _gkg_dmri_plugin_functors_DwiParallelNoiseFilter_DwiParallelNoiseFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiParallelNoiseFilterCommand :
                              public Command,
                              public Creator2Arg< DwiParallelNoiseFilterCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                              public Creator1Arg< DwiParallelNoiseFilterCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DwiParallelNoiseFilterCommand( int32_t argc,
                                   char* argv[],
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    DwiParallelNoiseFilterCommand( 
                            const std::string& fileNameT2,
                            const std::string& fileNameDW,
                            const std::string& fileNameMaskOfSignal,
                            const std::string& fileNameMaskOfNoise,
                            const std::string& fileNameAbacus,
                            int32_t neighborhoodSize,
                            const std::string& fileNameDenoisedT2,
                            const std::string& fileNameDenoisedDW,
                            const std::string& fileNameNormalizedNoiseStdDevMap,
                            int32_t maximumSphericalHarmonicOrder,
                            double laplaceBeltramiFactor,
                            double spatialWeightFactor,
                            double structuralWeightFactor,
                            float noiseStdDev,
                            bool noWeighting,
                            bool onlySpatialWeighting,
                            bool onlyStructuralWeighting,
                            bool verbose );
    DwiParallelNoiseFilterCommand( const Dictionary& parameters );
    virtual ~DwiParallelNoiseFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiParallelNoiseFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiParallelNoiseFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameT2,
                  const std::string& fileNameDW,
                  const std::string& fileNameMaskOfSignal,
                  const std::string& fileNameMaskOfNoise,
                  const std::string& fileNameAbacus,
                  int32_t neighborhoodSize,
                  const std::string& fileNameDenoisedT2,
                  const std::string& fileNameDenoisedDW,
                  const std::string& fileNameNormalizedNoiseStdDevMap,
                  int32_t maximumSphericalHarmonicOrder,
                  double laplaceBeltramiFactor,
                  double spatialWeightFactor,
                  double structuralWeightFactor,
                  float noiseStdDev,
                  bool noWeighting,
                  bool onlySpatialWeighting,
                  bool onlyStructuralWeighting,
                  bool verbose );

};


}


#endif
