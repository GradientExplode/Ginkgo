#ifndef _gkg_processing_plugin_functors_RescalerWithSlopeAndIntercept_RescalerWithSlopeAndInterceptCommand_h_
#define _gkg_processing_plugin_functors_RescalerWithSlopeAndIntercept_RescalerWithSlopeAndInterceptCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class RescalerWithSlopeAndInterceptCommand :
                       public Command,
                       public Creator2Arg< RescalerWithSlopeAndInterceptCommand,
                     	   		   Command,
                     	   		   int32_t,
                     	   		   char** >,
                       public Creator1Arg< RescalerWithSlopeAndInterceptCommand,
                                           Command,
                                           const Dictionary& >
{

  public:

    RescalerWithSlopeAndInterceptCommand( int32_t argc,
                                          char* argv[],
                                          bool loadPlugin = false,
                                          bool removeFirst = true );
    RescalerWithSlopeAndInterceptCommand( const std::string& fileNameIn,
                                          const std::string& fileNameOut,
                                          bool ascii,
                                          const std::string& format,
                                          bool verbose );
    RescalerWithSlopeAndInterceptCommand( const Dictionary& parameters );
    virtual ~RescalerWithSlopeAndInterceptCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RescalerWithSlopeAndInterceptCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RescalerWithSlopeAndInterceptCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
