#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperCommand.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/MultiCompartmentRelaxometryMapper/MultiCompartmentRelaxometryMapperGauge.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// class MultiCompartmentRelaxometryMapperCommand
////////////////////////////////////////////////////////////////////////////////

gkg::MultiCompartmentRelaxometryMapperCommand::MultiCompartmentRelaxometryMapperCommand(
                                                              int32_t argc, 
                                                              char* argv[], 
                                                              bool loadPlugin, 
                                                              bool removeFirst )
                           : gkg::Command( argc, argv, loadPlugin, removeFirst )

{
  
  try
  {
  
    parse();
    
  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryMapperCommand::"
             "MultiCompartmentRelaxometryMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MultiCompartmentRelaxometryMapperCommand::MultiCompartmentRelaxometryMapperCommand( 
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
                    const std::string& fileNameArteriesProtonDensity,
                    const std::vector< std::string >& fileNameArteriesT1s,
                    const std::vector< std::string >& fileNameArteriesT2s, 
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
                    bool verbose )
                                         : gkg::Command()
{
  
  try
  {
    
    execute( fileNameT2WeightedVolumes, 
             fileNameT1WeightedVolumes, 
             fileNameB1Mapping, 
             fileNameHCMapping, 
             fileNameBCMapping, 
             fileNameEchoTimes, 
             fileNameSPGRFlipAngles, 
             fileNameRepetitionTimes, 
             fileNameMask, 
             hasCSF, 
             arteryCount, 
             fileNameParenchymaFraction, 
             fileNameParenchymaProtonDensity, 
             fileNameParenchymaT1, 
             fileNameParenchymaT2, 
             fileNameArteriesFractions, 
             fileNameArteriesProtonDensity, 
             fileNameArteriesT1s, 
             fileNameArteriesT2s, 
             fileNameCSFFraction,
             fileNameCSFProtonDensity,
             fileNameCSFT1, 
             fileNameCSFT2,
             fileNameT1WeightedScaling, 
             fileNameT2WeightedScaling, 
             fileNameFittedData, 
             optimizerParameters, 
             initialParameters, 
             lowerParameterBoundaries, 
             upperParameterBoundaries, 
             isFixedParameters, 
             deltaParameters, 
             t1NoiseStandardDeviations, 
             t2NoiseStandardDeviations, 
             competitiveT2, 
             competitiveT1, 
             ascii, 
             format, 
             verbose );

  }
  GKG_CATCH( "gkg::MultiCompartmentRelaxometryMapperCommand::" 
             "MultiCompartmentRelaxometryMapperCommand( "
             "const std::vector< std::string >& fileNameT2WeightedVolumes, "
             "const std::vector< std::string >& fileNameT1WeightedVolumes, "
             "const std::string& fileNameB1Mapping, "
             "const std::string& fileNameHCMapping, "
             "const std::string& fileNameBCMapping, "
             "const std::vector< std::string >& fileNameEchoTimes, "
             "const std::string& fileNameSPGRFlipAngles, "
             "const std::vector< std::string >& fileNameRepetitionTimes, "
             "const std::string& fileNameMask, "
             "const bool& hasCSF, "
             "const int32_t& arteryCount, "
             "const std::string& fileNameParenchymaFraction, "
             "const std::string& fileNameParenchymaProtonDensity, "
             "const std::string& fileNameParenchymaT1, "
             "const std::string& fileNameParenchymaT2, "
             "const std::vector< std::string>& fileNameArteriesFractions, "
             "const std::string& fileNameArteriesProtonDensity, "
             "const std::vector< std::string>& fileNameArteriesT1s, "
             "const std::vector< std::string>& fileNameArteriesT2s, "
             "const std::string& fileNameT1WeightedScaling, "
             "const std::string& fileNameT2WeightedScaling, "
             "const std::string& fileNameFittedData, "
             "std::vector< double >& optimizerParameters, "
             "std::vector< double >& initialParameters, "
             "std::vector< double >& lowerParameterBoundaries, "
             "std::vector< double >& upperParameterBoundaries, "
             "std::vector< int32_t >& isFixedParameters, "
             "std::vector< double >& deltaParameters, "
             "const std::vector< double >& t1NoiseStandardDeviations, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "const bool competitiveT2, "
             "const bool competitiveT1, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::MultiCompartmentRelaxometryMapperCommand::MultiCompartmentRelaxometryMapperCommand( 
                                             const gkg::Dictionary& parameters )
                                         : gkg::Command( parameters )
{

  try
  {
    
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameT2WeightedVolumes );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameT1WeightedVolumes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameB1Mapping );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameHCMapping );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBCMapping );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameEchoTimes ); 
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSPGRFlipAngles );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameRepetitionTimes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, hasCSF );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, arteryCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameParenchymaFraction ); 
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameParenchymaProtonDensity ); 
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameParenchymaT1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameParenchymaT2 );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameArteriesFractions );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameArteriesProtonDensity );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameArteriesT1s );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameArteriesT2s ); 
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameT1WeightedScaling );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameT2WeightedScaling );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFittedData );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           initialParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           lowerParameterBoundaries );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           upperParameterBoundaries );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          isFixedParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           deltaParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           t1NoiseStandardDeviations );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           t2NoiseStandardDeviations );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, competitiveT2 );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, competitiveT1 );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
  
  }
  GKG_CATCH( "pkt::MultiCompartmentRelaxometryMapperCommand::"
             "MultiCompartmentRelaxometryMapperCommand( "
             "const pkt::Dictionary& parameters )" );
             
}


gkg::MultiCompartmentRelaxometryMapperCommand::~MultiCompartmentRelaxometryMapperCommand()
{
}


std::string gkg::MultiCompartmentRelaxometryMapperCommand::getStaticName()
{

  try
  {
  
    return "MultiCompartmentRelaxometryMapper"; 
    
  }
  GKG_CATCH( "std::string "
             "gkg::MultiCompartmentRelaxometryMapperCommand::getStaticName()" );
             
}


void gkg::MultiCompartmentRelaxometryMapperCommand::parse()
{

  try
  {
    
    std::vector< std::string > fileNameT2WeightedVolumes; 
    std::vector< std::string > fileNameT1WeightedVolumes;
    std::string fileNameB1Mapping;
    std::string fileNameHCMapping;
    std::string fileNameBCMapping; 
    std::vector< std::string > fileNameEchoTimes;
    std::string fileNameSPGRFlipAngles;
    std::vector< std::string > fileNameRepetitionTimes;
    std::string fileNameMask;
    bool hasCSF = false;
    int32_t arteryCount = 0;
    std::string fileNameParenchymaFraction; 
    std::string fileNameParenchymaProtonDensity;
    std::string fileNameParenchymaT1;
    std::string fileNameParenchymaT2;
    std::vector< std::string> fileNameArteriesFractions;
    std::string fileNameArteriesProtonDensity;
    std::vector< std::string> fileNameArteriesT1s;
    std::vector< std::string> fileNameArteriesT2s; 
    std::string fileNameCSFFraction;
    std::string fileNameCSFProtonDensity;
    std::string fileNameCSFT1;
    std::string fileNameCSFT2; 
    std::string fileNameT1WeightedScaling;
    std::string fileNameT2WeightedScaling;
    std::string fileNameFittedData; 
    std::vector< double > optimizerParameters; 
    std::vector< double > initialParameters; 
    std::vector< double > lowerParameterBoundaries; 
    std::vector< double > upperParameterBoundaries; 
    std::vector< int32_t > isFixedParameters; 
    std::vector< double > deltaParameters;
    std::vector< double > t1NoiseStandardDeviations;
    std::vector< double > t2NoiseStandardDeviations;
    bool competitiveT2 = false; 
    bool competitiveT1 = false; 
    bool ascii = false; 
    std::string format = ""; 
    bool verbose = false;
    
    gkg::Application application( _argc, _argv,
                                  "Multi compartment relaxometry mapper",
                                   _loadPlugin );

    application.addSeriesOption( "-t2w",
                                 "List of multi T2-weighted input file names",
                                 fileNameT2WeightedVolumes,
                                 0 );    
    application.addSeriesOption( "-t1w",
                                 "List of multi T1-weighted input file names",
                                 fileNameT1WeightedVolumes,
                                 0 );
    application.addSingleOption( "-b1",
                                 "B1 mapping input file name",
                                 fileNameB1Mapping,
                                 true );
    application.addSingleOption( "-hc",
                                 "Head Coil mapping input file name",
                                 fileNameHCMapping,
                                 true );
    application.addSingleOption( "-bc",
                                 "Body Coil mapping input file name",
                                 fileNameBCMapping,
                                 true );
    application.addSeriesOption( "-te",
                                 "List of TE echo time list text file names",
                                 fileNameEchoTimes,
                                 0 );    
    application.addSingleOption( "-fa",
                                 "SPGR FA flip angle list text file name "
                                 "(must be filled with -tr)",
                                 fileNameSPGRFlipAngles,
                                 true );
    application.addSeriesOption( "-tr", 
                                 "TR repetition time list text file names "
                                 "(must be filled with -fa)",
                                 fileNameRepetitionTimes, 
                                 0 );
    application.addSingleOption( "-m",
                                 "Input int16_t mask file name",
                                 fileNameMask );    
    application.addSingleOption( "-hasCSF", 
                                 "Boolean, turn true if presence of CSF "
                                 "(default = false)",
                                 hasCSF,
                                 true );
    application.addSingleOption( "-arteryCount", 
                                 "Number of arteries (default = 0)",
                                 arteryCount, 
                                 true );
    application.addSingleOption( "-pf", 
                                 "Parenchyma fraction ouptput volume", 
                                 fileNameParenchymaFraction, 
                                 true );    
    application.addSingleOption( "-ppd",
                                 "Parenchyma proton density output volume", 
                                 fileNameParenchymaProtonDensity, 
                                 true );     
    application.addSingleOption( "-pT1", 
                                 "Parenchyma T1 output volume", 
                                 fileNameParenchymaT1, 
                                 true );     
    application.addSingleOption( "-pT2", 
                                 "Parenchyma T2 output volume", 
                                 fileNameParenchymaT2, 
                                 true );
    application.addSeriesOption( "-af", 
                                 "List of arteries fraction output volumes", 
                                 fileNameArteriesFractions);    
    application.addSingleOption( "-apd", 
                                 "Arteries proton density output volume", 
                                 fileNameArteriesProtonDensity, 
                                 true );    
    application.addSeriesOption( "-aT1", 
                                 "List of arteries T1 output volumes", 
                                 fileNameArteriesT1s );
    application.addSeriesOption( "-aT2", 
                                 "List of arteries T2 output volumes", 
                                 fileNameArteriesT2s );
    application.addSingleOption( "-csff", 
                                 "CSF fraction ouptput volume", 
                                 fileNameCSFFraction, 
                                 true );    
    application.addSingleOption( "-csfpd",
                                 "CSF proton density output volume", 
                                 fileNameCSFProtonDensity, 
                                 true );     
    application.addSingleOption( "-csfT1", 
                                 "CSF T1 output volume", 
                                 fileNameCSFT1, 
                                 true );     
    application.addSingleOption( "-csfT2", 
                                 "CSF T2 output volume", 
                                 fileNameCSFT2, 
                                 true );
    application.addSingleOption( "-t1s",
                                 "T1-weighted scaling output volume",
                                 fileNameT1WeightedScaling,
                                 true );
    application.addSingleOption( "-t2s",
                                 "T2-weighted scaling output volume",
                                 fileNameT2WeightedScaling,
                                 true );
    application.addSingleOption( "-fitted",
                                 "Fitted data output volume",
                                 fileNameFittedData,
                                 true );
    application.addSeriesOption( "-optimizerParameters",
                                 "Parameters used during optimization:\n"
                                 ". <P1> : NLP maximum iteration count\n"
                                 ". <P2> : NLP test size\n"
                                 ". <P3> : 0->do not apply MCMC 1->apply MCMC\n"
                                 ". <P4> : MCMC burnin count\n"
                                 ". <P5> : MCMC sample count\n"
                                 ". <P6> : MCMC interval count\n"
                                 ". <P7> : MCMC maximum iteration count\n",
                                 optimizerParameters,
                                 7, 7 );
    application.addSeriesOption( "-initialParameters",
                                 "Initial parameters:\n"
                                 "( 01 ) -> parenchyma proton density \n"
                                 "( 02 ) -> parenchyma T1 \n"
                                 "( 03 ) -> parenchyma T2 \n"
                                 "( 04 ) -> parenchyma fraction \n"
                                 "( 05 ) -> CSF proton density \n"
                                 "( 06 ) -> CSF T1 \n"
                                 "( 07 ) -> CSF T2 \n"
                                 "( 08 ) -> CSF fraction \n"
                                 "( 09 ) -> arteries proton density \n"
                                 "( 10 ) -> artery 1 T1 \n"
                                 "( 11 ) -> artery 1 T2 \n"
                                 "( 12 ) -> artery 1 fraction \n"
                                 "so on, BUT no fraction for the last artery \n"
                                 "T2 scalings \n"
                                 "T1 scalings \n",
                                 initialParameters,
                                 2 );
    application.addSeriesOption( "-lowerParameterBoundaries", 
                                 "Lower parameter boundaries:\n"
                                 "( 01 ) -> parenchyma proton density \n"
                                 "( 02 ) -> parenchyma T1 \n"
                                 "( 03 ) -> parenchyma T2 \n"
                                 "( 04 ) -> parenchyma fraction \n"
                                 "( 05 ) -> CSF proton density \n"
                                 "( 06 ) -> CSF T1 \n"
                                 "( 07 ) -> CSF T2 \n"
                                 "( 08 ) -> CSF fraction \n"
                                 "( 09 ) -> arteries proton density \n"
                                 "( 10 ) -> artery 1 T1 \n"
                                 "( 11 ) -> artery 1 T2 \n"
                                 "( 12 ) -> artery 1 fraction \n"
                                 "so on, BUT no fraction for the last artery \n"
                                 "T2 scalings \n"
                                 "T1 scalings \n",
                                 lowerParameterBoundaries, 
                                 2 );
    application.addSeriesOption( "-upperParameterBoundaries", 
                                 "( 01 ) -> parenchyma proton density \n"
                                 "( 02 ) -> parenchyma T1 \n"
                                 "( 03 ) -> parenchyma T2 \n"
                                 "( 04 ) -> parenchyma fraction \n"
                                 "( 05 ) -> CSF proton density \n"
                                 "( 06 ) -> CSF T1 \n"
                                 "( 07 ) -> CSF T2 \n"
                                 "( 08 ) -> CSF fraction \n"
                                 "( 09 ) -> arteries proton density \n"
                                 "( 10 ) -> artery 1 T1 \n"
                                 "( 11 ) -> artery 1 T2 \n"
                                 "( 12 ) -> artery 1 fraction \n"
                                 "so on, BUT no fraction for the last artery \n"
                                 "T2 scalings \n"
                                 "T1 scalings \n",
                                 upperParameterBoundaries, 
                                 2 ); 
    application.addSeriesOption( "-isFixedParameters", 
                                 "0 or 1 to freeze a parameter to its initial "
                                 "value:\n"
                                 "( 01 ) -> parenchyma proton density \n"
                                 "( 02 ) -> parenchyma T1 \n"
                                 "( 03 ) -> parenchyma T2 \n"
                                 "( 04 ) -> parenchyma fraction \n"
                                 "( 05 ) -> CSF proton density \n"
                                 "( 06 ) -> CSF T1 \n"
                                 "( 07 ) -> CSF T2 \n"
                                 "( 08 ) -> CSF fraction \n"
                                 "( 09 ) -> arteries proton density \n"
                                 "( 10 ) -> artery 1 T1 \n"
                                 "( 11 ) -> artery 1 T2 \n"
                                 "( 12 ) -> artery 1 fraction \n"
                                 "so on, BUT no fraction for the last artery \n"
                                 "T2 scalings \n"
                                 "T1 scalings \n", 
                                 isFixedParameters, 
                                 2 );
    application.addSeriesOption( "-deltaParameters", 
                                 "Variation of parameters:\n"
                                 "( 01 ) -> parenchyma proton density \n"
                                 "( 02 ) -> parenchyma T1 \n"
                                 "( 03 ) -> parenchyma T2 \n"
                                 "( 04 ) -> parenchyma fraction \n"
                                 "( 05 ) -> CSF proton density \n"
                                 "( 06 ) -> CSF T1 \n"
                                 "( 07 ) -> CSF T2 \n"
                                 "( 08 ) -> CSF fraction \n"
                                 "( 09 ) -> arteries proton density \n"
                                 "( 10 ) -> artery 1 T1 \n"
                                 "( 11 ) -> artery 1 T2 \n"
                                 "( 12 ) -> artery 1 fraction \n"
                                 "so on, BUT no fraction for the last artery \n"
                                 "T2 scalings \n"
                                 "T1 scalings \n",
                                 deltaParameters, 
                                 2 );
    application.addSeriesOption( "-t1NoiseStdDevs",
                                 "List of noise standard deviation of "
                                 "T1-weighted data",
                                 t1NoiseStandardDeviations,
                                 0 );
    application.addSeriesOption( "-t2NoiseStdDevs",
                                 "List of noise standard deviation of "
                                 "T2-weighted data",
                                 t2NoiseStandardDeviations,
                                 0 );
    application.addSingleOption( "-competitiveT2", 
                                 "Apply a competitive model with a ponderation "
                                 "of the different T2 (default = false)",
                                 competitiveT2, 
                                 true );
    application.addSingleOption( "-competitiveT1", 
                                 "Apply a competitive model with a ponderation "
                                 "of the different T1 (default = false)",
                                 competitiveT1, 
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();


    execute( fileNameT2WeightedVolumes, 
             fileNameT1WeightedVolumes, 
             fileNameB1Mapping, 
             fileNameHCMapping,
             fileNameBCMapping,
             fileNameEchoTimes, 
             fileNameSPGRFlipAngles, 
             fileNameRepetitionTimes, 
             fileNameMask, 
             hasCSF, 
             arteryCount, 
             fileNameParenchymaFraction, 
             fileNameParenchymaProtonDensity, 
             fileNameParenchymaT1, 
             fileNameParenchymaT2, 
             fileNameArteriesFractions, 
             fileNameArteriesProtonDensity, 
             fileNameArteriesT1s, 
             fileNameArteriesT2s, 
             fileNameCSFFraction, 
             fileNameCSFProtonDensity, 
             fileNameCSFT1, 
             fileNameCSFT2, 
             fileNameT1WeightedScaling, 
             fileNameT2WeightedScaling, 
             fileNameFittedData, 
             optimizerParameters, 
             initialParameters, 
             lowerParameterBoundaries, 
             upperParameterBoundaries, 
             isFixedParameters, 
             deltaParameters, 
             t1NoiseStandardDeviations, 
             t2NoiseStandardDeviations, 
             competitiveT2, 
             competitiveT1, 
             ascii, 
             format, 
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MultiCompartmentRelaxometryMapperCommand::"
                     "parse()" );

}


void gkg::MultiCompartmentRelaxometryMapperCommand::execute( 
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
                    const std::string& fileNameArteriesProtonDensity,
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
                    bool verbose )
{

  try
  {
    
    bool t2Relaxometry = !fileNameT2WeightedVolumes.empty();
    bool t1Relaxometry = !fileNameT1WeightedVolumes.empty();
    bool hasB1Mapping = false;
    bool hasHCMapping = false;
    int32_t t2WeightedVolumeCount = ( int32_t )fileNameT2WeightedVolumes.size();
    int32_t t1WeightedVolumeCount = ( int32_t )fileNameT1WeightedVolumes.size();
    int32_t parameterCount = ( int32_t )initialParameters.size(); 
    int32_t a = 0;
    
    if ( verbose ) 
    {
      
      std::cout << std::endl; 
      
      if ( t2Relaxometry && t1Relaxometry )
      {
        
        std::cout << "Performing T1 and T2 Relaxometries" << std::endl;
        if( competitiveT2 )
        {
        
          std::cout << "With competitive T2" << std::endl;
        
        }
        else
        {
          
          std::cout << "Without competitive T2" << std::endl;
        
        }
        
        if ( competitiveT1 )
        {
        
          std::cout << "With competitive T1" << std::endl; 
          
        }
        else
        {
        
          std::cout << "Without competitive T1" << std::endl; 
        
        }
        
      }
      else if ( t2Relaxometry ) 
      {
      
        std::cout << "Permforming T2 Relaxometry" << std::endl;
        if ( competitiveT2 )
        {
          
          std::cout << "With competitive T2" << std::endl;
        
        }
        else
        {
          
          std::cout << "Without competitive T2" << std::endl; 
        
        }
        
      }
      else
      {
      
        std::cout << "Performing T1 Relaxometry" << std::endl;
        if ( competitiveT1 )
        {
          
          std::cout << "With competitive T1" << std::endl; 
        
        }
        else
        {
          
          std::cout << "Without competitive T1" << std::endl; 
        
        }
        
      }
            
      std::cout << "performing sanity checks : " << std::flush; 
      
    }

    if ( verbose )
    {
      
      std::cout << arteryCount << " arteries" << std::endl;
      if ( hasCSF )
      {
        
        std::cout << "Presence of CSF" << std::endl;
        
      }
      else
      {
        
        std::cout << "Without CSF" << std::endl;
      
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ( int32_t )lowerParameterBoundaries.size() != parameterCount || 
         ( int32_t )upperParameterBoundaries.size() != parameterCount ||
         ( int32_t )isFixedParameters.size() != parameterCount || 
         ( int32_t )deltaParameters.size() != parameterCount ) 
    {
    
      std::cout << "Number of parameters : " << parameterCount << std::endl;
      std::cout << "Number of lower boundaries : " << 
                                   lowerParameterBoundaries.size() << std::endl;
      std::cout << "Number of upper boundaries : " << 
                                   upperParameterBoundaries.size() << std::endl;
      std::cout << "Number of isFixedParameters : " << 
                                          isFixedParameters.size() << std::endl;
      std::cout << "Number of delta arameters : " << 
                                            deltaParameters.size() << std::endl;
      throw std::runtime_error( 
                    "not the same number of parameters and their constraints" ); 
      
    }

    if ( !fileNameSPGRFlipAngles.empty() )
    {

      if ( fileNameRepetitionTimes.empty() )
      {

        throw std::runtime_error( "missing repetition time text file or -tr "
                                  "option" );

      }
      
      if ( ( int32_t )fileNameRepetitionTimes.size() != t1WeightedVolumeCount )
      {
        
        throw std::runtime_error( "not the same number of T1 weighted volumes "
                                  "repetition time text files" );
      
      }
      
      if ( fileNameB1Mapping.empty() )
      {

        if ( verbose )
        {

          std::cout << "no b1 mapping provided to compute real flip angles"
                    << std::endl;

        }

      }
      else
      {

        hasB1Mapping = true;

      }
      
    }
    
    if ( !fileNameHCMapping.empty() && !fileNameBCMapping.empty() )
    {
      
      hasHCMapping = true;
    
    }
    else
    {
      
      if ( verbose )
      {
        
        std::cout << "no HC or BC mapping provided to compute corrections" 
                  << std::endl;
        
      }
      
    }
    
    if ( !fileNameArteriesFractions.empty() && 
         ( int32_t )fileNameArteriesFractions.size() != arteryCount ) 
    {
    
      throw std::runtime_error( 
                            "not the right number of Arteries Fraction files" );
    
    }
    
    if ( !fileNameArteriesT1s.empty() && 
         ( int32_t )fileNameArteriesT1s.size() != arteryCount ) 
    {
    
      throw std::runtime_error( "not the right number of Arteries T1 files" );
    
    }
    
    if ( !fileNameArteriesT2s.empty() && 
         ( int32_t )fileNameArteriesT2s.size() != arteryCount ) 
    {
    
      throw std::runtime_error( "not the right number of Arteries T2 files" );
    
    }
    
    if ( t1Relaxometry && 
         ( int32_t )fileNameRepetitionTimes.size() != t1WeightedVolumeCount )
    {
      
      throw std::runtime_error( 
         "not the same number of t1 weighted files and repetition time files" );
    
    }
    
    if ( t2Relaxometry && 
         ( int32_t )fileNameEchoTimes.size() != t2WeightedVolumeCount )
    {
    
      throw std::runtime_error( 
               "not the same number of t2 weighted files and echo time files" ); 
    
    }
        
    if ( t1Relaxometry && 
         ( int32_t )t1NoiseStandardDeviations.size() != t1WeightedVolumeCount ) 
    {
    
      throw std::runtime_error( 
                       "not the right number of t1 Noise Standard Deviations" ); 
    
    }
    
    if ( t2Relaxometry && 
         ( int32_t )t2NoiseStandardDeviations.size() != t2WeightedVolumeCount ) 
    {
    
      throw std::runtime_error( 
                       "not the right number of t2 Noise Standard Deviations" ); 
    
    }
    
    if ( !optimizerParameters.empty() && 
         ( int32_t )optimizerParameters.size() != 7 )
    {
    
      throw std::runtime_error( "not the right number of OptimizerParameters" );
    
    }
    
    if ( optimizerParameters.empty() )
    {
      
      optimizerParameters.resize( 7U ); 
      optimizerParameters[ 0 ] = 500.0; 
      optimizerParameters[ 1 ] = 1e-3; 
      optimizerParameters[ 2 ] = 0.0; 
      optimizerParameters[ 3 ] = 0.0; 
      optimizerParameters[ 4 ] = 0.0; 
      optimizerParameters[ 5 ] = 0.0; 
      optimizerParameters[ 6 ] = 0.0; 
      
    }
        
    if ( verbose )
    {
      
      std::cout << "done " << std::endl; 
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // preparing data for T2 relaxometry
    ////////////////////////////////////////////////////////////////////////////
    
    std::vector< gkg::Volume< float > > 
                                     t2WeightedVolumes( t2WeightedVolumeCount );
    int32_t globalT2WeightedMeasurementCount = 0;
    std::vector< gkg::Vector > echoTimes( t2WeightedVolumeCount );
    std::vector< int32_t > t2WeightedMeasurementCounts( t2WeightedVolumeCount );

    if ( t2Relaxometry ) 
    {
      
      if ( verbose ) 
      { 
        
        std::cout << "preparing data for T2 Relaxometry : " << std::endl; 
        
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // reading T2-weighted input data
      //////////////////////////////////////////////////////////////////////////
      
      if ( verbose ) 
      {

        std::cout << "reading T2-weighted input data : " << std::flush;

      }
      
      int32_t t2WeightedVolumeIndex = 0;
      for ( t2WeightedVolumeIndex = 0;
            t2WeightedVolumeIndex < t2WeightedVolumeCount;
            t2WeightedVolumeIndex++ )
      {

        gkg::TypedVolumeReaderProcess< float > 
          t2WeightedVolumeReaderProcess(
                                   t2WeightedVolumes[ t2WeightedVolumeIndex ] );
        t2WeightedVolumeReaderProcess.execute( 
                           fileNameT2WeightedVolumes[ t2WeightedVolumeIndex ] );

        if ( t2WeightedVolumeIndex > 0 )
        {

          if ( !t2WeightedVolumes[ t2WeightedVolumeIndex ].hasSameSize(
                                   t2WeightedVolumes[ 0 ], true ) ||
               !t2WeightedVolumes[ t2WeightedVolumeIndex ].hasSameResolution(
                                   t2WeightedVolumes[ 0 ], true ) )
          {

            throw std::runtime_error(
             "inconsistent size or resolution between T2-weighted datasets" );

          }

        }

      }


      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
    
    
      //////////////////////////////////////////////////////////////////////////
      // reading echo time text file 
      //////////////////////////////////////////////////////////////////////////
      
      if ( verbose )
      {

        std::cout << "reading TE echo time text file : " << std::flush;

      }

      for ( t2WeightedVolumeIndex = 0;
            t2WeightedVolumeIndex < t2WeightedVolumeCount;
            t2WeightedVolumeIndex++ )
      {

        t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ] =
                          t2WeightedVolumes[ t2WeightedVolumeIndex ].getSizeT();

        echoTimes[ t2WeightedVolumeIndex ].reallocate(
                         t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ] );
        this->readTextFile( fileNameEchoTimes[ t2WeightedVolumeIndex ],
                           t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ],
                           echoTimes[ t2WeightedVolumeIndex ] );

        globalT2WeightedMeasurementCount += 
                           t2WeightedMeasurementCounts[ t2WeightedVolumeIndex ];

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << globalT2WeightedMeasurementCount << " echo times : ";
        for ( t2WeightedVolumeIndex = 0;
              t2WeightedVolumeIndex < t2WeightedVolumeCount;
              t2WeightedVolumeIndex++ )
        {

          std::cout << echoTimes[ t2WeightedVolumeIndex ] << " " << std::flush; 

        }
        std::cout << std::endl;

      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // preparing data for T1 relaxometry
    ////////////////////////////////////////////////////////////////////////////
    
    std::vector< gkg::Volume< float > >
                                     t1WeightedVolumes( t1WeightedVolumeCount );
    int32_t globalT1WeightedMeasurementCount = 0;
    gkg::RCPointer< gkg::Volume< float > > b1MappingVolumePointer;
    std::vector< gkg::Vector > repetitionTimes( t1WeightedVolumeCount );
    gkg::Vector flipAngles( globalT1WeightedMeasurementCount ); 
    std::vector< int32_t > t1WeightedMeasurementCounts( t1WeightedVolumeCount );

    if ( t1Relaxometry ) 
    {
    
      if ( verbose )
      { 
        
        std::cout << "preparing data for T1 Relaxometry : " << std::endl; 
        
      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // reading T1-weighted input data
      //////////////////////////////////////////////////////////////////////////

      if ( verbose ) 
      {

        std::cout << "reading T1-weighted input data : " << std::flush;

      }
      
      int32_t t1WeightedVolumeIndex = 0;
      for ( t1WeightedVolumeIndex = 0;
            t1WeightedVolumeIndex < t1WeightedVolumeCount;
            t1WeightedVolumeIndex++ )
      {
        
        gkg::TypedVolumeReaderProcess< float > 
          t1WeightedVolumeReaderProcess(
                                   t1WeightedVolumes[ t1WeightedVolumeIndex ] );
        t1WeightedVolumeReaderProcess.execute( 
                           fileNameT1WeightedVolumes[ t1WeightedVolumeIndex ] );

        if ( t1WeightedVolumeIndex > 0 )
        {

          if ( !t1WeightedVolumes[ t1WeightedVolumeIndex ].hasSameSize(
                                   t1WeightedVolumes[ 0 ], true ) ||
               !t1WeightedVolumes[ t1WeightedVolumeIndex ].hasSameResolution(
                                   t1WeightedVolumes[ 0 ], true ) )
          {

            throw std::runtime_error(
             "inconsistent size or resolution between T1-weighted datasets" );

          }

        }
        
      }


      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
    
    
      //////////////////////////////////////////////////////////////////////////
      // reading repetition time text file 
      //////////////////////////////////////////////////////////////////////////
      
      if ( verbose )
      {

        std::cout << "reading TR repetition time text file : " << std::flush;

      }

      for ( t1WeightedVolumeIndex = 0;
            t1WeightedVolumeIndex < t1WeightedVolumeCount;
            t1WeightedVolumeIndex++ )
      {

        t1WeightedMeasurementCounts[ t1WeightedVolumeIndex ] =
                          t1WeightedVolumes[ t1WeightedVolumeIndex ].getSizeT();

        repetitionTimes[ t1WeightedVolumeIndex ].reallocate(
                         t1WeightedMeasurementCounts[ t1WeightedVolumeIndex ] );
        this->readTextFile( fileNameRepetitionTimes[ t1WeightedVolumeIndex ],
                           t1WeightedMeasurementCounts[ t1WeightedVolumeIndex ],
                            repetitionTimes[ t1WeightedVolumeIndex ] );

        globalT1WeightedMeasurementCount += 
                           t1WeightedMeasurementCounts[ t1WeightedVolumeIndex ];

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << globalT1WeightedMeasurementCount << " repetition times : ";
        for ( t1WeightedVolumeIndex = 0;
              t1WeightedVolumeIndex < t1WeightedVolumeCount;
              t1WeightedVolumeIndex++ )
        {

          std::cout << repetitionTimes[ t1WeightedVolumeIndex ] 
                    << " " << std::flush; 

        }
        std::cout << std::endl;

      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // reading flip angles
      //////////////////////////////////////////////////////////////////////////
            
      if ( verbose ) 
      { 
        
        std::cout << "reading FA flip angle text file : " << std::flush; 
      
      }
      
      this->readTextFile( fileNameSPGRFlipAngles, 
                          globalT1WeightedMeasurementCount, 
                          flipAngles ); 
      
      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << globalT1WeightedMeasurementCount << " flip angles : "
                  << flipAngles << std::endl;

      }
      
      
      //////////////////////////////////////////////////////////////////////////
      // reading B1 mapping if any 
      //////////////////////////////////////////////////////////////////////////
      
      if ( hasB1Mapping )
      {
      
        if ( verbose )
        {
        
          std::cout << "reading B1 mapping input data : " << std::flush;
        
        }
        
        b1MappingVolumePointer.reset( new gkg::Volume< float > );
        gkg::TypedVolumeReaderProcess< float >
          b1MappingVolumeReaderProcess( *b1MappingVolumePointer );
        b1MappingVolumeReaderProcess.execute( fileNameB1Mapping );
        if ( !b1MappingVolumePointer->hasSameSize( t1WeightedVolumes[ 0 ], 
                                                                       true ) ||
             !b1MappingVolumePointer->hasSameResolution( t1WeightedVolumes[ 0 ],
                                                                        true ) )
        {

          throw std::runtime_error(
           "inconsistent size or resolution of B1 and T1-weighted datasets" );

        }

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

      }
      else
      {

        if ( verbose )
        {

          std::cout << "no B1 mapping provided" << std::endl;

        }

      }
        
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // checking resolution between T1 and T2 datasets
    ////////////////////////////////////////////////////////////////////////////
    
    if ( t1Relaxometry && t2Relaxometry )
    {
      
      if ( !t1WeightedVolumes[ 0 ].hasSameSize( t2WeightedVolumes[ 0 ], true ) 
           || !t1WeightedVolumes[ 0 ].hasSameResolution(
                                                t2WeightedVolumes[ 0 ], true ) )
      {

        throw std::runtime_error( "inconsistent size or resolution between "
                                       "T1-weighted and T2-weighted datasets" );

      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // reading mask 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {
    
      std::cout << "reading mask : " << std::flush; 
      
    }
    
    gkg::Volume< float > maskVolume; 
    gkg::TypedVolumeReaderProcess< float >
      maskVolumeReaderProcess( maskVolume ); 
    maskVolumeReaderProcess.execute( fileNameMask ); 
    if ( t2Relaxometry && 
         ( !maskVolume.hasSameSize( t2WeightedVolumes[ 0 ], true ) ||
         !maskVolume.hasSameResolution( t2WeightedVolumes[ 0 ], true ) ) )
    {

      throw std::runtime_error(
           "inconsistent size or resolution of mask and T2-weighted datasets" );

    }
    
    if ( t1Relaxometry && 
         ( !maskVolume.hasSameSize( t1WeightedVolumes[ 0 ], true ) ||
         !maskVolume.hasSameResolution( t1WeightedVolumes[ 0 ], true ) ) )
    {

      throw std::runtime_error(
           "inconsistent size or resolution of mask and T1-weighted datasets" );

    }
    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // reading HC and BC volumes 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::RCPointer< gkg::Volume< float > > hcMappingVolumePointer, 
                                                         bcMappingVolumePointer;
    if ( hasHCMapping ) 
    {
      
      if ( verbose )
      {
        
        std::cout << "reading HC and BC mapping : " << std::flush;
        
      }
      
      hcMappingVolumePointer.reset( new gkg::Volume< float > );
      gkg::TypedVolumeReaderProcess< float >
        hcMappingVolumeReaderProcess( *hcMappingVolumePointer );
      hcMappingVolumeReaderProcess.execute( fileNameHCMapping );
      if ( !hcMappingVolumePointer->hasSameSize( maskVolume, true ) ||
           !hcMappingVolumePointer->hasSameResolution( maskVolume, true ) )
      {
        
        throw std::runtime_error( 
                              "inconsistent size or resolution of HC Mapping ");
      
      }
      
      bcMappingVolumePointer.reset( new gkg::Volume< float > );
      gkg::TypedVolumeReaderProcess< float >
        bcMappingVolumeReaderProcess( *bcMappingVolumePointer );
      bcMappingVolumeReaderProcess.execute( fileNameBCMapping );
      if ( !bcMappingVolumePointer->hasSameSize( maskVolume, true ) ||
           !bcMappingVolumePointer->hasSameResolution( maskVolume, true ) )
      {
        
        throw std::runtime_error( 
                              "inconsistent size or resolution of BC Mapping ");
      
      }
      
      if ( verbose )
      {
        
        std::cout << "done" << std::endl;
        
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // converting fixedParameters from int32_t to bool type 
    ////////////////////////////////////////////////////////////////////////////
    
    std::vector< bool > isFixedParametersBool( isFixedParameters.size() );
    std::vector< int32_t >::const_iterator
      f = isFixedParameters.begin(),
      fe = isFixedParameters.end();
    std::vector< bool >::iterator
      fb = isFixedParametersBool.begin();

    while ( f != fe )
    {

      *fb = ( ( ( int32_t )( *f + 0.5 ) == 1 ) ? true : false );
      ++ f;
      ++ fb;

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // preparing NLP constraints and optimizer parameters
    ////////////////////////////////////////////////////////////////////////////
    
    std::vector< gkg::RCPointer< gkg::OptimizerConstraint > > 
      constraints( parameterCount ); 
    std::vector< double > nlpOptimizerParameters( 2 + parameterCount ); 
    
    nlpOptimizerParameters[ 0 ] = optimizerParameters[ 0 ]; 
    nlpOptimizerParameters[ 1 ] = optimizerParameters[ 1 ]; 

    int32_t p = 0; 
    for ( p = 0 ; p < parameterCount; p++ )
    {
    
      constraints[ p ].reset( new gkg::ClosedIntervalOptimizerConstraint( 
                                              lowerParameterBoundaries[ p ], 
                                              upperParameterBoundaries[ p ] ) );
      nlpOptimizerParameters[ 2 + p ] = 
                             ( ( isFixedParametersBool[ p ] ) ? 
                               0.0 : 
                               constraints[ p ]->constrainedDeltaParameter( 
                                                       deltaParameters[ p ] ) );

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // preparing MCMC optimizer parameters
    ////////////////////////////////////////////////////////////////////////////
    
    bool applyMCMC = ( ( ( int32_t )( optimizerParameters[ 2 ] + 0.5 ) == 1 ) ? 
                                                                 true : false ); 
    int32_t mcmcBurninCount = optimizerParameters[ 3 ];
    int32_t mcmcSampleCount = optimizerParameters[ 4 ]; 
    int32_t mcmcIntervalCount = optimizerParameters[ 5 ]; 
    int32_t mcmcMaximumIterationCount = optimizerParameters[ 6 ]; 
    
    
    ////////////////////////////////////////////////////////////////////////////
    // collecting size, resolution T2-weighted and T1-weighted meas. counts 
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::Vector3d< int32_t > volumeSize; 
    gkg::Vector3d< double > volumeResolution( 1.0, 1.0, 1.0 ); 
    double resolutionT = 1.0; 
    
    if ( t2Relaxometry )
    {
    
      t2WeightedVolumes[ 0 ].getSize( volumeSize ); 
      t2WeightedVolumes[ 0 ].getResolution( volumeResolution, resolutionT ); 

    }
    else
    {
    
      t1WeightedVolumes[ 0 ].getSize( volumeSize ); 
      t1WeightedVolumes[ 0 ].getResolution( volumeResolution, resolutionT ); 
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // allocating output volumes
    ////////////////////////////////////////////////////////////////////////////
    
    gkg::Volume< float > parenchymaFractionVolume( volumeSize ); 
    gkg::Volume< float > parenchymaProtonDensityVolume( volumeSize ); 
    gkg::Volume< float > parenchymaT1Volume( volumeSize ); 
    gkg::Volume< float > parenchymaT2Volume( volumeSize ); 
    gkg::Volume< float > arteriesProtonDensityVolume( volumeSize );

    parenchymaFractionVolume.setResolution( volumeResolution, resolutionT ); 
    parenchymaProtonDensityVolume.setResolution( volumeResolution, 
                                                                  resolutionT ); 
    parenchymaT1Volume.setResolution( volumeResolution, resolutionT ); 
    parenchymaT2Volume.setResolution( volumeResolution, resolutionT ); 
    arteriesProtonDensityVolume.setResolution( volumeResolution, resolutionT ); 
    
    parenchymaFractionVolume.fill( 0.0f ); 
    parenchymaProtonDensityVolume.fill( 0.0f ); 
    parenchymaT1Volume.fill( 0.0f ); 
    parenchymaT2Volume.fill( 0.0f ); 
    arteriesProtonDensityVolume.fill( 0.0f ); 
    
    gkg::Volume< float > csfFractionVolume( volumeSize );
    gkg::Volume< float > csfProtonDensityVolume( volumeSize );
    gkg::Volume< float > csfT1Volume( volumeSize );
    gkg::Volume< float > csfT2Volume( volumeSize );
    
    if ( hasCSF )
    {
      
      csfFractionVolume.setResolution( volumeResolution, resolutionT );
      csfProtonDensityVolume.setResolution( volumeResolution, resolutionT );
      csfT1Volume.setResolution( volumeResolution, resolutionT );
      csfT2Volume.setResolution( volumeResolution, resolutionT );
      
      csfFractionVolume.fill( 0.0f );
      csfProtonDensityVolume.fill( 0.0f );
      csfT1Volume.fill( 0.0f );
      csfT2Volume.fill( 0.0f );
    
    }

    std::vector< gkg::Volume< float > > arteriesFractionVolumes( arteryCount );
    std::vector< gkg::Volume< float > > arteriesT1Volumes( arteryCount ); 
    std::vector< gkg::Volume< float > > arteriesT2Volumes( arteryCount );
    
    for ( a = 0; a < arteryCount; a++ )
    {
    
      arteriesFractionVolumes[ a ].reallocate( volumeSize ); 
      arteriesFractionVolumes[ a ].setResolution( volumeResolution, 
                                                                  resolutionT ); 
      arteriesFractionVolumes[ a ].fill( 0.0f ); 
      
      arteriesT1Volumes[ a ].reallocate( volumeSize ); 
      arteriesT1Volumes[ a ].setResolution( volumeResolution, resolutionT ); 
      arteriesT1Volumes[ a ].fill( 0.0f ); 
      
      arteriesT2Volumes[ a ].reallocate( volumeSize ); 
      arteriesT2Volumes[ a ].setResolution( volumeResolution, resolutionT ); 
      arteriesT2Volumes[ a ].fill( 0.0f ); 
      
    }
    
    gkg::Volume< float > t2WeightedScalingVolume; 
    
    if ( t2WeightedVolumeCount > 1 )
    {
      
      t2WeightedScalingVolume.reallocate( volumeSize, 
                                          t2WeightedVolumeCount - 1 ); 
      t2WeightedScalingVolume.setResolution( volumeResolution ); 
      t2WeightedScalingVolume.fill( 1.0f ); 
      
    }
    
    gkg::Volume< float > t1WeightedScalingVolume( 0.0f, 0.0f, 0.0f, 0.0f ); 
    
    if ( t2Relaxometry && t1Relaxometry )
    {
      
      t1WeightedScalingVolume.reallocate( volumeSize, t1WeightedVolumeCount ); 
      t1WeightedScalingVolume.setResolution( volumeResolution ); 
      t1WeightedScalingVolume.fill( 1.0f );
      
    }    
    else if ( t1WeightedVolumeCount > 1 )
    {
    
      t1WeightedScalingVolume.reallocate( volumeSize, 
                                                    t1WeightedVolumeCount - 1 ); 
      t1WeightedScalingVolume.setResolution( volumeResolution ); 
      t1WeightedScalingVolume.fill( 1.0f );
      
    }
    
    bool computeFittedData = !fileNameFittedData.empty(); 
    gkg::Volume< float > fittedDataVolume; 
    if ( computeFittedData ) 
    {
    
      fittedDataVolume.reallocate( volumeSize, 
          globalT2WeightedMeasurementCount + globalT1WeightedMeasurementCount ); 
      fittedDataVolume.setResolution( volumeResolution, resolutionT ); 
      fittedDataVolume.fill( 0.0f ); 
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // computing vector of sites in mask 
    ////////////////////////////////////////////////////////////////////////////
    
    std::vector< gkg::Vector3d< int32_t > > sites;
    sites.reserve( volumeSize.x * volumeSize.y * volumeSize.z );
    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < volumeSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < volumeSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < volumeSize.x; site.x++ )
        {

          if ( maskVolume( site ) > 0 )
          {

            sites.push_back( site );

          }

        }

      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // creating threaded loop context and launching multithreaded loop 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {
      
      std::cout << "computing parenchyma/arteries parameters : " << std::flush;
      
    }
    
    gkg::MultiCompartmentRelaxometryMapperGauge gauge( ( int32_t ) sites.size() );
    gkg::Vector vectorOfLowerParameterBoundaries( lowerParameterBoundaries );
    gkg::Vector vectorOfUpperParameterBoundaries( upperParameterBoundaries );
    gkg::Vector vectorOfDeltaParameters( deltaParameters );
    
    gkg::MultiCompartmentRelaxometryMapperLoopContext
      multiCompartmentRelaxometryMapperLoopContext( gauge, 
                                               sites, 
                                               t2Relaxometry, 
                                               t1Relaxometry, 
                                               competitiveT2, 
                                               competitiveT1, 
                                               t2WeightedVolumes, 
                                               t1WeightedVolumes, 
                                               b1MappingVolumePointer, 
                                               hcMappingVolumePointer,
                                               bcMappingVolumePointer,
                                               echoTimes, 
                                               flipAngles, 
                                               repetitionTimes, 
                                               t2NoiseStandardDeviations, 
                                               t1NoiseStandardDeviations, 
                                               constraints, 
                                               nlpOptimizerParameters, 
                                               initialParameters, 
                                               vectorOfLowerParameterBoundaries,
                                               vectorOfUpperParameterBoundaries,
                                               isFixedParametersBool, 
                                               vectorOfDeltaParameters, 
                                               applyMCMC, 
                                               mcmcBurninCount, 
                                               mcmcSampleCount, 
                                               mcmcIntervalCount, 
                                               mcmcMaximumIterationCount, 
                                               parenchymaFractionVolume, 
                                               parenchymaProtonDensityVolume, 
                                               parenchymaT1Volume, 
                                               parenchymaT2Volume, 
                                               arteriesFractionVolumes, 
                                               arteriesProtonDensityVolume, 
                                               arteriesT1Volumes, 
                                               arteriesT2Volumes, 
                                               csfFractionVolume,
                                               csfProtonDensityVolume,
                                               csfT1Volume,
                                               csfT2Volume, 
                                               t2WeightedScalingVolume, 
                                               t1WeightedScalingVolume, 
                                               computeFittedData, 
                                               fittedDataVolume, 
                                               t2WeightedVolumeCount, 
                                               t1WeightedVolumeCount, 
                                               globalT2WeightedMeasurementCount,
                                               globalT1WeightedMeasurementCount,
                                               t2WeightedMeasurementCounts, 
                                               t1WeightedMeasurementCounts, 
                                               arteryCount, 
                                               parameterCount, 
                                               hasCSF,
                                               verbose ); 
    
    // creating a threaded loop
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &multiCompartmentRelaxometryMapperLoopContext, 
                    0, 
                    ( int32_t )sites.size() );
    
    // launching the threaded loop
    threadedLoop.launch();
    
    if ( verbose )
    {
      
      std::cout << "done" << std::endl;
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving parenchyma fraction map 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameParenchymaFraction.empty() )
    {
    
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameParenchymaFraction << "' : "
                  << std::flush; 
        
        }
        
      gkg::Writer::getInstance().write( fileNameParenchymaFraction, 
                                         parenchymaFractionVolume, 
                                         ascii, 
                                         format ); 

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving parenchyma proton density map 
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameParenchymaProtonDensity.empty() )
    {
        
      if (verbose )
      {
      
        std::cout << "writing '" << fileNameParenchymaProtonDensity << "' : "
                  << std::flush; 
      
      }

      gkg::Writer::getInstance().write( fileNameParenchymaProtonDensity, 
                                        parenchymaProtonDensityVolume, 
                                        ascii, 
                                        format );

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving parenchyma T1 map
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameParenchymaT1.empty() )
    {
      
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameParenchymaT1 << "' : "
                  << std::flush; 
      
      }
      
      gkg::Writer::getInstance().write( fileNameParenchymaT1, 
                                        parenchymaT1Volume, 
                                        ascii, 
                                        format );
      
      if ( verbose )
      {
        
        std::cout << "done " << std::endl; 
      
      }
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving parenchyma T2 map
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameParenchymaT2.empty() )
    {
      
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameParenchymaT2 << "' : "
                  << std::flush; 
      
      }
      
      gkg::Writer::getInstance().write( fileNameParenchymaT2, 
                                        parenchymaT2Volume, 
                                        ascii, 
                                        format );
      
      if ( verbose )
      {
        
        std::cout << "done " << std::endl; 
      
      }
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving arteries fraction maps
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ( int32_t )fileNameArteriesFractions.size() > 0 )
    {
    
      for ( a = 0; a < arteryCount; a++ )
      {
        
        if ( verbose )
        {
          
          std::cout << "writing '" << fileNameArteriesFractions[ a ] 
                    << "' : " << std::flush; 
        
        }
        
        gkg::Writer::getInstance().write( fileNameArteriesFractions[ a ], 
                                          arteriesFractionVolumes[ a ], 
                                          ascii, 
                                          format );
        
        if ( verbose ) 
        {
          
          std::cout << "done " << std::endl; 
        
        }
      
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving arteries proton density
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameArteriesProtonDensity.empty() )
    {
      
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameArteriesProtonDensity << "' : " 
                  << std::flush; 
      
      }
    
      gkg::Writer::getInstance().write( fileNameArteriesProtonDensity,
                                        arteriesProtonDensityVolume, 
                                        ascii, 
                                        format );
      
      if ( verbose )
      {
        
        std::cout << "done " << std::endl;
      
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving arteries T1 maps
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ( int32_t )fileNameArteriesT1s.size() > 0 )
    {
    
      for ( a = 0; a < arteryCount; a++ )
      {
        
        if ( verbose )
        {
          
          std::cout << "writing '" << fileNameArteriesT1s[ a ] 
                    << "' : " << std::flush; 
        
        }
        
        gkg::Writer::getInstance().write( fileNameArteriesT1s[ a ], 
                                          arteriesT1Volumes[ a ], 
                                          ascii, 
                                          format );
        
        if ( verbose ) 
        {
          
          std::cout << "done " << std::endl; 
        
        }
      
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving arteries T2 maps
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ( int32_t )fileNameArteriesT2s.size() > 0 )
    {
    
      for ( a = 0; a < arteryCount; a++ )
      {
        
        if ( verbose )
        {
          
          std::cout << "writing '" << fileNameArteriesT2s[ a ] 
                    << "' : " << std::flush; 
        
        }
        
        gkg::Writer::getInstance().write( fileNameArteriesT2s[ a ], 
                                          arteriesT2Volumes[ a ], 
                                          ascii, 
                                          format );
        
        if ( verbose ) 
        {
          
          std::cout << "done " << std::endl; 
        
        }
      
      }
    
    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving CSF fraction map
    ////////////////////////////////////////////////////////////////////////////

    if ( hasCSF && !fileNameCSFFraction.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameCSFFraction << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameCSFFraction,
                                        csfFractionVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving CSF proton density map
    ////////////////////////////////////////////////////////////////////////////

    if ( hasCSF && !fileNameCSFProtonDensity.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameCSFProtonDensity << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameCSFProtonDensity,
                                        csfProtonDensityVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving CSF T1 map
    ////////////////////////////////////////////////////////////////////////////

    if ( hasCSF && !fileNameCSFT1.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameCSFT1 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameCSFT1,
                                        csfT1Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving CSF T2 map
    ////////////////////////////////////////////////////////////////////////////

    if ( hasCSF && !fileNameCSFT2.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameCSFT2 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameCSFT2,
                                        csfT2Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving T1 scaling map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameT1WeightedScaling.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameT1WeightedScaling << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameT1WeightedScaling,
                                        t1WeightedScalingVolume,
                                        ascii,
                                        format );
      
      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving T2 scaling map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameT2WeightedScaling.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameT2WeightedScaling << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameT2WeightedScaling,
                                        t2WeightedScalingVolume,
                                        ascii,
                                        format );
      
      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving fitted data
    ////////////////////////////////////////////////////////////////////////////

    if ( computeFittedData )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameFittedData << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameFittedData,
                                        fittedDataVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }
        
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperCommand::execute( "
             "const std::vector< std::string >& fileNameT2WeightedVolumes, "
             "const std::vector< std::string >& fileNameT1WeightedVolumes, "
             "const std::string& fileNameB1Mapping, "
             "const std::string& fileNameHCMapping, "
             "const std::string& fileNameBCMapping, "
             "const std::vector< std::string >& fileNameEchoTimes, "
             "const std::string& fileNameSPGRFlipAngles, "
             "const std::vector< std::string >& fileNameRepetitionTimes, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameParenchymaFraction, "
             "const std::string& fileNameParenchymaProtonDensity, "
             "const std::string& fileNameParenchymaT1, "
             "const std::string& fileNameParenchymaT2, "
             "const std::vector< std::string>& fileNameArteriesFractions, "
             "const std::string& fileNameArteriesProtonDensity, "
             "const std::vector< std::string>& fileNameArteriesT1s, "
             "const std::vector< std::string>& fileNameArteriesT2s, "
             "const std::string& fileNameT1WeightedScaling, "
             "const std::string& fileNameT2WeightedScaling, "
             "const std::string& fileNameFittedData, "
             "std::vector< double >& optimizerParameters, "
             "std::vector< double >& initialParameters, "
             "std::vector< double >& lowerParameterBoundaries, "
             "std::vector< double >& upperParameterBoundaries, "
             "std::vector< int32_t >& isFixedParameters, "
             "std::vector< double >& deltaParameters, "
             "const std::vector< double >& t1NoiseStandardDeviations, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


void gkg::MultiCompartmentRelaxometryMapperCommand::readTextFile( 
                                                    const std::string& fileName, 
                                                    int32_t count, 
                                                    gkg::Vector& values ) const
{

  try
  {
  
    if ( values.getSize() != count )
    {
    
      values.reallocate( count ); 
      
    }
    
    std::ifstream is ( fileName.c_str() );
    
    if ( !is )
    {
    
      throw std::runtime_error( std::string("unable to open '" ) + 
                                fileName + "'" );

    }
    
    int32_t t; 
    for ( t = 0; t < count; t++ ) 
    {
    
      if ( is.eof() )
      {
      
        throw std::runtime_error(
                                "not enough values provided in the text file" );
      
      }
      is >> values( t );
      
    }
    
    is.close();
    
  }
  GKG_CATCH( "void gkg::MultiCompartmentRelaxometryMapperCommand::"
             "readTextFile( "
             "const std::string& fileName, "
             "int32_t count, "
             "gkg::Vector& values ) const" );
             
}


RegisterCommandCreator( 
       MultiCompartmentRelaxometryMapperCommand, 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameT2WeightedVolumes ) +
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameT1WeightedVolumes ) +
       DECLARE_STRING_PARAMETER_HELP( fileNameB1Mapping ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameHCMapping ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameBCMapping ) + 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameEchoTimes ) + 
       DECLARE_STRING_PARAMETER_HELP( fileSPGRFlipAngles ) + 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameRepetitionTimes ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameMask ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameParenchymaFraction ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameParenchymaProtonDensity ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameParenchymaT1 ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameParenchymaT2 ) + 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameArteriesFractions ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameArteriesProtonDensity ) + 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameArteriesT1s ) + 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameArteriesT2s ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameT1WeightedScaling ) +
       DECLARE_STRING_PARAMETER_HELP( fileNameT2WeightedScaling ) +
       DECLARE_STRING_PARAMETER_HELP( fileNameFittedData ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( lowerParameterBoundaries ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( upperParameterBoundaries ) +
       DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( isFixedParameters ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( deltaParameters ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( t1NoiseStandardDeviations ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( t2NoiseStandardDeviations ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
       DECLARE_STRING_PARAMETER_HELP( format ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
    

