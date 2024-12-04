#include <gkg-dmri-plugin-functors/DwiChangeRGBBackgroundColor/DwiChangeRGBBackgroundColorCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-processing-colorimetry/RGBComponent.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::DwiChangeRGBBackgroundColorCommand::DwiChangeRGBBackgroundColorCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                        : gkg::Command( argc, argv,
                                                        loadPlugin,
                                                        removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiChangeRGBBackgroundColorCommand::DwiChangeRGBBackgroundColorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiChangeRGBBackgroundColorCommand::DwiChangeRGBBackgroundColorCommand( const std::string& fileNameRGB,
                                        const std::string& fileNameMaskOfSignal,
                                        const std::string& fileNameOutput,
                                        const std::vector< int32_t >& rgb,
                                        bool verbose  )
                                        : gkg::Command()
{

  try
  {

    execute( fileNameRGB,
             fileNameMaskOfSignal,
             fileNameOutput,
             rgb,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiChangeRGBBackgroundColorCommand::DwiChangeRGBBackgroundColorCommand( "
             "const std::string& fileNameRGB, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameOutput, "
             "const std::vector< int32_t >& rgb, "
             "bool verbose  )" );

}


gkg::DwiChangeRGBBackgroundColorCommand::DwiChangeRGBBackgroundColorCommand(
                                             const gkg::Dictionary& parameters )
                                        : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRGB );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMaskOfSignal );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutput );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                          rgb );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameRGB,
             fileNameMaskOfSignal,
             fileNameOutput,
             rgb,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiChangeRGBBackgroundColorCommand::"
             "DwiChangeRGBBackgroundColorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiChangeRGBBackgroundColorCommand::~DwiChangeRGBBackgroundColorCommand()
{
}


std::string gkg::DwiChangeRGBBackgroundColorCommand::getStaticName()
{

  try
  {

    return "DwiChangeRGBBackgroundColor";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiChangeRGBBackgroundColorCommand::getStaticName()" );

}


void gkg::DwiChangeRGBBackgroundColorCommand::parse()
{

  try
  {

    std::string fileNameRGB;
    std::string fileNameMaskOfSignal;
    std::string fileNameOutput;
    std::vector< int32_t > rgb;
    bool verbose = false;


    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, 
                                  "DWI RGB map background color change",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input RGB map volume file name",
                                 fileNameRGB );
    application.addSingleOption( "-ms",
                                 "Input signal mask file name ",
                                 fileNameMaskOfSignal );
    application.addSingleOption( "-o",
                                 "Output volume file name",
                                 fileNameOutput );
    application.addSeriesOption( "-rgb",
                                 "R,G, and B components of color",
				 rgb,
				 3,
				 3 );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );

    application.initialize();

    execute( fileNameRGB,
             fileNameMaskOfSignal,
             fileNameOutput,
             rgb,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiChangeRGBBackgroundColorCommand::parse()" );

}


void gkg::DwiChangeRGBBackgroundColorCommand::execute(
                                        const std::string& fileNameRGB,
                                        const std::string& fileNameMaskOfSignal,
                                        const std::string& fileNameOutput,
                                        const std::vector< int32_t >& rgb,
                                        bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading RGB data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameRGB << "' : " << std::flush;

    }

    gkg::Volume< gkg::RGBComponent > rgbMap;
    gkg::Reader::getInstance().read( fileNameRGB, rgbMap );
    int32_t sizeX = rgbMap.getSizeX();
    int32_t sizeY = rgbMap.getSizeY();
    int32_t sizeZ = rgbMap.getSizeZ();

    gkg::Vector3d< double > resolution( 1.0, 1.0, 1.0 );
    rgbMap.getResolution( resolution );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mask of signal
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading '" << fileNameMaskOfSignal << "' : " << std::flush;

    }

    gkg::Volume< int16_t > maskOfSignal;
    gkg::Reader::getInstance().read( fileNameMaskOfSignal, maskOfSignal );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // changing background color
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "changing background color : " << std::flush;

    }

    gkg::Volume< gkg::RGBComponent > outputRGBMap( sizeX, sizeY, sizeZ );
    outputRGBMap.setResolution( resolution );
    
    gkg::RGBComponent backgroundColor( rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < sizeZ; site.z++ )
    {

      for ( site.y = 0; site.y < sizeY; site.y++ )
      {

        for ( site.x = 0; site.x < sizeX; site.x++ )
        {

          if ( maskOfSignal( site ) )
          {
	  
	    outputRGBMap( site ) = rgbMap( site );
	  
	  }
	  else
	  {
	  
	    outputRGBMap( site ) = backgroundColor;
	  
	  }
	  
	}
	
      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // writing the output RGB map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing output RGB volume : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameOutput, outputRGBMap );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiChangeRGBBackgroundColorCommand::execute( "
             "const std::string& fileNameRGB, "
             "const std::string& fileNameMaskOfSignal, "
             "const std::string& fileNameOutput, "
             "const std::vector< int32_t >& rgb, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiChangeRGBBackgroundColorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameRGB ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskOfSignal ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOutput ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( rgb ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
