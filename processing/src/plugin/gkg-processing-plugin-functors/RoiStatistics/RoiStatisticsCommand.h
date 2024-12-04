#ifndef _gkg_processing_plugin_functors_RoiStatistics_RoiStatisticsCommand_h_
#define _gkg_processing_plugin_functors_RoiStatistics_RoiStatisticsCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class RoiStatisticsCommand : public Command,
                             public Creator2Arg< RoiStatisticsCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< RoiStatisticsCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    RoiStatisticsCommand( int32_t argc,
                  	  char* argv[], 
                  	  bool loadPlugin = false,
                  	  bool removeFirst = true );
    RoiStatisticsCommand( const std::vector< std::string >& contrastFileNameIns,
                          const std::vector< std::string >& labelFileNameIns,
                          const std::string& roiDictionaryFileName,
                          const std::string& atlasFileName,
                          const std::string& contrastName,
                          const std::string& fileNameOut,
                          bool ascii,
                          const std::string& format,
                          bool verbose );
    RoiStatisticsCommand( const Dictionary& parameters );
    virtual ~RoiStatisticsCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< RoiStatisticsCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< RoiStatisticsCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& contrastFileNameIns,
                  const std::vector< std::string >& labelFileNameIns,
                  const std::string& roiDictionaryFileName,
                  const std::string& atlasFileName,
                  const std::string& contrastName,
                  const std::string& fileNameOut,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

};


}


#endif
