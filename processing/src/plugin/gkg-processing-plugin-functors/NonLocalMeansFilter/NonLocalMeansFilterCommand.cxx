#include <gkg-processing-plugin-functors/NonLocalMeansFilter/NonLocalMeansFilterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/NonLocalMeansFilter_i.h>
#include <gkg-processing-algobase/NLMNoiseModelFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//  NonLocalMeansFilterProcess
//


class NonLocalMeansFilterProcess : public gkg::Process
{

  public:

    NonLocalMeansFilterProcess( const std::string& fileNameOut,
                                const int32_t& halfSearchBlocSize,
                                const int32_t& halfNeighborhooSize,
                                const float& degreeOfFiltering,
                                const std::string& noiseModel,
                                const float& sigma,
                                const float& n,
                                const bool& ascii,
                                const std::string& format,
                                const bool& verbose );

    const std::string& getFileNameOut() const;
    const int32_t& getHalfSearchBlocSize() const;
    const int32_t& getHalfNeighborhoodSize() const;
    const float& getDegreeOfFiltering() const;
    const std::string& getNoiseModel() const;
    const float& getSigma() const;
    const float& getN() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void filter( gkg::Process& process,
                        const std::string& fileNameIn,
                        const gkg::AnalyzedObject&,
                        const std::string& );

    const std::string& _fileNameOut;
    const int32_t& _halfSearchBlocSize;
    const int32_t& _halfNeighborhoodSize;
    const float& _degreeOfFiltering;
    const std::string& _noiseModel;
    const float& _sigma;
    const float& _n;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


NonLocalMeansFilterProcess::NonLocalMeansFilterProcess( 
                                            const std::string& fileNameOut,
                                            const int32_t& halfSearchBlocSize,
                                            const int32_t& halfNeighborhoodSize,
                                            const float& degreeOfFiltering,
                                            const std::string& noiseModel,
                                            const float& sigma,
                                            const float& n,
                                            const bool& ascii,
                                            const std::string& format,
                                            const bool& verbose)
                          : gkg::Process( "Volume" ),
                            _fileNameOut( fileNameOut ),
                            _halfSearchBlocSize( halfSearchBlocSize ),
                            _halfNeighborhoodSize( halfNeighborhoodSize ),
                            _degreeOfFiltering( degreeOfFiltering ),
                            _noiseModel( noiseModel ),
                            _sigma( sigma ),
                            _n( n ),
                            _ascii( ascii ),
                            _format( format ),
                            _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &NonLocalMeansFilterProcess::filter< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &NonLocalMeansFilterProcess::filter< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &NonLocalMeansFilterProcess::filter< double > );

}


const std::string& NonLocalMeansFilterProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const int32_t& NonLocalMeansFilterProcess::getHalfSearchBlocSize() const
{

  return _halfSearchBlocSize;

}


const int32_t& NonLocalMeansFilterProcess::getHalfNeighborhoodSize() const
{

  return _halfNeighborhoodSize;

}


const float& NonLocalMeansFilterProcess::getDegreeOfFiltering() const
{

  return _degreeOfFiltering;

}


const std::string& NonLocalMeansFilterProcess::getNoiseModel() const
{

  return _noiseModel;

}


const float& NonLocalMeansFilterProcess::getSigma() const
{

  return _sigma;

}


const float& NonLocalMeansFilterProcess::getN() const
{

  return _n;

}


const bool& NonLocalMeansFilterProcess::getAscii() const
{

  return _ascii;

}


const std::string& NonLocalMeansFilterProcess::getFormat() const
{

  return _format;

}


const bool& NonLocalMeansFilterProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void NonLocalMeansFilterProcess::filter( gkg::Process& process,
                                         const std::string& fileNameIn,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    NonLocalMeansFilterProcess&
          filterProcess = static_cast< NonLocalMeansFilterProcess& >( process );

    // reading data
    if ( filterProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    if ( filterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( filterProcess.getVerbose() )
    {

      std::cout << "non local means filtering : " << std::flush;

    }

    gkg::NLMNoiseModel* noiseModel = 
        gkg::NLMNoiseModelFactory::getInstance().createNLMNoiseModelFunction(
                                                  filterProcess.getNoiseModel(),
                                                  filterProcess.getSigma(),
                                                  filterProcess.getN() );

    gkg::Volume< T > volumeOut;
    gkg::NonLocalMeansFilter< T, T > filter( 
                                        *noiseModel,
                                        filterProcess.getHalfSearchBlocSize(),
                                        filterProcess.getHalfNeighborhoodSize(),
                                        filterProcess.getDegreeOfFiltering() );
    filter.filter( volume, volumeOut );

    if ( filterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( filterProcess.getVerbose() )
    {

      std::cout << "writing '"
                << filterProcess.getFileNameOut() << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( filterProcess.getFileNameOut(),
                                      volumeOut,
                                      filterProcess.getAscii(),
                                      filterProcess.getFormat() );
    if ( filterProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    delete noiseModel;

  }
  GKG_CATCH( "template < class T > "
             "void NonLocalMeansFilterProcess::filter( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  NonLocalMeansFilterCommand
//


gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand( int32_t argc,
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
  GKG_CATCH( "gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand(
                                		 const std::string& fileNameIn,
                                		 const std::string& fileNameOut,
                                		 const std::string& noiseModel,
                                		 int32_t halfSearchBlocSize,
                                		 int32_t halfNeighborhoodSize,
                                		 float degreeOfFiltering,
                                		 float sigma,
                                		 float n,
                                		 bool ascii,
                                		 const std::string& format,
                                		 bool verbose )
                               : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, noiseModel, halfSearchBlocSize,
             halfNeighborhoodSize, degreeOfFiltering, sigma, n, ascii, 
             format, verbose );

  }
  GKG_CATCH( "gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& noiseModel, int32_t halfSearchBlocSize, "
             "int32_t halfNeighborhoodSize, float degreeOfFiltering, "
             "float sigma, float n, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand(
                                             const gkg::Dictionary& parameters )
                               : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, noiseModel );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, halfSearchBlocSize );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, halfNeighborhoodSize );
    DECLARE_FLOATING_PARAMETER( parameters, float, degreeOfFiltering );
    DECLARE_FLOATING_PARAMETER( parameters, float, sigma );
    DECLARE_FLOATING_PARAMETER( parameters, float, n );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, noiseModel, halfSearchBlocSize,
             halfNeighborhoodSize, degreeOfFiltering, sigma, n, ascii, 
             format, verbose );

  }
  GKG_CATCH( "gkg::NonLocalMeansFilterCommand::NonLocalMeansFilterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::NonLocalMeansFilterCommand::~NonLocalMeansFilterCommand()
{
}


std::string gkg::NonLocalMeansFilterCommand::getStaticName()
{

  try
  {

    return "NonLocalMeansFilter";

  }
  GKG_CATCH( "std::string gkg::NonLocalMeansFilterCommand::getStaticName()" );

}


void gkg::NonLocalMeansFilterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string noiseModel = "gaussian";
    int32_t halfSearchBlocSize = 8;
    int32_t halfNeighborhoodSize = 1;
    float degreeOfFiltering = 1.0f;
    float sigma = 1.0f;
    float n = 1.0f;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Non local means filtering of container",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-bs",
                                 "Half search bloc size (default=8)",
                                 halfSearchBlocSize,
                                 true );
    application.addSingleOption( "-hs",
                                 "Half neighborhood size (default=1)",
                                 halfNeighborhoodSize,
                                 true );
    application.addSingleOption( "-d",
                                 "Degree of filtering (default=1.0)",
                                 degreeOfFiltering,
                                 true );
    application.addSingleOption( "-s",
                                 "Noise standard deviation (default=1.0)",
                                 sigma,
                                 true );
    application.addSingleOption( "-n",
                                 "Number of coils (default=1.0)",
                                 n,
                                 true );
    application.addSingleOption( "-m",
                                 "Noise model (default=gaussian)\n"
                                 " - gaussian\n"
                                 " - rician\n"
                                 " - nonCentralChi",
                                 noiseModel,
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

    execute( fileNameIn, fileNameOut, noiseModel, halfSearchBlocSize,
             halfNeighborhoodSize, degreeOfFiltering, sigma, n, ascii, 
             format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::NonLocalMeansFilterCommand::parse()" );

}


void gkg::NonLocalMeansFilterCommand::execute( const std::string& fileNameIn,
                                	       const std::string& fileNameOut,
                                	       const std::string& noiseModel,
                                	       int32_t halfSearchBlocSize,
                                	       int32_t halfNeighborhoodSize,
                                	       float degreeOfFiltering,
                                	       float sigma,
                                	       float n,
                                	       bool ascii,
                                	       const std::string& format,
                                	       bool verbose )
{

  try
  {

    //
    //  sanity checks
    //

    if ( ( noiseModel != "gaussian" ) &&
         ( noiseModel != "rician" ) &&
         ( noiseModel != "nonCentralChi" ) )
    {

      throw std::runtime_error( "invalid noise model" );

    }

    if ( halfSearchBlocSize <= halfNeighborhoodSize )
    {

      throw std::runtime_error( 
        "halfSearchBlocSize and halfNeighborhoodSize values are inconsistent" );

    }

    //
    // launching process
    //
    NonLocalMeansFilterProcess filterProcess( fileNameOut,
                                              halfSearchBlocSize,
                                              halfNeighborhoodSize,
                                              degreeOfFiltering,
                                              noiseModel,
                                              sigma,
                                              n,
                                              ascii,
                                              format,
                                              verbose );
    filterProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::NonLocalMeansFilterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& noiseModel, int32_t halfSearchBlocSize, "
             "int32_t halfNeighborhoodSize, float degreeOfFiltering, "
             "float sigma, float n, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( NonLocalMeansFilterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( noiseModel ) +
    			DECLARE_INTEGER_PARAMETER_HELP( halfSearchBlocSize ) +
    			DECLARE_INTEGER_PARAMETER_HELP( halfNeighborhoodSize ) +
    			DECLARE_FLOATING_PARAMETER_HELP( degreeOfFiltering ) +
    			DECLARE_FLOATING_PARAMETER_HELP( sigma ) +
    			DECLARE_FLOATING_PARAMETER_HELP( n ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
