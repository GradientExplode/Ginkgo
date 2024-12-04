#ifndef _gkg_simulation_plugin_functors_MedusaDataToDeepLearningData_MedusaDataToDeepLearningDataCommand_h_
#define _gkg_simulation_plugin_functors_MedusaDataToDeepLearningData_MedusaDataToDeepLearningDataCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class MedusaDataToDeepLearningDataCommand :
                        public Command,
                        public Creator2Arg< MedusaDataToDeepLearningDataCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                        public Creator1Arg< MedusaDataToDeepLearningDataCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    MedusaDataToDeepLearningDataCommand( int32_t argc,
                                         char* argv[],
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    MedusaDataToDeepLearningDataCommand(
                       const std::string& medusaSimulationCampaignDirectoryName,
                       const std::string& outputDeepLearningMatrixFileName,
                       int64_t rotationCount,
                       float noiseStandardDeviation,
                       int64_t sphericalHarmonicsOrder,
                       double regularizationLCurveFactor,
                       bool csv,
                       bool verbose );
    MedusaDataToDeepLearningDataCommand( const Dictionary& parameters );
    virtual ~MedusaDataToDeepLearningDataCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MedusaDataToDeepLearningDataCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< MedusaDataToDeepLearningDataCommand,
                              Command,
                              const Dictionary& >;

    void parse();

    void execute( const std::string& medusaSimulationCampaignDirectoryName,
                  const std::string& outputDeepLearningMatrixFileName,
                  int64_t rotationCount,
                  float noiseStandardDeviation,
                  int64_t sphericalHarmonicsOrder,
                  double regularizationLCurveFactor,
                  bool csv,
                  bool verbose );

};


}


#endif
