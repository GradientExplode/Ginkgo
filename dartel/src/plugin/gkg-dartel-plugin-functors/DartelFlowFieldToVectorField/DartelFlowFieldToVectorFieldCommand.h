#ifndef _gkg_dartel_plugin_functors_DartelFlowFieldToVectorField_DartelFlowFieldToVectorFieldCommand_h_
#define _gkg_dartel_plugin_functors_DartelFlowFieldToVectorField_DartelFlowFieldToVectorFieldCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DartelFlowFieldToVectorFieldCommand : public Command,
                                            public Creator2Arg<
                                            DartelFlowFieldToVectorFieldCommand,
                                            Command,
                                            int32_t,
                                            char** >,
                                            public Creator1Arg<
                                            DartelFlowFieldToVectorFieldCommand,
                                            Command,
                                            const Dictionary& >
{

  public:

    DartelFlowFieldToVectorFieldCommand( int32_t argc,
                                         char* argv[], 
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    DartelFlowFieldToVectorFieldCommand( const std::string& fileNameFlowField,
                                         const std::string& fileNameOut,
                                         int32_t k,
                                         bool verbose );
    DartelFlowFieldToVectorFieldCommand( const Dictionary& parameters );
    virtual ~DartelFlowFieldToVectorFieldCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DartelFlowFieldToVectorFieldCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DartelFlowFieldToVectorFieldCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameFlowField,
                  const std::string& fileNameOut,
                  int32_t k,
    		  bool verbose );

};


}


#endif
