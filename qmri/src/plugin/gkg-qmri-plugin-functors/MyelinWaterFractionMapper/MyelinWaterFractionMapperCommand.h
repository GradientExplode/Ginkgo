#ifndef _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperCommand_h_
#define _gkg_qmri_plugin_functors_MyelinWaterFractionMapper_MyelinWaterFractionMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>

#include <string>
#include <vector>


namespace gkg
{


class MyelinWaterFractionMapperCommand : public Command,
                           public Creator2Arg< MyelinWaterFractionMapperCommand,
                                               Command,
                                               int32_t,
                                               char** >,
                           public Creator1Arg< MyelinWaterFractionMapperCommand,
                                               Command,
                                               const Dictionary& >
{

  public:

    MyelinWaterFractionMapperCommand( int32_t argc,
                                      char* argv[], 
                                      bool loadPlugin = false,
                                      bool removeFirst = true );
    MyelinWaterFractionMapperCommand(
                    const std::vector< std::string >& fileNameT2WeightedVolumes,
                    const std::string& fileNameT1WeightedVolume,
                    const std::string& fileNameB1Mapping, 
                    const std::vector< std::string >& fileNameEchoTimes, 
                    const std::string& fileNameInversionTimes, 
                    const std::string& fileNameSPGRFlipAngles,
                    const std::string& fileNameRepetitionTimes, 
                    const std::string& fileNameMask,
                    const std::string& fileNameMyelinWaterFraction,
                    const std::string& fileNameMyelinWaterT1,
                    const std::string& fileNameMyelinWaterT2,
                    const std::string& fileNameGrayWhiteFraction,
                    const std::string& fileNameGrayWhiteT1,
                    const std::string& fileNameGrayWhiteT2,
                    const std::string& fileNameCSFFraction,
                    const std::string& fileNameCSFT1,
                    const std::string& fileNameCSFT2,
                    const std::string& fileNameProtonDensity,
                    const std::string& fileNameProtonDensityScaling,
                    const std::string& fileNameT2WeightedScaling,
                    const std::string& fileNameFittedData,
                    const std::vector< double >& optimizerParameters,
                    const std::vector< double >& initialParameters,
                    const std::vector< double >& lowerParameterBoundaries,
                    const std::vector< double >& upperParameterBoundaries,
                    const std::vector< int32_t >& isFixedParameters,
                    const std::vector< double >& deltaParameters,
                    const std::vector< double >& t2NoiseStandardDeviations,
                    double t1NoiseStandardDeviation,
                    bool ascii,
                    const std::string& format,
                    bool verbose );
    MyelinWaterFractionMapperCommand( const Dictionary& parameters );
    virtual ~MyelinWaterFractionMapperCommand();

    static std::string getStaticName();

  protected:

    friend class Creator2Arg< MyelinWaterFractionMapperCommand, Command,
                              int32_t, char** >;
    friend class Creator1Arg< MyelinWaterFractionMapperCommand, Command,
                              const Dictionary& >;

    void parse();
    void execute( const std::vector< std::string >& fileNameT2WeightedVolumes,
                  const std::string& fileNameT1WeightedVolumes,
                  const std::string& fileNameB1Mapping,
                  const std::vector< std::string >& fileNameEchoTimes,
                  const std::string& fileNameInversionTimes,
                  const std::string& fileNameSPGRFlipAngles,
                  const std::string& fileNameRepetitionTimes,
                  const std::string& fileNameMask,
                  const std::string& fileNameMyelinWaterFraction,
                  const std::string& fileNameMyelinWaterT1,
                  const std::string& fileNameMyelinWaterT2,
                  const std::string& fileNameGrayWhiteFraction,
                  const std::string& fileNameGrayWhiteT1,
                  const std::string& fileNameGrayWhiteT2,
                  const std::string& fileNameCSFFraction,
                  const std::string& fileNameCSFT1,
                  const std::string& fileNameCSFT2,
                  const std::string& fileNameProtonDensity,
                  const std::string& fileNameProtonDensityScaling,
                  const std::string& fileNameT2WeightedScaling,
                  const std::string& fileNameFittedData,
                  const std::vector< double >& optimizerParameters,
                  const std::vector< double >& initialParameters,
                  const std::vector< double >& lowerParameterBoundaries,
                  const std::vector< double >& upperParameterBoundaries,
                  const std::vector< int32_t >& isFixedParameters,
                  const std::vector< double >& deltaParameters,
                  const std::vector< double >& t2NoiseStandardDeviations,
                  double t1NoiseStandardDeviation,
                  bool ascii,
                  const std::string& format,
                  bool verbose );

    void readTextFile( const std::string& fileName,
                       int32_t count,
                       Vector& values ) const;

};


}


#endif
