#ifndef _gkg_processing_plugin_functors_Thresholder_ThresholderCommand_h_
#define _gkg_processing_plugin_functors_Thresholder_ThresholderCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class ThresholderCommand : public Command,
                     	   public Creator2Arg< ThresholderCommand,
                     	   		       Command,
                     	   		       int32_t,
                     	   		       char** >,
                           public Creator1Arg< ThresholderCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    ThresholderCommand( int32_t argc,
                  	char* argv[], 
                  	bool loadPlugin = false,
                  	bool removeFirst = true );
    ThresholderCommand( const std::string& fileNameIn,
                        const std::string& fileNameOut,
                        const std::string& mode,
                        double threshold1,
                        double threshold2,
                        double background,
                        bool ascii,
                        const std::string& format,
                        bool verbose );
    ThresholderCommand( const Dictionary& parameters );
    virtual ~ThresholderCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< ThresholderCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< ThresholderCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  double background,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
