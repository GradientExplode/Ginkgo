#ifndef _gkg_dmri_plugin_functors_DwiMakeWholeOrientationSet_DwiMakeWholeOrientationSetCommand_h_
#define _gkg_dmri_plugin_functors_DwiMakeWholeOrientationSet_DwiMakeWholeOrientationSetCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DwiMakeWholeOrientationSetCommand :
                          public Command,
                          public Creator2Arg< DwiMakeWholeOrientationSetCommand,
                                              Command,
                                              int32_t,
                                              char** >,
                          public Creator1Arg< DwiMakeWholeOrientationSetCommand,
                                              Command,
                                              const Dictionary& >
{

  public:

    DwiMakeWholeOrientationSetCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin = false,
                                       bool removeFirst = true );
    DwiMakeWholeOrientationSetCommand( const std::string& fileNameIn,
                                       const std::string& fileNameOut,
                                       int32_t furtherOrientationCount,
                                       int32_t stepCount,
                                       float step,
                                       float minimumStep,
                                       bool verbose );
    DwiMakeWholeOrientationSetCommand( const Dictionary& parameters );
    virtual ~DwiMakeWholeOrientationSetCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DwiMakeWholeOrientationSetCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DwiMakeWholeOrientationSetCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t furtherOrientationCount,
                  int32_t stepCount,
                  float step,
                  float minimumStep,
                  bool verbose );

};


}


#endif
