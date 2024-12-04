#ifndef _gkg_mri_reconstruction_plugin_functors_CoilSensitivityMap_CoilSensitivityMapCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_CoilSensitivityMap_CoilSensitivityMapCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class CoilSensitivityMapCommand : public Command,
                                  public Creator2Arg< CoilSensitivityMapCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< CoilSensitivityMapCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    CoilSensitivityMapCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin = false,
                               bool removeFirst = true );
    CoilSensitivityMapCommand( const std::string& fileNameCoilInput,
                               const std::string& fileNameSensitivityOutput,
                               float dilationRadiusFactor,
                               float closingRadiusFactor,
                               double noisePosition,
                               double noiseRatio,
                               int32_t thrown,
                               int32_t kept,
                               bool extra,
                               bool verbose );
    CoilSensitivityMapCommand( const Dictionary& parameters );
    virtual ~CoilSensitivityMapCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CoilSensitivityMapCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< CoilSensitivityMapCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameCoilInput,
                  const std::string& fileNameSensitivityOutput,
                  float dilationRadiusFactor,
                  float closingRadiusFactor,
                  double noisePosition,
                  double noiseRatio,
                  int32_t thrown,
                  int32_t kept,
                  bool extra,
                  bool verbose );

};


}


#endif
