#ifndef _gkg_processing_plugin_functors_Resampling3d_Resampling3dCommand_h_
#define _gkg_processing_plugin_functors_Resampling3d_Resampling3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class Resampling3dCommand : public Command,
                            public Creator2Arg< Resampling3dCommand,
                                         	Command,
                                         	int32_t,
                                         	char** >,
                            public Creator1Arg< Resampling3dCommand,
                                                Command,
                                                const Dictionary& >
{

  public:

    Resampling3dCommand( int32_t argc,
                  	 char* argv[], 
                  	 bool loadPlugin = false,
                  	 bool removeFirst = true );
    Resampling3dCommand( const std::string& fileNameReference,
                         const std::string& fileNameTemplate,
                         const std::vector< int32_t >& outSize,
                         const std::vector< double >& outResolution,
                         const std::vector< std::string >& fileNameTransforms,
                         const std::string& fileNameOut,
                         int32_t order,
                         double outBackground,
                         bool ascii,
                         const std::string& format,
                         bool applyInverseLinearTransformFirst,
                         bool verbose );
    Resampling3dCommand( const Dictionary& parameters );
    virtual ~Resampling3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Resampling3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Resampling3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameReference,
                  const std::string& fileNameTemplate,
                  const std::vector< int32_t >& outSize,
                  const std::vector< double >& outResolution,
                  const std::vector< std::string >& fileNameTransforms,
                  const std::string& fileNameOut,
                  int32_t order,
                  double outBackground,
                  bool ascii,
                  const std::string& format,
                  bool applyInverseLinearTransformFirst,
                  bool verbose );

};


}


#endif
