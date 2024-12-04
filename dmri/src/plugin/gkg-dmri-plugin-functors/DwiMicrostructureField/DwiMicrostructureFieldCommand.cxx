#include <gkg-dmri-plugin-functors/DwiMicrostructureField/DwiMicrostructureFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-dmri-container/MicrostructureCartesianFieldFactory.h>
#include <gkg-dmri-io/OdfFunctorWriter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiMicrostructureFieldCommand::DwiMicrostructureFieldCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                   : gkg::Command( argc, argv, loadPlugin,
                                                   removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiMicrostructureFieldCommand::"
             "DwiMicrostructureFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiMicrostructureFieldCommand::DwiMicrostructureFieldCommand(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& odfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             odfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMicrostructureFieldCommand::"
             "DwiMicrostructureFieldCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiMicrostructureFieldCommand::DwiMicrostructureFieldCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, modelType );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       odfFunctorNames );
    DECLARE_LIST_OF_STRINGS_PARAMETER( parameters, std::list< std::string >,
                                       outputFileNames );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          iBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           fBox );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< float >,
                                           superResolution );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_FLOATING_PARAMETER( parameters, float, rgbScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, meshScale );
    DECLARE_FLOATING_PARAMETER( parameters, float, lowerGFAThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, float, upperGFAThreshold );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           specificScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         specificStringParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           furtherScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         furtherStringParameters );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             odfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMicrostructureFieldCommand::"
             "DwiMicrostructureFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiMicrostructureFieldCommand::~DwiMicrostructureFieldCommand()
{
}


std::string gkg::DwiMicrostructureFieldCommand::getStaticName()
{

  try
  {

    return "DwiMicrostructureField";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiMicrostructureFieldCommand::getStaticName()" );

}


void gkg::DwiMicrostructureFieldCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string modelType;
    std::list< std::string > odfFunctorNames;
    std::list< std::string > outputFileNames;
    std::vector< int32_t > iBox;
    std::vector< float > fBox;
    std::vector< float > superResolution;
    int32_t outputOrientationCount = 0;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    std::string volumeFormat = "gis";
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    float rgbScale = 1.0;
    float meshScale = 1.0;
    float lowerGFAThreshold = 0.0;
    float upperGFAThreshold = 1.0;
    std::vector< double > specificScalarParameters;
    std::vector< std::string > specificStringParameters;
    std::vector< double > furtherScalarParameters;
    std::vector< std::string > furtherStringParameters;
    bool ascii = false;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // building ODF model list
    ////////////////////////////////////////////////////////////////////////////
    std::string modelText = "Type of the microstructural model :\n";
    std::list< std::string >
      allModelNames = gkg::MicrostructureCartesianFieldFactory::getInstance().
                                                                     getNames();
    std::list< std::string >::const_iterator
      m = allModelNames.begin(),
      me = allModelNames.end();
    while ( m != me )
    {

      modelText += "- " + *m + "\n";
      ++ m;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building ODF functor list
    ////////////////////////////////////////////////////////////////////////////
    std::string odfFunctorText = "Microstructure functor list among :\n";
    std::list< std::string >
      allOdfFunctorNames = gkg::OdfFunctorWriter::getNames();
    std::list< std::string >::const_iterator
      f = allOdfFunctorNames.begin(),
      fe = allOdfFunctorNames.end();
    while ( f != fe )
    {

      odfFunctorText += "- " + *f + "\n";
      ++ f;

    }

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "DWI Microstructure field model",
                                  _loadPlugin );
    application.addSingleOption( "-t2",
                                 "Input T2 file name",
                                 fileNameT2 );
    application.addSingleOption( "-dw",
                                 "Input DW file name",
                                 fileNameDW );
    application.addSingleOption( "-m",
                                 "Input mask file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-type",
                                 modelText,
                                 modelType );
    application.addSeriesOption( "-f",
                                 odfFunctorText,
                                 odfFunctorNames,
                                 1 );
    application.addSeriesOption( "-o",
                                 "Output file name(s)",
                                 outputFileNames,
                                 1 );
    application.addSeriesOption( "-ibox",
                                 "Integer bounding box on which to compute"
                                 " the ODF field",
                                 iBox );
    application.addSeriesOption( "-fbox",
                                 "Float bounding box on which to compute"
                                 " the ODF field",
                                 fBox );
    application.addSeriesOption( "-superResolution",
                                 "Super resolution of the ODF field"
                                 " (default=input resolution)",
                                 superResolution );
    application.addSingleOption( "-outputOrientationCount",
                                 "Output orientation count "
                                 "(default=symmetrical input orientation set)",
                                 outputOrientationCount,
                                 true );

    application.addSingleOption( "-radius",
                                 "Radius of morphological operation for mask "
                                 "(default=2*min. res.)",
                                 radius,
                                 true );
    application.addSingleOption( "-thresholdRatio",
                                 "Threshold ratio for mask (default=0.04)",
                                 thresholdRatio,
                                 true );

    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );

    application.addSingleOption( "-meshMapFormat",
                                 "Ouput MeshMap format (default=aimsmesh)",
                                 meshMapFormat,
                                 true );
    application.addSingleOption( "-textureMapFormat",
                                 "Ouput MeshMap format (default=aimstexture)",
                                 textureMapFormat,
                                 true );

    application.addSingleOption( "-rgbScale",
                                 "Scale factor for RGB map(s) (default=1.0)",
                                 rgbScale,
                                 true );
    application.addSingleOption( "-meshScale",
                                 "Scale factor for mesh (default=1.0)",
                                 meshScale,
                                 true );
    application.addSingleOption( "-lowerGFAThreshold",
                                 "Lower GFA threshold (default=0.0)",
                                 lowerGFAThreshold,
                                 true );
    application.addSingleOption( "-upperGFAThreshold",
                                 "Upper GFA threshold (default=1.0)",
                                 upperGFAThreshold,
                                 true );

    application.addSeriesOption( 
                    "-scalarParameters",
                    "Microstructure cartesian field specific parameters as "
                    "a vector of double <P1> <P2> ... <Pn>:"
                    "\n\n"
                    "- in case of ivim_odf_cartesian_field,\n\n"
                    ".  <P1>: IVIM to parenchyma transition b-value in s/mm^2\n"
                    ".  <P2>: parenchyma Gaussian to non Gaussian transition "
                    "b-value in s/mm^2\n"
                    ".  <P3>: initial fVIM volume fraction between 0.0 and "
                    "1.0\n"
                    ".  <P4>: initial IVIM diffusivity D* in m^2/s (typically "
                    "1e-8 m^2/s)\n"
                    ".  <P5>: lower bound of IVIM volume fraction\n"
                    ".  <P6>: lower bound of IVIM diffusivity D*\n"
                    ".  <P7>: upper bound of IVIM  volume fraction\n"
                    ".  <P8>: upper bound of IVIM diffusivity D*\n"
                    ".  <P9>: variation of IVIM  volume fraction\n"
                    ".  <P10>: variation of IVIM diffusivity D*\n"
                    ".  <P11>: 1 for fixed IVIM volume fraction, 0 instead\n"
                    ".  <P12>: 1 for fixed IVIM diffusivity D*, 0 instead\n"
                    ".  <P13>: noise standard deviation of normalized signal\n"
                    ".  <P14>: using quick search optimizer\n"
                    ".  <P15>: quick search step count per parameter\n"
                    ".  <P16>: NLP maximum iteration count\n"
                    ".  <P17>: NLP maximum step size\n"
                    ".  (default: 250.0 / 1000.0 / 0.0 / 1e-8 / 0.0 / 0.0 /\n"
                    ".            1.0 / 3e-8 / 0.1 / 0.1e-8 / 0 / 0 / \n"
                    ".            0.2 / 1 / 5 / 1000 / 0.001 )\n\n"
                    "- in case of noddi_odf_cartesian_field,\n\n"
                    ".  -> with Watson distribution\n"
                    ".  <P1>: initial isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P2>: initial intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P3>: initial kappa (>=0)\n"
                    ".  <P4>: initial parallel diffusivity in m^2/s\n"
                    ".  <P5>: initial isotropic diffusivity in m^2/s\n"
                    ".  <P6>: initial stationary fraction between 0.0 and 1.0\n"
                    ".  <P7>: lower bound of isotropic fraction\n"
                    ".  <P8>: lower bound of intracellular fraction\n"
                    ".  <P9>: lower bound of kappa\n"
                    ".  <P10>: lower bound of parallel diffusivity "
                    "in m^2/s\n"
                    ".  <P11>: lower bound of isotropic diffusivity "
                    "in m^2/s\n"
                    ".  <P12>: lower bound of stationary fraction\n"
                    ".  <P13>: upper bound of isotropic fraction\n"
                    ".  <P14>: upper bound of intracellular fraction\n"
                    ".  <P15>: upper bound of kappa\n"
                    ".  <P16>: upper bound of parallel diffusivity "
                    "in m^2/s\n"
                    ".  <P17>: upper bound of isotropic diffusivity "
                    "in m^2/s\n"
                    ".  <P18>: upper bound of stationary fraction\n"
                    ".  <P19>: variation of isotropic fraction\n"
                    ".  <P20>: variation of intracellular fraction\n"
                    ".  <P21>: variation of kappa\n"
                    ".  <P22>: variation of parallel diffusivity "
                    "in m^2/s\n"
                    ".  <P23>: variation of isotropic diffusivity "
                    "in m^2/s\n"
                    ".  <P24>: variation of stationary fraction\n"
                    ".  <P25>: 1 for fixed isotropic fraction, 0 "
                    "instead\n"
                    ".  <P26>: 1 for fixed intracellular fraction, 0 "
                    "instead\n"
                    ".  <P27>: 1 for fixed kappa, 0 "
                    "instead\n"
                    ".  <P28>: 1 for fixed principal orientation, 0 "
                    "instead\n"
                    ".  <P29>: 1 for fixed parallel diffusivity, 0 "
                    "instead\n"
                    ".  <P30>: 1 for fixed isotropic diffusivity, 0 "
                    "instead\n"
                    ".  <P31>: 1 for fixed stationary fraction, 0 "
                    "instead\n"
                    ".  <P32>: noise standard deviation\n"
                    ".  <P33>: using quick search optimizer\n"
                    ".  <P34>: quick search step count per parameter\n"
                    ".  <P35>: using constrained NLP optimizer\n"
                    ".  <P36>: NLP maximum iteration count\n"
                    ".  <P37>: NLP maximum step size\n"
                    ".  <P38> using MCMC optimizer\n"
                    ".  <P39>: MCMC burnin count\n"
                    ".  <P40>: MCMC sample count\n"
                    ".  <P41>: MCMC interval count\n"
                    ".  <P42>: MCMC maximum iteration count\n"
                    ".  (default: \n"
                    ".        0.0 / 0.5 /   0.0   / 1.7e-9 / 3.0e-9 / 0.0 /\n"
                    ".        0.0 / 0.0 /   0.0   / 0.0    / 0.0    /  0.0 /\n"
                    ".        1.0 / 1.0 / 100.0   / 3.0e-9 / 6.0e-9 / 1.0 /\n"
                    ".        0.1 / 0.1 /   1.0   / 0.2e-9 / 0.2e-9 / 0.1/\n"
                    ".        0   / 0   /   0 / 0 / 1      / 1      / 0 /\n"
                    ".        50  / \n"
                    ".        1   / 10 / \n"
                    ".        1   / 1000 / 0.001 / \n"
                    ".        1 / 500 / 100 / 50 / 10000\n\n"
                    ".  -> with Bingham distribution\n"
                    ".  <P1>: initial isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P2>: initial intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P3>: initial kappa1 (>=0)\n"
                    ".  <P4>: initial kappa2 (>=0)\n"
                    ".  <P5>: initial parallel diffusivity in m^2/s\n"
                    ".  <P6>: initial isotropic diffusivity in m^2/s\n"
                    ".  <P7>: initial stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P8>: lower bound of isotropic fraction\n"
                    ".  <P9>: lower bound of intracellular fraction\n"
                    ".  <P10>: lower bound of kappa1\n"
                    ".  <P11>: lower bound of kappa2\n"
                    ".  <P12>: lower bound of parallel diffusivity "
                    "in m^2s\n"
                    ".  <P13>: lower bound of isotropic diffusivity "
                    "in m^2s\n"
                    ".  <P14>: lower bound of stationary fraction\n"
                    ".  <P15>: upper bound of isotropic fraction\n"
                    ".  <P16>: upper bound of intracellular fraction\n"
                    ".  <P17>: upper bound of kappa1\n"
                    ".  <P18>: upper bound of kappa2\n"
                    ".  <P19>: upper bound of parallel diffusivity "
                    "in m^2/s\n"
                    ".  <P20>: upper bound of isotropic diffusivity "
                    "in m^2/s\n"
                    ".  <P21>: upper bound of stationary fraction\n"
                    ".  <P22>: variation of isotropic fraction\n"
                    ".  <P23>: variation of intracellular fraction\n"
                    ".  <P24>: variation of kappa1\n"
                    ".  <P25>: variation of kappa2\n"
                    ".  <P26>: variation of parallel diffusivity "
                    "in m^2s\n"
                    ".  <P27>: variation of isotropic diffusivity "
                    "in m^2/s\n"
                    ".  <P28>: variation of stationary fraction\n"
                    ".  <P29>: 1 for fixed isotropic fraction, 0 "
                    "instead\n"
                    ".  <P30>: 1 for fixed intracellular fraction, 0 "
                    "instead\n"
                    ".  <P31>: 1 for fixed kappa1, 0 "
                    "instead\n"
                    ".  <P32>: 1 for fixed kappa2, 0 "
                    "instead\n"
                    ".  <P33>: 1 for fixed principal orientation, 0 "
                    "instead\n"
                    ".  <P34>: 1 for fixed secondary orientation, 0 "
                    "instead\n"
                    ".  <P35>: 1 for fixed parallel diffusivity, 0 "
                    "instead\n"
                    ".  <P36>: 1 for fixed isotropic diffusivity, 0 "
                    "instead\n"
                    ".  <P37>: 1 for fixed stationary fraction, 0 "
                    "instead\n"
                    ".  <P38>: noise standard deviation\n"
                    ".  <P39>: using quick search optimizer\n"
                    ".  <P40>: quick search step count per parameter\n"
                    ".  <P41>: using constrained NLP optimizer\n"
                    ".  <P42>: NLP maximum iteration count\n"
                    ".  <P43>: NLP maximum step size\n"
                    ".  <P44>: using MCMC optimizer\n"
                    ".  <P45>: MCMC burnin count\n"
                    ".  <P46>: MCMC sample count\n"
                    ".  <P47>: MCMC interval count\n"
                    ".  <P48>: MCMC maximum iteration count\n"
                    ".  (default: \n"
                    ".   0.0 / 0.5 /   0.0 /   0.0 / 1.7e-9 / 3.0e-9 / 0.0 /\n"
                    ".   0.0 / 0.0 /   0.0 /   0.0 / 0.0    / 0.0    / 0.0 /\n"
                    ".   1.0 / 1.0 / 100.0 / 100.0 / 3.0e-9 / 6.0e-9 / 1.0 /\n"
                    ".   0.1 / 0.1 /   1.0 /   1.0 / 0.2e-9 / 0.2e-9 / 0.1 /\n"
                    ".   0 / 0 / 0 / 0 / 0 / 0 / 1 / 1 / 0 /\n"
                    ".   50 / \n"
                    ".   1 / 10 / \n"
                    ".   1 / 1000 / 0.001 / \n"
                    ".   1 / 500 / 100 / 50 / 10000\n\n"

                    "- in case of axon_mapping_odf_cartesian_field,\n"
                    ".  -> with PGSE model\n"
                    ".  <P1>: initial isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P2>: initial isotropic diffusivity in m^2/s\n"
                    ".  <P3>: initial extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P4>: initial stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P5>: initial intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P6>: initial kappa (>=0)\n"
                    ".  <P7>: initial parallel diffusivity in m^2/s\n"
                    ".  <P8>: initial axon diameter in m\n"
                    ".  <P9>: lower isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P10>: lower isotropic diffusivity in m^2/s\n"
                    ".  <P11>: lower extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P12>: lower stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P13>: lower intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P14>: lower kappa (>=0)\n"
                    ".  <P15>: lower parallel diffusivity in m^2/s\n"
                    ".  <P16>: lower axon diameter in m\n"   
                    ".  <P17>: upper isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P18>: upper isotropic diffusivity in m^2/s\n"
                    ".  <P19>: upper extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P20>: upper stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P21>: upper intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P22>: upper kappa \n"
                    ".  <P23>: upper parallel diffusivity in m^2/s\n"
                    ".  <P24>: upper axon diameter in m\n"                 
                    ".  <P25>: variation isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P26>: variation isotropic diffusivity in m^2/s\n"
                    ".  <P27>: variation extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P28>: variation stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P29>: variation intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P30>: variation kappa (>=0)\n"
                    ".  <P31>: variation parallel diffusivity in m^2/s\n"
                    ".  <P32>: variation axon diameter in m\n"
                    ".  <P33>: 1 for fixed isotropic fraction, 0 instead\n"
                    ".  <P34>: 1 for fixed isotropic diffusivity, 0 instead\n"
                    ".  <P35>: 1 for fixed extracellular perpendicular"
                    " diffusivity, 0 instead\n"
                    ".  <P36>: 1 for fixed stationary fraction, 0 instead\n"
                    ".  <P37>: 1 for fixed intracellular fraction, 0 instead\n"
                    ".  <P38>: 1 for fixed kappa, 0 instead\n"
                    ".  <P39>: 1 for fixed parallel diffusivity, 0 instead\n"
                    ".  <P40>: 1 for fixed axon diameter, 0 instead\n"
                    ".  <P41>: 1 for fixed principal orientation, 0 instead\n"
                    ".  <P42>: noise standard deviation\n"
                    ".  <P43>: using quick search optimizer\n"
                    ".  <P44>: quick search step count per parameter\n"
                    ".  <P45>: using constrained NLP optimizer\n"
                    ".  <P46>: NLP maximum iteration count\n"
                    ".  <P47>: NLP maximum step size\n"
                    ".  <P48>: using MCMC optimizer\n"
                    ".  <P49>: MCMC burnin count\n"
                    ".  <P50>: MCMC sample count\n"
                    ".  <P51>: MCMC interval count\n"
                    ".  <P52>: MCMC maximum iteration count\n"
                    ".  (default: \n"
                    ".  0.0/ 3.0e-9/ 0.0e-9/ 0.0/ 0.5/ 0.0/ 1.7e-9 / 2.0e-6\n"
                    ".  0.0/ 0.0e-9/ 0.0e-9/ 0.0/ 0.0/ 0.0/ 0.0e-9 / 0.0e-6\n"
                    ".  1.0/ 6.0e-9/ 3.0e-9/ 1.0/ 1.0/ 200/ 3.0e-9/ 10.0e-6\n"
                    ".  0.1/ 0.5e-9/ 0.5e-9/ 0.1/ 0.1/ 2.0/ 0.5e-9/  1.0e-6\n"
                    ".  0.0/ 0.0   / 0.0   / 0.0/ 0.0/ 0.0/ 0.0   /  0.0/ 1.0\n"
                    ".  0.01\n"
                    ".  1.0, 10.0\n"
                    ".  1.0, 1000.0, 0.01\n"
                    ".  1.0, 5000.0, 500.0, 100.0, 100000.0"
                    "\n\n"

                    ".  -> with cosine-trapezoid-ogse model\n"
                    ".  <P1>: initial isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P2>: initial isotropic diffusivity in m^2/s\n"
                    ".  <P3>: initial extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P4>: initial stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P5>: initial intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P6>: initial kappa (>=0)\n"
                    ".  <P7>: initial parallel diffusivity in m^2/s\n"
                    ".  <P8>: initial axon diameter in m\n"
                    ".  <P9>: lower isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P10>: lower isotropic diffusivity in m^2/s\n"
                    ".  <P11>: lower extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P12>: lower stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P13>: lower intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P14>: lower kappa (>=0)\n"
                    ".  <P15>: lower parallel diffusivity in m^2/s\n"
                    ".  <P16>: lower axon diameter in m\n"   
                    ".  <P17>: upper isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P18>: upper isotropic diffusivity in m^2/s\n"
                    ".  <P19>: upper extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P20>: upper stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P21>: upper intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P22>: upper kappa \n"
                    ".  <P23>: upper parallel diffusivity in m^2/s\n"
                    ".  <P24>: upper axon diameter in m\n"                 
                    ".  <P25>: variation isotropic fraction between "
                    "0.0 and 1.0\n"
                    ".  <P26>: variation isotropic diffusivity in m^2/s\n"
                    ".  <P27>: variation extracellular perpendicular diffusivity "
                    "in m^2/s\n"
                    ".  <P28>: variation stationary fraction between "
                    "0.0 and 1.0\n"
                    ".  <P29>: variation intracellular fraction between "
                    "0.0 and 1.0\n"
                    ".  <P30>: variation kappa (>=0)\n"
                    ".  <P31>: variation parallel diffusivity in m^2/s\n"
                    ".  <P32>: variation axon diameter in m\n"
                    ".  <P33>: 1 for fixed isotropic fraction, 0 instead\n"
                    ".  <P34>: 1 for fixed isotropic diffusivity, 0 instead\n"
                    ".  <P35>: 1 for fixed extracellular perpendicular"
                    " diffusivity, 0 instead\n"
                    ".  <P36>: 1 for fixed stationary fraction, 0 instead\n"
                    ".  <P37>: 1 for fixed intracellular fraction, 0 instead\n"
                    ".  <P38>: 1 for fixed kappa, 0 instead\n"
                    ".  <P39>: 1 for fixed parallel diffusivity, 0 instead\n"
                    ".  <P40>: 1 for fixed axon diameter, 0 instead\n"
                    ".  <P41>: 1 for fixed principal orientation, 0 instead\n"
                    ".  <P42>: noise standard deviation\n"
                    ".  <P43>: using quick search optimizer\n"
                    ".  <P44>: quick search step count per parameter\n"
                    ".  <P45>: using constrained NLP optimizer\n"
                    ".  <P46>: NLP maximum iteration count\n"
                    ".  <P47>: NLP maximum step size\n"
                    ".  <P48>: using MCMC optimizer\n"
                    ".  <P49>: MCMC burnin count\n"
                    ".  <P50>: MCMC sample count\n"
                    ".  <P51>: MCMC interval count\n"
                    ".  <P52>: MCMC maximum iteration count\n"
                    ".  (default: \n"
                    ".  0.0/ 3.0e-9/ 0.0e-9/ 0.0/ 0.5/ 0.0/ 1.7e-9 / 2.0e-6\n"
                    ".  0.0/ 0.0e-9/ 0.0e-9/ 0.0/ 0.0/ 0.0/ 0.0e-9 / 0.0e-6\n"
                    ".  1.0/ 6.0e-9/ 3.0e-9/ 1.0/ 1.0/ 200/ 3.0e-9/ 10.0e-6\n"
                    ".  0.1/ 0.5e-9/ 0.5e-9/ 0.1/ 0.1/ 2.0/ 0.5e-9/  1.0e-6\n"
                    ".  0.0/ 0.0   / 0.0   / 0.0/ 0.0/ 0.0/ 0.0   /  0.0/ 1.0\n"
                    ".  0.01\n"
                    ".  1.0, 10.0\n"
                    ".  1.0, 1000.0, 0.01\n"
                    ".  1.0, 5000.0, 500.0, 100.0, 100000.0, \n\n"
                    
                    "- in case of ax_caliber_odf_cartesian_field,\n"
                    ".  N/A"
                    "\n",
                    specificScalarParameters );
    application.addSeriesOption( "-stringParameters",
                    "Microstructure cartesian field specific parameters as "
                    "a vector of string <P1> <P2> ... <Pn>:\n\n"
                    "- in case of ivim_odf_cartesian_field,\n"
                    ".  N/A\n\n"
                    "- in case of noddi_odf_cartesian_field,\n"
                    ".  <P1>: sphere distribution type\n"
                    ".        - watson\n"
                    ".        - bingham\n"
                    ".  (default= watson)\n\n"
                    "- in case of axon_mapping_odf_cartesian_field,\n"
                    ".  <P1>: microstructural model type\n"
                    ".        - pgse\n"
                    ".        - cosine-trapezoid-ogse\n"
                    ".  (default= pgse)\n\n"
                    "- in case of ax_caliber_odf_cartesian_field,\n"
                    ".  N/A"
                    "\n",
                    specificStringParameters );

    application.addSeriesOption(
                    "-furtherScalarParameters",
                    "Further scalar parameters to be used by functors\n"
                    "- in case of measurement_versus_model_plot\n"
                    ".  <P1>: output orientation count\n "
                    ".  <P2>: delta b-calue in s/mm2\n ",
                    furtherScalarParameters );
    application.addSeriesOption(
                    "-furtherStringParameters",
                    "Further string parameters to be used by functors\n"
                    "- in case of measurement_versus_model_plot\n"
                    ".  <P1>: volume mask filename\n "
                    ".  <P2>: 3D transformation from mask to ODF Cartesian\n"
                    ".        field filename\n ",
                    furtherStringParameters );


    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             modelType,
             odfFunctorNames,
             outputFileNames,
             iBox,
             fBox,
             superResolution,
             outputOrientationCount,
             radius,
             thresholdRatio,
             volumeFormat,
             meshMapFormat,
             textureMapFormat,
             rgbScale,
             meshScale,
             lowerGFAThreshold,
             upperGFAThreshold,
             specificScalarParameters,
             specificStringParameters,
             furtherScalarParameters,
             furtherStringParameters,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiMicrostructureFieldCommand::parse()" );

}


void gkg::DwiMicrostructureFieldCommand::execute(
                     const std::string& fileNameT2,
                     const std::string& fileNameDW,
                     const std::string& fileNameMask,
                     const std::string& modelType,
                     const std::list< std::string >& odfFunctorNames,
                     const std::list< std::string >& outputFileNames,
                     const std::vector< int32_t >& iBox,
                     const std::vector< float >& fBox,
                     const std::vector< float >& superResolution,
                     int32_t outputOrientationCount,
                     float radius,
                     float thresholdRatio,
                     const std::string& volumeFormat,
                     const std::string& meshMapFormat,
                     const std::string& textureMapFormat,
                     float rgbScale,
                     float meshScale,
                     float lowerGFAThreshold,
                     float upperGFAThreshold,
                     const std::vector< double >& specificScalarParameters,
                     const std::vector< std::string >& specificStringParameters,
                     const std::vector< double >& furtherScalarParameters,
                     const std::vector< std::string >& furtherStringParameters,
                     bool ascii,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( odfFunctorNames.size() != outputFileNames.size() )
    {

      throw std::runtime_error(
                       "different Microstructure functor and file name count" );

    }
    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }

    std::vector< double > 
      theSpecificScalarParameters = specificScalarParameters;
    std::vector< std::string >
      theSpecificStringParameters = specificStringParameters;
    gkg::MicrostructureCartesianFieldFactory::getInstance().
      checkOrInitializeDefaultParameters( modelType,
                                          theSpecificScalarParameters,
                                          theSpecificStringParameters );


    ////////////////////////////////////////////////////////////////////////////
    // reading T2 data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameT2 << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > t2( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > t2VolumeReaderProcess( *t2 );
    t2VolumeReaderProcess.execute( fileNameT2 );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading DW data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameDW << "' : " << std::flush;

    }
    gkg::RCPointer< gkg::Volume< float > > dw( new gkg::Volume< float > );
    gkg::TypedVolumeReaderProcess< float > dwVolumeReaderProcess( *dw );
    dwVolumeReaderProcess.execute( fileNameDW );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // making or reading mask
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::Volume< int16_t > > mask( new gkg::Volume< int16_t > );
    if ( fileNameMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "building mask over T2 data : " << std::flush;

      }
      if ( radius <= 0 )
      {

        if ( t2->getHeader().hasAttribute( "resolutionX" ) &&
             t2->getHeader().hasAttribute( "resolutionY" ) &&
             t2->getHeader().hasAttribute( "resolutionZ" ) )
        {

          double resolutionX = 0.0;
          double resolutionY = 0.0;
          double resolutionZ = 0.0;
          t2->getHeader().getAttribute( "resolutionX", resolutionX );
          t2->getHeader().getAttribute( "resolutionY", resolutionY );
          t2->getHeader().getAttribute( "resolutionZ", resolutionZ );
          radius = 2.0 * std::min( resolutionX,
                                   std::min( resolutionY, resolutionZ ) );

        }
        else
        {

          radius = 1.0;

        }

      }
      gkg::ClosedMask< float, int16_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( *t2, *mask, verbose );

      if ( verbose )
      {
	
	std::cout << "done" << std::endl;
	
      }

      if ( verbose )
      {
 
        std::cout << "writing 'mask' : " << std::flush;
 
      }

      gkg::File fileMask( outputFileNames.front() );
      std::string fileNameMask = fileMask.getDirectoryName() +
                                 gkg::getDirectorySeparator() +
                                 "mask";
      gkg::Writer::getInstance().write( fileNameMask,
                                        *mask,
                                        ascii,
                                        volumeFormat );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading '" << fileNameMask << "' : " << std::flush;

      }
      gkg::Reader::getInstance().read( fileNameMask, *mask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building output orientation set
    ////////////////////////////////////////////////////////////////////////////

    gkg::OrientationSet* outputOrientationSet = 0;
    if ( outputOrientationCount > 0 )
    {

      if ( verbose )
      {

        std::cout << "building output orientation count : " << std::flush;

      }
      outputOrientationSet = new gkg::OrientationSet(
                               gkg::ElectrostaticOrientationSet( 
                    outputOrientationCount / 2 ).getSymmetricalOrientations() );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // building ODF cartesian field
    ////////////////////////////////////////////////////////////////////////////

    gkg::OdfCartesianField*
      odfCartesianField =
        gkg::MicrostructureCartesianFieldFactory::getInstance().create(
                                                    modelType,
                                                    t2,
                                                    dw,
                                                    mask,
                                                    iBox,
                                                    fBox,
                                                    superResolution,
                                                    outputOrientationSet,
                                                    theSpecificScalarParameters,
                                                    theSpecificStringParameters,
                                                    verbose );


    ////////////////////////////////////////////////////////////////////////////
    // building and writing ODF functor map(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::OdfFunctorWriter odfFunctorWriter( rgbScale,
                                            meshScale,
                                            lowerGFAThreshold,
                                            upperGFAThreshold,
                                            volumeFormat,
                                            meshMapFormat,
                                            textureMapFormat,
                                            ascii,
                                            furtherScalarParameters,
                                            furtherStringParameters,
                                            verbose );
    odfFunctorWriter.write( odfFunctorNames,
                            outputFileNames,
                            *odfCartesianField );


    ////////////////////////////////////////////////////////////////////////////
    // deleting output orientationset, ODF field
    ////////////////////////////////////////////////////////////////////////////

    if ( outputOrientationCount > 0 )
    {

      delete outputOrientationSet;

    }
    delete odfCartesianField;

  }
  GKG_CATCH( "void gkg::DwiMicrostructureFieldCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& modelType, "
             "const std::list< std::string >& odfFunctorNames, "
             "const std::list< std::string >& outputFileNames, "
             "const std::vector< int32_t >& iBox, "
             "const std::vector< float >& fBox, "
             "const std::vector< float >& superResolution, "
             "int32_t outputOrientationCount, "
             "float radius, "
             "float thresholdRatio, "
             "const std::string& volumeFormat, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "float rgbScale, "
             "float meshScale, "
             "float lowerGFAThreshold, "
             "float upperGFAThreshold, "
             "const std::vector< double >& specificScalarParameters, "
             "const std::vector< std::string >& specificStringParameters, "
             "const std::vector< double >& furtherScalarParameters, "
             "const std::vector< std::string >& furtherStringParameters, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiMicrostructureFieldCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( modelType ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( odfFunctorNames ) +
    DECLARE_LIST_OF_STRINGS_PARAMETER_HELP( outputFileNames ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( iBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( fBox ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( superResolution ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_FLOATING_PARAMETER_HELP( rgbScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( meshScale ) +
    DECLARE_FLOATING_PARAMETER_HELP( lowerGFAThreshold ) +
    DECLARE_FLOATING_PARAMETER_HELP( upperGFAThreshold ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( specificScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( specificStringParameters ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( furtherScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( furtherStringParameters ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
