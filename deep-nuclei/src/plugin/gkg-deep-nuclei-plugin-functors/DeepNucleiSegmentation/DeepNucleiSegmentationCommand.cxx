#include <gkg-deep-nuclei-plugin-functors/DeepNucleiSegmentation/DeepNucleiSegmentationCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-transform/AcPcIhCoordinates.h>
#include <gkg-processing-transform/TalairachTransform3d_i.h>
#include <gkg-processing-histogram/HistogramAnalysis_i.h>
#include <gkg-processing-roi/AtlasInformation_i.h>
#include <gkg-processing-regiongrowing/ProbabilisticRoiSegmentation_i.h>
#include <gkg-processing-regiongrowing/ICMRegionGrowingOptimizer_i.h>
#include <gkg-processing-regiongrowing/SimulatedAnnealingRegionGrowingOptimizer_i.h>
#include <gkg-deep-nuclei-deepnuclei/CsfNormalizedSquareCenteredPhiFunction.h>
#include <gkg-deep-nuclei-deepnuclei/BackgroundNormalizedSquareCenteredPhiFunction.h>
#include <gkg-processing-numericalanalysis/PhiFunction.h>
#include <gkg-deep-nuclei-deepnuclei/LabelIndependentTransform3dRelaxationFunction_i.h>
#include <gkg-deep-nuclei-deepnuclei/LabelDependentTransform3dRelaxationFunction_i.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-exception/Exception.h>

#include <map>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>


gkg::DeepNucleiSegmentationCommand::DeepNucleiSegmentationCommand(
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
  GKG_CATCH( 
           "gkg::DeepNucleiSegmentationCommand::DeepNucleiSegmentationCommand( "
           "int32_t argc, char* argv[], bool loadPlugin, bool removeFirst )" );

}


gkg::DeepNucleiSegmentationCommand::DeepNucleiSegmentationCommand(
                     const std::vector< std::string >& contrastNames,
                     const std::vector< std::string >& atlasFileNames, 
                     const std::vector< std::string >& contrastFileNames,
                     std::vector< double >& allWeights,
                     std::vector< int32_t >& strides,
                     const std::vector< int32_t >& schemeIterationCounts,
                     const std::vector< int32_t >& regionGrowingIterationCounts,
                     const std::vector< int32_t >& transform3dIterationCounts,
                     bool sameTransformAcrossLabels,
                     const std::string& fileNameOut,
                     double maximumTestSize,
                     const std::vector< float >& deltaParameters,
                     int32_t storagePeriod,
                     int32_t displayPeriod,
                     bool verbose )
                                  : gkg::Command()
{

  try
  {

    execute( contrastNames, atlasFileNames, contrastFileNames, allWeights,
             strides, schemeIterationCounts, regionGrowingIterationCounts, 
             transform3dIterationCounts, sameTransformAcrossLabels, fileNameOut,
             maximumTestSize, deltaParameters, storagePeriod, displayPeriod,
             verbose );

  }
  GKG_CATCH( 
         "gkg::DeepNucleiSegmentationCommand::DeepNucleiSegmentationCommand("
         "const std::vector< std::string >& contrastNames, "
         "const std::vector< std::string >& atlasFileNames, "
         "const std::vector< std::string >& contrastFileNames, "
         "std::vector< double >& allWeights, "
         "std::vector< int32_t >& strides, "
         "const std::vector< int32_t >& schemeIterationCounts, "
         "const std::vector< int32_t >& regionGrowingIterationCounts, "
         "const std::vector< int32_t >& transform3dIterationCounts, "
         "bool sameTransformAcrossLabels, const std::string& fileNameOut, "
         "double maximumTestSize, const std::vector< float >& deltaParameters, "
         "int32_t storagePeriod, int32_t displayPeriod, bool verbose )" );

}


gkg::DeepNucleiSegmentationCommand::DeepNucleiSegmentationCommand(
                                             const gkg::Dictionary& parameters )
                                  : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         contrastNames );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         atlasFileNames );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         contrastFileNames );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< double >,
                                           allWeights );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          strides );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          schemeIterationCounts );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          regionGrowingIterationCounts );
    DECLARE_VECTOR_OF_INTEGERS_PARAMETER( parameters,
                                          std::vector< int32_t >,
                                          transform3dIterationCounts );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, sameTransformAcrossLabels );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_FLOATING_PARAMETER( parameters, double, maximumTestSize );
    DECLARE_VECTOR_OF_FLOATINGS_PARAMETER( parameters,
                                           std::vector< float >,
                                           deltaParameters );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, storagePeriod );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, displayPeriod );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( contrastNames, atlasFileNames, contrastFileNames, allWeights,
             strides, schemeIterationCounts, regionGrowingIterationCounts, 
             transform3dIterationCounts, sameTransformAcrossLabels, fileNameOut,
             maximumTestSize, deltaParameters, storagePeriod, displayPeriod,
             verbose );

  }
  GKG_CATCH( "gkg::DeepNucleiSegmentationCommand::"
             "DeepNucleiSegmentationCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::DeepNucleiSegmentationCommand::~DeepNucleiSegmentationCommand()
{
}


std::string gkg::DeepNucleiSegmentationCommand::getStaticName()
{

  try
  {

    return "DeepNucleiSegmentation";

  }
  GKG_CATCH( "std::string "
             "gkg::DeepNucleiSegmentationCommand::getStaticName()" );

}


void gkg::DeepNucleiSegmentationCommand::parse()
{

  try
  {

    std::vector< std::string > contrastNames;
    std::vector< std::string > atlasFileNames;
    std::vector< std::string > contrastFileNames;
    std::vector< double > allWeights;
    std::vector< int32_t > schemeIterationCounts;
    std::vector< int32_t > regionGrowingIterationCounts;
    std::vector< int32_t > transform3dIterationCounts;
    std::vector< int32_t > strides;
    bool sameTransformAcrossLabels = true;
    std::string fileNameOut;
    double maximumTestSize = 0.01;
    std::vector< float > deltaParameters;
    int32_t storagePeriod = 0;
    int32_t displayPeriod = 0;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Deep nuclei segmentation tool",
                                  _loadPlugin );
    application.addSeriesOption( "-c",
                                 "Contrast names",
                                 contrastNames,
                                 1 );
    application.addSeriesOption( "-a",
                                 "Input atlas volume file names",
                                 atlasFileNames,
                                 1 );
    application.addSeriesOption( "-i",
                                 "Input contrast volume file names",
                                 contrastFileNames,
                                 1 );
    application.addSeriesOption( "-w",
                                 "Potential weights for all contrasts "
                                 "in the following order: \n"
                                 "<data_attachment> <volume> <surface> "
                                 "< front_potts> < probabilistic_atlas> "
                                 "(default for T1=2.0/14000.0/11000.0/0.4/5.0",
                                 allWeights );
    application.addSeriesOption( "-s",
                                 "List of strides corresponding to the"
                                 "successive multi-resolution factors",
                                 strides,
                                 1 );
    application.addSeriesOption( "-schemeIterationCounts",
                                 "Number of times the double region growing / "
                                 "3D transform optimization scheme must be "
                                 "performed during the different strides",
                                 schemeIterationCounts,
                                 1 );
    application.addSeriesOption( "-regionGrowingIterationCounts",
                                 "Iteration counts to be performed during "
                                 "region growing corresponding to the different"
                                 " strides",
                                 regionGrowingIterationCounts,
                                 1 );
    application.addSeriesOption( "-transform3dIterationCounts",
                                 "Iteration counts to be performed during "
                                 "transform optimization "
                                 "corresponding to the different strides",
                                 transform3dIterationCounts,
                                 1 );
    application.addSingleOption( "-sameTransformAcrossLabels",
                                 "Apply the same 3D transform to all the labels"
                                 " for a given contrast (default=true)",
                                 sameTransformAcrossLabels,
                                 true );
    application.addSingleOption( "-o",
                                 "Output label volume file name",
                                 fileNameOut );
    application.addSingleOption( "-maximumTestSize",
                                 "Stopping criterion error of the Nelder-Mead "
                                 "Simplex optimizer (default=1e-2)",
                                 maximumTestSize,
                                 true );
    application.addSeriesOption( "-deltaParameters",
                                 "Variation of 3D tranform parameters:\n"
                                 " - scaling along X (default=0.05),\n"
                                 " - scaling along Y (default=0.05),\n"
                                 " - scaling along Z (default=0.05),\n"
                                 " - rotation around X (in degrees,"
                                 " default=10 degree),\n"
                                 " - rotation around Y (in degrees,"
                                 " default=10 degree),\n"
                                 " - rotation around Z (in degrees,"
                                 " default=10 degree),\n"
                                 " - translation along X (in mm,"
                                 " default=10),\n"
                                 " - translation along Y (in mm,"
                                 " default=10),\n"
                                 " - translation along Z (in mm,"
                                 " default=10),\n",
                                 deltaParameters,
                                 0, 9 );
     
    application.addSingleOption( "-matplotlib",
                                 "Register the results every N iterations and "
                                 "creates a matplotlib compatible python file, "
                                 "N being provided to this option "
                                 "(default=0->not activated)",
                                 storagePeriod, 
                                 true );
    application.addSingleOption( "-displayPeriod",
                                 "Display energy on a terminal every P "
                                 "iterations, P being provided to this "
                                 "option (default=0->not activated)",
                                 displayPeriod,
                                 true );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );
    application.initialize();

    execute( contrastNames, atlasFileNames, contrastFileNames, allWeights,
             strides, schemeIterationCounts, regionGrowingIterationCounts, 
             transform3dIterationCounts, sameTransformAcrossLabels, fileNameOut,
             maximumTestSize, deltaParameters, storagePeriod, displayPeriod,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::DeepNucleiSegmentationCommand::parse()" );

}


void gkg::DeepNucleiSegmentationCommand::execute(
                     const std::vector< std::string >& contrastNames,
                     const std::vector< std::string >& atlasFileNames, 
                     const std::vector< std::string >& contrastFileNames,
                     std::vector< double >& allWeights,
                     std::vector< int32_t >& strides,
                     const std::vector< int32_t >& schemeIterationCounts,
                     const std::vector< int32_t >& regionGrowingIterationCounts,
                     const std::vector< int32_t >& transform3dIterationCounts,
                     bool sameTransformAcrossLabels,
                     const std::string& fileNameOut,
                     double maximumTestSize,
                     const std::vector< float >& deltaParameters,
                     int32_t storagePeriod,
                     int32_t displayPeriod,
                     bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    int32_t strideCount = ( int32_t )strides.size();
    int32_t s = 0;

    if ( allWeights.empty() )
    {

      allWeights.resize( 5 * strideCount );
      for ( s = 0; s < strideCount; s++ )
      {

        allWeights[ 0 + 5 * s ] = 2.0;
        allWeights[ 1 + 5 * s ] = 14000.0;
        allWeights[ 2 + 5 * s ] = 11000.0;
        allWeights[ 3 + 5 * s ] = 0.4;
        allWeights[ 4 + 5 * s ] = 5.0;

      }

    }

    if ( !deltaParameters.empty() && ( deltaParameters.size() != 9U ) )
    {

      throw std::runtime_error(
                              "option '-deltaParameters': must have 9 values" );

    }

    if ( ( int32_t )schemeIterationCounts.size() != strideCount )
    {

      throw std::runtime_error( "bad '-schemeIterationCounts' value" );

    }

    if ( ( int32_t )regionGrowingIterationCounts.size() != strideCount )
    {

      throw std::runtime_error( "bad '-regionGrowingIterationCounts' value" );

    }

    if ( ( int32_t )transform3dIterationCounts.size() != strideCount )
    {

      throw std::runtime_error( "bad '-transform3dIterationCounts' value" );

    }

    gkg::Vector vectorOfDeltaParameters( 9 );
    if ( deltaParameters.empty() )
    {

      vectorOfDeltaParameters( 0 ) = 0.05;
      vectorOfDeltaParameters( 1 ) = 0.05;
      vectorOfDeltaParameters( 2 ) = 0.05;
      vectorOfDeltaParameters( 3 ) = M_PI / 18;
      vectorOfDeltaParameters( 4 ) = M_PI / 18;
      vectorOfDeltaParameters( 5 ) = M_PI / 18;
      vectorOfDeltaParameters( 6 ) = 10.0;
      vectorOfDeltaParameters( 7 ) = 10.0;
      vectorOfDeltaParameters( 8 ) = 10.0;

    }
    else
    {

      vectorOfDeltaParameters( 0 ) = deltaParameters[ 0 ];
      vectorOfDeltaParameters( 1 ) = deltaParameters[ 1 ];
      vectorOfDeltaParameters( 2 ) = deltaParameters[ 2 ];
      vectorOfDeltaParameters( 3 ) = deltaParameters[ 3 ] * M_PI / 180.0;
      vectorOfDeltaParameters( 4 ) = deltaParameters[ 4 ] * M_PI / 180.0;
      vectorOfDeltaParameters( 5 ) = deltaParameters[ 5 ] * M_PI / 180.0;
      vectorOfDeltaParameters( 6 ) = deltaParameters[ 6 ];
      vectorOfDeltaParameters( 7 ) = deltaParameters[ 7 ];
      vectorOfDeltaParameters( 8 ) = deltaParameters[ 8 ];

    }


    /////////////////////////////////////////////////////////////////////////
    // getting numerical analysis implementation factory
    /////////////////////////////////////////////////////////////////////////
    gkg::NumericalAnalysisImplementationFactory*
      factory = gkg::NumericalAnalysisSelector::getInstance().
                                                  getImplementationFactory();


    ////////////////////////////////////////////////////////////////////////////
    // reading data
    ////////////////////////////////////////////////////////////////////////////

    std::map< std::string,
              gkg::RCPointer< gkg::Volume< int16_t > > > atlases;
    std::map< std::string,
              gkg::RCPointer< gkg::Volume< int16_t > > > contrasts; 
    std::map< std::string,
              std::map< int16_t, 
                        gkg::RCPointer< gkg::PhiFunction > > >
                                                     dataAttachmentPhiFunctions;
    std::map< std::string,
              std::map< int16_t, 
                        gkg::RCPointer< gkg::Transform3d< float > > > >
                                                       labelToAtlasTransform3ds;
    std::map< std::string,
              std::map< int16_t, 
                        gkg::RCPointer<
                                    gkg::ParameterizedTransform3d< float > > > >
                                          labelToAtlasParameterizedTransform3ds;

    std::vector< std::map< std::string,
                           std::map< std::string, double > > >
      weights( strideCount );

    std::vector< std::string >::const_iterator
      n = contrastNames.begin(),
      ne = contrastNames.end();

    std::vector< std::string >::const_iterator
      a = atlasFileNames.begin();

    std::vector< std::string >::const_iterator
      c = contrastFileNames.begin(); 

    std::vector< double >::const_iterator
      w = allWeights.begin();

    while ( n != ne )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading atlas volume
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading atlas volume '" << *a << "' for "
                  << *n << " contrast : "
                  << std::flush;

      }
      gkg::RCPointer< gkg::Volume< int16_t > >
        atlas( new gkg::Volume< int16_t > );
      gkg::Reader::getInstance().read( *a, *atlas );
      atlases[ *n ] = atlas;
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading contrast volume
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading contrast volume '" << *c << "' for "
                  << *n << " contrast : "
                  << std::flush;

      }
      gkg::RCPointer< gkg::Volume< int16_t > >
        contrast( new gkg::Volume< int16_t > );
      gkg::Reader::getInstance().read( *c, *contrast );
      contrasts[ *n ] = contrast;
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading weights
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading weights for "
                  << *n << " contrast : "
                  << std::flush;

      }

      for ( s = 0; s < strideCount; s++ )
      {

        std::map< std::string, double > potentialWeights;
        potentialWeights[ "data_attachment" ]     = *w;     ++w;
        potentialWeights[ "volume" ]              = *w;     ++w;
        potentialWeights[ "surface" ]             = *w;     ++w;
        potentialWeights[ "front_potts" ]         = *w;     ++w;
        potentialWeights[ "probabilistic_atlas" ] = *w;     ++w;
        weights[ s ][ *n ] = potentialWeights;

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading phi functions
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "building phi functions for "
                  << *n << " contrast : "
                  << std::flush;

      }
      std::map< int16_t, gkg::RCPointer< gkg::PhiFunction > > phiFunctions;

      // obtaining atlas information
      gkg::AtlasInformation< std::string >
        atlasInformation( *atlas, "deep_nuclei_dictionary", *n );

      // getting structure based statistical map
      const gkg::StructureBasedStatisticalMap< std::string >&
        structureBasedStatisticalMap = 
                             atlasInformation.getStructureBasedStatisticalMap();

      // getting labelled ROI dictionary
      const gkg::LabelledRoiDictionary&
        labelledRoiDictionary = 
                             atlasInformation.getLabelledRoiDictionary();

      // getting dictionary
      gkg::Dictionary dictionary = labelledRoiDictionary.getDictionary();

      // obtaining histogram analysis from contrast
      gkg::HistogramAnalysis histogramAnalysis;
      histogramAnalysis = gkg::HistogramAnalysis( *contrast );

      // looping over ROI(s)
      std::string roiName;
      gkg::Dictionary::const_iterator
        r = dictionary.begin(),
        re = dictionary.end();
      while ( r != re )
      {

        // obtaining ROI name
        roiName = r->first;

        // obtaining label for the current ROI
        int16_t label = 0;
        labelledRoiDictionary.getRoiLabel( roiName, label );

        if ( label != BACKGROUND_LABEL )
        {

          double mean = 0.0;
          double standardDeviation = 0.0;

          if ( *n == "t1" )
          {

            double alpha = 
              structureBasedStatisticalMap.getStatisticalParameterMap(
                roiName, "gray_level_alpha" ).getStatisticalParameter( "mean" );
            double beta =
              structureBasedStatisticalMap.getStatisticalParameterMap(
                roiName, "gray_level_beta" ).getStatisticalParameter( "mean" );

          // in order to compute the phi function we must compute the mean and 
          // standard deviation
          mean =
            alpha * histogramAnalysis.getWhiteMatterStatistics().mean +
            ( 1.0 - alpha ) * histogramAnalysis.getGrayMatterStatistics().mean;
          standardDeviation =
            ( beta / 2.0 ) *
            ( histogramAnalysis.getWhiteMatterStatistics().standardDeviation +
              histogramAnalysis.getGrayMatterStatistics().standardDeviation );

          }

          if ( *n != "t1" )
          {

            mean =
              structureBasedStatisticalMap.getStatisticalParameterMap(
                roiName, "gray_level_mean" ).getStatisticalParameter( "mean" );
            standardDeviation =
              structureBasedStatisticalMap.getStatisticalParameterMap(
                roiName,
                "gray_level_standard_deviation" ).getStatisticalParameter(
                                                                       "mean" );

          }

          // if the ROI is not in LCR
          if ( ( *n == "t1" )&&
               ( label != 1000 ) &&
               ( label != 2000 ) &&
               ( label != 3000 ) &&
               ( label != 3001 ) )
          {

            phiFunctions[ label ] =
              gkg::RCPointer< gkg::NormalizedSquaredCenterPhiFunction >(
                new gkg::NormalizedSquaredCenterPhiFunction(
                                                          mean,
                                                          standardDeviation ) );

          }
          // else if the ROI is in LCR
          else
          {

            phiFunctions[ label ] =
              gkg::RCPointer< gkg::CsfNormalizedSquareCenteredPhiFunction >(
                new gkg::CsfNormalizedSquareCenteredPhiFunction(
                                                          mean,
                                                          standardDeviation ) );

          }

        }
        else
        {

          phiFunctions[ BACKGROUND_LABEL ] = 
          gkg::RCPointer< gkg::BackgroundNormalizedSquareCenteredPhiFunction >(
              new gkg::BackgroundNormalizedSquareCenteredPhiFunction(
                histogramAnalysis.getGrayMatterStatistics().mean,
                histogramAnalysis.getGrayMatterStatistics().standardDeviation,
                histogramAnalysis.getWhiteMatterStatistics().mean,
                histogramAnalysis.getWhiteMatterStatistics().standardDeviation )
                                                                    );


        }
        ++ r;

      }
      dataAttachmentPhiFunctions[ *n ] = phiFunctions;
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // building 3D Talairach transforms
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "building 3D Talairach transforms for "
                  << *n << " contrast : "
                  << std::flush;

      }
      std::map< int16_t,
                gkg::RCPointer< gkg::Transform3d< float > > >
        transform3ds;
      std::map< int16_t,
                gkg::RCPointer< gkg::ParameterizedTransform3d< float > > >
        parameterizedTransform3ds;

      if ( sameTransformAcrossLabels )
      {

        gkg::TalairachTransform3d< float > talairachTransform3d( *contrast,
                                                                 *atlas );


        gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< float > >
          parameterizedAffineWoShearingTransform3d =
            talairachTransform3d.getParameterizedAffineWoShearingTransform3d();

        // looping over ROI(s)
        r = dictionary.begin();
        while ( r != re )
        {

          // obtaining ROI name
          roiName = r->first;

          // obtaining label for the current ROI
          int16_t label = 0;
          labelledRoiDictionary.getRoiLabel( roiName, label );

          parameterizedTransform3ds[ label ] =
                                       parameterizedAffineWoShearingTransform3d;
          transform3ds[ label ] = parameterizedAffineWoShearingTransform3d;

          ++ r;

        }

      }
      else
      {

        // looping over ROI(s)
        r = dictionary.begin();
        while ( r != re )
        {

          // obtaining ROI name
          roiName = r->first;

          // obtaining label for the current ROI
          int16_t label = 0;
          labelledRoiDictionary.getRoiLabel( roiName, label );

          gkg::TalairachTransform3d< float > talairachTransform3d( *contrast,
                                                                   *atlas );


          gkg::RCPointer<
                        gkg::ParameterizedAffineWoShearingTransform3d< float > >
            parameterizedAffineWoShearingTransform3d =
             talairachTransform3d.getParameterizedAffineWoShearingTransform3d();

          parameterizedTransform3ds[ label ] =
                                       parameterizedAffineWoShearingTransform3d;
          transform3ds[ label ] = parameterizedAffineWoShearingTransform3d;

          ++ r;

        }

      }
      labelToAtlasTransform3ds[ *n ] = transform3ds;
      labelToAtlasParameterizedTransform3ds[ *n ] = parameterizedTransform3ds;


      

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

      ++ n;
      ++ a;
      ++ c;

    }

    gkg::RCPointer< gkg::Volume< int16_t > > labelVolume;
    std::ofstream valueOs;
    std::ofstream deltaValueOs;
    int32_t schemeIteration = 0;
    for( s = 0; s < strideCount; s++ )
    {

      for ( schemeIteration = 0;
            schemeIteration < schemeIterationCounts[ s ];
            schemeIteration++ )

      {

        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //
        // REGION GROWING OPTIMIZATION
        //
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        

        // allocating a probabilistic ROI segmentation smart pointer
        gkg::RCPointer< gkg::ProbabilisticRoiSegmentation< int16_t,
                                                           int16_t,
                                                           int16_t > >
          probabilisticRoiSegmentation;

        // collecting current stride value
        int32_t stride = strides[ s ];

        ////////////////////////////////////////////////////////////////////////
        // opening file stream(s) for matplotlib plot(s)
        ////////////////////////////////////////////////////////////////////////
        if ( storagePeriod > 0 )
        {

          valueOs.open( ( fileNameOut + "_value_stride" +
                          gkg::StringConverter::toString( s ) + '-' +
                          gkg::StringConverter::toString( stride )  +
                          "_scheme_iteration_" +
                          gkg::StringConverter::toString( schemeIteration )  +
                          ".py" ).c_str() );
          deltaValueOs.open( ( fileNameOut + "_delta_value_stride" +
                               gkg::StringConverter::toString( s ) + '-' +
                               gkg::StringConverter::toString( stride )  +
                               "_scheme_iteration_" +
                               gkg::StringConverter::toString(
                                                             schemeIteration ) +
                               ".py" ).c_str() );

        }


        ////////////////////////////////////////////////////////////////////////
        // creating a probabilistic ROI segmentation
        ////////////////////////////////////////////////////////////////////////

        if ( verbose )
        {

          std::cout << "creating a ROI segmentation tool (stride="
                    << stride << ", scheme_iteration="
                    << schemeIteration + 1 << ") : "
                    << std::flush;

        }
        if ( ( s == 0 ) && ( schemeIteration == 0 ) )
        {

          probabilisticRoiSegmentation.reset(
              new gkg::ProbabilisticRoiSegmentation< int16_t,
                                                     int16_t,
                                                     int16_t >(
                                                     "deep_nuclei_dictionary",
                                                     atlases,
                                                     contrasts,
                                                     dataAttachmentPhiFunctions,
                                                     labelToAtlasTransform3ds,
                                                     weights[ s ],
                                                     stride,
                                                     verbose ) );

        }
        else
        {

          probabilisticRoiSegmentation.reset(
              new gkg::ProbabilisticRoiSegmentation< int16_t,
                                                     int16_t,
                                                     int16_t >(
                                                     labelVolume,
                                                     "deep_nuclei_dictionary",
                                                     atlases,
                                                     contrasts,
                                                     dataAttachmentPhiFunctions,
                                                     labelToAtlasTransform3ds,
                                                     weights[ s ],
                                                     stride,
                                                     verbose ) );


        }

        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }


        ////////////////////////////////////////////////////////////////////////
        // saving initial solution
        ////////////////////////////////////////////////////////////////////////

        if ( ( s == 0 ) && ( schemeIteration == 0 ) )
        {

          if ( verbose )
          {

            std::cout << "writing initial label volume : " << std::flush;

          }
          labelVolume = probabilisticRoiSegmentation->getLabelVolume();
          gkg::Writer::getInstance().write( fileNameOut + "_initial",
                                            *labelVolume );
          if ( verbose )
          {

            std::cout << "done" << std::endl;

          }

        }


        ////////////////////////////////////////////////////////////////////////
        // creating a region growing ICM optimizer to optimize
        ////////////////////////////////////////////////////////////////////////

        if ( verbose )
        {

          std::cout << "creating an ICM optimizer (stride="
                    << stride << ", scheme_iteration="
                    << schemeIteration + 1 << ") : "
                    << std::flush;

        }
        if ( verbose && displayPeriod )
        {

          std::cout << std::endl;

        }
        gkg::ICMRegionGrowingOptimizer< int16_t >
          icmRegionGrowingOptimizer( probabilisticRoiSegmentation,
                                     verbose );

        //double oldEnergy = 0.0;
        //double newEnergy = 0.0;

        gkg::RCPointer< gkg::RegionGrowingEnergy< int16_t > >
          energy = probabilisticRoiSegmentation->getEnergy();

        if ( storagePeriod > 0 )
        {

          energy->writeMatPlotLibProlog( valueOs );
          energy->writeMatPlotLibProlog( deltaValueOs );

        }

        int32_t regionGrowingIteration = 0;
        int32_t regionGrowingIterationCount = regionGrowingIterationCounts[ s ];
        for ( regionGrowingIteration = 0;
              regionGrowingIteration < regionGrowingIterationCount;
              regionGrowingIteration++ )
        {

          if ( verbose && !displayPeriod )
          {

            if ( ( ( regionGrowingIteration + 1 ) % 1000 == 0 ) ||
                 ( regionGrowingIteration == 0 ) ||
                 ( regionGrowingIteration == regionGrowingIterationCount - 1 ) )
            {

              if ( regionGrowingIteration )
              {

                std::cout << gkg::Eraser( 29 );

              }
              std::cout << " [ " << std::setw( 10 )
                        << regionGrowingIteration + 1
                        << " / " << std::setw( 10 )
                        << regionGrowingIterationCount
                        << " ] " << std::flush;

            }

          }
          //oldEnergy = energy->getValue();
          icmRegionGrowingOptimizer.iterate();
          //newEnergy = energy->getValue();

          if ( storagePeriod > 0 )
          {

            if ( ( ( regionGrowingIteration + 1 ) % storagePeriod == 0 ) ||
                 ( regionGrowingIteration == 0 ) ||
                 ( regionGrowingIteration == regionGrowingIterationCount - 1 ) )
            {

              energy->writeMatPlotLibCurrentEnergy( regionGrowingIteration,
                                                    valueOs,
                                                    deltaValueOs );

            }

          }
          if ( verbose && displayPeriod )
          {

            if ( ( ( regionGrowingIteration + 1 ) % displayPeriod == 0 ) ||
                 ( regionGrowingIteration == 0 ) ||
                 ( regionGrowingIteration == regionGrowingIterationCount - 1 ) )
            {

              energy->displayCurrentEnergy( regionGrowingIteration );

            }

          }

        }

        if ( storagePeriod > 0 )
        {

          energy->writeMatPlotLibEpilog( valueOs );
          energy->writeMatPlotLibEpilog( deltaValueOs );

        }

        if ( verbose && !displayPeriod )
        {

          std::cout << std::endl;

        }


        ////////////////////////////////////////////////////////////////////////
        // collecting the label volume result after optimization
        ////////////////////////////////////////////////////////////////////////

        if ( verbose )
        {

          std::cout << "collecting the label volume (stride="
                    << stride << ", scheme_iteration="
                    << schemeIteration + 1 << ") : "
                    << std::flush;

        }
        labelVolume = probabilisticRoiSegmentation->getLabelVolume();
        gkg::Writer::getInstance().write(
          fileNameOut + "_stride" + gkg::StringConverter::toString( s ) +
          "_scheme_iteration_" +
          gkg::StringConverter::toString( schemeIteration ),
          *labelVolume );
        if ( verbose )
        {

          std::cout << "done" << std::endl;

        }

        ////////////////////////////////////////////////////////////////////////
        // closing file stream(s) for matplotlib plot(s)
        ////////////////////////////////////////////////////////////////////////
        if ( storagePeriod > 0 )
        {

          valueOs.close();
          deltaValueOs.close();

        }


        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        //
        // 3D TRANSFORM OPTIMIZATION
        //
        ////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////

        if ( transform3dIterationCounts[ s ] &&
             ( s < strideCount - 1 ) && 
             ( schemeIteration != schemeIterationCounts[ s ] - 1 ) )
        {

          if ( sameTransformAcrossLabels )
          {

            n = contrastNames.begin();
            ne = contrastNames.end();

            while ( n != ne )
            {

              // obtaining parameterized 3D transform for the current contrast
              // in case of same 3D transforms for all the labels of given 
              // contrast, we only have to get access to the transform of the
              // first label
              gkg::RCPointer< gkg::ParameterizedTransform3d< float > > 
                parameterizedTransform3d =
                  labelToAtlasParameterizedTransform3ds[ *n ].begin()->second;

              // creating cost function
              gkg::LabelIndependentTransform3dRelaxationFunction< int16_t >
                labelIndependentTransform3dRelaxationFunction(
                                                       parameterizedTransform3d,
                                                       energy,
                                                       *n );

              // launching Nelder-Mead Simplex optimizer
              double testSize = 0.0;
              gkg::Vector initialParameters;
              gkg::Vector deltaParameters = vectorOfDeltaParameters;
              parameterizedTransform3d->getDirectParameters(
                                                            initialParameters );
              if ( verbose )
              {

                std::cout << "initial parameters : "
                          << initialParameters
                          << std::endl;

              }
              factory->getNelderMeadSimplexOptimumParameters(
                                  initialParameters,
                                  deltaParameters,
                                  testSize,
                                  labelIndependentTransform3dRelaxationFunction,
                                  transform3dIterationCounts[ s ],
                                  maximumTestSize,
                                  verbose );
              if ( verbose )
              {

                std::cout << "optimal parameters : "
                          << initialParameters
                          << std::endl;

              }
              parameterizedTransform3d->setDirectParameters(
                                                            initialParameters );

              ++ n;

            }

          }
          else
          {

            n = contrastNames.begin();
            ne = contrastNames.end();

            while ( n != ne )
            {

              // obtaining atlas information
              gkg::AtlasInformation< std::string >
               atlasInformation( *atlases[ *n ], "deep_nuclei_dictionary", *n );

              // getting labelled ROI dictionary
              const gkg::LabelledRoiDictionary&
                labelledRoiDictionary = 
                                    atlasInformation.getLabelledRoiDictionary();

              // getting dictionary
              gkg::Dictionary dictionary =
                                          labelledRoiDictionary.getDictionary();

              std::string roiName;
              gkg::Dictionary::const_iterator
               r = dictionary.begin(),
               re = dictionary.end();
              while ( r != re )
              {

                // obtaining ROI name
                roiName = r->first;

                // obtaining label for the current ROI
                int16_t label = 0;
                labelledRoiDictionary.getRoiLabel( roiName, label );

                // obtaining parameterized 3D transform for current contrast and
                // label
                gkg::RCPointer< gkg::ParameterizedTransform3d< float > > 
                  parameterizedTransform3d =
                    labelToAtlasParameterizedTransform3ds[ *n ][ label ];

                // creating cost function
                gkg::LabelDependentTransform3dRelaxationFunction< int16_t >
                  labelDependentTransform3dRelaxationFunction(
                                                       parameterizedTransform3d,
                                                       energy,
                                                       *n,
                                                       label );

                // launching Nelder-Mead Simplex optimizer
                double testSize = 0.0;
                gkg::Vector initialParameters;
                gkg::Vector deltaParameters = vectorOfDeltaParameters;
                parameterizedTransform3d->getDirectParameters(
                                                            initialParameters );
                factory->getNelderMeadSimplexOptimumParameters(
                                  initialParameters,
                                  deltaParameters,
                                  testSize,
                                  labelDependentTransform3dRelaxationFunction,
                                  transform3dIterationCounts[ s ],
                                  maximumTestSize,
                                  verbose );
                parameterizedTransform3d->setDirectParameters(
                                                            initialParameters );

                ++ r;

              }
              ++ n;

            }

          }

        }

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing resulting label volume
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "writing '" << fileNameOut << "' label volume : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOut, *labelVolume );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( 
         "gkg::DeepNucleiSegmentationCommand::execute("
         "const std::vector< std::string >& contrastNames, "
         "const std::vector< std::string >& atlasFileNames, "
         "const std::vector< std::string >& contrastFileNames, "
         "std::vector< double >& allWeights, "
         "std::vector< int32_t >& strides, "
         "const std::vector< int32_t >& schemeIterationCounts, "
         "const std::vector< int32_t >& regionGrowingIterationCounts, "
         "const std::vector< int32_t >& transform3dIterationCounts, "
         "bool sameTransformAcrossLabels, const std::string& fileNameOut, "
         "double maximumTestSize, const std::vector< float >& deltaParameters, "
         "int32_t storagePeriod, int32_t displayPeriod, bool verbose )" );

}


RegisterCommandCreator(
     DeepNucleiSegmentationCommand,
     DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( contrastNames ) +
     DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( atlasFileNames ) +
     DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( contrastFileNames ) +
     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( allWeights ) +
     DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( strides ) +
     DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( schemeIterationCounts ) +
     DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( regionGrowingIterationCounts ) +
     DECLARE_VECTOR_OF_INTEGERS_PARAMETER_HELP( transform3dIterationCounts ) +
     DECLARE_BOOLEAN_PARAMETER_HELP( sameTransformAcrossLabels ) +
     DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
     DECLARE_FLOATING_PARAMETER_HELP( maximumTestSize ) +
     DECLARE_VECTOR_OF_FLOATINGS_PARAMETER_HELP( deltaParameters ) +
     DECLARE_INTEGER_PARAMETER_HELP( storagePeriod ) +
     DECLARE_INTEGER_PARAMETER_HELP( displayPeriod ) +
     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
