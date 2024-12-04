#ifndef _gkg_processing_plugin_functors_Dicom2GisConverter_Dicom2GisConverterCommand_h_
#define _gkg_processing_plugin_functors_Dicom2GisConverter_Dicom2GisConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Dicom2GisConverterCommand : public Command,
                                  public Creator2Arg< Dicom2GisConverterCommand,
                    			 	      Command,
                    			 	      int32_t,
                    			 	      char** >,
                                  public Creator1Arg< Dicom2GisConverterCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    Dicom2GisConverterCommand( int32_t argc,
                  	       char* argv[], 
                  	       bool loadPlugin = false,
                  	       bool removeFirst = true );
    Dicom2GisConverterCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               const std::string& outputFormat,
                               bool noFlip,
                               bool noDemosaic,
                               bool verbose );
    Dicom2GisConverterCommand( const Dictionary& parameters );
    virtual ~Dicom2GisConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Dicom2GisConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Dicom2GisConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& outputFormat,
                  bool noFlip,
                  bool noDemosaic,
                  bool verbose );

};


}


#endif
