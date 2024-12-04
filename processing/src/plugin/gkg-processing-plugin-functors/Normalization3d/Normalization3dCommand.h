#ifndef _gkg_processing_plugin_functors_Normalization3d_Normalization3dCommand_h_
#define _gkg_processing_plugin_functors_Normalization3d_Normalization3dCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>
#include <list>


namespace gkg
{


class Normalization3dCommand : public Command,
                               public Creator2Arg< Normalization3dCommand,
                                                   Command,
                                                   int32_t,
                                                   char** >,
                               public Creator1Arg< Normalization3dCommand,
                                                   Command,
                                                   const Dictionary& >
{

  public:

    Normalization3dCommand( int32_t argc,
                            char* argv[], 
                            bool loadPlugin = false,
                            bool removeFirst = true );
    Normalization3dCommand(
                  const std::string& specy,
                  const std::string& templateName,
                  const std::string& floatingName,
                  const std::vector< std::string >& flippings,
                  const std::string& fileNameTransform3dFloatingToTemplate,
                  const std::string& fileNameTransform3dFloatingToTalairachMNI,
                  const std::string& fileNameTransform3dFloatingToTalairachACPC,
                  const std::string& fileNameTransform3dTemplateToFloating,
                  const std::string& fileNameTransform3dTalairachMNIToFloating,
                  const std::string& fileNameTransform3dTalairachACPCToFloating,
                  const std::string& fileNameACPCCoordinates,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double templateLowerThreshold,
                  double floatingLowerThreshold,
                  int32_t resamplingOrder,
                  const std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< float >& initialParameters,
                  const std::string& initialTrmName,
                  bool verbose );
    Normalization3dCommand( const Dictionary& parameters );
    virtual ~Normalization3dCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< Normalization3dCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< Normalization3dCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::string& specy,
                  const std::string& templateName,
                  const std::string& floatingName,
                  const std::vector< std::string >& flippings,
                  const std::string& fileNameTransform3dFloatingToTemplate,
                  const std::string& fileNameTransform3dFloatingToTalairachMNI,
                  const std::string& fileNameTransform3dFloatingToTalairachACPC,
                  const std::string& fileNameTransform3dTemplateToFloating,
                  const std::string& fileNameTransform3dTalairachMNIToFloating,
                  const std::string& fileNameTransform3dTalairachACPCToFloating,
                  const std::string& fileNameACPCCoordinates,
                  const std::string& similarityMeasureName,
                  const std::string& optimizerName,
                  double templateLowerThreshold ,
                  double floatingLowerThreshold,
                  int32_t resamplingOrder,
                  const std::list< int32_t >& subSamplingMaximumSizes,
                  const std::vector< double >& similarityMeasureParameters,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< float >& initialParameters,
                  const std::string& initialTrmName,
                  bool verbose );

};


}


#endif
