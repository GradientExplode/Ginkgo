#include <gkg-processing-numericalanalysis/NumAnalysisImplementationFactory.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-communication-getopt/Application.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-processing-algobase/Convolution3d_i.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/Vector.h>
#include <gkg-processing-numericalanalysis/Matrix.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>
#include <gkg-processing-process/Process.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <gkg-processing-coordinates/ElectrostaticOrientationSet.h>
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-container/SiteMap_i.h>
#include <gkg-processing-container/TextureMap_i.h>
#include <gkg-processing-container/CartesianField_i.h>
#include <gkg-core-io/ItemWriter_i.h>
#include <gkg-core-io/ItemReader_i.h>
#include <gkg-core-io/DefaultItemIOFactory_i.h>
#include <gkg-core-io/StdVectorItemIOFactory_i.h>
#include <gkg-processing-algobase/MedianFilter_i.h>
#include <gkg-dmri-container/OdfCartesianField.h>
#include <gkg-dmri-pdf-dti/Tensor.h>
#include <gkg-dmri-odf-dti/TensorOdf.h>
#include <gkg-dmri-pdf-dti/TensorCartesianField.h>
#include <gkg-dmri-container/DiffusionCartesianField_i.h>


#include <string>
#include <iostream>
#include <math.h>
#include <list>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <stdio.h>
#include <limits.h>
#include <map>



int main( int argc, char* argv[] )
{

  int result = EXIT_SUCCESS;

  try
  {

    std::string fileNameInputVolume;
    std::string fileNameInputBinaryMask;
    std::string fileNameTensorSiteMap;
    std::string fileNameTensorTextureMap;
    std::string fileNamePdfSiteMap;
    std::string fileNamePdfTextureMap;
    std::string fileNameTensorOdfSiteMap;
    std::string fileNameTensorOdfTextureMap;
    std::vector< int32_t > displacementMagnitudes( 3, 0 );
    std::vector< int32_t > outputOrientationCounts( 3, 0 );


    gkg::Application application( argc, argv,
                                  "Conversion from DtiTk to Tensor" );
    application.addSingleOption( "-i",
                                 "Input gis volume",
                                 fileNameInputVolume );
    application.addSingleOption( "-m",
                                 "Input binary mask",
                                 fileNameInputBinaryMask );
    application.addSingleOption( "-tosm",
                                 "Output tensor odf siteMap file name",
                                 fileNameTensorOdfSiteMap );
    application.addSingleOption( "-totm",
                                 "Output tensor odf textureMap file name",
                                 fileNameTensorOdfTextureMap );
    application.addSeriesOption( "-ooc",
                                 " Output orientation counts ",
                                 outputOrientationCounts );
    application.addSeriesOption( "-dm",
                                 "Displacement magnitudes",
                                 displacementMagnitudes );
    application.initialize();

////////////////////////////////////////////////////////////////////////////////

 // reading input volume

  gkg::Volume< float > inputVolume;
  gkg::TypedVolumeReaderProcess< float > readerInputVolume( inputVolume );
  readerInputVolume.execute( fileNameInputVolume );

//  gkg::Reader::getInstance().read( fileNameInputVolume, inputVolume );

 // reading input binary mask

  gkg::Volume< int32_t > inputBinaryMask;
  gkg::TypedVolumeReaderProcess< int32_t > readerInputBinaryMask( 
                                                             inputBinaryMask );
  readerInputBinaryMask.execute( fileNameInputBinaryMask );

////////////////////////////////////////////////////////////////////////////////

  
  // creating the list of sites from mask

  std::list< gkg::Vector3d< int32_t > > sitesList;

  int32_t i = 0;
  int32_t j = 0;
  int32_t k = 0;

  for ( k = 0; k < inputBinaryMask.getSizeZ(); ++k )
  {

    for ( j = 0; j < inputBinaryMask.getSizeY(); ++j )
    {

      for ( i = 0; i < inputBinaryMask.getSizeX(); ++i )
      {

        gkg::Vector3d< int32_t > site( i, j, k );
        if ( inputBinaryMask( site ) != 0 )
        {

          sitesList.push_back( site );


        }

      }

    }
      
  }

  // creating site map

  gkg::SiteMap< int32_t, int32_t > siteMap;
  siteMap.addSites( 0, sitesList );

  // creating the items using tensor constructor from saved values

  // i) creating outmputmultipleshellsampling
  std::map< float, gkg::OrientationSet > outputMultipleShellSampling;
  if( outputOrientationCounts.size() ==  displacementMagnitudes.size() )
  {

    std::vector< int32_t >::iterator
    o = outputOrientationCounts.begin(),
    oe = outputOrientationCounts.end();

    std::vector< int32_t >::iterator
    d = displacementMagnitudes.begin();


    while( o != oe )
    {

      outputMultipleShellSampling.insert( std::make_pair(
                       *d, 
                       gkg::OrientationSet( gkg::ElectrostaticOrientationSet(
                                  *o / 2 ).getSymmetricalOrientations() ) ) );
      ++o;  
      ++d;
   
    }      

  }
  else
  {

    throw std::runtime_error(" Not the same number of output orientation "
                             " counts and displacement magnitudes ");

  }

  // ii) creating the list of items
  float diffusionTime = 1.0f;
  

  std::list< gkg::OrientationDistributionFunction > tensorOdfsList;
  std::list< gkg::Vector3d< int32_t > >::const_iterator
      s = sitesList.begin(),
      se = sitesList.end(); 

  gkg::OrientationSet tensorOdfOrientationSet( 
                           gkg::ElectrostaticOrientationSet(
                                             outputOrientationCounts[ 0 ] / 2 
                                           ).getSymmetricalOrientations() );

  while ( s != se )
  {
    
    gkg::Vector coefficients( 6 );

    // reading the 6 tensor component from volume
    coefficients( 0 ) = inputVolume( *s, 0 );
    coefficients( 1 ) = inputVolume( *s, 1 );
    coefficients( 2 ) = inputVolume( *s, 2 );
    coefficients( 3 ) = inputVolume( *s, 3 );
    coefficients( 4 ) = inputVolume( *s, 4 );
    coefficients( 5 ) = inputVolume( *s, 5 );


    gkg::Tensor tensor( &outputMultipleShellSampling,
                        coefficients,
                        diffusionTime );

    gkg::TensorOdf tensorOdf( &tensorOdfOrientationSet,
                              tensor );

    tensorOdfsList.push_back( tensorOdf );

    ++ s;

  }

  gkg::TextureMap< gkg::OrientationDistributionFunction > tensorOdfs;
  tensorOdfs.addTextures( tensorOdfsList );


  tensorOdfs.getHeader().addAttribute( "odf_type",
                                       std::string( "gkg_TensorOdf" ) );


  gkg::OdfCartesianField tensorOdfField( 160, 160, 80,
                                gkg::Vector3d< double >( 1.5, 1.5, 1.5 ), 
                                siteMap, 
                                tensorOdfs, 
                                0, 0, 0 ,
                                tensorOdfOrientationSet );
 
  std::cout << "\nWriting tensor odf field siteMap to file ... " << std::endl;

  gkg::Writer::getInstance().write( fileNameTensorOdfSiteMap,
                                    tensorOdfField.getSiteMap(),
                                    false,
                                    "sitemap" );


  std::cout << "\nDone " << std::endl;

  std::cout << "\nWriting tensor odf field textureMap to file ... " << std::endl;

  gkg::Writer::getInstance().write( fileNameTensorOdfTextureMap,
                                    tensorOdfField.getItems(),
                                    false,
                                    "texturemap" );

  std::cout << "\nDone " << std::endl;

  }
  GKG_CATCH_COMMAND( result );

  return result;

}











