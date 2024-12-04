#ifndef _gkg_simulation_plugin_functors_MedusaGeometryDeoverlaper_MedusaGeometryDeoverlaperCommand_h_
#define _gkg_simulation_plugin_functors_MedusaGeometryDeoverlaper_MedusaGeometryDeoverlaperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <string>


namespace gkg
{


class MedusaGeometryDeoverlaperCommand : public Command,
                      public Creator2Arg< MedusaGeometryDeoverlaperCommand,
                                          Command,
                                          int32_t,
                                          char** >,
                      public Creator1Arg< MedusaGeometryDeoverlaperCommand,
                                          Command,
                                          const Dictionary& >
{

  public:

    MedusaGeometryDeoverlaperCommand( int32_t argc,
                   char* argv[], 
                   bool loadPlugin = false,
                   bool removeFirst = true );
    MedusaGeometryDeoverlaperCommand(
                  const std::string& fileNameGeometryInputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMesh,
                  const std::string& fileNameGeometryOutputEnergyPlot,
                  float maximumForceNorm,
                  int32_t atomSolverMaximumIterationCount,
                  int32_t repulsionForceMovingAverageWindowSize,
                  float minimumRepulsionForceAttenuationPercentage,
                  float repulsionForceStddevPercentageThreshold,
                  const std::string& fileNameLog,
                  bool applyKokkosInitialize,
                  bool applyKokkosFinalize,
                  bool verbose );
    MedusaGeometryDeoverlaperCommand( const Dictionary& parameters );
    virtual ~MedusaGeometryDeoverlaperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MedusaGeometryDeoverlaperCommand,
                              Command,
                              int32_t, char** >;
    friend class Creator1Arg< MedusaGeometryDeoverlaperCommand,
                              Command,
                              const Dictionary& >;

    void parse();

    void execute( const std::string& fileNameGeometryInputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMedusaGeometry,
                  const std::string& fileNameGeometryOutputMesh,
                  const std::string& fileNameGeometryOutputEnergyPlot,
                  float maximumForceNorm,
                  int32_t atomSolverMaximumIterationCount,
                  int32_t repulsionForceMovingAverageWindowSize,
                  float minimumRepulsionForceAttenuationPercentage,
                  float repulsionForceStddevPercentageThreshold,
                  const std::string& fileNameLog,
                  bool applyKokkosInitialize,
                  bool applyKokkosFinalize,
                  bool verbose );

};


}


#endif
