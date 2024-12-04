#ifndef _gkg_processing_plugin_functors_BrainVISAVectorFieldConverter_BrainVISAVectorFieldConverterCommand_h_
#define _gkg_processing_plugin_functors_BrainVISAVectorFieldConverter_BrainVISAVectorFieldConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class BrainVISAVectorFieldConverterCommand : public Command,
                       public Creator2Arg< BrainVISAVectorFieldConverterCommand,
                                           Command,
                                           int32_t,
                                           char** >,
                       public Creator1Arg< BrainVISAVectorFieldConverterCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    BrainVISAVectorFieldConverterCommand( int32_t argc,
                             char* argv[],
                             bool loadPlugin = false,
                             bool removeFirst = true );
    BrainVISAVectorFieldConverterCommand(
                             const std::string& fileNameInputDirectVectorField,
                             const std::string& fileNameInputInverseVectorField,
                             const std::string& fileNameOutputVectorField,
                             const std::string& fileNameOutputDistanceField,
                             bool ascii,
                             const std::string& format,
                             bool verbose );
    BrainVISAVectorFieldConverterCommand( const Dictionary& parameters );
    virtual ~BrainVISAVectorFieldConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< BrainVISAVectorFieldConverterCommand,
                              Command, int32_t, char** >;
    friend class Creator1Arg< BrainVISAVectorFieldConverterCommand, 
                              Command, const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameInputDirectVectorField,
                  const std::string& fileNameInputInverseVectorField,
                  const std::string& fileNameOutputVectorField,
                  const std::string& fileNameOutputDistanceField,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
