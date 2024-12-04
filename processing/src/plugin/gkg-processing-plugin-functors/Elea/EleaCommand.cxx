#include <gkg-processing-plugin-functors/Elea/EleaCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


//
//   EleaCommand
//


gkg::EleaCommand::EleaCommand( int32_t argc,
                               char* argv[],
                               bool loadPlugin,
                               bool removeFirst )
                 : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::EleaCommand::EleaCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::EleaCommand::EleaCommand( const std::string& fileNameIn,
                               const std::string& fileNameMask,
                               const std::string& fileNameOut,
                               bool ascii,
                               const std::string& format,
                               bool verbose )
                : gkg::Command()
{

  try
  {

    execute( fileNameIn, fileNameMask, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::EleaCommand::EleaCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::EleaCommand::EleaCommand( const gkg::Dictionary& parameters )
                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );       
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn, fileNameMask, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::EleaCommand::EleaCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::EleaCommand::~EleaCommand()
{
}


std::string gkg::EleaCommand::getStaticName()
{

  try
  {

    return "Elea";

  }
  GKG_CATCH( "std::string gkg::EleaCommand::getStaticName()" );

}


void gkg::EleaCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameMask;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Tool for onlyt Elea",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input file name",
                                 fileNameIn );
    application.addSingleOption( "-m",
                                 "Mask file name",
                                 fileNameMask );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut );
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

    execute( fileNameIn, fileNameMask, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::EleaCommand::parse()" );

}


void gkg::EleaCommand::execute( const std::string& fileNameIn,
                          	const std::string& fileNameMask,
                          	const std::string& fileNameOut,
                           	bool ascii,
                           	const std::string& format,
                           	bool verbose )
{

  try
  {

    std::cout << "salut les amis" << std::endl;

    gkg::Volume< float > volume;
    
    std::cout << "fileNameIn=" << fileNameIn << std::endl;
    gkg::Reader::getInstance().read( fileNameIn,
                                     volume );

    std::cout << "volume read" << std::endl;

    gkg::Volume< int16_t > mask;
    gkg::Reader::getInstance().read( fileNameMask,
                                     mask );

    std::cout << "volume read" << std::endl;
    
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
    int32_t t = 0;

    gkg::Vector3d< int32_t > size;
    int32_t sizeT = 1;
    volume.getSize( size, sizeT );

    gkg::Vector3d< double > resolution;
    double resolutionT = 1.0;    
    volume.getResolution( resolution, resolutionT );
    
    std::vector< float > means( sizeT, 0.0f );
    int32_t voxelCount = 0;
    for ( t = 0; t < sizeT; t++ )
    {
    
      for ( z = 0; z < size.z; z++ )
      {
    
        for ( y = 0; y < size.y; y++ )
        {
      
          for ( x = 0; x < size.x; x++ )
          {
        
            if ( mask( x, y, z ) )
            {
            
              means[ t ] += volume( x, y, z, t );
              if ( t == 0 )
              {
              
                ++ voxelCount;
              
              }
              
            }
            
          }
        
        }
      
      }
      means[ t ] /= ( float )voxelCount;
    
    }
    
    std::cout << "means: ";
    for ( t = 0; t < sizeT; t++ )
    {
 
     std::cout << means[ t ]  << " ";
     
    }
    std::cout << std::endl;
    
    for ( t = 0; t < sizeT; t++ )
    {
    
      for ( z = 0; z < size.z; z++ )
      {
    
        for ( y = 0; y < size.y; y++ )
        {
      
          for ( x = 0; x < size.x; x++ )
          {
        
            if ( mask( x, y, z ) )
            {
            
              volume( x, y, z, t ) -= ( means[ t ] - means[ 0 ] );
              
            }
            
          }
        
        }
      
      }
    
    }

    std::cout << "mean correction done" << std::endl;
    
    gkg::Writer::getInstance().write( fileNameOut,
                                      volume, ascii, format );

    std::cout << "done" << std::endl;


  }
  GKG_CATCH( "void gkg::EleaCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "const std::string& axis, "
             "const std::vector< int32_t >& range, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator( EleaCommand,
    			DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +    			
    			DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_STRING_PARAMETER_HELP( format ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
