#include <gkg-deep-nuclei-plugin-functors/MidSagittalPlane/MidSagittalPlaneCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Thresholder_i.h>
#include <gkg-deep-nuclei-midsagittalplane/MidSagittalPlane_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>


class MidSagittalPlaneProcess : public gkg::Process
{

  public:
  
    MidSagittalPlaneProcess( const std::string& fileNameOut,
                             const std::string& fileNameMask,
                             const std::string& fileNameTransformation,
                             const std::string& fileNamePlane,
			     const int32_t& degreeOfFreedom,
			     const bool& output3d,
                             const bool& ascii,
                             const std::string& format,
                             const bool& verbose );

    const std::string& getFileNameOut() const;
    const std::string& getFileNameMask() const;
    const std::string& getFileNameTransformation() const;
    const std::string& getFileNamePlane() const;
    const int32_t& getDegreeOfFreedom() const;
    const bool& getOutput3d() const;

    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:
  
    template < class T >
    static void midSagittalPlane( gkg::Process& process,
                        	  const std::string& fileNameIn,
                        	  const gkg::AnalyzedObject&,
                        	  const std::string& );

    const std::string& _fileNameOut;
    const std::string& _fileNameMask;
    const std::string& _fileNameTransformation;
    const std::string& _fileNamePlane;
    const int32_t& _degreeOfFreedom;
    const bool& _output3d;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


MidSagittalPlaneProcess::MidSagittalPlaneProcess( 
                             	      const std::string& fileNameOut,
                             	      const std::string& fileNameMask,
                                      const std::string& fileNameTransformation,
                                      const std::string& fileNamePlane,
				      const int32_t& degreeOfFreedom,
				      const bool& output3d,
                             	      const bool& ascii,
                             	      const std::string& format,
                             	      const bool& verbose )
                       : gkg::Process(),
		         _fileNameOut( fileNameOut ),
			 _fileNameMask( fileNameMask ),
                         _fileNameTransformation( fileNameTransformation ),
                         _fileNamePlane( fileNamePlane ),
			 _degreeOfFreedom( degreeOfFreedom ),
			 _output3d( output3d ),
			 _ascii( ascii ),
			 _format( format ),
                         _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &MidSagittalPlaneProcess::midSagittalPlane< double > );

}


const std::string& MidSagittalPlaneProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const std::string& MidSagittalPlaneProcess::getFileNameMask() const
{

  return _fileNameMask;

}


const std::string& MidSagittalPlaneProcess::getFileNameTransformation() const
{

  return _fileNameTransformation;

}


const std::string& MidSagittalPlaneProcess::getFileNamePlane() const
{

  return _fileNamePlane;

}


const int32_t& MidSagittalPlaneProcess::getDegreeOfFreedom() const
{

  return _degreeOfFreedom;

}


const bool& MidSagittalPlaneProcess::getOutput3d() const
{

  return _output3d;

}


const bool& MidSagittalPlaneProcess::getAscii() const
{

  return _ascii;

}


const std::string& MidSagittalPlaneProcess::getFormat() const
{

  return _format;

}


const bool& MidSagittalPlaneProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void MidSagittalPlaneProcess::midSagittalPlane( gkg::Process& process,
 						const std::string& fileNameIn,
 						const gkg::AnalyzedObject&,
 						const std::string& )
{

  try
  {
  
    MidSagittalPlaneProcess&
      midSagittalPlaneProcess = static_cast< MidSagittalPlaneProcess& >( 
                                                                      process );  

    // reading data
    if ( midSagittalPlaneProcess.getVerbose() )
    {
    
      std::cout << "reading '" << fileNameIn << "' : " << std::flush;
    
    }    
    gkg::Volume< T > volumeIn;
    gkg::Reader::getInstance().read( fileNameIn, volumeIn );
    if ( midSagittalPlaneProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }
    
    // defining mask
    gkg::Volume< T > mask;
    if ( midSagittalPlaneProcess.getFileNameMask().empty() )
    {
    
      if ( midSagittalPlaneProcess.getVerbose() )
      {
      
        std::cout << "computing mask: " << std::flush;
		  
      }

      T lowerBoundary, upperBoundary;
      gkg::HistogramAnalyzer::getInstance().getPercentileLowerBoundaries(
                                                                volumeIn,
                                                                512,
                                                                80.0,
                                                                lowerBoundary,
                                                                upperBoundary );
      gkg::GreaterOrEqualToFunction< T > geFunc( lowerBoundary );
      gkg::Thresholder< gkg::Volume< T > > thresholder( geFunc, 0 );

      thresholder.threshold( volumeIn, mask );

      if ( midSagittalPlaneProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {
    
      if ( midSagittalPlaneProcess.getVerbose() )
      {
      
        std::cout << "reading '" << midSagittalPlaneProcess.getFileNameMask()
                  << "' mask: " << std::flush;
		  
      }
      gkg::Reader::getInstance().read( 
                                      midSagittalPlaneProcess.getFileNameMask(),
                                      mask );
      if ( midSagittalPlaneProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }
      
    }
    
    gkg::Volume< T > volumeOut;
    gkg::MidSagittalPlane< T > midSagittalPlane( 
                                 midSagittalPlaneProcess.getDegreeOfFreedom() );
    midSagittalPlane.compute( volumeIn, 
                              volumeOut, 
			      mask, 
			      midSagittalPlaneProcess.getOutput3d(),
			      midSagittalPlaneProcess.getVerbose() );
    
    // writing data
    if ( midSagittalPlaneProcess.getVerbose() )
    {

      std::cout << "writing '"
   		<< midSagittalPlaneProcess.getFileNameOut() 
		<< "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( midSagittalPlaneProcess.getFileNameOut(),
   				      volumeOut,
   				      midSagittalPlaneProcess.getAscii(),
   				      midSagittalPlaneProcess.getFormat() );
    if ( midSagittalPlaneProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }

    if ( !midSagittalPlaneProcess.getFileNameTransformation().empty() )
    {

      std::ofstream ofs( 
                  midSagittalPlaneProcess.getFileNameTransformation().c_str() );

      if ( ofs.good() )
      {

        if ( midSagittalPlaneProcess.getVerbose() )
        {

          std::cout << "writing '"
   	  	    << midSagittalPlaneProcess.getFileNameTransformation() 
		   << "' : " << std::flush;

        }

        midSagittalPlane.getTransformation().writeTrm( ofs );

        if ( midSagittalPlaneProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

        ofs.close();

      }

    }

    if ( !midSagittalPlaneProcess.getFileNamePlane().empty() )
    {

      std::ofstream ofs( midSagittalPlaneProcess.getFileNamePlane().c_str() );

      if ( ofs.good() )
      {

        if ( midSagittalPlaneProcess.getVerbose() )
        {

          std::cout << "writing '"
   	  	    << midSagittalPlaneProcess.getFileNamePlane() 
		   << "' : " << std::flush;

        }

        std::vector< double > planeCoefficients =
                                             midSagittalPlane.getCoefficients();
        std::vector< double >::const_iterator
          c = planeCoefficients.begin(),
          ce = planeCoefficients.end();

        while ( c != ce )
        {

          ofs << *c << std::endl;
          ++c;

        }

        if ( midSagittalPlaneProcess.getVerbose() )
        {

          std::cout << "done" << std::endl;

        }

        ofs.close();

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void MidSagittalPlaneProcess::midSagittalPlaneProcess( "
             "gkg::Process& process, "
 	     "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand(
                                      const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& fileNameMask,
                                      const std::string& fileNameTransformation,
                                      const std::string& fileNamePlane,
			              int32_t degreeOfFreedom,
			              bool output3d,
                                      bool ascii,
                                      const std::string& format,
                                      bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, fileNameMask,
             fileNameTransformation, fileNamePlane, degreeOfFreedom,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameTransformation, "
             "const std::string& fileNamePlane, "
	     "int32_t degreeOfFreedom, bool output3d, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand(
                                             const gkg::Dictionary& parameters )
                            : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTransformation );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNamePlane );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, degreeOfFreedom );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, output3d );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, fileNameMask,
             fileNameTransformation, fileNamePlane, degreeOfFreedom,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::MidSagittalPlaneCommand::MidSagittalPlaneCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MidSagittalPlaneCommand::~MidSagittalPlaneCommand()
{
}


std::string gkg::MidSagittalPlaneCommand::getStaticName()
{

  try
  {

    return "MidSagittalPlane";

  }
  GKG_CATCH( "std::string gkg::MidSagittalPlaneCommand::getStaticName()" );

}


void gkg::MidSagittalPlaneCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    std::string fileNameMask = "";
    std::string fileNameTransformation = "";
    std::string fileNamePlane = "";
    int32_t degreeOfFreedom = 3;
    bool output3d = false;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;
    
    gkg::Application application( _argc, _argv,
                                  "Mid sagittal plane",
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
    application.addSingleOption( "-t",
                                 "Output transformation file name",
                                 fileNameTransformation,
                                 true );
    application.addSingleOption( "-p",
                                 "Output plane coefficients file name",
                                 fileNamePlane,
                                 true );
    application.addSingleOption( "-d",
                                 "Degree of freedom "
				 "3 (Ry,Rz,Tx) or 6 (all) (default=3)",
                                 degreeOfFreedom,
                                 true );
    application.addSingleOption( "-3d",
                                 "Output plane in a 3D volume",
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
             fileNameTransformation, fileNamePlane, degreeOfFreedom,
             output3d, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::MidSagittalPlaneCommand::parse()" );

}


void gkg::MidSagittalPlaneCommand::execute(
                                      const std::string& fileNameIn,
                                      const std::string& fileNameOut,
                                      const std::string& fileNameMask,
                                      const std::string& fileNameTransformation,
                                      const std::string& fileNamePlane,
		                      int32_t degreeOfFreedom,
		                      bool output3d,
                                      bool ascii,
                                      const std::string& format,
                                      bool verbose )
{

  try
  {

    MidSagittalPlaneProcess midSagittalPlaneProcess( fileNameOut,
                                                     fileNameMask,
                                                     fileNameTransformation,
                                                     fileNamePlane,
						     degreeOfFreedom,
						     output3d,
						     ascii,
						     format,
						     verbose );
    midSagittalPlaneProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::MidSagittalPlaneCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameTransformation, "
             "const std::string& fileNamePlane, "
	     "int32_t degreeOfFreedom, bool output3d, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
                       MidSagittalPlaneCommand,
    		       DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNameTransformation ) +
    		       DECLARE_STRING_PARAMETER_HELP( fileNamePlane ) +
    		       DECLARE_INTEGER_PARAMETER_HELP( degreeOfFreedom ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( output3d ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		       DECLARE_STRING_PARAMETER_HELP( format ) +
    		       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
