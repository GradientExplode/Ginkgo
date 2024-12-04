#ifndef _gkg_mri_reconstruction_plugin_functors_CoilCovarianceMatrix_CoilCovarianceMatrixCommand_h_
#define _gkg_mri_reconstruction_plugin_functors_CoilCovarianceMatrix_CoilCovarianceMatrixCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class CoilCovarianceMatrixCommand :
                                public Command,
                                public Creator2Arg< CoilCovarianceMatrixCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< CoilCovarianceMatrixCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    CoilCovarianceMatrixCommand( int32_t argc,
                                 char* argv[],
                                 bool loadPlugin = false,
                                 bool removeFirst = true );
    CoilCovarianceMatrixCommand( const std::string& fileNameCoilInput,
                                 const std::string& fileNameCovarianceOutput,
                                 float dilationRadiusFactor,
                                 float closingRadiusFactor,
                                 double noisePosition,
                                 double noiseRatio,
                                 int32_t thrown,
                                 int32_t kept,
                                 bool extra,
                                 bool verbose );
    CoilCovarianceMatrixCommand( const Dictionary& parameters );
    virtual ~CoilCovarianceMatrixCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< CoilCovarianceMatrixCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< CoilCovarianceMatrixCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameCoilInput,
                  const std::string& fileNameCovarianceOutput,
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
