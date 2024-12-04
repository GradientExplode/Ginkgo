#include <gkg-dmri-plugin-functors/DwiDiffusionKernel/DwiDiffusionKernelCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-diffusion-kernel/DiffusionKernel.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand(
                                       const std::string& fileNameT2,
                                       const std::string& fileNameDW,
                                       const std::string& fileNameMask,
                                       const std::string& fileNameOutput,
                                       const std::string& volumeFormat,
                                       int32_t voxelCount,
                                       float fractionalAnisotropyLowerThreshold,
                                       float fractionalAnisotropyUpperThreshold,
                                       float radius,
                                       float thresholdRatio,
                                       bool verbose,
                                       bool ascii )
                               : gkg::Command()
{

  try
  {

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameOutput,
             volumeFormat,
             voxelCount,
             fractionalAnisotropyLowerThreshold,
             fractionalAnisotropyUpperThreshold,
             radius,
             thresholdRatio,
             verbose,
             ascii );

  }
  GKG_CATCH( "gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "const std::string& volumeFormat, "
             "int32_t voxelCount, "
             "float fractionalAnisotropyLowerThreshold, "
             "float fractionalAnisotropyUpperThreshold, "
             "float radius, "
             "float thresholdRatio, "
             "bool verbose, "
             "bool ascii )" );

}


gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameT2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDW );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, voxelCount );
    DECLARE_FLOATING_PARAMETER( parameters, float,
                                fractionalAnisotropyLowerThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, float,
                                fractionalAnisotropyUpperThreshold );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_FLOATING_PARAMETER( parameters, float, thresholdRatio );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameOutput,
             volumeFormat,
             voxelCount,
             fractionalAnisotropyLowerThreshold,
             fractionalAnisotropyUpperThreshold,
             radius,
             thresholdRatio,
             verbose,
             ascii );

  }
  GKG_CATCH( "gkg::DwiDiffusionKernelCommand::DwiDiffusionKernelCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiDiffusionKernelCommand::~DwiDiffusionKernelCommand()
{
}


std::string gkg::DwiDiffusionKernelCommand::getStaticName()
{

  try
  {

    return "DwiDiffusionKernel";

  }
  GKG_CATCH( "std::string gkg::DwiDiffusionKernelCommand::getStaticName()" );

}


void gkg::DwiDiffusionKernelCommand::parse()
{

  try
  {

    std::string fileNameT2;
    std::string fileNameDW;
    std::string fileNameMask = "";
    std::string fileNameOutput = "voxelUsedVolume";
    std::string volumeFormat = "gis";    
    int32_t voxelCount = 300;
    float fractionalAnisotropyLowerThreshold = 0.65;
    float fractionalAnisotropyUpperThreshold = 0.85;
    float radius = -1.0;
    float thresholdRatio = 0.04;
    bool verbose = false;
    bool ascii = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, 
				  "Diffusion Kernel Estimation",
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
    application.addSingleOption( "-voxelCount",
                                 "Voxel count to be visited (default=300)",
                                 voxelCount,
				 true);
    application.addSingleOption( "-faLow",
                                 "Threshold for lowest Fractional Anisotropy"
                                 " measure (default=0.65)",
				 fractionalAnisotropyLowerThreshold,
				 true);
    application.addSingleOption( "-faUp",
                                 "Threshold for highest Fractional Anisotropy"
                                 " measure (default=0.85)",
				 fractionalAnisotropyUpperThreshold,
				 true);
    application.addSingleOption( "-o",
                                 "Output file name with voxels used",
                                 fileNameOutput );
    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );    
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameT2,
             fileNameDW,
             fileNameMask,
             fileNameOutput,
             volumeFormat,
             voxelCount,
             fractionalAnisotropyLowerThreshold,
             fractionalAnisotropyUpperThreshold,
             radius,
             thresholdRatio,
             verbose,
             ascii );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiDiffusionKernelCommand::parse()" );

}


void gkg::DwiDiffusionKernelCommand::execute(
                                       const std::string& fileNameT2,
                                       const std::string& fileNameDW,
                                       const std::string& fileNameMask,
                                       const std::string& fileNameOutput,
                                       const std::string& /* volumeFormat */,
                                       int32_t voxelCount,
                                       float fractionalAnisotropyLowerThreshold,
                                       float fractionalAnisotropyUpperThreshold,
                                       float radius,
                                       float thresholdRatio,
                                       bool verbose,
                                       bool ascii )
{

  try
  {

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
    // processing and saving voxels used for Diffusion Kernel estimation
    ////////////////////////////////////////////////////////////////////////////

    gkg::RCPointer< gkg::TestFunction< float > >
      fractionalAnisotropyTestFunction(
        gkg::TestFunctionFactory< float>::getInstance().createTestFunction(
                                         "be",
                                         fractionalAnisotropyLowerThreshold,
                                         fractionalAnisotropyUpperThreshold ) );
        
    gkg::DiffusionKernel 
      symmetricAverageKernel(
                      t2,
                      dw,
                      mask,
                      fractionalAnisotropyTestFunction, 
                      gkg::DiffusionKernel::DecreasingFractionalAnisotropyOrder,
                      voxelCount,
                      gkg::DiffusionKernel::SymmetricTensor,
                      verbose );    
    
    const gkg::Tensor& tensor = symmetricAverageKernel.getTensor();
    
    if ( verbose )
    {
      
	gkg::Vector eigenValues = tensor.getEigenValues();
	float fa = tensor.getFractionalAnisotropy();
      
 	std::cout << "symmetric average tensor profile is " 
		  << eigenValues( 0 ) << "," 
		  << eigenValues( 1 ) << "," 
		  << eigenValues( 2 ) << " m^2/s"
                  << std::endl;
	std::cout << "Kernel ratio is : " << eigenValues(1)/eigenValues(0) 
		  << " " ;    
	std::cout << "and FA is : " << fa << "\n" ;
	
	std::cout << "saving voxel used to compute diffusion kernel '" 
		<< fileNameOutput << "' : "
                << std::flush;

    }
    gkg::Volume< uint8_t > selectedVoxelVolume = 
      symmetricAverageKernel.getSelectedVoxelVolume();

    gkg::Writer::getInstance().write(
				  fileNameOutput,
				  selectedVoxelVolume,
                                  ascii );
    if ( verbose )
    {
      
      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiDiffusionKernelCommand::execute( "
             "const std::string& fileNameT2, "
             "const std::string& fileNameDW, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOutput, "
             "const std::string& volumeFormat, "
             "int32_t voxelCount, "
             "float fractionalAnisotropyLowerThreshold, "
             "float fractionalAnisotropyUpperThreshold, "
             "float radius, "
             "float thresholdRatio, "
             "bool verbose, "
             "bool ascii )" );

}


RegisterCommandCreator( 
    DwiDiffusionKernelCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameT2 ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDW ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_INTEGER_PARAMETER_HELP( voxelCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( fractionalAnisotropyLowerThreshold ) +
    DECLARE_FLOATING_PARAMETER_HELP( fractionalAnisotropyUpperThreshold ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( thresholdRatio ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
