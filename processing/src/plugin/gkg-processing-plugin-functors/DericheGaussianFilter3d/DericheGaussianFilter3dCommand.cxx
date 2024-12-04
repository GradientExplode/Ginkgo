#include <gkg-processing-plugin-functors/DericheGaussianFilter3d/DericheGaussianFilter3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-signal/DericheGaussianFilter3d.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


class DericheGaussianFilter3dProcess : public gkg::Process
{

  public:

    DericheGaussianFilter3dProcess( const std::string& fileNameOut,
                                    const float& filteringFWHM,
                                    const std::string& filteringAxis,
                                    const std::string& filteringType,
                                    const bool& ascii,
                                    const std::string& format,
                                    const bool& verbose );

    const std::string& getFileNameOut() const;
    const float& getFilteringFWHM() const;
    const std::string& getFilteringAxis() const;
    const std::string& getFilteringType() const;
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
    const float& _filteringFWHM;
    const std::string& _filteringAxis;
    const std::string& _filteringType;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


DericheGaussianFilter3dProcess::DericheGaussianFilter3dProcess( 
                                               const std::string& fileNameOut,
                                               const float& filteringFWHM,
                                               const std::string& filteringAxis,
                                               const std::string& filteringType,
                                               const bool& ascii,
                                               const std::string& format,
                                               const bool& verbose )
                              : gkg::Process(),
                                _fileNameOut( fileNameOut ),
                                _filteringFWHM( filteringFWHM ),
                                _filteringAxis( filteringAxis ),
                                _filteringType( filteringType ),
                                _ascii( ascii ),
                                _format( format ),
                                _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &DericheGaussianFilter3dProcess::filter< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &DericheGaussianFilter3dProcess::filter< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &DericheGaussianFilter3dProcess::filter< double > );

}


const std::string& DericheGaussianFilter3dProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const float& DericheGaussianFilter3dProcess::getFilteringFWHM() const
{

  return _filteringFWHM;

}


const std::string& DericheGaussianFilter3dProcess::getFilteringAxis() const
{

  return _filteringAxis;

}


const std::string& DericheGaussianFilter3dProcess::getFilteringType() const
{

  return _filteringType;

}


const bool& DericheGaussianFilter3dProcess::getAscii() const
{

  return _ascii;

}


const std::string& DericheGaussianFilter3dProcess::getFormat() const
{

  return _format;

}


const bool& DericheGaussianFilter3dProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void DericheGaussianFilter3dProcess::filter( gkg::Process& process,
                                             const std::string& fileNameIn,
                                             const gkg::AnalyzedObject&,
                                             const std::string& )
{

  try
  {

    DericheGaussianFilter3dProcess&
      filter3dProcess = static_cast< DericheGaussianFilter3dProcess& >( 
                                                                      process );

    // reading data
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "reading '" << fileNameIn << "' : " << std::flush;

    }
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    // filtering
    gkg::Volume< T > volumeOut;
    gkg::DericheGaussianCoefficients< float >::Order order =
                           gkg::DericheGaussianCoefficients< float >::Smoothing;
    double filteringKernelSize = filter3dProcess.getFilteringFWHM() / 
                                 ( 2.0 * std::sqrt( 2.0 * std::log( 2.0 ) ) );
    std::string filteringType = filter3dProcess.getFilteringType();
    std::string filteringAxis = filter3dProcess.getFilteringAxis();

    if ( filteringType == "gradient" )
    {

      order = gkg::DericheGaussianCoefficients< float >::Gradient;

    }
    else if ( filteringType == "laplacian" )
    {

      order = gkg::DericheGaussianCoefficients< float >::Laplacian;

    }

    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "filtering with a " << filteringKernelSize
                << " mm " << filteringType 
                << " Gaussian kernel : " << std::flush;

    }

    gkg::DericheGaussianCoefficients< float > dericheGaussianCoefficients(
                                                           filteringKernelSize, 
                                                           order );             

    if ( filteringAxis.find( "x" ) != std::string::npos )
    {

      if ( filter3dProcess.getVerbose() )
      {

         std::cout << "x" << std::flush;

      }
      gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  volumeIn,
                                  dericheGaussianCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::X_AXIS,
                                  volumeOut );

    }
    if ( filteringAxis.find( "y" ) != std::string::npos )
    {

      if ( filter3dProcess.getVerbose() )
      {

         std::cout << "y" << std::flush;

      }
      gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                  volumeOut,
                                  dericheGaussianCoefficients,
                                  gkg::DericheGaussianFilter3d< float >::Y_AXIS,
                                  volumeOut );

    }
    if ( filteringAxis.find( "z" ) != std::string::npos )
    {

      if ( filter3dProcess.getVerbose() )
      {

         std::cout << "z" << std::flush;

      }
      gkg::DericheGaussianFilter3d< float >::getInstance().filter(
                                volumeOut,
                                dericheGaussianCoefficients,
                                gkg::DericheGaussianFilter3d< float >::Z_AXIS,
                                volumeOut );

    }

    if ( filter3dProcess.getVerbose() )
    {

      std::cout << " done" << std::endl;

    }

    // writing data
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "writing '"
                << filter3dProcess.getFileNameOut() << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( filter3dProcess.getFileNameOut(),
                                      volumeOut,
                                      filter3dProcess.getAscii(),
                                      filter3dProcess.getFormat() );
    if ( filter3dProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void DericheGaussianFilter3dProcess::filter( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::DericheGaussianFilter3dCommand::DericheGaussianFilter3dCommand(
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
  GKG_CATCH( "gkg::DericheGaussianFilter3dCommand::"
             "DericheGaussianFilter3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DericheGaussianFilter3dCommand::DericheGaussianFilter3dCommand(
                                    	       const std::string& fileNameIn,
                                    	       const std::string& fileNameOut,
                                    	       float filteringFWHM,
                                    	       const std::string& filteringAxis,
                                    	       const std::string& filteringType,
                                    	       bool ascii,
                                    	       const std::string& format,
                                    	       bool verbose )
                                   : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, filteringFWHM, filteringAxis,
             filteringType, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DericheGaussianFilter3dCommand::"
             "DericheGaussianFilter3dCommand( const std::string& fileNameIn, "
             "const std::string& fileNameOut, float filteringFWHM, "
             "const std::string& filteringAxis, "
             "const std::string& filteringType, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::DericheGaussianFilter3dCommand::DericheGaussianFilter3dCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, float, filteringFWHM );
    DECLARE_STRING_PARAMETER( parameters, std::string, filteringAxis );
    DECLARE_STRING_PARAMETER( parameters, std::string, filteringType );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, filteringFWHM, filteringAxis,
             filteringType, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::DericheGaussianFilter3dCommand::"
             "DericheGaussianFilter3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DericheGaussianFilter3dCommand::~DericheGaussianFilter3dCommand()
{
}


std::string gkg::DericheGaussianFilter3dCommand::getStaticName()
{

  try
  {

    return "DericheGaussianFilter3d";

  }
  GKG_CATCH( "std::string "
             "gkg::DericheGaussianFilter3dCommand::getStaticName()" );

}


void gkg::DericheGaussianFilter3dCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    float filteringFWHM = 1.0;
    std::string filteringAxis = "xyz";
    std::string filteringType = "smoothing";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "3D Deriche Gaussian filtering",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-k",
                                 "Isotropic filtering kernel FWHM in mm"
                                 " (default=1.0)",
                                 filteringFWHM,
                                 true );
    application.addSingleOption( "-a",
                                 "Filtering axis (default=xyz)",
                                 filteringAxis,
                                 true );
    application.addSingleOption( "-f",
                                 "Filter type (default=smoothing) :\n"
                                 "- smoothing\n"
                                 "- gradient\n"
                                 "- laplacian\n",
                                 filteringType,
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

    execute( fileNameIn, fileNameOut, filteringFWHM, filteringAxis,
             filteringType, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DericheGaussianFilter3dCommand::parse()" );

}


void gkg::DericheGaussianFilter3dCommand::execute(
                                               const std::string& fileNameIn,
                                    	       const std::string& fileNameOut,
                                    	       float filteringFWHM,
                                    	       const std::string& filteringAxis,
                                    	       const std::string& filteringType,
                                    	       bool ascii,
                                    	       const std::string& format,
                                    	       bool verbose )
{

  try
  {

    // sanity checks
    if ( ( filteringType != "smoothing" ) &&
         ( filteringType != "gradient" ) &&
         ( filteringType != "laplacian"  ) )
    {

      throw std::runtime_error( "invalid filtering type" );

    }

    if ( filteringFWHM < 0 )
    {

      throw std::runtime_error( "filtering kernel size must be positive" );

    }

    if ( ( filteringAxis != "xyz" ) &&
         ( filteringAxis != "xy" ) &&
         ( filteringAxis != "xz" ) &&
         ( filteringAxis != "yz" ) &&
         ( filteringAxis != "x" ) &&
         ( filteringAxis != "y" ) &&
         ( filteringAxis != "z" ) )
    {

      throw std::runtime_error( "not a valid filtering axis set" );


    }

    //
    // launching Deriche Gaussian 3D filter process
    //
    DericheGaussianFilter3dProcess filter3dProcess( fileNameOut,
                                                    filteringFWHM,
                                                    filteringAxis,
                                                    filteringType,
                                                    ascii,
                                                    format,
                                                    verbose );
    filter3dProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::DericheGaussianFilter3dCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "float filteringFWHM, "
             "const std::string& filteringAxis, "
             "const std::string& filteringType, "
             "bool ascii, const std::string& format, "
             "bool verbose)" );

}


RegisterCommandCreator( DericheGaussianFilter3dCommand,
                        DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
                        DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                        DECLARE_FLOATING_PARAMETER_HELP(filteringFWHM ) +
                        DECLARE_STRING_PARAMETER_HELP( filteringAxis ) +
                        DECLARE_STRING_PARAMETER_HELP( filteringType ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
                        DECLARE_STRING_PARAMETER_HELP( format ) +
                        DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
