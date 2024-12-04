#include <gkg-dmri-plugin-functors/DwiBundleMapSectionMask/DwiBundleMapSectionMaskCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-processing-container/BoundingBox.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <vector>



//
// class BundleMapSectionMaskProcess
//

class BundleMapSectionMaskProcess : public gkg::Process
{

  public:

    BundleMapSectionMaskProcess( 
                   const std::string& theFileNameCentroids,
                   const std::string& theFileNameSectionMask,
                   const std::string& theFileNameDensityMask,
                   const float& theSectionWidth,
                   const float& theResamplingStep,
                   const gkg::Vector3d< double >& theOutputResolution,
                   const std::string& theOutputFormat,
                   const bool& theNormalizedDensity,
                   const bool& theAscii,
                   const bool& theVerbose );

    const std::string& fileNameCentroids;
    const std::string& fileNameSectionMask;
    const std::string& fileNameDensityMask;
    const float& sectionWidth;
    const float& resamplingStep;
    const gkg::Vector3d< double >& outputResolution;
    const std::string& outputFormat;
    const bool& normalizedDensity;
    const bool& ascii;
    const bool& verbose;

  private:

    template < class L >
    static void operate( gkg::Process& process,
                         const std::string& fileNameIn,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


BundleMapSectionMaskProcess::BundleMapSectionMaskProcess(
                   const std::string& theFileNameCentroids,
                   const std::string& theFileNameSectionMask,
                   const std::string& theFileNameDensityMask,
                   const float& theSectionWidth,
                   const float& theResamplingStep,
                   const gkg::Vector3d< double >& theOutputResolution,
                   const std::string& theOutputFormat,
                   const bool& theNormalizedDensity,
                   const bool& theAscii,
                   const bool& theVerbose )
                         : gkg::Process(),
                           fileNameCentroids( theFileNameCentroids ),
                           fileNameSectionMask( theFileNameSectionMask ),
                           fileNameDensityMask( theFileNameDensityMask ),
                           sectionWidth( theSectionWidth ),
                           resamplingStep( theResamplingStep ),
                           outputResolution( theOutputResolution ),
                           outputFormat( theOutputFormat ),
                           normalizedDensity( theNormalizedDensity ),
                           ascii( theAscii ),
                           verbose( theVerbose )
{

  registerProcess( "BundleMap", gkg::TypeOf< std::string >::getName(),
                   &BundleMapSectionMaskProcess::operate< std::string > );
  registerProcess( "BundleMap", gkg::TypeOf< int16_t >::getName(),
                   &BundleMapSectionMaskProcess::operate< int16_t > );

}


template < class L >
void BundleMapSectionMaskProcess::operate( gkg::Process& process,
 					const std::string& fileNameIn,
 					const gkg::AnalyzedObject&,
 					const std::string& )
{

  try
  {

    BundleMapSectionMaskProcess&
      bundleMapSectionProcess =
        static_cast< BundleMapSectionMaskProcess& >( process );

    const std::string&
      fileNameCentroids = bundleMapSectionProcess.fileNameCentroids;
    const std::string& 
      fileNameSectionMask = bundleMapSectionProcess.fileNameSectionMask;
    const std::string& 
      fileNameDensityMask = bundleMapSectionProcess.fileNameDensityMask;
    const float& 
      sectionWidth = bundleMapSectionProcess.sectionWidth;
    const float& 
      resamplingStep = bundleMapSectionProcess.resamplingStep;
    const gkg::Vector3d< double >& 
      outputResolution = bundleMapSectionProcess.outputResolution;
    const std::string& 
      outputFormat = bundleMapSectionProcess.outputFormat;
    const bool& 
      normalizedDensity = bundleMapSectionProcess.normalizedDensity;
    const bool& 
      ascii = bundleMapSectionProcess.ascii;
    const bool& 
      verbose = bundleMapSectionProcess.verbose;


    ////////////////////////////////////////////////////////////////////////////
    // reading the input bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "reading input '" << fileNameIn << "' bundle map : "
                << std::flush;
    
    }
    gkg::BundleMap< L > bundleMap;
    gkg::Reader::getInstance().read( fileNameIn, bundleMap );

    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }


    ////////////////////////////////////////////////////////////////////////////
    // reading the centroid bundle map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "reading input '" << fileNameIn << "' centroids : "
                << std::flush;
    
    }
    gkg::BundleMap< L > bundleMapOfCentroids;
    gkg::Reader::getInstance().read( fileNameCentroids, bundleMapOfCentroids );

    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // preparing centroids
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "preparing centroids : "
                << std::flush;
    
    }
    int32_t centroidCount = bundleMapOfCentroids.getBundleCount();
    std::vector< gkg::LightCurve3d< float > > centroids;
    centroids.reserve( centroidCount );

    std::map< std::string, int32_t > labelToIndexLut;

    typename gkg::BundleMap< L >::const_iterator
      c = bundleMapOfCentroids.begin(),
      ce = bundleMapOfCentroids.end();
    int32_t count = 0;
    while ( c != ce )
    {

      const L& label = c->first;
      const typename gkg::BundleMap< L >::Bundle& bundle = c->second;
      if ( bundle.getCurve3dCount() != 1 )
      {

        throw std::runtime_error(
                  "bundles corresponding to centroids must have only 1 fiber" );

      }
      labelToIndexLut[ gkg::StringConverter::toString( label ) ] = count;
      centroids.push_back( bundle.getCurve3d( 0 ) );

      ++ count;
      ++ c;

    }

    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    ////////////////////////////////////////////////////////////////////////////
    // computing the density and section mask(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {
    
      std::cout << "computing the density and section mask(s) : "
                << std::flush;
    
    }
    gkg::Volume< float > densityMask;
    gkg::Volume< int16_t > sectionMask;
    std::vector< int32_t > sectionCounts;
    bundleMap.getDensityAndSectionMasks( densityMask,
                                         sectionMask,
                                         sectionCounts,
                                         centroids,
                                         labelToIndexLut,
                                         sectionWidth,
                                         resamplingStep,
                                         outputResolution,
                                         normalizedDensity,
                                         verbose );

    if ( verbose )
    {
    
      std::cout << "done " << std::endl;
    
    }

    //////////////////////////////////////////////////////////////////////////
    // writing output section mask
    //////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '"
                << fileNameSectionMask
                << "' : " << std::flush;

    }

    gkg::Writer::getInstance().write( fileNameSectionMask,
                                      sectionMask,
                                      ascii,
                                      outputFormat );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    //////////////////////////////////////////////////////////////////////////
    // writing output density mask
    //////////////////////////////////////////////////////////////////////////

    if ( !fileNameDensityMask.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '"
                  << fileNameDensityMask
                  << "' : " << std::flush;

      }

      gkg::Writer::getInstance().write( fileNameDensityMask,
                                        densityMask,
                                        ascii,
                                        outputFormat );

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class L > "
             "void BundleMapSectionMaskProcess::operate( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
// class DwiBundleMapSectionMaskCommand
//

gkg::DwiBundleMapSectionMaskCommand::DwiBundleMapSectionMaskCommand(
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
  GKG_CATCH( "gkg::DwiBundleMapSectionMaskCommand::DwiBundleMapSectionMaskCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiBundleMapSectionMaskCommand::DwiBundleMapSectionMaskCommand(
                          const std::string& fileNameInputBundleMap,
                          const std::string& fileNameCentroids,
                          const std::string& fileNameSectionMask,
                          const std::string& fileNameDensityMask,
                          float sectionWidth,
                          float resamplingStep,
                          const std::vector< double >& vectorOfOutputResolution,
                          const std::string& outputFormat,
                          bool normalizedDensity,
                          bool ascii,
                          bool verbose )
                                    : gkg::Command()
{

  try
  {

    execute( fileNameInputBundleMap,
             fileNameCentroids,
             fileNameSectionMask,
             fileNameDensityMask,
             sectionWidth,
             resamplingStep,
             vectorOfOutputResolution,
             outputFormat,
             normalizedDensity,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapSectionMaskCommand::"
             "DwiBundleMapSectionMaskCommand( "
             "const std::string& fileNameInputBundleMap, "
             "const std::string& fileNameCentroids, "
             "const std::string& fileNameSectionMask, "
             "const std::string& fileNameDensityMask, "
             "float sectionWidth, "
             "float resamplingStep, "
             "const std::vector< double >& vectorOfOutputResolution, "
             "const std::string& outputFormat, "
             "bool normalizedDensity, "
             "bool ascii, "
             "bool verbose )" );

}


gkg::DwiBundleMapSectionMaskCommand::DwiBundleMapSectionMaskCommand(
                                             const gkg::Dictionary& parameters )
                                    : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameInputBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameCentroids );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSectionMask );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDensityMask );
    DECLARE_FLOATING_PARAMETER( parameters, float, sectionWidth );
    DECLARE_FLOATING_PARAMETER( parameters, float, resamplingStep );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           vectorOfOutputResolution );
    DECLARE_STRING_PARAMETER( parameters, std::string, outputFormat );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, normalizedDensity );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputBundleMap,
             fileNameCentroids,
             fileNameSectionMask,
             fileNameDensityMask,
             sectionWidth,
             resamplingStep,
             vectorOfOutputResolution,
             outputFormat,
             normalizedDensity,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiBundleMapSectionMaskCommand::"
             "DwiBundleMapSectionMaskCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiBundleMapSectionMaskCommand::~DwiBundleMapSectionMaskCommand()
{
}


std::string gkg::DwiBundleMapSectionMaskCommand::getStaticName()
{

  try
  {

    return "DwiBundleMapSectionMask";

  }
  GKG_CATCH( "std::string "
             "gkg::DwiBundleMapSectionMaskCommand::getStaticName()" );

}


void gkg::DwiBundleMapSectionMaskCommand::parse()
{

  try
  {

    std::string fileNameInputBundleMap;
    std::string fileNameCentroids;
    std::string fileNameSectionMask;
    std::string fileNameDensityMask;
    float sectionWidth = 1.0;
    float resamplingStep = 0.1;
    std::vector< double > vectorOfOutputResolution;
    std::string outputFormat;
    bool normalizedDensity = false;
    bool ascii = false;
    bool verbose = false;

 
    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////
     
    gkg::Application application( _argc, _argv,
                                  "computing mask of bundle sections "
                                  "along centroids",
                                  _loadPlugin );
    application.addSingleOption( "-b",
                                 "Input bundle map file name",
                                 fileNameInputBundleMap );
    application.addSingleOption( "-c",
                                 "Centroid bundle map",
                                 fileNameCentroids );
    application.addSingleOption( "-os",
                                 "Output section mask volume file name",
                                 fileNameSectionMask );
    application.addSingleOption( "-od",
                                 "Output density mask volume file name",
                                 fileNameDensityMask,
                                 true );
    application.addSingleOption( "-w",
                                 "Section width in mm (default=1mm)",
                                 sectionWidth,
				 true );
    application.addSingleOption( "-s",
                                 "Fiber resampling step (default=0.1mm)",
                                 resamplingStep,
				 true );
    application.addSeriesOption( "-r",
                                 "Output volume resolution in mm "
                                 "(default=(1.0, 1.0, 1.0 ))",
                                 vectorOfOutputResolution,
                                 0, 3 );
    application.addSingleOption( "-of",
                                 "Output format name",
                                 outputFormat,
                                 true );
    application.addSingleOption( "-normalizedDensity",
                                 "Normalizing density mask (default=false)",
                                 ascii,
                                 true );
    application.addSingleOption( "-ascii",
                                 "Save ouput in ASCII mode",
                                 ascii,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose,
                                 true ); 
    application.initialize(); 			 


    execute( fileNameInputBundleMap,
             fileNameCentroids,
             fileNameSectionMask,
             fileNameDensityMask,
             sectionWidth,
             resamplingStep,
             vectorOfOutputResolution,
             outputFormat,
             normalizedDensity,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiBundleMapSectionMaskCommand::parse()" );

}


void gkg::DwiBundleMapSectionMaskCommand::execute(
                  	  const std::string& fileNameInputBundleMap,
                          const std::string& fileNameCentroids,
                          const std::string& fileNameSectionMask,
                          const std::string& fileNameDensityMask,
                          float sectionWidth,
                          float resamplingStep,
                          const std::vector< double >& vectorOfOutputResolution,
                          const std::string& outputFormat,
                          bool normalizedDensity,
                          bool ascii,
                          bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity check(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( sectionWidth <= 0 )
    {

       throw std::runtime_error( "section width must be strictly positive" );

    }

    if ( resamplingStep <= 0 )
    {

       throw std::runtime_error(
                            "fiber resampling step must be strictly positive" );

    }

    gkg::Vector3d< double > outputResolution( 1.0, 1.0, 1.0 );
    if ( vectorOfOutputResolution.size() == 3U )
    {

      outputResolution.x = vectorOfOutputResolution[ 0 ];
      outputResolution.y = vectorOfOutputResolution[ 1 ];
      outputResolution.z = vectorOfOutputResolution[ 2 ];

    }
    else if ( !vectorOfOutputResolution.empty() )
    {

       throw std::runtime_error( "output resolution size must be 3" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////

    BundleMapSectionMaskProcess 
      bundleMapSectionMaskProcess( fileNameCentroids,
                                   fileNameSectionMask,
                                   fileNameDensityMask,
                                   sectionWidth,
                                   resamplingStep,
                                   outputResolution,
                                   outputFormat,
                                   normalizedDensity,
                                   ascii,
                                   verbose );
    bundleMapSectionMaskProcess.execute( fileNameInputBundleMap );

  }
  GKG_CATCH( "void gkg::DwiBundleMapSectionMaskCommand::execute( "
             "const std::string& fileNameInputBundleMap, "
             "const std::string& fileNameCentroids, "
             "const std::string& fileNameSectionMask, "
             "const std::string& fileNameDensityMask, "
             "float sectionWidth, "
             "float resamplingStep, "
             "const std::vector< double >& vectorOfOutputResolution, "
             "const std::string& outputFormat, "
             "bool normalizedDensity, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator( 
    DwiBundleMapSectionMaskCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameInputBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameCentroids ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameSectionMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameDensityMask ) +
    DECLARE_FLOATING_PARAMETER_HELP( sectionWidth ) +
    DECLARE_FLOATING_PARAMETER_HELP( resamplingStep ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( vectorOfOutputResolution ) +
    DECLARE_STRING_PARAMETER_HELP( outputFormat ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( normalizedDensity ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
