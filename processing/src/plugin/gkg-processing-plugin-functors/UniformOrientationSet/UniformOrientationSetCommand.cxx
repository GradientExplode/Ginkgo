#include <gkg-processing-plugin-functors/UniformOrientationSet/UniformOrientationSetCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-algobase/RandomIndexVector.h>
#include <gkg-processing-orientations/Node.h>
#include <gkg-processing-orientations/SlidingWindowOrientationSet.h>
#include <gkg-processing-orientations/Orientation.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>


gkg::UniformOrientationSetCommand::UniformOrientationSetCommand(
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
  GKG_CATCH( "gkg::UniformOrientationSetCommand::UniformOrientationSetCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::UniformOrientationSetCommand::UniformOrientationSetCommand(
                                  	       const std::string& pattern,
                                  	       float multiplicator,
                                  	       int32_t orientationCount,
                                  	       int32_t windowSize,
                                  	       float threshold,
                                  	       float power,
                                  	       int32_t localMinimization,
                                  	       int32_t globalStepCount,
                                  	       int32_t internalStepCount,
                                  	       int32_t probabilityFactor,
                                  	       std::string& fileNameOut,
                                  	       const std::string& algorithmType,
                                  	       float initialStep,
                                  	       float minimumStep,
                                               bool avoidSymmetricOrientations,
                                  	       bool verbose )
                                 : gkg::Command()
{

  try
  {

    execute( pattern, multiplicator, orientationCount, windowSize, threshold,
             power, localMinimization, globalStepCount, internalStepCount,
             probabilityFactor, fileNameOut, algorithmType, initialStep,
             minimumStep, avoidSymmetricOrientations, verbose );

  }
  GKG_CATCH( "gkg::UniformOrientationSetCommand::UniformOrientationSetCommand( "
             "const std::string& pattern, float multiplicator, "
             "int32_t orientationCount, int32_t windowSize, float threshold, "
             "float power, int32_t localMinimization, int32_t globalStepCount, "
             "int32_t internalStepCount, int32_t probabilityFactor, "
             "std::string& fileNameOut, const std::string& algorithmType, "
             "float initialStep, float minimumStep, "
             "bool avoidSymmetricOrientations, bool verbose )" );

}


gkg::UniformOrientationSetCommand::UniformOrientationSetCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, pattern );
    DECLARE_FLOATING_PARAMETER( parameters, float, multiplicator );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, orientationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, windowSize );
    DECLARE_FLOATING_PARAMETER( parameters, float, threshold );
    DECLARE_FLOATING_PARAMETER( parameters, float, power );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, localMinimization );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, globalStepCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, internalStepCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, probabilityFactor );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, algorithmType );
    DECLARE_FLOATING_PARAMETER( parameters, float, initialStep );
    DECLARE_FLOATING_PARAMETER( parameters, float, minimumStep );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, avoidSymmetricOrientations );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( pattern, multiplicator, orientationCount, windowSize, threshold,
             power, localMinimization, globalStepCount, internalStepCount,
             probabilityFactor, fileNameOut, algorithmType, initialStep,
             minimumStep, avoidSymmetricOrientations, verbose );

  }
  GKG_CATCH( "gkg::UniformOrientationSetCommand::UniformOrientationSetCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::UniformOrientationSetCommand::~UniformOrientationSetCommand()
{
}


std::string gkg::UniformOrientationSetCommand::getStaticName()
{

  try
  {

    return "UniformOrientationSet";

  }
  GKG_CATCH( "std::string gkg::UniformOrientationSetCommand::getStaticName()" );

}


void gkg::UniformOrientationSetCommand::parse()
{

  try
  {

    std::string pattern = "";
    float multiplicator = 1.0;

    int32_t orientationCount = 0;
    int32_t windowSize = 6;
    float threshold = 1.0;
    float power = 1.0;
    int32_t localMinimization = 0;

    int32_t globalStepCount = 10000;
    int32_t internalStepCount = 10000;
    int32_t probabilityFactor = 2;
    std::string fileNameOut;
    std::string algorithmType = "tree";
    float initialStep = 0.01;
    float minimumStep = 1e-8;
    bool avoidSymmetricOrientations = true;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
             			  "Discrete spheric distribution\n\n"
             			  "p, m, pf are \"tree\" algorithm parameters\n"
             			  "oc, ws, t, po, lm are \"sliding_window\" "
             			  "algorithm parameters\n",
                                  _loadPlugin );

    application.addSingleOption( "-p",
                                 "Orientation pattern (ex: '[6,6],6' )",
                                 pattern,
                                 true );
    application.addSingleOption( "-m",
                                 "Interaction multiplicator >=1.0\n"
                                 "a = ( 1 / m )^( p - 1 ) with\n"
                                 "-> a is the resulting threshold\n"
                                 "-> p is the number of sub-set(s)\n"
                                 " (default=1.0)",
                                 multiplicator,
                                 true );
    application.addSingleOption( "-pf",
                                 "Probability factor that node in depth "
                                 "are probably moved >=1 (default=2)",
                                 probabilityFactor,
                                 true );

    application.addSingleOption( "-oc",
                                 "Orientation count",
                                 orientationCount,
                                 true );
    application.addSingleOption( "-ws",
                                 "Window size (default=6)",
                                 windowSize,
                                 true );
    application.addSingleOption( "-t",
                                 "Threshold between 0.0 and 1.0 (default=1.0)",
                                 threshold,
                                 true );
    application.addSingleOption( "-po",
                                 "Power (default=1.0)",
                                 power,
                                 true );
    application.addSingleOption( "-lm",
                                 "Local minimization of each subset :\n"
                                 "- 0 -> none\n"
                                 "- 1 -> loop over sub-set(s) "
                                 "and internal step(s)\n"
                                 "- 2 -> loop over internal step(s) "
                                 "and sub-set(s)\n",
                                 localMinimization,
                                 true );

    application.addSingleOption( "-gs",
                                 "Maximum number of global minimization steps "
                                 "(default=10000)",
                                 globalStepCount,
                                 true );
    application.addSingleOption( "-is",
                                 "Number of internal steps "
                                 "(default=10000)",
                                 internalStepCount,
                                 true );
    application.addSingleOption( "-o",
                                 "Output filename",
                                 fileNameOut );                             
    application.addSingleOption( "-a",
                                 "Algorithm type:\n"
                                 "- tree\n"
                                 "- sliding_window\n"
                                 "(default=tree)\n",
                                 algorithmType );
    application.addSingleOption( "-initialStep",
                                 "Initial motion step "
                                 "(default=0.01)",
                                 initialStep,
                                 true );
    application.addSingleOption( "-minimumStep",
                                 "Minimum motion step "
                                 "(default=1e-10)",
                                 minimumStep,
                                 true );
    application.addSingleOption( "-avoidSymmetricOrientations",
                                 "Do not induce orientation sets that include "
                                 "symmetric orientations (default=true )",
                                 avoidSymmetricOrientations, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( pattern, multiplicator, orientationCount, windowSize, threshold,
             power, localMinimization, globalStepCount, internalStepCount,
             probabilityFactor, fileNameOut, algorithmType, initialStep,
             minimumStep, avoidSymmetricOrientations, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::UniformOrientationSetCommand::parse()" );

}


void gkg::UniformOrientationSetCommand::execute(
                  	          	       const std::string& pattern,
                                  	       float multiplicator,
                                  	       int32_t orientationCount,
                                  	       int32_t windowSize,
                                  	       float threshold,
                                  	       float power,
                                  	       int32_t localMinimization,
                                  	       int32_t globalStepCount,
                                  	       int32_t internalStepCount,
                                  	       int32_t probabilityFactor,
                                  	       std::string& fileNameOut,
                                  	       const std::string& algorithmType,
                                  	       float initialStep,
                                  	       float minimumStep,
                                               bool avoidSymmetricOrientations,
                                  	       bool verbose )
{

  try
  {

    if ( !pattern.empty() && orientationCount )
    {

      throw std::runtime_error( "-p and -oc are not useable together" );

    }


    if ( !pattern.empty() && ( algorithmType == "sliding_window" ) )
    {

      throw std::runtime_error(
              "-p incompatible with sliding_window algorithm" );

    }

    if ( orientationCount && ( algorithmType == "tree" ) )
    {

      throw std::runtime_error(
              "-oc incompatible with tree algorithm" );

    }

    if ( ( algorithmType != "tree" ) &&
         ( algorithmType != "sliding_window" ) )
    {

      throw std::runtime_error( "not a valid algorithm type!" );

    }

    if ( ( threshold < 0.0 ) || ( threshold > 1.0 ) )
    {

      throw std::runtime_error( "threshold must be between 0.0 and 1.0" );

    }

    if ( probabilityFactor < 1 )
    {

      throw std::runtime_error( "probability factor must be >= 1" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // tree algorithm
    ////////////////////////////////////////////////////////////////////////////
    if ( algorithmType == "tree" )
    {

      gkg::Node* root = new gkg::Node( pattern, avoidSymmetricOrientations );
      gkg::Node* randomNode = 0;

      root->setWeight( 1, probabilityFactor );

      if ( verbose )
      {

        std::cout << "minimizing energy : " << std::flush;

      }

      float e0 = root->getGlobalEnergy( multiplicator );
      float e = 0.0;
      float globalStep = initialStep;
      float localStep;
      int32_t gs, is;
      for ( gs = 0; gs < globalStepCount; gs++ )
      {

        if ( verbose )
        {

          if ( gs != 0 )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << "step[ " << std::setw( 5 ) << gs + 1 
                    << " / " << std::setw( 5 ) << globalStepCount
                    << " ] " << std::flush;

        }

        // the deeper the node is,
        // the more important the probability to be moved
        randomNode = root->getRandomNode();
        localStep = initialStep;

        for ( is = 0; is < internalStepCount; is++ )
        {

          if ( verbose )
          {

            if ( is != 0 )
            {

              std::cout << gkg::Eraser( 22 );

            }
            std::cout << "step[ " << std::setw( 5 ) << is + 1
                      << " / " << std::setw( 5 ) << internalStepCount
                      << " ] " << std::flush;

          }
          root->resetForce();
          root->setForces( multiplicator );

          randomNode->move( localStep );

          e = root->getGlobalEnergy( multiplicator );

          if ( e >= e0 )
          {

            // not successful
            randomNode->undo();
            localStep /= 2.0;

          }
          else
          {

            // successful
            e0 = e;
            localStep *= 2;

          }

          if ( localStep < minimumStep )
          {

            break;

          }

        }
        if ( verbose )
        {

          std::cout << gkg::Eraser( 22 ) << std::flush;

        }

        if ( e >= e0 )
        {

          // not successful
          globalStep /= 2.0;

        }
        else
        {

          // successful
          globalStep *= 2;

        }
        if ( globalStep < minimumStep )
        {

          break;

        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 ) << "done" << std::endl;

      }

      if ( fileNameOut.substr( fileNameOut.length() - 4U, 4U ) != ".txt" )
      {

        fileNameOut = fileNameOut + ".txt";

      }
      std::ofstream os( fileNameOut.c_str() );
      os << root->getOrientationCount() << std::endl;
      os << *root << std::endl;
      os.close();


      std::string 
        fileNameCpp = fileNameOut.substr( 0U, fileNameOut.length() - 4U ) +
                      ".cxx";
      os.open( fileNameCpp.c_str() );

      os << "static float _orientation"
         << root->getOrientationCount()
         << "[ "
         << root->getOrientationCount()
         << " ][ 3 ] ="
         << std::endl
         << "{"
         << std::endl
         << std::endl;

      printCpp( os, *root );

      os << std::endl
         << "};"
         << std::endl;

      os.close();


      std::string fileNameEnergy = "";
      if ( fileNameOut.substr( fileNameOut.length() - 4U, 4U ) == ".txt" )
      {

        fileNameEnergy = fileNameOut.substr( 0U, fileNameOut.length() - 4U ) +
                         "_energy.txt";

      }
      else
      {

        fileNameEnergy = fileNameOut +
                         "_energy.txt";

      }
      os.open( fileNameEnergy.c_str() );
      os << "algorithm         : tree" << std::endl;
      os << "avoid sym. orient.: " << avoidSymmetricOrientations << std::endl;
      os << "pattern           : " << pattern << std::endl;
      os << "multiplicator     : " << multiplicator << std::endl;
      os << "probabilityFactor : " << probabilityFactor << std::endl;
      os << "globalStepCount   : " << globalStepCount << std::endl;
      os << "internalStepCount : " << internalStepCount << std::endl;
      os << "initialStep       : " << initialStep << std::endl;
      os << "minimumStep       : " << minimumStep << std::endl;
      os << "minimized energy  : " << e << std::endl;
      os.close();

      delete root;

    }
    ////////////////////////////////////////////////////////////////////////////
    // sliding_window algorithm
    ////////////////////////////////////////////////////////////////////////////
    else
    {

      gkg::SlidingWindowOrientationSet 
        slidingWindowOrientationSet( windowSize, orientationCount,
                                     avoidSymmetricOrientations );

      if ( verbose )
      {

        std::cout << "minimizing energy : " << std::flush;

      }
      
      float e0 = slidingWindowOrientationSet.getGlobalEnergy( threshold,
                                                              power );
      float globalStep = initialStep;
      float localStep;

      int32_t gs, is;
      float e = 0.0;
      std::vector< int32_t > 
           ranks( slidingWindowOrientationSet.getSubSetCount() );
      for ( gs = 0; gs < globalStepCount; gs++ )
      {

        if ( verbose )
        {

          if ( gs != 0 )
          {

            std::cout << gkg::Eraser( 22 );

          }
          std::cout << "step[ " << std::setw( 5 ) << gs + 1 
                    << " / " << std::setw( 5 ) << globalStepCount
                    << " ] " << std::flush;

        }

        slidingWindowOrientationSet.setForces( threshold, power );
        slidingWindowOrientationSet.move( globalStep );
        e = slidingWindowOrientationSet.getGlobalEnergy( threshold, power );
        if ( e >= e0 )
        {

          // not successful
          slidingWindowOrientationSet.undo();
          globalStep /= 2.0;

        }
        else
        {

          // successful
          e0 = e;
          globalStep *= 2;


        }
        if ( globalStep < minimumStep )
        {

          break;

        }

        if ( localMinimization == 1 )
        {
   
          gkg::getRandomIndexVector( ranks );

          int32_t s;
          int32_t subSetCount = slidingWindowOrientationSet.getSubSetCount();
          for ( s = 0; s < subSetCount; s++ )
          {

            localStep = globalStep;
            for ( is = 0; is < internalStepCount; is++ )
            {

              if ( verbose )
              {

                if ( is != 0 )
                {

                  std::cout << gkg::Eraser( 22 );

                }
                std::cout << "step[ " << std::setw( 5 ) << is + 1
                          << " / " << std::setw( 5 ) << internalStepCount
                          << " ] " << std::flush;

              }
              slidingWindowOrientationSet.setForces( threshold, power );
              slidingWindowOrientationSet.moveSubSet( ranks[ s ], localStep );

              e = slidingWindowOrientationSet.getGlobalEnergy( threshold,
                                                               power );
              if ( e >= e0 )
              {

                // not successful
                slidingWindowOrientationSet.undoSubSet( ranks[ s ] );
                localStep /= 2.0;

              }
              else
              {

                // successful
                e0 = e;
                localStep *= 2;

              }
              if ( localStep < minimumStep )
              {

                break;

              }

            }
            if ( verbose )
            {

              std::cout << gkg::Eraser( 22 ) << std::flush;

            }

          }


        }
        else if ( localMinimization == 2 )
        {

          
          gkg::getRandomIndexVector( ranks );

          int32_t s;
          int32_t subSetCount = slidingWindowOrientationSet.getSubSetCount();
          localStep = globalStep;
          for ( is = 0; is < internalStepCount; is++ )
          {

            if ( verbose )
            {

              if ( is != 0 )
              {

                std::cout << gkg::Eraser( 22 );

              }
              std::cout << "step[ " << std::setw( 5 ) << is + 1 
                        << " / " << std::setw( 5 ) << internalStepCount
                        << " ] " << std::flush;

            }

            for ( s = 0; s < subSetCount; s++ )
            {

              slidingWindowOrientationSet.setForces( threshold, power );
              slidingWindowOrientationSet.moveSubSet( ranks[ s ], localStep );

              e = slidingWindowOrientationSet.getGlobalEnergy( threshold,
                                                               power );
              if ( e >= e0 )
              {

                // not successful
                slidingWindowOrientationSet.undoSubSet( ranks[ s ] );

              }
              else
              {

                // successful
                e0 = e;

              }

            }

            if ( e >= e0 )
            {

              // not successful
              localStep /= 2.0;

            }
            else
            {

              // successful
              localStep *= 2;

            }
            if ( localStep < minimumStep )
            {

              break;

            }

          }
          if ( verbose )
          {

            std::cout << gkg::Eraser( 22 ) << std::flush;

          }


        }

      }
      if ( verbose )
      {

        std::cout << gkg::Eraser( 22 ) << "done" << std::endl;

      }

      if ( fileNameOut.substr( fileNameOut.length() - 4U, 4U ) != ".txt" )
      {

        fileNameOut = fileNameOut + ".txt";

      }

      std::ofstream os( fileNameOut.c_str() );
      os << slidingWindowOrientationSet.getOrientationCount() << std::endl;
      os << slidingWindowOrientationSet << std::endl;
      os.close();

      std::string 
        fileNameCpp = fileNameOut.substr( 0U, fileNameOut.length() - 4U ) +
                      ".cxx";
      os.open( fileNameCpp.c_str() );

      os << "static float _orientation"
         << slidingWindowOrientationSet.getOrientationCount()
         << "[ "
         << slidingWindowOrientationSet.getOrientationCount()
         << " ][ 3 ] ="
         << std::endl
         << "{"
         << std::endl
         << std::endl;

      std::list< Orientation* >::const_iterator
        o = slidingWindowOrientationSet.getOrientations().begin(),
        oe = slidingWindowOrientationSet.getOrientations().end();

      while ( o != oe )
      {

        os << "  { "
           << ( *o )->getUnitVector().x
           << ", "
           << ( *o )->getUnitVector().y
           << ", "
           << ( *o )->getUnitVector().z
           << " },"
           << std::endl;

        ++ o;

      }
      os << std::endl
         << "};"
         << std::endl;

      os.close();

      std::string fileNameEnergy = "";
      if ( fileNameOut.substr( fileNameOut.length() - 4U, 4U ) == ".txt" )
      {

        fileNameEnergy = fileNameOut.substr( 0U, fileNameOut.length() - 4U ) +
                         "_energy.txt";

      }
      else
      {

        fileNameEnergy = fileNameOut +
                         "_energy.txt";

      }
      os.open( fileNameEnergy.c_str() );
      os << "algorithm         : sliding_window" << std::endl;
      os << "avoid sym. orient.: " << avoidSymmetricOrientations << std::endl;
      os << "orientationCount  : " << orientationCount << std::endl;
      os << "windowSize        : " << windowSize << std::endl;
      os << "threshold         : " << threshold << std::endl;
      os << "power             : " << power << std::endl;
      os << "localMinimization : " << localMinimization << std::endl;
      os << "globalStepCount   : " << globalStepCount << std::endl;
      os << "internalStepCount : " << internalStepCount << std::endl;
      os << "initialStep       : " << initialStep << std::endl;
      os << "minimumStep       : " << minimumStep << std::endl;
      os << "minimized energy  : " << e << std::endl;
      os.close();

    }

  }
  GKG_CATCH( "void gkg::UniformOrientationSetCommand::execute( "
             "const std::string& pattern, float multiplicator, "
             "int32_t orientationCount, int32_t windowSize, float threshold, "
             "float power, int32_t localMinimization, int32_t globalStepCount, "
             "int32_t internalStepCount, int32_t probabilityFactor, "
             "std::string& fileNameOut, const std::string& algorithmType, "
             "float initialStep, float minimumStep, "
             "bool avoidSymmetricOrientations, bool verbose )" );

}


RegisterCommandCreator( UniformOrientationSetCommand,
    			DECLARE_STRING_PARAMETER_HELP( pattern ) +
    			DECLARE_FLOATING_PARAMETER_HELP( multiplicator ) +
    			DECLARE_INTEGER_PARAMETER_HELP( orientationCount ) +
    			DECLARE_INTEGER_PARAMETER_HELP( windowSize ) +
    			DECLARE_FLOATING_PARAMETER_HELP( threshold ) +
    			DECLARE_FLOATING_PARAMETER_HELP( power ) +
    			DECLARE_INTEGER_PARAMETER_HELP( localMinimization ) +
    			DECLARE_INTEGER_PARAMETER_HELP( globalStepCount ) +
    			DECLARE_INTEGER_PARAMETER_HELP( internalStepCount ) +
    			DECLARE_INTEGER_PARAMETER_HELP( probabilityFactor ) +
    			DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    			DECLARE_STRING_PARAMETER_HELP( algorithmType ) +
    			DECLARE_FLOATING_PARAMETER_HELP( initialStep ) +
    			DECLARE_FLOATING_PARAMETER_HELP( minimumStep ) +
  			DECLARE_FLOATING_PARAMETER_HELP(
                                                  avoidSymmetricOrientations ) +
    			DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
