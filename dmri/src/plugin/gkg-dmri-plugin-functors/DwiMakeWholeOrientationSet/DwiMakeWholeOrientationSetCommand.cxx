#include <gkg-dmri-plugin-functors/DwiMakeWholeOrientationSet/DwiMakeWholeOrientationSetCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-coordinates/OrientationSet.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <vector>
#include <fstream>

//
// Coulomb energy and forces
//

float 
getCoulombEnergy( const std::vector< gkg::Vector3d< float > >& orientations )
{

  try
  {

    float energy = 0;
    for ( std::vector< gkg::Vector3d< float > >::size_type i = 0;
          i < orientations.size(); ++i )
    {

      for ( std::vector< gkg::Vector3d< float > >::size_type j = i + 1;
            j < orientations.size(); ++j )
      {

        energy += 1 / ( orientations[ j ] - orientations[ i ] ).getNorm();
        if ( ( orientations[ j ] - orientations[ i ] ).getNorm() > 100000 )
        {

          std::cout << "warning" << std::endl;

        }

      }

    }
    return energy;

  }
  GKG_CATCH( "double "
             "getCoulombEnergy( "
             "const std::vector< gkg::Vector3d< float > >& orientations )" );

}


void getForces( std::vector< gkg::Vector3d< float > >& forces,
                const std::vector< gkg::Vector3d< float > >& orientations )
{

  try
  {

    std::vector< gkg::Vector3d< float > >::size_type i, j;
    for ( i = 0; i < forces.size(); ++i )
    {

      forces[ i ].x = forces[ i ].y = forces[ i ].z = 0;

    }

    for ( i = 0; i < forces.size(); ++i )
    {

      for ( j = i + 1; j < forces.size(); ++j )
      {

        gkg::Vector3d< float > difference = orientations[ i ] -
                                            orientations[ j ];
        float l = difference.getNorm();
        l = 1 / ( l * l * l );
        float ff;
        ff = l * difference.x; forces[ i ].x += ff; forces[ j ].x -= ff;
        ff = l * difference.y; forces[ i ].y += ff; forces[ j ].y -= ff;
        ff = l * difference.z; forces[ i ].z += ff; forces[ j ].z -= ff;

      }

    }

  }
  GKG_CATCH( "void getForces( std::vector< gkg::Vector3d< float > >& forces, "
             "const std::vector< gkg::Vector3d< float > >& orientations )" );

}


//
// class DwiMakeWholeOrientationSetCommand
//

gkg::DwiMakeWholeOrientationSetCommand::DwiMakeWholeOrientationSetCommand(
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
  GKG_CATCH( "gkg::DwiMakeWholeOrientationSetCommand::DwiMakeWholeOrientationSetCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiMakeWholeOrientationSetCommand::DwiMakeWholeOrientationSetCommand(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                int32_t furtherOrientationCount,
                                                int32_t stepCount,
                                                float step,
                                                float minimumStep,
                                                bool verbose )
                                       : gkg::Command()
{

  try
  {

    execute( fileNameIn,
             fileNameOut,
             furtherOrientationCount,
             stepCount,
             step,
             minimumStep,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMakeWholeOrientationSetCommand::DwiMakeWholeOrientationSetCommand( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t furtherOrientationCount, "
             "int32_t stepCount, "
             "float step, "
             "float minimumStep, "
             "bool verbose )" );

}


gkg::DwiMakeWholeOrientationSetCommand::DwiMakeWholeOrientationSetCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameIn );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, furtherOrientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCount );
    DECLARE_FLOATING_PARAMETER( parameters, float, step );
    DECLARE_FLOATING_PARAMETER( parameters, float, minimumStep );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameIn,
             fileNameOut,
             furtherOrientationCount,
             stepCount,
             step,
             minimumStep,
             verbose );

  }
  GKG_CATCH( "gkg::DwiMakeWholeOrientationSetCommand::"
             "DwiMakeWholeOrientationSetCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiMakeWholeOrientationSetCommand::~DwiMakeWholeOrientationSetCommand()
{
}


std::string gkg::DwiMakeWholeOrientationSetCommand::getStaticName()
{

  try
  {

    return "DwiMakeWholeOrientationSet";

  }
  GKG_CATCH( "std::string gkg::DwiMakeWholeOrientationSetCommand::getStaticName()" );

}


void gkg::DwiMakeWholeOrientationSetCommand::parse()
{

  try
  {

    std::string fileNameIn;
    std::string fileNameOut; 
    int32_t furtherOrientationCount = 0;
    int32_t stepCount = 10000;
    float step = 0.01;
    float minimumStep = 1e-10;   
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Adding further orientation to an existing "
                                  "orientation set",
                                  _loadPlugin );
    application.addSingleOption( "-i",
                                 "Input orientation set file name",
                                 fileNameIn );
    application.addSingleOption( "-o",
                                 "Output orientation set file name",
                                 fileNameOut );
    application.addSingleOption( "-furtherOrientationCount",
                                 "Further orientation count",
                                 furtherOrientationCount );		
    application.addSingleOption( "-stepCount",
                                 "Step count (default=10000)",
                                 stepCount,
                                 true );		
    application.addSingleOption( "-step",
                                 "Step (default=0.01)",
                                 step,
                                 true );		
    application.addSingleOption( "-minimumStep",
                                 "Minimum step (default=1e-10)",
                                 minimumStep,
                                 true );		
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true );
    application.initialize();

    execute( fileNameIn,
             fileNameOut,
             furtherOrientationCount,
             stepCount,
             step,
             minimumStep,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiMakeWholeOrientationSetCommand::parse()" );

}


void gkg::DwiMakeWholeOrientationSetCommand::execute(
                                                const std::string& fileNameIn,
                                                const std::string& fileNameOut,
                                                int32_t furtherOrientationCount,
                                                int32_t stepCount,
                                                float step,
                                                float minimumStep,
                                                bool verbose )
{

  try
  {

    //
    // building input orientationset
    //

    if ( verbose )
    {

      std::cout << "reading input orientation set : " << std::flush;

    }
    gkg::OrientationSet inputOrientationSet( fileNameIn );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // building further orientation set
    //

    if ( verbose )
    {

      std::cout << "preparing further orientation set : " << std::flush;

    }
    gkg::ElectrostaticOrientationSet
      furtherOrientationSet( furtherOrientationCount );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // building vector of final orientations
    //

    if ( verbose )
    {

      std::cout << "building vector of final orientations : " << std::flush;

    }
    int32_t globalOrientationCount = inputOrientationSet.getCount() +
                                     furtherOrientationSet.getCount();

    std::vector< gkg::Vector3d< float > >
                                        orientations0( globalOrientationCount );
    std::vector< gkg::Vector3d< float > >
                                        orientations1( globalOrientationCount );
    std::vector< gkg::Vector3d< float > >* pOrientations0 = &orientations0;
    std::vector< gkg::Vector3d< float > >* pOrientations1 = &orientations1;
    std::vector< gkg::Vector3d< float > > forces( globalOrientationCount );

    int32_t o = 0;
    for ( o = 0; o < inputOrientationSet.getCount(); o++ )
    {

      orientations0[ o ] = inputOrientationSet.getOrientation( o );

    }

    for ( o = 0; o < furtherOrientationSet.getCount(); o++ )
    {

      orientations0[ inputOrientationSet.getCount() + o ] = 
                                      furtherOrientationSet.getOrientation( o );

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    //
    // computing solution
    //

    if ( verbose )
    {

      std::cout << "optimizing orientations : " << std::flush;

    }
    float e0 = getCoulombEnergy( orientations0 );
        std::cout << std::endl << e0 << std::endl;
    for( int32_t k = 0; k < stepCount; ++k )
    {

      getForces( forces, orientations0 );
      for ( int32_t i = 0; i < globalOrientationCount; ++i )
      {

        if ( i >= inputOrientationSet.getCount() )
        {

          float d = forces[ i ].dot( ( *pOrientations0 )[ i ] );
  
          forces[ i ].x  -= ( *pOrientations0 )[ i ].x * d;
          forces[ i ].y  -= ( *pOrientations0 )[ i ].y * d;
          forces[ i ].z  -= ( *pOrientations0 )[ i ].z * d;
          ( *pOrientations1 )[ i ].x = ( *pOrientations0 )[ i ].x +
                                       forces[ i ].x * step;
          ( *pOrientations1 )[ i ].y = ( *pOrientations0 )[ i ].y +
                                       forces[ i ].y * step;
          ( *pOrientations1 )[ i ].z = ( *pOrientations0 )[ i ].z +
                                       forces[ i ].z * step;
          float l = ( *pOrientations1 )[ i ].getNorm();

          ( *pOrientations1 )[ i ].x /= l;
          ( *pOrientations1 )[ i ].y /= l;
          ( *pOrientations1 )[ i ].z /= l;

        }
        else
        {

          ( *pOrientations1 )[ i ] = ( *pOrientations0 )[ i ];

        }
        std::cout << ( *pOrientations1 )[ i ] << " ";
      }
      std::cout << std::endl;
      float e = getCoulombEnergy( *pOrientations1 );
      std::cout << e << std::endl;
      if ( e >= e0 )
      {

        step /= 2;
        if ( step < minimumStep )
        {
          
          break;

        }
        continue;

      }
      else
      {

        std::vector< gkg::Vector3d< float > >* t = pOrientations0;
        pOrientations0 = pOrientations1;
        pOrientations1 = t;
        e0 = e;
        step *= 2;

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //
    // writing result
    //

    if ( verbose )
    {

      std::cout << "writing output orientations : " << std::flush;

    }
    std::ofstream os( fileNameOut.c_str() );
    os << globalOrientationCount << std::endl;
    for ( int32_t o = 0; o < globalOrientationCount; o++ )
    {

      os << orientations0[ o ].x << " "
         << orientations0[ o ].y << " "
         << orientations0[ o ].z << std::endl;

    }
    os.close();
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "void gkg::DwiMakeWholeOrientationSetCommand::execute( "
             "const std::string& fileNameIn, "
             "const std::string& fileNameOut, "
             "int32_t furtherOrientationCount, "
             "int32_t stepCount, "
             "float step, "
             "float minimumStep, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiMakeWholeOrientationSetCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameIn ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    DECLARE_INTEGER_PARAMETER_HELP( furtherOrientationCount ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCount ) +
    DECLARE_FLOATING_PARAMETER_HELP( step ) +
    DECLARE_FLOATING_PARAMETER_HELP( minimumStep ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
