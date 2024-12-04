#include <gkg-processing-plugin-functors/SphericalHarmonicMesher/SphericalHarmonicMesherCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>


gkg::SphericalHarmonicMesherCommand::SphericalHarmonicMesherCommand(
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
  GKG_CATCH( "gkg::SphericalHarmonicMesherCommand::SphericalHarmonicMesherCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SphericalHarmonicMesherCommand::SphericalHarmonicMesherCommand(
                                    	    int32_t j,
                                    	    int32_t l,
                                    	    int32_t m,
                                    	    const std::string& fileNameOut,
                                    	    int32_t meshPointCount,
                                    	    const std::string& meshMapFormat,
                                    	    const std::string& textureMapFormat,
                                    	    bool ascii,
                                    	    bool verbose,
                                    	    bool minMax )
                                   : gkg::Command()
{

  try
  {

    execute( j, l, m, fileNameOut, meshPointCount, meshMapFormat, 
             textureMapFormat, ascii, verbose, minMax );

  }
  GKG_CATCH( "gkg::SphericalHarmonicMesherCommand::SphericalHarmonicMesherCommand( "
             "int32_t j, int32_t l, int32_t m, const std::string& fileNameOut, "
             "int32_t meshPointCount, const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, bool ascii, bool verbose, "
             "bool minMax )" );

}


gkg::SphericalHarmonicMesherCommand::SphericalHarmonicMesherCommand(
                                             const gkg::Dictionary& parameters )
                                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_INTEGER_PARAMETER( parameters, int32_t, j );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, l );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, m );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, meshPointCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, minMax );
    
    execute( j, l, m, fileNameOut, meshPointCount, meshMapFormat, 
             textureMapFormat, ascii, verbose, minMax );

  }
  GKG_CATCH( "gkg::SphericalHarmonicMesherCommand::"
             "SphericalHarmonicMesherCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::SphericalHarmonicMesherCommand::~SphericalHarmonicMesherCommand()
{
}


std::string gkg::SphericalHarmonicMesherCommand::getStaticName()
{

  try
  {

    return "SphericalHarmonicMesher";

  }
  GKG_CATCH( "std::string gkg::SphericalHarmonicMesherCommand::getStaticName()" );

}


void gkg::SphericalHarmonicMesherCommand::parse()
{

  try
  {

    int32_t j = -1000000;
    int32_t l = -1000000;
    int32_t m = -1000000;
    std::string fileNameOut;
    int32_t meshPointCount = 500;
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    bool ascii = false;
    bool verbose = false;
    bool minMax = true;

    gkg::Application application( _argc, _argv,
                                  "Mesher of spherical harmonics)",
                                  _loadPlugin );
    application.addSingleOption( "-j",
                                 "Real symmetrical spherical harmonic basis "
                                 "order",
                                 j,
                                 true );
    application.addSingleOption( "-l",
                                 "Standard spherical harmonic basis order",
                                 l,
                                 true );
    application.addSingleOption( "-m",
                                 "Standard spherical harmonic basis phase "
                                 "factor",
                                 m,
                                 true );
    application.addSingleOption( "-o",
                                 "Output mesh file name",
                                 fileNameOut );
    application.addSingleOption( "-c",
                                 "Mesh point count (default=500)",
                                 meshPointCount,
                                 true );
   application.addSingleOption( "-meshMapFormat",
                                 "Ouput MeshMap format (default=aimsmesh)",
                                 meshMapFormat,
                                 true );
    application.addSingleOption( "-minmax",
                                 "Min-max normalize mesh (default=true)",
                                 minMax,
                                 true );
    application.addSingleOption( "-textureMapFormat",
                                 "Ouput MeshMap format (default=aimstexture)",
                                 textureMapFormat,
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

    execute( j, l, m, fileNameOut, meshPointCount, meshMapFormat,
             textureMapFormat, ascii, verbose, minMax );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SphericalHarmonicMesherCommand::parse()" );

}


void gkg::SphericalHarmonicMesherCommand::execute(
                  	            	    int32_t j,
                                    	    int32_t l,
                                    	    int32_t m,
                                    	    const std::string& fileNameOut,
                                    	    int32_t meshPointCount,
                                    	    const std::string& meshMapFormat,
                                    	    const std::string& textureMapFormat,
                                    	    bool ascii,
                                    	    bool verbose,
                                    	    bool minMax )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( ( j != -1000000 ) && ( ( l != -1000000 ) || ( m != -1000000 ) ) )
    {

      throw std::runtime_error( "use either j or (l,m) parameters" );

    }
    if ( ( ( l != -1000000 ) && ( m == -1000000 ) ) ||
         ( ( l == -1000000 ) && ( m != -1000000 ) ) )
    {

      throw std::runtime_error( "both l and m argument must be filled" );

    }
    if ( l % 2 )
    {

      throw std::runtime_error( "l must be even" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // building vertices
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building spherical vertices : " << std::flush;

    }
    gkg::ElectrostaticOrientationSet orientationSet( meshPointCount, false );
    meshPointCount = orientationSet.getCount();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building convex hull
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building convex hull : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3U > orientationMeshMap;
    gkg::ConvexHull::getInstance().addConvexHull( 
                                               orientationSet.getOrientations(),
                                               0,
                                               orientationMeshMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building spherical harmonic
    ////////////////////////////////////////////////////////////////////////////

    // getting a pointer to the numerical analysis factory
    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

    if ( verbose )
    {

      if ( j != -1000000 )
      {

        std::cout << "processing symmetrical j= " << j
                  << " spherical harmonic : " << std::flush;

      }
      else
      {

        std::cout << "processing ( l=" << l << ", m=" << m
                  << " ) spherical harmonic : " << std::flush;

      }

    }

    std::vector< float > scaling( meshPointCount );
    std::vector< float > red( meshPointCount );
    std::vector< float > green( meshPointCount );
    std::vector< float > blue( meshPointCount );
    int32_t o;
    double radial = 0;
    double theta = 0;
    double phi = 0;
    double minimum = 1e38;
    double maximum = -1e38;

    for ( o = 0; o < meshPointCount; o++ )
    {

      // cartesian to spherical coordinates
      factory->getCartesianToSphericalCoordinates( 
                                 ( double )orientationSet.getOrientation( o ).x,
                                 ( double )orientationSet.getOrientation( o ).y,
                                 ( double )orientationSet.getOrientation( o ).z,
                                 radial,
                                 theta,
                                 phi );

      // processing color associated to orientation
      red[ o ] = std::fabs( orientationSet.getOrientation( o ).x ) *
                 255.0;
      green[ o ] = std::fabs( orientationSet.getOrientation( o ).y ) *
                   255.0;
      blue[ o ] = std::fabs( orientationSet.getOrientation( o ).z ) *
                  255.0;
      if ( red[ o ] > 255.0 )
      {

        red[ o ] = 255.0;

      }
      if ( green[ o ] > 255.0 )
      {

        green[ o ] = 255.0;

      }
      if ( blue[ o ] > 255.0 )
      {

        blue[ o ] = 255.0;

      }


      // careful: in physics, theta is azimuth  and phi is colatitule,
      // whereas for spherical harmonics, theta is colatitude and phi is
      // azimuth, so theta and phi are swapped in the following SH function 
      // call(s)
      if ( j != -1000000 )
      {

        scaling[ o ] = factory->getSymmetricalSphericalHarmonic( j,
                                                                 phi,
                                                                 theta ) ;

      }
      else
      {

        scaling[ o ] = std::abs(factory->getSphericalHarmonic( l,
							       m,
							       phi,
							       theta ) );

      }
	  
      if ( scaling[ o ] < minimum )
      {

        minimum = scaling[ o ];

      }

      if ( scaling[ o ] > maximum )
      {

        maximum = scaling[ o ];

      }
	  
    }
	
    // MinMax normalization of the meshPoints
    if ( minMax )
    { 

      //std::cout << "Min-max normalization...\t"<<min<<","<<max<<"\n";

      for ( o = 0; o < meshPointCount; o++ )
      {

        scaling[ o ] = ( scaling[ o ] - minimum ) / ( maximum - minimum );

      }

    }
	
    // applying spherical harmonic coefficient to spherical mesh
    gkg::MeshScaler< int32_t, float, 3U > meshScaler;
    gkg::MeshMap< int32_t, float, 3U > sphericalHarmonicMeshMap;
    meshScaler.scale( orientationMeshMap, 0, scaling,
                      sphericalHarmonicMeshMap );

    // adding red/green/blue texture(s)
    gkg::TextureMap< float > redTextureMap;
    gkg::TextureMap< float > greenTextureMap;
    gkg::TextureMap< float > blueTextureMap;
    std::vector< std::string > palettes( 3 );
    palettes[ 0 ] = "multitex-geom-red-mask";
    palettes[ 1 ] = "multitex-geom-green-mask";
    palettes[ 2 ] = "multitex-geom-blue-mask";
    redTextureMap.getHeader().addAttribute( "palette", palettes );

    redTextureMap.addTextures( red );
    greenTextureMap.addTextures( green );
    blueTextureMap.addTextures( blue );

    // adding texture to mesh
    std::vector< std::string > textureNames( 3 );
    textureNames[ 0 ] = "red";
    textureNames[ 1 ] = "green";
    textureNames[ 2 ] = "blue";
    std::vector< std::string > textureFileNames( 3 );
    textureFileNames[ 0 ] = fileNameOut + ".red";
    textureFileNames[ 1 ] = fileNameOut + ".green";
    textureFileNames[ 2 ] = fileNameOut + ".blue";
    sphericalHarmonicMeshMap.getHeader().addAttribute( "texture_names",
                                                       textureNames );
    sphericalHarmonicMeshMap.getHeader().addAttribute( "texture_filenames",
                                                       textureFileNames );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building convex hull
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, sphericalHarmonicMeshMap,
                                      ascii, meshMapFormat );
    gkg::Writer::getInstance().write( textureFileNames[ 0 ],
                                      redTextureMap,
                                      ascii, textureMapFormat );
    gkg::Writer::getInstance().write( textureFileNames[ 1 ],
                                      greenTextureMap,
                                      ascii, textureMapFormat );
    gkg::Writer::getInstance().write( textureFileNames[ 2 ],
                                      blueTextureMap,
                                      ascii, textureMapFormat );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::SphericalHarmonicMesherCommand::execute( "
             "int32_t j, int32_t l, int32_t m, const std::string& fileNameOut, "
             "int32_t meshPointCount, const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, bool ascii, bool verbose, "
             "bool minMax )" );

}


RegisterCommandCreator( SphericalHarmonicMesherCommand,
    			DECLARE_INTEGER_PARAMETER_HELP( j ) +
    			DECLARE_INTEGER_PARAMETER_HELP( l ) +
    			DECLARE_INTEGER_PARAMETER_HELP( m ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_INTEGER_PARAMETER_HELP( meshPointCount ) +
    			DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    			DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( minMax ) );
