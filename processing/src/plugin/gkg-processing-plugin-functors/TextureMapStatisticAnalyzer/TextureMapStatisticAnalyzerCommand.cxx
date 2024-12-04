#include <gkg-processing-plugin-functors/TextureMapStatisticAnalyzer/TextureMapStatisticAnalyzerCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMaxMedian.h>
#include <gkg-processing-statistics/MannWhitneyWilcoxonUTest.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <fstream>
#include <map>


//
//  TextureMapStatisticAnalyzerProcess
//


static std::string getNameWithoutExtension( const std::string& name )
{

  try
  {

    std::set< std::string > extensions;
    extensions.insert( ".txt" );
    extensions.insert( ".csv" );

    std::set< std::string >::const_iterator e = extensions.begin(),
                                            ee = extensions.end();
    while ( e != ee )
    {

      if ( name.length() > e->length() )
      {

         if ( name.substr( name.length() - e->length(), e->length()) == *e )
         {

           return name.substr( 0, name.length() - e->length() );

         }

      }
      ++ e;

    }
    return name;

  }
  GKG_CATCH( "static std::string getNameWithoutExtension( "
             "const std::string& name )" );

}


class TextureMapStatisticAnalyzerProcess : public gkg::Process
{

  public:

    TextureMapStatisticAnalyzerProcess( 
                         const std::vector< std::string >& theFileNameInputs,
                         const std::string& theFileNameOutputTexture,
                         const std::string& theFileNameRoiTexture,
                         const std::string& theFileNameRoiPairs,
                         const std::string& theFileNameCsvPyDistribution,
                         const std::string& theMode,
                         const bool& theAscii,
                         const std::string& theFormat,
                         const bool& theVerbose );

    const std::vector< std::string >& fileNameInputs;
    const std::string& fileNameOutputTexture;
    const std::string& fileNameRoiTexture;
    const std::string& fileNameRoiPairs;
    const std::string& fileNameCsvPyDistribution;
    const std::string& mode;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T >
    static void analyzeScalar( gkg::Process& process,
                               const std::string& fileNameInput,
                               const gkg::AnalyzedObject&,
                               const std::string& );

    template < class T >
    static void analyzeList( gkg::Process& process,
                             const std::string& fileNameInput,
                             const gkg::AnalyzedObject&,
                             const std::string& );

};


TextureMapStatisticAnalyzerProcess::TextureMapStatisticAnalyzerProcess(
                         const std::vector< std::string >& theFileNameInputs,
                         const std::string& theFileNameOutputTexture,
                         const std::string& theFileNameRoiTexture,
                         const std::string& theFileNameRoiPairs,
                         const std::string& theFileNameCsvPyDistribution,
                         const std::string& theMode,
                         const bool& theAscii,
                         const std::string& theFormat,
                         const bool& theVerbose )
                          : gkg::Process( "TextureMap" ),
                            fileNameInputs( theFileNameInputs ),
                            fileNameOutputTexture( theFileNameOutputTexture ),
                            fileNameRoiTexture( theFileNameRoiTexture ),
                            fileNameRoiPairs( theFileNameRoiPairs ),
                            fileNameCsvPyDistribution(
                                                 theFileNameCsvPyDistribution ),
                            mode( theMode ),
                            ascii( theAscii ),
                            format( theFormat ),
                            verbose( theVerbose )
{

  try
  {

    registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< int8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< uint8_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< int16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< uint16_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< int32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< uint32_t > );
    registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< int64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< uint64_t > );
    registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< float > );
    registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeScalar< double > );

    registerProcess( "TextureMap", gkg::TypeOf< std::list< float > >::getName(),
                     &TextureMapStatisticAnalyzerProcess::
                      analyzeList< float > );


  }
  GKG_CATCH( "TextureMapStatisticAnalyzerProcess::"
             "TextureMapStatisticAnalyzerProcess( "
             "const std::vector< std::string >& theFileNameInputs, "
             "const std::string& theFileNameOutputTexture, "
             "const std::string& theFileNameRoiTexture, "
             "const std::string& theFileNameRoiPairs, "
             "const std::string& theFileNameCsvPyDistribution, "
             "const std::string& theMode, "
             "const bool& theAscii, "
             "const std::string& theFormat, "
             "const bool& theVerbose )" );

}


template < class T >
void TextureMapStatisticAnalyzerProcess::analyzeScalar(
                                         gkg::Process& process,
                                         const std::string& /* fileNameInput */,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    TextureMapStatisticAnalyzerProcess&
      textureMapStatisticAnalyzerProcess =
        static_cast< TextureMapStatisticAnalyzerProcess& >( process );

    const std::vector< std::string >& 
      fileNameInputs = textureMapStatisticAnalyzerProcess.fileNameInputs;
    const std::string& 
      fileNameOutputTexture =
                     textureMapStatisticAnalyzerProcess.fileNameOutputTexture;
    const std::string& 
      fileNameRoiTexture =
                     textureMapStatisticAnalyzerProcess.fileNameRoiTexture;
    const std::string& 
      mode = textureMapStatisticAnalyzerProcess.mode;
    const bool& 
      ascii = textureMapStatisticAnalyzerProcess.ascii;
    const std::string& 
      format = textureMapStatisticAnalyzerProcess.format;
    const bool& 
      verbose = textureMapStatisticAnalyzerProcess.verbose;

    int16_t textureCount = ( int16_t )fileNameInputs.size();

    ///////////////////////////////////////////////////////////////////////////
    // declaration of the statistics mapper vector
    ///////////////////////////////////////////////////////////////////////////
    std::vector< gkg::UpdatableMeanStandardDeviationMinMaxMedian >
      statisticsMapper;

    gkg::RCPointer< gkg::TextureMap< int16_t > > roiTextureMap;
    std::vector< int16_t > labels;
    std::map< int16_t, int16_t > labelToIndexLut;
    std::map< int16_t, int16_t > indexToLabelLut;
    int16_t roiCount = 0;

    ///////////////////////////////////////////////////////////////////////////
    // looping over input texture file names
    ///////////////////////////////////////////////////////////////////////////
    int16_t itemCount = 0;
    int16_t i = 0;
    for ( i = 0; i < textureCount; i++ )
    {

      /////////////////////////////////////////////////////////////////////////
      // reading the current input texture map 
      /////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading '" << fileNameInputs[ i ] << "' : "
                  << std::flush;

      }

      gkg::TextureMap< T > currentInputTexture;
      gkg::Reader::getInstance().read( fileNameInputs[ i ],
                                       currentInputTexture );

      /////////////////////////////////////////////////////////////////////////
      // retrieving the item count 
      /////////////////////////////////////////////////////////////////////////
      itemCount = currentInputTexture.getCount();

      if ( verbose )
      {

        std::cout << itemCount << " items" << std::endl;

      }

      /////////////////////////////////////////////////////////////////////////
      // resizing the vector if it's the first iteration 
      /////////////////////////////////////////////////////////////////////////
      if ( i == 0 )
      {

        if ( !fileNameRoiTexture.empty() )
        {

          roiTextureMap.reset( new gkg::TextureMap< int16_t > );
          gkg::Reader::getInstance().read( fileNameRoiTexture,
                                           *roiTextureMap );

          labels.resize( roiTextureMap->getCount() );

          gkg::TextureMap< int16_t >::const_iterator
            r = roiTextureMap->begin(),
            re = roiTextureMap->end();
          std::vector< int16_t >::iterator
            l = labels.begin();

          while ( r != re )
          {

            if ( labelToIndexLut.find( r->second ) == labelToIndexLut.end() )
            {

              labelToIndexLut.insert(
                        std::pair< int16_t, int16_t >( r->second, roiCount ) );
              indexToLabelLut.insert(
                        std::pair< int16_t, int16_t >( roiCount, r->second ) );
              ++ roiCount;

            }
            *l = r->second;

            ++ r;
            ++ l;

          }

          std::cout << "roiCount : " << roiCount << std::endl;

          statisticsMapper.resize( roiCount );

          std::map< int16_t, int16_t >::const_iterator
            li = labelToIndexLut.begin(),
            lie = labelToIndexLut.end();
          while ( li != lie )
          {

            std::cout << "region of label " << li->first << " -> "
                      << "index " << li->second
                      << std::endl;
            ++ li;

          }

        }
        else
        {

          statisticsMapper.resize( itemCount );

        }

      }

      //computing the statistics
      if ( verbose )
      {

        std::cout << "adding the current texture contribution : "
                  << std::flush;

      }

      if ( roiTextureMap.isNull() )
      {

        std::vector< gkg::UpdatableMeanStandardDeviationMinMaxMedian >::iterator
          sm = statisticsMapper.begin(),
          sme = statisticsMapper.end();
        typename gkg::TextureMap< T >::const_iterator
          it = currentInputTexture.begin();

        while ( sm != sme )
        {

          sm->add( ( double )( it->second ) );

        }
        ++ sm;
        ++ it;

      }
      else
      {

        typename gkg::TextureMap< T >::const_iterator
          it = currentInputTexture.begin(),
          ite = currentInputTexture.end();
        std::vector< int16_t >::const_iterator
          l = labels.begin();

        while ( it != ite )
        {

          const int16_t& label = *l;
          const int16_t& index = labelToIndexLut[ label ];
          statisticsMapper[ index ].add( ( double )( it->second ) );
          ++ it;
          ++ l;

        }

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

    ////////////////////////////////////////////////////////////////////////////
    // computing statistics
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "computing statistics:  " << std::flush;

    }

    gkg::TextureMap< T > outputTexture;
    std::vector< T > items;
    if ( mode == "all" )
    {

      items.resize( itemCount * 5, 0.0 );

      if ( roiTextureMap.isNull() )
      {

        gkg::Dictionary dictionary;
        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMean();
          items[ i + itemCount ] =
                       ( float )statisticsMapper[ i ].getStandardDeviation();
          items[ i + 2 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMinimum();
          items[ i + 3 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMaximum();
          items[ i + 4 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMedian();

          dictionary[ gkg::StringConverter::toString( i ) ] = items[ i ];

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int16_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int16_t& index = li->second;

          statistics[ index ].resize( 5 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMean();
          statistics[ index ][ 1 ] =
                              statisticsMapper[ index ].getStandardDeviation();
          statistics[ index ][ 2 ] = statisticsMapper[ index ].getMinimum();
          statistics[ index ][ 3 ] = statisticsMapper[ index ].getMaximum();
          statistics[ index ][ 4 ] = statisticsMapper[ index ].getMedian();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                           statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int16_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];
            items[ i + itemCount ] = ( float )statistics[ index ][ 1 ];
            items[ i + 2 * itemCount ] = ( float )statistics[ index ][ 2 ];
            items[ i + 3 * itemCount ] = ( float )statistics[ index ][ 3 ];
            items[ i + 4 * itemCount ] = ( float )statistics[ index ][ 4 ];

          }

        }

      }

      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int16_t >( 5, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 5 );
      std::vector< int16_t > aimsRank( 5 );
      aimsRank[ 0 ] = 0;
      aimsRank[ 1 ] = 1;
      aimsRank[ 2 ] = 2;
      aimsRank[ 3 ] = 3;
      aimsRank[ 4 ] = 4;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }

    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ///////////////////////////////////////////////////////////////////////////
    // writing output texture
    ///////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOutputTexture << "' : "
                << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTexture,
                                      outputTexture,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

  }
  GKG_CATCH( "template < class T > "
             "void TextureMapStatisticAnalyzerProcess::analyzeScalar( "
             "gkg::Process& process, "
             "const std::string& fileNameInput, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


template < class T >
void TextureMapStatisticAnalyzerProcess::analyzeList(
                                         gkg::Process& process,
                                         const std::string& /* fileNameInput */,
                                         const gkg::AnalyzedObject&,
                                         const std::string& )
{

  try
  {

    TextureMapStatisticAnalyzerProcess&
      textureMapStatisticAnalyzerProcess =
        static_cast< TextureMapStatisticAnalyzerProcess& >( process );

    const std::vector< std::string >& 
      fileNameInputs = textureMapStatisticAnalyzerProcess.fileNameInputs;
    const std::string& 
      fileNameOutputTexture =
                     textureMapStatisticAnalyzerProcess.fileNameOutputTexture;
    const std::string& 
      fileNameRoiTexture =
                     textureMapStatisticAnalyzerProcess.fileNameRoiTexture;
    const std::string& 
      fileNameRoiPairs =
                     textureMapStatisticAnalyzerProcess.fileNameRoiPairs;
    const std::string& 
      fileNameCsvPyDistribution =
                   textureMapStatisticAnalyzerProcess.fileNameCsvPyDistribution;
    const std::string& 
      mode = textureMapStatisticAnalyzerProcess.mode;
    const bool& 
      ascii = textureMapStatisticAnalyzerProcess.ascii;
    const std::string& 
      format = textureMapStatisticAnalyzerProcess.format;
    const bool& 
      verbose = textureMapStatisticAnalyzerProcess.verbose;

    int32_t textureCount = ( int32_t )fileNameInputs.size();

    ////////////////////////////////////////////////////////////////////////////
    // declaration of the statistics mapper vector
    ////////////////////////////////////////////////////////////////////////////
    std::vector< gkg::UpdatableMeanStandardDeviationMinMaxMedian >
      statisticsMapper;

    gkg::RCPointer< gkg::TextureMap< int16_t > > roiTextureMap;
    std::vector< int16_t > labels;
    std::map< int16_t, int32_t > labelToIndexLut;
    std::map< int32_t, int16_t > indexToLabelLut;
    int32_t roiCount = 0;

    ////////////////////////////////////////////////////////////////////////////
    // looping over input texture file names
    ////////////////////////////////////////////////////////////////////////////
    int32_t itemCount = 0;
    int32_t i = 0;
    for ( i = 0; i < textureCount; i++ )
    {

      //////////////////////////////////////////////////////////////////////////
      // reading the current input texture map 
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "reading '" << fileNameInputs[ i ]
                  << "' : " << std::flush;

      }

      gkg::TextureMap< std::list< T > > currentInputTexture;
      gkg::Reader::getInstance().read( fileNameInputs[ i ],
                                       currentInputTexture );

      //////////////////////////////////////////////////////////////////////////
      // retrieving the item count 
      //////////////////////////////////////////////////////////////////////////
      itemCount = currentInputTexture.getCount();

      if ( verbose )
      {

        std::cout << itemCount << " items" << std::endl;

      }

      //////////////////////////////////////////////////////////////////////////
      // resizing the vector if it's the first iteration 
      //////////////////////////////////////////////////////////////////////////
      if ( i == 0 )
      {


        if ( !fileNameRoiTexture.empty() )
        {

          roiTextureMap.reset( new gkg::TextureMap< int16_t > );
          gkg::Reader::getInstance().read( fileNameRoiTexture,
                                           *roiTextureMap );

          labels.resize( roiTextureMap->getCount() );

          gkg::TextureMap< int16_t >::const_iterator
            r = roiTextureMap->begin(),
            re = roiTextureMap->end();
          std::vector< int16_t >::iterator
            l = labels.begin();

          while ( r != re )
          {

            if ( labelToIndexLut.find( r->second ) == labelToIndexLut.end() )
            {

              labelToIndexLut.insert(
                         std::pair< int16_t, int32_t >( r->second, roiCount ) );
              indexToLabelLut.insert(
                         std::pair< int32_t, int16_t >( roiCount, r->second ) );
              ++ roiCount;

            }
            *l = r->second;

            ++ r;
            ++ l;

          }

          std::cout << "roiCount : " << roiCount << std::endl;

          statisticsMapper.resize( roiCount );

          std::map< int16_t, int32_t >::const_iterator
            li = labelToIndexLut.begin(),
            lie = labelToIndexLut.end();
          while ( li != lie )
          {

            std::cout << "region of label " << li->first << " -> "
                      << "index " << li->second
                      << std::endl;
            ++ li;

          }

        }
        else
        {

          statisticsMapper.resize( itemCount );

        }

      }

      //////////////////////////////////////////////////////////////////////////
      // adding contribution of current texture
      //////////////////////////////////////////////////////////////////////////
      if ( verbose )
      {

        std::cout << "adding contribution:  " << std::flush;

      }

      if ( roiTextureMap.isNull() )
      {

        std::vector< gkg::UpdatableMeanStandardDeviationMinMaxMedian >::iterator
          sm = statisticsMapper.begin(),
          sme = statisticsMapper.end();
        typename
        gkg::TextureMap< std::list< T > >::const_iterator
          it = currentInputTexture.begin();

        while ( sm != sme )
        {

          const std::list< T >& textures = it->second;
          // hack if the texture is empty
          if ( textures.size() == 0 )
          {

            sm->add( ( double )( 0.0 ) );

          }
          else
          {

            typename std::list< T >::const_iterator
              t = textures.begin(),
              te = textures.end();

            while ( t != te )
            {

              sm->add( ( double )( *t ) );
              ++t;

            }

          }
          ++ sm;
          ++ it;

        }

      }
      else
      {

        typename
        gkg::TextureMap< std::list< T > >::const_iterator
          it = currentInputTexture.begin(),
          ite = currentInputTexture.end();
        std::vector< int16_t >::const_iterator
          l = labels.begin();

        while ( it != ite )
        {

          const std::list< T >& textures = it->second;
          const int16_t& label = *l;
          const int32_t& index = labelToIndexLut[ label ];

          typename std::list< T >::const_iterator
            t = textures.begin(),
            te = textures.end();
          while ( t != te )
          {

            statisticsMapper[ index ].add( ( double )( *t ) );
            ++t;

          }
          ++ it;
          ++ l;

        }

      }
      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing Mann-Whitney Wilcoxon test on prescribed pairs of ROIs
    ////////////////////////////////////////////////////////////////////////////
    if ( !fileNameRoiPairs.empty() && !roiTextureMap.isNull() )
    {


      if ( verbose )
      {

        std::cout << "reading pairs : "
                  << std::flush;

      }

      std::list< std::pair< int16_t, int16_t > > pairs;
      std::ifstream is( fileNameRoiPairs.c_str() );

      if ( !is )
      {

        throw std::runtime_error( std::string( "unable to open file '" ) +
                                  fileNameRoiPairs + "'" );

      }

      while ( !is.eof() )
      {

        int16_t label1 = 0;
        is >> label1;

        if ( is.eof() )
        {

          break;

        }

        int16_t label2 = 0;
        is >> label2;

        pairs.push_back( std::pair< int16_t, int16_t >( label1, label2 ) );

      }

      is.close();

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }


      // Mann-Whitney Wilcoxon test CSV file
      if ( verbose )
      {

        std::cout << "computing Mann-Whitney Wilcoxon tests  : "
                  << std::flush;

      }

      std::ofstream os( ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
                          "_MannWhitneyWilcoxonTest.csv" ).c_str() );

      gkg::MannWhitneyWilcoxonUTest mannWhitneyWilcoxonUTest;

      std::list< std::pair< int16_t, int16_t > >::const_iterator
        p = pairs.begin(),
        pe = pairs.end();
      while ( p != pe )
      { 

        if ( verbose )
        {

          std::cout << "( " << p->first << ", "
                    << p->second << " ) " << std::flush;

        }

        gkg::MannWhitneyWilcoxonUTest::Result
          result = mannWhitneyWilcoxonUTest.test( 
                 statisticsMapper[ labelToIndexLut[ p->first ] ].getValues(),
                 statisticsMapper[ labelToIndexLut[ p->second ] ].getValues() );
        os << p->first << " "
           << p->second << " "
           << result.twoTailedTestPValue << " "
           << result.leftTailedTestPValue << " "
           << result.rightTailedTestPValue << std::endl;

        ++ p;

      }
      os.close();

      if ( verbose )
      {

        std::cout << std::endl;

      }

      // Mann-Whitney Wilcoxon test python files
      if ( verbose )
      {

        std::cout << "building Mann-Whitney Wilcoxon test python files  : "
                  << std::flush;

      }

      p = pairs.begin();
      while ( p != pe )
      { 

        if ( verbose )
        {

          std::cout << "( " << p->first << ", "
                    << p->second << " ) " << std::flush;

        }

        std::ofstream os( 
          ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
            "_" + gkg::StringConverter::toString( p->first ) +
            "_" + gkg::StringConverter::toString( p->second ) +
            "_MannWhitneyWilcoxonTest.py" ).c_str() );

        os << "import numpy" << std::endl;
        os << "import scipy.stats" << std::endl;
        os << "import matplotlib.pyplot as plt" << std::endl;
        os << std::endl;

        os << "data_" << gkg::StringConverter::toString( p->first )
           << " = [ " << std::flush;

        const std::list< double >& 
          values1 = statisticsMapper[ labelToIndexLut[ p->first ] ].getValues();

        std::list< double >::const_iterator
          v1 = values1.begin(),
          v1e = values1.end();
        while ( v1 != v1e )
        {

          os << *v1 << ", " << std::flush;
          ++ v1;

        }
        os << " ]" << std::endl;

        os << "data_" << gkg::StringConverter::toString( p->second )
           << " = [ " << std::flush;

        const std::list< double >& 
          values2 = statisticsMapper[
                                     labelToIndexLut[ p->second ] ].getValues();

        std::list< double >::const_iterator
          v2 = values2.begin(),
          v2e = values2.end();
        while ( v2 != v2e )
        {

          os << *v2 << ", " << std::flush;
          ++ v2;

        }
        os << " ]" << std::endl;

        os << std::endl;
        os << "print \"Shapiro-Wilk p-value for "
           << "data_" + gkg::StringConverter::toString( p->first ) + "\", "
           << "scipy.stats.shapiro( "
           << "data_" + gkg::StringConverter::toString( p->first ) + " )[ 1 ]"
           << std::endl;

        os << std::endl;
        os << "print \"Shapiro-Wilk p-value for "
           << "data_" + gkg::StringConverter::toString( p->second ) + "\", "
           << "scipy.stats.shapiro( "
           << "data_" + gkg::StringConverter::toString( p->second ) + " )[ 1 ]"
           << std::endl;

        os << std::endl;
        os << "print \"Welch's T-Test p-value : \", scipy.stats.ttest_ind( "
           << "data_" + gkg::StringConverter::toString( p->first ) + ", "
           << "data_" + gkg::StringConverter::toString( p->second ) + ", "
           << "equal_var=False )[ 1 ]"
           << std::endl;

        os << std::endl;
        os << "print \"Mann-Whitney U p-value : \", scipy.stats.mannwhitneyu( "
           << "data_" + gkg::StringConverter::toString( p->first ) + ", "
           << "data_" + gkg::StringConverter::toString( p->second ) + " )[ 1 ]"
           << std::endl;

        os << std::endl;
        os << "fig = plt.figure()" << std::endl;
        os << "sub = fig.add_subplot( 111 )" << std::endl;
        os << "sub.boxplot( [ data_" +
               gkg::StringConverter::toString( p->first ) + ", "
           << "data_" +
               gkg::StringConverter::toString( p->second ) + " ], "
           << " whis=1)"
           << std::endl;
        os << "sub.set_xticklabels(( \"" +
               gkg::StringConverter::toString( p->first ) + "\", \"" +
               gkg::StringConverter::toString( p->second ) + "\"))"
           << std::endl;
        os << "sub.set_title( \"" +
               gkg::StringConverter::toString( p->first ) + " vs " +
               gkg::StringConverter::toString( p->second ) + "\" )"
           << std::endl;

        os << "plt.show()" << std::endl;

        os.close();


        ++ p;

      }

      if ( verbose )
      {

        std::cout << std::endl;

      }

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing statistics
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "computing statistics:  " << std::flush;

    }

    gkg::TextureMap< float > outputTexture;
    std::vector< float > items;
    if ( mode == "all" )
    {

      items.resize( itemCount * 5, 0.0 );

      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMean();
          items[ i + itemCount ] =
                       ( float )statisticsMapper[ i ].getStandardDeviation();
          items[ i + 2 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMinimum();
          items[ i + 3 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMaximum();
          items[ i + 4 * itemCount ] =
                       ( float )statisticsMapper[ i ].getMedian();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 5 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMean();
          statistics[ index ][ 1 ] = statisticsMapper[ index ].getStandardDeviation();
          statistics[ index ][ 2 ] = statisticsMapper[ index ].getMinimum();
          statistics[ index ][ 3 ] = statisticsMapper[ index ].getMaximum();
          statistics[ index ][ 4 ] = statisticsMapper[ index ].getMedian();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];
            items[ i + itemCount ] = ( float )statistics[ index ][ 1 ];
            items[ i + 2 * itemCount ] = ( float )statistics[ index ][ 2 ];
            items[ i + 3 * itemCount ] = ( float )statistics[ index ][ 3 ];
            items[ i + 4 * itemCount ] = ( float )statistics[ index ][ 4 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 5, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 5 );
      std::vector< int32_t > aimsRank( 5 );
      aimsRank[ 0 ] = 0;
      aimsRank[ 1 ] = 1;
      aimsRank[ 2 ] = 2;
      aimsRank[ 3 ] = 3;
      aimsRank[ 4 ] = 4;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "mean" )
    {

      items.resize( itemCount, 0.0 );

      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMean();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 1 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMean();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 1, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 1 );
      std::vector< int32_t > aimsRank( 1 );
      aimsRank[ 0 ] = 0;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "stddev" )
    {

      items.resize( itemCount, 0.0 );

      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getStandardDeviation();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 1 );
        
          statistics[ index ][ 0 ] =
                               statisticsMapper[ index ].getStandardDeviation();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 1, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 1 );
      std::vector< int32_t > aimsRank( 1 );
      aimsRank[ 0 ] = 0;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "min" )
    {

      items.resize( itemCount, 0.0 );

      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMinimum();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 1 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMinimum();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 1, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 1 );
      std::vector< int32_t > aimsRank( 1 );
      aimsRank[ 0 ] = 0;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "max" )
    {

      items.resize( itemCount, 0.0 );
      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMaximum();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 1 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMaximum();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 1, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 1 );
      std::vector< int32_t > aimsRank( 1 );
      aimsRank[ 0 ] = 0;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }
    else if ( mode == "median" )
    {

      items.resize( itemCount, 0.0 );
      if ( roiTextureMap.isNull() )
      {

        for ( i = 0; i < itemCount; i++ )
        {

          items[ i ] = ( float )statisticsMapper[ i ].getMedian();

        }

      }
      else
      {

        std::vector< std::vector< double > > statistics( roiCount );
        gkg::Dictionary dictionary;

        std::map< int16_t, int32_t >::const_iterator
          li = labelToIndexLut.begin(),
          lie = labelToIndexLut.end();
        while ( li != lie )
        {

          const int16_t& label = li->first;
          const int32_t& index = li->second;

          statistics[ index ].resize( 1 );
        
          statistics[ index ][ 0 ] = statisticsMapper[ index ].getMedian();

          dictionary[ gkg::StringConverter::toString( label ) ] = 
                                                            statistics[ index ];

          ++ li;

        }
        outputTexture.getHeader().addAttribute( "statistics", dictionary );

        for ( i = 0; i < itemCount; i++ )
        {

          if ( labels[ i ] )
          {

            const int32_t& index = labelToIndexLut[ labels[ i ] ];
            items[ i ] = ( float )statistics[ index ][ 0 ];

          }

        }

      }
      outputTexture.addTextures( items );

      outputTexture.getHeader().addAttribute(
                                "aims_item_count",
                                std::vector< int32_t >( 1, itemCount ) );
      outputTexture.getHeader().addAttribute( "aims_rank_count", 1 );
      std::vector< int32_t > aimsRank( 1 );
      aimsRank[ 0 ] = 0;
      outputTexture.getHeader().addAttribute( "aims_rank", aimsRank );

    }


    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // writing output texture
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "writing '" << fileNameOutputTexture << "' : " << std::flush;

    }
    gkg::Writer::getInstance().write( fileNameOutputTexture,
                                      outputTexture,
                                      ascii,
                                      format );
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // saving individual distributions
    ////////////////////////////////////////////////////////////////////////////
    if ( !fileNameCsvPyDistribution.empty() )
    {

      if ( verbose )
      {

        std::cout << "writing '" << fileNameCsvPyDistribution << "' : "
                  << std::flush;

      }

      std::vector< gkg::UpdatableMeanStandardDeviationMinMaxMedian >::
        const_iterator
        sm = statisticsMapper.begin(),
        sme = statisticsMapper.end();
      int32_t index = 0;
      while ( sm != sme )
      {

        const std::list< double >&
          values = sm->getValues();


        std::ofstream os;

        if ( roiTextureMap.isNull() )
        {


          os.open( ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
                     "_" +
                     gkg::StringConverter::toString( index ) +
                     ".csv" ).c_str() );

        }
        else
        {


          os.open( ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
                     "_" +
                     gkg::StringConverter::toString(
                                                  indexToLabelLut[ index ] ) +
                     ".csv" ).c_str() );

        }


        std::list< double >::const_iterator
          v = values.begin(),
          ve = values.end();

        while ( v != ve )
        {

          os << *v << std::endl;
          ++ v;

        }

        os.close();


        if ( roiTextureMap.isNull() )
        {


          os.open( ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
                     "_" +
                     gkg::StringConverter::toString( index ) +
                     ".py" ).c_str() );

        }
        else
        {


          os.open( ( getNameWithoutExtension( fileNameCsvPyDistribution ) +
                     "_" +
                     gkg::StringConverter::toString(
                                                  indexToLabelLut[ index ] ) +
                     ".py" ).c_str() );

        }

        os << "import numpy" << std::endl;
        os << "import matplotlib" << std::endl;
        os << "import matplotlib.pyplot" << std::endl;
        os << std::endl;
        os << "data = [ " << std::flush;
        
        v = values.begin();
        while ( v != ve )
        {

          os << *v << ", " << std::flush;
          ++ v;

        }
        os << " ]" << std::endl;

        os << std::endl;
        os << "binCount = 100" << std::endl;
        os << "n, bins, patches = matplotlib.pyplot.hist( data, "
           << std::endl;
        os << "                                           binCount,"
           << std::endl;
        os << "                                           normed=1,"
           << std::endl;
        os << "                                           facecolor='green',"
           << std::endl;
        os << "                                           alpha=0.5 )"
           << std::endl;

        os << "matplotlib.pyplot.show()" << std::endl;

        os.close();

        ++ sm;
        ++ index;

      }

      if ( verbose )
      {

        std::cout << "done" << std::endl;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void TextureMapStatisticAnalyzerProcess::analyzeList( "
             "gkg::Process& process, "
             "const std::string& fileNameInput, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//  TextureMapStatisticAnalyzerCommand
//


gkg::TextureMapStatisticAnalyzerCommand::TextureMapStatisticAnalyzerCommand(
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
  GKG_CATCH( "gkg::TextureMapStatisticAnalyzerCommand::"
             "TextureMapStatisticAnalyzerCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TextureMapStatisticAnalyzerCommand::TextureMapStatisticAnalyzerCommand(
                               const std::vector< std::string >& fileNameInputs,
                               const std::string& fileNameOutputTexture,
                               const std::string& fileNameRoiTexture,
                               const std::string& fileNameRoiPairs,
                               const std::string& fileNameCsvPyDistribution,
                               const std::string& mode,
                               bool ascii,
                               const std::string& format,
                               bool verbose )
                                       : gkg::Command()
{

  try
  {

    execute( fileNameInputs, fileNameOutputTexture, fileNameRoiTexture,
             fileNameRoiPairs, fileNameCsvPyDistribution, mode, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapStatisticAnalyzerCommand::"
             "TextureMapStatisticAnalyzerCommand( "
             "const std::vector< std::string >& fileNameInputs, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& fileNameRoiTexture, "
             "const std::string& fileNameRoiPairs, "
             "const std::string& fileNameCsvPyDistribution, "
             "const std::string& mode, bool ascii, const std::string& format, "
             "bool verbose )" );

}


gkg::TextureMapStatisticAnalyzerCommand::TextureMapStatisticAnalyzerCommand(
                                             const gkg::Dictionary& parameters )
                                       : gkg::Command( parameters )
{

  try
  {

    DECLARE_VECTOR_OF_STRINGS_PARAMETER( parameters,
                                         std::vector< std::string >,
                                         fileNameInputs );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameOutputTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRoiTexture );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameRoiPairs );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameCsvPyDistribution );
    DECLARE_STRING_PARAMETER( parameters, std::string, mode );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameInputs, fileNameOutputTexture, fileNameRoiTexture,
             fileNameRoiPairs, fileNameCsvPyDistribution, mode, ascii, format,
             verbose );

  }
  GKG_CATCH( "gkg::TextureMapStatisticAnalyzerCommand::"
             "TextureMapStatisticAnalyzerCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TextureMapStatisticAnalyzerCommand::~TextureMapStatisticAnalyzerCommand()
{
}


std::string gkg::TextureMapStatisticAnalyzerCommand::getStaticName()
{

  try
  {

    return "TextureMapStatisticAnalyzer";

  }
  GKG_CATCH( 
       "std::string gkg::TextureMapStatisticAnalyzerCommand::getStaticName()" );

}


void gkg::TextureMapStatisticAnalyzerCommand::parse()
{

  try
  {

    std::vector< std::string > fileNameInputs;
    std::string fileNameOutputTexture;
    std::string fileNameRoiTexture;
    std::string fileNameRoiPairs;
    std::string fileNameCsvPyDistribution;
    std::string mode = "all";
    bool ascii = false;
    std::string format = "";
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Texture map statistic analyzer",
                                  _loadPlugin );
    application.addSeriesOption( "-i",
                                 "Input texture map file names",
                                 fileNameInputs,
                                 1 );
    application.addSingleOption( "-o",
                                 "Output texture file name (can be 4D and"
                                 "include mean/stddev/min/max/median "
                                 "information)",
                                 fileNameOutputTexture );
    application.addSingleOption( "-M",
                                 "Surface ROI texture map in int16_t",
                                 fileNameRoiTexture,
                                 true );
    application.addSingleOption( "-P",
                                 "CSV text file of <N> ROI pairs to be tested "
                                 "using a Mann-Withney Wilcoxon test "
                                 "with the following structure: \n"
                                 "label1_1 label2_1\n"
                                 "........ ........\n"
                                 "label1_i label2_i\n"
                                 "........ ........\n"
                                 "label1_N label2_N\n",
                                 fileNameRoiPairs,
                                 true );
    application.addSingleOption( "-d",
                                 "Output CSV distribution file name",
                                 fileNameCsvPyDistribution,
                                 true );
    application.addSingleOption( "-m",
                                 "Output mode: \n"
                                 "- mean -> mean texture\n"
                                 "- stddev -> standard deviation texture\n"
                                 "- min -> minimum texture\n"
                                 "- max -> maximum texture\n"
                                 "- median -> median texture\n"
                                 "- all -> mean/stddev/min/max/median 4D "
                                 "texture\n (default=all)",
                                 mode,
                                 true );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=input)",
                                 format,
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

    execute( fileNameInputs, fileNameOutputTexture, fileNameRoiTexture,
             fileNameRoiPairs, fileNameCsvPyDistribution, mode, ascii, format,
             verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TextureMapStatisticAnalyzerCommand::parse()" );

}


void gkg::TextureMapStatisticAnalyzerCommand::execute(
                  	       const std::vector< std::string >& fileNameInputs,
                               const std::string& fileNameOutputTexture,
                               const std::string& fileNameRoiTexture,
                               const std::string& fileNameRoiPairs,
                               const std::string& fileNameCsvPyDistribution,
                               const std::string& mode,
                               bool ascii,
                               const std::string& format,
                               bool verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( !( ( mode == "all" ) ||
            ( mode == "mean" ) ||
            ( mode == "stddev" ) ||
            ( mode == "min" ) ||
            ( mode == "max" ) ||
            ( mode == "median" ) ) )
    {

      throw std::runtime_error(
                  "output mode must be one of all/mean/stddev/min/max/median" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // launching process
    ////////////////////////////////////////////////////////////////////////////
    TextureMapStatisticAnalyzerProcess textureMapStatisticAnalyzerProcess(
                                          	      fileNameInputs,
                                          	      fileNameOutputTexture,
                                          	      fileNameRoiTexture,
                                          	      fileNameRoiPairs,
                                          	      fileNameCsvPyDistribution,
                                          	      mode,
                                          	      ascii,
                                          	      format,
                                          	      verbose );
    textureMapStatisticAnalyzerProcess.execute( fileNameInputs[ 0 ] );

  }
  GKG_CATCH( "void gkg::TextureMapStatisticAnalyzerCommand::execute( "
             "const std::vector< std::string >& fileNameInputs, "
             "const std::string& fileNameOutputTexture, "
             "const std::string& fileNameRoiTexture, "
             "const std::string& fileNameRoiPairs, "
             "const std::string& fileNameCsvPyDistribution, "
             "const std::string& mode, bool ascii, const std::string& format, "
             "bool verbose )" );

}


RegisterCommandCreator(
                    TextureMapStatisticAnalyzerCommand,
    		    DECLARE_VECTOR_OF_STRINGS_PARAMETER_HELP( fileNameInputs ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameOutputTexture ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameRoiTexture ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameRoiPairs ) +
    		    DECLARE_STRING_PARAMETER_HELP( fileNameCsvPyDistribution ) +
    		    DECLARE_STRING_PARAMETER_HELP( mode ) +
    		    DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		    DECLARE_STRING_PARAMETER_HELP( format ) +
    		    DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
