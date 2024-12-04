#include <gkg-processing-plugin-functors/IntensityNormalization/IntensityNormalizationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
// class IntensityNormalizationProcess
//


class IntensityNormalizationProcess : public gkg::Process
{

  public:

    IntensityNormalizationProcess(
                        const std::string& theFileNameVolumeOut,
                        const std::string& theFileNameWeightVolume,
                        const bool& theVerbose );

  private:

    template < class T >
    static void analyze( gkg::Process& process,
                         const std::string& fileNameVolumeIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

    const std::string& fileNameVolumeOut;
    const std::string& fileNameWeightVolume;
    const bool& verbose;

};


IntensityNormalizationProcess::IntensityNormalizationProcess(
                        const std::string& theFileNameVolumeOut,
                        const std::string& theFileNameWeightVolume,
                        const bool& theVerbose )
                : gkg::Process( "Volume" ),
                  fileNameVolumeOut( theFileNameVolumeOut ),
                  fileNameWeightVolume( theFileNameWeightVolume ),
                  verbose( theVerbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &IntensityNormalizationProcess::analyze< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &IntensityNormalizationProcess::analyze< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &IntensityNormalizationProcess::analyze< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &IntensityNormalizationProcess::analyze< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &IntensityNormalizationProcess::analyze< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &IntensityNormalizationProcess::analyze< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &IntensityNormalizationProcess::analyze< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &IntensityNormalizationProcess::analyze< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &IntensityNormalizationProcess::analyze< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &IntensityNormalizationProcess::analyze< double > );

}



template < class T >
void IntensityNormalizationProcess::analyze( gkg::Process& process,
                                    const std::string& fileNameVolumeIn,
                                    const gkg::AnalyzedObject&,
                                    const std::string& )
{

  try
  {

    IntensityNormalizationProcess& intensityNormalizationProcess = 
                    static_cast< IntensityNormalizationProcess& >( process );

    // reading input measure volume 
    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "reading '" << fileNameVolumeIn << "' : " << std::flush;

    }

    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameVolumeIn, volumeIn );

    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    // reading weigth volume 
    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "reading '" 
                << intensityNormalizationProcess.fileNameWeightVolume
                << "' : " << std::flush;

    }

    gkg::Volume< T > volumeWeight;
    gkg::Reader::getInstance().read( 
                          intensityNormalizationProcess.fileNameWeightVolume,
                          volumeWeight );

    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    // normalizing volume
    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "normalizing : " << std::flush;

    }

    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();
    gkg::Vector3d< double > resolution;
    volumeIn.getResolution( resolution );

    gkg::Volume< T > volumeOut( sizeX, sizeY, sizeZ );
    volumeOut.setResolution( resolution );

    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    double wMean = 0.0;
    double sum = 0.0;

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          sum += (double)volumeWeight( x, y, z );

        }

      }

    }

    for ( z = 0; z < sizeZ; z++ )
    {

      for ( y = 0; y < sizeY; y++ )
      {

        for ( x = 0; x < sizeX; x++ )
        {

          double weight = (double)volumeWeight( x, y, z );
          double value = weight * (double)volumeIn( x, y, z ) / sum;
          volumeOut( x, y, z ) = (T)value;
          wMean += value;

        }

      }

    }

    if ( intensityNormalizationProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    std::cout << "Weighted mean = " << wMean << std::endl;

    if ( !intensityNormalizationProcess.fileNameVolumeOut.empty() )
    {

      if ( intensityNormalizationProcess.verbose )
      {

        std::cout << "writing '" 
                  << intensityNormalizationProcess.fileNameVolumeOut
                  << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write( 
                          intensityNormalizationProcess.fileNameVolumeOut,
                          volumeOut );

      if ( intensityNormalizationProcess.verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void IntensityNormalizationProcess::analyze( "
             "gkg::Process& process, "
             "const std::string& fileNameVolumeIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   IntensityNormalizationCommand
//


gkg::IntensityNormalizationCommand::IntensityNormalizationCommand(
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
  GKG_CATCH( "gkg::IntensityNormalizationCommand::"
             "IntensityNormalizationCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::IntensityNormalizationCommand::IntensityNormalizationCommand(
                                        const std::string& fileNameVolumeIn,
                                        const std::string& fileNameVolumeOut,
                                        const std::string& fileNameVolumeWeight,
                                        bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( fileNameVolumeIn, fileNameVolumeOut, fileNameVolumeWeight,
             verbose );

  }
  GKG_CATCH( "gkg::IntensityNormalizationCommand::"
             "IntensityNormalizationCommand( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeOut, "
             "const std::string& fileNameVolumeWeight, bool verbose )" );

}


gkg::IntensityNormalizationCommand::IntensityNormalizationCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameVolumeWeight );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameVolumeIn, fileNameVolumeOut, fileNameVolumeWeight,
             verbose );

  }
  GKG_CATCH( "gkg::IntensityNormalizationCommand::"
             "IntensityNormalizationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::IntensityNormalizationCommand::~IntensityNormalizationCommand()
{
}


std::string gkg::IntensityNormalizationCommand::getStaticName()
{

  try
  {

    return "IntensityNormalization";

  }
  GKG_CATCH( "std::string "
             "gkg::IntensityNormalizationCommand::getStaticName()" );

}


void gkg::IntensityNormalizationCommand::parse()
{

  try
  {

    std::string fileNameVolumeIn;
    std::string fileNameVolumeOut = "";
    std::string fileNameVolumeWeight;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Intensity normalization I=I*w/sum(w)",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input volume file name",
                                 fileNameVolumeIn );
    application.addSingleOption( "-w",
                                 "Input weight volume file name",
                                 fileNameVolumeWeight );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameVolumeOut,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameVolumeIn, fileNameVolumeOut, fileNameVolumeWeight,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::IntensityNormalizationCommand::parse()" );

}


void gkg::IntensityNormalizationCommand::execute(
                  	                const std::string& fileNameVolumeIn,
                                        const std::string& fileNameVolumeOut,
                                        const std::string& fileNameVolumeWeight,
                                        bool verbose )
{

  try
  {

    IntensityNormalizationProcess intensityNormalizationProcess(
                                     			   fileNameVolumeOut,
                                     			   fileNameVolumeWeight,
                                     			   verbose );
    intensityNormalizationProcess.execute( fileNameVolumeIn );

  }
  GKG_CATCH( "void gkg::IntensityNormalizationCommand::execute( "
             "const std::string& fileNameVolumeIn, "
             "const std::string& fileNameVolumeOut, "
             "const std::string& fileNameVolumeWeight, bool verbose )" );

}


RegisterCommandCreator( IntensityNormalizationCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameVolumeWeight ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
