#include <gkg-mri-reconstruction-plugin-functors/GehcEviReconstructor/GehcEviReconstructorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-processing-algobase/PowerOfTwo.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-mri-reconstruction-core/HalfKSpaceReconstructor3d.h>
#include <gkg-mri-reconstruction-correction/ModulationTransferFunction.h>
#include <gkg-mri-reconstruction-correction/AhnPhaseCorrection3d.h>
#include <gkg-mri-reconstruction-correction/EchoVolumarPhaseCorrection.h>
#include <gkg-mri-reconstruction-correction/DcOffsetCorrection.h>
#include <gkg-mri-reconstruction-correction/TrapezoidROGradient.h>
#include <gkg-mri-reconstruction-correction/RegridingCorrection.h>
#include <gkg-mri-reconstruction-correction/GehcSignaGradientCoil.h>
#include <gkg-mri-reconstruction-correction/GradientWarping2dCorrection.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/RealPartConverter_i.h>
#include <gkg-processing-algobase/MagnitudeConverter_i.h>
#include <gkg-processing-algobase/Scaler_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Converter_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <fstream>


//
// std::string getRFPulseTypeName()
//

static std::string getRFPulseTypeName( bool spectroSpatial )
{

  if ( spectroSpatial )
  {

    return "spectro-spatial";

  }
  else
  {

    return "SLR90";

  }

}


/*
//
// void reorderingLines()
//

static void reorderingLines( gkg::Volume< std::complex< float > >& rawVolume )
{

  int32_t sizeX = rawVolume.getSizeX();
  int32_t lineCount = rawVolume.getSizeY();
  int32_t sliceCount = rawVolume.getSizeZ();

  // 1) even line are time-flipped ( -> sizeX - 1 - x )
  // 2) we reorder lines of even slices because k-space lines are acquired from 
  //    ky=-overscan to ky=+kymax/2 in that case. They are stored in the
  //    PFile, using that order ( -> lineCount - 1 - line ). Odd slices are
  //    acquired from ky=+kmax/2 to ky=-overscan, and do not require any line
  //    reordering.

  int32_t x, line, slice;

  std::vector< std::complex< float > > tmpLine( sizeX );
  std::complex< float > tmpX;

  // processing even lines (0, 2, 4, 6, 8, ...)
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line += 2 )
    {

      if ( ( slice % 2 ) == 0 )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          tmpLine[ x ] = rawVolume( x, lineCount - 1 - line, slice );

        }

        for ( x = 0; x < sizeX; x++ )
        {

          rawVolume( sizeX - 1 - x, lineCount - 1 - line, slice ) =
          rawVolume( x, line, slice );

        }

        for ( x = 0; x < sizeX; x++ )
        {

          rawVolume( x, line, slice ) = tmpLine[ x ];

        }

      }
      else
      {

        for ( x = 0; x < sizeX; x++ )
        {
 
          tmpX = rawVolume( sizeX - 1 - x, line, slice );
          rawVolume( sizeX - 1 - x, line, slice ) = rawVolume( x, line, slice );
          rawVolume( x, line, slice ) = tmpX;

        }

      }

    }

  }

}
*/


//
// void reorderingLines()
//

static void reorderingLines( gkg::Volume< std::complex< float > >& rawVolume )
{

  int32_t sizeX = rawVolume.getSizeX();
  int32_t lineCount = rawVolume.getSizeY();
  int32_t sliceCount = rawVolume.getSizeZ();

  // 1) even line are time-flipped ( -> sizeX - 1 - x )

  int32_t x, line, slice;
  std::complex< float > tmpX;

  // processing even lines (0, 2, 4, 6, 8, ...)
  for ( slice = 0; slice < sliceCount; slice++ )
  {

    for ( line = 0; line < lineCount; line += 2 )
    {

        for ( x = 0; x < sizeX / 2; x++ )
        {
 
          tmpX = rawVolume( sizeX - 1 - x, line, slice );
          rawVolume( sizeX - 1 - x, line, slice ) = rawVolume( x, line, slice );
          rawVolume( x, line, slice ) = tmpX;

        }
 
    }

  }

}


#if 0
//
// void mirroring()
//

static void mirroring( gkg::Volume< std::complex< float > >& recVolume )
{

  int32_t sizeX = recVolume.getSizeX();
  int32_t lineCount = recVolume.getSizeY();
  int32_t sliceCount = recVolume.getSizeZ();
  int32_t halfLineCount = lineCount / 2;

  int32_t x, line, slice;

  // Y mirroring
  for ( slice = 1; slice < sliceCount; slice++ )
  {

    for ( x = 0; x < sizeX; x++ )
    {

      for ( line = 0; line < halfLineCount; line++ )
      {

        std::swap( recVolume( x, line, slice ),
                   recVolume( x, lineCount - 1 - line, slice ) );

      }

    }

  }

}
#endif


//
// void copyRaw2Rec()
//

static void copyRaw2Rec( const gkg::Volume< std::complex< float > >& rawVolume,
                         gkg::Volume< std::complex< float > >& recVolume,
                         bool verbose )
{

  try
  {

    int32_t sizeX = rawVolume.getSizeX();
    int32_t lineCount = rawVolume.getSizeY();
    int32_t sliceCount = rawVolume.getSizeZ();

    int32_t x, line, slice;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          recVolume( x, line, slice ) = rawVolume( x, line, slice );

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  catch ( std::exception& e )
  {

    throw std::runtime_error( std::string( "copyRaw2Rec :" ) + e.what() );

  }

}


//
// void addContribution()
//

static void addContribution( const gkg::Volume< float >& magnitudeVolume,
                             gkg::Volume< float >& meanSquareMagnitudeVolume,
                             int32_t coil, bool verbose )
{

  try
  {

    int32_t sizeX = meanSquareMagnitudeVolume.getSizeX();
    int32_t lineCount = meanSquareMagnitudeVolume.getSizeY();
    int32_t sliceCount = meanSquareMagnitudeVolume.getSizeZ();

    int32_t x, line, slice;

    if ( coil == 0 )
    {

      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            meanSquareMagnitudeVolume( x, line, slice ) =
              magnitudeVolume( x, line, slice ) *
              magnitudeVolume( x, line, slice );

          }

        }
        if ( verbose )
        {

          if ( slice != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                    << " / " << std::setw( 4 ) << sliceCount
                    << " ]" << std::flush;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 ) << std::flush;

      }

    }
    else
    {

      for ( slice = 0; slice < sliceCount; slice++ )
      {

        for ( line = 0; line < lineCount; line++ )
        {

          for ( x = 0; x < sizeX; x++ )
          {

            meanSquareMagnitudeVolume( x, line, slice ) +=
              magnitudeVolume( x, line, slice ) *
              magnitudeVolume( x, line, slice );

          }

        }
        if ( verbose )
        {

          if ( slice != 0 )
          {

            std::cout << gkg::Eraser( 21 );

          }
          std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                    << " / " << std::setw( 4 ) << sliceCount
                    << " ]" << std::flush;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 21 ) << std::flush;

      }

    }

  }
  catch ( std::exception& e )
  {

    throw std::runtime_error( std::string( "addContribution :" ) + e.what() );

  }

}


//
// void takeRootOf()
//

static void takeRootOf( gkg::Volume< float >& meanSquareMagnitudeVolume,
                        bool verbose )
{

  try
  {

    int32_t sizeX = meanSquareMagnitudeVolume.getSizeX();
    int32_t lineCount = meanSquareMagnitudeVolume.getSizeY();
    int32_t sliceCount = meanSquareMagnitudeVolume.getSizeZ();

    int32_t x, line, slice;
    for ( slice = 0; slice < sliceCount; slice++ )
    {

      for ( line = 0; line < lineCount; line++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          meanSquareMagnitudeVolume( x, line, slice ) = std::sqrt( 
                                  meanSquareMagnitudeVolume( x, line, slice ) );

        }

      }
      if ( verbose )
      {

        if ( slice != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " slice[ " << std::setw( 4 ) << slice + 1 
                  << " / " << std::setw( 4 ) << sliceCount
                  << " ]" << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 21 ) << std::flush;

    }

  }
  catch ( std::exception& e )
  {

    throw std::runtime_error( std::string( "takeRootOf :" ) + e.what() );

  }

}


//
// class GehcEviReconstructorCommand
//

gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand(
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
  GKG_CATCH( "gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand(
                                     const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     bool saveComplex,
                                     const std::string& xApodizationName,
                                     const std::string& yApodizationName,
                                     const std::string& zApodizationName,
                                     int32_t reconstructionSizeX,
                                     int32_t reconstructionSizeY,
                                     int32_t reconstructionSizeZ,
                                     float scale,
                                     bool noTrajectoryCorrection,
                                     const std::string& phaseCorrectionMethod,
                                     bool keepFirstMTF,
                                     int32_t fitPointCount,
                                     const std::string& outputFormat,
                                     const std::string& fileNamePhaseCorrection,
                                     bool allSliceRefScan,
                                     int32_t pcCoilNumber,
                                     bool multiChannel,
                                     const std::string& fileNamePhaseMap,
                                     bool gradwarp,
                                     const std::string& gradientCoilType,
                                     bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             scale,
             noTrajectoryCorrection,
             phaseCorrectionMethod,
             keepFirstMTF,
             fitPointCount,
             outputFormat,
             fileNamePhaseCorrection,
             allSliceRefScan,
             pcCoilNumber,
             multiChannel,
             fileNamePhaseMap,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool saveComplex, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "const std::string& zApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "int32_t reconstructionSizeZ, "
             "float scale, "
             "bool noTrajectoryCorrection, "
             "const std::string& phaseCorrectionMethod, "
             "bool keepFirstMTF, "
             "int32_t fitPointCount, "
             "const std::string& outputFormat, "
             "const std::string& fileNamePhaseCorrection, "
             "bool allSliceRefScan, "
             "int32_t pcCoilNumber, "
             "bool multiChannel, "
             "const std::string& fileNamePhaseMap, "
             "bool gradwarp, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, saveComplex );
    DECLARE_STRING_PARAMETER( parameters, std::string, xApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, yApodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, zApodizationName );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, reconstructionSizeZ );
    DECLARE_FLOATING_PARAMETER( parameters, float, scale );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, noTrajectoryCorrection );
    DECLARE_STRING_PARAMETER( parameters, std::string, phaseCorrectionMethod );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, keepFirstMTF );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, fitPointCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNamePhaseCorrection );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, allSliceRefScan );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, pcCoilNumber );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, multiChannel );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNamePhaseMap );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, gradwarp );
    DECLARE_STRING_PARAMETER( parameters, std::string, gradientCoilType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             scale,
             noTrajectoryCorrection,
             phaseCorrectionMethod,
             keepFirstMTF,
             fitPointCount,
             outputFormat,
             fileNamePhaseCorrection,
             allSliceRefScan,
             pcCoilNumber,
             multiChannel,
             fileNamePhaseMap,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH( "gkg::GehcEviReconstructorCommand::GehcEviReconstructorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::GehcEviReconstructorCommand::~GehcEviReconstructorCommand()
{
}


std::string gkg::GehcEviReconstructorCommand::getStaticName()
{

  try
  {

    return "GehcEviReconstructor";

  }
  GKG_CATCH( "std::string gkg::GehcEviReconstructorCommand::getStaticName()" );

}


void gkg::GehcEviReconstructorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    bool saveComplex = false;
    std::string xApodizationName = "tukey";
    std::string yApodizationName = "tukey";
    std::string zApodizationName = "tukey";
    int32_t reconstructionSizeX = -1;
    int32_t reconstructionSizeY = -1;
    int32_t reconstructionSizeZ = -1;
    float scale = 0.1;
    bool noTrajectoryCorrection = false;
    std::string phaseCorrectionMethod = "refscan";
    bool keepFirstMTF = false;
    int32_t fitPointCount = 16;
    std::string outputFormat = "gis";
    std::string fileNamePhaseCorrection;
    bool allSliceRefScan = true;
    int32_t pcCoilNumber = -1;
    bool multiChannel = false;
    std::string fileNamePhaseMap;
    bool gradwarp = false;
    std::string gradientCoilType = "CRM";
    bool verbose = false;

    std::string apodizationNames;
    std::list< std::string > apodizationNameList = 
      gkg::ApodizationFactory< float >::getInstance().getNames();
    std::list< std::string >::const_iterator n = apodizationNameList.begin(),
                                             ne = apodizationNameList.end();
    while ( n != ne )
    {

      apodizationNames += *n + '/';
      ++ n;

    }
    apodizationNames = apodizationNames.substr( 0,
                                                apodizationNames.length() - 1 );

    gkg::Application application( _argc, _argv,
                                  "Reconstructor for GEMS Signa 1.5T SHFJ"
                                  " GRE/SE Hyperscan"
                                  " EchoVolumar pulse sequence",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input P-data file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Reconstructed GIS image file name",
                                 fileNameOut );
    application.addSingleOption( "-saveComplex",
                                 "Complex value of reconstructed data is"
                                 " saved instead of magnitude",
                                 saveComplex,
                                 true );
    application.addSingleOption( "-ax",
                                 std::string( "X apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 xApodizationName,
                                 true );
    application.addSingleOption( "-ay",
                                 std::string( "Y apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 yApodizationName,
                                 true );
    application.addSingleOption( "-az",
                                 std::string( "Z apodization (" ) +
                                 apodizationNames + ") " +
                                 "(default=tukey)",
                                 zApodizationName,
                                 true );
    application.addSingleOption( "-sx",
                                 "Reconstruction X size "
                                 "(default=closest upper power of two)",
                                 reconstructionSizeX,
                                 true );
    application.addSingleOption( "-sy",
                                 "Reconstruction Y size "
                                 "(default=closest upper power of two)",
                                 reconstructionSizeY,
                                 true );
    application.addSingleOption( "-sz",
                                 "Reconstruction Z size "
                                 "(default=closest upper power of two)",
                                 reconstructionSizeZ,
                                 true );
    application.addSingleOption( "-scale",
                                 "Scale factor applied to reconstructed data"
                                 " (default=0.1)",
                                 scale,
                                 true );
    application.addSingleOption( "-notrajectory",
                                 "Not correcting for trajectories"
                                 " (default=false)",
                                 noTrajectoryCorrection,
                                 true );
    application.addSingleOption( "-pc",
                                 "Phase correction method (default=refscan): \n"
                                 "- mtf -> Modulation Transfer Function\n"
                                 "- refscan -> Reference Scan\n",
                                 phaseCorrectionMethod,
                                 true );
/*
    application.addSingleOption( "-keepFirstMTF",
                                 "Keep MTF result on the first rank for the "
                                 "following rank(s)",
                                 keepFirstMTF,
                                 true );
*/
    application.addSingleOption( "-fitPointCount",
                                 "Fit point count for reference scan based"
                                  "phase correction (default=16)",
                                 fitPointCount,
                                 true );
    application.addSingleOption( "-allSliceRefScan",
                                 "Estimate reference scan correction over all "
                                 "slice locations (default=all slices)",
                                 allSliceRefScan,
                                 true );
    application.addSingleOption( "-pcCoilNumber",
                                 "Phase correction coil number (default=-1);"
                                 "if set no -1, use all coils",
                                 pcCoilNumber,
                                 true );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-pcfile",
                                 "Output format (default=gis)",
                                 fileNamePhaseCorrection,
                                 true );
    application.addSingleOption( "-multiChannel",
                                 "Reconstruct all channels (default=false)",
                                 multiChannel,
                                 true );
    application.addSingleOption( "-phaseMap",
                                 "Phase map for susceptibility artifact "
                                 "correction",
                                 fileNamePhaseMap,
                                 true );
    application.addSingleOption( "-gw",
                                 "Apply gradient warping correction"
                                 " (default=false)",
                                 gradwarp, 
                                 true );
    application.addSingleOption( "-gradientCoilType",
                                 "GEMS Gradient coil type (default=CRM)\n"
                                 " - BRM\n"
                                 " - CRM\n"
                                 " - TRM.WHOLE\n"
                                 " - TRM.ZOOM\n",
                                 gradientCoilType, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             saveComplex,
             xApodizationName,
             yApodizationName,
             zApodizationName,
             reconstructionSizeX,
             reconstructionSizeY,
             reconstructionSizeZ,
             scale,
             noTrajectoryCorrection,
             phaseCorrectionMethod,
             keepFirstMTF,
             fitPointCount,
             outputFormat,
             fileNamePhaseCorrection,
             allSliceRefScan,
             pcCoilNumber,
             multiChannel,
             fileNamePhaseMap,
             gradwarp,
             gradientCoilType,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::GehcEviReconstructorCommand::parse()" );

}


void gkg::GehcEviReconstructorCommand::execute(
                                     const std::string& fileNameIn,
                                     const std::string& fileNameOut,
                                     bool saveComplex,
                                     const std::string& xApodizationName,
                                     const std::string& yApodizationName,
                                     const std::string& zApodizationName,
                                     int32_t reconstructionSizeX,
                                     int32_t reconstructionSizeY,
                                     int32_t reconstructionSizeZ,
                                     float scale,
                                     bool noTrajectoryCorrection,
                                     const std::string& phaseCorrectionMethod,
                                     bool keepFirstMTF,
                                     int32_t fitPointCount,
                                     const std::string& outputFormat,
                                     const std::string& fileNamePhaseCorrection,
                                     bool allSliceRefScan,
                                     int32_t pcCoilNumber,
                                     bool multiChannel,
                                     const std::string& /* fileNamePhaseMap */,
                                     bool gradwarp,
                                     const std::string& gradientCoilType,
                                     bool verbose )
{

  try
  {

    std::string apodizationNames;
    std::list< std::string > apodizationNameList = 
      gkg::ApodizationFactory< float >::getInstance().getNames();
    std::list< std::string >::const_iterator n = apodizationNameList.begin(),
                                             ne = apodizationNameList.end();
    while ( n != ne )
    {

      apodizationNames += *n + '/';
      ++ n;

    }
    apodizationNames = apodizationNames.substr( 0,
                                                apodizationNames.length() - 1 );

    // forcing some states
    if ( saveComplex )
    {

      multiChannel = true;

    }

    // sanity checks
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         xApodizationName ) )
    {

      throw std::runtime_error(
              std::string( "X apodization should be one of " ) +
              apodizationNames );

    }
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         yApodizationName ) )
    {

      throw std::runtime_error( 
              std::string( "Y apodization should be one of " ) +
              apodizationNames );

    }
    if ( !gkg::ApodizationFactory< float >::getInstance().hasApodization(
                                                         zApodizationName ) )
    {

      throw std::runtime_error( 
              std::string( "Z apodization should be one of " ) +
              apodizationNames );

    }
    if ( pcCoilNumber < -1 )
    {

      throw std::runtime_error(
               "phase correction coil number must be positive or equal to -1" );

    }
    if ( ( gradientCoilType != "BRM" ) &&
         ( gradientCoilType != "CRM" ) &&
         ( gradientCoilType != "TRM.WHOLE" ) &&
         ( gradientCoilType != "TRM.ZOOM" ) )
    {

      throw std::runtime_error( "not a valid gradient coil type" );

    }

    // checking that data is readable and collecting sizes
    if ( verbose )
    {

      std::cout << "reading P-data file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< std::complex< float > > inputVolumeProxy;
    format = gkg::Reader::getInstance().preparePartialBinaryRead<
                                       gkg::Volume< std::complex< float > > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );
    if ( format != "gehc-signa-pfile" )
    {

      throw std::runtime_error( "wrong format, should be \'gehc-signa-pfile\'");

    }

    // check that pulse program is one of the good McKinnon EVI pulse sequence
    std::string psdName = "";
    inputVolumeProxy.getHeader().getAttribute(
                      "pool_header.rdb_hdr_image.psd_iname", psdName );
    if ( ( psdName != "GkgDwSeEvi" ) &&
         ( psdName != "NmrDwSeEvi" ) )
    {

      throw std::runtime_error( "not a GKG EVI pulse sequence");

    }

    // getting field of view and slice thickness
    double fovX = 0.0, fovY = 0.0, fovZ = 0.0, sliceThickness = 0.0;
    float fov, slthick, scanspacing;
    int16_t slquant;
    inputVolumeProxy.getHeader().getAttribute( "pool_header.rdb_hdr_image.dfov",
                                               fov );
    inputVolumeProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slthick",
                                            slthick );
    inputVolumeProxy.getHeader().getAttribute(
                                            "pool_header.rdb_hdr_image.slquant",
                                            slquant );
    inputVolumeProxy.getHeader().getAttribute(
                                        "pool_header.rdb_hdr_image.scanspacing",
                                        scanspacing );
    fovX = fovY = ( double )fov;
    fovZ = (double)( slquant * slthick );
    sliceThickness = ( double )( slthick + scanspacing );

    // collecting coil information
    int16_t start_rcv, stop_rcv;
    inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].start_rcv",
                             start_rcv );
    inputVolumeProxy.getHeader().getAttribute(
                              "pool_header.rdb_hdr_rec.rdb_hdr_dab[0].stop_rcv",
                              stop_rcv );
    int32_t coilCount = ( int32_t )( stop_rcv - start_rcv + 1 );
    if ( pcCoilNumber >= coilCount )
    {

      throw std::runtime_error( 
              std::string( "phase correction coil number must be lower than" ) +
                                  gkg::StringConverter::toString( coilCount ) );

    }

    // collecting size(s)
    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT() /
                    ( multiChannel ? 1 :coilCount );

    // collecting UserCV(s)
    float opuser1, opuser2, opuser3;
    inputVolumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user1", opuser1 );
    inputVolumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user2", opuser2 );
    inputVolumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user3", opuser3 );
    bool spectroSpatial = ( int32_t )( opuser1 + 0.5 );
    int32_t overScanCountY = ( int32_t )( opuser2 + 0.5 ) % 1000;
    int32_t overScanCountZ = ( int32_t )( opuser2 + 0.5 ) / 1000;
    bool rampSamplingOn = ( int32_t )( opuser3 + 0.5 ) / 10;
    bool maxwellTermsCorrection = ( int32_t )( opuser3 + 0.5 ) % 10;

    // collecting parallel reduction factor
    float parallelReductionFactorY = 1.0;
    float parallelReductionFactorZ = 1.0;
    if ( ( psdName == "GkgDwSeEvi" ) ||
         ( psdName == "NmrDwSeEvi" ) )
    {

      float opuser9, opuser10;
      inputVolumeProxy.getHeader().getAttribute(
                                   "pool_header.rdb_hdr_image.user9", opuser9 );
      inputVolumeProxy.getHeader().getAttribute(
                                 "pool_header.rdb_hdr_image.user10", opuser10 );
      if ( opuser9 >= 1.0 )
      {

        parallelReductionFactorY = opuser9;

      }
      if ( opuser10 >= 1.0 )
      {

        parallelReductionFactorZ = opuser10;

      }

    }
    // ATTENTION : Hack pour les toutes premieres acquisitions faites
    parallelReductionFactorY = 1.0f;
    // fin du hack
    fovY /= parallelReductionFactorY;
    fovZ /= parallelReductionFactorZ;

    overScanCountY = ( int32_t )( overScanCountY /  
                                  parallelReductionFactorY );
    overScanCountZ = ( int32_t )( overScanCountZ /  
                                  parallelReductionFactorZ );

    // collecting ramp sampling information if ramp sampling on
    float samplingPeriod = 0.0;
    float gradientAmplitude = 0.0;
    float acquisitionLength = 0.0;
    float rampTime = 0.0;
    float topFlatTime = 0.0;
    float gradientBetaFactor = 0.0;
    int effectiveSizeX = sizeX;

    if ( rampSamplingOn && !noTrajectoryCorrection )
    {

      // collecting ramp sampling information if ramp sampling on
      float rhuser0, rhuser1, rhuser2, rhuser3, rhuser4, rhuser5;
      int32_t vrgfxres;
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user0", rhuser0 );
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user1", rhuser1 );
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user2", rhuser2 );
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user3", rhuser3 );
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user4", rhuser4 );
      inputVolumeProxy.getHeader().getAttribute(
                             "pool_header.rdb_hdr_rec.rdb_hdr_user5", rhuser5 );
      inputVolumeProxy.getHeader().getAttribute(
                         "pool_header.rdb_hdr_rec.rdb_hdr_vrgfxres", vrgfxres );

      samplingPeriod = rhuser0;
      gradientAmplitude = rhuser1;
      acquisitionLength = rhuser2;
      rampTime = rhuser3;
      topFlatTime = rhuser4;
      gradientBetaFactor = rhuser5;
      effectiveSizeX = vrgfxres;

    }

    if ( verbose )
    {

      std::cout << "raw data geometry: "
                << sizeX << " x "
                << lineCount << " x "
                << sliceCount << " x "
                << timeCount << std::endl;
      std::cout << "overscanY count: "
                << overScanCountY
                << std::endl;
      std::cout << "overscanZ count: "
                << overScanCountZ
                << std::endl;
      std::cout << "field of view: "
                << fovX << " mm x "
                << fovY << " mm x "
                << fovZ << " mm"
                << std::endl;
      std::cout << "slice thickness: "
                << slthick << " mm"
                << std::endl;
      std::cout << "coil channel count: "
                << coilCount
                << std::endl;
      std::cout << "Y parallel reduction factor: "
                << parallelReductionFactorY
                << std::endl;
      std::cout << "Z parallel reduction factor: "
                << parallelReductionFactorZ
                << std::endl;
      std::cout << "RF pulse type (opuser1): "
                << getRFPulseTypeName( spectroSpatial )
                << std::endl;
      if ( !noTrajectoryCorrection )
      {

        std::cout << "ramp sampling: "
                  << ( rampSamplingOn ? "on" : "off" )
                  << std::endl;

        if ( rampSamplingOn )
        {

          std::cout << "Sampling period in us: "
                    << samplingPeriod
                    << std::endl;
          std::cout << "Gradient amplitude in G/cm: "
                    << gradientAmplitude
                    << std::endl;
          std::cout << "Acquisition length in us: "
                    << acquisitionLength
                    << std::endl;
          std::cout << "Ramp time in us: "
                    << rampTime
                    << std::endl;
          std::cout << "Top flat time in us: "
                    << topFlatTime
                    << std::endl;
          std::cout << "Gradient beta factor: "
                    << gradientBetaFactor
                    << std::endl;
        /*  std::cout << "Effective X size: "
                    << effectiveSizeX
                    << std::endl;*/

        }

      }
      std::cout << "Maxwell terms correction (opuser3): "
                << ( maxwellTermsCorrection ? "on" : "off" )
                << std::endl;

    }

    // calculating reconstructed image size(s)
    if ( verbose )
    {

      std::cout << "evaluating reconstructed image size(s): " << std::flush;

    }

    if ( reconstructionSizeX == -1 )
    {

      if ( rampSamplingOn )
      {

        reconstructionSizeX = gkg::PowerOfTwo::getInstance().getLowerOrEqual( 
                                                               effectiveSizeX );

      }
      else
      {

        reconstructionSizeX = gkg::PowerOfTwo::getInstance().getGreaterOrEqual(
                                                               effectiveSizeX );

      }

    }
    if ( reconstructionSizeY == -1 )
    {

      reconstructionSizeY = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                           ( lineCount - overScanCountY ) * 2 );

    }
    reconstructionSizeY = std::max( reconstructionSizeY,
                                    ( lineCount - overScanCountY ) * 2 );
    if ( reconstructionSizeZ == -1 )
    {

      reconstructionSizeZ = gkg::PowerOfTwo::getInstance().getGreaterOrEqual( 
                                          ( sliceCount - overScanCountZ ) * 2 );

    }
    reconstructionSizeZ = std::max( reconstructionSizeZ,
                                    ( sliceCount - overScanCountZ ) * 2 );
    if ( verbose )
    {

      std::cout << reconstructionSizeX << " x "
                << reconstructionSizeY << " x "
                << reconstructionSizeZ << " x "
                << timeCount << std::endl;

    }

    // creating apodization
    if ( verbose )
    {

      std::cout << "creating apodization(s): " << std::flush;

    }
    int32_t regridingSizeX = ( sizeX <= reconstructionSizeX ? sizeX :
                                                          reconstructionSizeX );
    int32_t xApodizationSize = regridingSizeX;
    int32_t yApodizationSize = ( lineCount - overScanCountY ) * 2;
    int32_t zApodizationSize = ( sliceCount - overScanCountZ ) * 2;
    gkg::Apodization< float >* xApodization =
      gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          xApodizationName,
                                          xApodizationSize,
                                          gkg::Apodization< float >::Centered );
    gkg::Apodization< float >* yApodization =
        gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          yApodizationName,
                                          yApodizationSize,
                                          gkg::Apodization< float >::Centered );
    gkg::Apodization< float >* zApodization =
        gkg::ApodizationFactory< float >::getInstance().createApodization(
                                          zApodizationName,
                                          zApodizationSize,
                                          gkg::Apodization< float >::Centered );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int32_t > inputBoundingBox( 0, sizeX - 1,
                                                  0, lineCount - 1,
                                                  0, sliceCount - 1,
                                                  0, 0 );
    gkg::Volume< std::complex< float > > rawVolume;

    gkg::Volume< std::complex< float > > recVolume( reconstructionSizeX,
                                                    reconstructionSizeY,
                                                    reconstructionSizeZ );
    gkg::Volume< float > realPartVolume;
    gkg::Volume< float > magnitudeVolume;
    gkg::Volume< float > meanSquareMagnitudeVolume;
    gkg::Volume< int16_t > shortVolume;

    recVolume.getHeader().addAttribute( "resolutionX",
                                        fovX / reconstructionSizeX );
    recVolume.getHeader().addAttribute( "resolutionY",
                                        fovY / reconstructionSizeY );
    recVolume.getHeader().addAttribute( "resolutionZ",
                                        sliceThickness );
    recVolume.getHeader().addAttribute( "resolutionT",
                                        ( double )1.0 );

    if ( !saveComplex )
    {

      realPartVolume = gkg::Volume< float >( reconstructionSizeX,
                                             reconstructionSizeY,
                                             reconstructionSizeZ );
      magnitudeVolume = gkg::Volume< float >( reconstructionSizeX,
                                              reconstructionSizeY,
                                              reconstructionSizeZ );
      meanSquareMagnitudeVolume = gkg::Volume< float >( reconstructionSizeX,
                                                        reconstructionSizeY,
                                                        reconstructionSizeZ );
      shortVolume = gkg::Volume< int16_t >( reconstructionSizeX,
                                            reconstructionSizeY,
                                            reconstructionSizeZ );

    }

    // preparing converter(s)
    gkg::RealPartConverter< gkg::Volume< std::complex< float > >,
                            gkg::Volume< float > > realPartConverter;
    gkg::MagnitudeConverter< gkg::Volume< float >,
                             gkg::Volume< float > > magnitudeRealConverter;
    gkg::MagnitudeConverter< gkg::Volume< std::complex< float > >,
                             gkg::Volume< float > > magnitudeCplxConverter;
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


    // preparing modulation transfer function(s)
    std::vector< std::vector< std::vector< std::complex< float > > > >
      modulationTF( ( multiChannel ? 1 : coilCount ) );

    // preparing refscan phase correction
    std::vector< std::vector< std::vector< float > > >
      eviConstantPhaseCorrection( ( multiChannel ? 1 : coilCount ) );
    std::vector< std::vector< std::vector< float > > >
      eviLinearPhaseCorrection( ( multiChannel ? 1 : coilCount ) );

    // preparing partial write proxy
    if ( verbose )
    {

      std::cout << "preparing partial write in \'"
                << outputFormat
                << "\' format: "
                << std::flush;

    }


    gkg::BoundingBox< int32_t > outputBoundingBox( 0, reconstructionSizeX - 1,
                                                   0, reconstructionSizeY - 1,
                                                   0, reconstructionSizeZ - 1,
                                                   0, 0 );
    gkg::VolumeProxy< int16_t > outputVolumeProxy;
    gkg::VolumeProxy< std::complex< float > > outputCplxVolumeProxy;

    if ( saveComplex )
    {

      outputCplxVolumeProxy = gkg::VolumeProxy< std::complex< float > > (
                                                          reconstructionSizeX,
                                                          reconstructionSizeY,
                                                          reconstructionSizeZ,
                                                          timeCount );
      outputCplxVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
      outputCplxVolumeProxy.getHeader()[ "item_type" ] =
        gkg::TypeOf< std::complex< float > >::getName();
      outputCplxVolumeProxy.getHeader()[ "sizeX" ] = reconstructionSizeX;
      outputCplxVolumeProxy.getHeader()[ "sizeY" ] = reconstructionSizeY;
      outputCplxVolumeProxy.getHeader()[ "sizeZ" ] = reconstructionSizeZ;
      outputCplxVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionX",
                                                   fovX / reconstructionSizeX );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionY",
                                                   fovY / reconstructionSizeY );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                                      sliceThickness );
      outputCplxVolumeProxy.getHeader().addAttribute( "resolutionT",
                                                      ( double )1.0 );
      if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                  gkg::Volume< std::complex< float > > >( fileNameOut,
                                                          outputCplxVolumeProxy,
                                                          outputFormat ) !=
           outputFormat )
      {

        throw std::runtime_error(
              std::string( "failed to prepare partial write with format \' " ) +
              outputFormat + "\'" );

      }

    }
    else
    {

      outputVolumeProxy = gkg::VolumeProxy< int16_t > ( reconstructionSizeX,
                                                        reconstructionSizeY,
                                                        reconstructionSizeZ,
                                                        timeCount );
      outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
      outputVolumeProxy.getHeader()[ "item_type" ] =
        gkg::TypeOf< int16_t >::getName();
      outputVolumeProxy.getHeader()[ "sizeX" ] = reconstructionSizeX;
      outputVolumeProxy.getHeader()[ "sizeY" ] = reconstructionSizeY;
      outputVolumeProxy.getHeader()[ "sizeZ" ] = reconstructionSizeZ;
      outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
      outputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                                  fovX / reconstructionSizeX );
      outputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                                  fovY / reconstructionSizeY );
      outputVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                                  sliceThickness );
      outputVolumeProxy.getHeader().addAttribute( "resolutionT",
                                                  ( double )1.0 );
      if ( gkg::Writer::getInstance().preparePartialBinaryWrite< 
                                   gkg::Volume< int16_t > >( fileNameOut,
                                                             outputVolumeProxy,
                                                             outputFormat ) !=
           outputFormat )
      {

        throw std::runtime_error(
              std::string( "failed to prepare partial write with format \' " ) +
              outputFormat + "\'" );

      }

    }


    if ( verbose )
    {

      std::cout << "done " << std::endl;

    }

    // preparing regriding correction
    gkg::RegridingCorrection< float >* regridingCorrection = 0;
    if ( !rampSamplingOn )
    {

      if ( regridingSizeX != sizeX )
      {

        if ( verbose )
        {

          std::cout << "preparing sinc regriding correction: "
                    << std::flush;

        }
        regridingCorrection = new gkg::RegridingCorrection< float >( sizeX );
        regridingCorrection->setOutputSampleCount( regridingSizeX,
                                                   ( float )effectiveSizeX );
        if ( verbose )
        {

          std::cout << "done " << std::endl;
          std::cout << "Effective X size: " << effectiveSizeX << std::endl;

        }

      }

    }
    else
    {

      if ( !noTrajectoryCorrection )
      {

        if ( verbose )
        {

          std::cout << "preparing ramp sampling correction: "
                    << std::flush;

        }
        gkg::TrapezoidROGradient< float > roGradient( sizeX,
                                                      rampTime,
                                                      topFlatTime );
        regridingCorrection = new gkg::RegridingCorrection< float >(
                                                   roGradient.getTrajectory() );
        regridingCorrection->setOutputSampleCount( regridingSizeX,
                                                   ( float )effectiveSizeX );
        if ( verbose )
        {

          std::cout << "done " << std::endl;
          std::cout << "Effective X size: " << effectiveSizeX << std::endl;

        }

      }
      else
      {

        if ( regridingSizeX != sizeX )
        {

          if ( verbose )
          {

            std::cout << "preparing sinc regriding correction: "
                      << std::flush;

          }
          regridingCorrection = new gkg::RegridingCorrection< float >( sizeX );
          regridingCorrection->setOutputSampleCount( regridingSizeX,
                                                     ( float )effectiveSizeX );
          if ( verbose )
          {

            std::cout << "done " << std::endl;
            std::cout << "Effective X size: " << effectiveSizeX << std::endl;

          }

        }

      }

    }

    // y and z position of k-space center
    float bestKy = ( float )( lineCount - overScanCountY ) + 0.5;
    float bestKz = ( float )( sliceCount - overScanCountZ ) + 0.5;

    if ( !fileNamePhaseCorrection.empty() && verbose )
    {

      std::cout << "best k-space Y origin : " << bestKy << std::endl;
      std::cout << "best k-space Z origin : " << bestKz << std::endl;

    }

    if ( verbose )
    {

      std::cout << "phase correction method : "
                << phaseCorrectionMethod << std::endl;

    }

    // preparing some basic things
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setXApodization(
                                                                 xApodization );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setYApodization(
                                                                 yApodization );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setZApodization(
                                                                 zApodization );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setOverScanCountY(
                                                               overScanCountY );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setOverScanCountZ(
                                                               overScanCountZ );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setRawLineCount(
                                                                    lineCount );
    gkg::HalfKSpaceReconstructor3d< float >::getInstance().setRawSliceCount(
                                                                   sliceCount );

    // estimating gradient warping correction 
    std::vector< gkg::GradientWarping2dCorrection< float >::Wedge2d >
        gradientWarpingWedge2ds;
    bool swapXY = false;
    if ( gradwarp && !saveComplex )
    {

      if ( verbose )
      {

        std::cout << "preparing gradient warping correction : " << std::flush;

      }

      std::string gradientCoilName =
        gkg::ConfigurationInfo::getInstance().getGkgPath() +
        gkg::getDirectorySeparator() + "gehc" +
        gkg::getDirectorySeparator() + "gradwarp" +
        gkg::getDirectorySeparator() + "gw_coils.dat." + gradientCoilType;
      gkg::GehcSignaGradientCoil< float > gradientCoil( gradientCoilName );

      float start_loc, end_loc;
      inputVolumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.start_loc", start_loc );
      inputVolumeProxy.getHeader().getAttribute(
                            "pool_header.rdb_hdr_series.end_loc", end_loc );
      bool reverseZ = ( start_loc < end_loc );

      std::vector< gkg::GradientWarping2dCorrection< float >::Plane >
        planes( sliceCount );
      int32_t s, trueSlice;
      for ( s = 0; s < sliceCount; s++ )
      {

        trueSlice = ( reverseZ ? sliceCount - 1 - s : s );

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[0]",
            planes[ trueSlice ].point1.x );
        planes[ trueSlice ].point1.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[1]",
            planes[ trueSlice ].point1.y );
        planes[ trueSlice ].point1.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point1[2]",
            planes[ trueSlice ].point1.z );
        planes[ trueSlice ].point1.z /= 10.0;

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[0]",
            planes[ trueSlice ].point2.x );
        planes[ trueSlice ].point2.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[1]",
            planes[ trueSlice ].point2.y );
        planes[ trueSlice ].point2.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point2[2]",
            planes[ trueSlice ].point2.z );
        planes[ trueSlice ].point2.z /= 10.0;

        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[0]",
            planes[ trueSlice ].point3.x );
        planes[ trueSlice ].point3.x /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[1]",
            planes[ trueSlice ].point3.y );
        planes[ trueSlice ].point3.y /= 10.0;
        inputVolumeProxy.getHeader().getAttribute(
            std::string( "pool_header.rdb_hdr_data_acq_tab[" ) +
            gkg::StringConverter::toString( s ) +
            "].gw_point3[2]",
            planes[ trueSlice ].point3.z );
        planes[ trueSlice ].point3.z /= 10.0;

      }

      // detecting swap between X and Y
      if ( inputVolumeProxy.getHeader().hasAttribute(
             "pool_header.rdb_hdr_image.freq_dir" ) )
      {

        int16_t freq_dir;
        inputVolumeProxy.getHeader().getAttribute(
                                           "pool_header.rdb_hdr_image.freq_dir",
                                           freq_dir );
        swapXY = ( freq_dir == 1 ? true : false );

      }

      gkg::GradientWarping2dCorrection< float >::getInstance().estimate(
                                                        gradientCoil,
                                                        planes,
                                                        gradientWarpingWedge2ds,
                                                        reconstructionSizeX,
                                                        reconstructionSizeY,
                                                        verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // looping over ranks(s)
    if ( verbose )
    {

      std::cout << "reconstructing: "
                << std::flush;

    }

    int32_t line, slice, time, coil, phaseCorrectionCoil;
    std::complex< float > dcOffset( 0.0, 0.0 );
    for ( time = 0; time < timeCount; time++ )
    {

      if ( verbose )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      for ( coil = 0; coil < ( multiChannel ? 1 : coilCount ); coil++ )
      {

        // reading raw data from P-file
        inputBoundingBox.setLowerT( time * ( multiChannel ? 1 : coilCount ) +
                                    coil );
        inputBoundingBox.setUpperT( time * ( multiChannel ? 1 : coilCount ) +
                                    coil );
        format =
          gkg::Reader::getInstance().partialBinaryRead<
                                         gkg::Volume< std::complex< float > > >(
                                                         inputVolumeProxy,
                                                         rawVolume,
                                                         inputBoundingBox,
                                                         "gehc-signa-pfile" );
        if ( format != "gehc-signa-pfile" )
        {

          throw std::runtime_error(
                                "wrong format, should be \'gehc-signa-pfile\'");

        }


        // reordering raw data, and zero-filling
        if ( verbose )
        {

          std::cout << "reordering lines" << std::flush;

        }
        reorderingLines( rawVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 16 ) << std::flush;

        }

        // removing constant part of complex signal (DC offset correction)
        if ( verbose )
        {

          std::cout << "DcOffset->" << std::flush;

        }
        gkg::DcOffsetCorrection< float >::getInstance().apply( 
                                                           rawVolume,
                                                           sizeX,
                                                           lineCount,
                                                           sliceCount,
                                                           &dcOffset );
        if ( verbose )
        {

          std::cout << std:: setw( 30 ) << dcOffset
                    << gkg::Eraser( 40 ) << std::flush;

        }

        // correction for ramp sampling if ramp sampling on
        recVolume.fill( std::complex< float >( 0.0, 0.0 ) );
        if ( regridingCorrection )
        {

          if ( verbose )
          {

            std::cout << "Regriding->" << std::flush;

          }
          regridingCorrection->apply( rawVolume, recVolume, verbose );
          if ( verbose )
          {

            std::cout << gkg::Eraser( 11 ) << std::flush;

          }

        }
        else
        {

          if ( verbose )
          {

            std::cout << "Copying->" << std::flush;

          }
          copyRaw2Rec( rawVolume, recVolume, verbose );
          if ( verbose )
          {

            std::cout << gkg::Eraser( 9 ) << std::flush;

          }

        }

        // estimating phase correction function(s)
        if ( phaseCorrectionMethod == "mtf" )
        {

          if ( !keepFirstMTF || ( keepFirstMTF && ( time == 0 ) ) )
          {

            if ( verbose )
            {

              std::cout << "MTF estimation->" << std::flush;

            }
            gkg::ModulationTransferFunction< float >::getInstance().estimate(
                                                           recVolume,
                                                           modulationTF[ coil ],
                                                           0.0,
                                                           verbose );
            if ( verbose )
            {

              std::cout << gkg::Eraser( 16 ) << std::flush;

            }

          }

        }
        else if ( phaseCorrectionMethod == "refscan" )
        {

          if ( time == 0  )
          {

            if ( verbose )
            {

              std::cout << "Ahn Phase estimation->" << std::flush;

            }
            std::vector< std::vector< float > > constantPhaseCorrection;
            std::vector< std::vector< float > > linearPhaseCorrection;
            gkg::AhnPhaseCorrection3d< float >::Type refScanType =
                          ( allSliceRefScan ?
                            gkg::AhnPhaseCorrection3d< float >::ALL_SLICES :
                            gkg::AhnPhaseCorrection3d< float >::CENTRAL_SLICE );
            gkg::AhnPhaseCorrection3d< float >::getInstance().estimate(
                                                        recVolume,
                                                        lineCount,
                                                        sliceCount,
                                                        constantPhaseCorrection,
                                                        linearPhaseCorrection,
                                                        refScanType,
                                                        verbose );
            if ( verbose )
            {

              std::cout << gkg::Eraser( 22 ) << std::flush;

            }

            if ( verbose )
            {

              std::cout << "EVI Phase Correction->" << std::flush;

            }
            std::vector< int32_t > flip( lineCount * sliceCount );
            for ( line = 0; line < lineCount * sliceCount; line++ )
            {

              flip[ line ] = ( line % 2 ? -1 : +1 );

            }
            gkg::EchoVolumarPhaseCorrection< float >::getInstance().estimate(
                                             constantPhaseCorrection,
                                             linearPhaseCorrection,
                                             lineCount,
                                             sliceCount,
                                             eviConstantPhaseCorrection[ coil ],
                                             eviLinearPhaseCorrection[ coil ],
                                             verbose,
                                             flip,
                                             bestKy,
                                             bestKz,
                                             fitPointCount,
                                             2,
                                             2 );
            if ( verbose )
            {

              std::cout << gkg::Eraser( 22 ) << std::flush;

            }

            if ( !fileNamePhaseCorrection.empty() )
            {

              std::ofstream os;
              os.open( ( fileNamePhaseCorrection + "_coil" +
                         gkg::StringConverter::toString( coil ) +
                         ".txt" ).c_str() );
              for ( slice = 0; slice < sliceCount; slice++ )
              {

                for ( line = 0; line < lineCount; line++ )
                {

                  os << line << " "
                     << constantPhaseCorrection[ slice ][ line ]
                     << " "
                     << linearPhaseCorrection[ slice ][ line ]
                     << " "
                     << eviConstantPhaseCorrection[ coil ][ slice ][ line ]
                     << " "
                     << eviLinearPhaseCorrection[ coil ][ slice ][ line ]
                     << std::endl;

                }

              }
              os.close();

            }

          }

        }

        // reconstructing from Half K-space
        if ( verbose )
        {

          std::cout << "Fourier->" << std::flush;

        }

        if ( ( !multiChannel && ( time != 0 ) ) ||
             ( multiChannel && ( time >= coilCount ) ) )
        {

          if ( pcCoilNumber != -1 )
          {

            phaseCorrectionCoil = pcCoilNumber;

          }
          else
          {

            phaseCorrectionCoil = coil;

          }

        }
        else
        {

          phaseCorrectionCoil = coil;

        }

        if ( phaseCorrectionMethod == "mtf" )
        {

          gkg::HalfKSpaceReconstructor3d< float >::getInstance().
                                               setModulationTransferFunction(
                                          modulationTF[ phaseCorrectionCoil ] );

        }
        else if ( phaseCorrectionMethod == "refscan" )
        {

          gkg::HalfKSpaceReconstructor3d< float >::getInstance().
                                                   setPhaseCorrection(
                              eviConstantPhaseCorrection[ phaseCorrectionCoil ],
                              eviLinearPhaseCorrection[ phaseCorrectionCoil ] );

        }

        gkg::HalfKSpaceReconstructor3d< float >::getInstance().reconstruct(
                                                                      recVolume,
                                                                      recVolume,
                                                                      verbose );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 9 ) << std::flush;

        }

#if 0
        // mirroring according to Y axis
        if ( verbose )
        {

          std::cout << "Y mirroring" << std::flush;

        }
        mirroring( recVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 11 ) << std::flush;

        }
#endif

        // taking magnitude of real or complex part
        if ( !saveComplex )
        {

          if ( overScanCountY > 0.35 * lineCount && 
               overScanCountZ > 0.35 * sliceCount ) // no use of Half-Fourier
          {

            if ( verbose )
            {

              std::cout << "Magnitude cplx part" << std::flush;

            }
            magnitudeCplxConverter.convert( recVolume, magnitudeVolume );
            if ( verbose )
            {

              std::cout << gkg::Eraser( 19 ) << std::flush;

            }

          }
          else
          {

            if ( verbose )
            {

              std::cout << "Magnitude real part" << std::flush;

            }
            realPartConverter.convert( recVolume, realPartVolume );
            magnitudeRealConverter.convert( realPartVolume, magnitudeVolume );
            if ( verbose )
            {

              std::cout << gkg::Eraser( 19 ) << std::flush;

            }

          }
          addContribution( magnitudeVolume, meanSquareMagnitudeVolume, coil,
                           verbose );

        }

      }

      // further things to do if not complex 
      if ( !saveComplex )
      {

        // taking mean square root
        takeRootOf( meanSquareMagnitudeVolume, verbose );

        // applying 2d gradient warping
        if ( gradwarp )
        {

          if ( verbose )
          {

            std::cout << "Gradient warping correction" << std::flush;

          }
          gkg::GradientWarping2dCorrection< float >::getInstance().apply(
                                                      meanSquareMagnitudeVolume,
                                                      gradientWarpingWedge2ds,
                                                      swapXY,
                                                      verbose );
          if ( verbose )
          {

            std::cout << gkg::Eraser( 27 ) << std::flush;

          }

        }

        // rescaling and converting to short
        if ( verbose )
        {

          std::cout << "Rescaling+short convertion" << std::flush;

        }
        scaler.scale( meanSquareMagnitudeVolume );
        lowerThresholder.threshold( meanSquareMagnitudeVolume,
                                    meanSquareMagnitudeVolume );
        greaterThresholder.threshold( meanSquareMagnitudeVolume,
                                      meanSquareMagnitudeVolume );
        converter.convert( meanSquareMagnitudeVolume, shortVolume );
        if ( verbose )
        {

          std::cout << gkg::Eraser( 26 ) << std::flush;

        }

      }

      // saving reconstructed slice to disk
      if ( verbose )
      {

        std::cout << "Saving to disk" << std::flush;

      }
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );

      if ( saveComplex )
      {

        gkg::Writer::getInstance().partialBinaryWrite<
                  gkg::Volume< std::complex< float > > >( outputCplxVolumeProxy,
                                                          recVolume,
                                                          outputBoundingBox,
                                                          outputFormat );

      }
      else
      {

        gkg::Writer::getInstance().partialBinaryWrite< 
                                                      gkg::Volume< int16_t > >(
                                                            outputVolumeProxy,
                                                            shortVolume,
                                                            outputBoundingBox,
                                                            outputFormat );

      }

      if ( verbose )
      {

        std::cout << gkg::Eraser( 14 ) << std::flush;

      }

    }
    if ( verbose )
    {

      std::cout << gkg::Eraser( 20 ) << "done" << std::endl;

    }

    // deleting allocated object
    delete xApodization;
    delete yApodization;
    delete zApodization;
    if ( regridingCorrection )
    {

      delete regridingCorrection;

    }

  }
  GKG_CATCH( "void gkg::GehcEviReconstructorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "bool saveComplex, "
             "const std::string& xApodizationName, "
             "const std::string& yApodizationName, "
             "const std::string& zApodizationName, "
             "int32_t reconstructionSizeX, "
             "int32_t reconstructionSizeY, "
             "int32_t reconstructionSizeZ, "
             "float scale, "
             "bool noTrajectoryCorrection, "
             "const std::string& phaseCorrectionMethod, "
             "bool keepFirstMTF, "
             "int32_t fitPointCount, "
             "const std::string& outputFormat, "
             "const std::string& fileNamePhaseCorrection, "
             "bool allSliceRefScan, "
             "int32_t pcCoilNumber, "
             "bool multiChannel, "
             "const std::string& fileNamePhaseMap, "
             "bool gradwarp, "
             "const std::string& gradientCoilType, "
             "bool verbose  )" );

}


RegisterCommandCreator(
    GehcEviReconstructorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( saveComplex ) +
    DECLARE_STRING_PARAMETER_HELP( xApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( yApodizationName ) +
    DECLARE_STRING_PARAMETER_HELP( zApodizationName ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeX ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeY ) +
    DECLARE_INTEGER_PARAMETER_HELP( reconstructionSizeZ ) +
    DECLARE_FLOATING_PARAMETER_HELP( scale ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( noTrajectoryCorrection ) +
    DECLARE_STRING_PARAMETER_HELP( phaseCorrectionMethod ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( keepFirstMTF ) +
    DECLARE_INTEGER_PARAMETER_HELP( fitPointCount ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePhaseCorrection ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( allSliceRefScan ) +
    DECLARE_INTEGER_PARAMETER_HELP( pcCoilNumber ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( multiChannel ) +
    DECLARE_STRING_PARAMETER_HELP( fileNamePhaseMap ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( gradwarp ) +
    DECLARE_STRING_PARAMETER_HELP( gradientCoilType ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
