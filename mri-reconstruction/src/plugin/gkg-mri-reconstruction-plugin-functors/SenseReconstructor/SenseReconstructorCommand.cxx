#include <gkg-mri-reconstruction-plugin-functors/SenseReconstructor/SenseReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/ConditionalMask_i.h>
#include <gkg-processing-algobase/ConstantFilter_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/PhaseConverter_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-processing-extrapolation/RecursiveExtrapolator3d_i.h>
#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-numericalanalysis/CMatrix.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/CWienerSmoothing.h>
#include <gkg-processing-numericalanalysis/PseudoInverseFilterFactor.h>
#include <gkg-processing-numericalanalysis/SVDInverser.h>
#include <gkg-processing-numericalanalysis/TikhonovFilterFactor.h>
#include <gkg-processing-numericalanalysis/TruncatedFilterFactor.h>
#include <gkg-processing-resampling/QuickResamplerFactory.h>
#include <gkg-processing-resampling/SpreaderFactory.h>
#include <gkg-processing-signal/Filter3d_i.h>
#include <gkg-mri-reconstruction-parallel/B1ReferenceFunctor.h>
#include <gkg-mri-reconstruction-parallel/B1SensitivityFunctor.h>
#include <gkg-mri-reconstruction-parallel/NoiseCovarianceFunctor.h>
#include <gkg-mri-reconstruction-parallel/SenseReconstructor.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <set>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iomanip>


static std::string getExtraName( const std::string& name,
                                 const std::string& extraName )
{

  try
  {

    std::set< std::string > extensions;
    extensions.insert( ".dim" );
    extensions.insert( ".ima" );
    extensions.insert( ".header" );

    std::set< std::string >::const_iterator e = extensions.begin(),
                                            ee = extensions.end();

    while ( e != ee )
    {

      if ( name.length() > e->length() )
      {

        if ( name.substr( name.length() - e->length(), e->length()) == *e )
        {

          return name.substr( 0, name.length() - e->length() ) + '_' +
                 extraName + *e;

        }

      }
      ++ e;

    }
    return name + "_" + extraName;

  }
  GKG_CATCH( "std::string getExtraName( "
             "const std::string& name, "
             "const std::string& extraName )" );

}



gkg::SenseReconstructorCommand::SenseReconstructorCommand( int32_t argc,
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
  GKG_CATCH( "gkg::SenseReconstructorCommand::SenseReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SenseReconstructorCommand::SenseReconstructorCommand(
                                      const std::string& fileNameCoilInput,
                                      const std::string& fileNameFoldedInput,
                                      const std::string& fileNameUnfoldedOutput,
                                      float dilationRadiusFactor,
                                      float closingRadiusFactor,
                                      double noisePosition,
                                      double noiseRatio,
                                      int32_t thrown,
                                      int32_t kept,
                                      float scale,
                                      const std::string& inverseMethod,
                                      const std::string& normWeightingMethod,
                                      double tikhonovParameter,
                                      bool extra,
                                      const std::string& outputFormat,
                                      bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameCoilInput,
             fileNameFoldedInput,
             fileNameUnfoldedOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             scale,
             inverseMethod,
             normWeightingMethod,
             tikhonovParameter,
             extra,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::SenseReconstructorCommand::SenseReconstructorCommand( "
             "const std::string& fileNameCoilInput, "
             "const std::string& fileNameFoldedInput, "
             "const std::string& fileNameUnfoldedOutput, "
             "float dilationRadiusFactor, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "float scale, "
             "const std::string& inverseMethod, "
             "const std::string& normWeightingMethod, "
             "double tikhonovParameter, "
             "bool extra, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::SenseReconstructorCommand::SenseReconstructorCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCoilInput );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameFoldedInput );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameUnfoldedOutput );
    DECLARE_FLOATING_PARAMETER( parameters, float, dilationRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, float, closingRadiusFactor );
    DECLARE_FLOATING_PARAMETER( parameters, double, noisePosition );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseRatio );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, thrown );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, kept );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_STRING_PARAMETER( parameters, std::string, inverseMethod );
    DECLARE_STRING_PARAMETER( parameters, std::string, normWeightingMethod );
    DECLARE_FLOATING_PARAMETER( parameters, double, tikhonovParameter );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, extra );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameCoilInput,
             fileNameFoldedInput,
             fileNameUnfoldedOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             scale,
             inverseMethod,
             normWeightingMethod,
             tikhonovParameter,
             extra,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::SenseReconstructorCommand::SenseReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SenseReconstructorCommand::~SenseReconstructorCommand()
{
}


std::string gkg::SenseReconstructorCommand::getStaticName()
{

  try
  {

    return "SenseReconstructor";

  }
  GKG_CATCH( "std::string gkg::SenseReconstructorCommand::getStaticName()" );

}


void gkg::SenseReconstructorCommand::parse()
{

  try
  {

    std::string fileNameCoilInput;
    std::string fileNameFoldedInput;
    std::string fileNameUnfoldedOutput;
    float dilationRadiusFactor = 2.0;
    float closingRadiusFactor = 2.0;
    double noisePosition = 0.9;
    double noiseRatio = 0.01;
    int32_t thrown = 20;
    int32_t kept = 100;
    float scale = 0.1;
    std::string inverseMethod = "pseudo";
    std::string normWeightingMethod = "none";
    double tikhonovParameter = 0.01;
    bool extra = false;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Reconstructor SENSE",
                                  _loadPlugin );
    application.addSingleOption( "-c",
                                 "Multi-coil input file name",
                                 fileNameCoilInput );
    application.addSingleOption( "-f",
                                 "Folded input file name",
                                 fileNameFoldedInput );
    application.addSingleOption( "-o",
                                 "Output unfolded reconstructed file name",
                                 fileNameUnfoldedOutput );
    application.addSingleOption( "-d",
                                 "Dilation radius factor [default=2.0]\n"
                                 "Morphological dilation of the B1 reference "
                                 "mask is done with the maximum in-plane "
                                 "resolution times this factor",
                                 dilationRadiusFactor,
                                 true );
    application.addSingleOption( "-r",
                                 "Closing radius factor [default=2.0]\n"
                                 "Morphological closing is done with the"
                                 " minimum in-plane resolution times this "
                                 "factor",
                                 closingRadiusFactor,
                                 true );
    application.addSingleOption( "-noisePosition",
                                 "Noise position along x axis (default=0.9)",
                                 noisePosition,
                                 true );
    application.addSingleOption( "-noiseRatio",
                                 "Noise ratio (default=0.01)",
                                 noiseRatio,
                                 true );
    application.addSingleOption( "-thrown",
                                 "Maximum thrown count (default=20)",
                                 thrown,
                                 true );
    application.addSingleOption( "-kept",
                                 "Maximum kept (default=100)",
                                 kept,
                                 true );
    application.addSingleOption( "-scale",
                                 "Scale factor applied to reconstructed data"
                                 " (default=0.1)",
                                 scale,
                                 true );
    application.addSingleOption( "-inverseMethod",
                                 "Inversion method :\n"
                                 "- pseudo -> Pseudo-inversion,\n"
                                 "- tikhonov -> Tikohonov inversion,\n"
                                 "- truncated -> SVD Truncated inversion,\n"
                                 " (default=pseudo)",
                                 inverseMethod,
                                 true );
    application.addSingleOption( "-normWeightingMethod",
                                 "Norm weighting method :\n"
                                 "- none -> no weighting,\n"
                                 "- covariance -> whitening\n"
                                 " (default=none)",
                                 normWeightingMethod,
                                 true );
    application.addSingleOption( "-tikhonovParameter",
                                 "Tikhonov regularization parameter "
                                 "(default=0.01)",
                                 tikhonovParameter,
                                 true );
    application.addSingleOption( "-extra",
                                 "Save extra information :\n"
                                 "- magnitude of B1 reference\n"
                                 "- magnitude of B1 sensitivity\n"
                                 "- phase of B1 sensitivity\n"
                                 "- mask of B1 reference\n"
                                 "- enlarged mask of B1 reference\n"
                                 "- masked extrapolated filtered "
                                    "B1 sensitivity\n"
                                 "- magnitude of masked extrapolated filtered "
                                    "B1 sensitivity\n",
                                  extra,
                                 true );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameCoilInput,
             fileNameFoldedInput,
             fileNameUnfoldedOutput,
             dilationRadiusFactor,
             closingRadiusFactor,
             noisePosition,
             noiseRatio,
             thrown,
             kept,
             scale,
             inverseMethod,
             normWeightingMethod,
             tikhonovParameter,
             extra,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SenseReconstructorCommand::parse()" );

}


void gkg::SenseReconstructorCommand::execute(
                                      const std::string& fileNameCoilInput,
                                      const std::string& fileNameFoldedInput,
                                      const std::string& fileNameUnfoldedOutput,
                                      float dilationRadiusFactor,
                                      float closingRadiusFactor,
                                      double noisePosition,
                                      double noiseRatio,
                                      int32_t thrown,
                                      int32_t kept,
                                      float scale,
                                      const std::string& inverseMethod,
                                      const std::string& normWeightingMethod,
                                      double tikhonovParameter,
                                      bool extra,
                                      const std::string& outputFormat,
                                      bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading coil input
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reading coil input '" << fileNameCoilInput
                << "' : " << std::flush;

    }
    gkg::Volume< std::complex< float > > coilInput;
    gkg::Reader::getInstance().read( fileNameCoilInput, coilInput );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    double coilResolutionX = 0.0, coilResolutionY = 0.0;
    if ( coilInput.getHeader().hasAttribute( "resolutionX" ) &&
         coilInput.getHeader().hasAttribute( "resolutionY" ) )
    {

      coilInput.getHeader().getAttribute( "resolutionX", coilResolutionX );
      coilInput.getHeader().getAttribute( "resolutionY", coilResolutionY );

    }
    else
    {

      throw std::runtime_error( "cannot find X and Y in plane resolution" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading folded input proxy
    ////////////////////////////////////////////////////////////////////////////
    std::string format;
    gkg::VolumeProxy< std::complex< float > > foldedInputProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                       gkg::Volume< std::complex< float > > >(
                                                            fileNameFoldedInput,
                                                            foldedInputProxy );
    if ( format != "gis" )
    {

      throw std::runtime_error( "wrong format, should be \'gis\'");

    }


    ////////////////////////////////////////////////////////////////////////////
    // building B1 reference map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building B1 reference : " << std::flush;

    }
    gkg::MagnitudeConverter< gkg::Volume< std::complex< float > >,
                             gkg::Volume< float > > magnitudeConverter;
    gkg::PhaseConverter< gkg::Volume< std::complex< float > >,
                         gkg::Volume< float > > phaseConverter;
    gkg::B1ReferenceFunctor< float > b1ReferenceFunctor;
    gkg::Volume< std::complex< float > > b1Reference;
    b1ReferenceFunctor.process( coilInput, b1Reference, verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing B1 sensitivity map
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 sensitivity map : " << std::flush;

    }
    gkg::B1SensitivityFunctor< float > b1SensitivityFunctor;
    gkg::Volume< std::complex< float > > b1Sensitivity;
    b1SensitivityFunctor.process( coilInput,
                                  b1Reference,
                                  b1Sensitivity,
                                  verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving magnitude of B1 sensitivity : " << std::flush;

      }
      gkg::Volume< float > b1SensitivityMagnitude;
      magnitudeConverter.convert( b1Sensitivity, b1SensitivityMagnitude );
      gkg::Writer::getInstance().write(
             getExtraName( fileNameUnfoldedOutput, "b1SensitivityMagnitude" ),
             b1SensitivityMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( verbose )
      {

        std::cout << "saving phase of B1 sensitivity : " << std::flush;

      }
      gkg::Volume< float > b1SensitivityPhase;
      phaseConverter.convert( b1Sensitivity, b1SensitivityPhase );
      gkg::Writer::getInstance().write(
             getExtraName( fileNameUnfoldedOutput, "b1SensitivityPhase" ),
             b1SensitivityPhase );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // processing B1 reference magnitude
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "processing B1 reference magnitude : " << std::flush;

    }
    gkg::Volume< float > b1ReferenceMagnitude;
    magnitudeConverter.convert( b1Reference, b1ReferenceMagnitude );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving magnitude of B1 reference : " << std::flush;

      }
      gkg::Writer::getInstance().write(
              getExtraName( fileNameUnfoldedOutput, "b1ReferenceMagnitude" ),
              b1ReferenceMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building B1 reference mask : " << std::flush;

    }
    gkg::MorphoFilledMask< float, uint8_t >
      morphoFilledMaskAlgo( closingRadiusFactor * 
                            std::min( coilResolutionX,
                                      coilResolutionY ),
                            noisePosition,
                            noiseRatio,
                            thrown,
                            kept );
    gkg::Volume< uint8_t > b1ReferenceMask;
    morphoFilledMaskAlgo.getMask( b1ReferenceMagnitude,
                                  b1ReferenceMask,
                                  verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving B1 reference mask : " << std::flush;

      }
      gkg::Writer::getInstance().write( getExtraName( fileNameUnfoldedOutput,
                                                      "b1ReferenceMask" ),
                                         b1ReferenceMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "building enlarged B1 mask : " << std::flush;

    }
    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
    gkg::Dilation< uint8_t, uint8_t >
      dilation( dilationRadiusFactor * std::max( coilResolutionX,
                                                 coilResolutionY ),
                equalToFunction,
                1U,
                0U );
    gkg::Volume< uint8_t > enlargedB1ReferenceMask;
    dilation.dilate( b1ReferenceMask,
                     enlargedB1ReferenceMask,
                     verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving enlarged B1 reference mask : " << std::flush;

      }
      gkg::Writer::getInstance().write(
              getExtraName( fileNameUnfoldedOutput, "enlargedB1ReferenceMask" ),
              enlargedB1ReferenceMask );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // median filtering sensitivity map over B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "filtering B1 sensitivity map : " << std::flush;

    }
    gkg::MedianFilter< std::vector< std::complex< float > >,
                       std::complex< float > > medianFilter;
    gkg::Filter3d< std::complex< float >,
                   uint8_t,
                   std::complex< float > > medianFilter3d;
    medianFilter3d.filter( b1Sensitivity,
                           b1ReferenceMask,
                           equalToFunction,
                           medianFilter,
                           gkg::Neighborhood3d::Neighborhood2d_8xy,
                           b1Sensitivity,
                           verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // median extrapolating sensitivity map over enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "extrapolating B1 sensitivity map : " << std::flush;

    }
    gkg::RecursiveExtrapolator3d< std::complex< float >,
                                  uint8_t,
                                  std::complex< float > >
      medianExtrapolator3d( medianFilter,
                            gkg::Neighborhood3d::Neighborhood2d_8xy );
    medianExtrapolator3d.extrapolate( b1Sensitivity,
                                      b1ReferenceMask,
                                      enlargedB1ReferenceMask,
                                      equalToFunction,
                                      b1Sensitivity,
                                      verbose );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // masking sensitivity map outside enlarged B1 reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "masking B1 sensitivity map : " << std::flush;

    }
    gkg::ConstantFilter< std::complex< float >, std::complex< float > >
      constantFilter( std::complex< float >( 0.0, 0.0 ) );
    gkg::DifferentFromFunction< uint8_t > differentFromFunction( 1U );
    gkg::ConditionalMask< gkg::Volume< std::complex< float > >, uint8_t >
      conditionalMask;
    conditionalMask.mask( b1Sensitivity,
                          enlargedB1ReferenceMask,
                          differentFromFunction,
                          constantFilter );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving masked sensitivity B1 map
    ////////////////////////////////////////////////////////////////////////////
    if ( extra )
    {

      if ( verbose )
      {

        std::cout << "saving masked extrapolated B1 sensitivity : "
                  << std::flush;

      }
      gkg::Writer::getInstance().write(
            getExtraName( fileNameUnfoldedOutput, "extrapolatedB1Sensitivity" ),
            b1Sensitivity );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      if ( verbose )
      {

        std::cout << "saving magnitude of masked extrapolated B1 sensitivity : "
                  << std::flush;

      }
      gkg::Volume< float > b1SensitivityMagnitude;
      magnitudeConverter.convert( b1Sensitivity, b1SensitivityMagnitude );
      gkg::Writer::getInstance().write(
                           getExtraName( fileNameUnfoldedOutput,
                                         "extrapolatedB1SensitivityMagnitude" ),
                           b1SensitivityMagnitude );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    

    ////////////////////////////////////////////////////////////////////////////
    // collecting folded data information
    ////////////////////////////////////////////////////////////////////////////
    int32_t foldedSizeX = foldedInputProxy.getSizeX();
    int32_t foldedSizeY = foldedInputProxy.getSizeY();
    int32_t foldedSizeZ = foldedInputProxy.getSizeZ();
    int32_t foldedSizeT = foldedInputProxy.getSizeT();

    float yParallelReductionFactor = 1.0;
    double fovX = 0.0;
    double fovY = 0.0;
    double sliceThickness = 0.0;
    int32_t coilCount = 0;
    if ( foldedInputProxy.getHeader().hasAttribute(
                                       "pool_header.rdb_hdr_image.psd_iname" ) )
    {

      std::string psdName = "";
      foldedInputProxy.getHeader().getAttribute(
                               "pool_header.rdb_hdr_image.psd_iname", psdName );

      if ( ( psdName == "GkgGe" ) ||
           ( psdName == "NmrGe" ) )
      {

        // collecting parallel reduction factor
        double user0 = 1.0;
        foldedInputProxy.getHeader().getAttribute(
                                              "pool_header.rdb_hdr_image.user0",
                                              user0 );
        yParallelReductionFactor = ( float )user0;

        // collecting FOV, TH information
        double fov, slthick, scanspacing;
        foldedInputProxy.getHeader().getAttribute(
                                               "pool_header.rdb_hdr_image.dfov",
                                               fov );
        foldedInputProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slthick",
                                            slthick );
        foldedInputProxy.getHeader().getAttribute(
                                        "pool_header.rdb_hdr_image.scanspacing",
                                        scanspacing );
        fovX = fov;
        fovY = fov / yParallelReductionFactor;
        sliceThickness = ( double )( slthick + scanspacing );

        // collecting coil information
        double start_rcv, stop_rcv;
        foldedInputProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
        foldedInputProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
        coilCount = ( int32_t )( stop_rcv - start_rcv + 1 + 0.5 );

      }
      else if ( ( psdName == "GkgDwSeEpi" ) ||
                ( psdName == "NmrDwSeEpi" ) )
      {

        // collecting parallel reduction factor
        double user9 = 1.0;
        foldedInputProxy.getHeader().getAttribute(
                                              "pool_header.rdb_hdr_image.user9",
                                              user9 );
        yParallelReductionFactor = ( float )user9;

        // collecting FOV, TH information
        double fov, slthick, scanspacing;
        foldedInputProxy.getHeader().getAttribute(
                                               "pool_header.rdb_hdr_image.dfov",
                                               fov );
        foldedInputProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slthick",
                                            slthick );
        foldedInputProxy.getHeader().getAttribute(
                                        "pool_header.rdb_hdr_image.scanspacing",
                                        scanspacing );
        fovX = fov;
        fovY = fov / yParallelReductionFactor;
        sliceThickness = ( double )( slthick + scanspacing );

        // collecting coil information
        double start_rcv, stop_rcv;
        foldedInputProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
        foldedInputProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
        coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );

      }
      else if ( ( psdName == "GkgBoldGeEpi" ) ||
                ( psdName == "NmrBoldGeEpi" ) )
      {

        // collecting parallel reduction factor
        double user10 = 1.0;
        foldedInputProxy.getHeader().getAttribute(
                                             "pool_header.rdb_hdr_image.user10",
                                             user10 );
        yParallelReductionFactor = ( float )user10;

        // collecting FOV, TH information
        double fov, slthick, scanspacing;
        foldedInputProxy.getHeader().getAttribute(
                                               "pool_header.rdb_hdr_image.dfov",
                                               fov );
        foldedInputProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slthick",
                                            slthick );
        foldedInputProxy.getHeader().getAttribute(
                                        "pool_header.rdb_hdr_image.scanspacing",
                                        scanspacing );
        fovX = fov;
        fovY = fov / yParallelReductionFactor;
        sliceThickness = ( double )( slthick + scanspacing );

        // collecting coil information
        double start_rcv, stop_rcv;
        foldedInputProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
        foldedInputProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
        coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );

      }
      else
      {

        throw std::runtime_error( "not a managed pulse sequence");

      }

    }
    if ( yParallelReductionFactor <= 1.0 )
    {

      throw std::runtime_error(
           "parallel reduction factor incompatible with SENSE reconstruction" );

    }
    if ( coilCount == 1 )
    {

      throw std::runtime_error(
                          "coil count incompatible with SENSE reconstruction" );

    }

    int32_t rankCount = foldedSizeT / coilCount;
    int32_t unfoldedSizeX = foldedSizeX;
    int32_t unfoldedSizeY = ( int32_t )( foldedSizeY * 
                                         yParallelReductionFactor + 0.5 );
    int32_t unfoldedSizeZ = foldedSizeZ;
    int32_t unfoldedSizeT = rankCount;
    double unfoldedResolutionX = fovX / unfoldedSizeX;
    double unfoldedResolutionY = fovY * yParallelReductionFactor /
                                 unfoldedSizeY;
    double unfoldedResolutionZ = sliceThickness;


    ////////////////////////////////////////////////////////////////////////////
    // processing covariance
    ////////////////////////////////////////////////////////////////////////////
    gkg::CMatrix* noiseCovariance = 0;
    if ( normWeightingMethod == "covariance" )
    {

      if ( verbose )
      {

        std::cout << "processing noise covariance matrix : " << std::flush;

      }
      noiseCovariance = new gkg::CMatrix( coilCount, coilCount );

      gkg::NoiseCovarianceFunctor< float, uint8_t >
        noiseCovarianceFunctor;
      noiseCovarianceFunctor.getCovariance( coilInput,
                                            enlargedB1ReferenceMask,
                                            differentFromFunction,
                                            *noiseCovariance,
                                            verbose );
      if ( verbose )
      {

        std::cout << "done" << std::endl;
        std::cout << "magnitude noise covariance matrix :" << std::endl;
        std::cout << noiseCovariance->getMagnitude() << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing stuff for reconstruction
    ////////////////////////////////////////////////////////////////////////////
    // for folded input data
    gkg::BoundingBox< int32_t > foldedInputBoundingBox( 0, foldedSizeX - 1,
                                                        0, foldedSizeY - 1,
                                                        0, foldedSizeZ - 1,
                                                        0, 0 );
    gkg::Volume< std::complex< float > > foldedInputVolume;
    // for unfolded complex data
    gkg::Volume< std::complex< float > > unfoldedComplexVolume( unfoldedSizeX,
                                                                unfoldedSizeY,
                                                                unfoldedSizeZ );
    unfoldedComplexVolume.getHeader().addAttribute( "resolutionX",
                                                    unfoldedResolutionX );
    unfoldedComplexVolume.getHeader().addAttribute( "resolutionY",
                                                    unfoldedResolutionY );
    unfoldedComplexVolume.getHeader().addAttribute( "resolutionZ",
                                                    unfoldedResolutionZ );

    // for unfolded magnitude data
    gkg::Volume< float > unfoldedMagnitudeVolume( unfoldedSizeX,
                                                  unfoldedSizeY,
                                                  unfoldedSizeZ );

    // for unfolded output data
    gkg::BoundingBox< int32_t > unfoldedOutputBoundingBox( 0, unfoldedSizeX - 1,
                                                           0, unfoldedSizeY - 1,
                                                           0, unfoldedSizeZ - 1,
                                                           0, 0 );
    gkg::Volume< int16_t > unfoldedOutputVolume( unfoldedSizeX,
                                                 unfoldedSizeY,
                                                 unfoldedSizeZ );
    gkg::Scaler< gkg::Volume< float > > scaler( scale );
    gkg::LowerOrEqualToFunction< float > lowerOrEqualToFunction( 32767.0 );
    gkg::GreaterOrEqualToFunction< float > greaterOrEqualToFunction( 0.0 );
    gkg::Thresholder< gkg::Volume< float > > lowerThresholder(
                                                       lowerOrEqualToFunction,
                                                       32767.0 );
    gkg::Thresholder< gkg::Volume< float > > greaterThresholder(
                                                       greaterOrEqualToFunction,
                                                       0.0 );
    gkg::Converter< gkg::Volume< float >, gkg::Volume< int16_t > > converter;


    ////////////////////////////////////////////////////////////////////////////
    // preparing output disk proxy
    ////////////////////////////////////////////////////////////////////////////
    gkg::VolumeProxy< int16_t > unfoldedOutputProxy = 
      gkg::VolumeProxy< int16_t >( unfoldedSizeX,
                                   unfoldedSizeY,
                                   unfoldedSizeZ,
                                   unfoldedSizeT );
    unfoldedOutputProxy.getHeader() = foldedInputProxy.getHeader();
    unfoldedOutputProxy.getHeader()[ "item_type" ] =
      gkg::TypeOf< int16_t >::getName();
    unfoldedOutputProxy.getHeader()[ "sizeX" ] = unfoldedSizeX;
    unfoldedOutputProxy.getHeader()[ "sizeY" ] = unfoldedSizeY;
    unfoldedOutputProxy.getHeader()[ "sizeZ" ] = unfoldedSizeZ;
    unfoldedOutputProxy.getHeader()[ "sizeT" ] = unfoldedSizeT;
    unfoldedOutputProxy.getHeader().addAttribute( "resolutionX",
                                                  unfoldedResolutionX );
    unfoldedOutputProxy.getHeader().addAttribute( "resolutionY",
                                                  unfoldedResolutionY );
    unfoldedOutputProxy.getHeader().addAttribute( "resolutionZ",
                                                  unfoldedResolutionZ );
    unfoldedOutputProxy.getHeader().addAttribute( "resolutionT",
                                                  ( double )1.0 );
    if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                               gkg::Volume< int16_t > >( fileNameUnfoldedOutput,
                                                         unfoldedOutputProxy,
                                                         outputFormat ) !=
         outputFormat )
    {

      throw std::runtime_error(
            std::string( "failed to prepare partial write with format \' " ) +
            outputFormat + "\'" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing SENSE reconstructor
    ////////////////////////////////////////////////////////////////////////////
    gkg::ReductionStrategy< float >
      yReductionStrategy( yParallelReductionFactor );

    gkg::SenseReconstructor< float >::getInstance().setB1SensitivityMap(
                                                                b1Sensitivity );
    gkg::SenseReconstructor< float >::getInstance().setReductionStrategy(
                  gkg::SenseReconstructor< float >::YAxis, yReductionStrategy );
    gkg::SenseReconstructor< float >::getInstance().setQuickResampler(
           *gkg::QuickResamplerFactory< std::complex< float > >::getInstance().
                       getQuickResampler( "Partial Voluming QuickResampler" ) );
    gkg::SenseReconstructor< float >::getInstance().setSpreader(
           *gkg::SpreaderFactory< std::complex< float > >::getInstance().
                                   getSpreader( "Partial Voluming Spreader" ) );

    gkg::FilterFactor* filterFactor = 0;
    gkg::IllMatrixInverser* illMatrixInverser = 0;
    gkg::CNormWeighting* cNormWeighting = 0;
    if ( inverseMethod == "pseudo" )
    {


      filterFactor = new gkg::PseudoInverseFilterFactor();
      illMatrixInverser = new gkg::SVDInverser( *filterFactor );
      if ( normWeightingMethod == "covariance" )
      {

        cNormWeighting = new gkg::CWienerSmoothing( *noiseCovariance );

      }

    }
    else if ( inverseMethod == "tikhonov" )
    {

      gkg::TikhonovFilterFactor*
        tikhonovFilterFactor = new gkg::TikhonovFilterFactor();
      tikhonovFilterFactor->setLambda( tikhonovParameter );
      filterFactor = tikhonovFilterFactor;
      illMatrixInverser = new gkg::SVDInverser( *filterFactor );
      if ( normWeightingMethod == "covariance" )
      {

        cNormWeighting = new gkg::CWienerSmoothing( *noiseCovariance );

      }

    }
    else if ( inverseMethod == "truncated" )
    {

      filterFactor = new gkg::TruncatedFilterFactor();
      illMatrixInverser = new gkg::SVDInverser( *filterFactor );
      if ( normWeightingMethod == "covariance" )
      {

        cNormWeighting = new gkg::CWienerSmoothing( *noiseCovariance );

      }

    }
    gkg::SenseReconstructor< float >::getInstance().setIllMatrixInverser(
                                                           *illMatrixInverser );
    gkg::SenseReconstructor< float >::getInstance().setCNormWeighting(
                                                              *cNormWeighting );


    ////////////////////////////////////////////////////////////////////////////
    // looping over ranks(s) and reconstructing
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "reconstructing: " << std::flush;

    }

    int32_t rank;
    for ( rank = 0; rank < rankCount; rank++ )
    {

      if ( verbose )
      {

        if ( rank != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << rank + 1 
                  << " / " << std::setw( 4 ) << rankCount
                  << " ] " << std::flush;

      }

      ///////////////////////// reading folded input data //////////////////////
      if ( verbose )
      {

        std::cout << "Reading from disk" << std::flush;

      }
      foldedInputBoundingBox.setLowerT( rank * coilCount );
      foldedInputBoundingBox.setUpperT( ( rank + 1 ) * coilCount - 1 );
      gkg::Reader::getInstance().partialBinaryRead<
                                         gkg::Volume< std::complex< float > > >(
                                                       foldedInputProxy,
                                                       foldedInputVolume,
                                                       foldedInputBoundingBox );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 17 ) << std::flush;

      }


      //////////////////////////// unfolding complex data //////////////////////


      gkg::SenseReconstructor< float >::getInstance().reconstruct(
                                                          foldedInputVolume,
                                                          unfoldedComplexVolume,
                                                          verbose );


      ////////////////// taking magnitude of unfolded complex data /////////////
      if ( verbose )
      {

        std::cout << "Taking magnitude" << std::flush;

      }
      magnitudeConverter.convert( unfoldedComplexVolume,
                                  unfoldedMagnitudeVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 16 ) << std::flush;

      }


      ////////////////// rescaling and converting to short type ////////////////
      if ( verbose )
      {

        std::cout << "Rescaling+short convertion" << std::flush;

      }
      scaler.scale( unfoldedMagnitudeVolume );
      lowerThresholder.threshold( unfoldedMagnitudeVolume,
                                  unfoldedMagnitudeVolume );
      greaterThresholder.threshold( unfoldedMagnitudeVolume,
                                    unfoldedMagnitudeVolume );
      converter.convert( unfoldedMagnitudeVolume, unfoldedOutputVolume );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 26 ) << std::flush;

      }


      ///////////////// writing unfolded output magnitude data /////////////////
      if ( verbose )
      {

        std::cout << "Writing to disk" << std::flush;

      }
      unfoldedOutputBoundingBox.setLowerT( rank );
      unfoldedOutputBoundingBox.setUpperT( rank );
      gkg::Writer::getInstance().partialBinaryWrite<
                            gkg::Volume< int16_t > >( unfoldedOutputProxy,
                                                      unfoldedOutputVolume,
                                                      unfoldedOutputBoundingBox,
                                                      outputFormat );
      if ( verbose )
      {

        std::cout << gkg::Eraser( 15 ) << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // deleting allocated stuff
    ////////////////////////////////////////////////////////////////////////////
    if ( filterFactor )
    {

      delete filterFactor;

    }
    if ( illMatrixInverser )
    {

      delete illMatrixInverser;

    }
    if ( noiseCovariance )
    {

      delete noiseCovariance;

    }
    if ( cNormWeighting )
    {

      delete cNormWeighting;

    }

  }
  GKG_CATCH( "void gkg::SenseReconstructorCommand::execute( "
             "const std::string& fileNameCoilInput, "
             "const std::string& fileNameFoldedInput, "
             "const std::string& fileNameUnfoldedOutput, "
             "float dilationRadiusFactor, "
             "float closingRadiusFactor, "
             "double noisePosition, "
             "double noiseRatio, "
             "int32_t thrown, "
             "int32_t kept, "
             "float scale, "
             "const std::string& inverseMethod, "
             "const std::string& normWeightingMethod, "
             "double tikhonovParameter, "
             "bool extra, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    SenseReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameCoilInput ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameFoldedInput ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameUnfoldedOutput ) +
    DECLARE_FLOATING_PARAMETER_HELP( dilationRadiusFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( closingRadiusFactor ) +
    DECLARE_FLOATING_PARAMETER_HELP( noisePosition ) +
    DECLARE_FLOATING_PARAMETER_HELP( noiseRatio ) +
    DECLARE_INTEGER_PARAMETER_HELP( thrown ) +
    DECLARE_INTEGER_PARAMETER_HELP( kept ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_STRING_PARAMETER_HELP( inverseMethod ) +
    DECLARE_STRING_PARAMETER_HELP( normWeightingMethod ) +
    DECLARE_FLOATING_PARAMETER_HELP( tikhonovParameter ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( extra ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
