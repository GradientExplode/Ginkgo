#include <gkg-processing-plugin-functors/FistaDenoiserDeblurrer/FistaDenoiserDeblurrerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-denoising-deblurring/FastIterativeShrinkageThresholdingAlgorithm_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>



class FistaDenoiserDeblurrerProcess : public gkg::Process
{

  public:

    FistaDenoiserDeblurrerProcess(
                                  const std::string& theFileNameMask,
                                  const std::string& theFileNameOut,
                                  const std::vector< float >& theKernel3d,
                                  const std::vector< int32_t >& theKernel3dSize,
                                  const std::string& theMode,
                                  double theThreshold1,
                                  double theThreshold2,
                                  double theBackground,
                                  int32_t theMaximumIterationCount,
                                  double theTolerance,
                                  double theShrinkageStep,
                                  int32_t theSkrinkageMaximumIterationCount,
                                  double theShrinkageTolerance,
                                  bool theAscii,
                                  const std::string& theFormat,
                                  bool theVerbose );
 
    const std::string& fileNameMask;
    const std::string& fileNameOut;
    const std::vector< float >& kernel3d;
    const std::vector< int32_t >& kernel3dSize;
    const std::string& mode;
    double threshold1;
    double threshold2;
    double background;
    int32_t maximumIterationCount;
    double tolerance;
    double shrinkageStep;
    int32_t skrinkageMaximumIterationCount;
    double shrinkageTolerance;
    bool ascii;
    const std::string& format;
    bool verbose;

  private:

    template < class T >
    static void process( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


FistaDenoiserDeblurrerProcess::FistaDenoiserDeblurrerProcess( 
                                  const std::string& theFileNameMask,
                                  const std::string& theFileNameOut,
                                  const std::vector< float >& theKernel3d,
                                  const std::vector< int32_t >& theKernel3dSize,
                                  const std::string& theMode,
                                  double theThreshold1,
                                  double theThreshold2,
                                  double theBackground,
                                  int32_t theMaximumIterationCount,
                                  double theTolerance,
                                  double theShrinkageStep,
                                  int32_t theSkrinkageMaximumIterationCount,
                                  double theShrinkageTolerance,
                                  bool theAscii,
                                  const std::string& theFormat,
                                  bool theVerbose )
                              : gkg::Process( "Volume" ),
                                fileNameMask( theFileNameMask ),
                                fileNameOut( theFileNameOut ),
                                kernel3d( theKernel3d ),
                                kernel3dSize( theKernel3dSize ),
                                mode( theMode ),
                                threshold1( theThreshold1 ),
                                threshold2( theThreshold2 ),
                                background( theBackground ),
                                maximumIterationCount(
                                                     theMaximumIterationCount ),
                                tolerance( theTolerance ),
                                shrinkageStep( theShrinkageStep ),
                                skrinkageMaximumIterationCount(
                                            theSkrinkageMaximumIterationCount ),
                                shrinkageTolerance( theShrinkageTolerance ),
                                ascii( theAscii ),
                                format( theFormat ),
                                verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &FistaDenoiserDeblurrerProcess::process< double > );

}


template < class T >
void FistaDenoiserDeblurrerProcess::process( gkg::Process& process,
                                             const std::string& fileNameIn,
                                             const gkg::AnalyzedObject&,
                                             const std::string& )
{

  try
  {

    FistaDenoiserDeblurrerProcess&
      fistaDenoiserDeblurrerProcess = 
                       static_cast< FistaDenoiserDeblurrerProcess& >( process );

    const std::string& fileNameMask =
      fistaDenoiserDeblurrerProcess.fileNameMask;
    const std::string& fileNameOut =
      fistaDenoiserDeblurrerProcess.fileNameOut;
    const std::vector< float >& kernel3d =
      fistaDenoiserDeblurrerProcess.kernel3d;
    const std::vector< int32_t >& kernel3dSize =
      fistaDenoiserDeblurrerProcess.kernel3dSize;
    const std::string& mode =
      fistaDenoiserDeblurrerProcess.mode;
    double threshold1 =
      fistaDenoiserDeblurrerProcess.threshold1;
    double threshold2 =
      fistaDenoiserDeblurrerProcess.threshold2;
    double background =
      fistaDenoiserDeblurrerProcess.background;
    int32_t maximumIterationCount =
      fistaDenoiserDeblurrerProcess.maximumIterationCount;
    double tolerance =
      fistaDenoiserDeblurrerProcess.tolerance;
    double shrinkageStep =
      fistaDenoiserDeblurrerProcess.shrinkageStep;
    int32_t skrinkageMaximumIterationCount =
      fistaDenoiserDeblurrerProcess.skrinkageMaximumIterationCount;
    double shrinkageTolerance =
      fistaDenoiserDeblurrerProcess.shrinkageTolerance;
    bool ascii =
      fistaDenoiserDeblurrerProcess.ascii;
    const std::string& format =
      fistaDenoiserDeblurrerProcess.format;
    bool verbose =
      fistaDenoiserDeblurrerProcess.verbose;

    ////////////////////////////////////////////////////////////////////////////
    // reading input volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mask volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Volume< int16_t > volumeMask;
    try
    {

      gkg::Reader::getInstance().read( fileNameMask, volumeMask );

    }
    catch ( std::exception& )
    {

      try
      {

        gkg::Volume< uint8_t > volumeMaskUInt8;

        gkg::Reader::getInstance().read( fileNameMask, volumeMaskUInt8 );
        gkg::Vector3d< int32_t > maskSize( 1, 1, 1 );
        volumeMaskUInt8.getSize( maskSize );
        gkg::Vector3d< int32_t > voxel;

        volumeMask.reallocate( maskSize );

        for ( voxel.z = 0; voxel.z < maskSize.z; voxel.z++ )
        {

          for ( voxel.y = 0; voxel.y < maskSize.y; voxel.y++ )
          {

            for ( voxel.x = 0; voxel.x < maskSize.x; voxel.x++ )
            {

              volumeMask( voxel ) = ( int16_t )volumeMaskUInt8( voxel );

            }

          }

        }

      }
      catch ( std::exception& thing )
      {

        throw std::runtime_error( "unable to read mask" );

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building kernel volume
    ////////////////////////////////////////////////////////////////////////////
    gkg::Volume< double > volumeKernel( kernel3dSize[ 0 ],
                                        kernel3dSize[ 1 ],
                                        kernel3dSize[ 2 ] );
    int32_t x, y, z, count = 0;
    for ( z = 0; z < kernel3dSize[ 2 ]; z++ )
    {

      for ( y = 0; y < kernel3dSize[ 1 ]; y++ )
      {

        for ( x = 0; x < kernel3dSize[ 0 ]; x++ )
        {

          volumeKernel( x, y, z ) =  kernel3d[ count ];
          ++ count;

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // building test function according to provided mode
    ////////////////////////////////////////////////////////////////////////////
    gkg::TestFunction< int16_t >* testFunction =
      gkg::TestFunctionFactory< int16_t >::getInstance().createTestFunction( 
                                                                   mode,
	                                                           threshold1,
	                                                           threshold2 );


    ////////////////////////////////////////////////////////////////////////////
    // applying FISTA denoiser / deblurrer
    ////////////////////////////////////////////////////////////////////////////
 
    if ( verbose )
    {

      std::cout << "applying FISTA denoiser / deblurrer : " << std::flush;

    }

    gkg::Volume< T > volumeOut;
    

    gkg::FastIterativeShrinkageThresholdingAlgorithm< T, int16_t, double >
      fastIterativeShrinkageThresholdingAlgorithm;
    fastIterativeShrinkageThresholdingAlgorithm.solve(
                                                 volumeIn,
                                                 volumeMask,
                                                 volumeKernel,
                                                 *testFunction,
                                                 background,
                                                 maximumIterationCount,
                                                 tolerance,
                                                 shrinkageStep,
                                                 skrinkageMaximumIterationCount,
                                                 shrinkageTolerance,
                                                 volumeOut );
                                                  
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameOut << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut,
                                      volumeOut,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    delete testFunction;

  }
  GKG_CATCH( "template < class T > "
             "void FistaDenoiserDeblurrerProcess::binarize( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class FistaDenoiserDeblurrerCommand
//


gkg::FistaDenoiserDeblurrerCommand::FistaDenoiserDeblurrerCommand(
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
  GKG_CATCH( "gkg::FistaDenoiserDeblurrerCommand::FistaDenoiserDeblurrerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::FistaDenoiserDeblurrerCommand::FistaDenoiserDeblurrerCommand(
                          	     const std::string& fileNameIn,
                                     const std::string& fileNameMask,
                                     const std::string& fileNameOut,
                                     const std::vector< float >& kernel3d,
                                     const std::vector< int32_t >& kernel3dSize,
                                     const std::string& mode,
                                     double threshold1,
                                     double threshold2,
                                     double background,
                                     int32_t maximumIterationCount,
                                     double tolerance,
                                     double shrinkageStep,
                                     int32_t skrinkageMaximumIterationCount,
                                     double shrinkageTolerance,
                                     bool ascii,
                                     const std::string& format,
                                     bool verbose )
{

  try
  {

    execute( fileNameIn,
             fileNameMask,
             fileNameOut,
             kernel3d,
             kernel3dSize,
             mode,
             threshold1,
             threshold2,
             background,
             maximumIterationCount,
             tolerance,
             shrinkageStep,
             skrinkageMaximumIterationCount,
             shrinkageTolerance,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::FistaDenoiserDeblurrerCommand::"
             "FistaDenoiserDeblurrerCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOut, "
             "const std::vector< float >& kernel3d, "
             "const std::vector< int32_t >& kernel3dSize, "
             "const std::string& mode, "
             "double threshold1, "
             "double threshold2, "
             "double background, "
             "int32_t maximumIterationCount, "
             "double tolerance, "
             "double shrinkageStep, "
             "int32_t skrinkageMaximumIterationCount, "
             "double shrinkageTolerance, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::FistaDenoiserDeblurrerCommand::FistaDenoiserDeblurrerCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >, 
                                           kernel3d );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, 
                                          std::vector< int32_t >, 
                                          kernel3dSize );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, background );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, maximumIterationCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, tolerance );
    DECLARE_FLOATING_PARAMETER( parameters, double, shrinkageStep );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t,
                               skrinkageMaximumIterationCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, shrinkageTolerance );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameMask,
             fileNameOut,
             kernel3d,
             kernel3dSize,
             mode,
             threshold1,
             threshold2,
             background,
             maximumIterationCount,
             tolerance,
             shrinkageStep,
             skrinkageMaximumIterationCount,
             shrinkageTolerance,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::FistaDenoiserDeblurrerCommand::FistaDenoiserDeblurrerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::FistaDenoiserDeblurrerCommand::~FistaDenoiserDeblurrerCommand()
{
}


std::string gkg::FistaDenoiserDeblurrerCommand::getStaticName()
{

  try
  {

    return "FistaDenoiserDeblurrer";

  }
  GKG_CATCH( "std::string gkg::FistaDenoiserDeblurrerCommand::getStaticName()" );

}


void gkg::FistaDenoiserDeblurrerCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameMask;
    std::string fileNameOut;
    std::vector< float > kernel3d;
    std::vector< int32_t > kernel3dSize;
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double background = 0.0;
    int32_t maximumIterationCount = 1000;
    double tolerance = 1e-4;
    double shrinkageStep = 0.01;
    int32_t skrinkageMaximumIterationCount = 1000;
    double shrinkageTolerance = 1e-4;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "FISTA-based denoiser/deblurrer",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameIn );
    application.addSingleOption( "-M",
                                 "Mask volume file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOut );
    application.addSeriesOption( "-k",
                                 "3D convolution kernel; size is provided by -s"
                                 " option",
                                 kernel3d,
                                 1 );
    application.addSeriesOption( "-s",
                                 "Size of the 3D convolution kernel",
                                 kernel3dSize,
                                 3, 3 );
    application.addSingleOption( "-m",
                                 "Threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n",
                                 mode );
    application.addSingleOption( "-t1",
                                 "First threshold",
                                 threshold1 );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
                                 true );
    application.addSingleOption( "-maximumIterationCount",
                                 "Maximum iteration count  (default=1000)",
                                 maximumIterationCount,
                                 true );
    application.addSingleOption( "-tolerance",
                                 "Tolerance (default=1e-4)",
                                 tolerance,
                                 true );
    application.addSingleOption( "-shrinkageStep",
                                 "Shrinkage step (default=0.01)",
                                 shrinkageStep,
                                 true );
    application.addSingleOption( "-skrinkageMaximumIterationCount",
                                 "Shrinkage maximum iteration count "
                                 "(default=1000)",
                                 skrinkageMaximumIterationCount,
                                 true );
    application.addSingleOption( "-shrinkageTolerance",
                                 "Shrinkage tolerance (default=1e-4)",
                                 shrinkageTolerance,
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

    execute( fileNameIn,
             fileNameMask,
             fileNameOut,
             kernel3d,
             kernel3dSize,
             mode,
             threshold1,
             threshold2,
             background,
             maximumIterationCount,
             tolerance,
             shrinkageStep,
             skrinkageMaximumIterationCount,
             shrinkageTolerance,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::FistaDenoiserDeblurrerCommand::parse()" );

}


void gkg::FistaDenoiserDeblurrerCommand::execute(
                                     const std::string& fileNameIn,
                                     const std::string& fileNameMask,
                                     const std::string& fileNameOut,
                                     const std::vector< float >& kernel3d,
                                     const std::vector< int32_t >& kernel3dSize,
                                     const std::string& mode,
                                     double threshold1,
                                     double threshold2,
                                     double background,
                                     int32_t maximumIterationCount,
                                     double tolerance,
                                     double shrinkageStep,
                                     int32_t skrinkageMaximumIterationCount,
                                     double shrinkageTolerance,
                                     bool ascii,
                                     const std::string& format,
                                     bool verbose )
{

  try
  {

    // sanity checks
    if ( ( int32_t )kernel3d.size() != ( kernel3dSize[ 0 ] *
                                         kernel3dSize[ 1 ] *
                                         kernel3dSize[ 2 ] ) )
    {

      throw std::runtime_error(
                           "inconsistent 3D kernel size(s) and element count" );

    }

    if ( ( mode != "lt" ) &&
         ( mode != "le" ) &&
         ( mode != "eq" ) &&
         ( mode != "di" ) &&
         ( mode != "ge" ) &&
         ( mode != "gt" ) &&
         ( mode != "be" ) &&
         ( mode != "bt" ) &&
         ( mode != "oe" ) &&
         ( mode != "ou" ) )
    {

      throw std::runtime_error( "invalid test mode" );

    }


    if ( maximumIterationCount < 0 )
    {

      throw std::runtime_error(
                            "maximumIterationCount must be strictly positive" );

    }
    if ( tolerance < 0 )
    {

      throw std::runtime_error( "tolerance must be strictly positive" );

    }
    if ( shrinkageStep < 0 )
    {

      throw std::runtime_error( "shrinkageStep must be strictly positive" );

    }
    if ( skrinkageMaximumIterationCount < 0 )
    {

      throw std::runtime_error(
                   "skrinkageMaximumIterationCount must be strictly positive" );

    }
    if ( shrinkageTolerance < 0 )
    {

      throw std::runtime_error(
                               "shrinkageTolerance must be strictly positive" );

    }

    //
    // launching FISTA denoising / deblurring process
    //
    FistaDenoiserDeblurrerProcess 
      fistaDenoiserDeblurrerProcess( fileNameMask,
                                     fileNameOut,
                                     kernel3d,
                                     kernel3dSize,
                                     mode,
                                     threshold1,
                                     threshold2,
                                     background,
                                     maximumIterationCount,
                                     tolerance,
                                     shrinkageStep,
                                     skrinkageMaximumIterationCount,
                                     shrinkageTolerance,
                                     ascii,
                                     format,
                                     verbose );

    fistaDenoiserDeblurrerProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::FistaDenoiserDeblurrerCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "std::vector< float >& kernelX, "
             "std::vector< float >& kernelY, "
             "std::vector< float >& kernelZ, "
             "const std::vector< float >& kernel3d, "
             "const std::vector< int32_t >& kernel3dSize, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
    FistaDenoiserDeblurrerCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( kernel3d ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( kernel3dSize ) +
    DECLARE_STRING_PARAMETER_HELP( mode ) +
    DECLARE_FLOATING_PARAMETER_HELP(threshold1 ) +
    DECLARE_FLOATING_PARAMETER_HELP(threshold2 ) +
    DECLARE_FLOATING_PARAMETER_HELP(background ) +
    DECLARE_INTEGER_PARAMETER_HELP( maximumIterationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( tolerance ) +
    DECLARE_FLOATING_PARAMETER_HELP( shrinkageStep ) +
    DECLARE_INTEGER_PARAMETER_HELP( skrinkageMaximumIterationCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( shrinkageTolerance ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_STRING_PARAMETER_HELP( format ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
