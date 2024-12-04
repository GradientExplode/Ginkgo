#include <gkg-processing-plugin-functors/AffineTransform3d/AffineTransform3dCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-core-exception/Exception.h>

#include <fstream>
#include <iostream>


gkg::AffineTransform3dCommand::AffineTransform3dCommand( int32_t argc,
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
  GKG_CATCH( "gkg::AffineTransform3dCommand::AffineTransform3dCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AffineTransform3dCommand::AffineTransform3dCommand(
                              const std::vector< float >& scalingParameters,
	  		      const std::vector< float >& shearingParameters,
			      const std::vector< float >& rotationParameters,
			      const std::vector< float >& translationParameters,
			      const std::string& transformName )
{

  try
  {

    execute( scalingParameters, shearingParameters, rotationParameters,
             translationParameters, transformName );

  }
  GKG_CATCH( "gkg::AffineTransform3dCommand::AffineTransform3dCommand( "
             "const std::vector< float >& scalingParameters, "
	     "const std::vector< float >& shearingParameters, "
	     "const std::vector< float >& rotationParameters, "
	     "const std::vector< float >& translationParameters, "
	     "const std::string& transformName )" );

}


gkg::AffineTransform3dCommand::AffineTransform3dCommand(
                                             const gkg::Dictionary& parameters )
                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >,
                                           scalingParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >,
                                           shearingParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >,
                                           rotationParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, 
                                           std::vector< float >,
                                           translationParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, transformName );
    
    execute( scalingParameters, shearingParameters, rotationParameters,
             translationParameters, transformName );

  }
  GKG_CATCH( "gkg::AffineTransform3dCommand::AffineTransform3dCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AffineTransform3dCommand::~AffineTransform3dCommand()
{
}


std::string gkg::AffineTransform3dCommand::getStaticName()
{

  try
  {

    return "AffineTransform3d";

  }
  GKG_CATCH( "std::string gkg::AffineTransform3dCommand::getStaticName()" );

}


void gkg::AffineTransform3dCommand::parse()
{

  try
  {

    std::vector< float > scalingParameters;
    std::vector< float > shearingParameters;
    std::vector< float > rotationParameters;
    std::vector< float > translationParameters;
    std::string transformName;

    gkg::Application application( _argc, _argv,
                                  "Affine 3D transform generator",
                                  _loadPlugin );
    application.addSeriesOption( "-s",
                                 "Scaling parameters",
                                 scalingParameters,
                                 3, 3 );
    application.addSeriesOption( "-sh",
                                 "Shearing parameters",
                                 shearingParameters,
                                 3, 3 );
    application.addSeriesOption( "-r",
                                 "Rotation parameters in degrees",
                                 rotationParameters,
                                 3, 3 );
    application.addSeriesOption( "-t",
                                 "Translation parameters in millimeters",
                                 translationParameters,
                                 3, 3 );
    application.addSingleOption( "-o",
                                 "Output homogeneous transform file name",
                                 transformName );

    application.initialize();

    execute( scalingParameters, shearingParameters, rotationParameters,
             translationParameters, transformName );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AffineTransform3dCommand::parse()" );

}


void gkg::AffineTransform3dCommand::execute(
                  	      const std::vector< float >& scalingParameters,
                  	      const std::vector< float >& shearingParameters,
                  	      const std::vector< float >& rotationParameters,
                  	      const std::vector< float >& translationParameters,
                  	      const std::string& transformName )
{

  try
  {

    gkg::AffineTransform3d< float > transform3d( scalingParameters[ 0 ],
      				    		 scalingParameters[ 1 ],
      				    		 scalingParameters[ 2 ],
      				    		 shearingParameters[ 0 ],
      				    		 shearingParameters[ 1 ],
      				    		 shearingParameters[ 2 ],
      				    		 rotationParameters[ 0 ],
      				    		 rotationParameters[ 1 ],
      				    		 rotationParameters[ 2 ],
      				    		 translationParameters[ 0 ],
      				    		 translationParameters[ 1 ],
      				    		 translationParameters[ 2 ],
      				    		 true );

    std::ofstream os( transformName.c_str() );
    transform3d.writeTrm( os );
    os.close();

    std::ifstream is( transformName.c_str() );
    gkg::AffineTransform3d< float > transform3dBis;
    transform3dBis.readTrm( is );

    gkg::Vector3d< float > scalings;
    gkg::Vector3d< float > shearings;
    gkg::Vector3d< float > angles;
    gkg::Vector3d< float > translation;

    transform3dBis.getDirectParameters( scalings, shearings,
                                        angles, translation, true );

    std::cout << "scalings    : " << scalings << std::endl;
    std::cout << "shearings   : " << shearings << std::endl;
    std::cout << "angles      : " << angles << std::endl;
    std::cout << "translation : " << translation << std::endl;

  }
  GKG_CATCH( "void gkg::AffineTransform3dCommand::execute( "
             "const std::vector< float >& scalingParameters, "
             "const std::vector< float >& shearingParameters, "
             "const std::vector< float >& rotationParameters, "
             "const std::vector< float >& translationParameters, "
             "const std::string& transformName )" );

}


RegisterCommandCreator(
           AffineTransform3dCommand,
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalingParameters ) +
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( shearingParameters ) +
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( rotationParameters ) +
           DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( translationParameters ) +
           DECLARE_STRING_PARAMETER_HELP( transformName ) );
