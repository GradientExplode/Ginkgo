#ifndef _gkg_processing_plugin_functors_MergeZoomedDwData_MergeZoomedDwDataCommand_h_
#define _gkg_processing_plugin_functors_MergeZoomedDwData_MergeZoomedDwDataCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class MergeZoomedDwDataCommand : public Command,
                                 public Creator2Arg< MergeZoomedDwDataCommand,
                                                     Command,
                                         	     int32_t,
                                         	     char** >,
                                 public Creator1Arg< MergeZoomedDwDataCommand,
                                                     Command,
                                                     const Dictionary& >
{

  public:

    MergeZoomedDwDataCommand( int32_t argc,
                  	      char* argv[], 
                  	      bool loadPlugin = false,
                  	      bool removeFirst = true );
    MergeZoomedDwDataCommand( const std::vector< std::string >& fileNameIn,
                              const std::string& fileNameOut,
                              bool ascii,
                              const std::string& outputFormat,
                              bool verbose );
    MergeZoomedDwDataCommand( const Dictionary& parameters );
    virtual ~MergeZoomedDwDataCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MergeZoomedDwDataCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MergeZoomedDwDataCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameIn,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& outputFormat,
                  bool verbose );

};


}


#endif
