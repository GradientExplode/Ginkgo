#ifndef _gkg_processing_plugin_functors_DicomAnalyzer_DicomAnalyzerCommand_h_
#define _gkg_processing_plugin_functors_DicomAnalyzer_DicomAnalyzerCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DicomAnalyzerCommand : public Command,
                             public Creator2Arg< DicomAnalyzerCommand,
                    			         Command,
                      	   	  	         int32_t,
                    			         char** >,
                             public Creator1Arg< DicomAnalyzerCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    DicomAnalyzerCommand( int32_t argc,
                  	  char* argv[], 
                  	  bool loadPlugin = false,
                  	  bool removeFirst = true );
    DicomAnalyzerCommand( const std::string& inputDirectory,
                          const std::string& outputDirectory,
                          const std::string& defaultInstitutionName,
                          bool verbose );
    DicomAnalyzerCommand( const Dictionary& parameters );
    virtual ~DicomAnalyzerCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DicomAnalyzerCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DicomAnalyzerCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& inputDirectory,
                  const std::string& outputDirectory,
                  const std::string& defaultInstitutionName,
                  bool verbose );

};


}


#endif
