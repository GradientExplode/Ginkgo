#include <gkg-processing-plugin-functors/NonLocalMeansInPainter/NonLocalMeansInPainterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/NonLocalMeansInPainter_i.h>
#include <gkg-processing-algobase/NLMNoiseModelFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>



//
//  NonLocalMeansInPainterProcess
//


class NonLocalMeansInPainterProcess : public gkg::Process
{

  public:

    NonLocalMeansInPainterProcess( const std::string& theFileNameOut,
                                   const std::string& theFileNameMask,
                                   const std::string& theFileNameInPaintingMask,
                                   const int32_t& theHalfSearchBlockSize,
                                   const int32_t& theHalfNeighborhooSize,
                                   const float& theDegreeOfFiltering,
                                   const std::string& theNoiseModel,
                                   const float& theNoiseStdDev,
                                   const float& theCoilCount,
                                   const bool& theAscii,
                                   const std::string& theFormat,
                                   const bool& theVerbose );

    const std::string& fileNameOut;
    const std::string& fileNameMask;
    const std::string& fileNameInPaintingMask;
    const int32_t& halfSearchBlockSize;
    const int32_t& halfNeighborhoodSize;
    const float& degreeOfFiltering;
    const std::string& noiseModel;
    const float& noiseStdDev;
    const float& coilCount;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void inPainting( gkg::Process& process,
                            const std::string& fileNameIn,
                            const gkg::AnalyzedObject&,
                            const std::string& );


};


NonLocalMeansInPainterProcess::NonLocalMeansInPainterProcess( 
                                   const std::string& theFileNameOut,
                                   const std::string& theFileNameMask,
                                   const std::string& theFileNameInPaintingMask,
                                   const int32_t& theHalfSearchBlockSize,
                                   const int32_t& theHalfNeighborhooSize,
                                   const float& theDegreeOfFiltering,
                                   const std::string& theNoiseModel,
                                   const float& theNoiseStdDev,
                                   const float& theCoilCount,
                                   const bool& theAscii,
                                   const std::string& theFormat,
                                   const bool& theVerbose )
                              : gkg::Process( "Volume" ),
                                fileNameOut( theFileNameOut ),
                                fileNameMask( theFileNameMask ),
                                fileNameInPaintingMask(
                                                    theFileNameInPaintingMask ),
                                halfSearchBlockSize( theHalfSearchBlockSize ),
                                halfNeighborhoodSize( theHalfNeighborhoodSize ),
                                degreeOfFiltering( theDegreeOfFiltering ),
                                noiseModel( theNoiseModel ),
                                noiseStdDev( theNoiseStdDev ),
                                coilCount( theCoilCount ),
                                ascii( theAscii ),
                                format( theFormat ),
                                verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &NonLocalMeansInPainterProcess::inPainting< double > );

  }
  GKG_CATCH( "NonLocalMeansInPainterProcess::NonLocalMeansInPainterProcess( 
             "const std::string& theFileNameOut, "
             "const std::string& theFileNameMask, "
             "const std::string& theFileNameInPaintingMask, "
             "const int32_t& theHalfSearchBlockSize, "
             "const int32_t& theHalfNeighborhooSize, "
             "const float& theDegreeOfFiltering, "
             "const std::string& theNoiseModel, "
             "const float& theNoiseStdDev, "
             "const float& theCoilCount, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void NonLocalMeansInPainterProcess::inPainting( gkg::Process& process,
                                                const std::string& fileNameIn,
                                                const gkg::AnalyzedObject&,
                                                const std::string& )
{

  try
  {

    NonLocalMeansInPainterProcess&
      inPainterProcess = static_cast< NonLocalMeansInPainterProcess& >( 
                                                                      process );

    const std::string& fileNameIn = inPainterProcess.fileNameIn;
    const std::string& fileNameOut = inPainterProcess.fileNameOut;
    const std::string& fileNameMask = inPainterProcess.fileNameMask;
    const std::string& fileNameInPaintingMask = 
                                        inPainterProcess.fileNameInPaintingMask;
    const std::string& noiseModel = inPainterProcess.noiseModel;
    int32_t halfSearchBlockSize = inPainterProcess.halfSearchBlockSize;
    int32_t halfNeighborhoodSize = inPainterProcess.halfNeighborhoodSize;
    float degreeOfFiltering = inPainterProcess.degreeOfFiltering;
    float noiseStdDev = inPainterProcess.noiseStdDev;
    float coilCount = inPainterProcess.coilCount;
    bool ascii = inPainterProcess.ascii;
    const std::string& format = inPainterProcess.format;
    bool verbose = inPainterProcess.;


    ////////////////////////////////////////////////////////////////////////////
    // reading input volume to be in-painted
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }

    gkg::Volume< T > inputVolume;
    gkg::Reader::getInstance().read( fileNameIn, inputVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }

    gkg::Volume< int16_t > maskVolume;
    gkg::Reader::getInstance().read( fileNameMask, maskVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading in-painting mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameInPaintingMask << "' : "
                 << std::flush;

    }

    gkg::Volume< int16_t > inPaintingMaskVolume;
    gkg::Reader::getInstance().read( fileNameInPaintingMask,
                                     inPaintingMaskVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating noise model
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "creating " << noiseModel << " model : "
                 << std::flush;

    }

    gkg::RCPointer< gkg::NLMNoiseModel >
      noiseModel( 
        gkg::NLMNoiseModelFactory::getInstance().createNLMNoiseModelFunction(
                                                                  noiseModel,
                                                                  noiseStdDev,
                                                                  coilCount ) );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // non local means in-painting
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "non local means in-painting : " << std::flush;

    }

    gkg::Volume< T > outputVolume;
    gkg::NonLocalMeansInPainter< T, T > inPainter( noiseModel,
                                                   halfSearchBlockSize,
                                                   halfNeighborhoodSize,
                                                   degreeOfFiltering,
                                                   verbose );
    inPainter.inPaint( inputVolume,
                       maskVolume,
                       inPaintingMaskVolume,
                       outputVolume );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // Writing output volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOut,
                                      outputVolume,
                                      ascii,
                                      format );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void NonLocalMeansInPainterProcess::inPainter( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  NonLocalMeansInPainterCommand
//


gkg::NonLocalMeansInPainterCommand::NonLocalMeansInPainterCommand(
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
  GKG_CATCH( "gkg::NonLocalMeansInPainterCommand::"
             "NonLocalMeansInPainterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::NonLocalMeansInPainterCommand::NonLocalMeansInPainterCommand(
                                      const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& fileNameMask,
                                      const std::string& fileNameInPaintingMask,
                                      const std::string& noiseModel,
                                      int32_t halfSearchBlockSize,
                                      int32_t halfNeighborhoodSize,
                                      float degreeOfFiltering,
                                      float noiseStdDev,
                                      float coilCount,
                                      bool ascii,
                                      const std::string& format,
                                      bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             fileNameMask,
             fileNameInPaintingMask,
             noiseModel,
             halfSearchBlockSize,
             halfNeighborhoodSize,
             degreeOfFiltering,
             noiseStdDev,
             coilCount,
             ascii, 
             format,
             verbose );

  }
  GKG_CATCH( "gkg::NonLocalMeansInPainterCommand::"
             "NonLocalMeansInPainterCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameInPaintingMask, "
             "const std::string& noiseModel, "
             "int32_t halfSearchBlockSize, "
             "int32_t halfNeighborhoodSize, "
             "float degreeOfFiltering, "
             "float noiseStdDev, 
             "float coilCount, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::NonLocalMeansInPainterCommand::NonLocalMeansInPainterCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInPaintingMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, noiseModel );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, halfSearchBlockSize );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, halfNeighborhoodSize );
    DECLARE_FLOATING_PARAMETER( parameters, float, degreeOfFiltering );
    DECLARE_FLOATING_PARAMETER( parameters, float, noiseStdDev );
    DECLARE_FLOATING_PARAMETER( parameters, float, coilCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             fileNameMask,
             fileNameInPaintingMask,
             noiseModel,
             halfSearchBlockSize,
             halfNeighborhoodSize,
             degreeOfFiltering,
             noiseStdDev,
             coilCount,
             ascii, 
             format,
             verbose );

  }
  GKG_CATCH( "gkg::NonLocalMeansInPainterCommand::"
             "NonLocalMeansInPainterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::NonLocalMeansInPainterCommand::~NonLocalMeansInPainterCommand()
{
}


std::string gkg::NonLocalMeansInPainterCommand::getStaticName()
{

  try
  {

    return "NonLocalMeansInPainter";

  }
  GKG_CATCH( "std::string "
             "gkg::NonLocalMeansInPainterCommand::getStaticName()" );

}


void gkg::NonLocalMeansInPainterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string noiseModel = "gaussian";
    int32_t halfSearchBlockSize = 8;
    int32_t halfNeighborhoodSize = 1;
    float degreeOfFiltering = 1.0f;
    float noiseStdDev = 1.0f;
    float coilCount = 1.0f;
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
    application.addSingleOption( "-halfSearchBlockSize",
                                 "Half search block size (default=8)",
                                 halfSearchBlockSize,
                                 true );
    application.addSingleOption( "-halfNeighborhoodSize",
                                 "Half neighborhood size (default=1)",
                                 halfNeighborhoodSize,
                                 true );
    application.addSingleOption( "-d",
                                 "Degree of filtering (default=1.0)",
                                 degreeOfFiltering,
                                 true );
    application.addSingleOption( "-noiseStdDev",
                                 "Noise standard deviation (default=1.0)",
                                 noiseStdDev,
                                 true );
    application.addSingleOption( "-coilCount",
                                 "Number of coils (default=1.0)",
                                 coilCount,
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

    execute( fileNameIn,
             fileNameOut,
             fileNameMask,
             fileNameInPaintingMask,
             noiseModel,
             halfSearchBlockSize,
             halfNeighborhoodSize,
             degreeOfFiltering,
             noiseStdDev,
             coilCount,
             ascii, 
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::NonLocalMeansInPainterCommand::parse()" );

}


void gkg::NonLocalMeansInPainterCommand::execute(
                                      const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& fileNameMask,
                                      const std::string& fileNameInPaintingMask,
                                      const std::string& noiseModel,
                                      int32_t halfSearchBlockSize,
                                      int32_t halfNeighborhoodSize,
                                      float degreeOfFiltering,
                                      float noiseStdDev,
                                      float coilCount,
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

    if ( halfSearchBlockSize <= halfNeighborhoodSize )
    {

      throw std::runtime_error( 
       "halfSearchBlockSize and halfNeighborhoodSize values are inconsistent" );

    }

    //
    // launching process
    //
    NonLocalMeansInPainterProcess inPainterProcess( fileNameOut,
                                                    fileNameMask,
                                                    fileNameInPaintingMask,
                                                    halfSearchBlockSize,
                                                    halfNeighborhoodSize,
                                                    degreeOfFiltering,
                                                    noiseModel,
                                                    noiseStdDev,
                                                    coilCount,
                                                    ascii,
                                                    format,
                                                    verbose );
    inPainterProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::NonLocalMeansInPainterCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameInPaintingMask, "
             "const std::string& noiseModel, "
             "int32_t halfSearchBlockSize, "
             "int32_t halfNeighborhoodSize, "
             "float degreeOfFiltering, "
             "float noiseStdDev, "
             "float coilCount, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose ) " );

}


RegisterCommandCreator( NonLocalMeansInPainterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameInPaintingMask )+
    			DECLARE_STRING_PARAMETER_HELP( noiseModel ) +
    			DECLARE_INTEGER_PARAMETER_HELP( halfSearchBlockSize ) +
    			DECLARE_INTEGER_PARAMETER_HELP( halfNeighborhoodSize ) +
    			DECLARE_FLOATING_PARAMETER_HELP( degreeOfFiltering ) +
    			DECLARE_FLOATING_PARAMETER_HELP( noiseStdDev ) +
    			DECLARE_FLOATING_PARAMETER_HELP( coilCount ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
