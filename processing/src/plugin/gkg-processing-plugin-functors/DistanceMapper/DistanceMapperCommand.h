#ifndef _gkg_processing_plugin_functors_DistanceMapper_DistanceMapperCommand_h_
#define _gkg_processing_plugin_functors_DistanceMapper_DistanceMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class DistanceMapperCommand : public Command,
                              public Creator2Arg< DistanceMapperCommand,
                                         	  Command,
                                         	  int32_t,
                                         	  char** >,
                              public Creator1Arg< DistanceMapperCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DistanceMapperCommand( int32_t argc,
                  	   char* argv[], 
                  	   bool loadPlugin = false,
                  	   bool removeFirst = true );
    DistanceMapperCommand( const std::string& fileNameIn,
                           const std::string& fileNameOut,
                           const std::string& mode,
                           double threshold1,
                           double threshold2,
                           bool ascii,
                           const std::string& format,
                           bool verbose );
    DistanceMapperCommand( const Dictionary& parameters );
    virtual ~DistanceMapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DistanceMapperCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DistanceMapperCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameIn,
                  const std::string& fileNameOut,
                  const std::string& mode,
                  double threshold1,
                  double threshold2,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
