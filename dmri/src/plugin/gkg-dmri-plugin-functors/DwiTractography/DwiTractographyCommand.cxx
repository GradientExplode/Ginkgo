#include <gkg-dmri-plugin-functors/DwiTractography/DwiTractographyCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-container/SiteMapFactory_i.h>
#include <gkg-processing-coordinates/AntipodallySymmetricUniformOrientationSet.h>
#include <gkg-processing-coordinates/OrientationSetCache.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-algobase/Math.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-transform/Referential.h>
#include <gkg-processing-transform/Transform3dManager.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-processing-transform/RigidTransform3d.h>
#include <gkg-processing-transform/AffineWoShearingTransform3d.h>
#include <gkg-processing-transform/AffineTransform3d.h>
#include <gkg-processing-transform/Scaling3d.h>
#include <gkg-dmri-odf/OrientationDistributionFunction_i.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-container/OdfContinuousField.h>
#include <gkg-dmri-container/OdfCartesianToContinuousField.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-tractography/UnconstrainedSeedingStrategy.h>
#include <gkg-dmri-tractography/AnatomicallyConstrainedSeedingStrategy.h>
#include <gkg-dmri-tractography/MaskAndGFAStoppingStrategy.h>
#include <gkg-dmri-tractography/AnatomicallyInformedStoppingStrategy.h>
#include <gkg-dmri-tractography/PrincipalStartingOrientationStrategy.h>
#include <gkg-dmri-tractography/GibbsSampledStartingOrientationStrategy.h>
#include <gkg-dmri-tractography/TractographyAlgorithm_i.h>
#include <gkg-dmri-tractography/TractographyAlgorithmFactory.h>
#include <gkg-dmri-sh-basis/SymmetricalSphericalHarmonicsCache.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <list>
#include <set>


#define ODFS_VOXEL_REFERENTIAL_NAME  "odfs_voxel_referential"
#define ODFS_REAL_REFERENTIAL_NAME   "odfs_real_referential"

#define ROIS_VOXEL_REFERENTIAL_NAME  "rois_voxel_referential"
#define ROIS_REAL_REFERENTIAL_NAME   "rois_real_referential"

#define MASK_VOXEL_REFERENTIAL_NAME  "mask_voxel_referential"
#define MASK_REAL_REFERENTIAL_NAME   "mask_real_referential"


template < class L > inline
int32_t getGlobalSiteCount( const gkg::SiteMap< L, int32_t >& rois )
{

  try
  {

    // processing the global site count
    int32_t siteCount = 0;
    typename gkg::SiteMap< L, int32_t >::const_iterator
      l = rois.begin(),
      le = rois.end();
    while ( l != le )
    {

      siteCount += ( int32_t )l->second.size();
      ++ l;

    }
    return siteCount;

  }
  GKG_CATCH( "template < class L > "
             "int32_t getGlobalSiteCount( "
             "const gkg::SiteMap< L, int32_t >& rois )" );

}


static int32_t getSiteCountForStep( int32_t step,
                                    int32_t stepCount,
                                    int32_t globalSiteCount )
{

  try
  {

    int32_t siteCountPerStep = globalSiteCount / stepCount;
    if ( stepCount * siteCountPerStep != globalSiteCount )
    {

      ++ siteCountPerStep;

    }
    int32_t siteCountForStep = 0;
    if ( step != stepCount )
    {

      siteCountForStep = siteCountPerStep;

    }
    else
    {

      siteCountForStep = globalSiteCount - ( stepCount - 1 ) * siteCountPerStep;

    }
    return siteCountForStep;

  }
  GKG_CATCH( "int32_t getSiteCountForStep( "
             "int32_t step, "
             "int32_t stepCount, "
             "int32_t globalSiteCount )" );

}


template < class L > inline
gkg::SiteMap< L, int32_t >
getSubRegions( const gkg::SiteMap< L, int32_t >& rois,
	       int32_t step,
               int32_t stepCount )
{

  try
  {

    // speeding-up if one step is used
    if ( stepCount == 1 )
    {

      return rois;

    }

    // get site count already processed for previous step(s)
    int32_t siteCountForPreviousSteps = 0;
    int32_t globalSiteCount = getGlobalSiteCount( rois );
    int32_t s = 1;
    while ( s < step )
    {

      siteCountForPreviousSteps += getSiteCountForStep( s,
                                                        stepCount,
                                                        globalSiteCount );
      ++ s;

    }

    // get required site count for the current step
    int32_t siteCountForCurrentStep = getSiteCountForStep( step,
                                                           stepCount,
                                                           globalSiteCount );

    // allocating the sub-regions
    gkg::SiteMap< L, int32_t > subRois;

    // processing the global site count
    int32_t siteCount = 0;
    int32_t siteCountAccumulator = 0;
    typename gkg::SiteMap< L, int32_t >::const_iterator
      l = rois.begin(),
      le = rois.end();
    while ( ( l != le ) && ( siteCount < siteCountForCurrentStep ) )
    {

      int32_t siteCountForRank = ( int32_t )l->second.size();
      if ( ( siteCountAccumulator + siteCountForRank ) <
           siteCountForPreviousSteps )
      {

        siteCountAccumulator += siteCountForRank;

      }
      else
      {

        const L& label = l->first;
        std::list< gkg::Vector3d< int32_t > > sites;
        std::list< gkg::Vector3d< int32_t > >::const_iterator
          s = l->second.begin(),
          se = l->second.end();
        while ( ( s != se ) &&  ( siteCount < siteCountForCurrentStep ) )
        {

          if ( siteCountAccumulator < siteCountForPreviousSteps )
          {

            ++ siteCountAccumulator;

          }
          else
          {

            sites.push_back( *s );
            ++ siteCount;

          }
          ++ s;

        }
        subRois.addSites( label, sites );

      }
      ++ l;

    }
    return subRois;

  }
  GKG_CATCH( "template < class L > "
             "gkg::SiteMap< L, int32_t > "
             "getSubRegions( "
             "const gkg::SiteMap< L, int32_t >& rois, "
             "int32_t step, "
             "int32_t stepCount )" );

}


static std::string getFileNameBundleMapForStep( const std::string& name,
                                                int32_t step,
                                                int32_t stepCount )
{

  try
  {

    // speeding-up if one step is used
    if ( stepCount == 1 )
    {

      return name;

    }

    // getting header extension
    std::string headerExtensionName = 
      gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

    // building extension set
    std::set< std::string > extensions;
    extensions.insert( ".bundlemap" );
    extensions.insert( std::string( ".bundlemap" ) + headerExtensionName );
    extensions.insert( ".bundlesdata" );
    extensions.insert( ".bundles" );

    // processing name output name

    std::string sixZeros = "000000";
    std::string stepString = gkg::StringConverter::toString( step );
    stepString = sixZeros.substr( 0, 6 - stepString.length() ) + stepString;

    std::string stepCountString = gkg::StringConverter::toString( stepCount );
    stepCountString = sixZeros.substr( 0, 6 - stepCountString.length() ) +
                      stepCountString;


    std::string suffix = '_' + stepString + '_' + stepCountString;
    std::string fileName = name + suffix;
    std::set< std::string >::const_iterator e = extensions.begin(),
                                            ee = extensions.end();
    while ( e != ee )
    {

      if ( name.length() > e->length() )
      {

        if ( name.substr( name.length() - e->length(), e->length()) == *e )
        {

          fileName = name.substr( 0, name.length() - e->length() ) + 
                     suffix + *e;
          break;

        }

      }
      ++ e;

    }
    return fileName;

  }
  GKG_CATCH( "std::string getFileNameBundleMapForStep( "
             "const std::string& name, "
             "int32_t step, "
             "int32_t stepCount )" );

}


//
// class DwiTractographyCommand
//

gkg::DwiTractographyCommand::DwiTractographyCommand( int32_t argc,
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
  GKG_CATCH( "gkg::DwiTractographyCommand::DwiTractographyCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::DwiTractographyCommand::DwiTractographyCommand(
              const std::string& fileNameSiteMap,
              const std::string& fileNameOdfs,
              const std::string& fileNameRois,
              const std::string& fileNameMask,
              const std::string& fileNameRoisToOdfsTransform3d,
              const std::string& fileNameMaskToOdfsTransform3d,
              const std::string& fileNameBundleMap,
              const std::string& algorithmType,
              const std::vector< double >& algorithmScalarParameters,
              const std::vector< std::string >& algorithmStringParameters,
              const std::string& seedingStrategyType,
              const std::vector< std::string >& seedingRegionIds,
              const std::vector< std::string >& seedingSamplingTypes,
              const std::vector< int32_t >& seedingSeedPerVoxelCounts,
              const std::string& stoppingStrategyType,
              const std::vector< double >& stoppingStrategyScalarParameters,
              const std::string& startingOrientationStrategyType,
              const std::vector< double >& startingOrientationScalarParameters,
              int32_t outputOrientationCount,
              const std::string& volumeFormat,
              const std::string& bundleMapFormat,
              int32_t stepCount,
              bool ascii,
              bool verbose )
                            : gkg::Command()
{

  try
  {

    execute( fileNameSiteMap,
             fileNameOdfs,
             fileNameRois,
             fileNameMask,
             fileNameRoisToOdfsTransform3d,
             fileNameMaskToOdfsTransform3d,
             fileNameBundleMap,
             algorithmType,
             algorithmScalarParameters,
             algorithmStringParameters,
             seedingStrategyType,
             seedingRegionIds,
             seedingSamplingTypes,
             seedingSeedPerVoxelCounts,
             stoppingStrategyType,
             stoppingStrategyScalarParameters,
             startingOrientationStrategyType,
             startingOrientationScalarParameters,
             outputOrientationCount,
             volumeFormat,
             bundleMapFormat,
             stepCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTractographyCommand::DwiTractographyCommand( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::string& fileNameRois, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameRoisToOdfsTransform3d, "
             "const std::string& fileNameMaskToOdfsTransform3d, "
             "const std::string& fileNameBundleMap, "
             "const std::string& algorithmType, "
             "const std::vector< double >& algorithmScalarParameters, "
             "const std::vector< std::string >& algorithmStringParameters, "
             "const std::string& seedingStrategyType, "
             "const std::vector< std::string >& seedingRegionIds, "
             "const std::vector< std::string >& seedingSamplingTypes, "
             "const std::vector< int32_t >& seedingSeedPerVoxelCounts, "
             "const std::string& stoppingStrategyType, "
             "const std::vector< double >& stoppingStrategyScalarParameters, "
             "const std::string& startingOrientationStrategyType, "
             "const std::vector< double >& startingOrientationScalarParameters,"
             "int32_t outputOrientationCount, "
             "const std::string& volumeFormat, "
             "const std::string& bundleMapFormat, "
             "int32_t stepCount, "
             "bool ascii, "
             "bool verbose  )" );

}


gkg::DwiTractographyCommand::DwiTractographyCommand(
                                             const gkg::Dictionary& parameters )
                                 : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameSiteMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOdfs );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRois );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMask );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameRoisToOdfsTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              fileNameMaskToOdfsTransform3d );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameBundleMap );
    DECLARE_STRING_PARAMETER( parameters, std::string, algorithmType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           algorithmScalarParameters );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         algorithmStringParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string, seedingStrategyType );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         seedingRegionIds );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, std::vector< std::string >,
                                         seedingSamplingTypes );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters, std::vector< int32_t >,
                                         seedingSeedPerVoxelCounts );
    DECLARE_STRING_PARAMETER( parameters, std::string, stoppingStrategyType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                           stoppingStrategyScalarParameters );
    DECLARE_STRING_PARAMETER( parameters, std::string,
                              startingOrientationStrategyType );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters, std::vector< double >,
                                          startingOrientationScalarParameters );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, outputOrientationCount );
    DECLARE_STRING_PARAMETER( parameters, std::string, volumeFormat );
    DECLARE_STRING_PARAMETER( parameters, std::string, bundleMapFormat );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, stepCount );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameSiteMap,
             fileNameOdfs,
             fileNameRois,
             fileNameMask,
             fileNameRoisToOdfsTransform3d,
             fileNameMaskToOdfsTransform3d,
             fileNameBundleMap,
             algorithmType,
             algorithmScalarParameters,
             algorithmStringParameters,
             seedingStrategyType,
             seedingRegionIds,
             seedingSamplingTypes,
             seedingSeedPerVoxelCounts,
             stoppingStrategyType,
             stoppingStrategyScalarParameters,
             startingOrientationStrategyType,
             startingOrientationScalarParameters,
             outputOrientationCount,
             volumeFormat,
             bundleMapFormat,
             stepCount,
             ascii,
             verbose );

  }
  GKG_CATCH( "gkg::DwiTractographyCommand::DwiTractographyCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DwiTractographyCommand::~DwiTractographyCommand()
{
}


std::string gkg::DwiTractographyCommand::getStaticName()
{

  try
  {

    return "DwiTractography";

  }
  GKG_CATCH( "std::string gkg::DwiTractographyCommand::getStaticName()" );

}


void gkg::DwiTractographyCommand::parse()
{

  try
  {

    std::string fileNameSiteMap;
    std::string fileNameOdfs;

    std::string fileNameRois;
    std::string fileNameMask = "";
    std::string fileNameRoisToOdfsTransform3d = "";
    std::string fileNameMaskToOdfsTransform3d = "";

    std::string fileNameBundleMap;

    std::string algorithmType = "streamline-deterministic";

    std::vector< double > algorithmScalarParameters;
    std::vector< std::string > algorithmStringParameters;

    std::string seedingStrategyType = "unconstrained";
                // ex: anatomically-constrained
    std::vector< std::string > seedingRegionIds;
                // ex in case of anatomically-constrained mode:
                //     white-matter + 
                //     cortex-white-matter-interface +
                //     deep-gray-white-matter-interface
                // ex in case of unconstrained mode:
                //     1 2 4
    std::vector< std::string > seedingSamplingTypes;
                // ex: cartesian +
                //     random +
                //     random
    std::vector< int32_t > seedingSeedPerVoxelCounts;
                // ex: 8 +
                //     27 +
                //     27

    std::string stoppingStrategyType = "mask-and-gfa";
    std::vector< double > stoppingStrategyScalarParameters;

    std::string startingOrientationStrategyType = "principal-orientation";
    std::vector< double > startingOrientationStrategyScalarParameters;

    int32_t outputOrientationCount = 0;

    std::string volumeFormat = "gis";
    std::string bundleMapFormat = "bundlemap";
    int32_t stepCount = 1;
    bool ascii = false;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv, "DWI tractography tool",
                                  _loadPlugin );
    application.addSingleOption( "-s",
                                 "Input site map",
                                 fileNameSiteMap );
    application.addSingleOption( "-o",
                                 "Input odf texture map file name",
                                 fileNameOdfs );
    application.addSingleOption( "-r",
                                 "Input volume/site map ROI file name",
                                 fileNameRois );
    application.addSingleOption( "-m",
                                 "Input mask file name;  it can include several"
                                 " label values to allow seeding differently "
                                 "according to each label, for intance "
                                 "to seed differently from gray matter "
                                 "than from white matter; if the seeding "
                                 "strategy is chosen to be 'unconstrained' "
                                 "the mask can include any integer label "
                                 "different from 0; if the seeding strategy "
                                 "is chosen to be 'anatomically-constrained' "
                                 "labels of the mask are fixed and correspond "
                                 "to 1->cortex 2->white-matter "
                                 "3->deep-gray-matter 4->cerebro-spinal-fluid"
                                 "; in all cases, a value 0 means outside the "
                                 "brain",
                                 fileNameMask );
    application.addSingleOption( "-tr",
                                 "3D rigid/affine wo shearing/affine transform"
                                 " from ROIs to ODF "
                                 "referential (default=identity)",
                                 fileNameRoisToOdfsTransform3d,
                                 true );
    application.addSingleOption( "-tm",
                                 "3D rigid/affine wo shearing/affine transform"
                                 " from mask to ODF "
                                 "referential (default=identity)",
                                 fileNameMaskToOdfsTransform3d,
                                 true );
    application.addSingleOption( "-b",
                                 "Output bundle map file name",
                                 fileNameBundleMap );

    application.addSingleOption( "-type",
                                 "Tractography algorithm type "
                                 "(default=streamline-deterministic): \n"
                                 "- streamline-deterministic\n"
                                 "- streamline-regularized-deterministic\n"
                                 "- streamline-probabilistic\n"
                                 "- global-spin-glass\n",
                                 algorithmType,
                                 true );

    application.addSeriesOption( "-scalarParameters",
                                 "Tractography algorithm scalar parameters as a "
                                 "vector of double <P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of streamline-deterministic,\n"
                                 "   <P1>: forward step during fiber tracking\n"
                                 "   <P2>: storing increment, ie do not store"
                                         " each fiber point but only one out"
                                         " of the provided argument\n"
                                 "   <P3>: minimum fiber length in mm\n"
                                 "   <P4>: maximum fiber length in mm\n"
                                 "   <P5>: aperture angle,ie half cone inside"
                                         " which to look for neighbors given"
                                         " degrees\n"
                                 "   (default= 0.25 x minimum voxel "
                                 "resolution / 10 / 5 / 200 / 90 )\n\n"
                                 "- in case of "
                                 "streamline-regularized-deterministic,\n"
                                 "   <P1>: forward step during fiber tracking\n"
                                 "   <P2>: storing increment, ie do not store"
                                         " each fiber point but only one out"
                                         " of the provided argument\n"
                                 "   <P3>: minimum fiber length in mm\n"
                                 "   <P4>: maximum fiber length in mm\n"
                                 "   <P5>: aperture angle,ie half cone inside"
                                         " which to look for neighbors given"
                                         " degrees\n"
                                 "   <P6>: imposed lower GFA boundary\n"
                                        " (-1 = automatically processed)\n"
                                 "   <P7>: imposed upper GFA boundary\n"
                                        " (-1 = automatically processed)\n"
                                 "   (default= 0.25 x minimum voxel "
                                 "resolution / 10 / 5 / 200 / 90 / -1 / -1 / "
                                             "  )\n\n"
                                 "- in case of streamline-probabilistic,\n"
                                 "   <P1>: forward step during fiber tracking\n"
                                 "   <P2>: storing increment, ie do not store"
                                         " each fiber point but only one out"
                                         " of the provided argument\n"
                                 "   <P3>: minimum fiber length in mm\n"
                                 "   <P4>: maximum fiber length in mm\n"
                                 "   <P5>: aperture angle,ie half cone inside"
                                         " which to look for neighbors given"
                                         " degrees\n"
                                 "   <P6>: temperature of the Gibb's sampler\n"
                                 "   <P7>: imposed lower GFA boundary"
                                        " (-1 = automatically processed)\n"
                                 "   <P8>: imposed upper GFA boundary"
                                        " (-1 = automatically processed)\n"
                                 "   (default= 0.25 x minimum voxel "
                                 "resolution / 10 / 5 / 200 / 90 / 1 / -1 / "
                                 "-1 )\n\n"
                                 "- in case of global-spin-glass,\n"
                                 "   <P1>: minimum fiber length in mm\n"
                                 "   <P2>: maximum fiber length in mm\n"
                                 "   <P3>: iteration count\n"
                                 "   <P4>: Gibb's sampler temperature\n"
                                 "   <P5>: initial external temperature\n"
                                 "   <P6>: initial internal temperature\n"
                                 "   <P7>: final internal temperature\n"
                                 "   <P8>: external energy weight\n"
                                 "   <P9>: internal energy weight\n"
                                 "   <P10>: neighbor count for the clique"
                                 " used in the optimal  motion proposal\n"
                                 "   <P11>: orientation dispersion scaling "
                                 "(>0 in general and used to set the fixed "
                                 "value in case no orientation dispersion "
                                 "volume is given )\n"
                                 "   <P12>: spin glass density "
                                 "(-1 -> used string parameter P3 instead)\n"
                                 "   <P13>: connection likelihood\n"
                                 "   <P14>: curvature threshold\n"
                                 "   <P15>: maximum distance to mesh"
                                 " to be considered for sharp turn guidance\n"
                                 "   <P16>: target cache stride of scene"
                                 " modeler\n"
                                 "   (default= 5 / 200 / 10000000 / 0.01 / "
                                 "0.001 / 0.1 / 0.001 / 20 / 200 / "
                                 "5 / 1.0 / 1.0 / 0.5 / 10deg / "
                                 "1.0 / 1 )\n",
                                 algorithmScalarParameters );
    application.addSeriesOption( "-stringParameters",
                                 "Tractography algorithm string parameters as a"
                                 " vector of string <P1> <P2> <P3> ...<Pn>:\n\n"
                                 "- in case of streamline-deterministic,\n"
                                 "   N/A\n\n"
                                 "- in case of "
                                 "streamline-regularized-deterministic,\n"
                                 "   N/A\n\n"
                                 "- in case of streamline-probabilistic,\n"
                                 "   N/A\n\n"
                                 "- in case of global-spin-glass,\n"
                                 "   <P1>: pial surface filename "
                                 "(default= none)\n"
                                 "   <P2>: pial surface to ROI 3D transform "
                                 "filename (default= id)\n"
                                 "   <P3>: orientation dispersion volume "
                                 "filename (default= none)\n"
                                 "   <P4>: orientation dispersion volume to ROI"
                                 " 3D transform filename (default= id)\n"
                                 "   <P5>: spin glass density volume filename "
                                 "(default= none)\n"
                                 "   <P6>: spin glass density volume to ROI 3D "
                                 "transform filename (default= id)\n"
                                 "   <P7>: connection likelihood volume "
                                 "filename (default= none)\n"
                                 "   <P8>: connection likelihood volume to ROI"
                                 "3D transform filename (default= id)\n"
                                 "   <P9>: curvature threshold volume "
                                 "filename (default= none)\n"
                                 "   <P10>: curvature thresholed volume to ROI "
                                 "3D transform filename (default= id)\n"
                                 "   <P11>: distance to ventricles map volume "
                                 "filename (default= none)\n"
                                 "   <P12>: distance to ventricles map volume "
                                 "to ROI 3D transform filename "
                                 "(default= id)\n\n",
                                 algorithmStringParameters );

    application.addSingleOption( "-seedingStrategyType",
                                 "Seeding strategy type, being one of:\n"
                                 "- unconstrained\n"
                                 "- anatomically-constrained\n"
                                 "(default=unconstrained)\n\n",
                                 seedingStrategyType );

    application.addSeriesOption( "-seedingRegionIds",
                                 "- case of anatomically constrained strategy "
                                 "mode, providing the list of regions where to "
                                 "seed, being one of:\n"
                                 "  * pial-cortex_interface\n"
                                 "  * cortex\n"
                                 "  * cortex-white-matter-interface\n"
                                 "  * white-matter\n"
                                 "  * deep-gray-matter-white-matter-interface\n"
                                 "  * deep-gray-matter\n"
                                 "  (default=white-matter "
                                 "cortex-white-matter-interface "
                                 "deep-gray-matter-white-matter-interface)\n\n"
                                 "- case of unconstrained strategy mode "
                                 "providing the list of non-nul integer ids"
                                 "corresponding to regions where to seed "
                                 "(default=taking all available regions)\n\n",
                                 seedingRegionIds );

     application.addSeriesOption( "-seedingSamplingTypes",
                                 "Seeding strategies for each label of the "
                                 "computing mask in unconstrained mode or "
                                 "each region type in anatomically constrained "
                                 "mode, being one of:\n"
                                 "- cartesian\n"
                                 "- random\n"
                                 "(if a single strategy is given, then "
                                 "duplicate it for all the labels or regions) "
                                 "(default=cartesian)",
                                 seedingSamplingTypes );

    application.addSeriesOption( "-seedingSeedPerVoxelCounts",
                                 "Number of seeds per voxel corresponding to "
                                 "each region prescribed with the "
                                 "'-seedingRegionIds' option and "
                                 "with the same order; it looks like :\n"
                                 "27 1000\n"
                                 "(defaul=1, if a single integer is given, "
                                 "we assume that all labels or regions will have "
                                 "voxels seeded with the same "
                                 "number of seeds)",
                                 seedingSeedPerVoxelCounts );

    application.addSingleOption( "-stoppingStrategyType",
                                 "Stopping strategy type, being one of:\n"
                                 "- mask-and-gfa\n"
                                 "- anatomically-informed\n"
                                 "(default=mask-and-gfa)\n\n",
                                 stoppingStrategyType );

    application.addSeriesOption( "-stoppingStrategyScalarParameters",
                                 "Stopping strategy scalar parameters as a "
                                 "vector of double <P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of mask-and-gfa,\n"
                                 ".  <P1>: 0->no GFA threshold \n"
                                 ".        1->using GFA threshold \n"
                                 ".  <P2>: GFA threshold\n\n"
                                 "- in case of anatomically-informed,\n"
                                 ".  <P1>: 0->no GFA threshold \n"
                                 ".        1->using GFA threshold \n"
                                 ".  <P2>: GFA threshold\n"
                                 ".  (default= 0 / 0.0 )\n\n",
                                 stoppingStrategyScalarParameters );

    application.addSingleOption( "-startingOrientationStrategyType",
                                 "Starting orientation strategy type, "
                                 "being one of:\n"
                                 "- principal-orientation\n"
                                 "- gibbs-sampled-orientation\n"
                                 "(default=principal-orientation)\n\n",
                                 startingOrientationStrategyType );

    application.addSeriesOption( "-startingOrientationStrategyScalarParameters",
                                 "Starting orientation strategy scalar "
                                 "parameters as a "
                                 "vector of double <P1> <P2> <P3> ... <Pn>:\n\n"
                                 "- in case of principal-orientation,\n"
                                 ".  none\n\n"
                                 "- in case of gibbs-sampled-orientation,\n"
                                 ".  <P1>: Gibb's temperature \n"
                                 ".        = 1.0 -> no sharpening \n"
                                 ".        < 1.0 -> sharpening \n"
                                 ".  (default= 1.0 )\n\n",
                                 startingOrientationStrategyScalarParameters );

    application.addSingleOption( "-outputOrientationCount",
                                 "Ouput orientation count using an "
                                 "antipodally symmetric uniform orientation "
                                 "set generator (default=0, please use an "
                                 "even number)",
                                 outputOrientationCount,
                                 true );

    application.addSingleOption( "-volumeFormat",
                                 "Ouput volume format (default=GIS)",
                                 volumeFormat,
                                 true );
    application.addSingleOption( "-bundleMapFormat",
                                 "Ouput BundleMap format (default=bundlemap)",
                                 bundleMapFormat,
                                 true );
    application.addSingleOption( "-stepCount",
                                 "Divide the tractography into several steps "
				 "to prevent memory runout (default=1)",
                                 stepCount,
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

    execute( fileNameSiteMap,
             fileNameOdfs,
             fileNameRois,
             fileNameMask,
             fileNameRoisToOdfsTransform3d,
             fileNameMaskToOdfsTransform3d,
             fileNameBundleMap,
             algorithmType,
             algorithmScalarParameters,
             algorithmStringParameters,
             seedingStrategyType,
             seedingRegionIds,
             seedingSamplingTypes,
             seedingSeedPerVoxelCounts,
             stoppingStrategyType,
             stoppingStrategyScalarParameters,
             startingOrientationStrategyType,
             startingOrientationStrategyScalarParameters,
             outputOrientationCount,
             volumeFormat,
             bundleMapFormat,
             stepCount,
             ascii,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DwiTractographyCommand::parse()" );

}


void gkg::DwiTractographyCommand::execute(
                const std::string& fileNameSiteMap,
                const std::string& fileNameOdfs,
                const std::string& fileNameRois,
                const std::string& fileNameMask,
                const std::string& fileNameRoisToOdfsTransform3d,
                const std::string& fileNameMaskToOdfsTransform3d,
                const std::string& fileNameBundleMap,
                const std::string& algorithmType,
                const std::vector< double >& algorithmScalarParameters,
                const std::vector< std::string >& algorithmStringParameters,
                const std::string& seedingStrategyType,
                const std::vector< std::string >& seedingRegionIds,
                const std::vector< std::string >& seedingSamplingTypes,
                const std::vector< int32_t >& seedingSeedPerVoxelCounts,
                const std::string& stoppingStrategyType,
                const std::vector< double >& stoppingStrategyScalarParameters,
                const std::string& startingOrientationStrategyType,
                const std::vector< double >&
                                    startingOrientationStrategyScalarParameters,
                int32_t outputOrientationCount,
                const std::string& /* volumeFormat */,
                const std::string& bundleMapFormat,
                int32_t stepCount,
                bool ascii,
                bool verbose )
 {

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( ( algorithmType != "streamline-deterministic" ) &&
         ( algorithmType != "streamline-regularized-deterministic" ) &&
         ( algorithmType != "streamline-probabilistic" ) &&
         ( algorithmType != "global-spin-glass" ) )
    {

      throw std::runtime_error( "bad tractography algorithm type" );

    }
    if ( outputOrientationCount < 0 )
    {

      throw std::runtime_error( "output orientation count must be positive" );

    }
    if ( stepCount < 1 )
    {

      throw std::runtime_error( "step count must be strictly positive" );

    }
    if ( algorithmType == "global-spin-glass" )
    {

      if ( stepCount != 1 )
      {

        throw std::runtime_error( "cannot divide the optimization process "
                                  "into several steps when using "
                                  "'global-spin-glass' tractography and" 
                                  "step count must be equal to 1" );

      }

    }
    if ( ( seedingStrategyType != "unconstrained" ) &&
          ( seedingStrategyType != "anatomically-constrained" ) )
    {

      throw std::runtime_error( "bad seeding strategy type" );

    }
    if ( ( stoppingStrategyType != "mask-and-gfa" ) &&
          ( stoppingStrategyType != "anatomically-informed" ) )
    {

      throw std::runtime_error( "bad stopping strategy type" );

    }
    if ( ( startingOrientationStrategyType != "principal-orientation" ) &&
          ( startingOrientationStrategyType != "gibbs-sampled-orientation" ) )
    {

      throw std::runtime_error( "bad starting orientation strategy type" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // algorithm parameter(s) check or initialization
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double >
      theAlgorithmScalarParameters = algorithmScalarParameters;
    std::vector< std::string >
      theAlgorithmStringParameters = algorithmStringParameters;
    if ( algorithmType == "streamline-deterministic" )
    {

      if ( !theAlgorithmScalarParameters.empty() )
      {

        if ( theAlgorithmScalarParameters.size() != 5U )
        {

          throw std::runtime_error( "there must be 5 scalar parameters" );

        }

        // forward step
        if ( theAlgorithmScalarParameters[ 0 ] < 0.0 )
        {

          throw std::runtime_error( "forward step must be positive or null" );

        }
        // storing increment
        if ( theAlgorithmScalarParameters[ 1 ] < 1 )
        {

          throw std::runtime_error( "storing increment must be at least 1" );

        }
        // minimum fiber length
        if ( theAlgorithmScalarParameters[ 2 ] <= 0.0 )
        {

          throw std::runtime_error(
                             "minimum fiber length must be at least positive" );

        }
        // maximum fiber length
        if ( theAlgorithmScalarParameters[ 3 ] <
             theAlgorithmScalarParameters[ 2 ] )
        {

          throw std::runtime_error(
                             "storing increment must be at least greater than "
                             "the minimum fiber length" );

        }
        // aperture angle
        if ( theAlgorithmScalarParameters[ 4 ] <= 0.0 )
        {

          throw std::runtime_error(
                                  "aperture angle must be strictly positive" );

        }
        // converting from degree to radian
        theAlgorithmScalarParameters[ 4 ] *= M_PI / 180.0;

      }
      else if ( theAlgorithmScalarParameters.empty() )
      {

        theAlgorithmScalarParameters.resize( 5 );
        theAlgorithmScalarParameters[ 0 ] = 0;     // initialized after
        theAlgorithmScalarParameters[ 1 ] = 10;
        theAlgorithmScalarParameters[ 2 ] = 5.0;
        theAlgorithmScalarParameters[ 3 ] = 200.0;
        theAlgorithmScalarParameters[ 4 ] = M_PI / 2;

      }

    }
    else if ( algorithmType == "streamline-regularized-deterministic" )
    {

      if ( !theAlgorithmScalarParameters.empty() )
      {

        if ( theAlgorithmScalarParameters.size() != 7U )
        {

          throw std::runtime_error( "there must be 7 scalar parameters" );

        }

        // forward step
        if ( theAlgorithmScalarParameters[ 0 ] < 0.0 )
        {

          throw std::runtime_error( "forward step must be positive or null" );

        }
        // storing increment
        if ( theAlgorithmScalarParameters[ 1 ] < 1 )
        {

          throw std::runtime_error( "storing increment must be at least 1" );

        }
        // minimum fiber length
        if ( theAlgorithmScalarParameters[ 2 ] <= 0.0 )
        {

          throw std::runtime_error(
                             "minimum fiber length must be at least positive" );

        }
        // maximum fiber length
        if ( theAlgorithmScalarParameters[ 3 ] <
             theAlgorithmScalarParameters[ 2 ] )
        {

          throw std::runtime_error(
                             "storing increment must be at least greater than "
                             "the minimum fiber length" );

        }
        // aperture angle
        if ( theAlgorithmScalarParameters[ 4 ] <= 0.0 )
        {

          throw std::runtime_error(
                                  "aperture angle must be strictly positive" );

        }
        // converting from degree to radian
        theAlgorithmScalarParameters[ 4 ] *= M_PI / 180.0;

      }
      else if ( theAlgorithmScalarParameters.empty() )
      {

        theAlgorithmScalarParameters.resize( 7 );
        theAlgorithmScalarParameters[ 0 ] = 0;     // initialized after
        theAlgorithmScalarParameters[ 1 ] = 10;
        theAlgorithmScalarParameters[ 2 ] = 5.0;
        theAlgorithmScalarParameters[ 3 ] = 200.0;
        theAlgorithmScalarParameters[ 4 ] = M_PI / 2;
        theAlgorithmScalarParameters[ 5 ] = -1.0;
        theAlgorithmScalarParameters[ 6 ] = -1.0;

      }

    }
    else if ( algorithmType == "streamline-probabilistic" )
    {

      if ( !theAlgorithmScalarParameters.empty() )
      {

        if ( theAlgorithmScalarParameters.size() != 8U )
        {

          throw std::runtime_error( "there must be 8 scalar parameters" );

        }

        // forward step
        if ( theAlgorithmScalarParameters[ 0 ] < 0.0 )
        {

          throw std::runtime_error( "forward step must be positive or null" );

        }
        // storing increment
        if ( theAlgorithmScalarParameters[ 1 ] < 1 )
        {

          throw std::runtime_error( "storing increment must be at least 1" );

        }
        // minimum fiber length
        if ( theAlgorithmScalarParameters[ 2 ] <= 0.0 )
        {

          throw std::runtime_error(
                             "minimum fiber length must be at least positive" );

        }
        // maximum fiber length
        if ( theAlgorithmScalarParameters[ 3 ] <
             theAlgorithmScalarParameters[ 2 ] )
        {

          throw std::runtime_error(
                             "storing increment must be at least greater than "
                             "the minimum fiber length" );

        }
        // aperture angle
        if ( theAlgorithmScalarParameters[ 4 ] <= 0.0 )
        {

          throw std::runtime_error(
                                  "aperture angle must be strictly positive" );

        }
        // Gibb's temperature
        if ( theAlgorithmScalarParameters[ 5 ] <= 0.0 )
        {

          throw std::runtime_error(
                               "Gibb's temperature must be strictly positive" );

        }
        // converting from degree to radian
        theAlgorithmScalarParameters[ 4 ] *= M_PI / 180.0;

      }
      else if ( theAlgorithmScalarParameters.empty() )
      {

        theAlgorithmScalarParameters.resize( 8 );
        theAlgorithmScalarParameters[ 0 ] = 0;     // initialized after
        theAlgorithmScalarParameters[ 1 ] = 10;
        theAlgorithmScalarParameters[ 2 ] = 5.0;
        theAlgorithmScalarParameters[ 3 ] = 200.0;
        theAlgorithmScalarParameters[ 4 ] = M_PI / 2;
        theAlgorithmScalarParameters[ 5 ] = 1;
        theAlgorithmScalarParameters[ 6 ] = -1.0;
        theAlgorithmScalarParameters[ 7 ] = -1.0;

      }

    }
    else if ( algorithmType == "global-spin-glass" )
    {

      if ( !theAlgorithmScalarParameters.empty() )
      {

        if ( theAlgorithmScalarParameters.size() != 16U )
        {

          throw std::runtime_error( "there must be 16 scalar parameters" );

        }

        // minimum fiber length
        if ( theAlgorithmScalarParameters[ 0 ] <= 0.0 )
        {

          throw std::runtime_error(
                             "minimum fiber length must be strictly positive" );

        }
        // maximum fiber length
        if ( theAlgorithmScalarParameters[ 1 ] <
             theAlgorithmScalarParameters[ 0 ] )
        {

          throw std::runtime_error( "the maximum fiber length must be greater "
                                    "than the minimum fiber length" );

        }
        // iteration count
        if ( theAlgorithmScalarParameters[ 2 ] <= 0.0 )
        {

          throw std::runtime_error(
                                  "iteration count must be strictly positive" );

        }
        // Gibb's temperature for the spin glass creation orientation
        // computation
        if ( theAlgorithmScalarParameters[ 3 ] <= 0.0 )
        {

          throw std::runtime_error(
                                "Gibbs temperature must be strictly positive" );

        }
        // initial external temperature
        if ( theAlgorithmScalarParameters[ 4 ] <= 0.0 )
        {

          throw std::runtime_error(
                     "initial external temperature must be strictly positive" );

        }
        // initial internal temperature
        if ( theAlgorithmScalarParameters[ 5 ] <= 0.0 )
        {

          throw std::runtime_error(
                     "initial internal temperature must be strictly positive" );

        }
        // final internal temperature
        if ( theAlgorithmScalarParameters[ 6 ] >
             theAlgorithmScalarParameters[ 5 ] )
        {

          throw std::runtime_error( "final internal temperature must be lower "
                                    "than the initial one" );

        }
        // external energy weight
        if ( theAlgorithmScalarParameters[ 7 ] < 0.0 )
        {

          throw std::runtime_error( "external energy weight must be positive" );

        }
        // internal energy weight
        if ( theAlgorithmScalarParameters[ 8 ] < 0.0 )
        {

          throw std::runtime_error( "internal energy weight must be positive" );

        }
                                     
        // neighbor count used for the clique size in the optimal motion 
        // proposal
        if ( theAlgorithmScalarParameters[ 9 ] < 0.0 )
        {

          throw std::runtime_error( 
                   "neighbor count used for the clique size in the "
                   "optimal motion proposal must be strictly positive or nul" );

        }

        // orientation dispersion scaling
        if ( theAlgorithmScalarParameters[ 10 ] < 0.0 )
        {

          throw std::runtime_error( 
                  "orientation dispersion scaling must be strictly positive " );

        }

        // spin glass density
        if ( theAlgorithmScalarParameters[ 11 ] < 0.0 )
        {

          throw std::runtime_error( 
                                 "spin glass density must be positive or nul" );

        }

        // connection likelihood
        if ( theAlgorithmScalarParameters[ 12 ] <= 0.0 )
        {

          throw std::runtime_error( "connection likelihood must be positive or "
                                    "equal to 0.0 if the volume is given" );

        }

        // curvature threshold
        if ( theAlgorithmScalarParameters[ 13 ] <= 0.0 &&
             theAlgorithmScalarParameters[ 13 ] >= 360.0 )
        {

          throw std::runtime_error( "curvature threshold must be positive or "
                                    "equal to 0.0 if the volume is given" );

        }

        // maximum distance to mesh to be considered for sharp turn guidance
        if ( theAlgorithmScalarParameters[ 14 ] < 0.0 )
        {

          throw std::runtime_error(
                     "maximum distance to mesh to be considered for sharp turn "
                     "guidance must be positive" );

        }

        // target cache stride
        if ( theAlgorithmScalarParameters[ 15 ] < 1.0 )
        {

          throw std::runtime_error(
                          "target cache stride must be greater or equal to 1" );

        }

      }
      else if ( theAlgorithmScalarParameters.empty() )
      {

        theAlgorithmScalarParameters.resize( 16 );
        theAlgorithmScalarParameters[ 0 ] = 5.0;
        theAlgorithmScalarParameters[ 1 ] = 200.0;
        theAlgorithmScalarParameters[ 2 ] = 10000000;
        theAlgorithmScalarParameters[ 3 ] = 0.01;
        theAlgorithmScalarParameters[ 4 ] = 0.001;
        theAlgorithmScalarParameters[ 5 ] = 0.1;
        theAlgorithmScalarParameters[ 6 ] = 0.001;
        theAlgorithmScalarParameters[ 7 ] = 200;
        theAlgorithmScalarParameters[ 8 ] = 20;
        theAlgorithmScalarParameters[ 9 ] = 5.0;
        theAlgorithmScalarParameters[ 10 ] = 1.0;
        theAlgorithmScalarParameters[ 11 ] = 1.0;
        theAlgorithmScalarParameters[ 12 ] = 0.5;
        theAlgorithmScalarParameters[ 13 ] = 20.0;
        theAlgorithmScalarParameters[ 14 ] = 1.0;
        theAlgorithmScalarParameters[ 15 ] = 1.0;

      }
      else
      {

        throw std::runtime_error( "bad algorithm parameter count" );

      }

      if ( !theAlgorithmStringParameters.empty() )
      {

        if ( theAlgorithmStringParameters.size() != 12U )
        {

          throw std::runtime_error( "there must be 12 string parameters" );

        }

      }
      else
      {

        theAlgorithmStringParameters.resize( 12 );
        theAlgorithmStringParameters[ 0 ] = "none";
        theAlgorithmStringParameters[ 1 ] = "id";
        theAlgorithmStringParameters[ 2 ] = "none";
        theAlgorithmStringParameters[ 3 ] = "id";
        theAlgorithmStringParameters[ 4 ] = "none";
        theAlgorithmStringParameters[ 5 ] = "id";
        theAlgorithmStringParameters[ 6 ] = "none";
        theAlgorithmStringParameters[ 7 ] = "id";
        theAlgorithmStringParameters[ 8 ] = "none";
        theAlgorithmStringParameters[ 9 ] = "id";
        theAlgorithmStringParameters[ 10 ] = "none";
        theAlgorithmStringParameters[ 11 ] = "id";

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // stopping parameter(s) check or initialization
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double >
      theStoppingStrategyScalarParameters = stoppingStrategyScalarParameters;
    if ( algorithmType == "mask-and-gfa" )
    {

      if ( !theStoppingStrategyScalarParameters.empty() )
      {

        if ( theStoppingStrategyScalarParameters.size() != 2U )
        {

          throw std::runtime_error(
                        "there must be 2 stopping strategy scalar parameters" );

        }

        // using GFA or not ?
        int32_t usingGFA = 
                  ( int32_t )( theStoppingStrategyScalarParameters[ 0 ] + 0.5 );
        if ( ( usingGFA != 0 ) && ( usingGFA != 1 ) )
        {

          throw std::runtime_error( "using GFA must be equal to 0 or 1" );

        }

        if ( ( theStoppingStrategyScalarParameters[ 1 ] < 0.0 ) ||
             ( theStoppingStrategyScalarParameters[ 1 ] > 1.0 ) )
        {

          throw std::runtime_error(
                   "stopping strategy GFA threshold must belong to [0.0;1.0]" );

        }

      }
      else if ( theStoppingStrategyScalarParameters.empty() )
      {

        theStoppingStrategyScalarParameters.resize( 2 );
        theStoppingStrategyScalarParameters[ 0 ] = 0.0;
        theStoppingStrategyScalarParameters[ 1 ] = 0.0;

      }

    }
    else if ( algorithmType == "anatomically-informed" )
    {

      if ( !theStoppingStrategyScalarParameters.empty() )
      {

        if ( theStoppingStrategyScalarParameters.size() != 2U )
        {

          throw std::runtime_error(
                        "there must be 2 stopping strategy scalar parameters" );

        }

        // using GFA or not ?
        int32_t usingGFA = 
                  ( int32_t )( theStoppingStrategyScalarParameters[ 0 ] + 0.5 );
        if ( ( usingGFA != 0 ) && ( usingGFA != 1 ) )
        {

          throw std::runtime_error( "using GFA must be equal to 0 or 1" );

        }

        if ( ( theStoppingStrategyScalarParameters[ 1 ] < 0.0 ) ||
             ( theStoppingStrategyScalarParameters[ 1 ] > 1.0 ) )
        {

          throw std::runtime_error(
                   "stopping strategy GFA threshold must belong to [0.0;1.0]" );

        }

      }
      else if ( theStoppingStrategyScalarParameters.empty() )
      {

        theStoppingStrategyScalarParameters.resize( 2 );
        theStoppingStrategyScalarParameters[ 0 ] = 0.0;
        theStoppingStrategyScalarParameters[ 1 ] = 0.0;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // starting orientation parameter(s) check or initialization
    ////////////////////////////////////////////////////////////////////////////

    std::vector< double >
      theStartingOrientationStrategyScalarParameters = 
                                    startingOrientationStrategyScalarParameters;
    if ( algorithmType == "principal-orientation" )
    {

      if ( !theStartingOrientationStrategyScalarParameters.empty() )
      {

        throw std::runtime_error(
           "there must be no starting orientation strategy scalar parameters" );

      }

    }
    else if ( algorithmType == "gibbs-sampled-orientation" )
    {

      if ( !theStartingOrientationStrategyScalarParameters.empty() )
      {

        if ( theStartingOrientationStrategyScalarParameters.size() != 1U )
        {

          throw std::runtime_error(
            "there must be 1 starting orientation strategy scalar parameters" );

        }

        if ( ( theStartingOrientationStrategyScalarParameters[ 0 ] <= 0.0 ) ||
             ( theStartingOrientationStrategyScalarParameters[ 0 ] > 1.0 ) )
        {

          throw std::runtime_error(
            "starting orientation strategy temperature must belong to ]0.0;1.0]"
                                  );

        }

      }
      else if ( theStartingOrientationStrategyScalarParameters.empty() )
      {

        theStartingOrientationStrategyScalarParameters.resize( 1U );
        theStartingOrientationStrategyScalarParameters[ 0 ] = 1.0;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading site(s)
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameSiteMap << "' : " << std::flush;

    }
    gkg::SiteMap< int32_t, int32_t > siteMap;
    gkg::Reader::getInstance().read( fileNameSiteMap, siteMap );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading ODF texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameOdfs << "' : " << std::flush;

    }
    gkg::TextureMap< gkg::OrientationDistributionFunction > odfs;
    gkg::Reader::getInstance().read( fileNameOdfs, odfs );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading ROIs
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameRois << "' : " << std::flush;

    }
    gkg::SiteMap< std::string, int32_t > roisString;
    gkg::SiteMap< int16_t, int32_t > roisInt16;
    gkg::Vector3d< double > roisResolution( 1.0, 1.0, 1.0 );
    try
    {

      // first case where the file format on disk fit with a site map
      // labelled with strings
      gkg::Reader::getInstance().read( fileNameRois, roisString );
      if ( roisString.getHeader().hasAttribute( "resolutionX" ) )
      {

        roisString.getHeader().getAttribute( "resolutionX",
                                             roisResolution.x );

      }
      if ( roisString.getHeader().hasAttribute( "resolutionY" ) )
      {

        roisString.getHeader().getAttribute( "resolutionY",
                                             roisResolution.y );

      }
      if ( roisString.getHeader().hasAttribute( "resolutionZ" ) )
      {

        roisString.getHeader().getAttribute( "resolutionZ",
                                             roisResolution.z );

      }

    }
    catch ( std::exception& )
    {


      try
      {

        // first case where the file format on disk fit with a site map
        // labelled with int16_t
        gkg::Reader::getInstance().read( fileNameRois, roisInt16 );
        if ( roisInt16.getHeader().hasAttribute( "resolutionX" ) )
        {

          roisInt16.getHeader().getAttribute( "resolutionX",
                                              roisResolution.x );

        }
        if ( roisInt16.getHeader().hasAttribute( "resolutionY" ) )
        {

          roisInt16.getHeader().getAttribute( "resolutionY",
                                              roisResolution.y );

        }
        if ( roisInt16.getHeader().hasAttribute( "resolutionZ" ) )
        {

          roisInt16.getHeader().getAttribute( "resolutionZ",
                                              roisResolution.z );

        }

      }
      catch ( std::exception& )
      {


        try
        {

          // in case disk file is a volume of int16_t, convert it to a site map
          // on the fly
          gkg::Volume< int16_t > volumeOfRoisInt16;
          gkg::Reader::getInstance().read( fileNameRois, volumeOfRoisInt16 );
          gkg::EqualToFunction< int16_t > inBackgroundTest( 0 );
          roisInt16 = 
            gkg::SiteMapFactory< int16_t, int32_t >::getInstance().create(
                                                             volumeOfRoisInt16,
                                                             inBackgroundTest );
          volumeOfRoisInt16.getResolution( roisResolution ); 

        }
        catch ( std::exception& thing )
        {

          throw std::runtime_error( "unable to read ROIs" );

        }

      }

    }
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    if ( algorithmType == "global-spin-glass" )
    {

      if ( !roisString.getRanks().empty() )
      {

        if ( roisString.getRankCount() != 1 )
        {

           throw std::runtime_error(
                                    "ROI with only one rank allowed when using "
                                    "'global-spin-glass' tractography" );

        }

      }
      if ( !roisInt16.getRanks().empty() )
      {

        if ( roisInt16.getRankCount() != 1 )
        {

           throw std::runtime_error(
                                    "ROI with only one rank allowed when using "
                                    "'global-spin-glass' tractography" );

        }

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading computing mask
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "reading '" << fileNameMask << "' : " << std::flush;

    }
    gkg::Volume< int16_t > mask;

    gkg::TypedVolumeReaderProcess< int16_t >
      typedVolumeReaderProcess( mask );
    typedVolumeReaderProcess.execute( fileNameMask );

    gkg::Vector3d< int32_t > maskSize( 1, 1, 1 );
    gkg::Vector3d< double > maskResolution( 1.0, 1.0, 1.0 );
    std::set< int32_t > maskValues;

    mask.getSize( maskSize );
    mask.getResolution( maskResolution );

    gkg::EqualToFunction< int16_t > outsideBrainTest( OUTSIDE_BRAIN_ID );
    gkg::Vector3d< int32_t > site;

    for ( site.z = 0; site.z < maskSize.z; site.z++ )
    {

      for ( site.y = 0; site.y < maskSize.y; site.y++ )
      {

        for ( site.x = 0; site.x < maskSize.x; site.x++ )
        {

          const int16_t& maskValue = mask( site );
          if ( !outsideBrainTest.getState( maskValue ) )
          {

            maskValues.insert( ( int32_t )maskValue );

          }

        }

      }

    }

    int32_t allMaskValueCount = ( int32_t )maskValues.size();

    // creating bouding box for computing mask
    gkg::BoundingBox< int32_t > maskBoundingBox( mask );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    //  seeding parameter(s) check or initialization
    ////////////////////////////////////////////////////////////////////////////

    // collecting region ids ///////////////////////////////////////////////////
    // in case of unconstrained strategy
    std::vector< gkg::SeedingStrategy::RegionId > theSeedingRegionIds; 
    int32_t theMaskValueCount = 0;
    if ( seedingStrategyType == "unconstrained" )
    {

      // in case region ids is empty, we use all of the region ids
      if ( seedingRegionIds.empty() )
      {

        theSeedingRegionIds = std::vector< gkg::SeedingStrategy::RegionId >(
                                             maskValues.begin(),
                                             maskValues.end() );
        theMaskValueCount = allMaskValueCount;

      }
      // else we use the precribed ids and check that they are present in
      // the provided computing mask
      else
      {

        // sanity check(s) and collecting integer values
        std::set< gkg::SeedingStrategy::RegionId > seedingIntegerRegionIds;
        gkg::SeedingStrategy::RegionId integerRegionId = 0;
        std::vector< std::string >::const_iterator
          r = seedingRegionIds.begin(),
          re = seedingRegionIds.end();
        while ( r != re )
        {

          gkg::StringConverter::toScalar( *r, integerRegionId );
          if ( maskValues.find( integerRegionId ) != maskValues.end() )
          {

            seedingIntegerRegionIds.insert( integerRegionId );
            ++ theMaskValueCount;

          }
          else
          {

            throw std::runtime_error( std::string( "region " ) + *r + 
                                        " not present in the computing mask" );

          }
          ++ r;

        }
        theSeedingRegionIds = std::vector< gkg::SeedingStrategy::RegionId >(
                                             seedingIntegerRegionIds.begin(),
                                             seedingIntegerRegionIds.end() );

      }

    }
    // anatomically constrained strategy ///////////////////////////////////////
    else if ( seedingStrategyType == "anatomically-constrained" )
    {

      // checking that mask labels include only 1, 2, 3, 4 or 100 for specific
      // voxels 
      std::set< int32_t > wantedLabels;
      wantedLabels.insert( 1 );
      wantedLabels.insert( 2 );
      wantedLabels.insert( 3 );
      wantedLabels.insert( 4 );
      wantedLabels.insert( 100 );

      std::set< int32_t >::const_iterator
        mv = maskValues.begin(),
        mve = maskValues.end();
      while ( mv != mve )
      {

        if ( wantedLabels.find( *mv ) == wantedLabels.end() )
        {

          throw std::runtime_error(
                   gkg::StringConverter::toString( *mv ) +
                   " is not an expected mask label in anatomically-constrained "
                   "seeding strategy that is limited to 1, 2, 3, 4 mask values" );

        }
        ++ mv;

      }


      // region ids ////////////////////////////////////////////////////////////
      // in case region ids is empty, we use all of the region ids
      theMaskValueCount = 0;
      if ( seedingRegionIds.empty() )
      {

        theSeedingRegionIds.resize( 3U );
        theSeedingRegionIds[ 0 ] = CORTEX_WHITE_MATTER_INTERFACE_ID;
        theSeedingRegionIds[ 1 ] = WHITE_MATTER_ID;
        theSeedingRegionIds[ 2 ] = DEEP_GRAY_MATTER_WHITE_MATTER_INTERFACE_ID;
        theMaskValueCount = 3;

      }
      // else we use the precribed ids and check that they are present in
      // the provided computing mask
      else
      {

        // sanity check(s) and collecting integer values
        std::set< gkg::SeedingStrategy::RegionId > seedingIntegerRegionIds;
        gkg::SeedingStrategy::RegionId integerRegionId = 0;
        std::vector< std::string >::const_iterator
          r = seedingRegionIds.begin(),
          re = seedingRegionIds.end();
        while ( r != re )
        {

          if ( *r == "pial-cortex_interface" )
          {

            integerRegionId = PIAL_CORTEX_INTERFACE_ID;

          }
          else if ( *r == "cortex" )
          {

            integerRegionId = CORTEX_ID;

          }
          else if ( *r == "cortex-white-matter-interface" )
          {

            integerRegionId = CORTEX_WHITE_MATTER_INTERFACE_ID;

          }
          else if ( *r == "white-matter" )
          {

            integerRegionId = WHITE_MATTER_ID;

          }
          else if ( *r == "deep-gray-matter-white-matter-interface" )
          {

            integerRegionId = DEEP_GRAY_MATTER_WHITE_MATTER_INTERFACE_ID;

          }
          else if ( *r == "deep-gray-matter" )
          {

            integerRegionId = DEEP_GRAY_MATTER_ID;

          }
          else
          {

            throw std::runtime_error( *r +  " is not a valid region id" );

          }

          seedingIntegerRegionIds.insert( integerRegionId );
          ++ theMaskValueCount;

          ++ r;

        }
        theSeedingRegionIds = std::vector< gkg::SeedingStrategy::RegionId >(
                                             seedingIntegerRegionIds.begin(),
                                             seedingIntegerRegionIds.end() );

      }

    }
    else
    {

      throw std::runtime_error( "not a valid seeding strategy" );

    }

    // collecting the seeding sampling types ///////////////////////////////////
    std::vector< gkg::SeedingStrategy::SamplingType > theSeedingSamplingTypes;
    // if sampling types not provided, initializing them to cartesian
    if ( seedingSamplingTypes.empty() )
    {

      theSeedingSamplingTypes.resize( theMaskValueCount,
                                      gkg::SeedingStrategy::Cartesian );

    }
    // if one sampling type provided, duplicating it; else throw exception
    else if ( ( int32_t )seedingSamplingTypes.size() != theMaskValueCount )
    {

      if  ( seedingSamplingTypes.size() == 1U )
      {

        gkg::SeedingStrategy::SamplingType
          seedingSamplingType = gkg::SeedingStrategy::Cartesian;
        if ( seedingSamplingTypes[ 0 ] == "cartesian" )
        {

          seedingSamplingType = gkg::SeedingStrategy::Cartesian;

        }
        else if ( seedingSamplingTypes[ 0 ] == "random" )
        {

          seedingSamplingType = gkg::SeedingStrategy::Random;

        }
        else
        {

          throw std::runtime_error( "bad seeding sampling type" );

        }
        theSeedingSamplingTypes.resize( theMaskValueCount,
                                        seedingSamplingType );

      }
      else
      {

        throw std::runtime_error(
           std::string( "there must be 1 or " ) +
           gkg::StringConverter::toString( theMaskValueCount ) +
           " seeding sampling types" );

      }

    }
    // if all sampling types provided, check their values and recopy them
    else
    {

      theSeedingSamplingTypes.resize( theMaskValueCount );
      std::vector< std::string >::const_iterator
        si = seedingSamplingTypes.begin(),
        sie = seedingSamplingTypes.end();
      std::vector< gkg::SeedingStrategy::SamplingType >::iterator
        so = theSeedingSamplingTypes.begin();
      while ( si != sie )
      {

        if ( *si == "cartesian" )
        {

          *so = gkg::SeedingStrategy::Cartesian;

        }
        else if ( *si == "random" )
        {

          *so = gkg::SeedingStrategy::Random;

        }
        else
        {

          throw std::runtime_error( "bad seeding sampling type" );

        }
        ++ so;
        ++ si;

      }

    }

    // collecting the seeding seed per voxel counts ////////////////////////////
    std::vector< int32_t > theSeedingSeedPerVoxelCounts;
    // if seed per voxel counts not provided, initizaing them to 1
    if ( seedingSeedPerVoxelCounts.empty() )
    {

      theSeedingSeedPerVoxelCounts.resize( theMaskValueCount, 1 );

    }
    // if one seed count is provided, duplicating it; else throw exception
    else if ( ( int32_t )seedingSeedPerVoxelCounts.size() !=
                theMaskValueCount )
    {

      if  ( seedingSeedPerVoxelCounts.size() == 1U )
      {

        int32_t seedPerVoxelCount = seedingSeedPerVoxelCounts[ 0 ];
        if ( seedPerVoxelCount < 1 )
        {

          throw std::runtime_error(
                    "seed per voxel count should be  greater or equal to 1" );

        }
        theSeedingSeedPerVoxelCounts.resize( theMaskValueCount,
                                             seedPerVoxelCount );

      }
      else
      {

         throw std::runtime_error(
           std::string( "there must be 1 or " ) +
           gkg::StringConverter::toString( theMaskValueCount ) +
           " seed per voxel counts" );

      }

    }
    else
    {

      theSeedingSeedPerVoxelCounts = seedingSeedPerVoxelCounts;

      // sanity check(s)
      std::vector< int32_t >::const_iterator
        spvc = seedingSeedPerVoxelCounts.begin(),
        spvce = seedingSeedPerVoxelCounts.end();
      while ( spvc != spvce )
      {

        if ( *spvc < 1 )
        {

          throw std::runtime_error(
                    "seed per voxel count should be  greater or equal to 1" );

        }
        ++ spvc;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // building output orientation count
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building output orientation set : " << std::flush;

    }

    gkg::TextureMap< gkg::OrientationDistributionFunction >::const_iterator
      o = odfs.begin();
    if ( o == odfs.end() )
    {

      throw std::runtime_error( "ODF field is empty!" );

    }
    gkg::OrientationDistributionFunction::BasisType
      basisType = o->second.getBasisType();

    if ( basisType == gkg::OrientationDistributionFunction::SphericalHarmonics )
    {

      if ( outputOrientationCount <= 0 )
      {

        throw std::runtime_error( "missing or bad output orientation count" );

      }

    }
    else if ( basisType == gkg::OrientationDistributionFunction::Standard )
    {

      if ( outputOrientationCount > 0 )
      {

        std::cerr << std::endl <<
               "'outputOrientationCount' parameter will be ignored as ODF "
               "texture map provided has got a standard basis, and therefore a "
               "fixed output orientation count" << std::endl;

      }
      outputOrientationCount = o->second.getValueCount();

    }
    gkg::AntipodallySymmetricUniformOrientationSet
      outputOrientationSet( outputOrientationCount );

    if ( verbose )
    {

      std::cout << "done" << std::endl;
      float minimumAngle = 0.0;
      float maximumAngle = 0.0;
      float averageAngle = 0.0;
      float angleStdDev = 0.0;

      outputOrientationSet.getStatistics( &minimumAngle,
                                          &maximumAngle,
                                          &averageAngle,
                                          &angleStdDev );
      std::cout << "minimum angle : "
                << minimumAngle * 180.0 / M_PI << " deg" << std::endl;
      std::cout << "maximum angle : "
                << maximumAngle * 180.0 / M_PI << " deg" << std::endl;
      std::cout << "average angle : "
                << averageAngle * 180.0 / M_PI << " deg" << std::endl;
      std::cout << "angle standard deviation : "
                << angleStdDev * 180.0 / M_PI << " deg" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building cartesian ODF field from ODF texture map / site map
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building ODF cartesian field : " << std::flush;

    }

    gkg::OdfCartesianField* 
      odfCartesianField = new gkg::OdfCartesianField( siteMap,
                                                      odfs,
                                                      outputOrientationSet );
    gkg::Vector3d< int32_t > odfsSize = odfCartesianField->getSize();
    gkg::Vector3d< double > odfsResolution = odfCartesianField->getResolution();

    // forcing memory deallocation to preserve memory consumption
    siteMap.clear();
    odfs.clear();

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building 3D transforms between ODFs / ROIs / MASK referential(s)
    ////////////////////////////////////////////////////////////////////////////

    // creating referential(s)
    gkg::Referential* referentialOfOdfsVoxel =
                       new gkg::Referential( ODFS_VOXEL_REFERENTIAL_NAME );
    gkg::Referential* referentialOfOdfsReal =
                       new gkg::Referential( ODFS_REAL_REFERENTIAL_NAME );

    gkg::Referential* referentialOfRoisVoxel = 
                       new gkg::Referential( ROIS_VOXEL_REFERENTIAL_NAME );
    gkg::Referential* referentialOfRoisReal =
                       new gkg::Referential( ROIS_REAL_REFERENTIAL_NAME );

    gkg::Referential* referentialOfMaskVoxel =
                       new gkg::Referential( MASK_VOXEL_REFERENTIAL_NAME );
    gkg::Referential* referentialOfMaskReal =
                       new gkg::Referential( MASK_REAL_REFERENTIAL_NAME );

    // adding 3D transform from ODFs voxel to real coordinates
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromOdfsVoxelToOdfsReal( new gkg::Scaling3d< float >(
                                                  ( float )odfsResolution.x,
                                                  ( float )odfsResolution.y,
                                                  ( float )odfsResolution.z ) );
    gkg::Transform3dManager< float >::getInstance().registerTransform3d(
      referentialOfOdfsVoxel,
      referentialOfOdfsReal,
      transform3dFromOdfsVoxelToOdfsReal );

    // adding 3D transform from ROIs voxel to real coordinates
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromRoisVoxelToRoisReal ( new gkg::Scaling3d< float >(
                                                  ( float )roisResolution.x,
                                                  ( float )roisResolution.y,
                                                  ( float )roisResolution.z ) );
    gkg::Transform3dManager< float >::getInstance().registerTransform3d(
      referentialOfRoisVoxel,
      referentialOfRoisReal,
      transform3dFromRoisVoxelToRoisReal );

    // adding 3D transform from MASK voxel to real coordinates
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromMaskVoxelToMaskReal( new gkg::Scaling3d< float >(
                                                  ( float )maskResolution.x,
                                                  ( float )maskResolution.y,
                                                  ( float )maskResolution.z ) );
    gkg::Transform3dManager< float >::getInstance().registerTransform3d(
      referentialOfMaskVoxel,
      referentialOfMaskReal,
      transform3dFromMaskVoxelToMaskReal );

    // adding 3D rigid transform from ROIs to ODFs real coordinates
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromRoisRealToOdfsReal;
    if ( fileNameRoisToOdfsTransform3d.empty() )
    {

      if ( verbose )
      {

       std::cout << "initializing 3D ROIs->ODFs transform to identity : "
                 << std::flush;

      }
      // by default, initializaing to inverse scaling
      transform3dFromRoisRealToOdfsReal.reset(
          new gkg::IdentityTransform3d< float > );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading 3D ROIs->ODFs transform : " << std::flush;

      }

      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( fileNameRoisToOdfsTransform3d.c_str() );
        if ( is.good() )
        {

          rigidTransform3d->readTrm( is );
          is.close();
          transform3dFromRoisRealToOdfsReal = rigidTransform3d;

        }
        else
        {

          throw std::runtime_error( "invalid 3D ROIs->ODFs transform path" );

        }

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineWoShearingTransform3d< float > >
            affineWoShearingTransform3d(
                               new gkg::AffineWoShearingTransform3d< float > );
          std::ifstream is( fileNameRoisToOdfsTransform3d.c_str() );
          if ( is.good() )
          {

            affineWoShearingTransform3d->readTrm( is );
            is.close();
            transform3dFromRoisRealToOdfsReal = affineWoShearingTransform3d;

          }
          else
          {

            throw std::runtime_error( "invalid 3D ROIs->ODFs transform path" );

          }

        }
        catch( std::exception& )
        {

          try
          {

            gkg::RCPointer< gkg::AffineTransform3d< float > >
              affineTransform3d( new gkg::AffineTransform3d< float > );
            std::ifstream is( fileNameRoisToOdfsTransform3d.c_str() );
            if ( is.good() )
            {

              affineTransform3d->readTrm( is );
              is.close();
              transform3dFromRoisRealToOdfsReal = affineTransform3d;

            }
            else
            {

              throw std::runtime_error(
                                       "invalid 3D ROIs->ODFs transform path" );

            }

          }
          catch( std::exception& )
          {

            throw std::runtime_error( "invalid 3D ROIs->ODFs transform" );

          }

        }

      }


      if ( verbose )
      {

          std::cout << "done" << std::endl;

      }

    }
    gkg::Transform3dManager< float >::getInstance().registerTransform3d(
      referentialOfRoisReal,
      referentialOfOdfsReal,
      transform3dFromRoisRealToOdfsReal );


    // adding 3D rigid transform from MASK to ODFs real coordinates
    gkg::RCPointer< gkg::Transform3d< float > >
      transform3dFromMaskRealToOdfsReal;
    if ( fileNameMaskToOdfsTransform3d.empty() )
    {

      if ( verbose )
      {

       std::cout << "initializing 3D MASK->ODFs transform to identity : "
                 << std::flush;

      }
      // by default, initializaing to inverse scaling
      transform3dFromMaskRealToOdfsReal.reset(
          new gkg::IdentityTransform3d< float > );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

       std::cout << "reading 3D MASK->ODFs transform : " << std::flush;

      }

      try
      {

        gkg::RCPointer< gkg::RigidTransform3d< float > >
          rigidTransform3d( new gkg::RigidTransform3d< float > );
        std::ifstream is( fileNameMaskToOdfsTransform3d.c_str() );
        rigidTransform3d->readTrm( is );
        is.close();
        transform3dFromMaskRealToOdfsReal = rigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {

          gkg::RCPointer< gkg::AffineWoShearingTransform3d< float > >
            affineWoShearingTransform3d(
                                new gkg::AffineWoShearingTransform3d< float > );
          std::ifstream is( fileNameMaskToOdfsTransform3d.c_str() );
          affineWoShearingTransform3d->readTrm( is );
          is.close();
          transform3dFromMaskRealToOdfsReal = affineWoShearingTransform3d;

        }
        catch( std::exception& )
        {

          try
          {

            gkg::RCPointer< gkg::AffineTransform3d< float > >
              affineTransform3d( new gkg::AffineTransform3d< float > );
            std::ifstream is( fileNameMaskToOdfsTransform3d.c_str() );
            affineTransform3d->readTrm( is );
            is.close();
            transform3dFromMaskRealToOdfsReal = affineTransform3d;

          }
          catch( std::exception& )
          {

            throw std::runtime_error( "invalid 3D MASK->ODFs transform" );

          }

        }

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    gkg::Transform3dManager< float >::getInstance().registerTransform3d(
      referentialOfMaskReal,
      referentialOfOdfsReal,
      transform3dFromMaskRealToOdfsReal );

    // obtaining the 3D transforms required during tractography
    gkg::Transform3d< float >*
      transform3dFromRoisVoxelToOdfsReal =
        gkg::Transform3dManager< float >::getInstance().getTransform3d(
          referentialOfRoisVoxel,
          referentialOfOdfsReal );

    gkg::Transform3d< float >*
      transform3dFromRoisVoxelToMaskVoxel =
        gkg::Transform3dManager< float >::getInstance().getTransform3d(
          referentialOfRoisVoxel,
          referentialOfMaskVoxel );

    gkg::Transform3d< float >*
      transform3dFromOdfsRealToMaskVoxel =
        gkg::Transform3dManager< float >::getInstance().getTransform3d(
          referentialOfOdfsReal,
          referentialOfMaskVoxel );

    gkg::Transform3d< float >*
      transform3dFromOdfsRealToRoisVoxel =
        gkg::Transform3dManager< float >::getInstance().getTransform3d(
          referentialOfOdfsReal,
          referentialOfRoisVoxel );


    ////////////////////////////////////////////////////////////////////////////
    // updating forward step
    ////////////////////////////////////////////////////////////////////////////

    if ( algorithmType == "streamline-deterministic" )
    {

      if ( theAlgorithmScalarParameters[ 0 ] == 0.0 )
      {

        theAlgorithmScalarParameters[ 0 ] = 0.25 *
                                   std::min( std::min( odfsResolution.x,
                                                       odfsResolution.y ),
                                             odfsResolution.z );

      }

    }
    else if ( algorithmType == "streamline-regularized-deterministic" )
    {

      if ( theAlgorithmScalarParameters[ 0 ] == 0.0 )
      {

        theAlgorithmScalarParameters[ 0 ] = 0.25 *
                                   std::min( std::min( odfsResolution.x,
                                                       odfsResolution.y ),
                                             odfsResolution.z );

      }

    }
    else if ( algorithmType == "streamline-probabilistic" )
    {

      if ( theAlgorithmScalarParameters[ 0 ] == 0.0 )
      {

        theAlgorithmScalarParameters[ 0 ] = 0.25 *
                                   std::min( std::min( odfsResolution.x,
                                                       odfsResolution.y ),
                                             odfsResolution.z );

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // building continuous ODF field from ODF cartesian field
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building ODF continuous field : " << std::flush;

    }

    gkg::OdfContinuousField*
      odfContinuousField = new gkg::OdfCartesianToContinuousField(
                                                           *odfCartesianField,
                                                           false );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // building seeding strategy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building seeding strategy : " << std::flush;

    }

    // unconstrained strategy //////////////////////////////////////////////////
    gkg::RCPointer< gkg::SeedingStrategy > seedingStrategy;
    if ( seedingStrategyType == "unconstrained" )
    {

      seedingStrategy.reset( new gkg::UnconstrainedSeedingStrategy(
                                            roisResolution,
                                            mask,
                                            maskBoundingBox,
                                            theSeedingRegionIds,
                                            theSeedingSamplingTypes,
                                            theSeedingSeedPerVoxelCounts ) );

    }
    else if ( seedingStrategyType == "anatomically-constrained" )
    {

      seedingStrategy.reset( new gkg::AnatomicallyConstrainedSeedingStrategy(
                                            roisResolution,
                                            mask,
                                            maskBoundingBox,
                                            theSeedingRegionIds,
                                            theSeedingSamplingTypes,
                                            theSeedingSeedPerVoxelCounts ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building stopping strategy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building stopping strategy : " << std::flush;

    }

    // unconstrained strategy //////////////////////////////////////////////////
    gkg::RCPointer< gkg::StoppingStrategy > stoppingStrategy;
    if ( stoppingStrategyType == "mask-and-gfa" )
    {

      stoppingStrategy.reset(
         new gkg::MaskAndGFAStoppingStrategy(
              *odfContinuousField,
              maskBoundingBox,
              mask,
              *transform3dFromOdfsRealToMaskVoxel,
              ( float )theAlgorithmScalarParameters[ 0 ],
              ( float )theAlgorithmScalarParameters[ 3 ],
              theStoppingStrategyScalarParameters[ 0 ] > 0.0f ? true : false,
              ( float )theStoppingStrategyScalarParameters[ 1 ] ) );

    }
    else if ( stoppingStrategyType == "anatomically-informed" )
    {

      stoppingStrategy.reset(
         new gkg::AnatomicallyInformedStoppingStrategy(
              *odfContinuousField,
              maskBoundingBox,
              mask,
              *transform3dFromOdfsRealToMaskVoxel,
              ( float )theAlgorithmScalarParameters[ 0 ],
              ( float )theAlgorithmScalarParameters[ 3 ],
              theStoppingStrategyScalarParameters[ 0 ] > 0.0f ? true : false,
              ( float )theStoppingStrategyScalarParameters[ 1 ] ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building starting orientation strategy
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "building starting orientation strategy : " << std::flush;

    }

    // unconstrained strategy //////////////////////////////////////////////////
    gkg::RCPointer< gkg::StartingOrientationStrategy >
      startingOrientationStrategy;
    if ( startingOrientationStrategyType == "principal-orientation" )
    {

      startingOrientationStrategy.reset(
         new gkg::PrincipalStartingOrientationStrategy );

    }
    else if ( startingOrientationStrategyType == "gibbs-sampled-orientation" )
    {

      startingOrientationStrategy.reset(
         new gkg::GibbsSampledStartingOrientationStrategy(
              theStartingOrientationStrategyScalarParameters[ 0 ] ) );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // updating orientation set cache
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

     std::cout << "updating orientation set cache : " << std::flush;

    }

    float apertureAngle = ( algorithmType == "global-spin-glass" ) ?
                          theAlgorithmScalarParameters[ 13 ] * M_PI / 180.0 :
                          theAlgorithmScalarParameters[ 4 ];


    gkg::OrientationSetCache::getInstance().update(
                                   odfCartesianField->getOutputOrientationSet(),
                                   apertureAngle,
                                   true );

    if ( verbose )
    {

      std::cout << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // performing fiber tracking
    ////////////////////////////////////////////////////////////////////////////

    if ( !roisString.getRanks().empty() )
    {

      // allocating tractography algorithm
      gkg::TractographyAlgorithm< std::string >* tractographyAlgorithm =
      gkg::TractographyAlgorithmFactory< std::string >::getInstance().create(
                                                 algorithmType,
                                                 odfsSize,
                                                 odfsResolution,
                                                 theAlgorithmScalarParameters,
                                                 theAlgorithmStringParameters );

      // preparing fiber tracking
      tractographyAlgorithm->prepare( *odfCartesianField,
                                      mask,
                                      *transform3dFromRoisVoxelToOdfsReal,
                                      *transform3dFromRoisVoxelToMaskVoxel,
                                      *transform3dFromOdfsRealToMaskVoxel,
                                      *transform3dFromOdfsRealToRoisVoxel,
                                      verbose );

      // looping over step(s)
      int32_t step;
      for ( step = 1; step <= stepCount; step++ )
      {
      
        if ( verbose )
        {

          std::cout << "tracking ( step " << step << "/" << stepCount 
	            << " ) : " << std::flush;

        }

        // allocating bundle map of string labels
        gkg::BundleMap< std::string > bundleMap;

        // tracking fiber bundles
        tractographyAlgorithm->track( roisString,
                                      getSubRegions( roisString,
	                                             step,
						     stepCount ),
                                      seedingStrategy,
                                      stoppingStrategy,
                                      startingOrientationStrategy,
                                      *odfContinuousField,
                                      mask,
                                      *transform3dFromRoisVoxelToOdfsReal,
                                      *transform3dFromRoisVoxelToMaskVoxel,
                                      *transform3dFromOdfsRealToMaskVoxel,
                                      *transform3dFromOdfsRealToRoisVoxel,
                                      bundleMap,
                                      verbose );

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

        // saving bundle map
        std::string fileNameBundleMapForStep = getFileNameBundleMapForStep(
                                                              fileNameBundleMap,
                                                              step,
							      stepCount );
        if ( verbose )
        {

          std::cout << "saving bundle map '"
	            << fileNameBundleMapForStep
		    << "' : "
                    << std::flush;

        }
        bundleMap.getHeader().addAttribute( "sizeX", odfsSize.x );
        bundleMap.getHeader().addAttribute( "sizeY", odfsSize.y );
        bundleMap.getHeader().addAttribute( "sizeZ", odfsSize.z );
        bundleMap.getHeader().addAttribute( "resolutionX", odfsResolution.x );
        bundleMap.getHeader().addAttribute( "resolutionY", odfsResolution.y );
        bundleMap.getHeader().addAttribute( "resolutionZ", odfsResolution.z );
        gkg::Writer::getInstance().write( fileNameBundleMapForStep,
                                          bundleMap,
                                          ascii,
                                          bundleMapFormat );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }
	
      }

      // deleting tractography algorithm
      if ( tractographyAlgorithm )
      {

        delete tractographyAlgorithm;

      }

    }
    else if ( !roisInt16.getRanks().empty() )
    {


      // allocating tractography algorithm
      gkg::TractographyAlgorithm< int16_t >* tractographyAlgorithm =
      gkg::TractographyAlgorithmFactory< int16_t >::getInstance().create(
                                             algorithmType,
                                             odfCartesianField->getSize(),
                                             odfCartesianField->getResolution(),
                                             theAlgorithmScalarParameters,
                                             theAlgorithmStringParameters );

      // preparing fiber tracking
      tractographyAlgorithm->prepare( *odfCartesianField,
                                      mask,
                                      *transform3dFromRoisVoxelToOdfsReal,
                                      *transform3dFromRoisVoxelToMaskVoxel,
                                      *transform3dFromOdfsRealToMaskVoxel,
                                      *transform3dFromOdfsRealToRoisVoxel,
                                      verbose );

      // looping over step(s)
      int32_t step;
      for ( step = 1; step <= stepCount; step++ )
      {
      
        if ( verbose )
        {

          std::cout << "tracking ( step " << step << "/" << stepCount 
	            << " ) : " << std::flush;

        }

        // allocating bundle map of int16_t labels
        gkg::BundleMap< int16_t > bundleMap;

        // tracking fiber bundles
        tractographyAlgorithm->track( roisInt16,
                                      getSubRegions( roisInt16,
	                                             step,
                                                     stepCount ),
                                      seedingStrategy,
                                      stoppingStrategy,
                                      startingOrientationStrategy,
                                      *odfContinuousField,
                                      mask,
                                      *transform3dFromRoisVoxelToOdfsReal,
                                      *transform3dFromRoisVoxelToMaskVoxel,
                                      *transform3dFromOdfsRealToMaskVoxel,
                                      *transform3dFromOdfsRealToRoisVoxel,
                                      bundleMap,
                                      verbose );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

        // saving bundle map
        std::string fileNameBundleMapForStep = getFileNameBundleMapForStep(
                                                              fileNameBundleMap,
                                                              step,
                                                              stepCount );
        if ( verbose )
        {

          std::cout << "saving bundle map '"
                    << fileNameBundleMapForStep << "' : "
                    << std::flush;

        }
        if ( bundleMapFormat == "aimsbundlemap" )
        {


          gkg::BundleMap< std::string > bundleMapString;
          std::set< int16_t> labels = bundleMap.getLabels();
          std::set< int16_t >::const_iterator l = labels.begin(),
                                              le = labels.end();
          while ( l != le )
          {

            bundleMapString.addBundle( gkg::StringConverter::toString( *l ),
                                       bundleMap.getBundle( *l ) );
            ++ l;

          }

          bundleMapString.getHeader().addAttribute( "sizeX", odfsSize.x );
          bundleMapString.getHeader().addAttribute( "sizeY", odfsSize.y );
          bundleMapString.getHeader().addAttribute( "sizeZ", odfsSize.z );
          bundleMapString.getHeader().addAttribute( "resolutionX",
                                                    odfsResolution.x );
          bundleMapString.getHeader().addAttribute( "resolutionY",
                                                    odfsResolution.y );
          bundleMapString.getHeader().addAttribute( "resolutionZ",
                                                    odfsResolution.z );
          gkg::Writer::getInstance().write( fileNameBundleMapForStep,
	                                    bundleMapString,
                                            ascii,
                                            bundleMapFormat );

        }
        else
        {

          bundleMap.getHeader().addAttribute( "sizeX", odfsSize.x );
          bundleMap.getHeader().addAttribute( "sizeY", odfsSize.y );
          bundleMap.getHeader().addAttribute( "sizeZ", odfsSize.z );
          bundleMap.getHeader().addAttribute( "resolutionX", odfsResolution.x );
          bundleMap.getHeader().addAttribute( "resolutionY", odfsResolution.y );
          bundleMap.getHeader().addAttribute( "resolutionZ", odfsResolution.z );
          gkg::Writer::getInstance().write( fileNameBundleMapForStep,
	                                    bundleMap,
                                            ascii,
                                            bundleMapFormat );

        }

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }
	
      }

      // deleting tractography algorithm
      if ( tractographyAlgorithm )
      {

        delete tractographyAlgorithm;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // deleting continuous ODF field
    ////////////////////////////////////////////////////////////////////////////

    if ( odfContinuousField )
    {

      delete odfContinuousField;

    }


    ////////////////////////////////////////////////////////////////////////////
    // deleting cartesian ODF field
    ////////////////////////////////////////////////////////////////////////////

    if ( odfCartesianField )
    {

      delete odfCartesianField;

    }

    ////////////////////////////////////////////////////////////////////////////
    // deleting 3D transform
    ////////////////////////////////////////////////////////////////////////////

    gkg::Transform3dManager< float >::getInstance().unregisterTransform3d(
      referentialOfOdfsVoxel,
      referentialOfOdfsReal );
    gkg::Transform3dManager< float >::getInstance().unregisterTransform3d(
      referentialOfRoisVoxel,
      referentialOfRoisReal );
    gkg::Transform3dManager< float >::getInstance().unregisterTransform3d(
      referentialOfMaskVoxel,
      referentialOfMaskReal );
    gkg::Transform3dManager< float >::getInstance().unregisterTransform3d(
      referentialOfRoisReal,
      referentialOfOdfsReal );
    gkg::Transform3dManager< float >::getInstance().unregisterTransform3d(
      referentialOfMaskReal,
      referentialOfOdfsReal );


    if ( referentialOfOdfsVoxel )
    {

      delete referentialOfOdfsVoxel;

    }
    if ( referentialOfOdfsReal )
    {

      delete referentialOfOdfsReal;

    }
    if ( referentialOfRoisVoxel )
    {

      delete referentialOfRoisVoxel;

    }
    if ( referentialOfRoisReal )
    {

      delete referentialOfRoisReal;

    }
    if ( referentialOfMaskVoxel )
    {

      delete referentialOfMaskVoxel;

    }
    if ( referentialOfMaskReal )
    {

      delete referentialOfMaskReal;

    }

  }
  GKG_CATCH( "void gkg::DwiTractographyCommand::execute( "
             "const std::string& fileNameSiteMap, "
             "const std::string& fileNameOdfs, "
             "const std::string& fileNameRois, "
             "const std::string& fileNameMask, "
             "const std::string& fileNameRoisToOdfsTransform3d, "
             "const std::string& fileNameMaskToOdfsTransform3d, "
             "const std::string& fileNameBundleMap, "
             "const std::string& algorithmType, "
             "const std::vector< double >& algorithmScalarParameters, "
             "const std::vector< std::string >& algorithmStringParameters, "
             "const std::string& seedingStrategyType, "
             "const std::vector< std::string >& seedingRegionIds, "
             "const std::vector< std::string >& seedingSamplingTypes, "
             "const std::vector< int32_t >& seedingSeedPerVoxelCounts, "
             "const std::string& stoppingStrategyType, "
             "const std::vector< double >& stoppingStrategyScalarParameters, "
             "const std::string& startingOrientationStrategyType, "
             "const std::vector< double >& "
             "startingOrientationStrategyScalarParameters, "
             "int32_t outputOrientationCount, "
             "const std::string& /* volumeFormat */, "
             "const std::string& bundleMapFormat, "
             "int32_t stepCount, "
             "bool ascii, "
             "bool verbose )" );

}


RegisterCommandCreator(
    DwiTractographyCommand,
    DECLARE_STRING_PARAMETER_HELP( fileNameSiteMap ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameOdfs ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameRois ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMask ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameRoisToOdfsTransform3d ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameMaskToOdfsTransform3d ) +
    DECLARE_STRING_PARAMETER_HELP( fileNameBundleMap ) +
    DECLARE_STRING_PARAMETER_HELP( algorithmType ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( algorithmScalarParameters ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( algorithmStringParameters ) +
    DECLARE_STRING_PARAMETER_HELP( seedingStrategyType ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( seedingRegionIds ) +
    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( seedingSamplingTypes ) +
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( seedingSeedPerVoxelCounts ) +
    DECLARE_STRING_PARAMETER_HELP( stoppingStrategyType ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( 
                                            stoppingStrategyScalarParameters ) +
    DECLARE_STRING_PARAMETER_HELP( startingOrientationStrategyType ) +
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( 
                                 startingOrientationStrategyScalarParameters ) +
    DECLARE_INTEGER_PARAMETER_HELP( outputOrientationCount ) +
    DECLARE_STRING_PARAMETER_HELP( volumeFormat ) +
    DECLARE_STRING_PARAMETER_HELP( bundleMapFormat ) +
    DECLARE_INTEGER_PARAMETER_HELP( stepCount ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );


#undef ODFS_VOXEL_REFERENTIAL_NAME
#undef ODFS_REAL_REFERENTIAL_NAME 

#undef ROIS_VOXEL_REFERENTIAL_NAME
#undef ROIS_REAL_REFERENTIAL_NAME 

#undef MASK_VOXEL_REFERENTIAL_NAME
#undef MASK_REAL_REFERENTIAL_NAME 

