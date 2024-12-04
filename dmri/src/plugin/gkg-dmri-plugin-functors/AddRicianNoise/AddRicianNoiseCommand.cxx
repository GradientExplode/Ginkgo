#include <gkg-dmri-plugin-functors/AddRicianNoise/AddRicianNoiseCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/RandomGenerator.h>
#include <gkg-core-io/Reader_i.h> 
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>


//
// class RicianNoiseAdditionProcess
//

class RicianNoiseAdditionProcess : public gkg::Process
{

  public:

    RicianNoiseAdditionProcess( const std::string& fileNameOut,
                                double noiseStdDev,
                                bool verbose );

    const std::string& getFileNameOut() const;
    double getNoiseStandardDeviation() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void addRicianNoise( gkg::Process& process,
                                const std::string& fileName,
                                const gkg::AnalyzedObject&,
                                const std::string& );

    const std::string& _fileNameOut;
    double _noiseStdDev;
    bool _verbose;

};


RicianNoiseAdditionProcess::RicianNoiseAdditionProcess( 
                                                 const std::string& fileNameOut,
                                                 double noiseStdDev,
                                                 bool verbose )
                           : gkg::Process(),
                             _fileNameOut( fileNameOut ),
                             _noiseStdDev( noiseStdDev ),
                             _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &RicianNoiseAdditionProcess::addRicianNoise< double > );

}


const std::string& RicianNoiseAdditionProcess::getFileNameOut() const
{

  return _fileNameOut;

}


double RicianNoiseAdditionProcess::getNoiseStandardDeviation() const
{

  return _noiseStdDev;

}


bool RicianNoiseAdditionProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void RicianNoiseAdditionProcess::addRicianNoise( gkg::Process& process,
                                                 const std::string& fileName,
                                                 const gkg::AnalyzedObject&,
                                                 const std::string& )
{

  try
  {

    RicianNoiseAdditionProcess&
      ricianNoiseAdditionProcess = static_cast< RicianNoiseAdditionProcess& >(
                                                                      process ); 


    ////////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    ////////////////////////////////////////////////////////////////////////////
   
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                     getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // reading volume
    ////////////////////////////////////////////////////////////////////////////

    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "reading volume : " << std::flush;

    }

    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileName, volume );
    gkg::Vector3d< int32_t > size;
    int32_t sizeT;
    gkg::Vector3d< double > resolution ( 1.0, 1.0, 1.0 );
    double resolutionT = 1.0;
    volume.getSize( size, sizeT );
    volume.getResolution( resolution, resolutionT );

    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // allocating noisy volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "creating noisy volume : " << std::flush;

    }

    gkg::Volume< T > noisyVolume( size, sizeT );
    noisyVolume.setResolution( resolution, resolutionT );

    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // read b-values / orientations from the input file and put them in the 
    // noisy volume
    ////////////////////////////////////////////////////////////////////////////

    if ( volume.getHeader().hasAttribute( "bvalues" ) )
    {

      std::vector< double > bValues;
      volume.getHeader().getAttribute( "bvalues", bValues );
      noisyVolume.getHeader().addAttribute( "bvalues", bValues );

    }

    if ( volume.getHeader().hasAttribute( "diffusion_gradient_orientations" ) )
    {

      gkg::GenericObjectList diffusion_gradient_orientations;
      volume.getHeader().getAttribute( "diffusion_gradient_orientations",
                                       diffusion_gradient_orientations );
      noisyVolume.getHeader().addAttribute( "diffusion_gradient_orientations",
                                            diffusion_gradient_orientations );

    }


    ////////////////////////////////////////////////////////////////////////////
    // add Rician noise in the data
    ////////////////////////////////////////////////////////////////////////////

    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "adding Rician noise in the data : " << std::flush;

    }
    
    // creating randomGenerator and initializing noiseRe and noiseIm
    gkg::RandomGenerator randomGenerator( gkg::RandomGenerator::Taus );
    double noiseRe = 0.0;
    double noiseIm = 0.0;
    double stdDev = ricianNoiseAdditionProcess.getNoiseStandardDeviation();
        
    // initializing other variables
    typename gkg::Volume< T >::iterator
      v = volume.begin(),
      ve = volume.end(),
      n = noisyVolume.begin();
    
    while ( v != ve )
    {

      noiseRe = factory->getGaussianRandomNumber( randomGenerator,
                                                  0.0, 
                                                  stdDev );
      noiseIm = factory->getGaussianRandomNumber( randomGenerator,
                                                  0.0, 
                                                  stdDev );

      double noiseValueRe = (double)*v + noiseRe;

      *n = ( T )( std::sqrt( noiseValueRe * noiseValueRe + 
                             noiseIm * noiseIm ) );

      ++v;
      ++n;

    }
    
    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the noisy volume
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "writing output volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( 
                                    ricianNoiseAdditionProcess.getFileNameOut(),
                                    noisyVolume );

    if ( ricianNoiseAdditionProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void RicianNoiseAdditionProcess::addRicianNoise( "
             "gkg::Process& process, "
             "const std::string& fileName, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::AddRicianNoiseCommand::AddRicianNoiseCommand( int32_t argc,
                                                   char* argv[],
                                                   bool loadPlugin,
                                                   bool removeFirst )
                          : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::AddRicianNoiseCommand::AddRicianNoiseCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AddRicianNoiseCommand::AddRicianNoiseCommand(
                                                 const std::string& fileNameIn,
                                                 const std::string& fileNameOut,
                                                 double noiseStdDev,
                                                 bool verbose )
                          : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, noiseStdDev, verbose );

  }
  GKG_CATCH( "gkg::SubVolumeCommand::SubVolumeCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "double noisStdDev, bool verbose )" );

}


gkg::AddRicianNoiseCommand::AddRicianNoiseCommand(
                                             const gkg::Dictionary& parameters )
                           : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, double, noiseStdDev );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, noiseStdDev, verbose );

  }
  GKG_CATCH( "gkg::AddRicianNoiseCommand::AddRicianNoiseCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AddRicianNoiseCommand::~AddRicianNoiseCommand()
{
}


std::string gkg::AddRicianNoiseCommand::getStaticName()
{

  try
  {

    return "AddRicianNoise";

  }
  GKG_CATCH( "std::string gkg::AddRicianNoiseCommand::getStaticName()" );

}


void gkg::AddRicianNoiseCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut; 
    double noiseStdDev = 0.0;    
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, 
                                  "Addition of a Rician Noise",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Noisy data file name",
                                 fileNameOut );
    application.addSingleOption( "-s",
                                 "Noise standard deviation (default=0.0)",
                                 noiseStdDev,
				 true );		
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameIn, fileNameOut, noiseStdDev, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AddRicianNoiseCommand::parse()" );

}


void gkg::AddRicianNoiseCommand::execute( const std::string& fileNameIn,
                                          const std::string& fileNameOut,
                                          double noiseStdDev,
                                          bool verbose )
{

  try
  {

    //
    // sanity checks
    //

    if ( fileNameIn.empty() )
    {

      throw std::runtime_error( "Input file name is missing" );

    }

    if ( fileNameOut.empty() )
    {

      throw std::runtime_error( "Output file name is missing" );

    }

    //
    // launching Rician noise addition process
    //

    RicianNoiseAdditionProcess ricianNoiseAdditionProcess( fileNameOut,
                                                           noiseStdDev,
                                                           verbose );
    ricianNoiseAdditionProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::AddRicianNoiseCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "double noisStdDev, bool verbose )" );

}


RegisterCommandCreator(
    AddRicianNoiseCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_FLOATING_PARAMETER_HELP( noiseStdDev ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
