#include <gkg-deep-nuclei-plugin-functors/MidSagittalSurface/MidSagittalSurfaceCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-deep-nuclei-midsagittalsurface/MidSagittalSurface_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>


class MidSagittalSurfaceProcess : public gkg::Process
{

  public:
  
    MidSagittalSurfaceProcess( const std::string& fileNameOut,
                               const std::string& fileNameMask,
                               int32_t slabWidth,
                               double sigma,
                               double minCorrelation,
                               double regularization,
			       const bool& output3d,
                               const bool& ascii,
                               const std::string& format,
                               const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getFileNameMask() const;
    int32_t getSlabWidth() const;
    double getSigma() const;
    double getMinimumCorrelation() const;
    double getRegularization() const;
    const bool& getOutput3d() const;

    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:
  
    template < class T >
    static void midSagittalSurface( gkg::Process& process,
                        	    const std::string& fileNameIn,
                        	    const gkg::AnalyzedObject&,
                        	    const std::string& );

    const std::string& _fileNameOut;
    const std::string& _fileNameMask;
    int32_t _slabWidth;
    double _sigma;
    double _minCorrelation;
    double _regularization;
    const bool& _output3d;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


MidSagittalSurfaceProcess::MidSagittalSurfaceProcess( 
                             	                const std::string& fileNameOut,
                             	                const std::string& fileNameMask,
                                                int32_t slabWidth,
                                                double sigma,
                                                double minCorrelation,
                                                double regularization,
				                const bool& output3d,
                             	                const bool& ascii,
                             	                const std::string& format,
                             	                const bool& verbose )
                         : gkg::Process(),
		           _fileNameOut( fileNameOut ),
		           _fileNameMask( fileNameMask ),
                           _slabWidth( slabWidth ),
                           _sigma( sigma ),
                           _minCorrelation( minCorrelation ),
                           _regularization( regularization ),
		           _output3d( output3d ),
		           _ascii( ascii ),
		           _format( format ),
                           _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MidSagittalSurfaceProcess::midSagittalSurface< double > );

}


const std::string& MidSagittalSurfaceProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& MidSagittalSurfaceProcess::getFileNameMask() const
{

  return _fileNameMask;

}


int32_t MidSagittalSurfaceProcess::getSlabWidth() const
{

  return _slabWidth;

}


double MidSagittalSurfaceProcess::getSigma() const
{

  return _sigma;

}


double MidSagittalSurfaceProcess::getMinimumCorrelation() const
{

  return _minCorrelation;

}


double MidSagittalSurfaceProcess::getRegularization() const
{

  return _regularization;

}


const bool& MidSagittalSurfaceProcess::getOutput3d() const
{

  return _output3d;

}


const bool& MidSagittalSurfaceProcess::getAscii() const
{

  return _ascii;

}


const std::string& MidSagittalSurfaceProcess::getFormat() const
{

  return _format;

}


const bool& MidSagittalSurfaceProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void MidSagittalSurfaceProcess::midSagittalSurface( 
                                                  gkg::Process& process,
 						  const std::string& fileNameIn,
 						  const gkg::AnalyzedObject&,
                                                  const std::string& )
{

  try
  {
  
    MidSagittalSurfaceProcess&
      midSagittalSurfaceProcess = static_cast< MidSagittalSurfaceProcess& >( 
                                                                      process );

    // reading data
    if ( midSagittalSurfaceProcess.getVerbose() )
    {
    
      std::cout << "reading '" << fileNameIn << "' : " << std::flush;
    
    }    
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( midSagittalSurfaceProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
    
    // defining mask
    gkg::Volume< uint8_t > mask;

    if ( midSagittalSurfaceProcess.getFileNameMask().empty() )
    {
    
      if ( midSagittalSurfaceProcess.getVerbose() )
      {
      
        std::cout << "computing mask: " << std::flush;
		  
      }

      float thresholdRatio = 0.02f;
      gkg::Vector3d< double > resolution;
      volumeIn.getResolution( resolution );
      float radius = 2.0 * std::min( resolution.x,
                                     std::min( resolution.y, resolution.z ) );

      gkg::ClosedMask< T, uint8_t > closedMask( radius, thresholdRatio );
      closedMask.getMask( volumeIn, mask, false );

      if ( midSagittalSurfaceProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {
    
      if ( midSagittalSurfaceProcess.getVerbose() )
      {
      
        std::cout << "reading '" << midSagittalSurfaceProcess.getFileNameMask()
                  << "' mask: " << std::flush;
		  
      }
      gkg::Reader::getInstance().read( 
                                    midSagittalSurfaceProcess.getFileNameMask(),
                                    mask );
      if ( midSagittalSurfaceProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      
    }
    
    gkg::Volume< T > volumeOut;
    gkg::MidSagittalSurface< T > mss( 
                              midSagittalSurfaceProcess.getSlabWidth(),
                              midSagittalSurfaceProcess.getSigma(),
                              midSagittalSurfaceProcess.getMinimumCorrelation(),
                              midSagittalSurfaceProcess.getRegularization() );

    mss.compute( volumeIn, 
                 mask, 
                 volumeOut, 
                 midSagittalSurfaceProcess.getOutput3d(),
                 midSagittalSurfaceProcess.getVerbose() );

    // writing data
    if ( midSagittalSurfaceProcess.getVerbose() )
    {

      std::cout << "writing '"
   		<< midSagittalSurfaceProcess.getFileNameOut() 
		<< "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( 
                                     midSagittalSurfaceProcess.getFileNameOut(),
   				     volumeOut,
   				     midSagittalSurfaceProcess.getAscii(),
   				     midSagittalSurfaceProcess.getFormat() );
    if ( midSagittalSurfaceProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void MidSagittalSurfaceProcess::midSagittalSurfaceProcess( "
             "gkg::Process& process, "
 	     "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                const std::string& fileNameMask,
                                                int32_t slabWidth,
                                                double sigma,
                                                double minCorrelation,
                                                double lambda,
			                        bool output3d,
                                                bool ascii,
                                                const std::string& format,
                                                bool verbose )
                              : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, fileNameMask,
             slabWidth, sigma, minCorrelation, lambda,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& fileNameMask, int32_t slabWidth, "
             "double sigma, double minCorrelation, double lambda, "
	     "bool output3d, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand(
                                             const gkg::Dictionary& parameters )
                              : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, slabWidth );
    DECLARE_FLOATING_PARAMETER( parameters, double, sigma );
    DECLARE_FLOATING_PARAMETER( parameters, double, minCorrelation );
    DECLARE_FLOATING_PARAMETER( parameters, double, lambda );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, output3d );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, fileNameMask,
             slabWidth, sigma, minCorrelation, lambda,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MidSagittalSurfaceCommand::MidSagittalSurfaceCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MidSagittalSurfaceCommand::~MidSagittalSurfaceCommand()
{
}


std::string gkg::MidSagittalSurfaceCommand::getStaticName()
{

  try
  {

    return "MidSagittalSurface";

  }
  GKG_CATCH( "std::string gkg::MidSagittalSurfaceCommand::getStaticName()" );

}


void gkg::MidSagittalSurfaceCommand::parse()
{

  try
  {
  
    std::string fileNameIn;
    std::string fileNameOut;
    std::string fileNameMask = "";
    int32_t slabWidth = 30;
    double sigma = 4.0;
    double minCorrelation = 0.9;
    double lambda = 0.3;
    bool output3d = false;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;
    
    gkg::Application application( _argc, _argv,
                                  "Mid sagittal Surface",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
    application.addSingleOption( "-m",
                                 "Mask (default=ad-hoc computation)",
                                 fileNameMask,
                                 true );
    application.addSingleOption( "-w",
                                 "Slab width (default=30)",
                                 slabWidth,
                                 true );
    application.addSingleOption( "-s",
                                 "Deriche smoothing filter sigma "
                                 "(default=4.0)",
                                 sigma,
                                 true );
    application.addSingleOption( "-c",
                                 "Minimum correlation value "
                                 "(default=0.9)",
                                 minCorrelation,
                                 true );
    application.addSingleOption( "-l",
                                 "Thin plate spline regularization "
                                 "(default=0.3)",
                                 lambda,
                                 true );
    application.addSingleOption( "-3d",
                                 "Output Surface in a 3D volume",
                                 output3d, 
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

    execute( fileNameIn, fileNameOut, fileNameMask,
             slabWidth, sigma, minCorrelation, lambda,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MidSagittalSurfaceCommand::parse()" );

}


void gkg::MidSagittalSurfaceCommand::execute( const std::string& fileNameIn,
                                              const std::string& fileNameOut,
                                              const std::string& fileNameMask,
                                              int32_t slabWidth,
                                              double sigma,
                                              double minCorrelation,
                                              double lambda,
		                              bool output3d,
                                              bool ascii,
                                              const std::string& format,
                                              bool verbose )
{

  try
  {

    MidSagittalSurfaceProcess midSagittalSurfaceProcess( fileNameOut,
                                                         fileNameMask,
                                                         slabWidth,
                                                         sigma,
                                                         minCorrelation,
                                                         lambda,
						         output3d,
						         ascii,
						         format,
						         verbose );
    midSagittalSurfaceProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::MidSagittalSurfaceCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "const std::string& fileNameMask, int32_t slabWidth, "
             "double sigma, double minCorrelation, double lambda, "
	     "bool output3d, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( MidSagittalSurfaceCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_INTEGER_PARAMETER_HELP( slabWidth ) +
    			DECLARE_FLOATING_PARAMETER_HELP( sigma ) +
    			DECLARE_FLOATING_PARAMETER_HELP( minCorrelation ) +
    			DECLARE_FLOATING_PARAMETER_HELP( lambda ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( output3d ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
