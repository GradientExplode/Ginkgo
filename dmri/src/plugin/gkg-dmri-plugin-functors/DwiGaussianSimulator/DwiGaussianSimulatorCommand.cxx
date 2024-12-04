#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DwiGaussianSimulatorCommand.h>
#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel1.h>
#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel2.h>
#include <gkg-dmri-plugin-functors/DwiGaussianSimulator/DiffusionGaussianModel3.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-coordinates/OrientationSet_i.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-mesh/ConvexHull_i.h>
#include <gkg-processing-mesh/MeshScaler_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-odf/OrientationDistributionFunction.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-exception/Exception.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>


//
// readInformationFile()
//

void readInformationFile( const std::string& name,
                          std::vector< double >& volumeFractions,
                          std::vector< double >& fastRatios,
                          std::vector< gkg::Matrix >& fastDirectTensors,
                          std::vector< gkg::Matrix >& slowDirectTensors,
                          std::vector< gkg::Vector3d< float > >& orientations )
{

  try
  {

    gkg::UncommentCounterInputFileStream is( name.c_str() );

    if ( !is )
    {

      throw std::runtime_error( std::string( "cannot open '" ) +
                                name + "' file" );

    }

    int32_t count = 0;
    is >> count;

    volumeFractions.resize( count );
    fastRatios.resize( count );
    fastDirectTensors.resize( count );
    slowDirectTensors.resize( count );

    int32_t i;
    double angle = 0.0;
    for ( i = 0; i < count; i++ )
    {

      // reading volume fraction of component
      is >> volumeFractions[ i ];

      // reading fast tensor ratio
      is >> fastRatios[ i ];

      // reading rotation angle and building rotation matrix
      is >> angle;

      angle *= M_PI / 180.0;

      gkg::Matrix Rz( 3, 3 );
      Rz( 0, 0 ) = std::cos( angle );
      Rz( 0, 1 ) = -std::sin( angle );
      Rz( 0, 2 ) = 0.0;
      Rz( 1, 0 ) = std::sin( angle );
      Rz( 1, 1 ) = std::cos( angle );
      Rz( 1, 2 ) = 0.0;
      Rz( 2, 0 ) = 0.0;
      Rz( 2, 1 ) = 0.0;
      Rz( 2, 2 ) = 1.0;

      // reading fast tensor component
      fastDirectTensors[ i ].reallocate( 3, 3 );
      is >> fastDirectTensors[ i ]( 0, 0 )
         >> fastDirectTensors[ i ]( 0, 1 )
         >> fastDirectTensors[ i ]( 0, 2 )
         >> fastDirectTensors[ i ]( 1, 1 )
         >> fastDirectTensors[ i ]( 1, 2 )
         >> fastDirectTensors[ i ]( 2, 2 );

      fastDirectTensors[ i ]( 1, 0 ) = fastDirectTensors[ i ]( 0, 1 );
      fastDirectTensors[ i ]( 2, 0 ) = fastDirectTensors[ i ]( 0, 2 );
      fastDirectTensors[ i ]( 2, 1 ) = fastDirectTensors[ i ]( 1, 2 );

      // rotating fast tensor component
      fastDirectTensors[ i ] = Rz.getTransposition().getComposition(
                                  fastDirectTensors[ i ] ).getComposition( Rz );

      // reading slow tensor component
      slowDirectTensors[ i ].reallocate( 3, 3 );
      is >> slowDirectTensors[ i ]( 0, 0 )
         >> slowDirectTensors[ i ]( 0, 1 )
         >> slowDirectTensors[ i ]( 0, 2 )
         >> slowDirectTensors[ i ]( 1, 1 )
         >> slowDirectTensors[ i ]( 1, 2 )
         >> slowDirectTensors[ i ]( 2, 2 );

      slowDirectTensors[ i ]( 1, 0 ) = slowDirectTensors[ i ]( 0, 1 );
      slowDirectTensors[ i ]( 2, 0 ) = slowDirectTensors[ i ]( 0, 2 );
      slowDirectTensors[ i ]( 2, 1 ) = slowDirectTensors[ i ]( 1, 2 );

      // rotating slow tensor component
      slowDirectTensors[ i ] = Rz.getTransposition().getComposition(
                                  slowDirectTensors[ i ] ).getComposition( Rz );

    }

    // reading orientation(s)
    int32_t orientationCount = 0;
    is >> orientationCount;
    orientations.resize( orientationCount );
    int32_t o;
    for ( o = 0; o < orientationCount; o++ )
    {

      is >> orientations[ o ].x
         >> orientations[ o ].y
         >> orientations[ o ].z;

    }

    is.close();

  }
  GKG_CATCH( "void readInformationFile( const std::string& name, "
             "std::vector< double >& volumeFractions, "
             "std::vector< double >& fastRatios, "
             "std::vector< gkg::Matrix >& fastDirectTensors, "
             "std::vector< gkg::Matrix >& slowDirectTensors, "
             "std::vector< gkg::Vector3d< float > >& orientations )" );

}


//
// class DwiGaussianSimulatorCommand
//

gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand(
                                                              int32_t argc,
                                                              char* argv[],
                                                              bool loadPlugin,
                                                              bool removeFirst )
                                 : gkg::Command( argc, argv, loadPlugin,
                                                 removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand(
                                       const std::string& fileNameIn,
                                       const std::string& fileNameSignal,
                                       const std::string& fileNameOdf,
                                       const std::string& fileNameNormalizedOdf,
                                       const std::string& fileNameTexture,
                                       const std::string& fileNameOdfValue,
                                       int32_t modelIndex,
                                       double bValue,
                                       int32_t meshPointCount,
                                       double bigDelta,
                                       double littleDelta,
                                       double radius,
                                       double snr,
                                       const std::string& meshMapFormat,
                                       const std::string& textureMapFormat,
                                       bool ascii,
                                       bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameSignal,
             fileNameOdf,
             fileNameNormalizedOdf,
             fileNameTexture,
             fileNameOdfValue,
             modelIndex,
             bValue,
             meshPointCount,
             bigDelta,
             littleDelta,
             radius,
             snr,
             meshMapFormat,
             textureMapFormat,
             ascii,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameSignal, "
             "const std::string& fileNameOdf, "
             "const std::string& fileNameNormalizedOdf, "
             "const std::string& fileNameTexture, "
             "const std::string& fileNameOdfValue, "
             "int32_t modelIndex, "
             "double bValue, "
             "int32_t meshPointCount, "
             "double bigDelta, "
             "double littleDelta, "
             "double radius, "
             "double snr, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSignal );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdf );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameNormalizedOdf );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfValue );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, modelIndex );
    DECLARE_FLOATING_PARAMETER( parameters, double, bValue );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, meshPointCount );
    DECLARE_FLOATING_PARAMETER( parameters, double, bigDelta );
    DECLARE_FLOATING_PARAMETER( parameters, double, littleDelta );
    DECLARE_FLOATING_PARAMETER( parameters, double, radius );
    DECLARE_FLOATING_PARAMETER( parameters, double, snr );
    DECLARE_STRING_PARAMETER( parameters, std::string, meshMapFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, textureMapFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameSignal,
             fileNameOdf,
             fileNameNormalizedOdf,
             fileNameTexture,
             fileNameOdfValue,
             modelIndex,
             bValue,
             meshPointCount,
             bigDelta,
             littleDelta,
             radius,
             snr,
             meshMapFormat,
             textureMapFormat,
             ascii,
             verbose  );

  }
  GKG_CATCH( "gkg::DwiGaussianSimulatorCommand::DwiGaussianSimulatorCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiGaussianSimulatorCommand::~DwiGaussianSimulatorCommand()
{
}


std::string gkg::DwiGaussianSimulatorCommand::getStaticName()
{

  try
  {

    return "DwiGaussianSimulator";

  }
  GKG_CATCH( "std::string gkg::DwiGaussianSimulatorCommand::getStaticName()" );

}


void gkg::DwiGaussianSimulatorCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameSignal;
    std::string fileNameOdf;
    std::string fileNameNormalizedOdf;
    std::string fileNameTexture;
    std::string fileNameOdfValue;
    int32_t modelIndex = 1;
    double bValue = 0;
    int32_t meshPointCount = 2000;
    double bigDelta = 0.0;
    double littleDelta = 0.0;
    double radius = 0.0;
    double snr = 30;
    std::string meshMapFormat = "aimsmesh";
    std::string textureMapFormat = "aimstexture";
    bool ascii = false;
    bool verbose = false;


    gkg::Application application( _argc, _argv,
                                  "DWI gaussian simulator",
                                  _loadPlugin );

    application.addSingleOption( "-i",
                      "Input information file name.\n"
                      "Diffusion coefficients are given in mm2/s.\n"
                      "Rotation angles are given in degrees.\n"
                      "Must follow the convention:\n"
                      "<component_count>\n"
                      "\n"
                      "<volume_fraction_1>\n"
                      "<fast_ratio_1>\n"
                      "<rotation_angle_1>\n"
                      "Df_0(xx) Df_0(xy) Df_0(xz) Df_0(yy) Df_0(yz) Df_0(zz)\n"
                      "Ds_0(xx) Ds_0(xy) Ds_0(xz) Ds_0(yy) Ds_0(yz) Ds_0(zz)\n"
                      "...\n"
                      "<volume_fraction_N>\n"
                      "<fast_ratio_N>\n"
                      "<rotation_angle_N>\n"
                      "Df_N(xx) Df_N(xy) Df_N(xz) Df_N(yy) Df_N(yz) Df_N(zz)\n"
                      "Ds_N(xx) Ds_N(xy) Ds_N(xz) Ds_N(yy) Ds_N(yz) Ds_N(zz)\n",
                      fileNameIn );
    application.addSingleOption( "-s",
                                 "Output signal mesh file name",
                                 fileNameSignal,
                                 true );
    application.addSingleOption( "-o",
                                 "Output ODF mesh file name",
                                 fileNameOdf,
                                 true );
    application.addSingleOption( "-on",
                                 "Output min-max normalized ODF mesh file name",
                                 fileNameNormalizedOdf,
                                 true );
    application.addSingleOption( "-t",
                                 "Output texture file name",
                                 fileNameTexture,
                                 true );
    application.addSingleOption( "-ov",
                                 "Output orientation ODF value file name",
                                 fileNameOdfValue,
                                 true );
    application.addSingleOption( "-m",
                                 "Model index (default=1)\n"
                                 "Required parameters for each model: \n"
                                 "- 1 -> none\n"
                                 "- 2 -> -bigDelta, -littleDelta, -radius\n"
                                 "- 3 -> -snr\n",
                                 modelIndex,
                                 true );
    application.addSingleOption( "-b",
                                 "B-value in s/mm2",
                                 bValue );
    application.addSingleOption( "-c",
                                 "Mesh point count (default=2000)",
                                 meshPointCount,
                                 true );
    application.addSingleOption( "-snr",
                                 "Signal to Noise Ratio (default=30)",
                                 snr,
                                 true );
    application.addSingleOption( "-bigDelta",
                                 "Big delta in us for "
                                 "b = gamma^2 x G^2 x d^2 x ( D - d/3 ) "
                                 "(default=0.0)",
                                 bigDelta,
                                 true );
    application.addSingleOption( "-littleDelta",
                                 "Big delta in us for "
                                 "b = gamma^2 x G^2 x d^2 x ( D - d/3 ) "
                                 "(default=0.0)",
                                 littleDelta,
                                 true );
    application.addSingleOption( "-radius",
                                 "Minimum radius in um of diffusion "
                                 "(default=0.0)",
                                 radius,
                                 true );
    application.addSingleOption( "-meshMapFormat",
                                 "Ouput MeshMap format (default=aimsmesh)",
                                 meshMapFormat,
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

    execute( fileNameIn,
             fileNameSignal,
             fileNameOdf,
             fileNameNormalizedOdf,
             fileNameTexture,
             fileNameOdfValue,
             modelIndex,
             bValue,
             meshPointCount,
             bigDelta,
             littleDelta,
             radius,
             snr,
             meshMapFormat,
             textureMapFormat,
             ascii,
             verbose  );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiGaussianSimulatorCommand::parse()" );

}


void gkg::DwiGaussianSimulatorCommand::execute(
                                       const std::string& fileNameIn,
                                       const std::string& fileNameSignal,
                                       const std::string& fileNameOdf,
                                       const std::string& fileNameNormalizedOdf,
                                       const std::string& fileNameTexture,
                                       const std::string& fileNameOdfValue,
                                       int32_t modelIndex,
                                       double bValue,
                                       int32_t meshPointCount,
                                       double bigDelta,
                                       double littleDelta,
                                       double radius,
                                       double snr,
                                       const std::string& meshMapFormat,
                                       const std::string& textureMapFormat,
                                       bool ascii,
                                       bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( meshPointCount <= 0 )
    {

      throw std::runtime_error( "mesh point count must be strictly positive" );

    }
    if ( bValue < 0 )
    {

      throw std::runtime_error( "b-value must be positive" );

    }
    if ( ( modelIndex <= 0 ) || ( modelIndex > 3 ) )
    {

      throw std::runtime_error( "bad model index" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading information file
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading information file : " << std::flush;

    }
    std::vector< double > volumeFractions;
    std::vector< double > fastRatios;
    std::vector< gkg::Matrix > fastDirectTensors;
    std::vector< gkg::Matrix > slowDirectTensors;
    std::vector< gkg::Vector3d< float > > orientations;
    readInformationFile( fileNameIn,
                         volumeFractions,
                         fastRatios,
                         fastDirectTensors,
                         slowDirectTensors,
                         orientations );
    int32_t componentCount = ( int32_t )volumeFractions.size();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // allocating diffusion Gaussian model(s)
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::DiffusionGaussianModel* >
      diffusionGaussianModels( componentCount );
    int32_t c;
    for ( c = 0; c < componentCount; c++ )
    {

      if ( modelIndex == 1 )
      {

        diffusionGaussianModels[ c ] = new gkg::DiffusionGaussianModel1(
                                                         volumeFractions[ c ],
                                                         fastRatios[ c ],
                                                         fastDirectTensors[ c ],
                                                         slowDirectTensors[ c ],
                                                         bValue );

      }
      else if ( modelIndex == 2 )
      {

        diffusionGaussianModels[ c ] = new gkg::DiffusionGaussianModel2(
                                                         volumeFractions[ c ],
                                                         fastRatios[ c ],
                                                         fastDirectTensors[ c ],
                                                         slowDirectTensors[ c ],
                                                         bValue,
                                                         bigDelta,
                                                         littleDelta,
                                                         radius );

      }
      else if ( modelIndex == 3 )
      {

        diffusionGaussianModels[ c ] = new gkg::DiffusionGaussianModel3(
                                                         volumeFractions[ c ],
                                                         fastRatios[ c ],
                                                         fastDirectTensors[ c ],
                                                         slowDirectTensors[ c ],
                                                         bValue,
                                                         snr );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building symmetrical vertices
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building spherical vertices : " << std::flush;

    }
    gkg::OrientationSet orientationSet( gkg::ElectrostaticOrientationSet( 
                            meshPointCount / 2 ).getSymmetricalOrientations() );
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
    // building Gaussian signal and ODF scalings
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building signal & ODF scalings : " << std::flush;

    }

    // allocating containers for result(s)
    std::vector< float > signalScaling( meshPointCount );
    std::vector< float > odfScaling( meshPointCount );
    std::vector< float > normalizedOdfScaling( meshPointCount );

    // looping over orientation(s)
    float odfSum = 0.0;
    int32_t o;
    for ( o = 0; o < meshPointCount; o++ )
    {

      // getting unitary orientation
      gkg::Vector u( 3 );
      u( 0 ) = ( double )orientationSet.getOrientation( o ).x;
      u( 1 ) = ( double )orientationSet.getOrientation( o ).y;
      u( 2 ) = ( double )orientationSet.getOrientation( o ).z;

      // initializing signal and ODF value(s) for current orientation u
      signalScaling[ o ] = 0;
      odfScaling[ o ] = 0;

      // looping over components
      for ( c = 0; c < componentCount; c++ )
      {

        signalScaling[ o ] +=
          diffusionGaussianModels[ c ]->getSignalContribution( u );
        odfScaling[ o ] +=
          diffusionGaussianModels[ c ]->getOdfContribution( u );

      }

      odfSum += odfScaling[ o ];

    }

    // normalizing ODF scaling and detecting min-max values
    float minimum = 1e38;
    float maximum = 0.0;
    for ( o = 0; o < meshPointCount; o++ )
    {

      odfScaling[ o ] /= odfSum;
      if ( odfScaling[ o ] < minimum  )
      {

        minimum = odfScaling[ o ];

      }
      if ( odfScaling[ o ] > maximum  )
      {

        maximum = odfScaling[ o ];

      }

    }


    for ( o = 0; o < meshPointCount; o++ )
    {

      normalizedOdfScaling[ o ] = ( odfScaling[ o ] - minimum ) /
                                  ( maximum - minimum );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building diffusion Gaussian signal and ODF meshes
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "building ODF, signal & ODF meshes : " << std::flush;

    }

    // generating ODF
    gkg::OrientationDistributionFunction
      odf( gkg::OrientationDistributionFunction::Standard,
           &orientationSet );
    odf.setValues( odfScaling );

    // generating signal and ODF meshes
    gkg::MeshScaler< int32_t, float, 3U > meshScaler;

    gkg::MeshMap< int32_t, float, 3U > signalMeshMap;
    meshScaler.scale( orientationMeshMap, 0, signalScaling, signalMeshMap );

    gkg::MeshMap< int32_t, float, 3U > odfMeshMap;
    meshScaler.scale( orientationMeshMap, 0, odfScaling, odfMeshMap );

    gkg::MeshMap< int32_t, float, 3U > normalizedOdfMeshMap;
    meshScaler.scale( orientationMeshMap, 0, normalizedOdfScaling,
                      normalizedOdfMeshMap );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // generating texture(s) if needed
    ////////////////////////////////////////////////////////////////////////////

    std::vector< std::string > textureFileNames( 3 );
    gkg::TextureMap< float > rTextureMap;
    gkg::TextureMap< float > gTextureMap;
    gkg::TextureMap< float > bTextureMap;
    if ( !fileNameTexture.empty() )
    {

      if ( verbose )
      {

        std::cout << "generating texture : " << std::flush;

      }

      // processing color associated to each orientation
      std::vector< float > r( meshPointCount );
      std::vector< float > g( meshPointCount );
      std::vector< float > b( meshPointCount );
      for ( o = 0; o < meshPointCount; o++ )
      {

        // getting unitary orientation
        gkg::Vector u( 3 );
        u( 0 ) = ( double )orientationSet.getOrientation( o ).x;
        u( 1 ) = ( double )orientationSet.getOrientation( o ).y;
        u( 2 ) = ( double )orientationSet.getOrientation( o ).z;

        r[ o ] = std::min( std::fabs( orientationSet.getOrientation( o ).x ) *
                           255.0,
                           255.0 );
        g[ o ] = std::min( std::fabs( orientationSet.getOrientation( o ).y ) *
                           255.0,
                           255.0 );
        b[ o ] = std::min( std::fabs( orientationSet.getOrientation( o ).z ) *
                           255.0,
                           255.0 );

      }

      // generating texture maps
      rTextureMap.addTextures( r );
      gTextureMap.addTextures( g );
      bTextureMap.addTextures( b );

      std::vector< std::string > palettes( 3 );
      palettes[ 0 ] = "multitex-geom-red-mask";
      palettes[ 1 ] = "multitex-geom-green-mask";
      palettes[ 2 ] = "multitex-geom-blue-mask";

      rTextureMap.getHeader().addAttribute( "palette", palettes );
      gTextureMap.getHeader().addAttribute( "palette", palettes );
      bTextureMap.getHeader().addAttribute( "palette", palettes );


      // adding texture to mesh
      std::vector< std::string > textureNames( 3 );
      textureNames[ 0 ] = "red";
      textureNames[ 1 ] = "green";
      textureNames[ 2 ] = "blue";

      textureFileNames[ 0 ] = gkg::File( fileNameTexture ).getBaseName() +
                              ".red";
      textureFileNames[ 1 ] = gkg::File( fileNameTexture ).getBaseName() +
                              ".green";
      textureFileNames[ 2 ] = gkg::File( fileNameTexture ).getBaseName() +
                              ".blue";

      signalMeshMap.getHeader().addAttribute( "texture_names",
                                              textureNames );
      signalMeshMap.getHeader().addAttribute( "texture_filenames",
                                              textureFileNames );

      odfMeshMap.getHeader().addAttribute( "texture_names",
                                           textureNames );
      odfMeshMap.getHeader().addAttribute( "texture_filenames",
                                           textureFileNames );

      normalizedOdfMeshMap.getHeader().addAttribute( "texture_names",
                                                     textureNames );
      normalizedOdfMeshMap.getHeader().addAttribute( "texture_filenames",
                                                     textureFileNames );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving result
    ////////////////////////////////////////////////////////////////////////////

    // saving signal
    if ( !fileNameSignal.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing signal : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameSignal, signalMeshMap,
                                        ascii, meshMapFormat );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // saving ODF
    if ( !fileNameOdf.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing ODF : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOdf, odfMeshMap,
                                        ascii, meshMapFormat );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // saving min-max normalized ODF
    if ( !fileNameNormalizedOdf.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing normalized ODF : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameNormalizedOdf,
                                        normalizedOdfMeshMap,
                                        ascii, meshMapFormat );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // saving texture
    if ( !fileNameTexture.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing texture : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameTexture + ".red",
                                        rTextureMap,
                                        ascii, textureMapFormat );
      gkg::Writer::getInstance().write( fileNameTexture + ".green",
                                        gTextureMap,
                                        ascii, textureMapFormat );
      gkg::Writer::getInstance().write( fileNameTexture + ".blue",
                                        bTextureMap,
                                        ascii, textureMapFormat );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    // saving ODF values
    if ( !fileNameOdfValue.empty() && !orientations.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing ODF value : " << std::flush;

      }
      std::ofstream os( fileNameOdfValue.c_str() );

      int32_t orientationValueCount = ( int32_t )orientations.size();
      int32_t v;
      for ( v = 0; v < orientationValueCount; v++ )
      {

        os << orientations[ v ]
           << "        "
           << odf.getOrientationProbability( orientations[ v ] )
           << std::endl;

      }
      os.close();
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // deleting diffusion Gaussian models
    ////////////////////////////////////////////////////////////////////////////

    for ( c = 0; c < componentCount; c++ )
    {

      delete diffusionGaussianModels[ c ];

    }

  }
  GKG_CATCH( "void gkg::DwiGaussianSimulatorCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameSignal, "
             "const std::string& fileNameOdf, "
             "const std::string& fileNameNormalizedOdf, "
             "const std::string& fileNameTexture, "
             "const std::string& fileNameOdfValue, "
             "int32_t modelIndex, "
             "double bValue, "
             "int32_t meshPointCount, "
             "double bigDelta, "
             "double littleDelta, "
             "double radius, "
             "double snr, "
             "const std::string& meshMapFormat, "
             "const std::string& textureMapFormat, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiGaussianSimulatorCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSignal ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdf ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameNormalizedOdf ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameTexture ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfValue ) +
    DECLARE_INTEGER_PARAMETER_HELP( modelIndex ) +
    DECLARE_FLOATING_PARAMETER_HELP( bValue ) +
    DECLARE_INTEGER_PARAMETER_HELP( meshPointCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( bigDelta ) +
    DECLARE_FLOATING_PARAMETER_HELP( littleDelta ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_FLOATING_PARAMETER_HELP( snr ) +
    DECLARE_STRING_PARAMETER_HELP( meshMapFormat ) +
    DECLARE_STRING_PARAMETER_HELP( textureMapFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
