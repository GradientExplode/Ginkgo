#include <gkg-communication-getopt/Application.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-core-io/StringConverter.h>
#include <gkg-dmri-io/ForceHardLinkWithGinkgoLibrary.h>
#include <gkg-processing-colorimetry/RandomColorGenerator.h>
#include <complex>
#include <iostream>
#include <fstream>
#include <string>


FORCE_HARD_LINK_WITH_CONNECTOMIST

int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameInputClusterBundleMap;
    std::string fileNameInputCentroidBundleMap;
    std::string fileNameOutputClusterBundleMap;
    std::string fileNameOutputCentroidBundleMap;
    std::string fileNameOutputHierarchy;
    float lowerLength = 0.0f;
    gkg::Application application( argc, argv, "DWI bundle measure tool" );
    application.addSingleOption( "-iCluster",
                                 "Input cluster bundle map file name",
                                 fileNameInputClusterBundleMap );
    application.addSingleOption( "-iCentroid",
                                 "Input centroid bundle map file name",
                                 fileNameInputCentroidBundleMap );
    application.addSingleOption( "-oCluster",
                                 "Output cluster bundle map file name",
                                 fileNameOutputClusterBundleMap );
    application.addSingleOption( "-oCentroid",
                                 "Output centroid bundle map file name",
                                 fileNameOutputCentroidBundleMap );
    application.addSingleOption( "-oHierarchy",
                                 "Output hierarchy file name",
                                 fileNameOutputHierarchy );
    application.addSingleOption( "-l",
                                 "Lower length (default=0.0)",
                                 lowerLength,
                                 true );

    application.initialize();

    gkg::BundleMap< std::string > inputClusterBundleMap;
    gkg::BundleMap< std::string > inputCentroidBundleMap;
    gkg::BundleMap< std::string > outputClusterBundleMap;
    gkg::BundleMap< std::string > outputCentroidBundleMap;

    gkg::Reader::getInstance().read( fileNameInputClusterBundleMap,
                                     inputClusterBundleMap );
    gkg::Reader::getInstance().read( fileNameInputCentroidBundleMap,
                                     inputCentroidBundleMap );


    std::ofstream os( fileNameOutputHierarchy );

    gkg::RandomColorGenerator randomColorGenerator( 0.7, 0.95 );

    os << "# tree 1.0" << std::endl;
    os << std::endl;
    os << "*BEGIN TREE hierarchy" << std::endl;
    os << "graph_syntax RoiArg" << std::endl;
    os << std::endl;
    os << "*BEGIN TREE fold_name" << std::endl;
    os << "name all" << std::endl;
    os << std::endl;




    int32_t index = 1;
    gkg::RGBComponent rgbComponent;
    gkg::BundleMap< std::string >::const_iterator
      iclb = inputClusterBundleMap.begin(),
      icle = inputClusterBundleMap.end();
    gkg::BundleMap< std::string >::const_iterator
      iceb = inputCentroidBundleMap.begin();
    while ( iclb != icle )
    {

      int32_t count = 1;
      while ( iclb->first[ 17 + count ] != 'm' )
      {

        ++ count;

      }
      std::string strLowerLength = iclb->first.substr( 17, count );

      std::cout << iclb->first << " : " << std::endl
                << index << " -> lower_length=" 
                << strLowerLength << std::endl;

      std::string newLabel = gkg::StringConverter::toString( index );

      float currentLowerLength = 0.0f;
      gkg::StringConverter::toScalar( strLowerLength, currentLowerLength );

      if ( currentLowerLength >= lowerLength )
      {

        outputClusterBundleMap.addBundle( iclb->first,
                                          iclb->second );
        outputCentroidBundleMap.addBundle( iclb->first,
                                           iceb->second );

        rgbComponent = randomColorGenerator.getRandomColor();

        os << "*BEGIN TREE fold_name" << std::endl;
        os << "name " << iclb->first << std::endl;
        os << "color "
           << ( int32_t )rgbComponent.r << " "
           << ( int32_t )rgbComponent.g << " "
           << ( int32_t )rgbComponent.b << std::endl;
        os << "*END" << std::endl;
        os << std::endl;

      }

      ++ iclb;
      ++ iceb;
      ++ index;

    }

    os << std::endl;
    os << "*END" << std::endl;
    os << std::endl;
    os << "*END" << std::endl;
    os << std::endl;

    os.close();

    gkg::Writer::getInstance().write( fileNameOutputClusterBundleMap,
                                      outputClusterBundleMap );
    gkg::Writer::getInstance().write( fileNameOutputCentroidBundleMap,
                                      outputCentroidBundleMap );


  }
  GKG_CATCH_COMMAND( result );

  return result;

}
