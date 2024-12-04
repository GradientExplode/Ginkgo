#ifndef _gkg_processing_plugin_functors_Dicom2NiftiConverter_Dicom2NiftiConverterCommand_h_
#define _gkg_processing_plugin_functors_Dicom2NiftiConverter_Dicom2NiftiConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Dicom2NiftiConverterCommand :
                                public Command,
                                public Creator2Arg< Dicom2NiftiConverterCommand,
                    			            Command,
                    			            int32_t,
                    			            char** >,
                                public Creator1Arg< Dicom2NiftiConverterCommand,
                                                    Command,
                                                    const Dictionary& >
{

  public:

    Dicom2NiftiConverterCommand( int32_t argc,
                 		 char* argv[], 
                 		 bool loadPlugin = false,
                 		 bool removeFirst = true );
    Dicom2NiftiConverterCommand( const std::string& fileNameIn,
                                 const std::string& fileNameOut,
                                 bool verbose );
    Dicom2NiftiConverterCommand( const Dictionary& parameters );
    virtual ~Dicom2NiftiConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Dicom2NiftiConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Dicom2NiftiConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
