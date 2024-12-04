#include <gkg-mri-reconstruction-plugin-functors/Autoshim/AutoshimCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <cmath>


static void masking( gkg::Volume< float >& volume,
                     const gkg::Volume< int16_t >& mask )
{

  int32_t x, y, plane;
  for ( plane = 0; plane < mask.getSizeT(); plane ++ )
  {

    for ( y = 0; y < mask.getSizeY(); y++ )
    {

      for ( x = 0; x < mask.getSizeX(); x++ )
      {

        if ( mask( x, y, 0, plane ) == 0 )
        {

          volume( x, y, 0, plane ) = 0.0;

        }

      }

    }

  }


}


gkg::AutoshimCommand::AutoshimCommand( int32_t argc,
                                       char* argv[],
                                       bool loadPlugin,
                                       bool removeFirst )
                     : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::AutoshimCommand::AutoshimCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AutoshimCommand::AutoshimCommand( const std::string& fileNameDeltaPhase,
                                       const std::string& fileNameMagnitude,
                                       float radius,
                                       int32_t entry,
                                       int32_t position,
                                       int16_t xfull,
                                       int16_t yfull,
                                       int16_t zfull,
                                       float xfs,
                                       float yfs,
                                       float zfs,
                                       int16_t oldShimX,
                                       int16_t oldShimY,
                                       int16_t oldShimZ )
                     : gkg::Command()
{

  try
  {

    execute( fileNameDeltaPhase,
             fileNameMagnitude,
             radius,
             entry,
             position,
             xfull,
             yfull,
             zfull,
             xfs,
             yfs,
             zfs,
             oldShimX,
             oldShimY,
             oldShimZ );

  }
  GKG_CATCH( "gkg::AutoshimCommand::AutoshimCommand( "
             "const std::string& fileNameDeltaPhase, "
             "const std::string& fileNameMagnitude, "
             "float radius, "
             "int32_t entry, "
             "int32_t position, "
             "int16_t xfull, "
             "int16_t yfull, "
             "int16_t zfull, "
             "float xfs, "
             "float yfs, "
             "float zfs, "
             "int16_t oldShimX, "
             "int16_t oldShimY, "
             "int16_t oldShimZ )" );

}


gkg::AutoshimCommand::AutoshimCommand( const gkg::Dictionary& parameters )
                     : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDeltaPhase );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMagnitude );
    DECLARE_FLOATING_PARAMETER( parameters, float, radius );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, entry );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, position );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, xfull );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, yfull );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, zfull );
    DECLARE_FLOATING_PARAMETER( parameters, float, xfs );
    DECLARE_FLOATING_PARAMETER( parameters, float, yfs );
    DECLARE_FLOATING_PARAMETER( parameters, float, zfs );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, oldShimX );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, oldShimY );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, oldShimZ );

    execute( fileNameDeltaPhase,
             fileNameMagnitude,
             radius,
             entry,
             position,
             xfull,
             yfull,
             zfull,
             xfs,
             yfs,
             zfs,
             oldShimX,
             oldShimY,
             oldShimZ );

  }
  GKG_CATCH( "gkg::AutoshimCommand::"
             "AutoshimCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AutoshimCommand::~AutoshimCommand()
{
}


std::string gkg::AutoshimCommand::getStaticName()
{

  try
  {

    return "Autoshim";

  }
  GKG_CATCH( "std::string gkg::AutoshimCommand::getStaticName()" );

}


void gkg::AutoshimCommand::parse()
{

  try
  {

    std::string fileNameDeltaPhase;
    std::string fileNameMagnitude;
    float radius = 64.0;
    int32_t entry = 1;
    int32_t position = 1;
    int16_t xfull = 29607;
    int16_t yfull = 29662;
    int16_t zfull = 29249;
    float xfs = 4.0;
    float yfs = 4.0;
    float zfs = 4.0;
    int16_t oldShimX = 0;
    int16_t oldShimY = 0;
    int16_t oldShimZ = 0;

    gkg::Application application( _argc, _argv,
                                  "First order auto-shimming calculation",
                                  _loadPlugin );
    application.addSingleOption( "-d",
                                 "Input delta phase map",
                                 fileNameDeltaPhase );
    application.addSingleOption( "-m",
                                 "Input magnitude map",
                                 fileNameMagnitude );
    application.addSingleOption( "-r",
                                 "radius (default=64.0)",
                                 radius, true );
    application.addSingleOption( "-entry",
                                 "1->Head first 2->Feet first (default=1)",
                                 entry, true );
    application.addSingleOption( "-position",
                                 "1->Supine 2->Prone "
                                 "3->Left decub 4->Right decub (default=1)",
                                 position, true );
    application.addSingleOption( "-xfull",
                                 "Short full X gradient scale (default=29607)",
                                 xfull, true );
    application.addSingleOption( "-yfull",
                                 "Short full Y gradient scale (default=29662)",
                                 yfull, true );
    application.addSingleOption( "-zfull",
                                 "Short full gradient scale (default=29249)",
                                 xfull, true );
    application.addSingleOption( "-xfs",
                                 "Maximum X gradient amplitude in G/cm "
                                 "(default=4.0)",
                                 xfs, true );
    application.addSingleOption( "-yfs",
                                 "Maximum Y gradient amplitude in G/cm "
                                 "(default=4.0)",
                                 yfs, true );
    application.addSingleOption( "-zfs",
                                 "Maximum Z gradient amplitude in G/cm "
                                 "(default=4.0)",
                                 zfs, true );
    application.addSingleOption( "-sx",
                                 "Old X shim (default=0)",
                                 oldShimX, true );
    application.addSingleOption( "-sy",
                                 "Old Y shim (default=0)",
                                 oldShimY, true );
    application.addSingleOption( "-sz",
                                 "Old Z shim (default=0)",
                                 oldShimZ, true );

    application.initialize();

    execute( fileNameDeltaPhase,
             fileNameMagnitude,
             radius,
             entry,
             position,
             xfull,
             yfull,
             zfull,
             xfs,
             yfs,
             zfs,
             oldShimX,
             oldShimY,
             oldShimZ );

  }
  GKG_CATCH_FUNCTOR( "void gkg::AutoshimCommand::parse()" );

}


void gkg::AutoshimCommand::execute( const std::string& fileNameDeltaPhase,
                                    const std::string& fileNameMagnitude,
                                    float radius,
                                    int32_t entry,
                                    int32_t position,
                                    int16_t xfull,
                                    int16_t yfull,
                                    int16_t zfull,
                                    float xfs,
                                    float yfs,
                                    float zfs,
                                    int16_t oldShimX,
                                    int16_t oldShimY,
                                    int16_t oldShimZ )
{

  try
  {

    // definitions
    float GAMMA = 4257.0;
    float field_of_view = 320;
    int32_t xres = 256;
    int32_t yres = 256;
    float pixelSize = field_of_view / ( float )xres;
    float scale1 = pixelSize * 0.1;
    float scale2 = scale1 * scale1;
    scale1 = 1 / ( scale1 * GAMMA );
    scale2 = 1 / ( scale2 * GAMMA ) * 1000.0;

    // reading delta phase map
    gkg::Volume< float > deltaPhase;
    gkg::Reader::getInstance().read( fileNameDeltaPhase, deltaPhase );

    // reading magnitude map
    gkg::Volume< float > magnitude;
    gkg::Reader::getInstance().read( fileNameMagnitude, magnitude );

    // processing mask
    gkg::Volume< int16_t > mask( deltaPhase.getSizeX(),
                                 deltaPhase.getSizeY(),
                                 1,
                                 deltaPhase.getSizeT() );
    mask.fill( 0 );
    int32_t x, y, plane;
    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
       {
 
          if ( deltaPhase( x, y, 0, plane ) != 0.0 )
          {

            mask( x, y, 0, plane ) = 1;

          }

        }

      }

    }

    // processing x derivative
    gkg::Volume< float > xDerivative( deltaPhase.getSizeX(),
                                      deltaPhase.getSizeY(),
                                      1,
                                      deltaPhase.getSizeT() );
    xDerivative.fill( 0 );
    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres - 1; x++ )
        {

          if ( mask( x, y, 0, plane ) && mask( x + 1, y, 0, plane ) )
          {

            xDerivative( x, y, 0, plane ) = deltaPhase( x + 1, y, 0, plane ) -
                                            deltaPhase( x, y, 0, plane );

          }

        }

      }

    }

    // processing y derivative
    gkg::Volume< float > yDerivative( deltaPhase.getSizeX(),
                                      deltaPhase.getSizeY(),
                                      1,
                                      deltaPhase.getSizeT() );
    yDerivative.fill( 0 );
    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres - 1; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          if ( mask( x, y, 0, plane ) && mask( x, y + 1, 0, plane ) )
          {

            yDerivative( x, y, 0, plane ) = deltaPhase( x, y + 1, 0, plane ) -
                                            deltaPhase( x, y, 0, plane );

          }

        }

      }

    }

    // updating mask
    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          if ( ( x == xres - 1 ) ||
               ( ( mask( x, y, 0, plane ) == 1 ) &&
                 ( mask( x + 1, y, 0, plane ) == 0 ) ) )
          {

            mask( x, y, 0, plane ) = 0;

          }

        }

      }

    }

    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          if ( ( y == yres - 1 ) ||
               ( ( mask( x, y, 0, plane ) == 1 ) &&
                 ( mask( x, y + 1, 0, plane ) == 0 ) ) )
          {

            mask( x, y, 0, plane ) = 0;

          }

        }

      }

    }


    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          if ( ( ( float )( x - 128 ) * ( x - 128 ) ) +
               ( ( float )( y - 128 ) * ( y - 128 ) ) > radius * radius )
          {

            mask( x, y, 0, plane ) = 0;

          }

        }

      }

    }


    // create x ramp image
    gkg::Volume< float > xRamp( deltaPhase.getSizeX(),
                                deltaPhase.getSizeY(),
                                1,
                                deltaPhase.getSizeT() );
    gkg::Volume< float > yRamp( deltaPhase.getSizeX(),
                                deltaPhase.getSizeY(),
                                1,
                                deltaPhase.getSizeT() );
    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres - 1; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          xRamp( x, y, 0, plane ) = - 256.0 / 2.0 + ( float )x;
          yRamp( x, y, 0, plane ) = - 256.0 / 2.0 + ( float )y;

        }

      }

    }

    // saving result
    masking( deltaPhase, mask );
    masking( magnitude, mask );
    masking( xDerivative, mask );
    masking( yDerivative, mask );
    masking( xRamp, mask );
    masking( yRamp, mask );


    // processing maximum x derivative
    for ( plane = 0; plane < 3; plane ++ )
    {

      float maxXDerivative = 0.0;
      int32_t xMaxXDerivative = -1, yMaxXDerivative = -1;
      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

            if ( std::fabs( xDerivative( x, y, 0, plane ) ) > maxXDerivative )
            {

              maxXDerivative = std::fabs( xDerivative( x, y, 0, plane ) );
              xMaxXDerivative = x;
              yMaxXDerivative = y;

            }

        }

      }
      std::cout << "maximum x derivative for plane " << plane + 1 << ": "
                << maxXDerivative
                << " at ( " << xMaxXDerivative << ", " 
                << yMaxXDerivative << " )" << std::endl;

    }

    // processing maximum y derivative
    for ( plane = 0; plane < 3; plane ++ )
    {

      float maxYDerivative = 0.0;
      int32_t xMaxYDerivative = -1, yMaxYDerivative = -1;
      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

            if ( std::fabs( yDerivative( x, y, 0, plane ) ) > maxYDerivative )
            {

              maxYDerivative = std::fabs( yDerivative( x, y, 0, plane ) );
              xMaxYDerivative = x;
              yMaxYDerivative = y;

            }

        }

      }
      std::cout << "maximum y derivative for plane " << plane + 1 << ": "
                << maxYDerivative
                << " at ( " << xMaxYDerivative << ", " 
                << yMaxYDerivative << " )" << std::endl;

    }

    gkg::Writer::getInstance().write( "mask", mask );
    gkg::Writer::getInstance().write( "magnitude", magnitude );
    gkg::Writer::getInstance().write( "xDerivative", xDerivative );
    gkg::Writer::getInstance().write( "yDerivative", yDerivative );
    gkg::Writer::getInstance().write( "xRamp", xRamp );
    gkg::Writer::getInstance().write( "yRamp", yRamp );

    // number of points used, sum of magnitude, sum of x derivatives,
    // sum of y derivatives
    int32_t as_Sn[ 3 ] = { 0, 0, 0 };
    float as_Sampl[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sdx[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sdy[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sx[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sy[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sxdx[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sxampl[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sxx[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Sydy[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Syampl[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_Syy[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_S1pd[ 3 ] = { 0.0, 0.0, 0.0 };
    float as_S2pd[ 3 ] = { 0.0, 0.0, 0.0 };
    float avg[ 3 ] = { 0.0, 0.0, 0.0 };
    float var[ 3 ] = { 0.0, 0.0, 0.0 };
    float sigma[ 3 ] = { 0.0, 0.0, 0.0 };
    float determX[ 3 ] = { 0.0, 0.0, 0.0 };
    float shimGx2[ 3 ] = { 0.0, 0.0, 0.0 };
    float shimGx[ 3 ] = { 0.0, 0.0, 0.0 };
    float determY[ 3 ] = { 0.0, 0.0, 0.0 };
    float shimGy2[ 3 ] = { 0.0, 0.0, 0.0 };
    float shimGy[ 3 ] = { 0.0, 0.0, 0.0 };
    float scaledShimGx2[ 3 ] = { 0.0, 0.0, 0.0 };
    float scaledShimGx[ 3 ] = { 0.0, 0.0, 0.0 };
    float scaledShimGy2[ 3 ] = { 0.0, 0.0, 0.0 };
    float scaledShimGy[ 3 ] = { 0.0, 0.0, 0.0 };


    for ( plane = 0; plane < 3; plane ++ )
    {

      for ( y = 0; y < yres; y++ )
      {

        for ( x = 0; x < xres; x++ )
        {

          as_Sn[ plane ] += mask( x, y, 0, plane );
          as_Sampl[ plane ] += magnitude( x, y, 0, plane );
          as_Sdx[ plane ] += xDerivative( x, y, 0, plane );
          as_Sdy[ plane ] += yDerivative( x, y, 0, plane );

          as_Sx[ plane ] += xRamp( x, y, 0, plane );
          as_Sxdx[ plane ] += xRamp( x, y, 0, plane ) *
                              xDerivative( x, y, 0, plane );
          as_Sxampl[ plane ] += xRamp( x, y, 0, plane ) *
                                magnitude( x, y, 0, plane );
          as_Sxx[ plane ] += xRamp( x, y, 0, plane ) * xRamp( x, y, 0, plane );

          as_Sy[ plane ] += yRamp( x, y, 0, plane );
          as_Sydy[ plane ] += yRamp( x, y, 0, plane ) *
                              yDerivative( x, y, 0, plane );
          as_Syampl[ plane ] += yRamp( x, y, 0, plane ) *
                                magnitude( x, y, 0, plane );
          as_Syy[ plane ] += yRamp( x, y, 0, plane ) * yRamp( x, y, 0, plane );
          as_S1pd[ plane ] += deltaPhase( x, y, 0, plane );
          as_S2pd[ plane ] += deltaPhase( x, y, 0, plane ) *
                              deltaPhase( x, y, 0, plane );

        }

      }

      // statistics
      avg[ plane ] = as_S1pd[ plane ] / as_Sn[ plane ];
      var[ plane ] = as_S2pd[ plane ] / as_Sn[ plane ] -
                     avg[ plane ] * avg[ plane ];
      sigma[ plane ] = std::sqrt( var[ plane ] );


      // X gradient shim coefficients
      determX[ plane ] = as_Sxx[ plane ] * as_Sn[ plane ] - 
                         as_Sx[ plane ] * as_Sx[ plane ];
      shimGx2[ plane ] = 0.5 * ( as_Sxdx[ plane ] * as_Sn[ plane ] -
                                 as_Sdx[ plane ] * as_Sx[ plane ] ) /
                         determX[ plane ];
      shimGx[ plane ] = ( as_Sxx[ plane ] * as_Sdx[ plane ] -
                          as_Sx[ plane ] * as_Sxdx[ plane ] ) /
                        determX[ plane ] +
                        2.0 * shimGx2[ plane ] *
                        ( as_Sxampl[ plane ] / as_Sampl[ plane ]);

      // Y gradient shim coefficients
      determY[ plane ] = as_Syy[ plane ] * as_Sn[ plane ] - 
                         as_Sy[ plane ] * as_Sy[ plane ];
      shimGy2[ plane ] = 0.5 * ( as_Sydy[ plane ] * as_Sn[ plane ] -
                                 as_Sdy[ plane ] * as_Sy[ plane ] ) /
                         determY[ plane ];
      shimGy[ plane ] = ( as_Syy[ plane ] * as_Sdy[ plane ] -
                          as_Sy[ plane ] * as_Sydy[ plane ] ) /
                        determX[ plane ] +
                        2.0 * shimGy2[ plane ] *
                        ( as_Syampl[ plane ] / as_Sampl[ plane ] );

      // scaled X gradient shim coefficient
      scaledShimGx[ plane ] = shimGx[ plane ] * ( -scale1 );
      scaledShimGy[ plane ] = shimGy[ plane ] * ( -scale1 );
      scaledShimGx2[ plane ] = shimGx2[ plane ] * scale2;
      scaledShimGy2[ plane ] = shimGy2[ plane ] * scale2;

      std::cout << "plane " << plane + 1 << " : " << std::endl;
      std::cout << "as_Sn = " << as_Sn[ plane ] << std::endl;
      std::cout << "as_Sampl = " << as_Sampl[ plane ] << std::endl;
      std::cout << "as_Sdx = " << as_Sdx[ plane ] << std::endl;
      std::cout << "as_Sdy = " << as_Sdy[ plane ] << std::endl;
      std::cout << "as_Sx = " << as_Sx[ plane ] << std::endl;
      std::cout << "as_Sxdx = " << as_Sxdx[ plane ] << std::endl;
      std::cout << "as_Sxampl = " << as_Sxampl[ plane ] << std::endl;
      std::cout << "as_Sxx = " << as_Sxx[ plane ] << std::endl;
      std::cout << "as_Sy = " << as_Sy[ plane ] << std::endl;
      std::cout << "as_Sydy = " << as_Sydy[ plane ] << std::endl;
      std::cout << "as_Syampl = " << as_Syampl[ plane ] << std::endl;
      std::cout << "as_Syy = " << as_Syy[ plane ] << std::endl;
      std::cout << "as_S1pd = " << as_S1pd[ plane ] << std::endl;
      std::cout << "as_S2pd = " << as_S2pd[ plane ] << std::endl;
      std::cout << "avg = " << avg[ plane ] << std::endl;
      std::cout << "var = " << var[ plane ] << std::endl;
      std::cout << "sigma = " << sigma[ plane ] << std::endl;
      std::cout << "determX = " << determX[ plane ] << std::endl;
      std::cout << "shimGx2 = " << shimGx2[ plane ] << std::endl;
      std::cout << "shimGx = " << shimGx[ plane ] << std::endl;
      std::cout << "determY = " << determY[ plane ] << std::endl;
      std::cout << "shimGy2 = " << shimGy2[ plane ] << std::endl;
      std::cout << "shimGy = " << shimGy[ plane ] << std::endl << std::endl;
      std::cout << "scaledShimGx2 = " << scaledShimGx2[ plane ] << std::endl;
      std::cout << "scaledShimGx = " << scaledShimGx[ plane ] << std::endl;
      std::cout << "scaledShimGy2 = " << scaledShimGy2[ plane ] << std::endl;
      std::cout << "scaledShimGy = " << scaledShimGy[ plane ] << std::endl;
      std::cout << std::endl;

    }

    // process new shimming values
    float xx = 0;
    float yy = 0;
    float zz = 0;
    float xx2 = 0;
    float yy2 = 0;
    float zz2 = 0;

    int32_t xshim_values = 0;
    int32_t yshim_values = 0;
    int32_t zshim_values = 0;

    // processing AXIAL results
    if ( entry == 1 ) // Head first
    {

      xx += scaledShimGx[ 0 ];
      xx2 += scaledShimGx2[ 0 ];

    }
    else // Feet first
    {

      xx -= scaledShimGx[ 0 ];
      xx2 -= scaledShimGx2[ 0 ];

    }
    yy += scaledShimGy[ 0 ];
    yy2 += scaledShimGy2[ 0 ];

    ++ xshim_values;
    ++ yshim_values;

    // processing SAGITTAL results
    if ( entry == 1 ) // Head first
    {

      zz += scaledShimGy[ 1 ];
      zz2 += scaledShimGy2[ 1 ];

      switch ( position )
      {

        case 1:

          yy += scaledShimGx[ 1 ];
          yy2 += scaledShimGx2[ 1 ];
          break;

        case 2:

          yy -= scaledShimGx[ 1 ];
          yy2 -= scaledShimGx2[ 1 ];
          break;

        case 3:

          xx -= scaledShimGx[ 1 ];
          xx2 -= scaledShimGx2[ 1 ];
          break;

        case 4:

          xx += scaledShimGx[ 1 ];
          xx2 += scaledShimGx2[ 1 ];
          break;

      }

    }
    else // Feet first
    {

      zz -= scaledShimGy[ 1 ];
      zz2 -= scaledShimGy2[ 1 ];

      switch ( position )
      {

        case 1:

          yy += scaledShimGx[ 1 ];
          yy2 += scaledShimGx2[ 1 ];
          break;

        case 2:

          yy -= scaledShimGx[ 1 ];
          yy2 -= scaledShimGx2[ 1 ];
          break;

        case 3:

          xx += scaledShimGx[ 1 ];
          xx2 += scaledShimGx2[ 1 ];
          break;

        case 4:

          xx -= scaledShimGx[ 1 ];
          xx2 -= scaledShimGx2[ 1 ];
          break;

      }

    }
    yy += scaledShimGy[ 0 ];
    yy2 += scaledShimGy2[ 0 ];

    ++ zshim_values;
    if ( ( position == 3 ) || ( position == 4 ) )
    {

      ++ xshim_values;

    }
    else
    {

      ++ yshim_values;

    }

    // processing CORONAL results
    if ( entry == 1 ) // Head first
    {

      zz += scaledShimGy[ 2 ];
      zz2 += scaledShimGy2[ 2 ];

      switch ( position )
      {

        case 1:

          xx += scaledShimGx[ 2 ];
          xx2 += scaledShimGx2[ 2 ];
          break;

        case 2:

          xx -= scaledShimGx[ 2 ];
          xx2 -= scaledShimGx2[ 2 ];
          break;

        case 3:

          yy += scaledShimGx[ 2 ];
          yy2 += scaledShimGx2[ 2 ];
          break;

        case 4:

          yy -= scaledShimGx[ 2 ];
          yy2 -= scaledShimGx2[ 2 ];
          break;

      }

    }
    else // Feet first
    {

      zz -= scaledShimGy[ 2 ];
      zz2 -= scaledShimGy2[ 2 ];

      switch ( position )
      {

        case 1:

          xx -= scaledShimGx[ 2 ];
          xx2 += scaledShimGx2[ 2 ];
          break;

        case 2:

          xx += scaledShimGx[ 2 ];
          xx2 -= scaledShimGx2[ 2 ];
          break;

        case 3:

          yy += scaledShimGx[ 2 ];
          yy2 -= scaledShimGx2[ 2 ];
          break;

        case 4:

          yy -= scaledShimGx[ 2 ];
          yy2 += scaledShimGx2[ 2 ];
          break;

      }

    }
    yy += scaledShimGy[ 0 ];
    yy2 += scaledShimGy2[ 0 ];

    ++ zshim_values;
    if ( ( position == 1 ) || ( position == 2 ) )
    {

      ++ xshim_values;

    }
    else
    {

      ++ yshim_values;

    }

    // processing shim variation
    int16_t deltaShimX = 0;
    int16_t deltaShimY = 0;
    int16_t deltaShimZ = 0;
    if ( xshim_values > 0 )
    {

      deltaShimX = ( int16_t )rint( ( xx / xshim_values ) * xfull / xfs );
      xx2 /= xshim_values;

    }
    if ( yshim_values > 0 )
    {

      deltaShimY = ( int16_t )rint( ( yy / yshim_values ) * yfull / yfs );
      yy2 /= yshim_values;

    }
    if ( zshim_values > 0 )
    {

      deltaShimZ = ( int16_t )rint( ( zz / zshim_values ) * zfull / zfs );
      zz2 /= zshim_values;

    }

    std::cout << "Old X shim : " << oldShimX << std::endl;
    std::cout << "Old Y shim : " << oldShimY << std::endl;
    std::cout << "Old Z shim : " << oldShimZ << std::endl << std::endl;

    std::cout << "Delta X shim : " << deltaShimX << std::endl;
    std::cout << "Delta Y shim : " << deltaShimY << std::endl;
    std::cout << "Delta Z shim : " << deltaShimZ << std::endl << std::endl;

    std::cout << "New X shim : " << oldShimX + deltaShimX << std::endl;
    std::cout << "New Y shim : " << oldShimY + deltaShimY << std::endl;
    std::cout << "New Z shim : " << oldShimZ + deltaShimZ << std::endl;

  }
  GKG_CATCH( "void gkg::AutoshimCommand::execute( "
             "const std::string& fileNameDeltaPhase, "
             "const std::string& fileNameMagnitude, "
             "float radius, "
             "int32_t entry, "
             "int32_t position, "
             "int16_t xfull, "
             "int16_t yfull, "
             "int16_t zfull, "
             "float xfs, "
             "float yfs, "
             "float zfs, "
             "int16_t oldShimX, "
             "int16_t oldShimY, "
             "int16_t oldShimZ )" );

}


RegisterCommandCreator(
    AutoshimCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameDeltaPhase ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMagnitude ) +
    DECLARE_FLOATING_PARAMETER_HELP( radius ) +
    DECLARE_INTEGER_PARAMETER_HELP( entry ) +
    DECLARE_INTEGER_PARAMETER_HELP( position ) +
    DECLARE_INTEGER_PARAMETER_HELP( xfull ) +
    DECLARE_INTEGER_PARAMETER_HELP( yfull ) +
    DECLARE_INTEGER_PARAMETER_HELP( zfull ) +
    DECLARE_FLOATING_PARAMETER_HELP( xfs ) +
    DECLARE_FLOATING_PARAMETER_HELP( yfs ) +
    DECLARE_FLOATING_PARAMETER_HELP( zfs ) +
    DECLARE_INTEGER_PARAMETER_HELP( oldShimX ) +
    DECLARE_INTEGER_PARAMETER_HELP( oldShimY ) +
    DECLARE_INTEGER_PARAMETER_HELP( oldShimZ ) );
