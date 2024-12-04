#include <gkg-dmri-plugin-functors/DwiOdfField/DwiOdfFieldCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-dmri-container/OdfCartesianFieldFactory.h>
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


gkg::DwiOdfFieldCommand::DwiOdfFieldCommand( int32_t argc,
                                             char* argv[],
                                             bool loadPlugin,
                                             bool removeFirst )
                        : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiOdfFieldCommand::DwiOdfFieldCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiOdfFieldCommand::DwiOdfFieldCommand(
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
  GKG_CATCH( "gkg::DwiOdfFieldCommand::DwiOdfFieldCommand( "
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


gkg::DwiOdfFieldCommand::DwiOdfFieldCommand(
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
  GKG_CATCH( "gkg::DwiOdfFieldCommand::DwiOdfFieldCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiOdfFieldCommand::~DwiOdfFieldCommand()
{
}


std::string gkg::DwiOdfFieldCommand::getStaticName()
{

  try
  {

    return "DwiOdfField";

  }
  GKG_CATCH( "std::string gkg::DwiOdfFieldCommand::getStaticName()" );

}


void gkg::DwiOdfFieldCommand::parse()
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
    float thresholdRatio = 0.01;
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
    std::string modelText = "Type of the ODF model :\n";
    std::list< std::string >
      allModelNames = gkg::OdfCartesianFieldFactory::getInstance().getNames();
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
    std::string odfFunctorText = "ODF functor list among :\n";
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

    gkg::Application application( _argc, _argv, "DWI ODF field model",
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
			   "ODF cartesian field specific parameters as "
			   "a vector of double <P1> <P2> ... <Pn>:"
			   "\n\n"
			   "- in case of tensor_odf_cartesian_field,\n"
			   ".  N/A"
			   "\n\n"
			   "- in case of qball_odf_cartesian_field,\n"
			   ".  <P1>: equator point count\n"
			   ".  <P2>: phi function angle in degrees\n"
			   ".  <P3>: phi function maximum angle in "
			   "degrees\n"
			   ".  (default= 50 / minimum measure angle / "
			   "3 x minimum measure angle)"
			   "\n\n"
			   "- in case of dsi_odf_cartesian_field,\n"
			   ".  <P1>: Filtering data or not before FFT\n"
			   ".  <P2>: Minimum R0 for ODF computation (in [0,8])\n"
			   ".  <P3>: Maximum R0 for ODF computation (in [0,8])\n"
			   ".  <P4>: Marginal ODF or not\n"
			   ".  (default= 1 / 1 / 7 / 1 )"
			   "\n\n"
			   "- in case of dot_odf_cartesian_field,\n"
			   ".  <P1>: maximum spherical harmonic order\n"
			   ".  <P2>: effective diffusion time (ms) \n"
			   ".  <P3>: R0 for DOT/ODF computation (um) \n"
			   ".  <P4>: ODF computation or not\n"
			   ".  (default= 4 / 25 / 12 / 1 )"
			   "\n\n"
			   "- in case of aqball_odf_cartesian_field,\n"
			   ".  <P1>: spherical harmonics order\n"
			   ".  <P2>: regularization L-curve factor\n"
			   ".  <P3>: Laplace-Beltrami sharpening factor\n"
			   ".  (default= 4 / 0.006 / 0.0)"
			   "\n\n"
			   "- in case of sa-aqball_odf_cartesian_field,\n"
			   ".  <P1>: spherical harmonics order\n"
			   ".  <P2>: regularization L-curve factor\n"
			   ".  <P3>: Laplace-Beltrami sharpening factor\n"
			   ".  (default= 4 / 0.006 / 0.0)"
			   "\n\n"
			   "- in case of sdt_odf_cartesian_field,\n"
			   ".  <P1>: kernel voxel count\n"
			   ".  <P2>: kernel lower FA threshold or "
			   "eigenvalue if <P1> is nul\n"
			   ".  <P3>: kernel upper FA threshold or "
			   "ratio if <P1> is nul\n"
			   ".  <P4>: spherical harmonics order\n"
			   ".  <P5>: regularization L-curve factor\n"
			   ".  <P6>: use constrained spherical "
			   "deconvolution (CSD) \n"
			   //".  <P7>: lambda in CSD Tournier et al 2007\n"
			   ".  (default= 300 / 0.65 / 0.85 / 4 / 0.006 "
			   "/ 0)"// / 1.0)"
			   "\n\n"
			   "- in case of sd_odf_cartesian_field,\n"
			   ".  <P1>: kernel voxel count\n"
			   ".  <P2>: kernel lower FA threshold or "
			   "eigenvalue if <P1> is nul\n"
			   ".  <P3>: kernel upper FA threshold or "
			   "ratio if <P1> is nul\n"
			   ".  <P4>: spherical harmonics order\n"
			   ".  <P5-N>: filter coefficients\n"
			   ".  <PN+1>: use constrained spherical "
			   "deconvolution\n"
			   ".  (default=300 / 0.65 / 0.85 / 4 / \n"
			   ".   1:1:1:0.5:0.1:0.02:0.002:"
			   "0.0005:\n"
			   ".   0.0001:0.0001:0.00001:0.00001:0.00001:\n"
			   ".   0.00001:0.00001:0.00001:0.00001 / 0)"
			   "\n\n"
			   "- in case of msmt_csd_odf_cartesian_field,\n"
			   ".  <P1>: tissue count\n"
			   ".  <P2>: CQP solver maximum iteration count\n"
			   ".  <P3>: CQP solver epsilon gap\n"
			   ".  <P4>: CQP solver epsilon residuals\n"
			   ".  <P5>: CQP solver epsilon infeasible\n"
			   ".  <P6>: tissue 1: kernel wanted voxel count\n"
			   ".  <P7>: tissue 1: tissue class 1st threshold\n"
			   ".  <P8>: tissue 1: tissue class 2nd threshold\n"
			   ".  <P9>: tissue 1: FA 1st threshold\n"
			   ".  <P10>: tissue 1: FA 2nd threshold\n"
			   ".  <P11>: tissue 1: spherical harmonics order\n"
			   ".  <P12>: tissue 2: ...\n"
			   ".  <P13>: tissue 2: ...\n"
			   ".  (default=3 / 100 / 1e-10 / 1e-10 / 1e-10 / \n"
			   ".   300 / 0.9 / 0.0 / 0.8 / 0.0 / 8 / \n"
			   ".   300 / 0.9 / 0.0 / 0.2 / 0.0 / 0 / \n"
			   ".   300 / 0.9 / 0.0 / 0.1 / 0.0 / 0 )"
			   "\n",
			   specificScalarParameters );
    application.addSeriesOption( "-stringParameters",
                                 "ODF cartesian field specific parameters as "
                                 "a vector of string <P1> <P2> ... <Pn>:"
                                 "\n\n"
                                 "- in case of tensor_odf_cartesian_field,\n"
                                 ".  <P1>: estimate type\n"
                                 ".        - linear_square\n"
                                 ".        - robust_positive_definite\n"
                                 ".  (default=linear_square)"
                                 "\n\n"
                                 "- in case of qball_odf_cartesian_field,\n"
                                 ".  <P1>: phi function type\n"
                                 ".        - gaussian\n"
                                 ".        - multiquadric\n"
                                 ".        - thinplate\n"
                                 ".  (default= gaussian)"
                                 "\n\n"
                                 "- in case of dsi_odf_cartesian_field,\n"
                                 ".  N/A"
                                 "\n\n"
                                 "- in case of dot_odf_cartesian_field,\n"
                                 ".  N/A"
                                 "\n\n"
                                 "- in case of aqball_odf_cartesian_field,\n"
                                 ".  N/A"
                                 "\n\n"
                                 "- in case of sa-aqball_odf_cartesian_field,\n"
                                 ".  N/A"
                                 "\n\n"
                                 "- in case of sdt_odf_cartesian_field,\n"
                                 ".  <P1>: kernel type\n"
                                 ".        - symmetric_tensor\n"
                                 ".  (default= symmetric_tensor)"
                                 "\n\n"
                                 "- in case of sd_odf_cartesian_field,\n"
                                 ".  <P1>: kernel type\n"
                                 ".        - symmetric_tensor\n"
                                 ".  (default= symmetric_tensor)"
                                 "\n\n"
                                 "- in case of msmt_csd_odf_cartesian_field,\n"
                                 ".  <P1>: tissue class probability map\n"
                                 ".  <P2>: tissue to diffusion 3D transform \n"
                                 ".        type (none, linear, or "
                                 "linear_and_diffeomorphic)\n"
                                 ".  <P3>: tissue to diffusion 3D linear \n"
                                 ".        transform\n"
                                 ".  <P4>: tissue to diffusion direct 3D \n"
                                 ".        diffeomorphic transform\n"
                                 ".  <P5>: tissue to diffusion inverse 3D \n"
                                 ".        diffeomorphic transform\n"
                                 ".  <P6>: tissue 1: kernel type \n"
                                 ".        (symmetric_tensor)\n"
                                 ".  <P7>: tissue 1: tissue class\n"
                                 ".        threshdolding mode\n"
                                 ".  <P8>: tissue 1: FA threshdolding mode\n"
                                 ".  <P9>: tissue 1: FA sorting type \n"
                                 ".                  (increasing or\n"
                                 ".                   decreasing)\n"
                                 ".  <P10>: tissue 2: ...\n"
                                 ".  <P11>: tissue 2: ...\n"
                                 ".  <P12>: tissue 2: ...\n"
                                 ".  <P13>: tissue 2: ...\n"
                                 "\n",
                                 specificStringParameters );

    application.addSeriesOption( "-furtherScalarParameters",
                                 "Further scalar parameters to be used by "
                                 "functors",
                                 furtherScalarParameters );
    application.addSeriesOption( "-furtherStringParameters",
                                 "Further string parameters to be used by "
                                 "functors",
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
  GKG_CATCH_FUNCTOR( "void gkg::DwiOdfFieldCommand::parse()" );

}


void gkg::DwiOdfFieldCommand::execute(
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

      throw std::runtime_error( "different ODF functor and file name count" );

    }
    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }

    std::vector< double > 
      theSpecificScalarParameters = specificScalarParameters;
    std::vector< std::string >
      theSpecificStringParameters = specificStringParameters;
    gkg::OdfCartesianFieldFactory::getInstance().
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
    else
    {
      
      // In case of DSI, cannot take the input gradients as output gradients
      // must create a set of output orientation
      if ( modelType == "dsi_odf_cartesian_field" ) 
      {
	
	if ( verbose )
	{

	  std::cout << "building output orientation count : " << std::flush;

	}
	outputOrientationSet = new gkg::OrientationSet(
        gkg::ElectrostaticOrientationSet( 500 / 2 ).getSymmetricalOrientations() );
	if ( verbose )
	{

	  std::cout << "done" << std::endl;
	  
	}

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // building ODF cartesian field
    ////////////////////////////////////////////////////////////////////////////

    gkg::OdfCartesianField*
      odfCartesianField = gkg::OdfCartesianFieldFactory::getInstance().create(
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
  GKG_CATCH( "void gkg::DwiOdfFieldCommand::execute( "
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
    DwiOdfFieldCommand,
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
