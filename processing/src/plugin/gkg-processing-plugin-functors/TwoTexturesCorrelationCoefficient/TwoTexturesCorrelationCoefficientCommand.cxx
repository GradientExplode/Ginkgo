#include <gkg-processing-plugin-functors/TwoTexturesCorrelationCoefficient/TwoTexturesCorrelationCoefficientCommand.h>
#include <gkg-communication-command/CommandFactory.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-container/MeshMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-process/Process.h>
#include <gkg-processing-algobase/AverageFilter_i.h>
#include <gkg-processing-algobase/UpdatableMeanStandardDeviationMinMax.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Eraser.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-core-exception/Exception.h>

#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>


////////////////////////////////////////////////////////////////////////////////
// computeCorrelationCoefficient()
////////////////////////////////////////////////////////////////////////////////
float computeCorrelationCoefficient( const std::vector< float >& texture1s,
                                     const std::vector< float >& texture2s )
{

  try
  {

    //   CC = sum( ( x - <x> ) ( y - <y> ) ) /
    //        sqrt( sum( ( x - <x> )^2 ) ) x
    //        sqrt( sum( ( y - <y> )^2 ) )


    static gkg::AverageFilter< std::vector< float >, float > averageFilter;

    float meanOfTexture1 = 0.0;
    float meanOfTexture2 = 0.0;
    averageFilter.filter( texture1s, meanOfTexture1 );
    averageFilter.filter( texture2s, meanOfTexture2 );

    float deviation1 = 0.0;
    float deviation2 = 0.0;
    float productOfTexture1AndTexture2 = 0.0;
    float sumOfSquareTexture1 = 0.0;
    float sumOfSquareTexture2 = 0.0;

    std::vector< float >::const_iterator
      t1 = texture1s.begin(),
      t1e = texture1s.end();
    std::vector< float >::const_iterator
      t2 = texture2s.begin();

    while ( t1 != t1e )
    {

      deviation1 = *t1 - meanOfTexture1;
      deviation2 = *t2 - meanOfTexture2;

      productOfTexture1AndTexture2 += deviation1 * deviation2;

      sumOfSquareTexture1 += deviation1 * deviation1;
      sumOfSquareTexture2 += deviation2 * deviation2;

      ++ t1;
      ++ t2;

    }

    if ( ( sumOfSquareTexture1 == 0.0 ) || ( sumOfSquareTexture2 == 0.0 ) )
    {

      return 0.0;

    }
    return productOfTexture1AndTexture2 / 
           ( std::sqrt( sumOfSquareTexture1 ) *
             std::sqrt( sumOfSquareTexture2 ) );

  }
  GKG_CATCH( "float computeCorrelationCoefficient( "
             "const std::vector< float >& texture1s, "
             "const std::vector< float >& texture2s )" );

}
                                     


////////////////////////////////////////////////////////////////////////////////
// output filename generators
////////////////////////////////////////////////////////////////////////////////

std::string getFileNameTextureMap( const std::string& fileName,
                                   const int16_t& label )
{

  try
  {

    std::string fileNameOut;
    if ( fileName.substr( fileName.length() - 4U, 4U ) == ".tex" )
    {

      fileNameOut = fileName.substr( 0U, fileName.length() - 4U ) +
                    gkg::StringConverter::toString( label ) +
                    ".tex";

    }
    else if ( fileName.substr( fileName.length() - 11U, 11U ) ==
              ".texturemap" )
    {

      fileNameOut = fileName.substr( 0U, fileName.length() - 11U ) +
                    gkg::StringConverter::toString( label ) +
                    ".texturemap";

    }
    else
    {

      fileNameOut = fileName +
                    gkg::StringConverter::toString( label );


    }

    return fileNameOut;

  }
  GKG_CATCH( "std::string getFileNameTextureMap( "
             "const std::string& fileName, "
             "const int16_t& label )" );

}


std::string getFileNameText( const std::string& fileName,
                             const int16_t& label )
{

  try
  {

    std::string fileNameOut;
    if ( fileName.substr( fileName.length() - 4U, 4U ) == ".txt" )
    {

      fileNameOut = fileName.substr( 0U, fileName.length() - 4U ) +
                    gkg::StringConverter::toString( label ) +
                    ".txt";

    }
    else
    {

      fileNameOut = fileName +
                    gkg::StringConverter::toString( label ) +
                    ".txt";


    }

    return fileNameOut;

  }
  GKG_CATCH( "std::string getFileNameText( "
             "const std::string& fileName, "
             "const int16_t& label )" );

}



////////////////////////////////////////////////////////////////////////////////
// class LocalCCSubProcess
////////////////////////////////////////////////////////////////////////////////

template < class T1 >
class LocalCCSubProcess : public gkg::Process
{

  public:

    LocalCCSubProcess( const std::string& theFileNameTextureMap2,
                       const std::string& theFileNameTextureMapRois,
                       const std::string& theFileNameMeshMap,
                       const std::string& theFileNameTextureMapCCOut,
                       const std::string& theFileNameTextureMapRoiOut,
                       const std::string& theFileNameStatisticsOut,
                       const int32_t& theRank1,
                       const int32_t& theRank2,
                       const int32_t& theRankRois,
                       const float& theDistance,
                       const bool& theAscii,
                       const std::string& theFormat,
                       const bool& theVerbose );

    const std::string& fileNameTextureMap2;
    const std::string& fileNameTextureMapRois;
    const std::string& fileNameMeshMap;
    const std::string& fileNameTextureMapCCOut;
    const std::string& fileNameTextureMapRoiOut;
    const std::string& fileNameStatisticsOut;
    const int32_t& rank1;
    const int32_t& rank2;
    const int32_t& rankRois;
    const float& distance;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;

  private:

    template < class T2 >
    static void process( gkg::Process& process,
                         const std::string& fileNameTextureMap1,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


template < class T1 >
LocalCCSubProcess< T1 >::LocalCCSubProcess(
                                const std::string& theFileNameTextureMap2,
                                const std::string& theFileNameTextureMapRois,
                                const std::string& theFileNameMeshMap,
                                const std::string& theFileNameTextureMapCCOut,
                                const std::string& theFileNameTextureMapRoiOut,
                                const std::string& theFileNameStatisticsOut,
                                const int32_t& theRank1,
                                const int32_t& theRank2,
                                const int32_t& theRankRois,
                                const float& theDistance,
                                const bool& theAscii,
                                const std::string& theFormat,
                                const bool& theVerbose )
               : gkg::Process( "TextureMap" ),
                 fileNameTextureMap2( theFileNameTextureMap2 ),
                 fileNameTextureMapRois( theFileNameTextureMapRois ),
                 fileNameMeshMap( theFileNameMeshMap ),
                 fileNameTextureMapCCOut( theFileNameTextureMapCCOut ),
                 fileNameTextureMapRoiOut( theFileNameTextureMapRoiOut ),
                 fileNameStatisticsOut( theFileNameStatisticsOut ),
                 rank1( theRank1 ),
                 rank2( theRank2 ),
                 rankRois( theRankRois ),
                 distance( theDistance ),
                 ascii( theAscii ),
                 format( theFormat ),
                 verbose( theVerbose )
{

  registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                   &LocalCCSubProcess::template process< int8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                   &LocalCCSubProcess::template process< uint8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                   &LocalCCSubProcess::template process< int16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                   &LocalCCSubProcess::template process< uint16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                   &LocalCCSubProcess::template process< int32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                   &LocalCCSubProcess::template process< uint32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                   &LocalCCSubProcess::template process< int64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                   &LocalCCSubProcess::template process< uint64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                   &LocalCCSubProcess::template process< float > );
  registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                   &LocalCCSubProcess::template process< double > );

}


template < class T1 > template < class T2 >
void 
LocalCCSubProcess< T1 >::process( gkg::Process& process,
                                  const std::string& fileNameTextureMap1,
                                  const gkg::AnalyzedObject&,
                                  const std::string& )
{

  try
  {

    LocalCCSubProcess< T1 >&
      localCCSubProcess = static_cast< LocalCCSubProcess< T1 >& >( process );

    ////////////////////////////////////////////////////////////////////////////
    // reading 1st texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "reading '" <<  fileNameTextureMap1  << "' : " << std::flush;

    }
    gkg::TextureMap< T1 > textureMap1;
    gkg::Reader::getInstance().read( fileNameTextureMap1,
                                     textureMap1 );
    if ( localCCSubProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading 2nd texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "reading '"
                << localCCSubProcess.fileNameTextureMap2
                << "' : " << std::flush;

    }
    gkg::TextureMap< T2 > textureMap2;
    gkg::Reader::getInstance().read( localCCSubProcess.fileNameTextureMap2,
                                     textureMap2 );
    if ( localCCSubProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // reading ROIs texture map
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "reading '" 
                <<  localCCSubProcess.fileNameTextureMapRois
                << "' : " << std::flush;

    }
    gkg::TextureMap< int16_t > textureMapRois;
    gkg::Reader::getInstance().read( localCCSubProcess.fileNameTextureMapRois,
                                     textureMapRois );
    if ( localCCSubProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // reading mesh map
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "reading '" 
                <<  localCCSubProcess.fileNameMeshMap
                << "' : " << std::flush;

    }
    gkg::MeshMap< int32_t, float, 3 > meshMap;
    gkg::Reader::getInstance().read( localCCSubProcess.fileNameMeshMap,
                                     meshMap );
    int32_t vertexCount = meshMap.vertices.getSiteCount( 0 );
    if ( localCCSubProcess.verbose )
    {

      std::cout << vertexCount << " vertices" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // sanity checks
    ////////////////////////////////////////////////////////////////////////////

    if ( meshMap.vertices.getRankCount() != 1 )
    {

      throw std::runtime_error( "mesh map does not have a unique rank count" );

    }

    ////////////////////////////////////////////////////////////////////////////
    // extracting vertex geodesic neighborhoods
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "extracting mesh neighborhoods of vertices at " 
                <<  localCCSubProcess.distance
                << " mm : " << std::flush;

    }
    std::map< int32_t, std::set< int32_t > >
      vertexNeighborMap = meshMap.getVertexNeighbors( 0,
                                                   localCCSubProcess.distance );
    gkg::UpdatableMeanStandardDeviationMinMax neighborhoodStatistics;
    std::map< int32_t, std::set< int32_t > >::const_iterator
      ns = vertexNeighborMap.begin(),
      nse = vertexNeighborMap.end();
    while ( ns != nse )
    {

      neighborhoodStatistics.add( ( float )ns->second.size() );
      ++ ns;

    }
    if ( localCCSubProcess.verbose )
    {

      std::cout << " mean=" << neighborhoodStatistics.getMean()
                << " stddev=" << neighborhoodStatistics.getStandardDeviation()
                << " min=" << neighborhoodStatistics.getMinimum()
                << " max=" << neighborhoodStatistics.getMaximum()
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // collecting vertex indices for each ROI
    ////////////////////////////////////////////////////////////////////////////

    if ( localCCSubProcess.verbose )
    {

      std::cout << "collecting vertex indices for each ROI : " << std::flush;

    }
    std::map< int16_t, std::set< int32_t > > roiIndexMap;
    gkg::TextureMap< int16_t >::const_iterator
      tr = textureMapRois.begin(),
      tre = textureMapRois.end();
    int32_t index = 0;
    while ( tr != tre )
    {

      if ( tr->second > 0 )
      {

        roiIndexMap[ tr->second ].insert( index );

      }
      ++ tr;
      ++ index;

    }
    if ( localCCSubProcess.verbose )
    {

      std::cout << "done" << std::endl;

    }


    ////////////////////////////////////////////////////////////////////////////
    // computing local correlation coefficients
    ////////////////////////////////////////////////////////////////////////////


    if ( localCCSubProcess.verbose )
    {

      std::cout << "computing local correlation coefficient : " << std::flush;

    }

    int32_t offsetIndex1 = localCCSubProcess.rank1 * vertexCount;
    int32_t offsetIndex2 = localCCSubProcess.rank2 * vertexCount;

    int32_t roiIndex = 0;
    int32_t roiCount = ( int32_t )roiIndexMap.size();


    std::map< int16_t, gkg::UpdatableMeanStandardDeviationMinMax > 
      mapOfUpdatableMeanStandardDeviationMinMax; 

    std::map< int16_t, std::set< int32_t > >::const_iterator
      r = roiIndexMap.begin(),
      re = roiIndexMap.end();

    while ( r != re )
    {

      // getting current label and ROI vertices ////////////////////////////////
      int16_t label = r->first;
      const std::set< int32_t >& vertexIndices = r->second;

      if ( localCCSubProcess.verbose )
      {

        if ( roiIndex != 0 )
        {

          std::cout << gkg::Eraser( 21 );

        }
        std::cout << " [ " << std::setw( 3 ) << roiIndex + 1 
                  << "(" << std::setw( 5 ) << label << ")"
        	  << " / " << std::setw( 3 ) << roiCount
        	  << " ]" << std::flush;


      }

      // allocating outputs ////////////////////////////////////////////////////
      std::vector< float > correlationCoefficients( vertexCount, 0.0f );
      std::vector< float > textureRoi( vertexCount, 0.0f );

      // looping of vertices of the ROI ////////////////////////////////////////
      std::set< int32_t >::const_iterator
        v = vertexIndices.begin(),
        ve = vertexIndices.end();
      while ( v != ve )
      {

        ns = vertexNeighborMap.find( *v );

        if ( ns != vertexNeighborMap.end() )
        {

          const std::set< int32_t >& vertexNeighbors = ns->second;

          std::vector< float >
            texture1s( ( int32_t )vertexNeighbors.size() + 1 );
          std::vector< float >
            texture2s( ( int32_t )vertexNeighbors.size() + 1 );

          index = 0;
          texture1s[ index ] = textureMap1.getTexture( offsetIndex1 + *v );
          texture2s[ index ] = textureMap2.getTexture( offsetIndex2 + *v );
          ++ index;

          std::set< int32_t >::const_iterator
            n = vertexNeighbors.begin(),
            ne = vertexNeighbors.end();
          while ( n != ne )
          {

            texture1s[ index ] = textureMap1.getTexture( *n );
            texture2s[ index ] = textureMap2.getTexture( *n );

            ++ n;
            ++ index;

          }

          float cc = computeCorrelationCoefficient( texture1s, texture2s );
          correlationCoefficients[ *v ] = cc; 
          textureRoi[ *v ] = 1.0;
          mapOfUpdatableMeanStandardDeviationMinMax[ label ].add ( cc );

        }

        ++ v;

      }

      // writing CC texture map ////////////////////////////////////////////////
      if ( localCCSubProcess.verbose )
      {

        std::cout << " writing CC texture" << std::flush;

      }

      gkg::TextureMap< float > textureMapCC;
      textureMapCC.setTextures( correlationCoefficients );


      gkg::Writer::getInstance().write(
        getFileNameTextureMap( localCCSubProcess.fileNameTextureMapCCOut,
                               label ),
        textureMapCC,
        localCCSubProcess.ascii,
        localCCSubProcess.format );

      if ( localCCSubProcess.verbose )
      {

        std::cout << gkg::Eraser( 19 ) << std::flush;

      }

      // writing ROI texture map ///////////////////////////////////////////////
      if ( localCCSubProcess.verbose )
      {

        std::cout << " writing ROI texture" << std::flush;

      }

      gkg::TextureMap< float > textureMapRoi;
      textureMapRoi.setTextures( textureRoi );

      gkg::Writer::getInstance().write(
        getFileNameTextureMap( localCCSubProcess.fileNameTextureMapRoiOut, 
                               label ),
        textureMapRoi,
        localCCSubProcess.ascii,
        localCCSubProcess.format );


      if ( localCCSubProcess.verbose )
      {

        std::cout << gkg::Eraser( 20 ) << std::flush;

      }

      // writing statistics ////////////////////////////////////////////////////
      if ( localCCSubProcess.verbose )
      {

        std::cout << " writing statistics" << std::flush;

      }

      std::ofstream 
        os( getFileNameText( localCCSubProcess.fileNameStatisticsOut, 
                             label ).c_str() );
      
      os << "label: " << label << std::endl;
      os << "vertex count:"
         << mapOfUpdatableMeanStandardDeviationMinMax[ label ].getCount()
         << std::endl;
      os << "mean: "
         << mapOfUpdatableMeanStandardDeviationMinMax[ label ].getMean()
         << std::endl;
      os << "stddev: "
         << mapOfUpdatableMeanStandardDeviationMinMax[ label ].
                                                          getStandardDeviation()
         << std::endl;
      os << "minimum: "
         << mapOfUpdatableMeanStandardDeviationMinMax[ label ].getMinimum()
         << std::endl;
      os << "maximum: "
         << mapOfUpdatableMeanStandardDeviationMinMax[ label ].getMaximum()
         << std::endl;


      os.close();

      if ( localCCSubProcess.verbose )
      {

        std::cout << gkg::Eraser( 19 ) << std::flush;

      }

      ++ r;
      ++ roiIndex;

    }
    if ( localCCSubProcess.verbose )
    {

      std::cout << std::endl;

    }

  }
  GKG_CATCH( "template < class T1 > template < class T2 > "
             "void "
             "LocalCCSubProcess< T1 >::process( "
             "gkg::Process& process, "
             "const std::string& fileNameTextureMap1, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


////////////////////////////////////////////////////////////////////////////////
// class LocalCCProcess
////////////////////////////////////////////////////////////////////////////////


class LocalCCProcess : public gkg::Process
{

  public:

    LocalCCProcess( const std::string& theFileNameTextureMap2,
                    const std::string& theFileNameTextureMapRois,
                    const std::string& theFileNameMeshMap,
                    const std::string& theFileNameTextureMapCCOut,
                    const std::string& theFileNameTextureMapRoiOut,
                    const std::string& theFileNameStatisticsOut,
                    const int32_t& theRank1,
                    const int32_t& theRank2,
                    const int32_t& theRankRois,
                    const float& theDistance,
                    const bool& theAscii,
                    const std::string& theFormat,
                    const bool& theVerbose );

    
    const std::string& fileNameTextureMap2;
    const std::string& fileNameTextureMapRois;
    const std::string& fileNameMeshMap;
    const std::string& fileNameTextureMapCCOut;
    const std::string& fileNameTextureMapRoiOut;
    const std::string& fileNameStatisticsOut;
    const int32_t& rank1;
    const int32_t& rank2;
    const int32_t& rankRois;
    const float& distance;
    const bool& ascii;
    const std::string& format;
    const bool& verbose;


  private:

    template < class T1 >
    static void process( gkg::Process& process,
                         const std::string& fileNameTextureMap1,
                         const gkg::AnalyzedObject&,
                         const std::string& );

};


LocalCCProcess::LocalCCProcess( const std::string& theFileNameTextureMap2,
                                const std::string& theFileNameTextureMapRois,
                                const std::string& theFileNameMeshMap,
                                const std::string& theFileNameTextureMapCCOut,
                                const std::string& theFileNameTextureMapRoiOut,
                                const std::string& theFileNameStatisticsOut,
                                const int32_t& theRank1,
                                const int32_t& theRank2,
                                const int32_t& theRankRois,
                                const float& theDistance,
                                const bool& theAscii,
                                const std::string& theFormat,
                                const bool& theVerbose )
               : gkg::Process( "TextureMap" ),
                 fileNameTextureMap2( theFileNameTextureMap2 ),
                 fileNameTextureMapRois( theFileNameTextureMapRois ),
                 fileNameMeshMap( theFileNameMeshMap ),
                 fileNameTextureMapCCOut( theFileNameTextureMapCCOut ),
                 fileNameTextureMapRoiOut( theFileNameTextureMapRoiOut ),
                 fileNameStatisticsOut( theFileNameStatisticsOut ),
                 rank1( theRank1 ),
                 rank2( theRank2 ),
                 rankRois( theRankRois ),
                 distance( theDistance ),
                 ascii( theAscii ),
                 format( theFormat ),
                 verbose( theVerbose )
{

  registerProcess( "TextureMap", gkg::TypeOf< int8_t >::getName(),
                   &LocalCCProcess::process< int8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint8_t >::getName(),
                   &LocalCCProcess::process< uint8_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int16_t >::getName(),
                   &LocalCCProcess::process< int16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint16_t >::getName(),
                   &LocalCCProcess::process< uint16_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int32_t >::getName(),
                   &LocalCCProcess::process< int32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint32_t >::getName(),
                   &LocalCCProcess::process< uint32_t > );
  registerProcess( "TextureMap", gkg::TypeOf< int64_t >::getName(),
                   &LocalCCProcess::process< int64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< uint64_t >::getName(),
                   &LocalCCProcess::process< uint64_t > );
  registerProcess( "TextureMap", gkg::TypeOf< float >::getName(),
                   &LocalCCProcess::process< float > );
  registerProcess( "TextureMap", gkg::TypeOf< double >::getName(),
                   &LocalCCProcess::process< double > );

}


template < class T1 >
void LocalCCProcess::process( gkg::Process& process,
                              const std::string& fileNameTextureMap1,
                              const gkg::AnalyzedObject&,
                              const std::string& )
{

  try
  {

    LocalCCProcess&
      localCCProcess = static_cast< LocalCCProcess& >( process );

    LocalCCSubProcess< T1 >
      localCCSubProcess( localCCProcess.fileNameTextureMap2,
                         localCCProcess.fileNameTextureMapRois,
                         localCCProcess.fileNameMeshMap,
                         localCCProcess.fileNameTextureMapCCOut,
                         localCCProcess.fileNameTextureMapRoiOut,
                         localCCProcess.fileNameStatisticsOut,
                         localCCProcess.rank1,
                         localCCProcess.rank2,
                         localCCProcess.rankRois,
                         localCCProcess.distance,
                         localCCProcess.ascii,
                         localCCProcess.format,
                         localCCProcess.verbose );
    localCCSubProcess.execute( fileNameTextureMap1 );
                         

  }
  GKG_CATCH( "template < class T1 > "
             "void LocalCCProcess::process( "
             "gkg::Process& process, "
             "const std::string& fileNameTextureMap1, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}


//
//   TwoTexturesCorrelationCoefficientCommand
//


gkg::TwoTexturesCorrelationCoefficientCommand::
                                       TwoTexturesCorrelationCoefficientCommand(
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
  GKG_CATCH( "gkg::TwoTexturesCorrelationCoefficientCommand::"
             "TwoTexturesCorrelationCoefficientCommand( "
             "int32_t argc, char* argv[], bool loadPlugin, "
             "bool removeFirst )" );

}


gkg::TwoTexturesCorrelationCoefficientCommand::
                                       TwoTexturesCorrelationCoefficientCommand(
                                    const std::string& fileNameTextureMap1,
    				    const std::string& fileNameTextureMap2,
    				    const std::string& fileNameTextureMapRois,
    				    const std::string& fileNameMeshMap,
    				    const std::string& fileNameTextureMapCCOut,
    				    const std::string& fileNameTextureMapRoiOut,
    				    const std::string& fileNameStatisticsOut,
    				    int32_t rank1,
    				    int32_t rank2,
    				    int32_t rankRois,
    				    float distance,
    				    const std::string& format,
    				    bool ascii,
    				    bool verbose )
                                             : gkg::Command()
{

  try
  {

    execute( fileNameTextureMap1, fileNameTextureMap2, fileNameTextureMapRois,
             fileNameMeshMap, fileNameTextureMapCCOut, fileNameTextureMapRoiOut,
             fileNameStatisticsOut, rank1, rank2, rankRois, distance, format,
             ascii, verbose );

  }
  GKG_CATCH( "gkg::TwoTexturesCorrelationCoefficientCommand::"
             "TwoTexturesCorrelationCoefficientCommand( "
             "const std::string& fileNameTextureMap1, "
    	     "const std::string& fileNameTextureMap2, "
    	     "const std::string& fileNameTextureMapRois, "
    	     "const std::string& fileNameMeshMap, "
    	     "const std::string& fileNameTextureMapCCOut, "
    	     "const std::string& fileNameTextureMapRoiOut, "
    	     "const std::string& fileNameStatisticsOut, "
    	     "int32_t rank1, int32_t rank2, int32_t rankRois, float distance, "
    	     "const std::string& format, bool ascii, bool verbose )" );

}


gkg::TwoTexturesCorrelationCoefficientCommand::
                                       TwoTexturesCorrelationCoefficientCommand(
                                             const gkg::Dictionary& parameters )
                                             : gkg::Command( parameters )
{

  try
  {

    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTextureMap1 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTextureMap2 );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameTextureMapRois );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameMeshMap );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTextureMapCCOut );
    DECLARE_STRING_PARAMETER( parameters,
                              std::string,
                              fileNameTextureMapRoiOut );
    DECLARE_STRING_PARAMETER( parameters, std::string, fileNameStatisticsOut );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rank1 );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rank2 );
    DECLARE_INTEGER_PARAMETER( parameters, int32_t, rankRois );
    DECLARE_FLOATING_PARAMETER( parameters, float, distance );
    DECLARE_STRING_PARAMETER( parameters, std::string, format );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, ascii );
    DECLARE_BOOLEAN_PARAMETER( parameters, bool, verbose );
    
    execute( fileNameTextureMap1, fileNameTextureMap2, fileNameTextureMapRois,
             fileNameMeshMap, fileNameTextureMapCCOut, fileNameTextureMapRoiOut,
             fileNameStatisticsOut, rank1, rank2, rankRois, distance, format,
             ascii, verbose );

  }
  GKG_CATCH( "gkg::TwoTexturesCorrelationCoefficientCommand::"
             "TwoTexturesCorrelationCoefficientCommand( "
             "const gkg::Dictionary& parameters )" );

}


gkg::TwoTexturesCorrelationCoefficientCommand::
                                     ~TwoTexturesCorrelationCoefficientCommand()
{
}


std::string gkg::TwoTexturesCorrelationCoefficientCommand::getStaticName()
{

  try
  {

    return "TwoTexturesCorrelationCoefficient";

  }
  GKG_CATCH( "std::string gkg::TwoTexturesCorrelationCoefficientCommand::"
             "getStaticName()" );

}


void gkg::TwoTexturesCorrelationCoefficientCommand::parse()
{

  try
  {

    std::string fileNameTextureMap1;
    std::string fileNameTextureMap2;
    std::string fileNameTextureMapRois;
    std::string fileNameMeshMap;
    std::string fileNameTextureMapCCOut;
    std::string fileNameTextureMapRoiOut;
    std::string fileNameStatisticsOut;
    int32_t rank1 = 0;
    int32_t rank2 = 0;
    int32_t rankRois = 0;
    float distance = 0.0;
    std::string format = "";
    bool ascii = false;
    bool verbose = false;

    gkg::Application application( _argc, _argv,
                                  "Process the correlation coefficient"
                                  "between two textures within ROIs "
                                  "specified by a third int16_t texture",
                                  _loadPlugin );
    application.addSingleOption( "-t1",
                                 "First input texture map file name",
                                 fileNameTextureMap1 );
    application.addSingleOption( "-t2",
                                 "Second input texture map file name",
                                 fileNameTextureMap2 );
    application.addSingleOption( "-rois",
                                 "Input ROIs texture map file name",
                                 fileNameTextureMapRois );
    application.addSingleOption( "-m",
                                 "Input mesh map file name",
                                 fileNameMeshMap );
    application.addSingleOption( "-o",
                                 "Output texture map file name",
                                 fileNameTextureMapCCOut );
    application.addSingleOption( "-or",
                                 "Output ROI texture file name",
                                 fileNameTextureMapRoiOut );
    application.addSingleOption( "-os",
                                 "Output statistics file name",
                                 fileNameStatisticsOut );
    application.addSingleOption( "-rank1",
                                 "Rank in 1st input texture map",
                                 rank1 );
    application.addSingleOption( "-rank2",
                                 "Rank in 2nd input texture map",
                                 rank2 );
    application.addSingleOption( "-rankRois",
                                 "Rank in ROIs input texture map",
                                 rankRois );
    application.addSingleOption( "-d",
                                 "Geodesic distance for local CC computation",
                                 distance );
    application.addSingleOption( "-format",
                                 "Ouput volume format (default=first input)",
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

    execute( fileNameTextureMap1, fileNameTextureMap2, fileNameTextureMapRois,
             fileNameMeshMap, fileNameTextureMapCCOut, fileNameTextureMapRoiOut,
             fileNameStatisticsOut, rank1, rank2, rankRois, distance, format,
             ascii, verbose );

  }
  GKG_CATCH_FUNCTOR( "void gkg::TwoTexturesCorrelationCoefficientCommand::"
                     "parse()" );

}


void gkg::TwoTexturesCorrelationCoefficientCommand::execute(
                  	            const std::string& fileNameTextureMap1,
    				    const std::string& fileNameTextureMap2,
    				    const std::string& fileNameTextureMapRois,
    				    const std::string& fileNameMeshMap,
    				    const std::string& fileNameTextureMapCCOut,
    				    const std::string& fileNameTextureMapRoiOut,
    				    const std::string& fileNameStatisticsOut,
    				    int32_t rank1,
    				    int32_t rank2,
    				    int32_t rankRois,
    				    float distance,
    				    const std::string& format,
    				    bool ascii,
    				    bool verbose )
{

  try
  {

    LocalCCProcess localCCProcess( fileNameTextureMap2,
                                   fileNameTextureMapRois,
                                   fileNameMeshMap,
                                   fileNameTextureMapCCOut,
                                   fileNameTextureMapRoiOut,
                                   fileNameStatisticsOut,
                                   rank1,
                                   rank2,
                                   rankRois,
                                   distance,
                                   ascii,
                                   format,
                                   verbose );
    localCCProcess.execute( fileNameTextureMap1 );

  }
  GKG_CATCH( "void gkg::TwoTexturesCorrelationCoefficientCommand::execute( "
             "const std::string& fileNameTextureMap1, "
    	     "const std::string& fileNameTextureMap2, "
    	     "const std::string& fileNameTextureMapRois, "
    	     "const std::string& fileNameMeshMap, "
    	     "const std::string& fileNameTextureMapCCOut, "
    	     "const std::string& fileNameTextureMapRoiOut, "
    	     "const std::string& fileNameStatisticsOut, "
    	     "int32_t rank1, int32_t rank2, int32_t rankRois, float distance, "
    	     "const std::string& format, bool ascii, bool verbose )" );

}


RegisterCommandCreator(
                     TwoTexturesCorrelationCoefficientCommand,
    		     DECLARE_STRING_PARAMETER_HELP( fileNameTextureMap1 ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameTextureMap2 ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameTextureMapRois ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameMeshMap ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameTextureMapCCOut ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameTextureMapRoiOut ) +
    		     DECLARE_STRING_PARAMETER_HELP( fileNameStatisticsOut ) +
    		     DECLARE_INTEGER_PARAMETER_HELP( rank1 ) +
    		     DECLARE_INTEGER_PARAMETER_HELP( rank2 ) +
    		     DECLARE_INTEGER_PARAMETER_HELP( rankRois ) +
    		     DECLARE_FLOATING_PARAMETER_HELP( distance ) +
    		     DECLARE_STRING_PARAMETER_HELP( format ) +
    		     DECLARE_BOOLEAN_PARAMETER_HELP( ascii ) +
    		     DECLARE_BOOLEAN_PARAMETER_HELP( verbose ) );
