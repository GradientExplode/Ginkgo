#ifndef _gkg_deep_nuclei_plugin_functors_DeepNucleiSegmentation_DeepNucleiSegmentationCommand_h_
#define _gkg_deep_nuclei_plugin_functors_DeepNucleiSegmentation_DeepNucleiSegmentationCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>

#include <string>
#include <vector>


namespace gkg
{


class DeepNucleiSegmentationCommand : public Command,
                                      public Creator2Arg<
                                                  DeepNucleiSegmentationCommand,
                                                  Command,
                                                  int32_t,
                                                  char** >,
                                      public Creator1Arg<
                                                  DeepNucleiSegmentationCommand,
                                                  Command,
                                                  const Dictionary& >
{

  public:

    DeepNucleiSegmentationCommand( int32_t argc,
                                   char* argv[], 
                                   bool loadPlugin = false,
                                   bool removeFirst = true );
    DeepNucleiSegmentationCommand(
                     const std::vector< std::string >& contrastNames,
                     const std::vector< std::string >& atlasFileNames, 
                     const std::vector< std::string >& contrastFileNames,
                     std::vector< double >& allWeights,
                     std::vector< int32_t >& strides,
                     const std::vector< int32_t >& schemeIterationCounts,
                     const std::vector< int32_t >& regionGrowingIterationCounts,
                     const std::vector< int32_t >& transform3dIterationCounts,
                     bool sameTransformAcrossLabels,
                     const std::string& fileNameOut,
                     double maximumTestSize,
                     const std::vector< float >& deltaParameters,
                     int32_t storagePeriod,
                     int32_t displayPeriod,
                     bool verbose );
    DeepNucleiSegmentationCommand( const Dictionary& parameters );
    virtual ~DeepNucleiSegmentationCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< DeepNucleiSegmentationCommand, Command, 
                              int32_t, char** >;
    friend class Creator1Arg< DeepNucleiSegmentationCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& contrastNames,
                  const std::vector< std::string >& atlasFileNames, 
                  const std::vector< std::string >& contrastFileNames,
                  std::vector< double >& allWeights,
                  std::vector< int32_t >& strides,
                  const std::vector< int32_t >& schemeIterationCounts,
                  const std::vector< int32_t >& regionGrowingIterationCounts,
                  const std::vector< int32_t >& transform3dIterationCounts,
                  bool sameTransformAcrossLabels,
                  const std::string& fileNameOut,
                  double maximumTestSize,
                  const std::vector< float >& deltaParameters,
                  int32_t storagePeriod,
                  int32_t displayPeriod,
                  bool verbose );
};


}


#endif
