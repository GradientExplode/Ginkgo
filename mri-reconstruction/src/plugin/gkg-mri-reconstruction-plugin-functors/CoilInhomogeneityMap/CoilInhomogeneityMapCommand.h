#ifndef _gkg_mri_reconstruction_plugin_functors_CoilInhomogeneityMap_CoilInhomogeneityMapCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_CoilInhomogeneityMap_CoilInhomogeneityMapCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class CoilInhomogeneityMapCommand :
                                public Command,
                                public Creator2Arg< CoilInhomogeneityMapCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< CoilInhomogeneityMapCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    CoilInhomogeneityMapCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    CoilInhomogeneityMapCommand( const std::string& fileNameMultiCoilInput,
                                 const std::string& fileNameBodyCoilInput,
                                 const std::string& fileNameInhomogeneityOutput,
                                 float closingRadiusFactor,
                                 double noisePosition,
                                 double noiseRatio,
                                 int32_t thrown,
                                 int32_t kept,
                                 bool extra,
                                 const std::string& filter,
                                 int32_t filterIterationCount,
                                 bool verbose );
    CoilInhomogeneityMapCommand( const Dictionary& parameters );
    virtual ~CoilInhomogeneityMapCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CoilInhomogeneityMapCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< CoilInhomogeneityMapCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameMultiCoilInput,
                  const std::string& fileNameBodyCoilInput,
                  const std::string& fileNameInhomogeneityOutput,
                  float closingRadiusFactor,
                  double noisePosition,
                  double noiseRatio,
                  int32_t thrown,
                  int32_t kept,
                  bool extra,
                  const std::string& filter,
                  int32_t filterIterationCount,
                  bool verbose );

};


}


#endif
