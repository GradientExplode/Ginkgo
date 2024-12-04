#include <gkg-simulation-plugin-functors/Medusa/MedusaCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-cppext/UncommentCounterInputFileStream.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-object/TypedObject_i.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-simulation-virtual-tissue/VirtualTissue.h>
#include <gkg-simulation-diffusion-process/MonteCarloDiffusionProcess.h>
#include <gkg-simulation-mri-sequence/MRISequencePool.h>
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


gkg::MedusaCommand::MedusaCommand( int32_t argc,
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
  GKG_CATCH( "gkg::MedusaCommand::"
             "MedusaCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::MedusaCommand::MedusaCommand(
                const std::string& fileNameGeometryInputDictionary,
                const std::string& fileNameGeometryInputMedusaGeometry,
                const std::string& fileNameGeometryOutputMedusaGeometry,
                const std::string& fileNameGeometryOutputMesh,
                const std::string& fileNameGeometryOutputEnergyPlot,
                const std::string& fileNameTissuePropertyInputDictionary,
                const std::string& 
                      fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
                const std::string& fileNameDiffusionProcessInputDictionary,
                const std::string& fileNameDiffusionProcessOutputParticles,
                const std::string& fileNameDiffusionProcessOutputTrajectories,
                const std::string& fileNameMRISequenceInputDictionary,
                const std::string&
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                const std::string& fileNameMRISequenceOutputPythonAttenuation,
                const std::string& fileNameMRISequenceOutputVolumeAttenuation,
                const std::string& outputVolumeFormat,
                bool outputVolumeAscii,
                const std::string& fileNameLog,
                bool applyKokkosInitialize,
                bool applyKokkosFinalize,
                bool verbose )
{

  try
  {

    execute( fileNameGeometryInputDictionary,
             fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             fileNameTissuePropertyInputDictionary,
             fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
             fileNameDiffusionProcessInputDictionary,
             fileNameDiffusionProcessOutputParticles,
             fileNameDiffusionProcessOutputTrajectories,
             fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             fileNameMRISequenceOutputPythonAttenuation,
             fileNameMRISequenceOutputVolumeAttenuation,
             outputVolumeFormat,
             outputVolumeAscii,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaCommand::MedusaCommand( "
             "const std::string& fileNameGeometryInputDictionary, "
             "const std::string& fileNameGeometryInputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMesh, "
             "const std::string& fileNameGeometryOutputEnergyPlot, "
             "const std::string& fileNameTissuePropertyInputDictionary, "
             "const std::string& "
             "fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile, "
             "const std::string& fileNameDiffusionProcessInputDictionary, "
             "const std::string& fileNameDiffusionProcessOutputParticles, "
             "const std::string& fileNameDiffusionProcessOutputTrajectories, "
             "const std::string& fileNameMRISequenceInputDictionary, "
             "const std::string& "
             "directoryNameMRISequenceOutputNormalizedGradientProfiles, "
             "const std::string& fileNameMRISequenceOutputPythonAttenuation, "
             "const std::string& fileNameMRISequenceOutputVolumeAttenuation, "
             "const std::string& outputVolumeFormat, "
             "bool outputVolumeAscii, "
             "const std::string& fileNameLog, "
             "bool applyKokkosInitialize, "
             "bool applyKokkosFinalize, "
             "bool verbose )" );

}


gkg::MedusaCommand::MedusaCommand( const gkg::Dictionary& parameters )
                   : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryInputDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryInputMedusaGeometry );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputMedusaGeometry );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputMesh );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameGeometryOutputEnergyPlot );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameTissuePropertyInputDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                    fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDiffusionProcessInputDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDiffusionProcessOutputParticles );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameDiffusionProcessOutputTrajectories );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMRISequenceInputDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                     directoryNameMRISequenceOutputNormalizedGradientProfiles );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMRISequenceOutputPythonAttenuation );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMRISequenceOutputVolumeAttenuation );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              outputVolumeFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, outputVolumeAscii );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameLog );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyKokkosInitialize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, applyKokkosFinalize );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameGeometryInputDictionary,
             fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             fileNameTissuePropertyInputDictionary,
             fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
             fileNameDiffusionProcessInputDictionary,
             fileNameDiffusionProcessOutputParticles,
             fileNameDiffusionProcessOutputTrajectories,
             fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             fileNameMRISequenceOutputPythonAttenuation,
             fileNameMRISequenceOutputVolumeAttenuation,
             outputVolumeFormat,
             outputVolumeAscii,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH( "gkg::MedusaCommand::"
             "MedusaCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::MedusaCommand::~MedusaCommand()
{
}


std::string gkg::MedusaCommand::getStaticName()
{

  try
  {

    return "Medusa";

  }
  GKG_CATCH( "std::string "
             "gkg::MedusaCommand::"
             "getStaticName()" );

}


void gkg::MedusaCommand::parse()
{

  try
  {

    std::string fileNameGeometryInputDictionary;
    std::string fileNameGeometryInputMedusaGeometry;
    std::string fileNameGeometryOutputMedusaGeometry;
    std::string fileNameGeometryOutputMesh;
    std::string fileNameGeometryOutputEnergyPlot;
    std::string fileNameTissuePropertyInputDictionary;
    std::string fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile;
    std::string fileNameDiffusionProcessInputDictionary;
    std::string fileNameDiffusionProcessOutputParticles;
    std::string fileNameDiffusionProcessOutputTrajectories;
    std::string fileNameMRISequenceInputDictionary;
    std::string directoryNameMRISequenceOutputNormalizedGradientProfiles;
    std::string fileNameMRISequenceOutputPythonAttenuation;
    std::string fileNameMRISequenceOutputVolumeAttenuation;
    std::string outputVolumeFormat = "gis";
    bool outputVolumeAscii = false;
    std::string fileNameLog;
    bool applyKokkosInitialize = true;
    bool applyKokkosFinalize = true;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "The Ultimate MEDUSA simulation framework",
                                  _loadPlugin );
    application.addSingleOption( "-geometryInputDictionary",
                                 "Virtual tissue dictionary file name",
                                 fileNameGeometryInputDictionary,
                                 true );
    application.addSingleOption( "-geometryInputMedusaGeometry",
                                 "Virtual tissue dictionary file name",
                                 fileNameGeometryInputMedusaGeometry,
                                 true );
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
    application.addSingleOption( "-tissuePropertyInputDictionary",
                                 "Tissue property dictionary file name",
                                 fileNameTissuePropertyInputDictionary,
                                 true );
    application.addSingleOption( 
                      "-tissuePropertyOutputPopulationAndCellBasedCSVFile",
                      "Output population and cell-based tissue property "
                      "CSV file name",
                      fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
                      true );
    application.addSingleOption( "-diffusionProcessInputDictionary",
                                 "Monte-Carlo diffusion process dictionary "
                                 "file name",
                                 fileNameDiffusionProcessInputDictionary,
                                 true );
    application.addSingleOption( "-diffusionProcessOutputParticles",
                                 "Output initial particle positions point "
                                 "cloud file name",
                                 fileNameDiffusionProcessOutputParticles,
                                 true );
    application.addSingleOption( "-diffusionProcessOutputTrajectories",
                                 "Output particle trajectories file name"
                                 " (saved as a bundle map)",
                                 fileNameDiffusionProcessOutputTrajectories,
                                 true );
    application.addSingleOption( "-mriSequenceInputDictionary",
                                 "MRI sequence(s) dictionary "
                                 "file name",
                                 fileNameMRISequenceInputDictionary,
                                 true );
    application.addSingleOption(
                       "-mriSequenceOutputNormalizedGradientProfileDirectory",
                       "MRI sequence(s) directory name where to store "
                       "normalized gradient profiles (empty=not stored)",
                       directoryNameMRISequenceOutputNormalizedGradientProfiles,
                       true );
    application.addSingleOption( "-mriSequenceOutputPythonAttenuation",
                                 "Output diffusion attenuation python "
                                 "file name",
                                 fileNameMRISequenceOutputPythonAttenuation,
                                 true );
    application.addSingleOption( "-mriSequenceOutputVolumeAttenuation",
                                 "Output diffusion attenuation volume "
                                 "file name",
                                 fileNameMRISequenceOutputVolumeAttenuation,
                                 true );
    application.addSingleOption( "-outputVolumeFormat",
                                 "Putput volume format (default=gis)",
                                 outputVolumeFormat,
                                 true );
    application.addSingleOption( "-outputVolumeAscii",
                                 "Save output volume in ascii format"
                                 " ( default=false)",
                                 outputVolumeAscii, 
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



    execute( fileNameGeometryInputDictionary,
             fileNameGeometryInputMedusaGeometry,
             fileNameGeometryOutputMedusaGeometry,
             fileNameGeometryOutputMesh,
             fileNameGeometryOutputEnergyPlot,
             fileNameTissuePropertyInputDictionary,
             fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
             fileNameDiffusionProcessInputDictionary,
             fileNameDiffusionProcessOutputParticles,
             fileNameDiffusionProcessOutputTrajectories,
             fileNameMRISequenceInputDictionary,
             directoryNameMRISequenceOutputNormalizedGradientProfiles,
             fileNameMRISequenceOutputPythonAttenuation,
             fileNameMRISequenceOutputVolumeAttenuation,
             outputVolumeFormat,
             outputVolumeAscii,
             fileNameLog,
             applyKokkosInitialize,
             applyKokkosFinalize,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void "
                     "gkg::MedusaCommand::parse()" );

}


void gkg::MedusaCommand::execute(
  const std::string& fileNameGeometryInputDictionary,
  const std::string& fileNameGeometryInputMedusaGeometry,
  const std::string& fileNameGeometryOutputMedusaGeometry,
  const std::string& fileNameGeometryOutputMesh,
  const std::string& fileNameGeometryOutputEnergyPlot,
  const std::string& fileNameTissuePropertyInputDictionary,
  const std::string& fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile,
  const std::string& fileNameDiffusionProcessInputDictionary,
  const std::string& fileNameDiffusionProcessOutputParticles,
  const std::string& fileNameDiffusionProcessOutputTrajectories,
  const std::string& fileNameMRISequenceInputDictionary,
  const std::string& directoryNameMRISequenceOutputNormalizedGradientProfiles,
  const std::string& fileNameMRISequenceOutputPythonAttenuation,
  const std::string& fileNameMRISequenceOutputVolumeAttenuation,
  const std::string& outputVolumeFormat,
  bool outputVolumeAscii,
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
    // sanity check(s)
    //
    if ( fileNameGeometryInputDictionary.empty() &&
         fileNameGeometryInputMedusaGeometry.empty() )
    {

      throw std::runtime_error( "at least one input geometry dictionary or "
                                "one medusa geometry is required" );

    }

    if ( !fileNameGeometryInputDictionary.empty() &&
         !fileNameGeometryInputMedusaGeometry.empty() )
    {

      throw std::runtime_error( 
               "a single input dictionary or medusa geometry file is allowed" );

    }
    if ( !fileNameGeometryInputMedusaGeometry.empty() &&
         !fileNameGeometryOutputEnergyPlot.empty() )
    {

      throw std::runtime_error(
          "a medusa geometry file is set as input which is compatible with "
          "prescribing an output geometry energy plot" );

    }

    if ( !fileNameGeometryInputMedusaGeometry.empty() &&
         !fileNameGeometryOutputMedusaGeometry.empty() )
    {

      throw std::runtime_error(
          "a medusa geometry file is set as input which is compatible with "
          "prescribing an output medusa geometry file" );

    }

    gkg::RCPointer< gkg::VirtualTissue > virtualTissue;
    if ( !fileNameGeometryInputDictionary.empty() ||
         !fileNameGeometryInputMedusaGeometry.empty() )
    {


      std::ofstream* osLog = 0;
      if ( !fileNameLog.empty() )
      {

        osLog = new std::ofstream( fileNameLog.c_str() );

      }

      //
      // reading geometry input dictionary
      //
      if ( !fileNameGeometryInputDictionary.empty() )
      {

        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << std::endl
                   << "================================================="
                   << std::endl;
            *osLog << "reading geometry input dictionary : " << std::flush;

          }
          else
          {

            std::cout << std::endl
                      << "================================================="
                      << std::endl;
            std::cout << "reading geometry input dictionary : " << std::flush;

          }

        }


        gkg::UncommentCounterInputFileStream
          geometryUcis( fileNameGeometryInputDictionary.c_str() );
        gkg::BaseObjectReader geometryBaseObjectReader;
        gkg::TypedObject< gkg::Dictionary > geometryDictionary;
        geometryBaseObjectReader.read( geometryUcis, geometryDictionary );
        geometryUcis.close();

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
        // reading tissue property input dictionary if provided
        //
        gkg::RCPointer< gkg::Dictionary > populationTissuePropertyDictionary;
        if ( !fileNameTissuePropertyInputDictionary.empty() )
        {

          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << std::endl
                     << "================================================="
                     << std::endl;
              *osLog << "reading tissue property input dictionary : "
                     << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "reading tissue property  input dictionary : "
                        << std::flush;

            }

          }


          gkg::UncommentCounterInputFileStream
            tissuePropertyUcis( fileNameTissuePropertyInputDictionary.c_str() );
          gkg::BaseObjectReader tissuePropertyBaseObjectReader;
          gkg::TypedObject< gkg::Dictionary > tissuePropertyDictionary;
          tissuePropertyBaseObjectReader.read( tissuePropertyUcis,
                                               tissuePropertyDictionary );
          tissuePropertyUcis.close();

          populationTissuePropertyDictionary.reset( new gkg::Dictionary );
          *populationTissuePropertyDictionary =
                                       tissuePropertyDictionary.getTypedValue();

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
        // creating virtual tissue
        //
        if ( verbose )
        {

          if ( osLog )
          {

            *osLog << std::endl
                   << "================================================="
                   << std::endl;
            *osLog << "creating virtual tissue : " << std::endl;

          }
          else
          {

            std::cout << std::endl
                      << "================================================="
                      << std::endl;
            std::cout << "creating virtual tissue : " << std::endl;

          }

        }

        virtualTissue.reset(
                   new gkg::VirtualTissue( geometryDictionary.getTypedValue(),
                                           populationTissuePropertyDictionary,
                                           verbose,
                                           osLog ) );

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
        // saving the raw Medusa geometry
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
              *osLog << "saving virtual tissue : " << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "saving virtual tissue : " << std::flush;

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

      }
      else
      {

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
        virtualTissue.reset( new gkg::VirtualTissue );

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
        // reading tissue property input dictionary if provided
        //
        gkg::RCPointer< gkg::Dictionary > populationTissuePropertyDictionary;
        if ( !fileNameTissuePropertyInputDictionary.empty() )
        {

          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << std::endl
                     << "================================================="
                     << std::endl;
              *osLog << "reading tissue property input dictionary : "
                     << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "reading tissue property  input dictionary : "
                        << std::flush;

            }

          }


          gkg::UncommentCounterInputFileStream
            tissuePropertyUcis( fileNameTissuePropertyInputDictionary.c_str() );
          gkg::BaseObjectReader tissuePropertyBaseObjectReader;
          gkg::TypedObject< gkg::Dictionary > tissuePropertyDictionary;
          tissuePropertyBaseObjectReader.read( tissuePropertyUcis,
                                               tissuePropertyDictionary );
          tissuePropertyUcis.close();

          populationTissuePropertyDictionary.reset( new gkg::Dictionary );
          *populationTissuePropertyDictionary =
                                       tissuePropertyDictionary.getTypedValue();

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

          virtualTissue->setPopulationTissueProperties(
                                           populationTissuePropertyDictionary );

        }

      }
      
      int32_t maximumAtomCountExceeded = 0;
      virtualTissue->getHeader().getAttribute(  "maximum_atom_count_exceeded",
                                                maximumAtomCountExceeded );

      if ( !maximumAtomCountExceeded )
      {

        //
        // creating and saving tissue property output population and cell-based
        // CSV file
        //

        if ( !fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile.empty())
        {

          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << std::endl
                     << "================================================="
                     << std::endl;
              *osLog << "creating tissue property population and cell-based "
                     << "CSV file : " << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "creating tissue property population and cell-based "
                        << "CSV file : " << std::flush;

            }

          }

          virtualTissue->getTissuePropertyOutputPopulationAndCellBasedCSVFile(
                    fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile );

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
        if ( !fileNameGeometryInputDictionary.empty() &&
             !fileNameGeometryOutputEnergyPlot.empty() )
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


        //
        // creating Monte-Carlo diffusion process on demand
        //
        if ( !fileNameDiffusionProcessInputDictionary.empty() )
        {


          //
          // reading diffusion process input dictionary
          //
          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << std::endl
                     << "================================================="
                     << std::endl;
              *osLog << "reading diffusion process input dictionary : "
                     << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "reading diffusion process input dictionary : "
                        << std::flush;

            }

          }

          gkg::UncommentCounterInputFileStream
            diffusionProcessUcis(
                                fileNameDiffusionProcessInputDictionary.c_str() );
          gkg::BaseObjectReader diffusionProcessBaseObjectReader;
          gkg::TypedObject< gkg::Dictionary > diffusionProcessDictionary;
          diffusionProcessBaseObjectReader.read( diffusionProcessUcis,
                                                 diffusionProcessDictionary );
          diffusionProcessUcis.close();

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
          // creating Monte-Carlo diffusion process
          //
          if ( verbose )
          {

            if ( osLog )
            {

              *osLog << std::endl
                     << "================================================="
                     << std::endl;
              *osLog << "creating Monte-Carlo diffusion process : "
                     << std::flush;

            }
            else
            {

              std::cout << std::endl
                        << "================================================="
                        << std::endl;
              std::cout << "creating Monte-Carlo diffusion process : "
                        << std::flush;

            }

          }

          gkg::RCPointer< gkg::MonteCarloDiffusionProcess >
            monteCarloDiffusionProcess(
              new gkg::MonteCarloDiffusionProcess(
                                 virtualTissue,
                                 diffusionProcessDictionary.getTypedValue() ) );
  
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

            monteCarloDiffusionProcess->displayInformation( osLog );

          }



          //
          // creating MRI sequence pool of sequence(s) on demand
          //
          if ( !fileNameMRISequenceInputDictionary.empty() )
          {

            if ( verbose )
            {

              if ( osLog )
              {

                *osLog << std::endl
                       << "================================================="
                       << std::endl;
                *osLog << "reading MRI sequence input dictionary : "
                        << std::flush;

              }
              else
              {

                std::cout << std::endl
                          << "================================================="
                          << std::endl;
                std::cout << "reading MRI sequence input dictionary : "
                          << std::flush;

              }

            }
      
            gkg::UncommentCounterInputFileStream
              mriSequenceUcis( fileNameMRISequenceInputDictionary.c_str() );
            gkg::BaseObjectReader mriSequenceBaseObjectReader;
            gkg::TypedObject< gkg::Dictionary > mriSequenceDictionary;
            mriSequenceBaseObjectReader.read( mriSequenceUcis,
                                              mriSequenceDictionary );
            mriSequenceUcis.close();

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
            // creating MRI sequence pool
            //
            if ( verbose )
            {

              if ( osLog )
              {

                *osLog << std::endl
                       << "================================================="
                       << std::endl;
                *osLog << "creating MRI sequence pool : "
                       << std::endl;

              }
              else
              {

                std::cout << std::endl
                          << "================================================="
                          << std::endl;
                std::cout << "creating MRI sequence pool : "
                          << std::endl;

              }

            }

            gkg::RCPointer< gkg::MRISequencePool >
              mriSequencePool(
                new gkg::MRISequencePool(
                                  mriSequenceDictionary.getTypedValue(),
                                  monteCarloDiffusionProcess->getTimeStepInUs(),
                                  verbose, 
                                  osLog ) );

            if ( !directoryNameMRISequenceOutputNormalizedGradientProfiles.
                                                                       empty() )
            {

              mriSequencePool->saveNormalizedGradientProfiles(
                     directoryNameMRISequenceOutputNormalizedGradientProfiles );

            }


            //
            // writing diffusion signal attenuation volume on demand
            //
            if ( !fileNameMRISequenceOutputVolumeAttenuation.empty() )
            {

              if ( verbose )
              {

                if ( osLog )
                {

                  *osLog << std::endl
                         << "================================================="
                         << std::endl;
                  *osLog << "writing diffusion information file(s) : "
                         << std::flush;

                }
                else
                {

                  std::cout << std::endl
                          << "================================================="
                          << std::endl;
                  std::cout << "writing diffusion information file(s) "
                            << "volume(s) : "
                            << std::flush;

                }

              }

              int32_t mriSequenceCount = mriSequencePool->getCount();
              int32_t mriSequenceIndex = 0;
              for ( mriSequenceIndex = 0;
                    mriSequenceIndex < mriSequenceCount;
                    mriSequenceIndex++ )
              {

                // saving further diffusion information file(s) such as
                // *.bval / *.bvec file(s)
                const gkg::RCPointer< gkg::MRISequence >&
                  mriSequence = mriSequencePool->getMRISequence(
                                                             mriSequenceIndex );
                mriSequence->saveDiffusionInformation(
                       fileNameMRISequenceOutputVolumeAttenuation + "_" +
                       mriSequencePool->getMRISequenceName( mriSequenceIndex ) +
                       "_dw" );


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
            // running the Monte-Carlo simulation
            //
            if ( verbose )
            {

              if ( osLog )
              {

                *osLog << std::endl
                       << "================================================="
                       << std::endl;
                *osLog << "running Monte-Carlo + MRI simulation : "
                       << std::endl;

              }
              else
              {

                std::cout << std::endl
                          << "================================================="
                          << std::endl;
                std::cout << "running Monte-Carlo + MRI simulation : "
                          << std::endl;

              }

            }

            std::vector< std::vector< float > > 
              perSequenceDiffusionWeightedAttenuations;
            monteCarloDiffusionProcess->simulate(
                                    mriSequencePool,
                                    fileNameDiffusionProcessOutputParticles,
                                    fileNameDiffusionProcessOutputTrajectories,
                                    perSequenceDiffusionWeightedAttenuations,
                                    verbose,
                                    osLog );

            if ( verbose )
            {

              if ( osLog )
              {

                *osLog << std::endl;

              }
              else
              {

                std::cout << std::endl;

              }

            }


            //
            // writing diffusion signal attenuation volume on demand
            //
            if ( !fileNameMRISequenceOutputVolumeAttenuation.empty() )
            {

              if ( verbose )
              {

                if ( osLog )
                {

                  *osLog << std::endl
                         << "================================================="
                         << std::endl;
                  *osLog << "writing diffusion signal attenuation volume(s) : "
                         << std::flush;

                }
                else
                {

                  std::cout << std::endl
                          << "================================================="
                          << std::endl;
                  std::cout << "writing diffusion signal attenuation "
                            << "volume(s) : "
                            << std::flush;

                }

              }

              int32_t mriSequenceCount = mriSequencePool->getCount();
              int32_t mriSequenceIndex = 0;
              for ( mriSequenceIndex = 0;
                    mriSequenceIndex < mriSequenceCount;
                    mriSequenceIndex++ )
              {

                 gkg::Vector3d< double > resolution(
                       ( double )virtualTissue->getFieldOfView().getSpanX(),
                       ( double )virtualTissue->getFieldOfView().getSpanY(),
                       ( double )virtualTissue->getFieldOfView().getSpanZ() );

                gkg::Volume< float > 
                  volumeDiffusionSignalAttenuation(
                    1, 1, 1, 
                    ( int32_t )perSequenceDiffusionWeightedAttenuations[
                                                   mriSequenceIndex ].size() );

               volumeDiffusionSignalAttenuation.setResolution( resolution );
                std::vector< float >::const_iterator
                  a = perSequenceDiffusionWeightedAttenuations[
                                                     mriSequenceIndex ].begin(),
                  ae = perSequenceDiffusionWeightedAttenuations[
                                                     mriSequenceIndex ].end();
                gkg::Volume< float >::iterator
                  va = volumeDiffusionSignalAttenuation.begin();
                while ( a != ae )
                {

                  *va = *a;
                  ++ a;
                  ++ va;

                }

                gkg::Writer::getInstance().write( 
                  fileNameMRISequenceOutputVolumeAttenuation + "_" +
                  mriSequencePool->getMRISequenceName( mriSequenceIndex ) + 
                  "_dw",
                  volumeDiffusionSignalAttenuation,
                  outputVolumeAscii,
                  outputVolumeFormat );

                gkg::Volume< float > volumeT2Signal( 1, 1, 1 );
                volumeT2Signal.setResolution( resolution );
                volumeT2Signal( 0, 0, 0 ) = 1.0f;

                gkg::Writer::getInstance().write( 
                  fileNameMRISequenceOutputVolumeAttenuation + "_" +
                  mriSequencePool->getMRISequenceName( mriSequenceIndex ) + 
                  "_t2",
                  volumeT2Signal,
                  outputVolumeAscii,
                  outputVolumeFormat );

                gkg::Volume< int16_t > volumeMask( 1, 1, 1 );
                volumeMask.setResolution( resolution );
                volumeMask( 0, 0, 0 ) = 1;

                gkg::Writer::getInstance().write( 
                  fileNameMRISequenceOutputVolumeAttenuation + "_" +
                  mriSequencePool->getMRISequenceName( mriSequenceIndex ) + 
                  "_mask",
                  volumeMask,
                  outputVolumeAscii,
                  outputVolumeFormat );

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
            // writing diffusion signal attenuation python file on demand
            //
            if ( !fileNameMRISequenceOutputPythonAttenuation.empty() )
            {

              if ( verbose )
              {

                if ( osLog )
                {

                  *osLog << std::endl
                         << "================================================="
                         << std::endl;
                  *osLog << "writing diffusion signal attenuation "
                         << "python file(s) : "
                         << std::flush;

                }
                else
                {

                  std::cout << std::endl
                          << "================================================="
                          << std::endl;
                  std::cout << "writing diffusion signal attenuation "
                            << "python file(s) : "
                            << std::flush;

                }

              }

              int32_t mriSequenceCount = mriSequencePool->getCount();
              int32_t mriSequenceIndex = 0;
              for ( mriSequenceIndex = 0;
                    mriSequenceIndex < mriSequenceCount;
                    mriSequenceIndex++ )
              {

                std::ofstream
                  os( ( fileNameMRISequenceOutputPythonAttenuation +
                        mriSequencePool->getMRISequenceName(
                                                mriSequenceIndex ) ).c_str() );

                os << "diffusion_weighted_attenuations = [ " << std::flush;

                std::vector< float >::const_iterator
                  a = perSequenceDiffusionWeightedAttenuations[
                                                     mriSequenceIndex ].begin(),
                  ae = perSequenceDiffusionWeightedAttenuations[
                                                     mriSequenceIndex ].end();
                while ( a != ae )
                {

                  os << *a << ", ";
                  ++ a;

                }

                os << "]" << std::endl;

                os.close();

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

          }

        }

      }
      
      if ( !fileNameLog.empty() )
      {

        osLog->close();
        delete osLog;

      }
      
    }

    //
    // finalizing Kokkos if wanted
    //

    if ( applyKokkosFinalize )
    {

      Kokkos::finalize();

    }

  }
  GKG_CATCH( "void gkg::MedusaCommand::execute( "
             "const std::string& fileNameGeometryInputDictionary, "
             "const std::string& fileNameGeometryInputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMedusaGeometry, "
             "const std::string& fileNameGeometryOutputMesh, "
             "const std::string& fileNameGeometryOutputEnergyPlot, "
             "const std::string& fileNameTissuePropertyInputDictionary, "
             "const std::string& "
             "fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile, "
             "const std::string& fileNameDiffusionProcessInputDictionary, "
             "const std::string& fileNameDiffusionProcessOutputParticles, "
             "const std::string& fileNameDiffusionProcessOutputTrajectories, "
             "const std::string& fileNameMRISequenceInputDictionary, "
             "const std::string& "
             "directoryNameMRISequenceOutputNormalizedGradientProfiles, "
             "const std::string& fileNameMRISequenceOutputPythonAttenuation, "
             "const std::string& fileNameMRISequenceOutputVolumeAttenuation, "
             "const std::string& outputVolumeFormat, "
             "bool outputVolumeAscii, "
             "const std::string& fileNameLog, "
             "bool applyKokkosInitialize, "
             "bool applyKokkosFinalize, "
             "bool verbose )" );

}


RegisterCommandCreator(
         MedusaCommand,
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryInputDictionary ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryInputMedusaGeometry ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputMedusaGeometry ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputMesh ) +
         DECLARE_STRING_PARAMETER_HELP( fileNameGeometryOutputEnergyPlot ) +
         DECLARE_STRING_PARAMETER_HELP( 
                                       fileNameTissuePropertyInputDictionary ) +
         DECLARE_STRING_PARAMETER_HELP(
                   fileNameTissuePropertyOutputPopulationAndCellBasedCSVFile ) +
    	 DECLARE_STRING_PARAMETER_HELP( 
                                 fileNameDiffusionProcessInputDictionary ) +
    	 DECLARE_STRING_PARAMETER_HELP( 
                                 fileNameDiffusionProcessOutputParticles ) +
    	 DECLARE_STRING_PARAMETER_HELP( 
                              fileNameDiffusionProcessOutputTrajectories ) +
    	 DECLARE_STRING_PARAMETER_HELP(
                                      fileNameMRISequenceInputDictionary ) +
    	 DECLARE_STRING_PARAMETER_HELP(
                directoryNameMRISequenceOutputNormalizedGradientProfiles ) +
    	 DECLARE_STRING_PARAMETER_HELP(
                              fileNameMRISequenceOutputPythonAttenuation ) +
    	 DECLARE_STRING_PARAMETER_HELP(
                              fileNameMRISequenceOutputVolumeAttenuation ) +
    	 DECLARE_STRING_PARAMETER_HELP( outputVolumeFormat ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( outputVolumeAscii ) +
    	 DECLARE_STRING_PARAMETER_HELP( fileNameLog ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( applyKokkosInitialize ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( applyKokkosFinalize ) +
    	 DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );

