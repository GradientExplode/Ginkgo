#ifndef _gkg_processing_plugin_functors_Gis2NiftiConverter_Gis2NiftiConverterCommand_h_
#define _gkg_processing_plugin_functors_Gis2NiftiConverter_Gis2NiftiConverterCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class Gis2NiftiConverterCommand : public Command,
                                  public Creator2Arg< Gis2NiftiConverterCommand,
                                                      Command,
                                                      int32_t,
                                                      char** >,
                                  public Creator1Arg< Gis2NiftiConverterCommand,
                                                      Command,
                                                      const Dictionary& >
{

  public:

    Gis2NiftiConverterCommand( int32_t argc,
                               char* argv[], 
                               bool loadPlugin = false,
                               bool removeFirst = true );
    Gis2NiftiConverterCommand( const std::string& fileNameIn,
                               const std::string& fileNameOut,
                               bool verbose );
    Gis2NiftiConverterCommand( const Dictionary& parameters );
    virtual ~Gis2NiftiConverterCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Gis2NiftiConverterCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Gis2NiftiConverterCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool verbose );

};


}


#endif
