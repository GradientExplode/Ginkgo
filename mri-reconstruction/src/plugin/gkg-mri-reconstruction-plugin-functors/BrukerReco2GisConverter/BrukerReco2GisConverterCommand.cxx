#include <gkg-mri-reconstruction-plugin-functors/BrukerReco2GisConverter/BrukerReco2GisConverterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>


//
// class BrukerRecoReadProcess
//

class BrukerRecoReadProcess : public gkg::Process
{

  public:

    BrukerRecoReadProcess( const std::string& fileNameOut,
                 const std::string& outputFormat,
                 bool verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutputFormat() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void read( gkg::Process& process,
                      const std::string& fileNameIn,
                      const gkg::AnalyzedObject&,
                      const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outputFormat;
    bool _verbose;

};


BrukerRecoReadProcess::BrukerRecoReadProcess( const std::string& fileNameOut,
                                              const std::string& outputFormat,
                                              bool verbose )
                      : gkg::Process( "Volume" ),
                        _fileNameOut( fileNameOut ),
                        _outputFormat( outputFormat ),
                        _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &BrukerRecoReadProcess::read< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &BrukerRecoReadProcess::read< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &BrukerRecoReadProcess::read< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &BrukerRecoReadProcess::read< float > );

}


const std::string& BrukerRecoReadProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& BrukerRecoReadProcess::getOutputFormat() const
{

  return _outputFormat;

}


bool BrukerRecoReadProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void BrukerRecoReadProcess::read( gkg::Process& process,
                                  const std::string& fileNameIn,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    BrukerRecoReadProcess&
      readProcess = static_cast< BrukerRecoReadProcess& >( process );

    // checking that data is readable and collecting sizes
    if ( readProcess.getVerbose() )
    {

      std::cout << "reading Bruker Reco file header: "
                << fileNameIn << std::endl;

    }
    std::string format;
    gkg::VolumeProxy< T > inputVolumeProxy;
    format = gkg::Reader::getInstance().template preparePartialBinaryRead<
                                                           gkg::Volume< T > >(
                                                             fileNameIn,
                                                             inputVolumeProxy );

    if ( format != "bruker-reco" )
    {

      throw std::runtime_error( "wrong format, should be \'bruker-reco\'");

    }

    // check that pulse program is managed
    std::string pulseProgramName = "";
    inputVolumeProxy.getHeader().getAttribute( "PULPROG", pulseProgramName );
    if ( pulseProgramName == "fh_inepi.ppg" )  // old PV1 sequence from F.Hennel
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, slthick = 0.0;
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ", slthick );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( ( pulseProgramName == "fh_ge3d.ppg" ) || // old PV1 3D sequence
              ( pulseProgramName == "anat_T1.ppg" ) || // old PV1 3D sequence
              ( pulseProgramName == "anat_T1_301.ppg" ) )  // new PV302 3D seq.
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, fovZ = 0.0;
      std::vector< double > fov( 3 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );

      std::string ACQ_slice_orient;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_orient",
                                                 ACQ_slice_orient );

      if ( ACQ_slice_orient == "Transverse_Posterior_Anterior" )
      {

        fovX = fov[ 1 ] * 10.0;
        fovY = fov[ 0 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }
      else if ( ACQ_slice_orient == "Arbitrary_Oblique" )
      {

        fovX = fov[ 1 ] * 10.0;
        fovY = fov[ 0 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }
      else
      {

        fovX = fov[ 0 ] * 10.0;
        fovY = fov[ 1 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                           fovZ / inputVolumeProxy.getSizeZ() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( ( pulseProgramName == "fieldmap.ppg" ) )  // new PV302 3D B0 map
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, fovZ = 0.0;
      std::vector< double > fov( 3 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );

      std::string ACQ_slice_orient;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_orient",
                                                 ACQ_slice_orient );

      if ( ACQ_slice_orient == "Transverse_Posterior_Anterior" )
      {

        fovX = fov[ 1 ] * 10.0;
        fovY = fov[ 0 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }
      else if ( ACQ_slice_orient == "Arbitrary_Oblique" )
      {

        fovX = fov[ 1 ] * 10.0;
        fovY = fov[ 0 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }
      else
      {

        fovX = fov[ 0 ] * 10.0;
        fovY = fov[ 1 ] * 10.0;
        fovZ = fov[ 2 ] * 10.0;

      }

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ",
                                           fovZ / inputVolumeProxy.getSizeZ() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( pulseProgramName == "epi_shfj.ppg" )
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, slthick = 0.0;
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ", slthick );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( pulseProgramName == "mecaDTI1128.ppg" )
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, slthick = 0.0;
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ", slthick );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( pulseProgramName == "DTI_shfj.ppg" )
    {

      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, slthick = 0.0;
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ", slthick );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else if ( !pulseProgramName.empty() )
    {
      // getting field of view and slice thickness
      double fovX = 0.0, fovY = 0.0, slthick = 0.0;
      std::vector< double > fov( 2 );
      inputVolumeProxy.getHeader().getAttribute( "RECO_fov", fov );
      fovX = fov[ 0 ] * 10.0;
      fovY = fov[ 1 ] * 10.0;
      inputVolumeProxy.getHeader().getAttribute( "ACQ_slice_thick", slthick );

      // adding resolution(s)
      inputVolumeProxy.getHeader().addAttribute( "resolutionX",
                                           fovX / inputVolumeProxy.getSizeX() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionY",
                                           fovY / inputVolumeProxy.getSizeY() );
      inputVolumeProxy.getHeader().addAttribute( "resolutionZ", slthick );
      inputVolumeProxy.getHeader().addAttribute( "resolutionT", ( double )1.0 );

    }
    else
    {

      throw std::runtime_error( "not a managed pulse program");

    }

    int32_t sizeX = inputVolumeProxy.getSizeX();
    int32_t lineCount = inputVolumeProxy.getSizeY();
    int32_t sliceCount = inputVolumeProxy.getSizeZ();
    int32_t timeCount = inputVolumeProxy.getSizeT();

    // collecting resolution(s)
    double resolutionX;
    double resolutionY;
    double resolutionZ;
    double resolutionT;
    inputVolumeProxy.getHeader().getAttribute( "resolutionX", resolutionX );
    inputVolumeProxy.getHeader().getAttribute( "resolutionY", resolutionY );
    inputVolumeProxy.getHeader().getAttribute( "resolutionZ", resolutionZ );
    inputVolumeProxy.getHeader().getAttribute( "resolutionT", resolutionT );


    // preparing a bounding box and a slice data for slice reading
    gkg::BoundingBox< int > inputBoundingBox( 0, sizeX - 1,
                                              0, lineCount - 1,
                                              0, sliceCount - 1,
                                              0, 0 );
    gkg::Volume< T > inputSubVolume( sizeX, lineCount, sliceCount );

    // preparing partial write proxy
    if ( readProcess.getVerbose() )
    {

      std::cout << "preparing partial write in \'"
                << readProcess.getOutputFormat()
                << "\' format: "
                << std::flush;

    }
    gkg::VolumeProxy< T >
      outputVolumeProxy( sizeX, lineCount, sliceCount, timeCount );
    outputVolumeProxy.getHeader() = inputVolumeProxy.getHeader();
    outputVolumeProxy.getHeader()[ "item_type" ] =
                                gkg::TypeOf< T >::getName();
    outputVolumeProxy.getHeader()[ "sizeX" ] = sizeX;
    outputVolumeProxy.getHeader()[ "sizeY" ] = lineCount;
    outputVolumeProxy.getHeader()[ "sizeZ" ] = sliceCount;
    outputVolumeProxy.getHeader()[ "sizeT" ] = timeCount;
    outputVolumeProxy.getHeader()[ "resolutionX" ] = resolutionX;
    outputVolumeProxy.getHeader()[ "resolutionY" ] = resolutionY;
    outputVolumeProxy.getHeader()[ "resolutionZ" ] = resolutionZ;
    outputVolumeProxy.getHeader()[ "resolutionT" ] = resolutionT;

    gkg::BoundingBox< int32_t > outputBoundingBox( 0, sizeX - 1,
                                                   0, lineCount - 1,
                                                   0, sliceCount - 1,
                                                   0, 0 );

    if ( gkg::Writer::getInstance().template preparePartialBinaryWrite< 
                                              gkg::Volume< T > >(
                                              readProcess.getFileNameOut(),
                                              outputVolumeProxy,
                                              readProcess.getOutputFormat() ) !=
         readProcess.getOutputFormat() )
    {

      throw std::runtime_error(
      std::string( "failed to prepare partial write with format \' " ) +
      readProcess.getOutputFormat() + "\'" );

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << "done " << std::endl;

    }

    // looping over ranks(s)
    if ( readProcess.getVerbose() )
    {

      std::cout << "converting: "
                << std::flush;

    }

    int32_t time;
    for ( time = 0; time < timeCount; time++ )
    {

      if ( readProcess.getVerbose() )
      {

        if ( time != 0 )
        {

          std::cout << gkg::Eraser( 20 );

        }
        std::cout << "rank[ " << std::setw( 4 ) << time + 1 
                  << " / " << std::setw( 4 ) << timeCount
                  << " ] " << std::flush;

      }

      // reading reco data from 2dsequ Bruker file
      inputBoundingBox.setLowerT( time );
      inputBoundingBox.setUpperT( time );
      format =
        gkg::Reader::getInstance().template partialBinaryRead<
                                             gkg::Volume< T > >(
                                                               inputVolumeProxy,
                                                               inputSubVolume,
                                                               inputBoundingBox,
                                                               "bruker-reco" );
      if ( format != "bruker-reco" )
      {

        throw std::runtime_error( "wrong format, should be \'bruker-reco\'");

      }

      // saving sub-volume(s) to disk
      outputBoundingBox.setLowerT( time );
      outputBoundingBox.setUpperT( time );
      gkg::Writer::getInstance().template partialBinaryWrite<
                                             gkg::Volume< T > >(
                                                outputVolumeProxy,
                                                inputSubVolume,
                                                outputBoundingBox,
                                                readProcess.getOutputFormat() );

    }
    if ( readProcess.getVerbose() )
    {

      std::cout << std::endl;

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void BrukerRecoReadProcess::read( gkg::Process& process, "
             "const std::string& fileNameReference, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class BrukerReco2GisConverterCommand
//

gkg::BrukerReco2GisConverterCommand::BrukerReco2GisConverterCommand(
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
  GKG_CATCH( "gkg::BrukerReco2GisConverterCommand::BrukerReco2GisConverterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::BrukerReco2GisConverterCommand::BrukerReco2GisConverterCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerReco2GisConverterCommand::"
             "BrukerReco2GisConverterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


gkg::BrukerReco2GisConverterCommand::BrukerReco2GisConverterCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH( "gkg::BrukerReco2GisConverterCommand::"
             "BrukerReco2GisConverterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::BrukerReco2GisConverterCommand::~BrukerReco2GisConverterCommand()
{
}


std::string gkg::BrukerReco2GisConverterCommand::getStaticName()
{

  try
  {

    return "BrukerReco2GisConverter";

  }
  GKG_CATCH( "std::string gkg::BrukerReco2GisConverterCommand::getStaticName()" );

}


void gkg::BrukerReco2GisConverterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outputFormat = "gis";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Bruker Reco to GIS disk format converter",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input Bruker Reco directory name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "GIS reco image file name",
                                 fileNameOut );
    application.addSingleOption( "-format",
                                 "Output format (default=gis)",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             outputFormat,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::BrukerReco2GisConverterCommand::parse()" );

}


void gkg::BrukerReco2GisConverterCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& outputFormat,
                                                bool verbose )
{

  try
  {

    BrukerRecoReadProcess brukerRecoReadProcess( fileNameOut, outputFormat,
                                                 verbose );
    brukerRecoReadProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::BrukerReco2GisConverterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outputFormat, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    BrukerReco2GisConverterCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
