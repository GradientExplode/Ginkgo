#ifndef _gkg_processing_plugin_functors_PostMortemBlockToWholeImageComposer_PostMortemBlockToWholeImageComposerCommand_h_
#define _gkg_processing_plugin_functors_PostMortemBlockToWholeImageComposer_PostMortemBlockToWholeImageComposerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-coordinates/Vector3d.h>

#include <string>
#include <vector>


namespace gkg
{


class PostMortemBlockToWholeImageComposerCommand :
                 public Command,
                 public Creator2Arg< PostMortemBlockToWholeImageComposerCommand,
                                     Command,
                                     int32_t,
                                     char** >,
                 public Creator1Arg< PostMortemBlockToWholeImageComposerCommand,
                                     Command,
                                     const Dictionary& >
{

  public:

    PostMortemBlockToWholeImageComposerCommand( int32_t argc,
                                                char* argv[],
                                                bool loadPlugin = false,
                                                bool removeFirst = true );
    PostMortemBlockToWholeImageComposerCommand(
                      const std::vector< std::string >& fileNameFovs,
                      const std::vector< std::string >&
                                             fileNameFovToReferenceTransform3ds,
                      const std::vector< std::string >&
                                                 fovToReferenceTransform3dTypes,
                      const std::string& fileNameOutput,
                      const std::vector< double >& fovScalings,
                      const Vector3d< int32_t >& referenceSizes,
                      const Vector3d< double >& referenceResolutions,
                      int32_t resamplingOrder,
                      double background,
                      bool ascii,
                      const std::string& format,
                      bool verbose );
    PostMortemBlockToWholeImageComposerCommand( const Dictionary& parameters );
    virtual ~PostMortemBlockToWholeImageComposerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< PostMortemBlockToWholeImageComposerCommand,
                              Command, 
                              int32_t, char** >;
    friend class Creator1Arg< PostMortemBlockToWholeImageComposerCommand,
                              Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameFovs,
                  const std::vector< std::string >&
                                             fileNameFovToReferenceTransform3ds,
                  const std::vector< std::string >&
                                                 fovToReferenceTransform3dTypes,
                  const std::string& fileNameOutput,
                  const std::vector< double >& fovScalings,
                  const Vector3d< int32_t >& referenceSizes,
                  const Vector3d< double >& referenceResolutions,
                  int32_t resamplingOrder,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
