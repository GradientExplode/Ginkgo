#ifndef _gkg_qmri_plugin_functors_RelaxometryKalmanFilter_RelaxometryKalmanFilterCommand_h_
#define _gkg_qmri_plugin_functors_RelaxometryKalmanFilter_RelaxometryKalmanFilterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class RelaxometryKalmanFilterCommand :
                             public Command,
                             public Creator2Arg< RelaxometryKalmanFilterCommand,
                                                 Command,
                                                 int32_t,
                                                 char** >,
                             public Creator1Arg< RelaxometryKalmanFilterCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    RelaxometryKalmanFilterCommand( int32_t argc,
                                    char* argv[], 
                                    bool loadPlugin = false,
                                    bool removeFirst = true );
    RelaxometryKalmanFilterCommand(
                                  const std::string& fileNameIn, 
                                  const std::string& fileNameMask,
                                  const std::string& fileNameOut,
                                  const std::string& type,
                                  const std::vector< double >& scalarParameters,
                                  bool verbose );
    RelaxometryKalmanFilterCommand( const Dictionary& parameters );
    virtual ~RelaxometryKalmanFilterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RelaxometryKalmanFilterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RelaxometryKalmanFilterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn, 
                  const std::string& fileNameMask,
                  const std::string& fileNameOut,
                  const std::string& type,
                  const std::vector< double >& scalarParameters,
                  bool verbose );

};


}


#endif
