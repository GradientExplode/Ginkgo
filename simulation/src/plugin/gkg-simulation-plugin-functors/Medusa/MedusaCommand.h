#ifndef _gkg_simulation_plugin_functors_Medusa_MedusaCommand_h_
#define _gkg_simulation_plugin_functors_Medusa_MedusaCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class MedusaCommand : public Command,
                      public Creator2Arg< MedusaCommand,
                                          Command,
                                          int32_t,
                                          char** >,
                      public Creator1Arg< MedusaCommand,
                                          Command,
                                          const Dictionary& >
{

  public:

    MedusaCommand( int32_t argc,
                   char* argv[], 
                   bool loadPlugin = false,
                   bool removeFirst = true );
    MedusaCommand(
                  const std::string& fileNameGeometryInputDictionary,
                  const std::string& fileNameGeometryInputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMesh,
                  const std::string& fileNameGeometryOutputEnergyPlot,
                  const std::string& fileNameTissuePropertyInputDictionary,
                  const std::string& 
                      fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
                  const std::string& fileNameDiffusionProcessInputDictionary,
                  const std::string& fileNameDiffusionProcessOutputParticles,
                  const std::string& fileNameDiffusionProcessOutputTrajectories,
                  const std::string& fileNameMRISequenceInputDictionary,
                  const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                  const std::string& fileNameMRISequenceOutputPythonAttenuation,
                  const std::string& fileNameMRISequenceOutputVolumeAttenuation,
                  const std::string& outputVolumeFormat,
                  bool outputVolumeAscii,
                  const std::string& fileNameLog,
                  bool applyKokkosInitialize,
                  bool applyKokkosFinalize,
                  bool verbose );
    MedusaCommand( const Dictionary& parameters );
    virtual ~MedusaCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MedusaCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< MedusaCommand,
                              Command,
                              const Dictionary& >;

    void parse();

    void execute( const std::string& fileNameGeometryInputDictionary,
                  const std::string& fileNameGeometryInputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMesh,
                  const std::string& fileNameGeometryOutputEnergyPlot,
                  const std::string& fileNameTissuePropertyInputDictionary,
                  const std::string& 
                      fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
                  const std::string& fileNameDiffusionProcessInputDictionary,
                  const std::string& fileNameDiffusionProcessOutputParticles,
                  const std::string& fileNameDiffusionProcessOutputTrajectories,
                  const std::string& fileNameMRISequenceInputDictionary,
                  const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                  const std::string& fileNameMRISequenceOutputPythonAttenuation,
                  const std::string& fileNameMRISequenceOutputVolumeAttenuation,
                  const std::string& outputVolumeFormat,
                  bool outputVolumeAscii,
                  const std::string& fileNameLog,
                  bool applyKokkosInitialize,
                  bool applyKokkosFinalize,
                  bool verbose );

};


}


#endif
