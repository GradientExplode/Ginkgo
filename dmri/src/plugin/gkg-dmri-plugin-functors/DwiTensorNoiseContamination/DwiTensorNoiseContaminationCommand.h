#ifndef _gkg_dmri_plugin_functors_DwiTensorNoiseContamination_DwiTensorNoiseContaminationCommand_h_
#define _gkg_dmri_plugin_functors_DwiTensorNoiseContamination_DwiTensorNoiseContaminationCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiTensorNoiseContaminationCommand :
                         public Command,
                         public Creator2Arg< DwiTensorNoiseContaminationCommand,
                                             Command,
                                             int32_t,
                                             char** >,
                         public Creator1Arg< DwiTensorNoiseContaminationCommand,
                                             Command,
                                             const Dictionary& >
{

  public:

    DwiTensorNoiseContaminationCommand( int32_t argc,
                                        char* argv[],
                                        bool loadPlugin = false,
                                        bool removeFirst = true );
    DwiTensorNoiseContaminationCommand( const std::string& fileNameIn,
                                        double bValue,
                                        const std::string& fileNameTensor,
                                        double minimumSNR,
                                        double maximumSNR,
                                        int32_t stepCount,
                                        int32_t orientationCount,
                                        bool verbose );
    DwiTensorNoiseContaminationCommand( const Dictionary& parameters );
    virtual ~DwiTensorNoiseContaminationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiTensorNoiseContaminationCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiTensorNoiseContaminationCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  double bValue,
                  const std::string& fileNameTensor,
                  double minimumSNR,
                  double maximumSNR,
                  int32_t stepCount,
                  int32_t orientationCount,
                  bool verbose );

};


}


#endif
