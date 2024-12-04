#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperCommand.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperLoopContext.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/SingleCompartmentRelaxometryMapperGauge.h>
#include <gkg-qmri-plugin-functors/SingleCompartmentRelaxometryMapper/AcquisitionParameterSetFactory.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-numericalanalysis/OptimizerConstraint.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-communication-thread/ThreadedLoop.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// class SingleCompartmentRelaxometryMapperCommand
////////////////////////////////////////////////////////////////////////////////

gkg::SingleCompartmentRelaxometryMapperCommand::
                                      SingleCompartmentRelaxometryMapperCommand(
                                                              int32_t argc, 
                                                              char* argv[], 
                                                              bool loadPlugin, 
                                                              bool removeFirst )
                                               : gkg::Command( argc,
                                                               argv,
                                                               loadPlugin,
                                                               removeFirst )

{
  
  try
  {
  
    parse();
    
  }
  GKG_CATCH( "gkg::SingleCompartmentRelaxometryMapperCommand::"
             "SingleCompartmentRelaxometryMapperCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::SingleCompartmentRelaxometryMapperCommand::
                                      SingleCompartmentRelaxometryMapperCommand( 
                    const std::vector< std::string >& fileNameInputVolumes,
                    const std::string& fileNameMask,
                    const std::string& qMriMappingType,
                    const std::vector< double >& optimizerParameters,
                    const std::vector< double >& scalarParameters,
                    const std::vector< std::string >& stringParameters,
                    const std::string& fileNameOutputProtonDensityVolume,
                    const std::string& fileNameOutputRelaxationTimeVolume,
                    const std::string& fileNameOutputScalingVolume,
                    const std::string& fileNameFittedMeasurementVolume,
                    bool ascii,
                    const std::string& format,
                    bool verbose )
                                               : gkg::Command()
{
  
  try
  {
    
    execute( fileNameInputVolumes,
             fileNameMask,
             qMriMappingType,
             optimizerParameters,
             scalarParameters,
             stringParameters,
             fileNameOutputProtonDensityVolume,
             fileNameOutputRelaxationTimeVolume,
             fileNameOutputScalingVolume,
             fileNameFittedMeasurementVolume,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH( "gkg::SingleCompartmentRelaxometryMapperCommand::" 
             "SingleCompartmentRelaxometryMapperCommand( "
             "const std::vector< std::string >& fileNameInputVolumes, "
             "const std::string& fileNameMask, "
             "const std::string& qMriMappingType, "
             "const std::vector< double >& optimizerParameters, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters, "
             "const std::string& fileNameOutputProtonDensityVolume, "
             "const std::string& fileNameOutputRelaxationTimeVolume, "
             "const std::string& fileNameOutputScalingVolume, "
             "const std::string& fileNameFittedMeasurementVolume, "
             "bool ascii, "
             "const std::string& format, "
             "bool verbose )" );

}


gkg::SingleCompartmentRelaxometryMapperCommand::
                                      SingleCompartmentRelaxometryMapperCommand(
                                             const gkg::Dictionary& parameters )
                                         : gkg::Command( parameters )
{

  try
  {
    
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameInputVolumes );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, qMriMappingType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           optimizerParameters );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           scalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         stringParameters );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameOutputProtonDensityVolume );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameOutputRelaxationTimeVolume );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameOutputScalingVolume );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameFittedMeasurementVolume );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );

    execute( fileNameInputVolumes,
             fileNameMask,
             qMriMappingType,
             optimizerParameters,
             scalarParameters,
             stringParameters,
             fileNameOutputProtonDensityVolume,
             fileNameOutputRelaxationTimeVolume,
             fileNameOutputScalingVolume,
             fileNameFittedMeasurementVolume,
             ascii,
             format,
             verbose );
  
  }
  GKG_CATCH( "pkt::SingleCompartmentRelaxometryMapperCommand::"
             "SingleCompartmentRelaxometryMapperCommand( "
             "const pkt::Dictionary& parameters )" );
             
}


gkg::SingleCompartmentRelaxometryMapperCommand::
                                    ~SingleCompartmentRelaxometryMapperCommand()
{
}


std::string gkg::SingleCompartmentRelaxometryMapperCommand::getStaticName()
{

  try
  {
  
    return "SingleCompartmentRelaxometryMapper"; 
    
  }
  GKG_CATCH( "std::string "
             "gkg::SingleCompartmentRelaxometryMapperCommand::"
             "getStaticName()" );
             
}


void gkg::SingleCompartmentRelaxometryMapperCommand::parse()
{

  try
  {
    
    std::vector< std::string > fileNameInputVolumes;
    std::string fileNameMask;
    std::string qMriMappingType;
    std::vector< double > optimizerParameters;
    std::vector< double > scalarParameters;
    std::vector< std::string > stringParameters;
    std::string fileNameOutputProtonDensityVolume;
    std::string fileNameOutputRelaxationTimeVolume;
    std::string fileNameOutputScalingVolume;
    std::string fileNameFittedMeasurementVolume;
    bool ascii = false; 
    std::string format = ""; 
    bool verbose = false;
    
    gkg::Application application( _argc, _argv,
                                  "Single compartment relaxometry mapper",
                                   _loadPlugin );

    application.addSeriesOption( "-i",
                                 "Input qMRI volume file names",
                                 fileNameInputVolumes,
                                 0 );    
    application.addSingleOption( "-m",
                                 "Input int16_t mask file name",
                                 fileNameMask );    
    application.addSingleOption( "-t",
                                 "qMRI mapping type, one of: \n"
                                 "- t2-mapping-msme\n"
                                 "- t1-mapping-irse\n"
                                 "- t1-mapping-vfa-spgr\n"
                                 "- t2-star-mapping-mgre\n",
                                 qMriMappingType );


    application.addSeriesOption( "-optimizerParameters",
                                 "Parameters used during optimization:\n"
                                 ". <P1> : NLP maximum iteration count\n"
                                 ". <P2> : NLP test size\n"
                                 ". <P3> : 0->do not apply MCMC 1->apply MCMC\n"
                                 ". <P4> : MCMC burnin count\n"
                                 ". <P5> : MCMC sample count\n"
                                 ". <P6> : MCMC interval count\n"
                                 ". <P7> : MCMC maximum iteration count\n",
                                 optimizerParameters,
                                 7, 7 );

    application.addSeriesOption(
                            "-scalarParameters",
                            "According to the qMRI target,\n"
                            "- T2 mapping using MSME sequences :\n"
                            ". initial proton density \n"
                            ". initial quantitative T2 in ms \n"
                            ". initial scaling of 2nd input volume \n"
                            ". ... \n"
                            ". initial scaling of last input volume \n"
                            ". lower proton density \n"
                            ". lower quantitative T2 in ms \n"
                            ". lower scaling of 2nd input volume \n"
                            ". ... \n"
                            ". lower scaling of last input volume \n"
                            ". upper proton density \n"
                            ". upper quantitative T2 in ms \n"
                            ". upper scaling of 2nd input volume \n"
                            ". ... \n"
                            ". upper scaling of last input volume \n"
                            ". delta proton density \n"
                            ". delta quantitative T2 in ms \n"
                            ". delta scaling of 2nd input volume \n"
                            ". ... \n"
                            ". delta scaling of last input volume \n"
                            ". fixed scaling of 2nd input volume \n"
                            ". ... \n"
                            ". fixed scaling of last input volume \n"
                            ". noise standard deviation of 1st input volume\n"
                            ". ...\n"
                            ". noise standard deviation of last input volume\n"
                            "\n"
                            "- T1 mapping using IR-SE sequences :\n"
                            ". initial proton density \n"
                            ". initial quantitative T1 in ms \n"
                            ". initial scaling of 2nd input volume \n"
                            ". ... \n"
                            ". initial scaling of last input volume \n"
                            ". lower proton density \n"
                            ". lower quantitative T1 in ms \n"
                            ". lower scaling of 2nd input volume \n"
                            ". ... \n"
                            ". lower scaling of last input volume \n"
                            ". upper proton density \n"
                            ". upper quantitative T1 in ms \n"
                            ". upper scaling of 2nd input volume \n"
                            ". ... \n"
                            ". upper scaling of last input volume \n"
                            ". delta proton density \n"
                            ". delta quantitative T1 in ms \n"
                            ". delta scaling of 2nd input volume \n"
                            ". ... \n"
                            ". delta scaling of last input volume \n"
                            ". fixed scaling of 2nd input volume \n"
                            ". ... \n"
                            ". fixed scaling of last input volume \n"
                            ". noise standard deviation of 1st input volume\n"
                            ". ...\n"
                            ". noise standard deviation of last input volume\n"
                            "\n"
                            "- T1 mapping using VFA-SPGR sequences :\n"
                            ". initial proton density \n"
                            ". initial quantitative T1 in ms \n"
                            ". initial scaling of 2nd input volume \n"
                            ". ... \n"
                            ". initial scaling of last input volume \n"
                            ". lower proton density \n"
                            ". lower quantitative T1 in ms \n"
                            ". lower scaling of 2nd input volume \n"
                            ". ... \n"
                            ". lower scaling of last input volume \n"
                            ". upper proton density \n"
                            ". upper quantitative T1 in ms \n"
                            ". upper scaling of 2nd input volume \n"
                            ". ... \n"
                            ". upper scaling of last input volume \n"
                            ". delta proton density \n"
                            ". delta quantitative T1 in ms \n"
                            ". delta scaling of 2nd input volume \n"
                            ". ... \n"
                            ". delta scaling of last input volume \n"
                            ". fixed scaling of 2nd input volume \n"
                            ". ... \n"
                            ". fixed scaling of last input volume \n"
                            ". noise standard deviation of 1st input volume\n"
                            ". ...\n"
                            ". noise standard deviation of last input volume\n"
                            "\n"
                            "- T2* mapping using MGRE sequences :\n"
                            ". initial proton density \n"
                            ". initial quantitative T2* in ms \n"
                            ". initial scaling of 2nd input volume \n"
                            ". ... \n"
                            ". initial scaling of last input volume \n"
                            ". lower proton density \n"
                            ". lower quantitative T2* in ms \n"
                            ". lower scaling of 2nd input volume \n"
                            ". ... \n"
                            ". lower scaling of last input volume \n"
                            ". upper proton density \n"
                            ". upper quantitative T2* in ms \n"
                            ". upper scaling of 2nd input volume \n"
                            ". ... \n"
                            ". upper scaling of last input volume \n"
                            ". delta proton density \n"
                            ". delta quantitative T2* in ms \n"
                            ". delta scaling of 2nd input volume \n"
                            ". ... \n"
                            ". delta scaling of last input volume \n"
                            ". fixed scaling of 2nd input volume \n"
                            ". ... \n"
                            ". fixed scaling of last input volume \n"
                            ". noise standard deviation of 1st input volume\n"
                            ". ...\n"
                            ". noise standard deviation of last input volume\n",
                            scalarParameters,
                            9 );


    application.addSeriesOption(
                      "-stringParameters",
                      "According to the qMRI target,\n"
                      "- T2 mapping using MSME sequences :\n"
                      ". echo times text file name of 1st input volume\n"
                      ". ...\n"
                      ". echo times text file name of last input volume\n"
                      "\n"
                      "- T1 mapping using IR-SE sequences :\n"
                      ". inversion times text file name of 1st input volume\n"
                      ". ...\n"
                      ". inversion times text file name of last input volume\n"
                      "\n"
                      "- T1 mapping using VFA-SPGR sequences :\n"
                      ". repetition times text file name of 1st input volume\n"
                      ". ...\n"
                      ". repetition times text file name of last input volume\n"
                      ". flip angles text file name of 1st input volume\n"
                      ". ...\n"
                      ". flip angles text file name of last input volume\n"
                      ". B1 map file name (in the format given by Siemens XFL"
                      "  sequence i.e. neutral value is 900)\n"
                      "\n"
                      "- T2* mapping using MGRE sequences :\n"
                      ". echo times text file name of 1st input volume\n"
                      ". ...\n"
                      ". echo times text file name of last input volume\n",
                      stringParameters,
                      1 );

    application.addSingleOption( "-op", 
                                 "Output proton density volume file name", 
                                 fileNameOutputProtonDensityVolume,
                                 true );    
    application.addSingleOption( "-ot", 
                                 "Output relaxation time volume file name", 
                                 fileNameOutputRelaxationTimeVolume,
                                 true );    
    application.addSingleOption( "-os", 
                                 "Output scaling volume file name", 
                                 fileNameOutputScalingVolume,
                                 true );    
    application.addSingleOption( "-f", 
                                 "Output fitted measurement volume file name", 
                                 fileNameFittedMeasurementVolume, 
                                 true );    

    application.addSingleOption( "-ascii",
                                 "Save ouput volume in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameInputVolumes,
             fileNameMask,
             qMriMappingType,
             optimizerParameters,
             scalarParameters,
             stringParameters,
             fileNameOutputProtonDensityVolume,
             fileNameOutputRelaxationTimeVolume,
             fileNameOutputScalingVolume,
             fileNameFittedMeasurementVolume,
             ascii,
             format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::SingleCompartmentRelaxometryMapperCommand::"
                     "parse()" );

}


void gkg::SingleCompartmentRelaxometryMapperCommand::execute( 
                         const std::vector< std::string >& fileNameInputVolumes,
                         const std::string& fileNameMask,
                         const std::string& qMriMappingType,
                         const std::vector< double >& optimizerParameters,
                         const std::vector< double >& scalarParameters,
                         const std::vector< std::string >& stringParameters,
                         const std::string& fileNameOutputProtonDensityVolume,
                         const std::string& fileNameOutputRelaxationTimeVolume,
                         const std::string& fileNameOutputScalingVolume,
                         const std::string& fileNameFittedMeasurementVolume,
                         bool ascii,
                         const std::string& format,
                         bool verbose )
{

  try
  {
    
    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( ( qMriMappingType != "t2-mapping-msme" ) &&
         ( qMriMappingType != "t1-mapping-irse" ) &&
         ( qMriMappingType != "t1-mapping-vfa-spgr" ) &&
         ( qMriMappingType != "t2-star-mapping-mgre" ) )
    {

      throw std::runtime_error( "bad qMRI mapping type" );

    }
    if ( verbose )
    {

      std::cout << "qMRI mapping type : " << qMriMappingType << std::endl;

    }

    int32_t inputVolumeCount = ( int32_t )fileNameInputVolumes.size();


    ////////////////////////////////////////////////////////////////////////////
    // reading input volumes
    ////////////////////////////////////////////////////////////////////////////

    std::vector< gkg::Volume< float > > inputVolumes( inputVolumeCount );

    gkg::Vector3d< int32_t > inputVolumeSize; 
    gkg::Vector3d< double > inputVolumeResolution( 1.0, 1.0, 1.0 ); 
    double resolutionT = 1.0; 

    std::vector< int32_t > measurementCounts( inputVolumeCount );
    int32_t globalMeasurementCount = 0;
    
    int32_t i = 0;
    for ( i = 0; i < inputVolumeCount; i++ )
    {

      if ( verbose )
      {

        std::cout << "reading '" << fileNameInputVolumes[ i ] << "' : "
                  << std::flush;

      }

      gkg::TypedVolumeReaderProcess< float > 
        inputVolumeReaderProcess( inputVolumes[ i ] );
      inputVolumeReaderProcess.execute( fileNameInputVolumes[ i ] );

      if ( i > 0 )
      {

        if ( !inputVolumes[ i ].hasSameSize( inputVolumes[ 0 ], true ) ||
             !inputVolumes[ i ].hasSameResolution( inputVolumes[ 0 ], true ) )
        {

          throw std::runtime_error(
           "inconsistent size or resolution between input volume(s)" );

        }

      }

      if ( i == 0 )
      {

        inputVolumes[ 0 ].getSize( inputVolumeSize ); 
        inputVolumes[ 0 ].getResolution( inputVolumeResolution, resolutionT ); 

      }

      measurementCounts[ i ] = inputVolumes[ i ].getSizeT();
      globalMeasurementCount += measurementCounts[ i ];


      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    
    ////////////////////////////////////////////////////////////////////////////
    // reading mask 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {
    
      std::cout << "reading mask : " << std::flush; 
      
    }
    
    gkg::Volume< float > maskVolume; 
    gkg::TypedVolumeReaderProcess< float >
      maskVolumeReaderProcess( maskVolume ); 
    maskVolumeReaderProcess.execute( fileNameMask ); 
    if ( !maskVolume.hasSameSize( inputVolumes[ 0 ], true ) ||
         !maskVolume.hasSameResolution( inputVolumes[ 0 ], true ) )
    {

      throw std::runtime_error(
                "inconsistent size or resolution of mask and input volume(s)" );

    }

    
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing vector of sites in mask 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {
    
      std::cout << "counting sites in mask : " << std::flush; 
      
    }
    
    std::vector< gkg::Vector3d< int32_t > > sites;
    sites.reserve( inputVolumeSize.x * inputVolumeSize.y * inputVolumeSize.z );
    gkg::Vector3d< int32_t > site;
    for ( site.z = 0; site.z < inputVolumeSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < inputVolumeSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < inputVolumeSize.x; site.x++ )
        {

          if ( maskVolume( site ) > 0 )
          {

            sites.push_back( site );

          }

        }

      }

    }
    int32_t siteCount = ( int32_t )sites.size();

    if ( verbose )
    {

      std::cout << siteCount << " sites" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // allocating the adequate acquisition parameter set
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
      
      std::cout << "reading acquisition parameter(s) : " << std::flush;
      
    }

    gkg::RCPointer< gkg::AcquisitionParameterSet > 
      acquisitionParameterSet(
        gkg::AcquisitionParameterSetFactory::getInstance().create(
                                                          qMriMappingType,
                                                          measurementCounts,
                                                          stringParameters ) );

    if ( verbose )
    {
      
      std::cout << "done" << std::endl;
      
    }


    ////////////////////////////////////////////////////////////////////////////
    // creating threaded loop context and launching multithreaded loop
    ////////////////////////////////////////////////////////////////////////////
    
    if ( verbose )
    {
      
      std::cout << "computing quantitative map(s) : " << std::flush;
      
    }

    gkg::SingleCompartmentRelaxometryMapperGauge gauge( siteCount );


    gkg::Volume< float > protonDensityVolume( inputVolumeSize );
    protonDensityVolume.setResolution( inputVolumeResolution );
    protonDensityVolume.fill( 0.0f );

    gkg::Volume< float > relaxationTimeVolume( inputVolumeSize );
    relaxationTimeVolume.setResolution( inputVolumeResolution );
    relaxationTimeVolume.fill( 0.0f );

    gkg::Volume< float > scalingVolume;
    if ( inputVolumeCount > 1 )
    {

      scalingVolume.reallocate( inputVolumeSize, inputVolumeCount - 1 );
      scalingVolume.setResolution( inputVolumeResolution );
      scalingVolume.fill( 0.0f );

    }

    bool computeFittedMeasurements = false;
    gkg::Volume< float > fittedMeasurementVolume;

    if ( !fileNameFittedMeasurementVolume.empty() )
    {

      computeFittedMeasurements = true;
      fittedMeasurementVolume.reallocate( inputVolumeSize,
                                          globalMeasurementCount );
      fittedMeasurementVolume.setResolution( inputVolumeResolution,
                                             resolutionT );
      fittedMeasurementVolume.fill( 0.0f );

    }

    gkg::SingleCompartmentRelaxometryMapperLoopContext
      singleCompartmentRelaxometryMapperLoopContext(
        gauge,
        inputVolumes,
        sites,
        qMriMappingType,
        optimizerParameters,
        scalarParameters,
        acquisitionParameterSet,
        inputVolumeCount,
        measurementCounts,
        globalMeasurementCount,
        verbose,
        protonDensityVolume,
        relaxationTimeVolume,
        scalingVolume,
        computeFittedMeasurements,
        fittedMeasurementVolume );

    // creating a threaded loop
    gkg::ThreadedLoop< int32_t >
      threadedLoop( &singleCompartmentRelaxometryMapperLoopContext, 
                    0, 
                    siteCount );

    // launching the threaded loop
    threadedLoop.launch();
    
    if ( verbose )
    {
      
      std::cout << "done" << std::endl;
      
    }


    ////////////////////////////////////////////////////////////////////////////
    // saving proton density volume 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameOutputProtonDensityVolume.empty() )
    {
    
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameOutputProtonDensityVolume << "' : "
                  << std::flush; 
        
        }
        
      gkg::Writer::getInstance().write( fileNameOutputProtonDensityVolume, 
                                        protonDensityVolume, 
                                        ascii, 
                                        format ); 

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving relaxation time volume 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameOutputRelaxationTimeVolume.empty() )
    {
    
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameOutputRelaxationTimeVolume << "' : "
                  << std::flush; 
        
        }
        
      gkg::Writer::getInstance().write( fileNameOutputRelaxationTimeVolume, 
                                        relaxationTimeVolume, 
                                        ascii, 
                                        format ); 

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving scaling volume 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( ( inputVolumeCount > 1 ) && !fileNameOutputScalingVolume.empty() )
    {
    
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameOutputScalingVolume << "' : "
                  << std::flush; 
        
        }
        
      gkg::Writer::getInstance().write( fileNameOutputScalingVolume, 
                                        scalingVolume, 
                                        ascii, 
                                        format ); 

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }

    }
    
    
    ////////////////////////////////////////////////////////////////////////////
    // saving fitted measurement volume 
    ////////////////////////////////////////////////////////////////////////////
    
    if ( !fileNameFittedMeasurementVolume.empty() )
    {
    
      if ( verbose )
      {
      
        std::cout << "writing '" << fileNameFittedMeasurementVolume << "' : "
                  << std::flush; 
        
        }
        
      gkg::Writer::getInstance().write( fileNameFittedMeasurementVolume, 
                                        fittedMeasurementVolume, 
                                        ascii, 
                                        format ); 

      if ( verbose ) 
      {
      
        std::cout << "done " << std::endl; 
        
      }

    }


  }
  GKG_CATCH( "void gkg::SingleCompartmentRelaxometryMapperCommand::execute( "
              "const std::vector< std::string >& fileNameInputVolumes, "
              "const std::string& fileNameMask, "
              "const std::string& qMriMappingType, "
              "const std::vector< double >& optimizerParameters, "
              "const std::vector< double >& scalarParameters, "
              "const std::vector< std::string >& stringParameters, "
              "const std::string& fileNameOutputProtonDensityVolume, "
              "const std::string& fileNameOutputRelaxationTimeVolume, "
              "const std::string& fileNameOutputScalingVolume, "
              "const std::string& fileNameFittedMeasurementVolume, "
              "bool ascii, "
              "const std::string& format, "
              "bool verbose )" );

}


RegisterCommandCreator( 
       SingleCompartmentRelaxometryMapperCommand, 
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameInputVolumes ) +
       DECLARE_STRING_PARAMETER_HELP( fileNameMask ) + 
       DECLARE_STRING_PARAMETER_HELP( qMriMappingType ) + 
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( optimizerParameters ) +
       DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( scalarParameters ) +
       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( stringParameters ) +
       DECLARE_STRING_PARAMETER_HELP( fileNameOutputProtonDensityVolume ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameOutputRelaxationTimeVolume ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameOutputScalingVolume ) + 
       DECLARE_STRING_PARAMETER_HELP( fileNameFittedMeasurementVolume ) + 
       DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
       DECLARE_STRING_PARAMETER_HELP( format ) +
       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
    

