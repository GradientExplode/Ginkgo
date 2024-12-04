#ifndef _gkg_processing_plugin_functors_DataBaseClassifier_DataBaseClassifierCommand_h_
#define _gkg_processing_plugin_functors_DataBaseClassifier_DataBaseClassifierCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DataBaseClassifierCommand : public Command,
                                  public Creator2Arg< DataBaseClassifierCommand,
                    			              Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< DataBaseClassifierCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    DataBaseClassifierCommand( int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    DataBaseClassifierCommand( const std::string& folderNameIn,
                               const std::string& folderNameOut,
                               bool apply,
                               bool verbose );
    DataBaseClassifierCommand( const Dictionary& parameters );
    virtual ~DataBaseClassifierCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DataBaseClassifierCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DataBaseClassifierCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& folderNameIn,
                  const std::string& folderNameOut,
                  bool apply,
                  bool verbose );

};


}


#endif
