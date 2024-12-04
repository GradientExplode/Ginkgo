#include <gkg-processing-plugin-functors/Voronoi/VoronoiCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-morphology/DanielssonDistanceMapper.h>
#include <gkg-processing-algobase/LowerThanFunction_i.h>
#include <gkg-processing-algobase/LowerOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/DifferentFromFunction_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/GreaterThanFunction_i.h>
#include <gkg-processing-algobase/BetweenOrEqualToFunction_i.h>
#include <gkg-processing-algobase/BetweenFunction_i.h>
#include <gkg-processing-algobase/OutsideOrEqualToFunction_i.h>
#include <gkg-processing-algobase/OutsideFunction_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class VoronoiProcess : public gkg::Process
{

  public:

    VoronoiProcess( const std::string& theFileNameOut,
                    const std::string& theOutType,
                    const std::string& theInsideMode,
                    const double& theInsideThreshold1,
                    const double& theInsideThreshold2,
                    const std::string& theOutsideMode,
                    const double& theOutsideThreshold1,
                    const double& theOutsideThreshold2,
                    const double& theOutsideValue,
                    const bool& theAscii,
                    const std::string& theFormat,
                    const bool& theVerbose );

    const std::string& fileNameOut;
    const std::string& outType;
    const std::string& insideMode;
    const double& insideThreshold1;
    const double& insideThreshold2;
    const std::string& outsideMode;
    const double& outsideThreshold1;
    const double& outsideThreshold2;
    const double& outsideValue;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );


};


VoronoiProcess::VoronoiProcess( const std::string& theFileNameOut,
                                const std::string& theOutType,
                                const std::string& theInsideMode,
                                const double& theInsideThreshold1,
                                const double& theInsideThreshold2,
                                const std::string& theOutsideMode,
                                const double& theOutsideThreshold1,
                                const double& theOutsideThreshold2,
                                const double& theOutsideValue,
                                const bool& theAscii,
                                const std::string& theFormat,
                                const bool& theVerbose )
               : gkg::Process( "Volume" ),
                 fileNameOut( theFileNameOut ),
                 outType( theOutType ),
                 insideMode( theInsideMode ),
                 insideThreshold1( theInsideThreshold1 ),
                 insideThreshold2( theInsideThreshold2 ),
                 outsideMode( theOutsideMode ),
                 outsideThreshold1( theOutsideThreshold1 ),
                 outsideThreshold2( theOutsideThreshold2 ),
                 outsideValue( theOutsideValue ),
                 ascii( theAscii ),
                 format( theFormat ),
                 verbose( theVerbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &VoronoiProcess::operate< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &VoronoiProcess::operate< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &VoronoiProcess::operate< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &VoronoiProcess::operate< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &VoronoiProcess::operate< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &VoronoiProcess::operate< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &VoronoiProcess::operate< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &VoronoiProcess::operate< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &VoronoiProcess::operate< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &VoronoiProcess::operate< double > );

  }
  GKG_CATCH( "VoronoiProcess::VoronoiProcess( "
             "const std::string& theFileNameOut, "
             "const std::string& theOutType, "
             "const std::string& theInsideMode, "
             "const double& theInsideThreshold1, "
             "const double& theInsideThreshold2, "
             "const std::string& theOutsideMode, "
             "const double& theOutsideThreshold1, "
             "const double& theOutsideThreshold2, "
             "const double& theOutsideValue, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void VoronoiProcess::operate( gkg::Process& process,
                              const std::string& fileNameIn,
                              const gkg::AnalyzedObject&,
                              const std::string& )
{

  try
  {

    VoronoiProcess&
      voronoiProcess = static_cast< VoronoiProcess& >( process );

    const std::string& fileNameOut =
      voronoiProcess.fileNameOut;

    const std::string& insideMode =
      voronoiProcess.insideMode;
    const double& insideThreshold1 =
      voronoiProcess.insideThreshold1;
    const double& insideThreshold2 =
      voronoiProcess.insideThreshold2;
    const std::string& outsideMode =
      voronoiProcess.outsideMode;
    const double& outsideThreshold1 =
      voronoiProcess.outsideThreshold1;
    const double& outsideThreshold2 =
      voronoiProcess.outsideThreshold2;

    const bool& ascii =
      voronoiProcess.ascii;
    const std::string& format =
      voronoiProcess.format;
    const bool& verbose =
      voronoiProcess.verbose;

    // reading data
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

    if ( verbose )
    {

      std::cout << "processing Voronoi diagram : "
                << std::flush;

    }

    gkg::TestFunction< T >* insideTestFunction = 0;
    if ( insideMode == "lt" )
    {

      insideTestFunction = new gkg::LowerThanFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "le" )
    {

      insideTestFunction = new gkg::LowerOrEqualToFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "eq" )
    {

      insideTestFunction = new gkg::EqualToFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "di" )
    {

      insideTestFunction = new gkg::DifferentFromFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "ge" )
    {

      insideTestFunction = new gkg::GreaterOrEqualToFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "gt" )
    {

      insideTestFunction = new gkg::GreaterThanFunction< T >(
                                    ( T )insideThreshold1 );

    }
    else if ( insideMode == "be" )
    {

      insideTestFunction = new gkg::BetweenOrEqualToFunction< T >(
                                    ( T )insideThreshold1,
                                    ( T )insideThreshold2 );

    }
    else if ( insideMode == "bt" )
    {

      insideTestFunction = new gkg::BetweenFunction< T >(
                                    ( T )insideThreshold1,
                                    ( T )insideThreshold2 );

    }
    else if ( insideMode == "oe" )
    {

      insideTestFunction = new gkg::OutsideOrEqualToFunction< T >(
                                    ( T )insideThreshold1,
                                    ( T )insideThreshold2 );

    }
    else if ( insideMode == "ou" )
    {

      insideTestFunction = new gkg::OutsideFunction< T >(
                                    ( T )insideThreshold1,
                                    ( T )insideThreshold2 );

    }

    gkg::TestFunction< T >* outsideTestFunction = 0;
    if ( outsideMode == "lt" )
    {

      outsideTestFunction = new gkg::LowerThanFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "le" )
    {

      outsideTestFunction = new gkg::LowerOrEqualToFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "eq" )
    {

      outsideTestFunction = new gkg::EqualToFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "di" )
    {

      outsideTestFunction = new gkg::DifferentFromFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "ge" )
    {

      outsideTestFunction = new gkg::GreaterOrEqualToFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "gt" )
    {

      outsideTestFunction = new gkg::GreaterThanFunction< T >(
                                   ( T )outsideThreshold1 );

    }
    else if ( outsideMode == "be" )
    {

      outsideTestFunction = new gkg::BetweenOrEqualToFunction< T >(
                                   ( T )outsideThreshold1,
                                   ( T )outsideThreshold2 );

    }
    else if ( outsideMode == "bt" )
    {

      outsideTestFunction = new gkg::BetweenFunction< T >(
                                   ( T )outsideThreshold1,
                                   ( T )outsideThreshold2 );

    }
    else if ( outsideMode == "oe" )
    {

      outsideTestFunction = new gkg::OutsideOrEqualToFunction< T >(
                                   ( T )outsideThreshold1,
                                   ( T )outsideThreshold2 );

    }
    else if ( outsideMode == "ou" )
    {

      outsideTestFunction = new gkg::OutsideFunction< T >(
                                   ( T )outsideThreshold1,
                                   ( T )outsideThreshold2 );

    }

    std::string outType = voronoiProcess.outType;
    if ( outType.empty() )
    {

      outType = gkg::TypeOf< int16_t >::getName();

    }

    if ( gkg::TypeOf< uint8_t >::isSameType( outType ) )
    {

      gkg::Volume< uint8_t > volumeOut;
      uint8_t outsideValue = ( uint8_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int8_t >::isSameType( outType ) )
    {

      gkg::Volume< int8_t > volumeOut;
      int8_t outsideValue = ( int8_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( outType ) )
    {

      gkg::Volume< uint16_t > volumeOut;
      uint16_t outsideValue = ( uint16_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( outType ) )
    {

      gkg::Volume< int16_t > volumeOut;
      int16_t outsideValue = ( int16_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( outType ) )
    {

      gkg::Volume< uint32_t > volumeOut;
      uint32_t outsideValue = ( uint32_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( outType ) )
    {

      gkg::Volume< int32_t > volumeOut;
      int32_t outsideValue = ( int32_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( outType ) )
    {

      gkg::Volume< uint64_t > volumeOut;
      uint64_t outsideValue = ( uint64_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( outType ) )
    {

      gkg::Volume< int64_t > volumeOut;
      int64_t outsideValue = ( int64_t )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( outType ) )
    {

      gkg::Volume< float > volumeOut;
      float outsideValue = ( float )voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( outType ) )
    {

      gkg::Volume< double > volumeOut;
      double outsideValue = voronoiProcess.outsideValue;

      gkg::DanielssonDistanceMapper< T >
        danielssonDistanceMapper( *insideTestFunction );
      danielssonDistanceMapper.getVoronoi( volumeIn,
                                           *outsideTestFunction,
                                           volumeOut,
                                           outsideValue,
                                           verbose );


      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut,
                                        volumeOut,
                                        ascii,
                                        format );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void VoronoiProcess::operate( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VoronoiCommand
//


gkg::VoronoiCommand::VoronoiCommand( int32_t argc,
                                     char* argv[],
                                     bool loadPlugin,
                                     bool removeFirst )
                   : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::VoronoiCommand::VoronoiCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VoronoiCommand::VoronoiCommand( const std::string& fileNameIn,
                    		     const std::string& fileNameOut,
                    		     const std::string& outType,
                                     const std::string& insideMode,
                                     const double& insideThreshold1,
                                     const double& insideThreshold2,
                                     const std::string& outsideMode,
                                     const double& outsideThreshold1,
                                     const double& outsideThreshold2,
                                     const double& outsideValue,
                    		     const bool& ascii,
                    		     const std::string& format,
                    		     const bool& verbose )
                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, outType, 
             insideMode, insideThreshold1, insideThreshold2,
             outsideMode, outsideThreshold1, outsideThreshold2,
             outsideValue, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VoronoiCommand::VoronoiCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "const std::string& insideMode, "
             "const double& insideThreshold1, "
             "const double insideThreshold2, "
             "const std::string& outsideMode, "
             "const double outsideThreshold1, "
             "const double outsideThreshold2, "
             "const double outsideValue, "
             "const bool& ascii, "
             "const std::string& format, "
             "const bool& verbose )" );

}


gkg::VoronoiCommand::VoronoiCommand( const gkg::Dictionary& parameters )
                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, outType );
    DECLARE_STRING_PARAMETER( parameters, std::string, insideMode );
    DECLARE_FLOATING_PARAMETER( parameters, double, insideThreshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, insideThreshold2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, outsideMode );
    DECLARE_FLOATING_PARAMETER( parameters, double, outsideThreshold1 );
    DECLARE_FLOATING_PARAMETER( parameters, double, outsideThreshold2 );
    DECLARE_FLOATING_PARAMETER( parameters, double, outsideValue );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, outType, 
             insideMode, insideThreshold1, insideThreshold2,
             outsideMode, outsideThreshold1, outsideThreshold2,
             outsideValue, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VoronoiCommand::VoronoiCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VoronoiCommand::~VoronoiCommand()
{
}


std::string gkg::VoronoiCommand::getStaticName()
{

  try
  {

    return "Voronoi";

  }
  GKG_CATCH( "std::string gkg::VoronoiCommand::getStaticName()" );

}


void gkg::VoronoiCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outType = "";
    std::string insideMode;
    double insideThreshold1;
    double insideThreshold2 = 0.0;
    std::string outsideMode = "eq";
    double outsideThreshold1 = -1.0;
    double outsideThreshold2 = 0.0;
    double outsideValue = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Voronoi diagram mapper",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type uint8_t/int16_t "
                                 "(default=int16_t)",
                                 outType,
                                 true );
    application.addSingleOption( "-im",
                                 "Inside threshold mode\n"
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
                                 insideMode );
    application.addSingleOption( "-it1",
                                 "Inside first threshold",
                                 insideThreshold1 );
    application.addSingleOption( "-it2",
                                 "Inside second threshold "
                                 "(required for be/bt/oe/ou, default=0.0)",
                                 insideThreshold2,
                                 true );
    application.addSingleOption( "-om",
                                 "Outside threshold mode\n"
                                 " - lt -> lower than\n"
                                 " - le -> lower or equal to\n"
                                 " - eq -> equal to\n"
                                 " - di -> different from\n"
                                 " - ge -> greater or equal to\n"
                                 " - gt -> greater than\n"
                                 " - be -> between or equal\n"
                                 " - bt -> between\n"
                                 " - oe -> outside or equal\n"
                                 " - ou -> outside\n"
                                 "(default='eq')",
                                 outsideMode,
                                 true );
    application.addSingleOption( "-ot1",
                                 "Outside first threshold (default=-1.0)",
                                 outsideThreshold1,
                                 true );
    application.addSingleOption( "-ot2",
                                 "Outside second threshold "
                                 "(required for be/bt/oe/ou, default=0.0)",
                                 outsideThreshold2,
                                 true );
    application.addSingleOption( "-ov",
                                 "Outside value (default=0.0)",
                                 outsideValue,
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

    execute( fileNameIn, fileNameOut, outType, 
             insideMode, insideThreshold1, insideThreshold2,
             outsideMode, outsideThreshold1, outsideThreshold2,
             outsideValue,
             ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VoronoiCommand::parse()" );

}


void gkg::VoronoiCommand::execute( const std::string& fileNameIn,
                    		   const std::string& fileNameOut,
                    		   const std::string& outType,
                                   const std::string& insideMode,
                                   const double& insideThreshold1,
                                   const double& insideThreshold2,
                                   const std::string& outsideMode,
                                   const double& outsideThreshold1,
                                   const double& outsideThreshold2,
                                   const double& outsideValue,
                    		   const bool& ascii,
                    		   const std::string& format,
                    		   const bool& verbose )
{

  try
  {


    if ( ( insideMode != "lt" ) &&
         ( insideMode != "le" ) &&
         ( insideMode != "eq" ) &&
         ( insideMode != "di" ) &&
         ( insideMode != "ge" ) &&
         ( insideMode != "gt" ) &&
         ( insideMode != "be" ) &&
         ( insideMode != "bt" ) &&
         ( insideMode != "oe" ) &&
         ( insideMode != "ou" ) )
    {

      throw std::runtime_error( "invalid inside threshold mode" );

    }

    if ( ( outsideMode != "lt" ) &&
         ( outsideMode != "le" ) &&
         ( outsideMode != "eq" ) &&
         ( outsideMode != "di" ) &&
         ( outsideMode != "ge" ) &&
         ( outsideMode != "gt" ) &&
         ( outsideMode != "be" ) &&
         ( outsideMode != "bt" ) &&
         ( outsideMode != "oe" ) &&
         ( outsideMode != "ou" ) )
    {

      throw std::runtime_error( "invalid outside threshold mode" );

    }

    if ( !outType.empty() &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }

    //
    // launching process
    //
    VoronoiProcess voronoiProcess( fileNameOut,
                                   outType,
                                   insideMode,
                                   insideThreshold1,
                                   insideThreshold2,
                                   outsideMode,
                                   outsideThreshold1,
                                   outsideThreshold2,
                                   outsideValue,
                                   ascii,
                                   format,
                                   verbose );
    voronoiProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::VoronoiCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& outType, "
             "const std::string& insideMode, "
             "const double& insideThreshold1, "
             "const double& insideThreshold2, "
             "const std::string& outsideMode, "
             "const double& outsideThreshold1, "
             "const double& outsideThreshold2, "
             "const double& outsideValue, "
             "const bool& ascii, "
             "const std::string& format, "
             "const bool& verbose )" );

}


RegisterCommandCreator( VoronoiCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( outType ) +
    			DECLARE_STRING_PARAMETER_HELP( insideMode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( insideThreshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( insideThreshold2 ) +
    			DECLARE_STRING_PARAMETER_HELP( outsideMode ) +
    			DECLARE_FLOATING_PARAMETER_HELP( outsideThreshold1 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( outsideThreshold2 ) +
    			DECLARE_FLOATING_PARAMETER_HELP( outsideValue ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
