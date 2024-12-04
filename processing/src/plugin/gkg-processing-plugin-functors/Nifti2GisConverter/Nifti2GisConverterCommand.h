#ifndef _gkg_processing_plugin_functors_Nifti2GisConverter_Nifti2GisConverterCommand_h_
#define _gkg_processing_plugin_functors_Nifti2GisConverter_Nifti2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Nifti2GisConverterCommand : public Command,
                                  public Creator2Arg< Nifti2GisConverterCommand,
                                         	      Command,
                                         	      int32_t,
                                         	      char** >,
                                  public Creator1Arg< Nifti2GisConverterCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    Nifti2GisConverterCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    Nifti2GisConverterCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               const std::string& outputFormat,
                               bool ascii,
                               bool verbose );
    Nifti2GisConverterCommand( const Dictionary& parameters );
    virtual ~Nifti2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Nifti2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Nifti2GisConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool ascii,
                  bool verbose );

};


}


#endif
