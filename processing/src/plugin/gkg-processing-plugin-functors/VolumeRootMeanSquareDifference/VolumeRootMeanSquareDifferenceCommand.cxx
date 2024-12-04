#include <gkg-processing-plugin-functors/VolumeRootMeanSquareDifference/VolumeRootMeanSquareDifferenceCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-io/MaskReaderProcess.h>
#include <gkg-processing-algobase/Combiner_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <string>
#include <cmath>
#include <iostream>


gkg::VolumeRootMeanSquareDifferenceCommand::
                                          VolumeRootMeanSquareDifferenceCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                           : gkg::Command( argc,
                                                           argv,
                                                           loadPlugin,
                                                           removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::VolumeRootMeanSquareDifferenceCommand::VolumeRootMeanSquareDifferenceCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeRootMeanSquareDifferenceCommand::
                                          VolumeRootMeanSquareDifferenceCommand(
                                  const std::string& fileNameVolumeReference,
                                  const std::string& fileNameVolumeTest,
                                  const std::string& fileNameAbsoluteDifference,
                                  const std::string& fileNameSignedDifference,
                                  const std::string& fileNameMask,
                                  const std::string& mode,
                                  double threshold1,
                                  double threshold2,
                                  bool ascii,
                                  const std::string& format,
                                  bool verbose )
{

  try
  {

    execute( fileNameVolumeReference,
             fileNameVolumeTest,
             fileNameAbsoluteDifference,
             fileNameSignedDifference,
             fileNameMask,
             mode,
             threshold1,
             threshold2,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::VolumeRootMeanSquareDifferenceCommand::"
             "VolumeRootMeanSquareDifferenceCommand( "
             "const std::string& fileNameVolumeReference, "
             "const std::string& fileNameVolumeTest, "
             "const std::string& fileNameAbsoluteDifference, "
             "const std::string& fileNameSignedDifference, "
             "const std::string& fileNameMask, "
             "const std::string& mode, "
             "double threshold1, "
             "double threshold2, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose" );

}


gkg::VolumeRootMeanSquareDifferenceCommand::
                                          VolumeRootMeanSquareDifferenceCommand(
                                             const gkg::Dictionary& parameters )
                                          : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameVolumeReference );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeTest );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameAbsoluteDifference );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string, fileNameSignedDifference );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, threshold2 );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameVolumeReference,
             fileNameVolumeTest,
             fileNameAbsoluteDifference,
             fileNameSignedDifference,
             fileNameMask,
             mode,
             threshold1,
             threshold2,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::VolumeRootMeanSquareDifferenceCommand::"
             "VolumeRootMeanSquareDifferenceCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeRootMeanSquareDifferenceCommand::
                                        ~VolumeRootMeanSquareDifferenceCommand()
{
}


std::string gkg::VolumeRootMeanSquareDifferenceCommand::getStaticName()
{

  try
  {

    return "VolumeRootMeanSquareDifference";

  }
  GKG_CATCH( "std::string "
             "gkg::VolumeRootMeanSquareDifferenceCommand::getStaticName()" );

}


void gkg::VolumeRootMeanSquareDifferenceCommand::parse()
{

  try
  {

    std::string fileNameVolumeReference;
    std::string fileNameVolumeTest;
    std::string fileNameAbsoluteDifference;
    std::string fileNameSignedDifference;
    std::string fileNameMask;
    std::string mode = "gt";
    double threshold1 = 0.0;
    double threshold2 = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application(
                     _argc, _argv,
                     "Computes root mean square difference between two volumes",
                     _loadPlugin );
    application.addSingleOption( "-r",
                                 "Reference volume file name",
                                 fileNameVolumeReference );
    application.addSingleOption( "-t",
                                 "Test volume file name",
                                 fileNameVolumeTest );
    application.addSingleOption( "-oa",
                                 "Output absolute difference volume file "
                                 "name",
                                 fileNameAbsoluteDifference );
    application.addSingleOption( "-os",
                                 "Output signed difference volume file "
                                 "name",
                                 fileNameSignedDifference );
    application.addSingleOption( "-M",
                                 "Mask volume file name",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-m",
                                 "Threshold masking mode (default=gt)\n"
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
                                 mode,
                                 true );
    application.addSingleOption( "-t1",
                                 "First threshold (default=0.0)",
                                 threshold1,
                                 true );
    application.addSingleOption( "-t2",
                                 "Second threshold (required for be/bt/oe/ou) "
                                 "(default=0.0)",
                                 threshold2,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( fileNameVolumeReference,
             fileNameVolumeTest,
             fileNameAbsoluteDifference,
             fileNameSignedDifference,
             fileNameMask,
             mode,
             threshold1,
             threshold2,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::VolumeRootMeanSquareDifferenceCommand::parse()" );

}


void gkg::VolumeRootMeanSquareDifferenceCommand::execute(
                                  const std::string& fileNameVolumeReference,
                                  const std::string& fileNameVolumeTest,
                                  const std::string& fileNameAbsoluteDifference,
                                  const std::string& fileNameSignedDifference,
                                  const std::string& fileNameMask,
                                  const std::string& mode,
                                  double threshold1,
                                  double threshold2,
                                  bool ascii,
                                  const std::string& format,
                                  bool verbose )
{

  try
  {

    //
    // sanity check(s)
    //
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

      throw std::runtime_error( "invalid masking mode" );

    }


    //
    // reading reference volume
    //
    if ( verbose )
    {

      std::cout << "reading '" << fileNameVolumeReference << "' : "
                << std::flush;

    }

    gkg::Volume< float > volumeReference;
    gkg::TypedVolumeReaderProcess< float >
      referenceVolumeReaderProcess( volumeReference );
    referenceVolumeReaderProcess.execute( fileNameVolumeReference );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // reading reference volume
    //
    if ( verbose )
    {

      std::cout << "reading '" << fileNameVolumeTest << "' : "
                << std::flush;

    }

    gkg::Volume< float > volumeTest;
    gkg::TypedVolumeReaderProcess< float >
      testVolumeReaderProcess( volumeTest );
    testVolumeReaderProcess.execute( fileNameVolumeTest );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // sanity checks()
    //
    if ( !volumeTest.hasSameSize( volumeReference ) )
    {

      throw std::runtime_error(
                             "incompatible reference and test volume size(s)" );

    }
    if ( !volumeTest.hasSameResolution( volumeReference ) )
    {

      throw std::runtime_error(
                       "incompatible reference and test volume resolution(s)" );

    }


    //
    // reading mask volume
    //
    gkg::Volume< int8_t > volumeMask;
    if ( !fileNameMask.empty() )
    {

      gkg::MaskReaderProcess maskReaderProcess( volumeMask,
                                                mode,
                                                threshold1,
                                                threshold2,
                                                verbose );
      maskReaderProcess.execute( fileNameMask );

      //
      // sanity checks()
      //
      if ( !volumeMask.hasSameSize( volumeReference ) )
      {

        throw std::runtime_error(
                             "incompatible reference and mask volume size(s)" );

      }
      if ( !volumeMask.hasSameResolution( volumeReference, true ) )
      {

        throw std::runtime_error(
                       "incompatible reference and mask volume resolution(s)" );

      }

    }



    //
    // computing absolute & signed difference volume
    //
    if ( verbose )
    {

      std::cout << "computing root mean square difference : " << std::flush;

    }

    gkg::Volume< float > absoluteDifferenceVolume( volumeReference.getSizeX(),
                                                   volumeReference.getSizeY(),
                                                   volumeReference.getSizeZ(),
                                                   volumeReference.getSizeT() );
    gkg::Volume< float > signedDifferenceVolume( volumeReference.getSizeX(),
                                                 volumeReference.getSizeY(),
                                                 volumeReference.getSizeZ(),
                                                 volumeReference.getSizeT() );
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    double resolutionT = 1.0;
    volumeReference.getResolution( resolution, resolutionT );
    absoluteDifferenceVolume.setResolution( resolution,resolutionT );
    absoluteDifferenceVolume.fill( 0.0f );
    signedDifferenceVolume.setResolution( resolution,resolutionT );
    signedDifferenceVolume.fill( 0.0f );

    gkg::Volume< float >::const_iterator r = volumeReference.begin(),
                                         re = volumeReference.end();
    gkg::Volume< float >::const_iterator t = volumeTest.begin();
    gkg::Volume< float >::iterator ad = absoluteDifferenceVolume.begin();
    gkg::Volume< float >::iterator sd = signedDifferenceVolume.begin();

    if ( fileNameMask.empty() )
    {

      float squareDifference = 0.0f;
      double sumOfSquareDifference = 0.0f;
      double rootMeanSquareDifference = 0.0f;
      double voxelCount = 0.0f;
      while ( r != re )
      {

        *sd = *r - *t;
        *ad = std::abs( *sd );
        squareDifference = *ad * *ad;
        sumOfSquareDifference += squareDifference;
        voxelCount += 1.0;

        ++ r;
        ++ t;
        ++ ad;
        ++ sd;

      }

      rootMeanSquareDifference = std::sqrt( sumOfSquareDifference /
                                            voxelCount );

      absoluteDifferenceVolume.getHeader().addAttribute(
                                                  "root_mean_square_difference",
                                                  rootMeanSquareDifference );
      signedDifferenceVolume.getHeader().addAttribute(
                                                  "root_mean_square_difference",
                                                  rootMeanSquareDifference );

      if ( verbose )
      {

        std::cout << rootMeanSquareDifference << std::endl;

      }

    }
    else
    {

      gkg::Volume< int8_t >::const_iterator m = volumeMask.begin();

      float squareDifference = 0.0f;
      double sumOfSquareDifference = 0.0f;
      double rootMeanSquareDifference = 0.0f;
      double voxelCount = 0.0f;
      while ( r != re )
      {

 
        if ( *m )
        {

          *sd = *r - *t;
          *ad = std::abs( *sd );
          squareDifference = *ad * *ad;
          sumOfSquareDifference += squareDifference;
          voxelCount += 1.0;

        }

        ++ r;
        ++ t;
        ++ m;
        ++ ad;
        ++ sd;

      }

      if ( voxelCount > 0.0f )
      {

        rootMeanSquareDifference = std::sqrt( sumOfSquareDifference /
                                              voxelCount );

      }

      absoluteDifferenceVolume.getHeader().addAttribute(
                                                  "root_mean_square_difference",
                                                  rootMeanSquareDifference );

      if ( verbose )
      {

        std::cout << rootMeanSquareDifference << std::endl;

      }

    }


    //
    // writing absolute difference volume
    //
    if ( verbose )
    {

      std::cout << "writing absolute difference volume : " << std::flush;

    }

      gkg::Writer::getInstance().write( fileNameAbsoluteDifference,
                                        absoluteDifferenceVolume,
                                        ascii,
                                        format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // writing signed difference volume
    //
    if ( verbose )
    {

      std::cout << "writing signed difference volume : " << std::flush;

    }

      gkg::Writer::getInstance().write( fileNameSignedDifference,
                                        signedDifferenceVolume,
                                        ascii,
                                        format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::VolumeRootMeanSquareDifferenceCommand::execute( "
             "const std::string& fileNameVolumeReference, "
             "const std::string& fileNameVolumeTest, "
             "const std::string& fileNameAbsoluteDifference, "
             "const std::string& fileNameSignedDifference, "
             "const std::string& fileNameMask, "
             "const std::string& mode, "
             "double threshold1, "
             "double threshold2, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
                   VolumeRootMeanSquareDifferenceCommand,
    		   DECLARE_STRING_PARAMETER_HELP( fileNameVolumeReference ) +
    		   DECLARE_STRING_PARAMETER_HELP( fileNameVolumeTest ) +
    		   DECLARE_STRING_PARAMETER_HELP( fileNameAbsoluteDifference ) +
    		   DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    		   DECLARE_STRING_PARAMETER_HELP( mode ) +
    		   DECLARE_FLOATING_PARAMETER_HELP( threshold1 ) +
    		   DECLARE_FLOATING_PARAMETER_HELP( threshold2 ) +
    		   DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		   DECLARE_STRING_PARAMETER_HELP( format ) +
    		   DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
