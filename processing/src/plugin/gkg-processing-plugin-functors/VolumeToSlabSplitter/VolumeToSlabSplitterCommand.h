#ifndef _gkg_processing_plugin_functors_VolumeToSlabSplitter_VolumeToSlabSplitterCommand_h_
#define _gkg_processing_plugin_functors_VolumeToSlabSplitter_VolumeToSlabSplitterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VolumeToSlabSplitterCommand : public Command,
                                    public Creator2Arg<
                                                    VolumeToSlabSplitterCommand,
                                                    Command,
                                                    int32_t,
                                                    char** >,
                                    public Creator1Arg<
                                                    VolumeToSlabSplitterCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    VolumeToSlabSplitterCommand( int32_t argc,
                  		 char* argv[], 
                  		 bool loadPlugin = false,
                  		 bool removeFirst = true );
    VolumeToSlabSplitterCommand( const std::string& fileNameIn,
                                 const std::string& fileNameOut,
                                 int32_t slabCount,
                                 bool ascii,
                                 const std::string& format,
                                 bool verbose );
    VolumeToSlabSplitterCommand( const Dictionary& parameters );
    virtual ~VolumeToSlabSplitterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VolumeToSlabSplitterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VolumeToSlabSplitterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  int32_t slabCount,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
