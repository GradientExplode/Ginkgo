#include <gkg-processing-plugin-functors/VolumeToSlabSplitter/VolumeToSlabSplitterCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <sstream>


class VolumeToSlabProcess : public gkg::Process
{

  public:

    VolumeToSlabProcess( const std::string& fileNameOut,
                         int32_t slabCount, 
		         bool ascii,
                         const std::string& format,
                         bool verbose );

    const std::string& getFileNameOut() const;
    int32_t getSlabCount() const;
    bool getAscii() const;
    const std::string& getFormat() const;
    bool getVerbose() const;

  private:

    template < class T >
    static void split( gkg::Process& process,
                       const std::string& fileNameIn,
                       const gkg::AnalyzedObject&,
                       const std::string& );

    const std::string& _fileNameOut;
    int32_t _slabCount;
    bool _ascii;
    std::string _format;
    bool _verbose;

};


VolumeToSlabProcess::VolumeToSlabProcess( const std::string& fileNameOut,
                                          int32_t slabCount, 
                                          bool ascii,
                                          const std::string& format,
                                          bool verbose )
                   : gkg::Process( "Volume" ),
                     _fileNameOut( fileNameOut ),
                     _slabCount( slabCount ),
                     _ascii( ascii ),
                     _format( format ),
                     _verbose( verbose )
{

  registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                   &VolumeToSlabProcess::split< int8_t > );
  registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                   &VolumeToSlabProcess::split< uint8_t > );
  registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                   &VolumeToSlabProcess::split< int16_t > );
  registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                   &VolumeToSlabProcess::split< uint16_t > );
  registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                   &VolumeToSlabProcess::split< int32_t > );
  registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                   &VolumeToSlabProcess::split< uint32_t > );
  registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                   &VolumeToSlabProcess::split< int64_t > );
  registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                   &VolumeToSlabProcess::split< uint64_t > );
  registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                   &VolumeToSlabProcess::split< float > );
  registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                   &VolumeToSlabProcess::split< double > );

}


const std::string& VolumeToSlabProcess::getFileNameOut() const
{

  return _fileNameOut;

}


int32_t VolumeToSlabProcess::getSlabCount() const
{

  return _slabCount;

}


bool VolumeToSlabProcess::getAscii() const
{

  return _ascii;

}


const std::string& VolumeToSlabProcess::getFormat() const
{

  return _format;

}


bool VolumeToSlabProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void VolumeToSlabProcess::split( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    VolumeToSlabProcess& splitProcess = static_cast< VolumeToSlabProcess& >( 
                                                                      process );

    bool verbose = splitProcess.getVerbose();

    // read the input volume
    if ( verbose )
    {

      std::cout << "reading file : " << std::flush;

    }

    gkg::Volume< T > volumeIn;
    std::string format = gkg::Reader::getInstance().read( fileNameIn, 
                                                          volumeIn );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    int32_t sizeX = volumeIn.getSizeX();
    int32_t sizeY = volumeIn.getSizeY();
    int32_t sizeZ = volumeIn.getSizeZ();

    // look for first and last non void slices
    
    int32_t x, y, z, minZ = -1, maxZ = -1;
    bool found = false;
    
    for ( z = 0; !found && ( z < sizeZ ); z++ )
    {
    
      for ( y = 0; !found && ( y < sizeY ); y++ )
      {
      
        for ( x = 0; !found && ( x < sizeX ); x++ )
	{
	
	  if ( volumeIn( x, y, z ) )
	  {
	  
	    minZ = z;
	    found = true;
	  
	  }
	
	}
      
      }
    
    }
    
    z = sizeZ;
    found = false;
    
    while ( !found && z-- )
    {
    
      for ( y = 0; !found && ( y < sizeY ); y++ )
      {
      
        for ( x = 0; !found && ( x < sizeX ); x++ )
	{
	
	  if ( volumeIn( x, y, z ) )
	  {
	  
	    maxZ = z;
	    found = true;
	  
	  }
	
	}
      
      }
    
    }

    if ( verbose )
    {

      std::cout << "Slices range : [ " << minZ 
                << " , " << maxZ << "]" << std::endl;

    }

    int32_t n;
    int32_t slabCount = splitProcess.getSlabCount();
    int32_t slabSize = ( maxZ - minZ + 1 ) / slabCount;
    int32_t firstZ = 0;
    int32_t lastZ = minZ + slabSize; 
    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    gkg::Volume< T > volumeOut( sizeX, sizeY, sizeZ );
    
    volumeIn.getResolution( resolution );
    volumeOut.setResolution( resolution );
    
    for ( n = 0; n < slabCount; n++ )
    {

      if ( verbose )
      {

        std::cout << "Slab from z=" << firstZ
                  << " to z=" << lastZ
                  << std::endl;

      }

      volumeOut.fill( T( 0 ) );

      for ( z = firstZ; z < lastZ; z++ )
      {
      
        for ( y = 0; y < sizeY; y++ )
	{
	
	  for ( x = 0; x < sizeX; x++ )
	  {
	  
	    volumeOut( x, y, z ) = volumeIn( x, y, z );
	  
	  }
	
	}
      
      }

      std::ostringstream oss;
      
      oss << splitProcess.getFileNameOut()
          << "_"
	  << n;

      // writing output slabs
      if ( verbose )
      {

        std::cout << "writing output volume \'"
                  << oss.str() << "\' : "
                  << std::flush;

      }

      gkg::Writer::getInstance().write( 
                                      oss.str(),
                                      volumeOut,
                                      splitProcess.getAscii(),
                                      ( splitProcess.getFormat().empty() ?
                                        format : 
                                        splitProcess.getFormat() ) );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }
      
      firstZ = lastZ;
      lastZ += slabSize;
      
      if ( n == ( slabCount - 1 ) )
      {
      
        lastZ = sizeZ;
      
      }

    }
  
  }
  GKG_CATCH( "template < class T > "
             "void VolumeToSlabProcess::split( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   VolumeToSlabSplitterCommand
//


gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand(
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
  GKG_CATCH( "gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand(
                                		 const std::string& fileNameIn,
                                		 const std::string& fileNameOut,
                                		 int32_t slabCount,
                                		 bool ascii,
                                		 const std::string& format,
                                		 bool verbose )
                                : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameOut, slabCount, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t slabCount, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand(
                                             const gkg::Dictionary& parameters )
                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, slabCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameOut, slabCount, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::VolumeToSlabSplitterCommand::VolumeToSlabSplitterCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::VolumeToSlabSplitterCommand::~VolumeToSlabSplitterCommand()
{
}


std::string gkg::VolumeToSlabSplitterCommand::getStaticName()
{

  try
  {

    return "VolumeToSlabSplitter";

  }
  GKG_CATCH( "std::string gkg::VolumeToSlabSplitterCommand::getStaticName()" );

}


void gkg::VolumeToSlabSplitterCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut;
    int32_t slabCount = 2;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Split a volume into slabs",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output file base name without extension",
                                 fileNameOut );
    application.addSingleOption( "-n", 
                                 "Number of slabs (default=2)",
                                 slabCount, 
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

    execute( fileNameIn, fileNameOut, slabCount, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::VolumeToSlabSplitterCommand::parse()" );

}


void gkg::VolumeToSlabSplitterCommand::execute( const std::string& fileNameIn,
                                		const std::string& fileNameOut,
                                		int32_t slabCount,
                                		bool ascii,
                                		const std::string& format,
                                		bool verbose )
{

  try
  {

    //
    // launching split to slab process
    //
    VolumeToSlabProcess splitProcess( fileNameOut,
                                      slabCount,
                                      ascii,
                                      format,
                                      verbose );
    splitProcess.execute( fileNameIn );

  }
  GKG_CATCH( "void gkg::VolumeToSlabSplitterCommand::execute( "
             "const std::string& fileNameIn, const std::string& fileNameOut, "
             "int32_t slabCount, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator( VolumeToSlabSplitterCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( slabCount ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
