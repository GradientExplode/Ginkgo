#include <gkg-processing-plugin-functors/RoiStatistics/RoiStatisticsCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-processing-roi/LabelledRoiDictionary.h>
#include <gkg-processing-algobase/UpdatableMeanAndStandardDeviation.h>
#include <gkg-processing-statistics/StructureBasedStatisticalMap_i.h>
#include <gkg-processing-statistics/IntraSubjectStatisticalMap_i.h>
#include <gkg-processing-histogram/HistogramAnalysis_i.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>


//
// class StatisticProcess
//

class StatisticProcess : public gkg::Process
{

  public:

    StatisticProcess( const std::vector< std::string >& contrastFileNameIns,
                      const std::vector< std::string >& labelFileNameIns,
                      const std::string& roiDictionaryFileName,
                      const std::string& atlasFileName,
                      const std::string& contrastName,
                      const std::string& fileNameOut,
                      const bool& ascii,
                      const std::string& format,
                      const bool& verbose );

    const std::vector< std::string >& getContrastFileNameIns() const;
    const std::vector< std::string >& getLabelFileNameIns() const;
    const std::string& getRoiDictionaryFileName() const;
    const std::string& getAtlasFileName() const;
    const std::string& getContrastName() const;
    const std::string& getFileNameOut() const;
    const bool& getAscii() const;
    const std::string& getFormat() const;
    const bool& getVerbose() const;

  private:

    template < class T >
    static void getStatistics( gkg::Process& process,
                               const std::string& fileNameIn,
                               const gkg::AnalyzedObject&,
                               const std::string& );

    const std::vector< std::string >& _contrastFileNameIns;
    const std::vector< std::string >& _labelFileNameIns;
    const std::string& _roiDictionaryFileName;
    const std::string& _atlasFileName;
    const std::string& _contrastName;
    const std::string& _fileNameOut;
    const bool& _ascii;
    const std::string& _format;
    const bool& _verbose;

};


StatisticProcess::StatisticProcess(
                         const std::vector< std::string >& contrastFileNameIns,
                         const std::vector< std::string >& labelFileNameIns,
                         const std::string& roiDictionaryFileName,
                         const std::string& atlasFileName,
                         const std::string& contrastName,
                         const std::string& fileNameOut,
                         const bool& ascii,
                         const std::string& format,
                         const bool& verbose )
                 : gkg::Process(),
                   _contrastFileNameIns( contrastFileNameIns ),
                   _labelFileNameIns( labelFileNameIns ),
                   _roiDictionaryFileName( roiDictionaryFileName ),
                   _atlasFileName( atlasFileName ),
                   _contrastName( contrastName ),
                   _fileNameOut( fileNameOut ),
                   _ascii( ascii ),
                   _format( format ),
                   _verbose( verbose )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &StatisticProcess::getStatistics< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &StatisticProcess::getStatistics< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &StatisticProcess::getStatistics< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &StatisticProcess::getStatistics< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &StatisticProcess::getStatistics< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &StatisticProcess::getStatistics< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &StatisticProcess::getStatistics< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &StatisticProcess::getStatistics< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &StatisticProcess::getStatistics< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &StatisticProcess::getStatistics< double > );

  }
  GKG_CATCH( "StatisticProcess::StatisticProcess( "
             "const std::vector< std::string >& contrastFileNameIns, "
             "const std::vector< std::string >& labelFileNameIns, "
             "const std::string& roiDictionaryFileName, "
             "const std::string& atlasFileName, "
             "const std::string& contrastName, "
             "const std::string& fileNameOut, "
             "const bool& ascii, "
             "const std::string& format, "
             "const bool& verbose )" );

}


const std::vector< std::string >& StatisticProcess::getContrastFileNameIns() const
{

  return _contrastFileNameIns;

}

const std::vector< std::string >& StatisticProcess::getLabelFileNameIns() const
{

  return _labelFileNameIns;

}


const std::string& StatisticProcess::getRoiDictionaryFileName() const
{

  return _roiDictionaryFileName;

}


const std::string& StatisticProcess::getAtlasFileName() const
{

  return _atlasFileName;

}


const std::string& StatisticProcess::getContrastName() const
{

  return _contrastName;

}


const std::string& StatisticProcess::getFileNameOut() const
{

  return _fileNameOut;

}


const bool& StatisticProcess::getAscii() const
{

  return _ascii;

}


const std::string& StatisticProcess::getFormat() const
{

  return _format;

}


const bool& StatisticProcess::getVerbose() const
{

  return _verbose;

}


template < class T >
void StatisticProcess::getStatistics( gkg::Process& process,
                                      const std::string& /*fileNameIn*/,
                                      const gkg::AnalyzedObject&,
                                      const std::string& )
{

  try
  {

    StatisticProcess&
      statisticProcess = static_cast< StatisticProcess& >( process );


    ////////////////////////////////////////////////////////////////////////////
    // reading labelled ROI dictionary
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "reading '"
                << statisticProcess.getRoiDictionaryFileName()
                << "' : " << std::flush;

    }
    gkg::LabelledRoiDictionary
      labelledRoiDictionary( "deep_nuclei_dictionary" );
    gkg::Reader::getInstance().read(
                                   statisticProcess.getRoiDictionaryFileName(), 
                                   labelledRoiDictionary );
    gkg::Dictionary roiDictionary = labelledRoiDictionary.getDictionary();
    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "reading '" << statisticProcess.getAtlasFileName()
                << "' : " << std::flush;

    }

    gkg::Volume< int16_t > atlas;
    gkg::Reader::getInstance().read( statisticProcess.getAtlasFileName(),
                                     atlas );
    bool ascii = false;
    if ( atlas.getHeader().hasAttribute( "io_mode" ) )
    {

      std::string io_mode;
      atlas.getHeader().getAttribute( "io_mode", io_mode );
      if ( gkg::IOMode::getInstance().getTypeFromName( io_mode ) == 
           gkg::IOMode::Ascii )
      {

        ascii = true;

      }

    }

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing functor(s) to be evaluated
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "preparing functor list : " << std::flush;

    }
    std::list< typename gkg::IntraSubjectStatisticalMap< T,
                                                         int16_t >::Functor >
      functors;

    functors.push_back( gkg::IntraSubjectStatisticalMap< 
                          T, 
                          int16_t >::grayLevelMeanAndStandardDeviationFunctor );
    functors.push_back( gkg::IntraSubjectStatisticalMap< 
                          T, 
                          int16_t >::grayLevelAlphaAndBetaFunctor );
    functors.push_back( gkg::IntraSubjectStatisticalMap< 
                          T, 
                          int16_t >::surfaceFunctor );
    functors.push_back( gkg::IntraSubjectStatisticalMap< 
                          T, 
                          int16_t >::volumeFunctor );
    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // preparing map(s) of updatable mean and standard deviation
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "preparing updatable statistics : " << std::flush;

    }
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
    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // looping over contrast and label volume(s)
    ////////////////////////////////////////////////////////////////////////////

    std::string format;
    std::vector< std::string >::const_iterator
      c = statisticProcess.getContrastFileNameIns().begin(),
      ce = statisticProcess.getContrastFileNameIns().end();
    std::vector< std::string >::const_iterator
      l = statisticProcess.getLabelFileNameIns().begin();
    while ( c != ce )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading contrast
      //////////////////////////////////////////////////////////////////////////
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "reading '" << *c << "' : " << std::flush;

      }
      gkg::Volume< T > contrastVolume;
      format = gkg::Reader::getInstance().read( *c, contrastVolume );
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // obtaining histogram analysis info from header of contrast volume
      //////////////////////////////////////////////////////////////////////////
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "histogram analysis of '" << *c << "' : " << std::flush;

      }
      gkg::HistogramAnalysis histogramAnalysis( contrastVolume );
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // reading label
      //////////////////////////////////////////////////////////////////////////
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "reading '" << *l << "' : " << std::flush;

      }
      gkg::Volume< int16_t > labelVolume;
      gkg::Reader::getInstance().read( *l, labelVolume );
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // processing intra-subject statistical map
      //////////////////////////////////////////////////////////////////////////

      if ( statisticProcess.getVerbose() )
      {

        std::cout << "processing intra-subject statistical map for '"
                  << *c << "' : " << std::flush;

      }
      gkg::IntraSubjectStatisticalMap< T, int16_t >
        intraSubjectStatisticalMap( statisticProcess.getContrastName(),
                                    contrastVolume,
                                    labelVolume,
                                    functors );
      if ( statisticProcess.getVerbose() )
      {

        std::cout << "done" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // looping over ROI(s)
      //////////////////////////////////////////////////////////////////////////

      gkg::Dictionary::const_iterator
        r = roiDictionary.begin(),
        re = roiDictionary.end();
      double mean = 0.0;
      double standardDeviation = 0.0;
      double alpha = 0.0;
      double beta = 0.0;
      double volume = 0.0;
      double surface = 0.0;
      while ( r != re )
      {

        // getting ROI key and label
        std::string roiKey = r->first;
        int16_t label = 0;
        labelledRoiDictionary.getRoiLabel( roiKey, label );

        // collecting intra-subject statistics
        mean = intraSubjectStatisticalMap.getStatisticalMap().
                  getStatisticalParameterMap( label, "mean" ).
                  getStatisticalParameter( "value" );
        standardDeviation = intraSubjectStatisticalMap.getStatisticalMap().
                  getStatisticalParameterMap( label, "standard_deviation" ).
                  getStatisticalParameter( "value" );
        alpha = intraSubjectStatisticalMap.getStatisticalMap().
                  getStatisticalParameterMap( label, "gray_level_alpha" ).
                  getStatisticalParameter( "value" );
        beta = intraSubjectStatisticalMap.getStatisticalMap().
                 getStatisticalParameterMap( label, "gray_level_beta" ).
                 getStatisticalParameter( "value" );
        volume = intraSubjectStatisticalMap.getStatisticalMap().
                   getStatisticalParameterMap( label, "volume" ).
                   getStatisticalParameter( "value" );
        surface = intraSubjectStatisticalMap.getStatisticalMap().
                    getStatisticalParameterMap( label, "surface" ).
                    getStatisticalParameter( "value" );

        // displaying value(s)
        if ( statisticProcess.getVerbose() )
        {

          std::cout << "ROI name: " << roiKey << std::endl;
          std::cout << "  mean                 = " << mean << std::endl;
          std::cout << "  standard deviation   = " << standardDeviation
                                                   << std::endl;
          std::cout << "  alpha                = " << alpha << std::endl;
          std::cout << "  beta                 = " << beta << std::endl;
          std::cout << "  volume               = " << volume << " mm^3"
                                                   << std::endl;
          std::cout << "  surface              = " << surface << " mm^2"
                                                   << std::endl;

        }

        // updating intra-subject gray level mean
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          m = grayLevelMeanStatistics.find( roiKey );
        if ( m != grayLevelMeanStatistics.end() )
        {

          m->second.add( mean );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( alpha );
          grayLevelMeanStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        // updating intra-subject gray level standard deviation
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          sd = grayLevelMeanStatistics.find( roiKey );
        if ( sd != grayLevelMeanStatistics.end() )
        {

          sd->second.add( standardDeviation );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( standardDeviation );
          grayLevelStandardDeviationStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        // updating intra-subject gray level alpha
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          a = grayLevelAlphaStatistics.find( roiKey );
        if ( a != grayLevelAlphaStatistics.end() )
        {

          a->second.add( alpha );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( alpha );
          grayLevelAlphaStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        // updating intra-subject gray level beta
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          b = grayLevelBetaStatistics.find( roiKey );
        if ( b != grayLevelBetaStatistics.end() )
        {

          b->second.add( beta );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( beta );
          grayLevelBetaStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        // updating intra-subject volume
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          v = volumeStatistics.find( roiKey );
        if ( v != volumeStatistics.end() )
        {

          v->second.add( volume );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( volume );
          volumeStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        // updating intra-subject surface
        std::map< std::string,
                  gkg::UpdatableMeanAndStandardDeviation >::iterator
          s = surfaceStatistics.find( roiKey );
        if ( s != surfaceStatistics.end() )
        {

          s->second.add( surface );

        }
        else
        {

          gkg::UpdatableMeanAndStandardDeviation 
            updatableMeanAndStandardDeviation;
          updatableMeanAndStandardDeviation.add( surface );
          surfaceStatistics.insert(
                          std::make_pair( roiKey,
                                          updatableMeanAndStandardDeviation ) );

        }

        ++ r;

      }

      ++ c;
      ++ l;

    }


    ////////////////////////////////////////////////////////////////////////////
    // creating inter-subject statistical parameter map
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "creating inter-subject statical parameter map : "
                << std::flush;

    }
    gkg::StructureBasedStatisticalMap< std::string >
      interSubjectStructureBasedStatisticalMap(
                                           statisticProcess.getContrastName() );
    gkg::Dictionary::const_iterator
      r = roiDictionary.begin(),
      re = roiDictionary.end();
    while ( r != re )
    {

      // getting ROI key
      std::string roiKey = r->first;

      // adding inter-subject gray level mean mean and standard deviation
      gkg::StatisticalParameterMap grayLevelMeanSPM;
      grayLevelMeanSPM.setStatisticalParameter(
                    "mean",
                    grayLevelMeanStatistics[ roiKey ].getMean() );
      grayLevelMeanSPM.setStatisticalParameter(
                    "standard_deviation",
                    grayLevelMeanStatistics[ roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                      roiKey,
                                                      "gray_level_mean",
                                                      grayLevelMeanSPM );

      // adding inter-subject gray level standard deviation mean and 
      // standard deviation
      gkg::StatisticalParameterMap grayLevelStandardDeviationSPM;
      grayLevelStandardDeviationSPM.setStatisticalParameter(
                    "mean",
                    grayLevelStandardDeviationStatistics[ roiKey ].getMean() );
      grayLevelStandardDeviationSPM.setStatisticalParameter(
                    "standard_deviation",
                    grayLevelStandardDeviationStatistics[
                                             roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                roiKey,
                                                "gray_level_standard_deviation",
                                                grayLevelStandardDeviationSPM );

      // adding inter-subject gray level alpha mean and standard deviation
      gkg::StatisticalParameterMap grayLevelAlphaSPM;
      grayLevelAlphaSPM.setStatisticalParameter(
                    "mean",
                    grayLevelAlphaStatistics[ roiKey ].getMean() );
      grayLevelAlphaSPM.setStatisticalParameter(
                    "standard_deviation",
                    grayLevelAlphaStatistics[ roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                      roiKey,
                                                      "gray_level_alpha",
                                                      grayLevelAlphaSPM );

      // adding inter-subject gray level beta mean and standard deviation
      gkg::StatisticalParameterMap grayLevelBetaSPM;
      grayLevelBetaSPM.setStatisticalParameter(
                    "mean",
                    grayLevelBetaStatistics[ roiKey ].getMean() );
      grayLevelBetaSPM.setStatisticalParameter(
                    "standard_deviation",
                    grayLevelBetaStatistics[ roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                      roiKey,
                                                      "gray_level_beta",
                                                      grayLevelBetaSPM );

      // adding inter-subject volume mean and standard deviation
      gkg::StatisticalParameterMap volumeSPM;
      volumeSPM.setStatisticalParameter(
                    "mean",
                    volumeStatistics[ roiKey ].getMean() );
      volumeSPM.setStatisticalParameter(
                    "standard_deviation",
                    volumeStatistics[ roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                      roiKey,
                                                      "volume",
                                                      volumeSPM );

      // adding inter-subject surface mean and standard deviation
      gkg::StatisticalParameterMap surfaceSPM;
      surfaceSPM.setStatisticalParameter(
                    "mean",
                    surfaceStatistics[ roiKey ].getMean() );
      surfaceSPM.setStatisticalParameter(
                    "standard_deviation",
                    surfaceStatistics[ roiKey ].getStandardDeviation() );
      interSubjectStructureBasedStatisticalMap.addStatisticalParameterMap( 
                                                      roiKey,
                                                      "surface",
                                                      surfaceSPM );

      ++ r;

    }

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // adding inter-subject structure based statistical map to atlas
    ////////////////////////////////////////////////////////////////////////////

    if ( statisticProcess.getVerbose() )
    {

      std::cout << "adding inter-subject statical parameter map to atlas : "
                << std::flush;

    }
    interSubjectStructureBasedStatisticalMap.addToHeaderedObject( atlas );
    if ( statisticProcess.getVerbose() )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading input data
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // writing output data
    ////////////////////////////////////////////////////////////////////////////
    if ( ( statisticProcess.getFileNameOut().empty() ||
           ( statisticProcess.getFileNameOut() ==
             statisticProcess.getAtlasFileName() ) ) &&
         ( statisticProcess.getFormat().empty() ||
           ( format == statisticProcess.getFormat() ) ) &&
         ( ascii == statisticProcess.getAscii() ) )
    {

      gkg::Writer::getInstance().writeHeader( 
                                           statisticProcess.getAtlasFileName(),
                                           atlas, format );

    }
    else
    {

      gkg::Writer::getInstance().write( statisticProcess.getFileNameOut(),
                                        atlas,
                                        statisticProcess.getAscii(),
                                        statisticProcess.getFormat() );

    }

  }
  GKG_CATCH( "template < class T > "
             "void StatisticProcess::getStatistics( "
             "gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   RoiStatisticsCommand
//


gkg::RoiStatisticsCommand::RoiStatisticsCommand( int32_t argc,
                                 		 char* argv[],
                                 		 bool loadPlugin,
                                 		 bool removeFirst )
                         : gkg::Command( argc, argv, loadPlugin, removeFirst )
{

  try
  {

    parse();

  }
  GKG_CATCH( "gkg::RoiStatisticsCommand::RoiStatisticsCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::RoiStatisticsCommand::RoiStatisticsCommand(
                          const std::vector< std::string >& contrastFileNameIns,
                          const std::vector< std::string >& labelFileNameIns,
                          const std::string& roiDictionaryFileName,
                          const std::string& atlasFileName,
                          const std::string& contrastName,
                          const std::string& fileNameOut,
                          bool ascii,
                          const std::string& format,
                          bool verbose )
                         : gkg::Command()
{

  try
  {

    execute( contrastFileNameIns, labelFileNameIns, roiDictionaryFileName,
             atlasFileName, contrastName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RoiStatisticsCommand::RoiStatisticsCommand( "
             "const std::vector< std::string >& contrastFileNameIns, "
             "const std::vector< std::string >& labelFileNameIns, "
             "const std::string& roiDictionaryFileName, "
             "const std::string& atlasFileName, "
             "const std::string& contrastName, const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


gkg::RoiStatisticsCommand::RoiStatisticsCommand(
                                             const gkg::Dictionary& parameters )
                         : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         contrastFileNameIns );
    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters, 
                                         std::vector< std::string >,
                                         labelFileNameIns );
    DECLARE_STRING_PARAMETER( parameters, std::string, roiDictionaryFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, atlasFileName );
    DECLARE_STRING_PARAMETER( parameters, std::string, contrastName );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOut );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( contrastFileNameIns, labelFileNameIns, roiDictionaryFileName,
             atlasFileName, contrastName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH( "gkg::RoiStatisticsCommand::RoiStatisticsCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::RoiStatisticsCommand::~RoiStatisticsCommand()
{
}


std::string gkg::RoiStatisticsCommand::getStaticName()
{

  try
  {

    return "RoiStatistics";

  }
  GKG_CATCH( "std::string gkg::RoiStatisticsCommand::getStaticName()" );

}


void gkg::RoiStatisticsCommand::parse()
{

  try
  {

    std::vector< std::string > contrastFileNameIns;
    std::vector< std::string > labelFileNameIns;
    std::string roiDictionaryFileName;
    std::string atlasFileName;
    std::string contrastName;
    std::string fileNameOut;
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    ////////////////////////////////////////////////////////////////////////////
    // managing command lines argument(s)
    ////////////////////////////////////////////////////////////////////////////

    gkg::Application application( _argc, _argv,
                                  "Computing statistics on a set of ROIs:\n"
                                  "- gray level alpha\n"
                                  "- gray level beta\n"
                                  "- volume\n"
                                  "- surface\n",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input volume file name list",
                                 contrastFileNameIns,
                                 1 );
    application.addSeriesOption( "-l",
                                 "Label volume file name list",
                                 labelFileNameIns,
                                 1 );
    application.addSingleOption( "-d",
                                 "Labelled ROI dictionary file name",
                                 roiDictionaryFileName );
    application.addSingleOption( "-a",
                                 "Input atlas file name",
                                 atlasFileName );
    application.addSingleOption( "-n",
                                 "Contrast name",
                                 contrastName );
    application.addSingleOption( "-o",
                                 "Output atlas file name (default=input atlas)",
                                 fileNameOut,
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

    execute( contrastFileNameIns, labelFileNameIns, roiDictionaryFileName,
             atlasFileName, contrastName, fileNameOut, ascii, format, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::RoiStatisticsCommand::parse()" );

}


void gkg::RoiStatisticsCommand::execute(
                  	  const std::vector< std::string >& contrastFileNameIns,
                          const std::vector< std::string >& labelFileNameIns,
                          const std::string& roiDictionaryFileName,
                          const std::string& atlasFileName,
                          const std::string& contrastName,
                          const std::string& fileNameOut,
                          bool ascii,
                          const std::string& format,
                          bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( contrastFileNameIns.size() != labelFileNameIns.size() )
    {

      throw std::runtime_error(
                            "not the same contrast and label volume count(s)" );

    }


    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////
    StatisticProcess statisticProcess( contrastFileNameIns,
                                       labelFileNameIns,
                                       roiDictionaryFileName,
                                       atlasFileName,
                                       contrastName,
                                       fileNameOut,
                                       ascii,
                                       format,
                                       verbose );
    statisticProcess.execute( contrastFileNameIns[ 0 ] );

  }
  GKG_CATCH( "void gkg::RoiStatisticsCommand::execute( "
             "const std::vector< std::string >& contrastFileNameIns, "
             "const std::vector< std::string >& labelFileNameIns, "
             "const std::string& roiDictionaryFileName, "
             "const std::string& atlasFileName, "
             "const std::string& contrastName, const std::string& fileNameOut, "
             "bool ascii, const std::string& format, bool verbose )" );

}


RegisterCommandCreator(
               RoiStatisticsCommand,
    	       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( contrastFileNameIns ) +
    	       DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( labelFileNameIns ) +
    	       DECLARE_STRING_PARAMETER_HELP( roiDictionaryFileName ) +
    	       DECLARE_STRING_PARAMETER_HELP( atlasFileName ) +
    	       DECLARE_STRING_PARAMETER_HELP( contrastName ) +
    	       DECLARE_STRING_PARAMETER_HELP( fileNameOut ) +
    	       DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    	       DECLARE_STRING_PARAMETER_HELP( format ) +
    	       DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
