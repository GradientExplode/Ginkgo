#ifndef _gkg_processing_plugin_functors_TwoTexturesCorrelationCoefficient_TwoTexturesCorrelationCoefficientCommand_h_
#define _gkg_processing_plugin_functors_TwoTexturesCorrelationCoefficient_TwoTexturesCorrelationCoefficientCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>


namespace gkg
{


class TwoTexturesCorrelationCoefficientCommand :
                   public Command,
                   public Creator2Arg< TwoTexturesCorrelationCoefficientCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< TwoTexturesCorrelationCoefficientCommand,
                                       Command,
                                       const Dictionary& >
{

  public:

    TwoTexturesCorrelationCoefficientCommand( int32_t argc,
                  			      char* argv[], 
                  			      bool loadPlugin = false,
                  			      bool removeFirst = true );
    TwoTexturesCorrelationCoefficientCommand(
    				    const std::string& fileNameTextureMap1,
    				    const std::string& fileNameTextureMap2,
    				    const std::string& fileNameTextureMapRois,
    				    const std::string& fileNameMeshMap,
    				    const std::string& fileNameTextureMapCCOut,
    				    const std::string& fileNameTextureMapRoiOut,
    				    const std::string& fileNameStatisticsOut,
    				    int32_t rank1,
    				    int32_t rank2,
    				    int32_t rankRois,
    				    float distance,
    				    const std::string& format,
    				    bool ascii,
    				    bool verbose );
    TwoTexturesCorrelationCoefficientCommand( const Dictionary& parameters );
    virtual ~TwoTexturesCorrelationCoefficientCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< TwoTexturesCorrelationCoefficientCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< TwoTexturesCorrelationCoefficientCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& fileNameTextureMap1,
    		  const std::string& fileNameTextureMap2,
    		  const std::string& fileNameTextureMapRois,
    		  const std::string& fileNameMeshMap,
    		  const std::string& fileNameTextureMapCCOut,
    		  const std::string& fileNameTextureMapRoiOut,
    		  const std::string& fileNameStatisticsOut,
    		  int32_t rank1,
    		  int32_t rank2,
    		  int32_t rankRois,
    		  float distance,
    		  const std::string& format,
    		  bool ascii,
    		  bool verbose );

};


}


#endif
