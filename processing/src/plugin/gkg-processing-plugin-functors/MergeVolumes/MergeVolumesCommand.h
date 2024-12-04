#ifndef _gkg_processing_plugin_functors_MergeVolumes_MergeVolumesCommand_h_
#define _gkg_processing_plugin_functors_MergeVolumes_MergeVolumesCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class MergeVolumesCommand : public Command,
                            public Creator2Arg< MergeVolumesCommand,
                                         	Command,
                                         	int32_t,
                                         	char** >,
                            public Creator1Arg< MergeVolumesCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    MergeVolumesCommand( int32_t argc,
                  	 char* argv[], 
                  	 bool loadPlugin = false,
                  	 bool removeFirst = true );
    MergeVolumesCommand( const std::string& fileNameReference,
                         const std::string& fileNameFloating,
                         const std::string& fileNameTransformation,
                         const std::string& fileNameOut,
                         int32_t resamplingOrder,
                         const std::string& fusionMethod,
                         bool forceReferenceSize,
                         bool verbose );
    MergeVolumesCommand( const Dictionary& parameters );
    virtual ~MergeVolumesCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MergeVolumesCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MergeVolumesCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameReference,
                  const std::string& fileNameFloating,
                  const std::string& fileNameTransformation,
                  const std::string& fileNameOut,
                  int32_t resamplingOrder,
                  const std::string& fusionMethod,
                  bool forceReferenceSize,
                  bool verbose );

};


}


#endif
