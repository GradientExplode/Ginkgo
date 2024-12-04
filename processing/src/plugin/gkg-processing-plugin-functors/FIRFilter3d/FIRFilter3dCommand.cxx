#include <gkg-processing-plugin-functors/FIRFilter3d/FIRFilter3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-signal/ApodizationFactory.h>
#include <gkg-processing-signal/FIRFilter3d_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class FIRFilter3dProcess : public gkg::Process
{

  public:

    FIRFilter3dProcess( const std::string& fileNameOut,
                        const std::string& filterType,
                        const int32_t& filterSize,
                        const std::vector< double >& frequencies,
                        const std::string& apodizationName,
                        const std::string& axes,
                        const bool& ascii,
                        const std::string& format,
                        const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getFilterType() const;
    const int32_t& getFilterSize() const;
    const std::vector< double >& getFrequencies() const;
    const std::string& getApodizationName() const;
    const std::string& getAxes() const;
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
    const std::string& _filterType;
    const int32_t& _filterSize;
    const std::vector< double >& _frequencies;
    const std::string& _apodizationName;
    const std::string& _axes;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


FIRFilter3dProcess::FIRFilter3dProcess( const std::string& fileNameOut,
                                        const std::string& filterType,
                                        const int32_t& filterSize,
                                        const std::vector< double>& frequencies,
                                        const std::string& apodizationName,
                                        const std::string& axes,
                                        const bool& ascii,
                                        const std::string& format,
                                        const bool& verbose )
                   : gkg::Process(),
                     _fileNameOut( fileNameOut ),
                     _filterType( filterType ),
                     _filterSize( filterSize ),
                     _frequencies( frequencies ),
                     _apodizationName( apodizationName ),
                     _axes( axes ),
                     _ascii( ascii ),
                     _format( format ),
                     _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &FIRFilter3dProcess::filter< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &FIRFilter3dProcess::filter< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &FIRFilter3dProcess::filter< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &FIRFilter3dProcess::filter< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &FIRFilter3dProcess::filter< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &FIRFilter3dProcess::filter< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &FIRFilter3dProcess::filter< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &FIRFilter3dProcess::filter< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &FIRFilter3dProcess::filter< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &FIRFilter3dProcess::filter< double > );

}


const std::string& FIRFilter3dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& FIRFilter3dProcess::getFilterType() const
{

  return _filterType;

}


const int32_t& FIRFilter3dProcess::getFilterSize() const
{

  return _filterSize;

}


const std::vector< double >& FIRFilter3dProcess::getFrequencies() const
{

  return _frequencies;

}


const std::string& FIRFilter3dProcess::getApodizationName() const
{

  return _apodizationName;

}


const std::string& FIRFilter3dProcess::getAxes() const
{

  return _axes;

}


const bool& FIRFilter3dProcess::getAscii() const
{

  return _ascii;

}


const std::string& FIRFilter3dProcess::getFormat() const
{

  return _format;

}


const bool& FIRFilter3dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void FIRFilter3dProcess::filter( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    FIRFilter3dProcess&
      firFilter3dProcess = static_cast< FIRFilter3dProcess& >( process );

    // reading data
    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "filtering : " << std::flush;

    }

    typename gkg::FIRFilter3d< T >::Type type;
    if ( firFilter3dProcess.getFilterType() == "lp" )
    {

      type = gkg::FIRFilter3d< T >::LowPass;

    }
    else if ( firFilter3dProcess.getFilterType() == "hp" )
    {

      type = gkg::FIRFilter3d< T >::HighPass;

    }
    else if ( firFilter3dProcess.getFilterType() == "bp" )
    {

      type = gkg::FIRFilter3d< T >::BandPass;

    }
    else if ( firFilter3dProcess.getFilterType() == "bs" )
    {

      type = gkg::FIRFilter3d< T >::BandStop;

    }
    else
    {

      throw std::runtime_error( "bad filter type" );

    }

    gkg::FIRFilter3d< T > firFilter3d( type,
                                       firFilter3dProcess.getFilterSize(),
                                       firFilter3dProcess.getFrequencies(),
                                       firFilter3dProcess.getApodizationName(),
                                       firFilter3dProcess.getVerbose() );
    if ( ( firFilter3dProcess.getAxes() == "x" ) ||
         ( firFilter3dProcess.getAxes() == "xy" ) ||
         ( firFilter3dProcess.getAxes() == "xz" ) ||
         ( firFilter3dProcess.getAxes() == "xyz" ) )
    {
         
      firFilter3d.filter( volumeIn, volumeIn, gkg::FIRFilter3d< T >::X );

    }

    if ( ( firFilter3dProcess.getAxes() == "y" ) ||
         ( firFilter3dProcess.getAxes() == "xy" ) ||
         ( firFilter3dProcess.getAxes() == "yz" ) ||
         ( firFilter3dProcess.getAxes() == "xyz" ) )
    {
         
      firFilter3d.filter( volumeIn, volumeIn, gkg::FIRFilter3d< T >::Y );

    }

    if ( ( firFilter3dProcess.getAxes() == "z" ) ||
         ( firFilter3dProcess.getAxes() == "xz" ) ||
         ( firFilter3dProcess.getAxes() == "yz" ) ||
         ( firFilter3dProcess.getAxes() == "xyz" ) )
    {
         
      firFilter3d.filter( volumeIn, volumeIn, gkg::FIRFilter3d< T >::Z );

    }

    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // writing data
    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "writing '"
                << firFilter3dProcess.getFileNameOut() << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( firFilter3dProcess.getFileNameOut(),
                                      volumeIn,
                                      firFilter3dProcess.getAscii(),
                                      firFilter3dProcess.getFormat() );
    if ( firFilter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void FIRFilter3dProcess::filter( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   FIRFilter3dCommand
//


gkg::FIRFilter3dCommand::FIRFilter3dCommand( int32_t argc,
                                 	     char* argv[],
                                 	     bool loadPlugin,
                                 	     bool removeFirst )
                       : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::FIRFilter3dCommand::FIRFilter3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::FIRFilter3dCommand::FIRFilter3dCommand(
                        	       const std::string& fileNameIn,
    				       const std::string& fileNameOut,
    				       const std::string& filterType,
    				       int32_t filterSize,
    				       const std::vector< double >& frequencies,
    				       const std::string& apodizationName,
    				       const std::string& axes,
    				       bool ascii,
    				       const std::string& format,
    				       bool verbose )
                       : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, filterType, filterSize, frequencies,
             apodizationName, axes, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::FIRFilter3dCommand::FIRFilter3dCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "const std::string& filterType, int32_t filterSize, "
    	     "const std::vector< double >& frequencies, "
    	     "const std::string& apodizationName, const std::string& axes, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


gkg::FIRFilter3dCommand::FIRFilter3dCommand( const gkg::Dictionary& parameters )
                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, filterType );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, filterSize );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           frequencies );
    DECLARE_STRING_PARAMETER( parameters, std::string, apodizationName );
    DECLARE_STRING_PARAMETER( parameters, std::string, axes );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, filterType, filterSize, frequencies,
             apodizationName, axes, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::FIRFilter3dCommand::FIRFilter3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::FIRFilter3dCommand::~FIRFilter3dCommand()
{
}


std::string gkg::FIRFilter3dCommand::getStaticName()
{

  try
  {

    return "FIRFilter3d";

  }
  GKG_CATCH( "std::string gkg::FIRFilter3dCommand::getStaticName()" );

}


void gkg::FIRFilter3dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string filterType = "lp";
    int32_t filterSize;
    std::vector< double > frequencies; 
    std::string apodizationName = "tukey";
    std::string axes = "xyz";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    std::string apodizationNames;
    std::list< std::string > apodizationNameList = 
      gkg::ApodizationFactory< float >::getInstance().getNames();
    std::list< std::string >::const_iterator n = apodizationNameList.begin(),
                                             ne = apodizationNameList.end();
    while ( n != ne )
    {

      apodizationNames += *n + '/';
      ++ n;

    }
    apodizationNames = apodizationNames.substr( 0,
                                                apodizationNames.length() - 1 );


    gkg::Application application( _argc, _argv,
                                  "3D Finite Impulse Response filtering",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-t",
                                 "Filter type: \n"
                                 "- lp -> Low Pass\n"
                                 "- hp -> High Pass\n"
                                 "- bp -> Band Pass\n"
                                 "- bs -> Band Stop\n"
                                 "(default=lp)",
                                 filterType,
                                 true );
    application.addSingleOption( "-s",
                                 "Filter size (default=21)",
                                 filterSize,
                                 true );
    application.addSeriesOption( "-f",
                                 "Frequencies depending on the filter type:\n"
                                 "- lp -> low pass cut frequency\n"
                                 "- hp -> high pass cut frequency\n"
                                 "- bp -> band pass low and high frequencies\n"
                                 "- bp -> band stop low and high frequencies",
                                 frequencies,
                                 1 );
    application.addSingleOption( "-apodization",
                                 std::string( "Apodization type among " ) +
                                 apodizationNames + " (default=tukey)",
                                 apodizationName,
                                 true );
    application.addSingleOption( "-axes",
                                 "Filter along given axes: \n"
                                 "- x\n"
                                 "- y\n"
                                 "- z\n"
                                 "- xy\n"
                                 "- xz\n"
                                 "- yz\n"
                                 "- xyz\n"
                                 "(default=xyz)",
                                 axes,
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

    execute( fileNameIn, fileNameOut, filterType, filterSize, frequencies,
             apodizationName, axes, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::FIRFilter3dCommand::parse()" );

}


void gkg::FIRFilter3dCommand::execute( const std::string& fileNameIn,
    				       const std::string& fileNameOut,
    				       const std::string& filterType,
    				       int32_t filterSize,
    				       const std::vector< double >& frequencies,
    				       const std::string& apodizationName,
    				       const std::string& axes,
    				       bool ascii,
    				       const std::string& format,
    				       bool verbose )
{

  try
  {

    // sanity checks
    if ( ( filterType != "lp" ) &&
         ( filterType != "hp" ) &&
         ( filterType != "bp" ) &&
         ( filterType != "bs" ) )
    {

      throw std::runtime_error( "invalid filter type" );

    }

    //
    // launching 3D FIR filter process
    //
    FIRFilter3dProcess firFilter3dProcess( fileNameOut,
                                           filterType,
                                           filterSize,
                                           frequencies,
                                           apodizationName,
                                           axes,
                                           ascii,
                                           format,
                                           verbose );
    firFilter3dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::FIRFilter3dCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
    	     "const std::string& filterType, int32_t filterSize, "
    	     "const std::vector< double >& frequencies, "
    	     "const std::string& apodizationName, const std::string& axes, "
    	     "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                     FIRFilter3dCommand,
                     DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                     DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                     DECLARE_STRING_PARAMETER_HELP( filterType ) +
                     DECLARE_INTEGER_PARAMETER_HELP( filterSize ) +
                     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( frequencies ) +
    		     DECLARE_STRING_PARAMETER_HELP( apodizationName ) + 
    		     DECLARE_STRING_PARAMETER_HELP( axes ) +	        
    		     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +	        
    		     DECLARE_STRING_PARAMETER_HELP( format ) +	        
    		     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );       
