#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperCommand.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/MyelinWaterFractionMapper/MyelinWaterFractionMapperGauge.h>
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
// class MyelinWaterFractionMapperCommand
////////////////////////////////////////////////////////////////////////////////

gkg::MyelinWaterFractionMapperCommand::MyelinWaterFractionMapperCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                      : gkg::Command( argc, argv,
                                                      loadPlugin, removeFirst )

{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::MyelinWaterFractionMapperCommand::"
             "MyelinWaterFractionMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MyelinWaterFractionMapperCommand::MyelinWaterFractionMapperCommand(
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
                    bool verbose )
                                     : gkg::Command()
{

  try
  {

    execute( fileNameT2WeightedVolumes,
             fileNameT1WeightedVolume,
             fileNameB1Mapping,
             fileNameEchoTimes,
             fileNameInversionTimes,
             fileNameSPGRFlipAngles,
             fileNameRepetitionTimes,
             fileNameMask,
             fileNameMyelinWaterFraction,
             fileNameMyelinWaterT1,
             fileNameMyelinWaterT2,
             fileNameGrayWhiteFraction,
             fileNameGrayWhiteT1,
             fileNameGrayWhiteT2,
             fileNameCSFFraction,
             fileNameCSFT1,
             fileNameCSFT2,
             fileNameProtonDensity,
             fileNameProtonDensityScaling,
             fileNameT2WeightedScaling,
             fileNameFittedData,
             optimizerParameters,
             initialParameters,
             lowerParameterBoundaries,
             upperParameterBoundaries,
             isFixedParameters,
             deltaParameters,
             t2NoiseStandardDeviations,
             t1NoiseStandardDeviation,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::MyelinWaterFractionMapperCommand::"
             "MyelinWaterFractionMapperCommand( "
             "const std::vector< std::string >& fileNameT2WeightedVolumes, "
             "const std::string& fileNameT1WeightedVolume, "
             "const std::string& fileNameB1Mapping, "
             "const std::vector< std::string >& fileNameEchoTimes, "
             "const std::string& fileNameInversionTimes, "
             "const std::string& fileNameSPGRFlipAngles, "
             "const std::string& fileNameRepetitionTimes, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameMyelinWaterFraction, "
             "const std::string& fileNameMyelinWaterT1, "
             "const std::string& fileNameMyelinWaterT2, "
             "const std::string& fileNameGrayWhiteFraction, "
             "const std::string& fileNameGrayWhiteT1, "
             "const std::string& fileNameGrayWhiteT2, "
             "const std::string& fileNameCSFFraction, "
             "const std::string& fileNameCSFT1, "
             "const std::string& fileNameCSFT2, "
             "const std::string& fileNameProtonDensity, "
             "const std::string& fileNameProtonDensityScaling, "
             "const std::string& fileNameT2WeightedScaling, "
             "const std::string& fileNameFittedData, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< double >& initialParameters, "
             "const std::vector< double >& lowerParameterBoundaries, "
             "const std::vector< double >& upperParameterBoundaries, "
             "const std::vector< int32_t >& isFixedParameters, "
             "const std::vector< double >& deltaParameters, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "double t1NoiseStandardDeviation, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::MyelinWaterFractionMapperCommand::MyelinWaterFractionMapperCommand(
                                             const gkg::Dictionary& parameters )
                                      : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameT2WeightedVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameT1WeightedVolume );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameB1Mapping );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         fileNameEchoTimes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInversionTimes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSPGRFlipAngles );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameRepetitionTimes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMyelinWaterFraction );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMyelinWaterT1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMyelinWaterT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, 
                              fileNameGrayWhiteFraction );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameGrayWhiteT1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameGrayWhiteT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCSFFraction );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCSFT1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCSFT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameProtonDensity );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameProtonDensityScaling );
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
                                           t2NoiseStandardDeviations );
    DECLARE_FLOATING_PARAMETER( parameters, double, t1NoiseStandardDeviation );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

  }
  GKG_CATCH( "gkg::MyelinWaterFractionMapperCommand::"
             "MyelinWaterFractionMapperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MyelinWaterFractionMapperCommand::~MyelinWaterFractionMapperCommand()
{
}


std::string gkg::MyelinWaterFractionMapperCommand::getStaticName()
{

  try
  {

    return "MyelinWaterFractionMapper";

  }
  GKG_CATCH( "std::string "
             "gkg::MyelinWaterFractionMapperCommand::getStaticName()" );

}


void gkg::MyelinWaterFractionMapperCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameT2WeightedVolumes;
    std::string fileNameT1WeightedVolume;
    std::string fileNameB1Mapping;
    std::vector< std::string > fileNameEchoTimes;
    std::string fileNameInversionTimes;
    std::string fileNameSPGRFlipAngles;
    std::string fileNameRepetitionTimes;
    std::string fileNameMask;
    std::string fileNameMyelinWaterFraction;
    std::string fileNameMyelinWaterT1;
    std::string fileNameMyelinWaterT2;
    std::string fileNameGrayWhiteFraction;
    std::string fileNameGrayWhiteT1;
    std::string fileNameGrayWhiteT2;
    std::string fileNameCSFFraction;
    std::string fileNameCSFT1;
    std::string fileNameCSFT2;
    std::string fileNameProtonDensity;
    std::string fileNameProtonDensityScaling;
    std::string fileNameT2WeightedScaling;
    std::string fileNameFittedData;
    std::vector< double > optimizerParameters;
    std::vector< double > initialParameters;
    std::vector< double > lowerParameterBoundaries;
    std::vector< double > upperParameterBoundaries;
    std::vector< int32_t > isFixedParameters;
    std::vector< double > deltaParameters;
    std::vector< double > t2NoiseStandardDeviations;
    double t1NoiseStandardDeviation;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Myelin water fraction mapper",
                                   _loadPlugin );

    application.addSeriesOption( "-t2w",
                                 "List of multi T2-weighted input file names",
                                 fileNameT2WeightedVolumes,
                                 1 );
    application.addSingleOption( "-t1w",
                                 "Multiple T1-weighted input file name",
                                 fileNameT1WeightedVolume );
    application.addSingleOption( "-b1",
                                 "B1 mapping input file name",
                                 fileNameB1Mapping,
                                 true );
    application.addSeriesOption( "-te",
                                 "List of TE echo time list text file names",
                                 fileNameEchoTimes,
                                 1 );
    application.addSingleOption( "-ti",
                                 "TI inversion time list text file name "
                                 "( cannot be used if fa and tr options "
                                 " are used)",
                                 fileNameInversionTimes,
                                 true );
    application.addSingleOption( "-fa",
                                 "SPGR FA flip angle list text file name "
                                 "( tr option must be filled if this option "
                                 "is used)",
                                 fileNameSPGRFlipAngles,
                                 true );
    application.addSingleOption( "-tr",
                                 "TR repetition time list text file name "
                                 "( fa option must be filled if this option "
                                 "is used)",
                                 fileNameRepetitionTimes,
                                 true );
    application.addSingleOption( "-m",
                                 "Input int16_t mask file name",
                                 fileNameMask );
    application.addSingleOption( "-mwFraction",
                                 "Myelin water fraction output volume",
                                 fileNameMyelinWaterFraction,
                                 true );
    application.addSingleOption( "-mwT1",
                                 "Myelin water T1 output volume",
                                 fileNameMyelinWaterT1,
                                 true );
    application.addSingleOption( "-mwT2",
                                 "Myelin water T2 output volume",
                                 fileNameMyelinWaterT2,
                                 true );
    application.addSingleOption( "-gwmFraction",
                                 "Gray/White matter fraction output volume",
                                 fileNameGrayWhiteFraction,
                                 true );
    application.addSingleOption( "-gwmT1",
                                 "Gray/White matter T1 output volume",
                                 fileNameGrayWhiteT1,
                                 true );
    application.addSingleOption( "-gwmT2",
                                 "Gray/White matter T2 output volume",
                                 fileNameGrayWhiteT2,
                                 true );
    application.addSingleOption( "-csfFraction",
                                 "CSF fraction output volume",
                                 fileNameCSFFraction,
                                 true );
    application.addSingleOption( "-csfT1",
                                 "CSF T1 output volume",
                                 fileNameCSFT1,
                                 true );
    application.addSingleOption( "-csfT2",
                                 "CSF T2 output volume",
                                 fileNameCSFT2,
                                 true );
    application.addSingleOption( "-pd",
                                 "Proton density output volume",
                                 fileNameProtonDensity,
                                 true );
    application.addSingleOption( "-pds",
                                 "Proton density scaling output volume",
                                 fileNameProtonDensityScaling,
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
                                 0, 7 );
    application.addSeriesOption( "-initialParameters",
                                 "Initial parameters:\n"
                                 "- (  0 ) -> myelinWaterFraction\n"
                                 "- (  1 ) -> myelinWaterT1\n"
                                 "- (  2 ) -> myelinWaterT2\n"
                                 "- (  3 ) -> grayWhiteT1\n"
                                 "- (  4 ) -> grayWhiteT2\n"
                                 "- (  5 ) -> csfFraction\n"
                                 "- (  6 ) -> csfT1\n"
                                 "- (  7 ) -> csfT2\n"
                                 "- (  8 ) -> protonDensity\n"
                                 "- (  9 ) -> protonDensityScaling\n"
                                 "- ( 10 ) -> scaling of second multi "
                                 "T2-weighted volume\n"
                                 "   ...\n"
                                 "- ( 10 + N ) -> scaling of (N+1)th multi "
                                 "T2-weighted volume\n",
                                   initialParameters,
                                 0 );
    application.addSeriesOption( "-lowerParameterBoundaries",
                                 "Lower parameter boundaries:\n"
                                 "- (  0 ) -> myelinWaterFraction\n"
                                 "- (  1 ) -> myelinWaterT1\n"
                                 "- (  2 ) -> myelinWaterT2\n"
                                 "- (  3 ) -> grayWhiteT1\n"
                                 "- (  4 ) -> grayWhiteT2\n"
                                 "- (  5 ) -> csfFraction\n"
                                 "- (  6 ) -> csfT1\n"
                                 "- (  7 ) -> csfT2\n"
                                 "- (  8 ) -> protonDensity\n"
                                 "- (  9 ) -> protonDensityScaling\n"
                                 "- ( 10 ) -> scaling of second multi "
                                 "T2-weighted volume\n"
                                 "   ...\n"
                                 "- ( 10 + N ) -> scaling of (N+1)th multi "
                                 "T2-weighted volume\n",
                                 lowerParameterBoundaries,
                                 0 );
    application.addSeriesOption( "-upperParameterBoundaries",
                                 "Upper parameter boundaries:\n"
                                 "- (  0 ) -> myelinWaterFraction\n"
                                 "- (  1 ) -> myelinWaterT1\n"
                                 "- (  2 ) -> myelinWaterT2\n"
                                 "- (  3 ) -> grayWhiteT1\n"
                                 "- (  4 ) -> grayWhiteT2\n"
                                 "- (  5 ) -> csfFraction\n"
                                 "- (  6 ) -> csfT1\n"
                                 "- (  7 ) -> csfT2\n"
                                 "- (  8 ) -> protonDensity\n"
                                 "- (  9 ) -> protonDensityScaling\n"
                                 "- ( 10 ) -> scaling of second multi "
                                 "T2-weighted volume\n"
                                 "   ...\n"
                                 "- ( 10 + N ) -> scaling of (N+1)th multi "
                                 "T2-weighted volume\n",
                                 upperParameterBoundaries,
                                 0 );
    application.addSeriesOption( "-isFixedParameters",
                                 "0 or 1 to freeze a parameter to its initial "
                                 "value:\n"
                                 "- (  0 ) -> myelinWaterFraction\n"
                                 "- (  1 ) -> myelinWaterT1\n"
                                 "- (  2 ) -> myelinWaterT2\n"
                                 "- (  3 ) -> grayWhiteT1\n"
                                 "- (  4 ) -> grayWhiteT2\n"
                                 "- (  5 ) -> csfFraction\n"
                                 "- (  6 ) -> csfT1\n"
                                 "- (  7 ) -> csfT2\n"
                                 "- (  8 ) -> protonDensity\n"
                                 "- (  9 ) -> protonDensityScaling\n"
                                 "- ( 10 ) -> scaling of second multi "
                                 "T2-weighted volume\n"
                                 "   ...\n"
                                 "- ( 10 + N ) -> scaling of (N+1)th multi "
                                 "T2-weighted volume\n",
                                 isFixedParameters,
                                 0 );
    application.addSeriesOption( "-deltaParameters",
                                 "Variation of parameters :\n"
                                 "- (  0 ) -> myelinWaterFraction\n"
                                 "- (  1 ) -> myelinWaterT1\n"
                                 "- (  2 ) -> myelinWaterT2\n"
                                 "- (  3 ) -> grayWhiteT1\n"
                                 "- (  4 ) -> grayWhiteT2\n"
                                 "- (  5 ) -> csfFraction\n"
                                 "- (  6 ) -> csfT1\n"
                                 "- (  7 ) -> csfT2\n"
                                 "- (  8 ) -> protonDensity\n"
                                 "- (  9 ) -> protonDensityScaling\n"
                                 "- ( 10 ) -> scaling of second multi "
                                 "T2-weighted volume\n"
                                 "   ...\n"
                                 "- ( 10 + N ) -> scaling of (N+1)th multi "
                                 "T2-weighted volume\n",
                                 deltaParameters,
                                 0 );
    application.addSeriesOption( "-t2NoiseStdDevs",
                                 "List of noise standard deviation of "
                                 "T2-weighted data",
                                 t2NoiseStandardDeviations,
                                 1 );
    application.addSingleOption( "-t1NoiseStdDev",
                                 "Noise standard deviation of T1-weighted data",
                                 t1NoiseStandardDeviation );
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
             fileNameT1WeightedVolume,
             fileNameB1Mapping,
             fileNameEchoTimes,
             fileNameInversionTimes,
             fileNameSPGRFlipAngles,
             fileNameRepetitionTimes,
             fileNameMask,
             fileNameMyelinWaterFraction,
             fileNameMyelinWaterT1,
             fileNameMyelinWaterT2,
             fileNameGrayWhiteFraction,
             fileNameGrayWhiteT1,
             fileNameGrayWhiteT2,
             fileNameCSFFraction,
             fileNameCSFT1,
             fileNameCSFT2,
             fileNameProtonDensity,
             fileNameProtonDensityScaling,
             fileNameT2WeightedScaling,
             fileNameFittedData,
             optimizerParameters,
             initialParameters,
             lowerParameterBoundaries,
             upperParameterBoundaries,
             isFixedParameters,
             deltaParameters,
             t2NoiseStandardDeviations,
             t1NoiseStandardDeviation,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MyelinWaterFractionMapperCommand::parse()" );

}


void gkg::MyelinWaterFractionMapperCommand::execute(
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
                    bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    // first checking whether inversion recovery SE or SPGR with variable flip
    // angles is used to provide T1-weighted data
    bool t1MappingUsingSPGR = false;
    bool hasB1Mapping = false;
    if ( !fileNameSPGRFlipAngles.empty() )
    {

      if ( fileNameRepetitionTimes.empty() )
      {

        throw std::runtime_error( "missing repetition time text file or -tr "
                                  "option" );

      }
      if ( !fileNameInversionTimes.empty() )
      {

        throw std::runtime_error( "cannot prescribe inversion times when using"
                                  "SPGR-based T1-weighted sequence" );

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

      t1MappingUsingSPGR = true;

    }
    else
    {

      // using IR-SE sequence
      if ( fileNameInversionTimes.empty() )
      {

        throw std::runtime_error( "missing iversion time text file or -ti "
                                  "option" );

      }

      // checking that other SGPR options are discarded
      if ( !fileNameRepetitionTimes.empty() )
      {

        throw std::runtime_error( "cannot prescribe repetition times when using"
                                  "IR-SE T1-weighted sequence" );

      } 
      if ( !fileNameB1Mapping.empty() )
      {

        throw std::runtime_error( "cannot prescribe B1 mapping when using"
                                  "IR-SE T1-weighted sequence" );

      } 


    }


    ///////////////////////////////////////////////////////////////////////////
    // reading T2-weighted input data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading T2-weighted input data : " << std::flush;

    }

    int32_t t2WeightedVolumeCount = ( int32_t )fileNameT2WeightedVolumes.size();
    std::vector< gkg::Volume< float > >
      t2WeightedVolumes( t2WeightedVolumeCount );
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


    ///////////////////////////////////////////////////////////////////////////
    // reading echo time text file
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading TE echo time text file : " << std::flush;

    }

    int32_t globalT2WeightedMeasurementCount = 0;
    std::vector< int32_t > t2WeightedMeasurementCounts( t2WeightedVolumeCount );
    std::vector< gkg::Vector > echoTimes( t2WeightedVolumeCount );

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


    ///////////////////////////////////////////////////////////////////////////
    // reading T1-weighted input data
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading T1-weighted input data : " << std::flush;

    }

    gkg::Volume< float > t1WeightedVolume;
    gkg::TypedVolumeReaderProcess< float > 
      t1WeightedVolumeReaderProcess( t1WeightedVolume );
    t1WeightedVolumeReaderProcess.execute( fileNameT1WeightedVolume );
    if ( !t1WeightedVolume.hasSameSize( t2WeightedVolumes[ 0 ], true ) ||
         !t1WeightedVolume.hasSameResolution( t2WeightedVolumes[ 0 ], true ) )
    {

      throw std::runtime_error(
      "inconsistent size or resolution of T1-weighted and T2-weighted datasets" 
                               );

    }
    int32_t t1WeightedMeasurementCount = t1WeightedVolume.getSizeT();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // reading mask
    ///////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading mask : " << std::flush;

    }

    gkg::Volume< float > maskVolume;
    gkg::TypedVolumeReaderProcess< float > 
      maskVolumeReaderProcess( maskVolume );
    maskVolumeReaderProcess.execute( fileNameMask );
    if ( !maskVolume.hasSameSize( t2WeightedVolumes[ 0 ], true ) ||
         !maskVolume.hasSameResolution( t2WeightedVolumes[ 0 ], true ) )
    {

      throw std::runtime_error(
           "inconsistent size or resolution of mask and T2-weighted datasets" );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // reading relevant information for T1-weighted data
    ///////////////////////////////////////////////////////////////////////////

    gkg::Vector flipAngles;
    gkg::Vector repetitionTimes;
    gkg::RCPointer< gkg::Volume< float > > b1MappingVolumePointer;
    gkg::Vector inversionTimes;

    // in case of variable flip angle SPGR sequence
    if ( t1MappingUsingSPGR )
    {

      /////////////////////////////////////////////////////////////////////////
      // reading flip angles
      /////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading FA flip angle text file : " << std::flush;

      }

      flipAngles.reallocate( t1WeightedMeasurementCount );
      this->readTextFile( fileNameSPGRFlipAngles,
                          t1WeightedMeasurementCount,
                          flipAngles );

      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << t1WeightedMeasurementCount << " flip angles : "
                  << flipAngles << std::endl;

      }


      /////////////////////////////////////////////////////////////////////////
      // reading repetition times
      /////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading TR repetition time text file : " << std::flush;

      }

      repetitionTimes.reallocate( t1WeightedMeasurementCount );
      this->readTextFile( fileNameRepetitionTimes,
                          t1WeightedMeasurementCount,
                          repetitionTimes );

      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << t1WeightedMeasurementCount << " repetition times : "
                  << repetitionTimes << std::endl;

      }


      /////////////////////////////////////////////////////////////////////////
      // reading B1 mapping if any
      /////////////////////////////////////////////////////////////////////////

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
        if ( !b1MappingVolumePointer->hasSameSize( t1WeightedVolume, true ) ||
             !b1MappingVolumePointer->hasSameResolution( t1WeightedVolume,
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
    // in case of IR-SE sequence
    else
    {

      /////////////////////////////////////////////////////////////////////////
      // reading inversion times
      /////////////////////////////////////////////////////////////////////////

      if ( verbose )
      {

        std::cout << "reading TI inversion time text file : " << std::flush;

      }

      inversionTimes.reallocate( t1WeightedMeasurementCount );
      this->readTextFile( fileNameInversionTimes,
                          t1WeightedMeasurementCount,
                          inversionTimes );

      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << t1WeightedMeasurementCount << "inversion times : "
                  << inversionTimes << std::endl;

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
      constraints( 10 + t2WeightedVolumeCount - 1 );
    std::vector< double >
      nlpOptimizerParameters( 12 + t2WeightedVolumeCount - 1 );

    nlpOptimizerParameters[ 0 ] = optimizerParameters[ 0 ];
    nlpOptimizerParameters[ 1 ] = optimizerParameters[ 1 ];

    int32_t p = 0;
    for ( p = 0; p < 10  + t2WeightedVolumeCount - 1; p++ )
    {

      constraints[ p ].reset(  new gkg::ClosedIntervalOptimizerConstraint(
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
    // collecting size, resolution, T2-weighted and T1-weighted meas. counts
    ////////////////////////////////////////////////////////////////////////////

    gkg::Vector3d< int32_t > volumeSize;
    t2WeightedVolumes[ 0 ].getSize( volumeSize );

    gkg::Vector3d< double > volumeResolution( 1.0, 1.0, 1.0 );
    t2WeightedVolumes[ 0 ].getResolution( volumeResolution );


    ////////////////////////////////////////////////////////////////////////////
    // allocating output volumes
    ////////////////////////////////////////////////////////////////////////////

    gkg::Volume< float > myelinWaterFractionVolume( volumeSize );
    gkg::Volume< float > myelinWaterT1Volume( volumeSize );
    gkg::Volume< float > myelinWaterT2Volume( volumeSize );
    gkg::Volume< float > grayWhiteFractionVolume( volumeSize );
    gkg::Volume< float > grayWhiteT1Volume( volumeSize );
    gkg::Volume< float > grayWhiteT2Volume( volumeSize );
    gkg::Volume< float > csfFractionVolume( volumeSize );
    gkg::Volume< float > csfT1Volume( volumeSize );
    gkg::Volume< float > csfT2Volume( volumeSize );
    gkg::Volume< float > protonDensityVolume( volumeSize );
    gkg::Volume< float > protonDensityScalingVolume( volumeSize );
    gkg::Volume< float > t2WeightedScalingVolume;

    if ( t2WeightedVolumeCount > 1 )
    {

      t2WeightedScalingVolume.reallocate( volumeSize,
                                          t2WeightedVolumeCount - 1 );

    }

    myelinWaterFractionVolume.setResolution( volumeResolution );
    myelinWaterT1Volume.setResolution( volumeResolution );
    myelinWaterT2Volume.setResolution( volumeResolution );
    grayWhiteFractionVolume.setResolution( volumeResolution );
    grayWhiteT1Volume.setResolution( volumeResolution );
    grayWhiteT2Volume.setResolution( volumeResolution );
    csfFractionVolume.setResolution( volumeResolution );
    csfT1Volume.setResolution( volumeResolution );
    csfT2Volume.setResolution( volumeResolution );
    protonDensityVolume.setResolution( volumeResolution );
    protonDensityScalingVolume.setResolution( volumeResolution );

    if ( t2WeightedVolumeCount > 1 )
    {

      t2WeightedScalingVolume.setResolution( volumeResolution );

    }

    myelinWaterFractionVolume.fill( 0.0f );
    myelinWaterT1Volume.fill( 0.0f );
    myelinWaterT2Volume.fill( 0.0f );
    grayWhiteFractionVolume.fill( 0.0f );
    grayWhiteT1Volume.fill( 0.0f );
    grayWhiteT2Volume.fill( 0.0f );
    csfFractionVolume.fill( 0.0f );
    csfT1Volume.fill( 0.0f );
    csfT2Volume.fill( 0.0f );
    protonDensityVolume.fill( 0.0f );
    protonDensityScalingVolume.fill( 0.0f );

    if ( t2WeightedVolumeCount > 1 )
    {

      t2WeightedScalingVolume.fill( 1.0f );

    }

    bool computeFittedData = !fileNameFittedData.empty();
    gkg::Volume< float > fittedDataVolume;
    if ( computeFittedData )
    {

      fittedDataVolume.reallocate( volumeSize,
                                   globalT2WeightedMeasurementCount +
                                   t1WeightedMeasurementCount );
      fittedDataVolume.setResolution( volumeResolution );
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

      std::cout << "computing myelin/GW matter/CSF parameters : " << std::flush;

    }

    gkg::MyelinWaterFractionMapperGauge gauge( ( int32_t )sites.size() );

    gkg::Vector vectorOfLowerParameterBoundaries( lowerParameterBoundaries );
    gkg::Vector vectorOfUpperParameterBoundaries( upperParameterBoundaries );
    gkg::Vector vectorOfDeltaParameters( deltaParameters );

    gkg::MyelinWaterFractionMapperLoopContext
      myelinWaterFractionMapperLoopContext( gauge,
                                            sites,
                                            t2WeightedVolumes,
                                            t1WeightedVolume,
                                            t1MappingUsingSPGR,
                                            b1MappingVolumePointer,
                                            echoTimes,
                                            flipAngles,
                                            repetitionTimes,
                                            inversionTimes,
                                            t2NoiseStandardDeviations,
                                            t1NoiseStandardDeviation,
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
                                            myelinWaterFractionVolume,
                                            myelinWaterT1Volume,
                                            myelinWaterT2Volume,
                                            grayWhiteFractionVolume,
                                            grayWhiteT1Volume,
                                            grayWhiteT2Volume,
                                            csfFractionVolume,
                                            csfT1Volume,
                                            csfT2Volume,
                                            protonDensityVolume,
                                            protonDensityScalingVolume,
                                            t2WeightedScalingVolume,
                                            computeFittedData,
                                            fittedDataVolume,
                                            verbose );

    // creating a threaded loop
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &myelinWaterFractionMapperLoopContext,
                    0,                       // start index
                    ( int32_t )sites.size()
                  );

    // launching the threaded loop
    threadedLoop.launch();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving myelin water fraction map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameMyelinWaterFraction.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameMyelinWaterFraction << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameMyelinWaterFraction,
                                        myelinWaterFractionVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving myelin water T1 map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameMyelinWaterT1.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameMyelinWaterT1 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameMyelinWaterT1,
                                        myelinWaterT1Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving myelin water T2 map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameMyelinWaterT2.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameMyelinWaterT2 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameMyelinWaterT2,
                                        myelinWaterT2Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving gray white fraction map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameGrayWhiteFraction.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameGrayWhiteFraction << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameGrayWhiteFraction,
                                        grayWhiteFractionVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving gray white T1 map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameGrayWhiteT1.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameGrayWhiteT1 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameGrayWhiteT1,
                                        grayWhiteT1Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving gray white T2 map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameGrayWhiteT2.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameGrayWhiteT2 << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameGrayWhiteT2,
                                        grayWhiteT2Volume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving CSF fraction map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameCSFFraction.empty() )
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
    // saving CSF T1 map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameCSFT1.empty() )
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

    if ( !fileNameCSFT2.empty() )
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
    // saving proton density map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameProtonDensity.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameProtonDensity << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameProtonDensity,
                                        protonDensityVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving proton density scaling map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameProtonDensityScaling.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameProtonDensityScaling << "' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameProtonDensityScaling,
                                        protonDensityScalingVolume,
                                        ascii,
                                        format );

      if ( verbose )
      {

        std::cout << "done " << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving T2-weighted scaling map
    ////////////////////////////////////////////////////////////////////////////

    if ( !fileNameT2WeightedScaling.empty() &&  ( t2WeightedVolumeCount > 1 ) )
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
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperCommand::execute( "
             "const std::vector< std::string >& fileNameT2WeightedVolumes, "
             "const std::string& fileNameT1WeightedVolume, "
             "const std::string& fileNameB1Mapping, "
             "const std::vector< std::string >& fileNameEchoTimes, " 
             "const std::string& fileNameInversionTimes, "
             "const std::string& fileNameSPGRFlipAngles, "
             "const std::string& fileNameRepetitionTimes, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameMyelinWaterFraction, "
             "const std::string& fileNameMyelinWaterT1, "
             "const std::string& fileNameMyelinWaterT2, "
             "const std::string& fileNameGrayWhiteFraction, "
             "const std::string& fileNameGrayWhiteT1, "
             "const std::string& fileNameGrayWhiteT2, "
             "const std::string& fileNameCSFFraction, "
             "const std::string& fileNameCSFT1, "
             "const std::string& fileNameCSFT2, "
             "const std::string& fileNameProtonDensity, "
             "const std::string& fileNameProtonDensityScaling, "
             "const std::string& fileNameT2WeightedScaling, "
             "const std::string& fileNameFittedData, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< double >& initialParameters, "
             "const std::vector< double >& lowerParameterBoundaries, "
             "const std::vector< double >& upperParameterBoundaries, "
             "const std::vector< int32_t >& isFixedParameters, "
             "const std::vector< double >& deltaParameters, "
             "const std::vector< double >& t2NoiseStandardDeviations, "
             "double t1NoiseStandardDeviation, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


void gkg::MyelinWaterFractionMapperCommand::readTextFile(
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

    std::ifstream is( fileName.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "unable to open '" ) +
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
  GKG_CATCH( "void gkg::MyelinWaterFractionMapperCommand::readTextFile( "
             "const std::string& fileName, "
             "int32_t count, "
             "gkg::Vector& values ) const" );

}


RegisterCommandCreator(
        MyelinWaterFractionMapperCommand,
        DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameT2WeightedVolumes ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameT1WeightedVolume ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameB1Mapping ) +
        DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameEchoTimes ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameInversionTimes ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameSPGRFlipAngles ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameRepetitionTimes ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameMyelinWaterFraction ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameMyelinWaterT1 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameMyelinWaterT2 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameGrayWhiteFraction ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameGrayWhiteT1 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameGrayWhiteT2 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameCSFFraction ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameCSFT1 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameCSFT2 ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameProtonDensity ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameProtonDensityScaling ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameT2WeightedScaling ) +
        DECLARE_STRING_PARAMETER_HELP( fileNameFittedData ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( lowerParameterBoundaries ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( upperParameterBoundaries ) +
        DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( isFixedParameters ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( deltaParameters ) +
        DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP(
                                                   t2NoiseStandardDeviations ) +
        DECLARE_FLOATING_PARAMETER_HELP( t1NoiseStandardDeviation ) +
        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
        DECLARE_STRING_PARAMETER_HELP( format ) +
        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

