#ifndef _gkg_qmri_plugin_functors_MultiCompartmentrelaxometriesMapper_MultiCompartmentRelaxometryMapperCommand_h_
#define _gkg_qmri_plugin_functors_MultiCompartmentrelaxometriesMapper_MultiCompartmentRelaxometryMapperCommand_h_


#include <gkg-communication-command/Command.h>
#include <gkg-core-pattern/Creator.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-container/Volume.h>
#include <gkg-core-pattern/RCPointer.h>

#include <string>
#include <vector>


namespace gkg
{


class MultiCompartmentRelaxometryMapperCommand : public Command, 
                   public Creator2Arg< MultiCompartmentRelaxometryMapperCommand,
                                       Command,
                                       int32_t,
                                       char** >,
                   public Creator1Arg< MultiCompartmentRelaxometryMapperCommand,
                                            Command,
                                            const Dictionary& >
{

  public: 
  
    MultiCompartmentRelaxometryMapperCommand( int32_t argc, 
                                         char* argv[],
                                         bool loadPlugin = false,
                                         bool removeFirst = true );
    MultiCompartmentRelaxometryMapperCommand( 
                    const std::vector< std::string >& fileNameT2WeightedVolumes,
                    const std::vector< std::string >& fileNameT1WeightedVolumes,
                    const std::string& fileNameB1Mapping,
                    const std::string& fileNameHCMapping,
                    const std::string& fileNameBCMapping,
                    const std::vector< std::string >& fileNameEchoTimes,
                    const std::string& fileNameSPGRFlipAngles,
                    const std::vector< std::string >& fileNameRepetitionTimes,
                    const std::string& fileNameMask,
                    const bool& hasCSF,
                    const int32_t& arteryCount,
                    const std::string& fileNameParenchymaFraction, 
                    const std::string& fileNameParenchymaProtonDensity,
                    const std::string& fileNameParenchymaT1,
                    const std::string& fileNameParenchymaT2,
                    const std::vector< std::string>& fileNameArteriesFractions,
                    const std::string& fileNameArteriesProntonDensity,
                    const std::vector< std::string>& fileNameArteriesT1s,
                    const std::vector< std::string>& fileNameArteriesT2s, 
                    const std::string& fileNameCSFFraction, 
                    const std::string& fileNameCSFProtonDensity,
                    const std::string& fileNameCSFT1, 
                    const std::string& fileNameCSFT2, 
                    const std::string& fileNameT1WeightedScaling,
                    const std::string& fileNameT2WeightedScaling,
                    const std::string& fileNameFittedData, 
                    std::vector< double >& optimizerParameters, 
                    std::vector< double >& initialParameters, 
                    std::vector< double >& lowerParameterBoundaries, 
                    std::vector< double >& upperParameterBoundaries, 
                    std::vector< int32_t >& isFixedParameters, 
                    std::vector< double >& deltaParameters,
                    const std::vector< double >& t1NoiseStandardDeviations,
                    const std::vector< double >& t2NoiseStandardDeviations,
                    const bool competitiveT2, 
                    const bool competitiveT1, 
                    bool ascii, 
                    const std::string& format, 
                    bool verbose ); 
    MultiCompartmentRelaxometryMapperCommand( const Dictionary& parameters );
    virtual ~MultiCompartmentRelaxometryMapperCommand();
    
    static std::string getStaticName();
    
  protected:
    
    friend class Creator2Arg< MultiCompartmentRelaxometryMapperCommand, 
                              Command, 
                              int32_t,
                              char** >;
    friend class Creator1Arg< MultiCompartmentRelaxometryMapperCommand,
                              Command,
                              const Dictionary& >;
    
    void parse();
    void execute( const std::vector< std::string >& fileNameT2WeightedVolumes,
                  const std::vector< std::string >& fileNameT1WeightedVolumes,
                  const std::string& fileNameB1Mapping,
                  const std::string& fileNameHCMapping,
                  const std::string& fileNameBCMapping,
                  const std::vector< std::string >& fileNameEchoTimes,
                  const std::string& fileNameSPGRFlipAngles,
                  const std::vector< std::string >& fileNameRepetitionTimes,
                  const std::string& fileNameMask,
                  const bool& hasCSF,
                  const int32_t& arteryCount, 
                  const std::string& fileNameParenchymaFraction, 
                  const std::string& fileNameParenchymaProtonDensity,
                  const std::string& fileNameParenchymaT1,
                  const std::string& fileNameParenchymaT2,
                  const std::vector< std::string>& fileNameArteriesFractions,
                  const std::string& fileNameArteriesProntoDensity,
                  const std::vector< std::string>& fileNameArteriesT1s,
                  const std::vector< std::string>& fileNameArteriesT2s, 
                  const std::string& fileNameCSFFraction, 
                  const std::string& fileNameCSFProtonDensity,
                  const std::string& fileNameCSFT1, 
                  const std::string& fileNameCSFT2, 
                  const std::string& fileNameT1WeightedScaling,
                  const std::string& fileNameT2WeightedScaling,
                  const std::string& fileNameFittedData, 
                  std::vector< double >& optimizerParameters, 
                  std::vector< double >& initialParameters, 
                  std::vector< double >& lowerParameterBoundaries, 
                  std::vector< double >& upperParameterBoundaries, 
                  std::vector< int32_t >& isFixedParameters, 
                  std::vector< double >& deltaParameters,
                  const std::vector< double >& t1NoiseStandardDeviations,
                  const std::vector< double >& t2NoiseStandardDeviations,
                  const bool competitiveT2, 
                  const bool competitiveT1, 
                  bool ascii, 
                  const std::string& format, 
                  bool verbose ); 

    void readTextFile( const std::string& fileName,
                       int32_t count, 
                       Vector& values ) const;

};


}


#endif
