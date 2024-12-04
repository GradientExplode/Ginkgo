#ifndef _gkg_processing_plugin_functors_VoiStatistics_VoiStatisticsCommand_h_
#define _gkg_processing_plugin_functors_VoiStatistics_VoiStatisticsCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class VoiStatisticsCommand : public Command,
                             public Creator2Arg< VoiStatisticsCommand,
                                         	 Command,
                                         	 int32_t,
                                         	 char** >,
                             public Creator1Arg< VoiStatisticsCommand,
                                                 Command,
                                                 const Dictionary& >
{

  public:

    VoiStatisticsCommand( int32_t argc,
                  	  char* argv[], 
                  	  bool loadPlugin = false,
                  	  bool removeFirst = true );
    VoiStatisticsCommand(
    	      const std::string& fileNameMeasureVolume,
    	      const std::string& fileNameLabelVolume,
    	      const std::string& fileNameDensityMask,
              const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d,
    	      const std::string& fileNameStatistics,
    	      int32_t rank,
    	      double background,
    	      bool verbose );
    VoiStatisticsCommand( const Dictionary& parameters );
    virtual ~VoiStatisticsCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< VoiStatisticsCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< VoiStatisticsCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute(
              const std::string& fileNameMeasureVolume,
    	      const std::string& fileNameLabelVolume,
    	      const std::string& fileNameDensityMask,
              const std::string& fileNameLabelOrDensityMaskToMeasureTransform3d,
    	      const std::string& fileNameStatistics,
    	      int32_t rank,
    	      double background,
    	      bool verbose );

};


}


#endif
