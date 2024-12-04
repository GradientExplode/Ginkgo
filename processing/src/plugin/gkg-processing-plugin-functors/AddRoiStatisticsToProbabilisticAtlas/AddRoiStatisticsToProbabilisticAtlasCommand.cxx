#include <gkg-processing-plugin-functors/AddRoiStatisticsToProbabilisticAtlas/AddRoiStatisticsToProbabilisticAtlasCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-histogram/HistogramAnalysis_i.h>
#include <gkg-processing-statistics/IntraSubjectStatisticalMap_i.h>
#include <gkg-processing-statistics/StructureBasedStatisticalMap_i.h>
#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-communication-sysinfo/File.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>


gkg::AddRoiStatisticsToProbabilisticAtlasCommand::
                                    AddRoiStatisticsToProbabilisticAtlasCommand(
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
  GKG_CATCH( "gkg::AddRoiStatisticsToProbabilisticAtlasCommand::"
             "AddRoiStatisticsToProbabilisticAtlasCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::AddRoiStatisticsToProbabilisticAtlasCommand::
                                    AddRoiStatisticsToProbabilisticAtlasCommand(
                            const std::vector< std::string >& fileNameContrasts,
                            const std::vector< std::string >& fileNameRois,
                            const std::string& fileNameAtlas,
                            const std::string& fileNameDictionary,
                            const std::string& fileNameOut,
                            const std::string& dictionaryName,
                            const std::string& contrastName,
                            bool verbose )
                                                : gkg::Command()
{

  try
  {

    execute( fileNameContrasts, fileNameRois, fileNameAtlas, fileNameDictionary,
             fileNameOut, dictionaryName, contrastName, verbose );

  }
  GKG_CATCH( "gkg::AddRoiStatisticsToProbabilisticAtlasCommand::"
             "AddRoiStatisticsToProbabilisticAtlasCommand( "
             "const std::vector< std::string >& fileNameContrasts, "
             "const std::vector< std::string >& fileNameRois, "
             "const std::string& fileNameAtlas, "
             "const std::string& fileNameDictionary, "
             "const std::string& fileNameOut, "
             "const std::string& dictionaryName, "
             "const std::string& contrastName, "
             "bool verbose )" );

}


gkg::AddRoiStatisticsToProbabilisticAtlasCommand::
                                    AddRoiStatisticsToProbabilisticAtlasCommand(
                                             const gkg::Dictionary& parameters )
                                                : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         fileNameContrasts );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         fileNameRois );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameAtlas );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameDictionary );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, dictionaryName );
    DECLARE_STRING_PARAMETER( parameters, std::string, contrastName );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameContrasts, fileNameRois, fileNameAtlas, fileNameDictionary,
             fileNameOut, dictionaryName, contrastName, verbose );

  }
  GKG_CATCH( "gkg::AddRoiStatisticsToProbabilisticAtlasCommand::"
             "AddRoiStatisticsToProbabilisticAtlasCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::AddRoiStatisticsToProbabilisticAtlasCommand::
                                  ~AddRoiStatisticsToProbabilisticAtlasCommand()
{
}


std::string gkg::AddRoiStatisticsToProbabilisticAtlasCommand::getStaticName()
{

  try
  {

    return "AddRoiStatisticsToProbabilisticAtlas";

  }
  GKG_CATCH(
          "std::string " 
          "gkg::AddRoiStatisticsToProbabilisticAtlasCommand::getStaticName()" );

}


void gkg::AddRoiStatisticsToProbabilisticAtlasCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameContrasts;
    std::vector< std::string > fileNameRois;
    std::string fileNameAtlas;
    std::string fileNameDictionary;
    std::string fileNameOut;
    std::string dictionaryName;
    std::string contrastName;
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Computing ROI statistics from contrasts "
                                  "and labels, and adding them to the "
                                  "corresponding atlas",
                                  _loadPlugin );
    application.addSeriesOption( "-c",
                                 "Input contrast volume filename list",
                                 fileNameContrasts );
    application.addSeriesOption( "-r",
                                 "ROI label volume filename list",
                                 fileNameRois );
    application.addSingleOption( "-a",
                                 "Input atlas filename",
                                 fileNameAtlas );
    application.addSingleOption( "-d",
                                 "Labelled ROI dictionary filename",
                                 fileNameDictionary );
    application.addSingleOption( "-o",
                                 "Output file name",
                                 fileNameOut,
                                 true );
    application.addSingleOption( "-dn",
                                 "Dictionary name",
                                 dictionaryName );
    application.addSingleOption( "-cn",
                                 "Contrast name",
                                 contrastName );
    application.addSingleOption( "-verbose",
                                 "Show as much information as possible",
                                 verbose, 
                                 true );

    application.initialize();

    execute( fileNameContrasts, fileNameRois, fileNameAtlas, fileNameDictionary,
             fileNameOut, dictionaryName, contrastName, verbose );


  }
  GKG_CATCH_FUNCTOR( "void gkg::AddRoiStatisticsToProbabilisticAtlasCommand::"
                     "parse()" );

}


void gkg::AddRoiStatisticsToProbabilisticAtlasCommand::execute( 
                            const std::vector< std::string >& fileNameContrasts,
                            const std::vector< std::string >& fileNameRois,
                            const std::string& fileNameAtlas,
                            const std::string& fileNameDictionary,
                            const std::string& fileNameOut,
                            const std::string& dictionaryName,
                            const std::string& contrastName,
                            bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // reading labelled Roi Dictionary data
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading labelled ROI dictionary '"
                << fileNameDictionary << "' : "
                << std::flush;

    }
    gkg::LabelledRoiDictionary labelledRoiDictionary( dictionaryName );
    gkg::Reader::getInstance().read( fileNameDictionary,
                                     labelledRoiDictionary );
    gkg::StructureBasedStatisticalMap< std::string >
      structureBasedStatisticalMap( contrastName );
    if ( verbose )
    {

      std::cout << "done" << std::endl;
    }

    ////////////////////////////////////////////////////////////////////////////
    //reading atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( verbose )
    {

      std::cout << "reading atlas '" << fileNameAtlas << "' : " << std::flush;

    }

    gkg::Volume< int16_t > atlas;
    gkg::Reader::getInstance().read( fileNameAtlas, atlas );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading contrast data
    ////////////////////////////////////////////////////////////////////////////


    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      ia;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      ib;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      igm;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      igs;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      iv;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >::iterator
      is;

    gkg::Dictionary dictionary = labelledRoiDictionary.getDictionary();  

    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      grayLevelMeanStatistics;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      grayLevelStandardDeviationStatistics;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      grayLevelAlphaStatistics;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      grayLevelBetaStatistics;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      volumeStatistics;
    std::map< std::string, gkg::UpdatableMeanAndStandardDeviation >
      surfaceStatistics;

    std::vector< std::string >::const_iterator
      c = fileNameContrasts.begin(),
      ce = fileNameContrasts.end();

    std::vector< std::string >::const_iterator
      r = fileNameRois.begin();

    while ( c != ce )
    {

      // reading current contrast volume
      if ( verbose )
      {

        std::cout << "reading contrast '" << *c << "' : " << std::flush;

      }

      gkg::Volume< int16_t > contrast;
      std::string format = gkg::Reader::getInstance().read( *c, contrast );

      if ( verbose )
      {

        std::cout << std::endl;

      }

      // reading current ROI label volume
      if ( verbose )
      {

        std::cout << "reading ROI label '" << *r << "' : " << std::flush;

      }

      gkg::Volume< int16_t > roi;
      std::string format2 = gkg::Reader::getInstance().read( *r, roi );

      if ( verbose )
      {

        std::cout << std::endl;

      }

      // creatng map of functors
      std::list< gkg::IntraSubjectStatisticalMap< int16_t, 
                                                  int16_t >::Functor > functors;

      if( contrastName == "t1" )
      {

        functors.push_back( gkg::IntraSubjectStatisticalMap< 
                                      int16_t, 
                                      int16_t >::grayLevelAlphaAndBetaFunctor );
      }

      functors.push_back( 
          gkg::IntraSubjectStatisticalMap< 
                          int16_t, 
                          int16_t >::grayLevelMeanAndStandardDeviationFunctor );

      functors.push_back( gkg::IntraSubjectStatisticalMap< 
                                                    int16_t, 
                                                    int16_t >::surfaceFunctor );

      functors.push_back( gkg::IntraSubjectStatisticalMap< 
                                                     int16_t, 
                                                     int16_t >::volumeFunctor );

      // computing the intra-subject statistics for each ROI
      gkg::IntraSubjectStatisticalMap< int16_t, int16_t >
        intraSubjectStatisticalMap( contrastName,
                                    contrast,
                                    roi,
                                    functors );

      // adding contribution of the current subject to inter-subject statistics
      double alpha = 0.0;
      double beta = 0.0;
      double grayLevelMean = 0.0;
      double grayLevelStandardDeviation = 0.0;
      double volume = 0.0;
      double surface = 0.0;

      gkg::Dictionary::const_iterator s = dictionary.begin(),
                                      se = dictionary.end();

      while ( s != se )
      {
 
        std::string roiName = s->first;
        int16_t label = 0;
	labelledRoiDictionary.getRoiLabel( roiName, label );

        if ( label )
        {

          if ( contrastName == "t1" )
          {

            alpha = intraSubjectStatisticalMap.getStatisticalMap().
                      getStatisticalParameterMap(
                        label, 
                        "gray_level_alpha" ).getStatisticalParameter( "value" );
            beta = intraSubjectStatisticalMap.getStatisticalMap().
                     getStatisticalParameterMap(
                        label, 
                        "gray_level_beta" ).getStatisticalParameter( "value" );

          }

          grayLevelMean =
            intraSubjectStatisticalMap.getStatisticalMap().
                   getStatisticalParameterMap(
                      label, 
                      "gray_level" ).getStatisticalParameter( "mean" );

          grayLevelStandardDeviation = 
            intraSubjectStatisticalMap.getStatisticalMap().
              getStatisticalParameterMap( 
               label,
               "gray_level" ).getStatisticalParameter( "standard_deviation" );

 
          volume = intraSubjectStatisticalMap.getStatisticalMap().
                     getStatisticalParameterMap(
                         label, 
                         "volume" ).getStatisticalParameter( "value" );
          surface = intraSubjectStatisticalMap.getStatisticalMap().
          	    getStatisticalParameterMap(
                         label, 
                         "surface" ).getStatisticalParameter( "value" );

          if ( verbose )
          {
       
            std::cout << "ROI name: " << roiName << std::endl; 
            if( contrastName == "t1" )
            {

              std::cout << "alpha: " << alpha << std::endl;
              std::cout << "beta: " << beta << std::endl;  

            }
            std::cout << "gray level mean: " << grayLevelMean << std::endl;
            std::cout << "gray level standard deviation: " 
	              << grayLevelStandardDeviation << std::endl;
            std::cout << "volume: " << volume << std::endl;
            std::cout << "surface: " << surface << std::endl;
            std::cout << std::endl;
	    
          }

          igm = grayLevelMeanStatistics.find( roiName );
          if ( igm != grayLevelMeanStatistics.end() )
          {

            igm->second.add( grayLevelMean );

          }
          else
          {

            gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
            meanAndStandardDeviation.add( grayLevelMean );
            grayLevelMeanStatistics.insert( std::make_pair( 
                                                   roiName, 
                                                   meanAndStandardDeviation ) );

          }

          igs = grayLevelStandardDeviationStatistics.find( roiName );
          if ( igs != grayLevelStandardDeviationStatistics.end() )
          {

            igs->second.add( grayLevelStandardDeviation );

          }
          else
          {

            gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
            meanAndStandardDeviation.add( grayLevelStandardDeviation );
            grayLevelStandardDeviationStatistics.insert( std::make_pair( 
                                                   roiName, 
                                                   meanAndStandardDeviation ) );

          }

          if( contrastName == "t1" )
          {
       
            ia = grayLevelAlphaStatistics.find( roiName );
            if ( ia != grayLevelAlphaStatistics.end() )
            {

              ia->second.add( alpha );

            }
            else
            {

              gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
              meanAndStandardDeviation.add( alpha );
              grayLevelAlphaStatistics.insert( std::make_pair( 
            					   roiName, 
            					   meanAndStandardDeviation ) );

            }
	    
            ib = grayLevelBetaStatistics.find( roiName );
            if ( ib != grayLevelBetaStatistics.end() )
            {

              ib->second.add( beta );

            }
            else
            {

              gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
              meanAndStandardDeviation.add( beta );
              grayLevelBetaStatistics.insert( std::make_pair(
            					   roiName, 
            					   meanAndStandardDeviation ) );

            }
	   
          }

          iv = volumeStatistics.find( roiName );
          if ( iv != volumeStatistics.end() )
          {

            iv->second.add( volume );

          }
          else
          {

            gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
            meanAndStandardDeviation.add( volume );
            volumeStatistics.insert( std::make_pair( 
                                                   roiName, 
          					   meanAndStandardDeviation ) );

          }

          is = surfaceStatistics.find( roiName );
          if ( is != surfaceStatistics.end() )
          {

            is->second.add( surface );

          }
          else
          {

            gkg::UpdatableMeanAndStandardDeviation meanAndStandardDeviation;
            meanAndStandardDeviation.add( surface );
            surfaceStatistics.insert( std::make_pair( 
                                                   roiName, 
          					   meanAndStandardDeviation ) );

          }

        }

        ++ s;  
 
      }  

      ++ c;
      ++ r;
       
    }

    // computing inter-subject statistics
    if ( verbose )
    {

      std::cout << "computing inter-subject statistics : " << std::flush;

    }

    gkg::StatisticalParameterMap statisticalParameterMap; 
    gkg::Dictionary::const_iterator s = dictionary.begin(),
                                    se = dictionary.end();
 
    while ( s != se )
    {

      std::string roiName = s->first;
      int16_t label = 0;
      labelledRoiDictionary.getRoiLabel( roiName, label );

      if ( label )
      {
      
      	// adding gray level alpha statistics to structure based statistical map
	
        if ( contrastName == "t1" )
        {
	
      	  ia = grayLevelAlphaStatistics.find( roiName );
          statisticalParameterMap.setStatisticalParameter( 
                                            "mean", 
                                            ia->second.getMean() );
      	  statisticalParameterMap.setStatisticalParameter( 
                                            "standard_deviation", 
                                            ia->second.getStandardDeviation() );

      	  structureBasedStatisticalMap.addStatisticalParameterMap( 
                                            roiName,
                                            "gray_level_alpha",
                                            statisticalParameterMap );

      	  // adding gray level beta statistics to structure based statistical 
          // map
      	  ib = grayLevelBetaStatistics.find( roiName );
      	  statisticalParameterMap.setStatisticalParameter( 
                                            "mean", 
      	                                    ib->second.getMean() );
      	  statisticalParameterMap.setStatisticalParameter( 
                                            "standard_deviation", 
                                            ib->second.getStandardDeviation() );

      	  structureBasedStatisticalMap.addStatisticalParameterMap( 
                                            roiName,
                                            "gray_level_beta",
                                            statisticalParameterMap );

        }
        else
        {
	
          igm = grayLevelMeanStatistics.find( roiName );
            statisticalParameterMap.setStatisticalParameter( 
                                           "mean", 
                                           igm->second.getMean() );
          statisticalParameterMap.setStatisticalParameter( 
                                           "standard_deviation", 
                                           igm->second.getStandardDeviation() );

          structureBasedStatisticalMap.addStatisticalParameterMap( 
                                           roiName,
                                           "gray_level_mean",
                                           statisticalParameterMap );

          igs = grayLevelStandardDeviationStatistics.find( roiName );
            statisticalParameterMap.setStatisticalParameter( 
                                           "mean", 
                                           igs->second.getMean() );
          statisticalParameterMap.setStatisticalParameter( 
                                           "standard_deviation", 
                                           igs->second.getStandardDeviation() );

          structureBasedStatisticalMap.addStatisticalParameterMap( 
                                           roiName,
                                           "gray_level_standard_deviation",
                                           statisticalParameterMap );

        }
	
      	// adding volume statistics to structure based statistical map
      	iv = volumeStatistics.find( roiName );
      	statisticalParameterMap.setStatisticalParameter( "mean", 
                                           iv->second.getMean() );
      	statisticalParameterMap.setStatisticalParameter( 
                                           "standard_deviation", 
                                           iv->second.getStandardDeviation() );

      	structureBasedStatisticalMap.addStatisticalParameterMap( 
                                           roiName,
                                           "volume",
                                           statisticalParameterMap );

      	// adding surface statistics to structure based statistical map
      	is = surfaceStatistics.find( roiName );
      	statisticalParameterMap.setStatisticalParameter( "mean", 
                                           is->second.getMean() );
      	statisticalParameterMap.setStatisticalParameter( 
                                           "standard_deviation", 
                                           is->second.getStandardDeviation() );

      	structureBasedStatisticalMap.addStatisticalParameterMap( 
                                           roiName,
                                           "surface",
                                           statisticalParameterMap );

      }
      ++ s;

    }

    if ( verbose )
    {

      std::cout << std::endl;

    }

    // adding inter-subject statistics to atlas
    if ( verbose )
    {

      std::cout << "adding inter-subject statistics to atlas : " << std::flush;

    }
    structureBasedStatisticalMap.addToHeaderedObject( atlas );

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    // adding inter-subject statistics to atlas
    gkg::File file( fileNameOut );
    if ( file.isExisting() )
    {
    
      if ( verbose )
      {

        std::cout << "writing '" << fileNameOut << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameOut, atlas ); 
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }
    else
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameAtlas << "' : " << std::flush;

      }
      gkg::Writer::getInstance().write( fileNameAtlas, atlas );
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "void gkg::AddRoiStatisticsToProbabilisticAtlasCommand::execute( "
             "const std::vector< std::string >& fileNameContrats, "
             "const std::vector< std::string >& fileNameRois, "
             "const std::string& fileNameAtlas, "
             "const std::string& fileNameDictionary, "
             "const std::string& fileNameOut, "
             "const std::string& dictionaryName, "
             "const std::string& contrastName, "
             "bool verbose )" );

}


RegisterCommandCreator(
                 AddRoiStatisticsToProbabilisticAtlasCommand,
                 DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameContrasts ) +
                 DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameRois ) +
                 DECLARE_STRING_PARAMETER_HELP( fileNameAtlas ) +
                 DECLARE_STRING_PARAMETER_HELP( fileNameDictionary ) +
                 DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
                 DECLARE_STRING_PARAMETER_HELP( dictionaryName ) +
                 DECLARE_STRING_PARAMETER_HELP( contrastName ) +
                 DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
