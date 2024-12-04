#include <gkg-processing-plugin-functors/Normalization3d/Normalization3dCommand.h>
#include <gkg-processing-plugin-functors/Flipper/FlipperCommand.h>
#include <gkg-processing-plugin-functors/Registration3d/Registration3dCommand.h>
#include <gkg-processing-plugin-functors/Transform3dComposer/Transform3dComposerCommand.h>
#include <gkg-processing-plugin-functors/Transform3dInverter/Transform3dInverterCommand.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>
#include <fstream>


class ResolutionProcess : public gkg::Process
{

  public:

    ResolutionProcess( gkg::Vector3d< double >& theResolution );

    gkg::Vector3d< double >& resolution;

  private:

    template < class T >
    static void get( gkg::Process& process,
                     const std::string& fileNameIn,
                     const gkg::AnalyzedObject&,
                     const std::string& );

};


ResolutionProcess::ResolutionProcess( gkg::Vector3d< double >& theResolution )
                  : gkg::Process( "Volume" ),
                    resolution( theResolution )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &ResolutionProcess::get< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &ResolutionProcess::get< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &ResolutionProcess::get< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &ResolutionProcess::get< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &ResolutionProcess::get< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &ResolutionProcess::get< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &ResolutionProcess::get< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &ResolutionProcess::get< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &ResolutionProcess::get< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &ResolutionProcess::get< double > );

  }
  GKG_CATCH( "ResolutionProcess::ResolutionProcess( "
             "gkg::Vector3d< double >& theResolution )" );

}


template < class T >
void ResolutionProcess::get( gkg::Process& process,
                             const std::string& fileNameIn,
                             const gkg::AnalyzedObject&,
                             const std::string& )
{

  try
  {

    ResolutionProcess&
      resolutionProcess = static_cast< ResolutionProcess& >( process );


    gkg::Vector3d< double >& resolution = resolutionProcess.resolution;

    gkg::Volume< T > volumeProxy;
    gkg::Reader::getInstance().readHeader( fileNameIn, volumeProxy );

    volumeProxy.getResolution( resolution );

  }
  GKG_CATCH( "template < class T > "
             "void ResolutionProcess::get( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   Normalization3dCommand
//

gkg::Normalization3dCommand::Normalization3dCommand( int32_t argc,
                                                     char* argv[],
                                                     bool loadPlugin,
                                                     bool removeFirst )
                            : gkg::Command( argc, argv,
                                            loadPlugin,
                                            removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::Normalization3dCommand::Normalization3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::Normalization3dCommand::Normalization3dCommand(
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
                  bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( specy,
             templateName,
             floatingName,
             flippings,
             fileNameTransform3dFloatingToTemplate,
             fileNameTransform3dFloatingToTalairachMNI,
             fileNameTransform3dFloatingToTalairachACPC,
             fileNameTransform3dTemplateToFloating,
             fileNameTransform3dTalairachMNIToFloating,
             fileNameTransform3dTalairachACPCToFloating,
             fileNameACPCCoordinates,
             similarityMeasureName,
             optimizerName,
             templateLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             initialTrmName,
             verbose );

  }
  GKG_CATCH( "gkg::Normalization3dCommand::Normalization3dCommand( "
             "const std::string& specy, "
             "const std::string& templateName, "
             "const std::string& floatingName, "
             "const std::vector< std::string >& flippings, "
             "const std::string& fileNameTransform3dFloatingToTemplate, "
             "const std::string& fileNameTransform3dFloatingToTalairachMNI, "
             "const std::string& fileNameTransform3dFloatingToTalairachACPC, "
             "const std::string& fileNameTransform3dTemplateToFloating, "
             "const std::string& fileNameTransform3dTalairachMNIToFloating, "
             "const std::string& fileNameTransform3dTalairachACPCToFloating, "
             "const std::string& fileNameACPCCoordinates, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double templateLowerThreshold, "
             "double floatingLowerThreshold, "
             "int32_t resamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< float >& initialParameters, "
             "const std::string& initialTrmName, "
             "bool verbose )" );

}


gkg::Normalization3dCommand::Normalization3dCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, specy );
    DECLARE_STRING_PARAMETER( parameters, std::string, templateName );
    DECLARE_STRING_PARAMETER( parameters, std::string, floatingName );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         flippings );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dFloatingToTemplate );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dFloatingToTalairachMNI );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dFloatingToTalairachACPC );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dTemplateToFloating );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dTalairachMNIToFloating );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTransform3dTalairachACPCToFloating );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameACPCCoordinates );
    DECLARE_STRING_PARAMETER( parameters, std::string, similarityMeasureName );
    DECLARE_STRING_PARAMETER( parameters, std::string, optimizerName );
    DECLARE_FLOATING_PARAMETER( parameters, double, templateLowerThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, double, floatingLowerThreshold );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, resamplingOrder );
    DECLARE_LIST_OF_INTEGERS_PARAMETER( parameters,
                                        std::list< int32_t >,
                                        subSamplingMaximumSizes );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           similarityMeasureParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< float >,
                                           initialParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, initialTrmName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( specy,
             templateName,
             floatingName,
             flippings,
             fileNameTransform3dFloatingToTemplate,
             fileNameTransform3dFloatingToTalairachMNI,
             fileNameTransform3dFloatingToTalairachACPC,
             fileNameTransform3dTemplateToFloating,
             fileNameTransform3dTalairachMNIToFloating,
             fileNameTransform3dTalairachACPCToFloating,
             fileNameACPCCoordinates,
             similarityMeasureName,
             optimizerName,
             templateLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             initialTrmName,
             verbose );

  }
  GKG_CATCH( "gkg::Normalization3dCommand::Normalization3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::Normalization3dCommand::~Normalization3dCommand()
{
}


std::string gkg::Normalization3dCommand::getStaticName()
{

  try
  {

    return "Normalization3d";

  }
  GKG_CATCH( "std::string gkg::Normalization3dCommand::getStaticName()" );

}


void gkg::Normalization3dCommand::parse()
{

  try
  {

    std::string specy;
    std::string templateName;
    std::string floatingName;
    std::vector< std::string > flippings;
    std::string fileNameTransform3dFloatingToTemplate;
    std::string fileNameTransform3dFloatingToTalairachMNI;
    std::string fileNameTransform3dFloatingToTalairachACPC;
    std::string fileNameTransform3dTemplateToFloating;
    std::string fileNameTransform3dTalairachMNIToFloating;
    std::string fileNameTransform3dTalairachACPCToFloating;
    std::string fileNameACPCCoordinates;
    std::string similarityMeasureName = "mutual-information";
    std::string optimizerName = "nelder-mead";
    double templateLowerThreshold = 0;
    double floatingLowerThreshold = 0;
    int32_t resamplingOrder = 1;
    std::list< int32_t > subSamplingMaximumSizes;
    std::vector< double > similarityMeasureParameters;
    std::vector< double > optimizerParameters;
    std::vector< float > initialParameters;
    std::string initialTrmName;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D normalization tool",
                                  _loadPlugin );
    application.addSingleOption( "-specy",
                                 "Specy, one of human/baboon/macaque/rat/mouse",
                                 specy );
    application.addSingleOption( "-template",
                                 "Template image\n"
                                 "for human specy, one of:\n"
                                 "- t1-MNI152-1mm\n"
                                 "- t1-MNI152-1mm-brain\n"
                                 "- t1-MNI152-2mm-smoothed\n\n"
                                 "for mouse specy, one of:\n"
                                 "- t2-WHS-40um\n"
                                 "- t2-Duke-43um\n",
                                 templateName );
    application.addSingleOption( "-floating",
                                 "Floating object",
                                 floatingName );
    application.addSeriesOption( "-flippings",
                                 "List of flippings to be applied to the "
                                 "atlas before normalization (default=empty)",
                                 flippings );
    application.addSingleOption( "-toTemplate",
                                 "3D transform from floating to template "
                                 "space",
                                 fileNameTransform3dFloatingToTemplate );
    application.addSingleOption( "-toTalairachMNI",
                                 "3D transform from floating to Talairach MNI "
                                 "space (only for human specy)",
                                 fileNameTransform3dFloatingToTalairachMNI,
                                 true );
    application.addSingleOption( "-toTalairachACPC",
                                 "3D transform from floating to Talairach ACPC "
                                 "space (only for human specy)",
                                 fileNameTransform3dFloatingToTalairachACPC,
                                 true );
    application.addSingleOption( "-fromTemplate",
                                 "3D transform from template to floating "
                                 "space",
                                 fileNameTransform3dTemplateToFloating,
                                 true );
    application.addSingleOption( "-fromTalairachMNI",
                                 "3D transform from Talairach MNI to floating "
                                 "space (only for human specy)",
                                 fileNameTransform3dTalairachMNIToFloating,
                                 true );
    application.addSingleOption( "-fromTalairachACPC",
                                 "3D transform from Talairach ACPC to floating "
                                 "space (only for human specy)",
                                 fileNameTransform3dTalairachACPCToFloating,
                                 true );
    application.addSingleOption( "-acpcCoordinates",
                                 "AC/PC/IF coordinates filename "
                                 "(only for human specy)",
                                 fileNameACPCCoordinates,
                                 true );
    application.addSingleOption( "-similarityMeasureName",
                                 "Similarity measure name: \n"
                                 "- correlation-coefficient \n"
                                 "- mutual-information \n"
                                 "- normalized-mutual-information \n"
                                 "- correlation-ratio \n"
                                 "(default=mutual-information)",
                                 similarityMeasureName,
                                 true );
    application.addSingleOption( "-optimizerName",
                                 "Optimizer name: \n"
                                 "- nelder-mead \n"
                                 "- fletcher-reeves \n"
                                 "(default=nelder-mead)",
                                 optimizerName,
                                 true );
    application.addSingleOption( "-templateLowerThreshold",
                                 "Template lower threshold (default=0.0)",
                                 templateLowerThreshold,
                                 true );
    application.addSingleOption( "-floatingLowerThreshold",
                                 "Floating lower threshold (default=0.0)",
                                 floatingLowerThreshold,
                                 true );
    application.addSingleOption( "-resamplingOrder",
                                 "Resampling order (0 to 7) (default=1)",
                                 resamplingOrder,
                                 true );
    application.addSeriesOption( "-subSamplingMaximumSizes",
                                 "Sub-sampling maximum sizes during "
                                 "optimization (default=64 128)",
                                 subSamplingMaximumSizes );
    application.addSeriesOption( "-similarityMeasureParameters",
                                 "Scalar parameters specific to a similarity "
                                 "measure as a vector of double "
                                 "<P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of mutual-information,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n\n"
                                 "- in case of normalized-mutual-information,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n\n"
                                 "- in case of correlation-ratio,\n"
                                 ".  <P1>: level count\n"
                                 ".  <P2>: apply smoothing\n"
                                 ".  (default= 32 / 1)\n",
                                 similarityMeasureParameters );
    application.addSeriesOption(
      "-optimizerParameters",
      "Scalar parameters specific to an optimizer "
      "as a vector of double "
      "<P1> <P2> <P3> ... <Pn>:\n\n"
      "-> in case of nelder-mead,\n\n"
      ".  <P1>: maximum iteration count per sub-sampling count (default=2000)\n"
      ".  <P2>: stopping criterion error (default=0.01)\n"
      ".  <P3..14>: variations of affine transform parameters\n "
      ".       <P3> - scaling along X (default=0.02),\n"
      ".       <P4> - scaling along Y (default=0.02),\n"
      ".       <P5> - scaling along Z (default=0.02),\n"
      ".       <P6> - shearing along XY (default=0.01),\n"
      ".       <P7> - shearing along XZ (default=0.01),\n"
      ".       <P8> - shearing along YZ (default=0.01),\n"
      ".       <P9> - rotation around X (in degrees, default=10),\n"
      ".       <P10> - rotation around Y (in degrees, default=10),\n"
      ".       <P11> - rotation around Z (in degrees, default=10),\n"
      ".       <P12> - translation along X (in mm, default=5),\n"
      ".       <P13> - translation along Y (in mm, default=5),\n"
      ".       <P14> - translation along Z (in mm, default=5),\n\n"
      "-> in case of fletcher-reeves,\n\n"
      ".  <P1>: maximum iteration count per sub-sampling count (default=2000)\n"
      ".  <P2>: step size (default=0.1)\n"
      ".  <P3>: maximum test gradient (default=1000)\n"
      ".  <P4>: maximum tolerance (default=0.01)\n",
      optimizerParameters );
    application.addSeriesOption(
      "-initialParameters",
      "Initial transform coefficients "
      "(-initialTrm MUST not be set if this option is used):\n"
      "- scaling along X (default=1.0),\n"
      "- scaling along Y (default=1.0),\n"
      "- scaling along Z (default=1.0),\n"
      "- shearing along XY (default=0.0),\n"
      "- shearing along XZ (default=0.0),\n"
      "- shearing along YZ (default=0.0),\n"
      "- rotation around X (in degrees, default=0 degree),\n"
      "- rotation around Y (in degrees, default=0 degree),\n"
      "- rotation around Z (in degrees, default=0 degree),\n"
      "- translation along X (in mm, default=0),\n"
      "- translation along Y (in mm, default=0),\n"
      "- translation along Z (in mm, default=0)\n",
       initialParameters,
      0, 12 );
    application.addSingleOption( "-initialTrm",
                                 "Initial transformation .trm file "
                                 "(-initialParameters MUST not be set if this "
                                 "option is used)",
                                 initialTrmName,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( specy,
             templateName,
             floatingName,
             flippings,
             fileNameTransform3dFloatingToTemplate,
             fileNameTransform3dFloatingToTalairachMNI,
             fileNameTransform3dFloatingToTalairachACPC,
             fileNameTransform3dTemplateToFloating,
             fileNameTransform3dTalairachMNIToFloating,
             fileNameTransform3dTalairachACPCToFloating,
             fileNameACPCCoordinates,
             similarityMeasureName,
             optimizerName,
             templateLowerThreshold,
             floatingLowerThreshold,
             resamplingOrder,
             subSamplingMaximumSizes,
             similarityMeasureParameters,
             optimizerParameters,
             initialParameters,
             initialTrmName,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::Normalization3dCommand::parse()" );

}


void gkg::Normalization3dCommand::execute(
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
                  bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( ( specy != "chimapnzee" ) &&
         ( specy != "human" ) &&
         ( specy != "mouse" ) &&
         ( specy != "quail" ) )
    {

      throw std::runtime_error( "invalid specy" );

    }

    if ( specy == "chimapnzee" )
    {

      if ( ( templateName != "Juna" ) )
      {

        throw std::runtime_error( "invalid template name" );

      }

    }
    else if ( specy == "human" )
    {

      if ( ( templateName != "MNI152-1mm" ) &&
           ( templateName != "MNI152-1mm-brain" ) &&
           ( templateName != "MNI152-2mm-smoothed" ) )
      {

        throw std::runtime_error( "invalid template name" );

      }

    }
    else if ( specy == "mouse" )
    {

      if ( ( templateName != "WHS-40um" ) && 
           ( templateName != "Duke-43um" ) )
      {

        throw std::runtime_error( "invalid template name" );

      }

    }
    else if ( specy == "quail" )
    {

      if ( ( templateName != "INRA" ) )
      {

        throw std::runtime_error( "invalid template name" );

      }

    }

    std::list< int32_t > theSubSamplingMaximumSizes = subSamplingMaximumSizes;
    if ( subSamplingMaximumSizes.empty() )
    {

      theSubSamplingMaximumSizes.push_back( 64 );
      theSubSamplingMaximumSizes.push_back( 128 );

    }

    if ( !initialParameters.empty() && ( initialParameters.size() != 12U ) )
    {

      throw std::runtime_error(
                           "option '-initialParameters': must have 12 values" );

    }

    if ( ( specy != "human" ) &&
         ( ( !fileNameTransform3dFloatingToTalairachMNI.empty() ) ||
           ( !fileNameTransform3dFloatingToTalairachACPC.empty()  ) ||
           ( !fileNameTransform3dTalairachMNIToFloating.empty() ) ||
           ( !fileNameTransform3dTalairachACPCToFloating.empty() ) ||
           ( !fileNameACPCCoordinates.empty() ) ) )
    {

      throw std::runtime_error(
                     "options linked to MNI and Talairach space only available "
                     "for human specy" );

    }         


    ////////////////////////////////////////////////////////////////////////////
    // setting vectors of initial parameters and delta parameters
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double > theOptimizerParameters = optimizerParameters;

    // managing variation(s) of parameter(s)
    if ( optimizerName == "nelder-mead" )
    {

      if ( theOptimizerParameters.empty() )
      {

        theOptimizerParameters.resize( 2U + 12U );
        theOptimizerParameters[ 0 ] = 2000;
        theOptimizerParameters[ 1 ] = 0.01;

        theOptimizerParameters[ 2 + 0 ] = 0.02;
        theOptimizerParameters[ 2 + 1 ] = 0.02;
        theOptimizerParameters[ 2 + 2 ] = 0.02;
        theOptimizerParameters[ 2 + 3 ] = 0.01;
        theOptimizerParameters[ 2 + 4 ] = 0.01;
        theOptimizerParameters[ 2 + 5 ] = 0.01;
        theOptimizerParameters[ 2 + 6 ] = M_PI / 18;
        theOptimizerParameters[ 2 + 7 ] = M_PI / 18;
        theOptimizerParameters[ 2 + 8 ] = M_PI / 18;
        theOptimizerParameters[ 2 + 9 ] = 5.0;
        theOptimizerParameters[ 2 + 10 ] = 5.0;
        theOptimizerParameters[ 2 + 11 ] = 5.0;

      }
      else
      {

        if ( theOptimizerParameters.size() != 14U )
        {

          throw std::runtime_error(
                            "nelder-mead optimizer must have 14 parameters" );

        }

        theOptimizerParameters[ 2 + 6 ] *= M_PI / 180.0;
        theOptimizerParameters[ 2 + 7 ] *= M_PI / 180.0;
        theOptimizerParameters[ 2 + 8 ] *= M_PI / 180.0;

      }

    }
    else if ( optimizerName == "fletcher-reeves" )
    {

      if ( theOptimizerParameters.empty() )
      {

        theOptimizerParameters.resize( 4U );
        theOptimizerParameters[ 0 ] = 2000;
        theOptimizerParameters[ 1 ] = 0.1;
        theOptimizerParameters[ 2 ] = 1000;
        theOptimizerParameters[ 3 ] = 0.01;

      }
      else
      {

        if ( theOptimizerParameters.size() != 4U )
        {

          throw std::runtime_error(
                         "fletcher-reeves optimizer must have 4 parameters" );

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching 3D registration command from floating to template
    ////////////////////////////////////////////////////////////////////////////

    std::string templateFileName = 
      gkg::ConfigurationInfo::getInstance().getAnatomicalTemplatesPath( 
                                                     specy,
                                                     templateName );

    gkg::File
     fileTransform3dFloatingToTemplate( fileNameTransform3dFloatingToTemplate );
    std::string outputDirectoryName =
                           fileTransform3dFloatingToTemplate.getDirectoryName();

    std::string flippedTemplateFileName = templateFileName +
                                          gkg::getDirectorySeparator() +
                                          "template" ;
    if ( !flippings.empty() )
    {

      flippedTemplateFileName = outputDirectoryName +
                                "template" ;
      gkg::FlipperCommand flipperCommand( templateFileName,
                                          flippedTemplateFileName,
                                          flippings,
                                          false,
                                          "gis",
                                          verbose );

      if ( specy == "mouse" )
      {


        std::string labelFileName = 
          gkg::ConfigurationInfo::getInstance().getAnatomicalTemplatesPath( 
                                                   specy,
                                                   templateName ) +
          gkg::getDirectorySeparator() +
          "label" ;
        std::string flippedLabelFileName = outputDirectoryName +
                                           "label";
        gkg::FlipperCommand flipperCommand( labelFileName,
                                            flippedLabelFileName,
                                            flippings,
                                            false,
                                            "gis",
                                            verbose );

      }

    }

    gkg::Registration3dCommand
      registration3dCommand( flippedTemplateFileName,
                             floatingName,
                             fileNameTransform3dFloatingToTemplate,
                             fileNameTransform3dTemplateToFloating,
                             similarityMeasureName,
                             optimizerName,
                             templateLowerThreshold,
                             floatingLowerThreshold,
                             resamplingOrder,
                             theSubSamplingMaximumSizes,
                             similarityMeasureParameters,
                             theOptimizerParameters,
                             "affine",
                             initialParameters,
                             initialTrmName,
                             "",
                             true,
                             false,
                             verbose );

    ////////////////////////////////////////////////////////////////////////////
    // launching the composition to go to the Talairach-MNI space
    ////////////////////////////////////////////////////////////////////////////

    if ( ( specy == "human" ) &&
         !fileNameTransform3dFloatingToTalairachMNI.empty() )
    {

      std::string fileNameTransform3dMniSpmToTalairachMni = 
          gkg::ConfigurationInfo::getInstance().getAnatomicalTemplatesPath( 
                                                   specy,
                                                   templateName ) +
                                               "MNI_SPM_to_TALAIRACH_MNI.trm";

      std::vector< std::string > fileNameTransform3ds( 2 );
      fileNameTransform3ds[ 0 ] = fileNameTransform3dFloatingToTemplate;
      fileNameTransform3ds[ 1 ] = fileNameTransform3dMniSpmToTalairachMni;

    
      gkg::Transform3dComposerCommand
        transform3dComposerCommand1( fileNameTransform3ds,
                                     fileNameTransform3dFloatingToTalairachMNI,
                                     verbose );

      if ( !fileNameTransform3dTalairachMNIToFloating.empty() )
      {

        gkg::Transform3dInverterCommand
          transform3dInverterCommand( fileNameTransform3dFloatingToTalairachMNI,
                                      fileNameTransform3dTalairachMNIToFloating,
                                      verbose );

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching the composition to go to the Talairach-ACPC space
    ////////////////////////////////////////////////////////////////////////////

    if ( ( specy == "human" ) &&
         !fileNameTransform3dFloatingToTalairachACPC.empty() )
    {

      if ( fileNameTransform3dFloatingToTalairachMNI.empty() )
      {

        throw std::runtime_error(
          "'-toTalairachMNI' must be filled to compute transformation from "
          "floating to AC/PC Talairach frame" );

      }

      std::string fileNameTransform3dTalairachMniToTalairachAcpc = 
          gkg::ConfigurationInfo::getInstance().getAnatomicalTemplatesPath( 
                                                   specy,
                                                   templateName ) +
                                          "TALAIRACH_MNI_to_TALAIRACH_ACPC.trm";

      std::vector< std::string > fileNameTransform3ds( 2 );
      fileNameTransform3ds[ 0 ] = fileNameTransform3dFloatingToTalairachMNI;
      fileNameTransform3ds[ 1 ] = 
                                 fileNameTransform3dTalairachMniToTalairachAcpc;

    
      gkg::Transform3dComposerCommand
        transform3dComposerCommand2( fileNameTransform3ds,
                                     fileNameTransform3dFloatingToTalairachACPC,
                                     verbose );

      if ( !fileNameTransform3dTalairachACPCToFloating.empty() )
      {

        gkg::Transform3dInverterCommand
          transform3dInverterCommand(
                                     fileNameTransform3dFloatingToTalairachACPC,
                                     fileNameTransform3dTalairachACPCToFloating,
                                     verbose );

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing AC/PC/IH coordinates and storing them to disk
    ////////////////////////////////////////////////////////////////////////////
    if (  ( specy == "human" ) &&
           !fileNameACPCCoordinates.empty() )
    {


      if ( fileNameTransform3dFloatingToTalairachACPC.empty() )
      {

        throw std::runtime_error(
          "'-toTalairachACPC' must be filled to compute AC/PC/IH "
          "coordinate file" );

      }

      if ( verbose )
      {

        std::cout << "computing AC/PC/IH coordinates: " << std::flush; 

      }

      gkg::AffineTransform3d< float > transform3dFloatingToTalairachACPC;
      std::ifstream is( fileNameTransform3dFloatingToTalairachACPC.c_str() );

      transform3dFloatingToTalairachACPC.readTrm( is );

      is.close();

      gkg::Vector3d< float > ACmmInTalairachACPC( 0.0, 0.0, 0.0 );
      gkg::Vector3d< float > PCmmInTalairachACPC( 0.0, 30.0, 0.0 );
      gkg::Vector3d< float > IHmmInTalairachACPC( 0.0, 40.0, -60.0 );

      gkg::Vector3d< float > ACmmInFloating;
      gkg::Vector3d< float > PCmmInFloating;
      gkg::Vector3d< float > IHmmInFloating;

      transform3dFloatingToTalairachACPC.getInverse( ACmmInTalairachACPC,
                                                     ACmmInFloating );
      transform3dFloatingToTalairachACPC.getInverse( PCmmInTalairachACPC,
                                                     PCmmInFloating );
      transform3dFloatingToTalairachACPC.getInverse( IHmmInTalairachACPC,
                                                     IHmmInFloating );


      gkg::Vector3d< double > floatingResolution( 1.0, 1.0, 1.0 );
      ResolutionProcess resolutionProcess( floatingResolution );
      resolutionProcess.execute( floatingName );

      gkg::Vector3d< int32_t >
        ACInFloating( gkg::round( ACmmInFloating.x /
                                  ( float )floatingResolution.x ),
                      gkg::round( ACmmInFloating.y /
                                  ( float )floatingResolution.y ),
                      gkg::round( ACmmInFloating.z /
                                  ( float )floatingResolution.z ) );
      gkg::Vector3d< int32_t >
        PCInFloating( gkg::round( PCmmInFloating.x /
                                  ( float )floatingResolution.x ),
                      gkg::round( PCmmInFloating.y /
                                  ( float )floatingResolution.y ),
                      gkg::round( PCmmInFloating.z /
                                  ( float )floatingResolution.z ) );
      gkg::Vector3d< int32_t >
        IHInFloating( gkg::round( IHmmInFloating.x /
                                  ( float )floatingResolution.x ),
                      gkg::round( IHmmInFloating.y /
                                  ( float )floatingResolution.y ),
                      gkg::round( IHmmInFloating.z /
                                  ( float )floatingResolution.z ) );

      std::ofstream os( fileNameACPCCoordinates.c_str() );

      os << "AC: " << ACInFloating.x << " "
                   << ACInFloating.y << " "
                   << ACInFloating.z << std::endl;
      os << "PC: " << PCInFloating.x << " "
                   << PCInFloating.y << " "
                   << PCInFloating.z << std::endl;
      os << "IH: " << IHInFloating.x << " "
                   << IHInFloating.y << " "
                   << IHInFloating.z << std::endl;
      os << "The previous coordinates, used by the system, are defined in"
         << " voxels" 
         << std::endl;
      os << "They stem from the following coordinates in millimeters:"
         << std::endl;
      os << "ACmm: " << ACmmInFloating.x << " "
                     << ACmmInFloating.y << " "
                     << ACmmInFloating.z << std::endl;
      os << "PCmm: " << PCmmInFloating.x << " "
                     << PCmmInFloating.y << " "
                     << PCmmInFloating.z << std::endl;
      os << "IHmm: " << IHmmInFloating.x << " "
                     << IHmmInFloating.y << " "
                     << IHmmInFloating.z << std::endl;

      os.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

        std::cout << "AC in voxels: " << ACInFloating << std::endl;
        std::cout << "PC in voxels: " << PCInFloating << std::endl;
        std::cout << "IH in voxels: " << IHInFloating << std::endl;
        std::cout << "AC in mm: " << ACmmInFloating << std::endl;
        std::cout << "PC in mm: " << PCmmInFloating << std::endl;
        std::cout << "IH in mm: " << IHmmInFloating << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::Normalization3dCommand::execute( "
             "const std::string& specy, "
             "const std::string& templateName, "
             "const std::string& floatingName, "
             "const std::vector< std::string >& flippings, "
             "const std::string& fileNameTransform3dFloatingToTemplate, "
             "const std::string& fileNameTransform3dFloatingToTalairachMNI, "
             "const std::string& fileNameTransform3dFloatingToTalairachACPC, "
             "const std::string& fileNameTransform3dTemplateToFloating, "
             "const std::string& fileNameTransform3dTalairachMNIToFloating, "
             "const std::string& fileNameTransform3dTalairachACPCToFloating, "
             "const std::string& similarityMeasureName, "
             "const std::string& optimizerName, "
             "double referenceLowerThreshold, "
             "double floatingLowerThreshold, "
             "int32_t resamplingOrder, "
             "const std::list< int32_t >& subSamplingMaximumSizes, "
             "const std::vector< double >& similarityMeasureParameters, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< float >& initialParameters, "
             "bool verbose )" );

}


RegisterCommandCreator(
   Normalization3dCommand,
   DECLARE_STRING_PARAMETER_HELP( specy ) +
   DECLARE_STRING_PARAMETER_HELP( templateName ) +
   DECLARE_STRING_PARAMETER_HELP( floatingName ) +
   DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( flippings ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dFloatingToTemplate ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dFloatingToTalairachMNI ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dFloatingToTalairachACPC ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dTemplateToFloating ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dTalairachMNIToFloating ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameTransform3dTalairachACPCToFloating ) +
   DECLARE_STRING_PARAMETER_HELP( fileNameACPCCoordinates ) +
   DECLARE_STRING_PARAMETER_HELP( similarityMeasureName ) +
   DECLARE_STRING_PARAMETER_HELP( optimizerName ) +
   DECLARE_FLOATING_PARAMETER_HELP( templateLowerThreshold ) +
   DECLARE_FLOATING_PARAMETER_HELP( floatingLowerThreshold ) +
   DECLARE_INTEGER_PARAMETER_HELP( resamplingOrder );
   DECLARE_LIST_OF_INTEGERS_PARAMETER_HELP( subSamplingMaximumSizes ) +
   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( similarityMeasureParameters ) +
   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
   DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( initialParameters ) +
   DECLARE_STRING_PARAMETER_HELP( initialTrmName ) +
   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
