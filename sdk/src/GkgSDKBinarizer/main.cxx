#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <string>
#include <iostream>


class BinarizerProcess : public gkg::Process
{

  public:

    BinarizerProcess( const std::string& fileNameOut,
                      const std::string& outType,
                      const std::string& mode,
                      const double& threshold1,
                      const double& threshold2,
                      const double& foreground,
                      const double& background,
                      const bool& ascii,
                      const std::string& format,
                      const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getOutType() const;
    const std::string& getMode() const;
    const double& getThreshold1() const;
    const double& getThreshold2() const;
    const double& getForeground() const;
    const double& getBackground() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void binarize( gkg::Process& process,
                          const std::string& fileNameIn,
                          const gkg::AnalyzedObject&,
                          const std::string& );

    const std::string& _fileNameOut;
    const std::string& _outType;
    const std::string& _mode;
    const double& _threshold1;
    const double& _threshold2;
    const double& _foreground;
    const double& _background;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


BinarizerProcess::BinarizerProcess( const std::string& fileNameOut,
                                    const std::string& outType,
                                    const std::string& mode,
                                    const double& threshold1,
                                    const double& threshold2,
                                    const double& foreground,
                                    const double& background,
                                    const bool& ascii,
                                    const std::string& format,
                                    const bool& verbose)
                 : gkg::Process( "Volume" ),
                   _fileNameOut( fileNameOut ),
                   _outType( outType ),
                   _mode( mode ),
                   _threshold1( threshold1 ),
                   _threshold2( threshold2 ),
                   _foreground( foreground ),
                   _background( background ),
                   _ascii( ascii ),
                   _format( format ),
                   _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &BinarizerProcess::binarize< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &BinarizerProcess::binarize< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &BinarizerProcess::binarize< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &BinarizerProcess::binarize< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &BinarizerProcess::binarize< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &BinarizerProcess::binarize< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &BinarizerProcess::binarize< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &BinarizerProcess::binarize< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &BinarizerProcess::binarize< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &BinarizerProcess::binarize< double > );

}


const std::string& BinarizerProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& BinarizerProcess::getOutType() const
{

  return _outType;

}


const std::string& BinarizerProcess::getMode() const
{

  return _mode;

}


const double& BinarizerProcess::getThreshold1() const
{

  return _threshold1;

}


const double& BinarizerProcess::getThreshold2() const
{

  return _threshold2;

}


const double& BinarizerProcess::getForeground() const
{

  return _foreground;

}


const double& BinarizerProcess::getBackground() const
{

  return _background;

}


const bool& BinarizerProcess::getAscii() const
{

  return _ascii;

}


const std::string& BinarizerProcess::getFormat() const
{

  return _format;

}


const bool& BinarizerProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void BinarizerProcess::binarize( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    BinarizerProcess&
      binarizerProcess = static_cast< BinarizerProcess& >( process );

    // reading data
    if ( binarizerProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( binarizerProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( binarizerProcess.getVerbose() )
    {

      std::cout << "binarizing : " << std::flush;

    }

    gkg::TestFunction< T >* testFunction =
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                        binarizerProcess.getMode(),
			                ( T )binarizerProcess.getThreshold1(),
				        ( T )binarizerProcess.getThreshold2() );

    if ( binarizerProcess.getOutType().empty() )
    {

      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< T > >
        binarizer( *testFunction,
                   ( T )binarizerProcess.getForeground(),
                   ( T )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeIn );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeIn,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    if ( gkg::TypeOf< int8_t >::isSameType( binarizerProcess.getOutType() ) )
    {

      gkg::Volume< int8_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int8_t > >
        binarizer( *testFunction,
                   ( int8_t )binarizerProcess.getForeground(),
                   ( int8_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint8_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< uint8_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint8_t > >
        binarizer( *testFunction,
                   ( uint8_t )binarizerProcess.getForeground(),
                   ( uint8_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int16_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< int16_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int16_t > >
        binarizer( *testFunction,
                   ( int16_t )binarizerProcess.getForeground(),
                   ( int16_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint16_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< uint16_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint16_t > >
        binarizer( *testFunction,
                   ( uint16_t )binarizerProcess.getForeground(),
                   ( uint16_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int32_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< int32_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int32_t > >
        binarizer( *testFunction,
                   ( int32_t )binarizerProcess.getForeground(),
                   ( int32_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint32_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< uint32_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint32_t > >
        binarizer( *testFunction,
                   ( uint32_t )binarizerProcess.getForeground(),
                   ( uint32_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< int64_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< int64_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< int64_t > >
        binarizer( *testFunction,
                   ( int64_t )binarizerProcess.getForeground(),
                   ( int64_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< uint64_t >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< uint64_t > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< uint64_t > >
        binarizer( *testFunction,
                   ( uint64_t )binarizerProcess.getForeground(),
                   ( uint64_t )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< float >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< float > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< float > >
        binarizer( *testFunction,
                   ( float )binarizerProcess.getForeground(),
                   ( float )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else if ( gkg::TypeOf< double >::isSameType( 
                                               binarizerProcess.getOutType() ) )
    {

      gkg::Volume< double > volumeOut;
      gkg::Binarizer< gkg::Volume< T >, gkg::Volume< double > >
        binarizer( *testFunction,
                   ( double )binarizerProcess.getForeground(),
                   ( double )binarizerProcess.getBackground() );
      binarizer.binarize( volumeIn, volumeOut );

      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      // writing data
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "writing '"
                  << binarizerProcess.getFileNameOut() << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( binarizerProcess.getFileNameOut(),
                                        volumeOut,
                                        binarizerProcess.getAscii(),
                                        binarizerProcess.getFormat() );
      if ( binarizerProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }

    delete testFunction;

  }
  GKG_CATCH( "template < class T > "
             "void BinarizerProcess::binarize( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string outType = "";
    std::string mode;
    double threshold1;
    double threshold2 = 0.0;
    double foreground = 1.0;
    double background = 0.0;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    std::cout << "Build from SDK" << std::endl;

    gkg::Application application( argc, argv,
                                  "Conditional binarization operation "
                                  "of container" );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Output type (default=input type)",
                                 outType,
                                 true );
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
    application.addSingleOption( "-f",
                                 "Foreground value (default=1)",
                                 foreground,
                                 true );
    application.addSingleOption( "-b",
                                 "Background value (default=0)",
                                 background,
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

      throw std::runtime_error( "invalid threshold mode" );

    }
         
    if ( !outType.empty() &&
         ( ! gkg::TypeOf< int8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint8_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint16_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint32_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< int64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< uint64_t >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< float >::isSameType( outType ) ) &&
         ( ! gkg::TypeOf< double >::isSameType( outType ) ) )
    {

      throw std::runtime_error( "invalid output type" );

    }
         

    //
    // launching binarizer process
    //
    BinarizerProcess binarizerProcess( fileNameOut,
                                       outType,
                                       mode,
                                       threshold1,
                                       threshold2,
                                       foreground,
                                       background,
                                       ascii,
                                       format,
                                       verbose );
    binarizerProcess.execute( fileNameIn );

  }
  GKG_CATCH_COMMAND( result );

  return result;

}

