#ifndef _gkg_processing_plugin_functors_MeshSelectorUsingFileSize_MeshSelectorUsingFileSizeCommand_h_
#define _gkg_processing_plugin_functors_MeshSelectorUsingFileSize_MeshSelectorUsingFileSizeCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-colorimetry/RGBComponent.h>

#include <string>
#include <list>
#include <map>


namespace gkg
{


class MeshSelectorUsingFileSizeCommand :
                           public Command,
                           public Creator2Arg< MeshSelectorUsingFileSizeCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< MeshSelectorUsingFileSizeCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    MeshSelectorUsingFileSizeCommand( int32_t argc,
                  		      char* argv[], 
                  		      bool loadPlugin = false,
                  		      bool removeFirst = true );
    MeshSelectorUsingFileSizeCommand(
                              const std::string& inputDirectoryName,
                              const std::string& inputItkSnapLutName,
                              const std::string& outputDirectoryName,
                              const std::list< std::string >& excludedLabelList,
                              float transparency,
                              bool verbose );
    MeshSelectorUsingFileSizeCommand( const Dictionary& parameters );
    virtual ~MeshSelectorUsingFileSizeCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MeshSelectorUsingFileSizeCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< MeshSelectorUsingFileSizeCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& inputDirectoryName,
                  const std::string& inputItkSnapLutName,
                  const std::string& outputDirectoryName,
                  const std::list< std::string >& excludedLabelList,
                  float transparency,
                  bool verbose );

    void analyzeITKSnapFile( const std::string& itkSnapFileName,
                             std::map< std::string, RGBComponent >& 
                               roiToColorMap ) const;


};


}


#endif
