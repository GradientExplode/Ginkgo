#include <gkg-simulation-plugin-functors/MedusaGeometryDeoverlaper/MedusaGeometryDeoverlaperCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-kokkos/Defines.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <iomanip>

#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-communication-sysinfo/Directory.h>


gkg::MedusaGeometryDeoverlaperCommand::MedusaGeometryDeoverlaperCommand(
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
  GKG_CATCH( "gkg::MedusaGeometryDeoverlaperCommand::"
             "MedusaGeometryDeoverlaperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MedusaGeometryDeoverlaperCommand::MedusaGeometryDeoverlaperCommand(
                const std::string& fileNameGeometryInputMedusaGeometry,
                const std::string& fileNameGeometryOutputMedusaGeometry,
                const std::string& fileNameGeometryOutputMesh,
                const std::string& fileNameGeometryOutputEnergyPlot,
                float maximumForceNorm,
                int32_t atomSolverMaximumIterationCount,
                int32_t repulsionForceMovingAverageWindowSize,
                float minimumRepulsionForceAttenuationPercentage,
                float repulsionForceStddevPercentageThreshold,
                const std::string& fileNameLog,
                bool applyKokkosInitialize,
                bool applyKokkosFinalize,
                bool verbose )
{

  try
  {

    execute( fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             maximumForceNorm,
             atomSolverMaximumIterationCount,
             repulsionForceMovingAverageWindowSize,
             minimumRepulsionForceAttenuationPercentage,
             repulsionForceStddevPercentageThreshold,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaGeometryDeoverlaperCommand::"
             "MedusaGeometryDeoverlaperCommand( "
             "const std::string& fileNameGeometryInputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMesh, "
             "const std::string& fileNameGeometryOutputEnergyPlot, "
             "float maximumForceNorm, "
             "int32_t atomSolverMaximumIterationCount, "
             "int32_t repulsionForceMovingAverageWindowSize, "
             "float minimumRepulsionForceAttenuationPercentage, "
             "float repulsionForceStddevPercentageThreshold, "
             "const std::string& fileNameLog, "
             "bool applyKokkosInitialize, "
             "bool applyKokkosFinalize, "
             "bool verbose )" );

}


gkg::MedusaGeometryDeoverlaperCommand::MedusaGeometryDeoverlaperCommand(
                                             const gkg::Dictionary& parameters )
                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryInputMedusaGeometry );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputMedusaGeometry );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputEnergyPlot );
    DECLARE_FLOATING_PARAMETER( parameters, float, maximumForceNorm );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t,
                               atomSolverMaximumIterationCount );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t,
                               repulsionForceMovingAverageWindowSize );
    DECLARE_FLOATING_PARAMETER( parameters, float, 
                                minimumRepulsionForceAttenuationPercentage );
    DECLARE_FLOATING_PARAMETER( parameters, float, 
                                repulsionForceStddevPercentageThreshold );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameLog );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyKokkosInitialize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyKokkosFinalize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             maximumForceNorm,
             atomSolverMaximumIterationCount,
             repulsionForceMovingAverageWindowSize,
             minimumRepulsionForceAttenuationPercentage,
             repulsionForceStddevPercentageThreshold,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaGeometryDeoverlaperCommand::"
             "MedusaGeometryDeoverlaperCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MedusaGeometryDeoverlaperCommand::~MedusaGeometryDeoverlaperCommand()
{
}


std::string gkg::MedusaGeometryDeoverlaperCommand::getStaticName()
{

  try
  {

    return "MedusaGeometryDeoverlaper";

  }
  GKG_CATCH( "std::string "
             "gkg::MedusaGeometryDeoverlaperCommand::"
             "getStaticName()" );

}


void gkg::MedusaGeometryDeoverlaperCommand::parse()
{

  try
  {

    std::string fileNameGeometryInputMedusaGeometry;
    std::string fileNameGeometryOutputMedusaGeometry;
    std::string fileNameGeometryOutputMesh;
    std::string fileNameGeometryOutputEnergyPlot;
    float maximumForceNorm = 0.25f;
    int32_t atomSolverMaximumIterationCount = 500;
    int32_t repulsionForceMovingAverageWindowSize = 10;
    float minimumRepulsionForceAttenuationPercentage = 10.0f;
    float repulsionForceStddevPercentageThreshold = 1.0f;
    std::string fileNameLog;
    bool applyKokkosInitialize = true;
    bool applyKokkosFinalize = true;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "The MEDUSA Geometry Deoverlaper tool",
                                  _loadPlugin );
    application.addSingleOption( "-geometryInputMedusaGeometry",
                                 "Virtual tissue dictionary file name",
                                 fileNameGeometryInputMedusaGeometry );
    application.addSingleOption( "-geometryOutputMedusaGeometry",
                                 "Output virtual tissue raw file name",
                                 fileNameGeometryOutputMedusaGeometry,
                                 true );
    application.addSingleOption( "-geometryOutputMesh",
                                 "Output virtual tissue mesh file name",
                                 fileNameGeometryOutputMesh,
                                 true );
    application.addSingleOption( "-geometryOutputEnergyPlot",
                                 "Output geometry total repulsion force norm "
                                 "energy plot file name",
                                 fileNameGeometryOutputEnergyPlot,
                                 true );
    application.addSingleOption( "-maximumForceNorm",
                                 "Maximum force norm (default=0.25)",
                                 maximumForceNorm,
                                 true );
    application.addSingleOption( "-atomSolverMaximumIterationCount",
                                 "Atom deoverlap solver maximum iteration "
                                 "count (default=500)",
                                 atomSolverMaximumIterationCount,
                                 true );
    application.addSingleOption( "-repulsionForceMovingAverageWindowSize",
                                 "Repulsion force moving average window size "
                                 "(default=10)",
                                 repulsionForceMovingAverageWindowSize,
                                 true );
    application.addSingleOption( "-minimumRepulsionForceAttenuationPercentage",
                                 "Minimum repulsion force attenuation "
                                 "percentage (default=10.0)",
                                 minimumRepulsionForceAttenuationPercentage,
                                 true );

    application.addSingleOption( "-repulsionForceStddevPercentageThreshold",
                                 "Repulsion force stddev percentage threshold "
                                 "(default=1.0)",
                                 repulsionForceStddevPercentageThreshold,
                                 true );
    application.addSingleOption( "-log",
                                 "Log file name",
                                 fileNameLog,
                                 true );
    application.addSingleOption( "-applyKokkosInitialize",
                                 "Applying Kokkos::initialize() method"
                                 " (default=true)",
                                 verbose, 
                                 true );
    application.addSingleOption( "-applyKokkosFinalize",
                                 "Applying Kokkos::finalize() method"
                                 " (default=true)",
                                 verbose, 
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();



    execute( fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             maximumForceNorm,
             atomSolverMaximumIterationCount,
             repulsionForceMovingAverageWindowSize,
             minimumRepulsionForceAttenuationPercentage,
             repulsionForceStddevPercentageThreshold,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::MedusaGeometryDeoverlaperCommand::parse()" );

}


void gkg::MedusaGeometryDeoverlaperCommand::execute(
                        const std::string& fileNameGeometryInputMedusaGeometry,
                        const std::string& fileNameGeometryOutputMedusaGeometry,
                        const std::string& fileNameGeometryOutputMesh,
                        const std::string& fileNameGeometryOutputEnergyPlot,
                        float maximumForceNorm,
                        int32_t atomSolverMaximumIterationCount,
                        int32_t repulsionForceMovingAverageWindowSize,
                        float minimumRepulsionForceAttenuationPercentage,
                        float repulsionForceStddevPercentageThreshold,
                        const std::string& fileNameLog,
                        bool applyKokkosInitialize,
                        bool applyKokkosFinalize,
                        bool verbose )
{

  try
  {


    //
    // initializing Kokkos if wanted
    //

    if ( applyKokkosInitialize )
    {

      Kokkos::initialize();

    }


    //
    // creating log file if wanted
    //
    std::ofstream* osLog = 0;
    if ( !fileNameLog.empty() )
    {

      osLog = new std::ofstream( fileNameLog.c_str() );

    }


    //
    // reading input MEDUSA geometry
    //

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << std::endl
               << "================================================="
               << std::endl;
        *osLog << "reading MEDUSA geometry input file : " << std::flush;

      }
      else
      {

        std::cout << std::endl
                  << "================================================="
                  << std::endl;
        std::cout << "reading MEDUSA geometry input file : " << std::flush;

      }

    }
    gkg::RCPointer< gkg::VirtualTissue > virtualTissue(
                                                       new gkg::VirtualTissue );

    gkg::Reader::getInstance().read( fileNameGeometryInputMedusaGeometry,
                                     *virtualTissue );

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "done" << std::endl;

      }
      else
      {

        std::cout << "done" << std::endl;

      }

    }


    //
    // deoverlaping Medusa geometry
    //

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << std::endl
               << "================================================="
               << std::endl;
        *osLog << "deoverlaping virtual tissue : " << std::flush;

      }
      else
      {

        std::cout << std::endl
                  << "================================================="
                  << std::endl;
        std::cout << "deoverlaping virtual tissue : " << std::flush;

      }

    }

    virtualTissue->removeOverlaps(
                                  maximumForceNorm,
                                  atomSolverMaximumIterationCount,
                                  repulsionForceMovingAverageWindowSize,
                                  minimumRepulsionForceAttenuationPercentage,
                                  repulsionForceStddevPercentageThreshold,
                                  verbose,
                                  osLog );

    if ( verbose )
    {

      if ( osLog )
      {

        *osLog << "done" << std::endl;

      }
      else
      {

        std::cout << "done" << std::endl;

      }

    }

    //
    // saving the deoverlaped Medusa geometry
    //

    if ( !fileNameGeometryOutputMedusaGeometry.empty() )
    {

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << std::endl
                 << "================================================="
                 << std::endl;
          *osLog << "saving deoverlaped virtual tissue : " << std::flush;

        }
        else
        {

          std::cout << std::endl
                    << "================================================="
                    << std::endl;
          std::cout << "saving deoverlaped virtual tissue : " << std::flush;

        }

      }

      gkg::Writer::getInstance().write(
                                       fileNameGeometryOutputMedusaGeometry,
                                       *virtualTissue );

      if ( verbose )
      {

        if ( osLog )
        {

          *osLog << "done" << std::endl;

        }
        else
        {

          std::cout << "done" << std::endl;

        }

      }

    }


    int32_t maximumAtomCountExceeded = 0;
    if ( virtualTissue->getHeader().hasAttribute(  
                                             "maximum_atom_count_exceeded" ) )
    {

      virtualTissue->getHeader().getAttribute(  "maximum_atom_count_exceeded",
                                                maximumAtomCountExceeded );

    }

    if ( !maximumAtomCountExceeded )
    {

      //
      // creating and saving geometry output mesh on demand
      //
      if ( !fileNameGeometryOutputMesh.empty() )
      {

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << std::endl
                   << "================================================="
                   << std::endl;
            *osLog << "creating virtual tissue mesh : " << std::flush;

          }
          else
          {

            std::cout << std::endl
                      << "================================================="
                      << std::endl;
            std::cout << "creating virtual tissue mesh : " << std::flush;

          }

        }

        std::vector< gkg::MeshMap< int32_t, float, 3U > >
          geometryOutputMeshMaps( 2U );
        virtualTissue->getMeshMaps( geometryOutputMeshMaps );

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "done" << std::endl;

          }
          else
          {

            std::cout << "done" << std::endl;

          }

        }

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << std::endl
                   << "================================================="
                   << std::endl;
            *osLog << "writing virtual tissue mesh : " << std::flush;

          }
          else
          {

            std::cout << std::endl
                      << "================================================="
                      << std::endl;
            std::cout << "writing virtual tissue mesh : " << std::flush;

          }

        }

        int32_t meshMapCount = ( int32_t )geometryOutputMeshMaps.size();
        int32_t meshMapIndex = 0;
        for ( meshMapIndex = 0; meshMapIndex < meshMapCount; meshMapIndex++ )
        {

          gkg::Writer::getInstance().write( 
            fileNameGeometryOutputMesh + "_" +
            gkg::StringConverter::toString( meshMapIndex + 1 ),
            geometryOutputMeshMaps[ meshMapIndex ],
            false,
            "aimsmesh" );

        }

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << "done" << std::endl;

          }
          else
          {

            std::cout << "done" << std::endl;

          }

        }

      }


      //
      // creating and saving geometry output energy plot on demand
      //
      if ( !fileNameGeometryOutputEnergyPlot.empty() )
      {

        const std::list< float >& totalRepulsionForceNorms =
          virtualTissue->getTotalRepulsionForceNorms();

        int32_t iteration = 0;
        std::ofstream os( fileNameGeometryOutputEnergyPlot.c_str() );
      
        std::list< float >::const_iterator
          f = totalRepulsionForceNorms.begin(),
          fe = totalRepulsionForceNorms.end();
        while ( f != fe )
        {
      
          os << iteration << " " << *f << std::endl;
          ++ iteration;
          ++ f;
      
        }
        os.close();

      }

    }
    

    //
    // closing log file
    //

    if ( !fileNameLog.empty() )
    {

      osLog->close();
      delete osLog;

    }

    //
    // finalizing Kokkos if wanted
    //

    if ( applyKokkosFinalize )
    {

      Kokkos::finalize();

    }

  }
  GKG_CATCH( "void gkg::MedusaGeometryDeoverlaperCommand::execute( "
             "const std::string& fileNameGeometryInputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMesh, "
             "const std::string& fileNameGeometryOutputEnergyPlot, "
             "float maximumForceNorm, "
             "int32_t atomSolverMaximumIterationCount, "
             "int32_t repulsionForceMovingAverageWindowSize, "
             "float minimumRepulsionForceAttenuationPercentage, "
             "float repulsionForceStddevPercentageThreshold, "
             "const std::string& fileNameLog, "
             "bool applyKokkosInitialize, "
             "bool applyKokkosFinalize, "
             "bool verbose )" );

}


RegisterCommandCreator(
         MedusaGeometryDeoverlaperCommand,
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryInputMedusaGeometry ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputMedusaGeometry ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputMesh ) +
         DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputEnergyPlot ) +
         DECLARE_FLOATING_PARAMETER_HELP( maximumForceNorm ) +
         DECLARE_INTEGER_PARAMETER_HELP( atomSolverMaximumIterationCount ) +
         DECLARE_INTEGER_PARAMETER_HELP(
                                       repulsionForceMovingAverageWindowSize ) +
         DECLARE_FLOATING_PARAMETER_HELP(  
                                  minimumRepulsionForceAttenuationPercentage ) +
         DECLARE_FLOATING_PARAMETER_HELP( 
                                     repulsionForceStddevPercentageThreshold ) +
         DECLARE_STRING_PARAMETER_HELP( fileNameLog ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( applyKokkosInitialize ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( applyKokkosFinalize ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

