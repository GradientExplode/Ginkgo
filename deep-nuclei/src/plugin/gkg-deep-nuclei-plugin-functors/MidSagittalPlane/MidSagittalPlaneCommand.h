#ifndef _gkg_deep_nuclei_plugin_functors_MidSagittalPlane_MidSagittalPlaneCommand_h_
#define _gkg_deep_nuclei_plugin_functors_MidSagittalPlane_MidSagittalPlaneCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MidSagittalPlaneCommand : public Command,
                                public Creator2Arg< MidSagittalPlaneCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                public Creator1Arg< MidSagittalPlaneCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    MidSagittalPlaneCommand( int32_t argc,
                             char* argv[], 
                             bool loadPlugin = false,
                             bool removeFirst = true );
    MidSagittalPlaneCommand( const std::string& fileNameIn,
                             const std::string& fileNameOut,
                             const std::string& fileNameMask,
                             const std::string& fileNameTransformation,
                             const std::string& fileNamePlane,
			     int32_t degreeOfFreedom,
			     bool output3d,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    MidSagittalPlaneCommand( const Dictionary& parameters );
    virtual ~MidSagittalPlaneCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MidSagittalPlaneCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MidSagittalPlaneCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& fileNameMask,
                  const std::string& fileNameTransformation,
                  const std::string& fileNamePlane,
		  int32_t degreeOfFreedom,
		  bool output3d,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
